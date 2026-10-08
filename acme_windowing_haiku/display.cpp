#include "platform.h"
#include "display.h"
#include "monitor.h"
namespace haiku::acme::windowing {
void display::open_display(){monitor m;m_bDisplayOpened=m.update_native_cache();if(!m_bDisplayOpened)throw ::exception(error_failed,"Cannot open Haiku screen");_enumerate_monitors();}
bool display::is_display_opened() const{return m_bDisplayOpened;}
void display::_enumerate_monitors(){
 m_rectanglea.erase_all();
 for(int i=0;;++i){monitor m;m.m_iScreen=i;if(!m.update_native_cache())break;_on_monitor(i,m.m_rectangleNative,m.m_rectangleNativeWorkspace);}
}
::i32_size display::get_main_screen_size(){monitor m;if(!m.update_native_cache())throw ::exception(error_failed,"Cannot query Haiku screen");return m.m_rectangleNative.size();}
}
