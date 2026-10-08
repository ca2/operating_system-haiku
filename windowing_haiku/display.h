#pragma once
#include "aura/windowing/display.h"
#include "acme_windowing_haiku/display.h"
namespace windowing_haiku {
class display : virtual public ::windowing::display,virtual public ::haiku::acme::windowing::display {
public:
 void open_display() override;
 bool is_display_opened() const override;
 ::i32_point _get_mouse_cursor_position() override;
 void _enumerate_monitors() override;
 ::i32_size get_main_screen_size() override;
 bool _get_monitor_rectangle(::collection::index,::i32_rectangle &) override;
 bool _get_workspace_rectangle(::collection::index,::i32_rectangle &) override;
};
}
