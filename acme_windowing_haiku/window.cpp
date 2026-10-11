#include "platform.h"
#include "acme/operating_system/window.h"
#include "acme/nano/graphics/context.h"
#include "acme/platform/application.h"
#include "acme/platform/system.h"
#include "acme/user/micro/elemental.h"
#include "acme/user/micro/message_box.h"
#include "acme/user/user/interaction.h"
#include "acme/user/user/mouse.h"
#include "window.h"
namespace haiku::acme::windowing {
window::~window() {
  if (m_native)
    haiku_window_destroy(m_native);
}
void window::defer_show_system_menu(::user::mouse *mouse) {
  // Micro message boxes are borderless on Haiku, so their first text lines
  // belong to the client area too. Use the existing Acme dump popup there.
  ::cast<::micro::message_box> box = m_pacmeuserinteraction;
  if (box) box->on_context_menu(mouse);
}
static void dispatch_native_window_event(void *p, const haiku_window_event *e) {
  try {
    static_cast<window *>(p)->native_event(*e);
  } catch (const ::exception &error) {
    warning() << "Haiku window event: " << error.get_message();
  }
}
void window::_create_window() {
  auto r = m_rectangle;
  if (r.is_empty())
    r = {100, 100, 740, 580};
  ::string title =
      m_pacmeuserinteraction ? m_pacmeuserinteraction->get_title() : "ca2";
  m_native =
      haiku_window_create(this, dispatch_native_window_event, title.c_str(),
                          r.left, r.top, r.width(), r.height());
  if (!m_native)
    throw ::exception(error_failed, "Cannot create Haiku BWindow");
  m_nativeVisible = false;
}
void window::native_event(const haiku_window_event &e) {
  if (e.kind == 1)
    on_window_size({e.width, e.height});
  else if (e.kind == 2)
    on_window_position({e.x, e.y});
  else if (e.kind == 3)
    on_window_close();
  else if (e.kind == 20 && m_bMicroReady && m_pacmeuserinteraction) {
    auto context = system()->createø<::nano::graphics::context>();
    context->attach(e.graphics, {e.width, e.height}, 0);
    m_pacmeuserinteraction->on_window_paint(context);
    context->detach(0);
  } else {
    ::cast<::micro::elemental> elemental = m_pacmeuserinteraction;
    if (elemental) {
      ::user::mouse mouse;
      mouse.m_pointHost = {e.x, e.y};
      mouse.m_pointAbsolute = {e.width, e.height};
      if (e.kind == 4) {
        elemental->fore_on_left_button_down(&mouse);
        if (!mouse.m_bRet)
          elemental->back_on_left_button_down(&mouse);
      } else if (e.kind == 5) {
        elemental->fore_on_left_button_up(&mouse);
        if (!mouse.m_bRet)
          elemental->back_on_left_button_up(&mouse);
      } else if (e.kind == 6) {
        elemental->fore_on_mouse_move(&mouse);
        if (!mouse.m_bRet)
          elemental->back_on_mouse_move(&mouse);
      } else if (e.kind == 13) {
        elemental->fore_on_right_button_down(&mouse);
        if (!mouse.m_bRet) elemental->back_on_right_button_down(&mouse);
      } else if (e.kind == 14) {
        elemental->fore_on_right_button_up(&mouse);
        if (!mouse.m_bRet) elemental->back_on_right_button_up(&mouse);
      } else if (e.kind == 8)
        elemental->on_char(e.x);
    }
  }
}
void window::destroy_window() {
  auto p = m_native;
  m_native = nullptr;
  m_nativeVisible = false;
  haiku_window_destroy(p);
}
void window::set_window_text(const ::scoped_string &title) {
  ::string s(title);
  haiku_window_title(m_native, s.c_str());
}
::i32_rectangle window::get_window_rectangle() {
  int x = 0, y = 0, w = 0, h = 0;
  haiku_window_bounds(m_native, &x, &y, &w, &h);
  return {x, y, x + w, y + h};
}
void window::set_position(const ::i32_point &p) {
  auto r = get_window_rectangle();
  haiku_window_frame(m_native, p.x, p.y, r.width(), r.height());
}
void window::set_size(const ::i32_size &s) {
  auto r = get_window_rectangle();
  haiku_window_frame(m_native, r.left, r.top, s.cx, s.cy);
}
void window::set_active_window() { haiku_window_activate(m_native); }
::operating_system::window window::operating_system_window() const {
  return ::operating_system::window(
      ::operating_system::window_opaque_t((::u64)m_native, 0, 0),
      const_cast<window *>(this));
}

} // namespace haiku::acme::windowing

namespace haiku::acme::windowing {
::i32_rectangle window::window_get_client_rect() {
  auto r = get_window_rectangle();
  return {0, 0, r.width(), r.height()};
}
::i32_rectangle window::get_window_rect() { return get_window_rectangle(); }
::i32_rectangle window::get_window_rectangle_unlocked() {
  return get_window_rectangle();
}
} // namespace haiku::acme::windowing
namespace haiku::acme::windowing {
void window::create_window() {
  ::pointer<window> self = this;
  user_post([self]() {
    self->m_pacmeuserinteraction->on_before_create_window(self);
    self->m_rectangle = self->m_pacmeuserinteraction->initial_frame_rectangle();
    self->_create_window();
    self->on_window_create(nullptr);
    self->on_window_size(self->get_window_rectangle().size());
    self->m_bMicroReady = true;
    self->show_window(::user_interface::e_show_window_show_normal);
    self->redraw();
  });
}
void window::show_window(::user_interface::enum_show_window show) {
  if (m_native)
    haiku_window_show(m_native, show != ::user_interface::e_show_window_hide);
}
void window::redraw() {
  if (m_native)
    haiku_window_invalidate(m_native);
}
} // namespace haiku::acme::windowing
