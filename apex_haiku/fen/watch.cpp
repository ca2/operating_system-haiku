#include "platform.h"
#include "watch.h"
#include "acme/filesystem/watcher/action.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/listing.h"
#include "acme/parallelization/synchronous_lock.h"
#include <errno.h>

namespace apex_haiku::fen
{
   watch::watch() { m_bStop = false; defer_create_synchronization(); }
   watch::~watch()
   {
      m_entries.erase_all();
      m_parent.release();
      apex_haiku_fen_close(m_port);
   }
   bool watch::open(const ::file::path &folder, bool recursive)
   {
      if (!directory()->is(folder)) return false;
      ::file::watch::open(folder, recursive);
      m_port = apex_haiku_fen_port();
      if (m_port < 0) return false;
      auto parent = folder.folder();
      if (parent != folder && directory()->is(parent))
      {
         m_parent = create_newø<entry>();
         m_parent->m_path = parent; m_parent->m_port = m_port;
         m_parent->m_native = apex_haiku_fen_entry(parent.c_str());
         if (!m_parent->m_native) throw ::exception(error_no_memory);
      }
      reconcile(true);
      return true;
   }
   void watch::add_listener(const ::file::listener &listener)
   {
      synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      ::file::watch::add_listener(listener);
      if (m_monitor) return;
      auto hold = as_pointer(this);
      m_monitor = fork([this, hold]()
      {
         while (::task_get_run())
         {
            try { if (!file_watch_step()) break; }
            catch (const ::exception &e)
            { warning() << "Haiku file-event watcher stopped: " << e.get_message(); break; }
         }
      });
   }
   void watch::erase_listener(const ::file::listener &listener)
   {
      bool empty;
      {
         synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
         m_listenera.erase(listener);
         empty = m_listenera.is_empty();
      }
      if (empty && m_pwatcher) m_pwatcher->erase_watch(this);
   }
   void watch::handle_action(::file::action *action)
   {
      comparable_eq_array<::file::listener> listeners;
      {
         synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
         listeners = m_listenera;
      }
      for (auto &listener : listeners)
      {
         if (!::task_get_run()) break;
         listener(action);
      }
   }
   void watch::destroy()
   {
      synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      if (m_monitor) m_monitor->set_finish();
      m_monitor.release();
      for (auto &listener : m_listenera) listener.m_watcha.erase(this);
      m_listenera.erase_all();
      // Native objects remain alive until the worker finishes its bounded wait.
   }
   bool watch::file_watch_step()
   {
      int event = apex_haiku_fen_next(m_port, 100);
      if (!::task_get_run()) return false;
      if (event < 0) throw ::exception(error_failed, "Haiku event port error " + ::as_string(-event));
      if (!event) return true;
      for (int i = 0; i < 255 && apex_haiku_fen_next(m_port, 0) > 0; ++i) {}
      reconcile(false);
      return true;
   }
   void watch::reconcile(bool initial)
   {
      ::file::path_array_base paths;
      ::array<apex_haiku_fen_status> observed;
      apex_haiku_fen_status parentStatus{};
      if (m_parent) apex_haiku_fen_stat(m_parent->m_path.c_str(), &parentStatus);
      paths.add(m_pathFolder);
      // Walk one directory level at a time; don't recurse through symlinks.
      for (::collection::index i = 0; i < paths.get_count(); ++i)
      {
         auto &status = observed.add_new();
         status = {};
         if (apex_haiku_fen_stat(paths[i].c_str(), &status) != 0) continue;
         if (status.directory && !status.symlink && (i == 0 || m_bRecursive))
         {
            ::file::listing_base children;
            children.set_listing(paths[i]);
            directory()->enumerate(children);
            paths.append(children);
         }
      }
      ::string_map_base<::pointer<entry>> current;
      ::array<::file::action> actions;
      auto emit = [&](const ::file::path &path, ::file::enum_action kind)
      {
         if (initial || path == m_pathFolder) return;
         auto &action = actions.add_new();
         action.m_pfilewatch = this;
         action.m_pathFolder = path.folder();
         action.m_pathFile = path.name();
         action.m_eaction = kind;
      };
      for (::collection::index i = 0; i < paths.get_count(); ++i)
      {
         auto &path = paths[i];
         auto &status = observed[i];
         if (!status.exists) continue;
         auto previous = m_entries.find(path);
         ::pointer<entry> item = previous ? previous->element2() : nullptr;
         if (!item)
         {
            item = create_newø<entry>();
            item->m_port = m_port; item->m_path = path;
            item->m_native = apex_haiku_fen_entry(path.c_str());
            if (!item->m_native) throw ::exception(error_no_memory);
            emit(path, ::file::e_action_add);
         }
         else
         {
            auto &old = item->m_status;
            if (!status.directory && (old.device != status.device || old.inode != status.inode
               || old.size != status.size || old.mtime_sec != status.mtime_sec
               || old.mtime_nsec != status.mtime_nsec || old.ctime_sec != status.ctime_sec
               || old.ctime_nsec != status.ctime_nsec)) emit(path, ::file::e_action_modify);
         }
         item->m_status = status;
         int error = apex_haiku_fen_arm(m_port, item->m_native, &status);
         if (error == ENOENT) continue; // Concurrent delete/rename: parent event retries.
         if (error) throw ::exception(error_failed, "Cannot associate Haiku file event: "
            + path + " (errno " + ::as_string(error) + ")");
         current[path] = item;
      }
      for (auto &previous : m_entries)
         if (!current.find(previous.element1())) emit(previous.element2()->m_path, ::file::e_action_delete);
      m_entries = ::transfer(current);
      if (m_parent && parentStatus.exists)
      {
         int error = apex_haiku_fen_arm(m_port, m_parent->m_native, &parentStatus);
         if (error && error != ENOENT)
            throw ::exception(error_failed, "Cannot watch parent directory, errno=" + ::as_string(error));
      }
      for (auto &action : actions) handle_action(&action);
   }
   ::file::watch *watcher::add_watch_listener(const ::file::path &folder,
      const ::file::listener &listener, bool recursive)
   {
      auto item = create_newø<::apex_haiku::fen::watch>();
      item->m_pwatcher = this;
      // Associate before inserting into the registry, so a failed open does
      // not leave a half-initialized watch or native port behind.
      if (!item->open(folder, recursive)) return nullptr;
      synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      if (m_closing) return nullptr;
      ::pointer<::file::watch> baseWatch = item;
      m_watchset.set_item(baseWatch);
      try { item->add_listener(listener); }
      catch (...)
      {
         m_watchset.erase(baseWatch);
         item->destroy();
         throw;
      }
      return item;
   }
   void watcher::destroy()
   {
      ::pointer_array<::file::watch> watches;
      {
         synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
         m_closing = true;
         for (auto &item : m_watchset) watches.add(item.item());
         m_watchset.erase_all();
      }
      for (auto &item : watches) item->destroy();
      ::task::destroy();
   }
   void watcher::erase_watch(::file::watch *watch, ::function<void()> erased)
   {
      if (!watch) return;
      ::pointer<::file::watch> hold = watch;
      {
         synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
         watch->m_functionDestroy = erased;
         m_watchset.erase(watch);
      }
      watch->destroy();
   }
}
