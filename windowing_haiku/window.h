// Created by camilo on 2026-10-08 02:18 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "aura/windowing/window.h"
#include "acme_windowing_haiku/window.h"


namespace windowing_haiku
{
   class CLASS_DECL_WINDOWING_HAIKU window :
      virtual public ::windowing::window,
      virtual public ::haiku::acme::windowing::window
   {
   public:

      window();

      ~window() override;

      void _create_window() override;
      bool client_to_screen(::i32_point *) override;
      bool screen_to_client(::i32_point *) override;
      void native_event(const haiku_window_event &) override;
      void destroy_window() override;
      void main_send(const ::procedure &) override;
      void main_post(const ::procedure &) override;
      void set_window_text(const ::scoped_string &) override;
      ::i32_rectangle get_window_rectangle() override;
      void set_position(const ::i32_point &) override;
      void set_size(const ::i32_size &) override;
      void set_active_window() override;
      using ::windowing::window::set_keyboard_focus;
      void set_keyboard_focus() override;
      void _set_keyboard_focus_unlocked() override;
      bool has_keyboard_focus() override;
      ::operating_system::window operating_system_window() const override;
      bool is_active_window() override { return haiku_window_is_active(m_native) != 0; }
      bool is_window() override { return m_native != nullptr; }
      bool is_window_visible() override { return m_nativeVisible; }
      bool _is_window_visible_unlocked() override { return m_nativeVisible; }
      void window_update_screen() override;
      void draw_frame() override;
      //void present_buffer_item(::graphics::buffer_item *);
      bool _strict_set_window_position_unlocked(::i32, ::i32, ::i32, ::i32, bool, bool) override;

   };


} // namespace windowing_haiku




