#pragma once
#include "acme/windowing/window.h"
#include "native.h"
#include <atomic>
namespace haiku::acme::windowing {
class window : virtual public ::acme::windowing::window {
public:
 void *m_native=nullptr;
 bool m_nativeVisible=false;
 std::atomic_bool m_bMicroReady=false;
 ~window() override;
 virtual void native_event(const haiku_window_event &);
 void set_mouse_capture() override {}
 void release_mouse_capture() override {m_pacmeuserinteractionMouseCapture=nullptr;}
 bool _is_window() override {return m_native != nullptr;}
 void create_window() override;
  void defer_show_system_menu(::user::mouse *mouse) override;
 void show_window(::user_interface::enum_show_window show) override;
 void redraw() override;
 void _create_window() override;
 void destroy_window() override;
 void set_window_text(const ::scoped_string &) override;
 ::i32_rectangle get_window_rectangle() override;
 ::i32_rectangle window_get_client_rect() override;
 ::i32_rectangle get_window_rect() override;
 ::i32_rectangle get_window_rectangle_unlocked() override;
 void set_position(const ::i32_point &) override;
 void set_size(const ::i32_size &) override;
 void set_active_window() override;
 ::operating_system::window operating_system_window() const override;
};
}
