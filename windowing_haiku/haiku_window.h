#pragma once
#include "acme_windowing_haiku/haiku_windowing.h"
#include "aura/windowing/window.h"
#include "aura/windowing/windowing.h"
#include "aura/windowing/display.h"
#include "aura/graphics/graphics/double_buffer.h"
namespace windowing_haiku {
class window : virtual public ::windowing::window, virtual public ::haiku::acme::windowing::window {
public:
 void _create_window() override;
 void native_event(const haiku_window_event &) override;
 void destroy_window() override;
 void main_send(const ::procedure &) override;
 void main_post(const ::procedure &) override;
 void set_window_text(const ::scoped_string &) override;
 ::i32_rectangle get_window_rectangle() override;
 void set_position(const ::i32_point &) override;
 void set_size(const ::i32_size &) override;
 void set_active_window() override;
 ::operating_system::window operating_system_window() const override;
 bool is_window() override {return m_native!=nullptr;}
 bool is_window_visible() override {return m_nativeVisible;}
 bool _is_window_visible_unlocked() override {return m_nativeVisible;}
 void window_update_screen() override;
 bool _strict_set_window_position_unlocked(::i32,::i32,::i32,::i32,bool,bool) override;
};
class windowing : virtual public ::windowing::windowing,virtual public ::haiku::acme::windowing::windowing {
public:
 void initialize_windowing() override {::haiku::acme::windowing::windowing::initialize_windowing();}
 void run() override {::haiku::acme::windowing::windowing::run();}
 void main_send(const ::procedure &p) override {::haiku::acme::windowing::windowing::main_send(p);}
 void main_post(const ::procedure &p) override {::haiku::acme::windowing::windowing::main_post(p);}
};
class display : virtual public ::windowing::display,virtual public ::haiku::acme::windowing::display {
public:
 void open_display() override {}
 bool is_display_opened() const override {return true;}
 ::i32_point _get_mouse_cursor_position() override {int x=0,y=0;haiku_mouse_position(&x,&y);return {x,y};}
 void _enumerate_monitors() override {::haiku::acme::windowing::display::_enumerate_monitors();}
 ::i32_size get_main_screen_size() override {return ::haiku::acme::windowing::display::get_main_screen_size();}
};
class graphics : virtual public ::graphics::double_buffer_graphics {
public:
 void update_screen() override {if(m_pwindow)m_pwindow->window_update_screen();}
};
}
