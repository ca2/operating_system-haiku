// Created by camilo on 2023-04-15 19:30 <3ThomasBorregaardSorensen!!
#pragma once
#include <Messenger.h>
#include <OS.h>


#include "acme/accessibility/main_window.h"


namespace accessibility_haiku
{


   class CLASS_DECL_ACCESSIBILITY_HAIKU main_window :
      virtual public ::accessibility::main_window
   {
   public:


      // HWND m_hwnd; // Windows window handle.
      BMessenger m_window;
      team_id m_team = -1;


      main_window();
      ~main_window() override;


      void from_application(::accessibility::application * papplication) override;

      bool is_window() override;

      void post_close() override;

      void post_application_exit() override;

      ::string get_window_text(const ::iptr_array & idPath) override;
      ::string indexed_get_window_text(const ::iptr_array & idPath) override;
      void activate() override;


   };


} //    namespace accessibility_haiku





