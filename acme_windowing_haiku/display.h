#pragma once
#include "acme/windowing/display.h"
namespace haiku::acme::windowing {
class display : virtual public ::acme::windowing::display {
public:
 bool m_bDisplayOpened=false;
 void open_display() override;
 bool is_display_opened() const override;
 void _enumerate_monitors() override;
 ::i32_size get_main_screen_size() override;
 bool is_dark_mode_through_theming() override;
 ::string theming_ui_name() override;
 ::string impl_get_desktop_theme() override;
 void impl_set_desktop_theme(const ::scoped_string &theme) override;
};
}
