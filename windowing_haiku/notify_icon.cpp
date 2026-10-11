#include "platform.h"
#include "notify_icon.h"
#include "native_notify_icon.h"
#include "aura/windowing/icon.h"
#include "acme/platform/application.h"
#include "acme/platform/application_menu.h"
#include <unistd.h>

namespace windowing_haiku
{
   notify_icon::~notify_icon() { destroy_notify_icon(); }

   static void copy_menu(void * native, ::application_menu * menu, int parent, int & next)
   {
      for (auto & item : *menu)
      {
         int id = ++next;
         auto label = item->is_separator() ? ::string() : item->m_strName;
         auto command = item->is_popup() ? ::string() : item->m_atomMenu.as_string();
         haiku_notify_menu(native, parent, id, label.c_str(), command.c_str(), true, false);
         if (item->is_popup()) copy_menu(native, item, id, next);
      }
   }

   void notify_icon::create_notify_icon(const ::atom & atom, ::user::interaction * interaction, ::windowing::icon * icon)
   {
      if (m_bCreated) return;
      m_strId = application()->m_strAppId + ":" + ::as_string(getpid()) + ":" + atom.as_string();
      m_puserinteractionNotify = interaction;
      m_piconCurrent = icon;
      m_native = haiku_notify_create(m_strId, icon ? icon->get_os_data({16,16}) : nullptr, native_command, this);
      interaction->on_update_notify_icon_menu(menu());
      int next = 0;
      copy_menu(m_native, menu(), 0, next);
      auto status = haiku_notify_install(m_native);
      if (status != 0)
      {
         destroy_notify_icon();
         throw ::exception(error_failed, "Deskbar notification icon creation failed: " + ::as_string(status));
      }
      m_bCreated = true;
      information() << "Haiku Deskbar notification icon created: " << m_strId;
   }

   void notify_icon::native_command(void * context, const char * command)
   {
      auto icon = static_cast<notify_icon *>(context);
      ::pointer<notify_icon> self = icon;
      ::string id = command;
      auto interaction = icon->m_puserinteractionNotify;
      if (interaction) interaction->user_post([self, id]()
      {
         if (self->m_bCreated) self->handle_command(id, nullptr);
      });
   }

   void notify_icon::destroy_notify_icon()
   {
      m_bCreated = false;
      auto native = m_native;
      m_native = nullptr;
      if (native) haiku_notify_destroy(native);
   }

   void notify_icon::modify_icon(::windowing::icon * icon)
   {
      m_piconCurrent = icon;
      if (m_native && icon) haiku_notify_bitmap(m_native, icon->get_os_data({16,16}));
   }

   bool notify_icon::notify_icon_step()
   {
      if (!m_native) return false;
      if (haiku_notify_exists(m_native)) return true;
      return haiku_notify_install(m_native) == 0;
   }
}
