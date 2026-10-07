#pragma once
#include "acme/windowing/windowing.h"
#include "acme/windowing/window.h"
#include "acme/windowing/display.h"
#include "native.h"
namespace haiku::acme::windowing {
class windowing : virtual public ::acme::windowing::windowing {
public:
 void initialize_windowing() override;
 void run() override;
 void main_post(const ::procedure &) override;
 void main_send(const ::procedure &) override;
};
class display : virtual public ::acme::windowing::display {
public:
 void open_display() override {}
 bool is_display_opened() const override {return true;}
 void _enumerate_monitors() override;
 ::i32_size get_main_screen_size() override;
};
class window : virtual public ::acme::windowing::window {
public:
 void *m_native=nullptr;
 bool m_nativeVisible=false;
 ~window() override;
 virtual void native_event(const haiku_window_event &);
 void _create_window() override;
 void destroy_window() override;
 void set_window_text(const ::scoped_string &) override;
 ::i32_rectangle get_window_rectangle() override;
 void set_position(const ::i32_point &) override;
 void set_size(const ::i32_size &) override;
 void set_active_window() override;
 ::operating_system::window operating_system_window() const override;
};
}
