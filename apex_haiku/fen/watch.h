#pragma once
#include "acme/filesystem/watcher/watch.h"
#include "acme/filesystem/watcher/watcher.h"
#include "acme/prototype/collection/string_map.h"
#include "native.h"

namespace apex_haiku::fen
{
   class entry : virtual public ::particle
   {
   public:
      int m_port = -1;
      void *m_native = nullptr;
      ::file::path m_path;
      apex_haiku_fen_status m_status{};
      ~entry() override { apex_haiku_fen_remove(m_port, m_native); }
   };

   class watch : virtual public ::file::watch
   {
   public:
      int m_port = -1;
      ::string_map_base<::pointer<entry>> m_entries;
      ::pointer<entry> m_parent;
      ::task_pointer m_monitor;
      watch();
      ~watch() override;
      bool open(const ::file::path &folder, bool recursive) override;
      void add_listener(const ::file::listener &listener) override;
      void erase_listener(const ::file::listener &listener) override;
      void handle_action(::file::action *action) override;
      void destroy() override;
      bool file_watch_step() override;
      void reconcile(bool initial);
   };

   class watcher : virtual public ::file::watcher
   {
   public:
      watcher() { m_bCreateWatchThread = false; defer_create_synchronization(); }
      bool m_closing = false;
      using ::file::watcher::erase_watch;
      ::file::watch *add_watch_listener(const ::file::path &folder,
         const ::file::listener &listener, bool recursive) override;
      void erase_watch(::file::watch *watch, ::function<void()> erased = nullptr) override;
      void destroy() override;
   };
}
