# SunOS native file-event notifications

This backend implements ca2's `file::watcher` and `file::watch` interfaces
using illumos/Solaris event ports (`PORT_SOURCE_FILE`). It is registered by
`apex_haiku`; no inotify emulation or external library is required.

Entries, watches and watcher services inherit ca2 particles and use ca2
pointers, arrays, maps and tasks. Native `port.h` structures stay in
`native.cpp`, compiled without ca2 headers or a precompiled header.

Each watch owns an event port and a cancellable task with a 100 ms wait.
Files and directories are associated using their observed timestamps. The
one-shot associations are re-armed after each batch. Directory reconciliation
detects additions, deletions, renames (delete/add) and editor atomic replacements
(modify). Recursive scans do not follow directory symlinks. A parent-directory
association permits watching a root that is later deleted and recreated.

Listeners are invoked outside registry/listener locks. Watch removal stops
its task; native resources remain alive until that task leaves its bounded
wait. Todo now uses this watcher and unregisters it on application finish.

Events are coalesced; this is a change-notification interface, not an audit log
of every intermediate filesystem operation. A watched tree is rescanned after
a native event, so scan cost grows with tree size. Unsupported filesystems,
permission failures and event-port resource limits are reported as errors.
On NFS, native notifications cover local changes, not arbitrary remote-client
changes. Linux's inotify registration and the existing optional kqueue code
are unchanged.

## Native smoke test

From the OpenIndiana build directory:

```sh
cmake --build . --target haiku_fen_native_test
./output/haiku_fen_native_test
```

The test uses the production native adapter to verify modification,
one-shot re-arming and deletion. Rebuild `_app_core_todo` and test saved edits,
save-by-rename and directory changes for the full ca2 integration. Native
compilation and execution have not been verified on the Windows mirror host.

Primary API documentation:
https://raw.githubusercontent.com/illumos/illumos-gate/master/usr/src/man/man3c/port_associate.3c
