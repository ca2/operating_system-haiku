#include "native.h"
#include <port.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern "C" int apex_haiku_fen_port()
{
   int port = port_create();
   if (port >= 0 && fcntl(port, F_SETFD, FD_CLOEXEC) == -1)
   {
      int error = errno;
      close(port); errno = error; return -1;
   }
   return port;
}
extern "C" void apex_haiku_fen_close(int port) { if (port >= 0) close(port); }
extern "C" int apex_haiku_fen_stat(const char *path, apex_haiku_fen_status *out)
{
   struct stat status;
   out->exists = 0;
   if (lstat(path, &status) == -1) return errno;
   out->exists = 1;
   out->device = status.st_dev; out->inode = status.st_ino; out->size = status.st_size;
   out->atime_sec = status.st_atim.tv_sec; out->atime_nsec = status.st_atim.tv_nsec;
   out->mtime_sec = status.st_mtim.tv_sec; out->mtime_nsec = status.st_mtim.tv_nsec;
   out->ctime_sec = status.st_ctim.tv_sec; out->ctime_nsec = status.st_ctim.tv_nsec;
   out->directory = S_ISDIR(status.st_mode); out->symlink = S_ISLNK(status.st_mode);
   return 0;
}
extern "C" void *apex_haiku_fen_entry(const char *path)
{
   file_obj_t *file = static_cast<file_obj_t *>(calloc(1, sizeof(file_obj_t)));
   if (!file) return nullptr;
   file->fo_name = strdup(path);
   if (!file->fo_name) { free(file); return nullptr; }
   return file;
}
extern "C" int apex_haiku_fen_arm(int port, void *entry, const apex_haiku_fen_status *status)
{
   file_obj_t *file = static_cast<file_obj_t *>(entry);
   file->fo_atime.tv_sec = status->atime_sec; file->fo_atime.tv_nsec = status->atime_nsec;
   file->fo_mtime.tv_sec = status->mtime_sec; file->fo_mtime.tv_nsec = status->mtime_nsec;
   file->fo_ctime.tv_sec = status->ctime_sec; file->fo_ctime.tv_nsec = status->ctime_nsec;
   int events = FILE_MODIFIED | FILE_ATTRIB | FILE_TRUNC;
   if (status->symlink) events |= FILE_NOFOLLOW;
   return port_associate(port, PORT_SOURCE_FILE, reinterpret_cast<uintptr_t>(file), events, nullptr) == -1 ? errno : 0;
}
extern "C" void apex_haiku_fen_remove(int port, void *entry)
{
   if (!entry) return;
   file_obj_t *file = static_cast<file_obj_t *>(entry);
   port_dissociate(port, PORT_SOURCE_FILE, reinterpret_cast<uintptr_t>(file));
   free(file->fo_name); free(file);
}
extern "C" int apex_haiku_fen_next(int port, int timeout_ms)
{
   port_event_t event;
   struct timespec timeout = {timeout_ms / 1000, (timeout_ms % 1000) * 1000000L};
   if (port_get(port, &event, &timeout) == -1)
      return errno == ETIME || errno == EINTR ? 0 : -errno;
   // Do not dereference portev_object: an already-queued event can refer to
   // an association removed by the directory reconciliation pass.
   return 1;
}
