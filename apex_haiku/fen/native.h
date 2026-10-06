#pragma once
#include <stdint.h>

// Native SDK structures stay in a translation unit without ca2 headers.
struct apex_haiku_fen_status
{
   uint64_t device, inode, size;
   int64_t atime_sec, atime_nsec, mtime_sec, mtime_nsec, ctime_sec, ctime_nsec;
   int directory, symlink, exists;
};
extern "C"
{
   int apex_haiku_fen_port();
   void apex_haiku_fen_close(int port);
   int apex_haiku_fen_stat(const char *path, apex_haiku_fen_status *status);
   void *apex_haiku_fen_entry(const char *path);
   int apex_haiku_fen_arm(int port, void *entry, const apex_haiku_fen_status *status);
   void apex_haiku_fen_remove(int port, void *entry);
   int apex_haiku_fen_next(int port, int timeout_ms);
}
