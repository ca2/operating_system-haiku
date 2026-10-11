#pragma once
#include "aura/user/user/notify_icon.h"
namespace windowing_haiku
{
   class notify_icon : virtual public ::user::notify_icon
   {
   public:
      void * m_native = nullptr;
      ~notify_icon() override;
      void create_notify_icon(const ::atom &, ::user::interaction *, ::windowing::icon *) override;
      void destroy_notify_icon() override;
      void modify_icon(::windowing::icon *) override;
      bool notify_icon_step() override;
      static void native_command(void *, const char *);
   };
}
