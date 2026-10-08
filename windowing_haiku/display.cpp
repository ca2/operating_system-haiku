#include "platform.h"
#include "display.h"
#include "monitor.h"
#include "acme_windowing_haiku/native.h"
namespace windowing_haiku {
void display::open_display(){::haiku::acme::windowing::display::open_display();}
bool display::is_display_opened() const{return ::haiku::acme::windowing::display::is_display_opened();}
::i32_point display::_get_mouse_cursor_position(){int x=0,y=0;haiku_mouse_position(&x,&y);return {x,y};}
void display::_enumerate_monitors(){m_monitora.erase_all();::haiku::acme::windowing::display::_enumerate_monitors();}
::i32_size display::get_main_screen_size(){return ::haiku::acme::windowing::display::get_main_screen_size();}
bool display::_get_monitor_rectangle(::collection::index i,::i32_rectangle &r){::haiku::acme::windowing::monitor m;m.m_iScreen=int(i);if(!m.update_native_cache())return false;r=m.m_rectangleNative;return true;}
bool display::_get_workspace_rectangle(::collection::index i,::i32_rectangle &r){::haiku::acme::windowing::monitor m;m.m_iScreen=int(i);if(!m.update_native_cache())return false;r=m.m_rectangleNativeWorkspace;return true;}
}
