// Created by camilo on 2026-10-08 02:20 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "window.h"
#include "graphics.h"
#include "aura/user/user/interaction_thread.h"
#include "acme/windowing/display.h"
#include "acme_windowing_haiku/windowing.h"
#include "aura/user/user/interaction.h"
#include "aura/graphics/graphics/buffer_item.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/image_pixmap_lease.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/system.h"

namespace windowing_haiku
{


   window::window()
   {


   }



   window::~window()
   {


   }

   void window::_create_window()
   {
      auto *ui = user_interaction();
      if (ui)
         m_rectangle = {ui->const_layout().sketch().origin(), ui->const_layout().sketch().size()};
      if (m_rectangle.is_empty())
         m_rectangle = {100, 100, 740, 580};
      m_pointWindow = m_rectangle.origin();
      m_sizeWindow = m_rectangle.size();
      m_sizeRaw = m_sizeWindow;
      ::haiku::acme::windowing::window::_create_window();
      if (ui)
         ui->send_message(::user::e_message_create, 0, 0);
      create_graphics_thread();
   }

   bool window::client_to_screen(::i32_point *p)
   {
      auto r = get_window_rectangle();
      *p += r.origin();
      return true;
   }

   bool window::screen_to_client(::i32_point *p)
   {
      auto r = get_window_rectangle();
      *p -= r.origin();
      return true;
   }

   void window::native_event(const haiku_window_event &e)
   {
      if (e.kind == 7)
      {
         if (auto *ui = user_interaction())
         {
            ::pointer<window> self = this;
            ui->post([self,e]()
            {
               self->on_window_activate(e.x, false, self->operating_system_window());
               if (auto *interaction = self->user_interaction())
               {
                  interaction->set_need_redraw();
                  interaction->post_redraw();
               }
            });
         }
         return;
      }
      if (e.kind >= 4 && e.kind <= 6)
      {
         m_pointCursor2 = {e.x, e.y};
         if (m_pacmewindowingdisplayWindow)
            m_pacmewindowingdisplayWindow->m_pointCursor2 = {e.width, e.height};
         if (auto *ui = user_interaction())
            ui->post_message(
               e.kind == 4
                  ? ::user::e_message_left_button_down
                  : e.kind == 5
                  ? ::user::e_message_left_button_up
                  : ::user::e_message_mouse_move, 0, ::lparam(e.x, e.y));
         return;
      }
      if (e.kind == 1)
      {
         m_sizeWindow = {e.width, e.height};
         m_sizeRaw = m_sizeWindow;
      }
      else
         if (e.kind == 2)
            m_pointWindow = {e.x, e.y};
      ::haiku::acme::windowing::window::native_event(e);
   }

   void window::destroy_window()
   {
      ::haiku::acme::windowing::window::destroy_window();
      ::windowing::window::on_destroy();
   }

   void window::main_send(const ::procedure &p)
   {
      if (m_puserthread)
         m_puserthread->send(p);
      else
         system()->acme_windowing()->main_send(p);
   }

   void window::main_post(const ::procedure &p)
   {
      if (m_puserthread)
         m_puserthread->post(p);
      else
         system()->acme_windowing()->main_post(p);
   }

   void window::set_window_text(const ::scoped_string &p) { ::haiku::acme::windowing::window::set_window_text(p); }
   ::i32_rectangle window::get_window_rectangle() { return ::haiku::acme::windowing::window::get_window_rectangle(); }
   void window::set_position(const ::i32_point &p) { ::haiku::acme::windowing::window::set_position(p); }
   void window::set_size(const ::i32_size &p) { ::haiku::acme::windowing::window::set_size(p); }
   void window::set_active_window() { ::haiku::acme::windowing::window::set_active_window(); }

   ::operating_system::window window::operating_system_window() const
   {

      return ::haiku::acme::windowing::window::operating_system_window();

   }


   bool window::_strict_set_window_position_unlocked(::i32 x, ::i32 y, ::i32 w, ::i32 h, bool noMove, bool noSize)
   {

      auto r = get_window_rectangle();
      haiku_window_frame(m_native, noMove ? r.left : x, noMove ? r.top : y, noSize ? r.width() : w,
                         noSize ? r.height() : h);
      return true;

   }


   void window::draw_frame()
   {
      if (auto *ui = user_interaction())
      {
         auto edisplay = ui->const_layout().sketch().display();
         bool visible = ::is_screen_visible(edisplay);
         if (visible != m_nativeVisible)
         {
            haiku_window_show(m_native, visible ? 1 : 0);
            m_nativeVisible = visible;
         }
         ui->set_display(edisplay, ::user::e_layout_window);
         if (!visible)
            return;
      }
      ::windowing::window::draw_frame();
   }

   void window::window_update_screen()
   {
      if (!m_pgraphicsgraphics)
         return;
      synchronous_lock graphicsLock(m_pgraphicsgraphics->synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      m_pgraphicsgraphics->on_update_screen(m_pgraphicsgraphics->get_screen_item());
      //present_buffer_item();
   }

   // void window::present_buffer_item(::graphics::buffer_item *item)
   // {
   //    if (!m_native || !item || !item->m_pimageBufferItem)
   //       return;
   //    synchronous_lock imageLock(item->m_pmutex, DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
   //    auto pixels = item->m_pimageBufferItem->map();
   //    if (!pixels || !pixels->m_pimage32)
   //       return;
   //    auto point = item->m_pointBufferItem;
   //    auto size = item->m_sizeBufferItem;
   //    int x = 0, y = 0, w = 0, h = 0;
   //    haiku_window_bounds(m_native, &x, &y, &w, &h);
   //    if (x != point.x || y != point.y || w != size.cx || h != size.cy)
   //       haiku_window_frame(m_native, point.x, point.y, size.cx, size.cy);
   //    auto source = pixels->m_point; // Present the window region of the fullscreen bitmap.
   //    source.x = constrained(source.x, 0, pixels->m_sizeRaw.cx);
   //    source.y = constrained(source.y, 0, pixels->m_sizeRaw.cy);
   //    int width = minimum(size.cx, pixels->m_sizeRaw.cx - source.x), height = minimum(
   //           size.cy, pixels->m_sizeRaw.cy - source.y);
   //    if (width <= 0 || height <= 0)
   //       return;
   //    auto *data = reinterpret_cast<const unsigned char *>(pixels->m_pimage32Raw) + source.y * pixels->m_iScan + source.
   //                 x * 4;
   //    haiku_window_present(m_native, data, width, height, pixels->m_iScan);
   // }
}
