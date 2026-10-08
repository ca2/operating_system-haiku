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
};
}
