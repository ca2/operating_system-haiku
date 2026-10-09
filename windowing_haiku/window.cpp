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
#include "aura/message/user.h"
#include "aura/platform/session.h"
#include <InterfaceDefs.h>

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
      if (e.kind == 8 || e.kind == 9 || e.kind == 11)
      {
         if (auto *ui = user_interaction())
         {
            ::pointer<window> self = this;
            ui->post([self,e]()
            {
               auto *interaction = self->user_interaction();
               if (!interaction) return;
               auto session = interaction->session();
               session->set_key_pressed(::user::e_key_shift, (e.width & B_SHIFT_KEY) != 0);
               session->set_key_pressed(::user::e_key_control, (e.width & (B_CONTROL_KEY | B_COMMAND_KEY)) != 0);
               session->set_key_pressed(::user::e_key_alt, (e.width & B_OPTION_KEY) != 0);
               if (e.kind == 11) return;
               ::user::e_key key = ::user::e_key_none;
               switch (e.x)
               {
               case B_BACKSPACE: key = ::user::e_key_back; break;
               case B_DELETE: key = ::user::e_key_delete; break;
               case B_LEFT_ARROW: key = ::user::e_key_left; break;
               case B_RIGHT_ARROW: key = ::user::e_key_right; break;
               case B_UP_ARROW: key = ::user::e_key_up; break;
               case B_DOWN_ARROW: key = ::user::e_key_down; break;
               case B_HOME: key = ::user::e_key_home; break;
               case B_END: key = ::user::e_key_end; break;
               case B_PAGE_UP: key = ::user::e_key_page_up; break;
               case B_PAGE_DOWN: key = ::user::e_key_page_down; break;
               case B_ENTER: key = ::user::e_key_return; break;
               case B_TAB: key = ::user::e_key_tab; break;
               case B_ESCAPE: key = ::user::e_key_escape; break;
               case B_SPACE: key = ::user::e_key_space; break;
               default:
                  if (e.x >= 'a' && e.x <= 'z') key = ::user::e_key_a + (e.x - 'a');
                  else if (e.x >= '0' && e.x <= '9') key = ::user::e_key_0 + (e.x - '0');
                  break;
               }
               auto message = self->create_newø<::message::key>();
               message->m_eusermessage = e.kind == 8 ? ::user::e_message_key_down : ::user::e_message_key_up;
               message->m_operatingsystemwindow = self->operating_system_window();
               message->m_pwindow = self;
               message->m_ekey = key;
               message->m_nChar = e.x;
               message->m_nScanCode = e.y;
               message->m_strText = e.text;
               interaction->send_message(message);
               if (e.kind == 8 && (unsigned char)e.text[0] >= 32 && e.x != B_DELETE
                  && !(e.width & (B_CONTROL_KEY | B_COMMAND_KEY | B_OPTION_KEY)))
               {
                  auto character = self->create_newø<::message::key>();
                  character->m_eusermessage = ::user::e_message_char;
                  character->m_operatingsystemwindow = self->operating_system_window();
                  character->m_pwindow = self;
                  character->m_ekey = ::user::e_key_refer_to_text_member;
                  character->m_strText = e.text;
                  interaction->send_message(character);
               }
            });
         }
         return;
      }
      if (e.kind == 10)
      {
         if (auto *ui = user_interaction())
         {
            ::pointer<window> self = this;
            ui->post([self,e]()
            {
               if (e.x) self->window_on_set_keyboard_focus();
               else self->window_on_kill_keyboard_focus();
               if (auto *interaction = self->user_interaction())
                  interaction->send_message(e.x ? ::user::e_message_set_focus : ::user::e_message_kill_focus);
            });
         }
         return;
      }
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
   void window::set_keyboard_focus() { _set_keyboard_focus_unlocked(); }
   void window::_set_keyboard_focus_unlocked()
   {
      haiku_window_focus(m_native);
      window_on_set_keyboard_focus();
      if (auto *ui = user_interaction()) ui->send_message(::user::e_message_set_focus);
   }
   bool window::has_keyboard_focus() { return haiku_window_has_focus(m_native) != 0; }

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
