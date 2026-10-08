#include "platform.h"
#include "monitor.h"
#include "aura/windowing/display.h"
namespace windowing_haiku {
void monitor::initialize_monitor(::windowing::display *d,int i){m_pdisplay=d;m_iIndex=i;m_iScreen=i;update_cache();}
void monitor::update_cache(){if(!update_native_cache())throw ::exception(error_failed,"Cannot query Haiku monitor");m_rectangle=m_rectangleNative;m_rectangleWorkspace=m_rectangleNativeWorkspace;}
::i32_rectangle monitor::monitor_rectangle(){update_cache();return m_rectangle;}
::i32_rectangle monitor::_workspace_rectangle(){update_cache();return m_rectangleWorkspace;}
}
