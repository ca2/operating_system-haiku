// Native integration test: exercise the same adapter as the ca2 backend.
#include "native.h"
#include <Application.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
   BApplication application("application/x-vnd.ca2-node-monitor-test");
   char path[] = "/tmp/ca2-fen-test-XXXXXX";
   int file = mkstemp(path);
   int port = apex_haiku_fen_port();
   void *entry = file >= 0 ? apex_haiku_fen_entry(path) : nullptr;
   bool ok = file >= 0 && port >= 0 && entry;
   apex_haiku_fen_status status{};
   for (int i = 0; ok && i < 2; ++i)
   {
      ok = apex_haiku_fen_stat(path, &status) == 0
         && apex_haiku_fen_arm(port, entry, &status) == 0;
      if (ok) { usleep(20000); ok = write(file, "test\n", 5) == 5 && fsync(file) == 0; }
      // BFS emits the final write-stat notification when the file is closed.
      if (ok)
      {
         close(file);
         file = -1;
         ok = apex_haiku_fen_next(port, 2000) == 1;
         if (ok) { file = open(path, O_WRONLY | O_APPEND); ok = file >= 0; }
      }
   }
   if (ok)
   {
      ok = apex_haiku_fen_stat(path, &status) == 0
         && apex_haiku_fen_arm(port, entry, &status) == 0;
      if (ok) ok = unlink(path) == 0 && apex_haiku_fen_next(port, 2000) == 1;
   }
   apex_haiku_fen_remove(port, entry);
   apex_haiku_fen_close(port);
   if (file >= 0) { close(file); unlink(path); }
   printf("Haiku node-monitor modification/re-arm/delete test: %s\n", ok ? "passed" : "FAILED");
   return ok ? 0 : 1;
}
