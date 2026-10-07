#include "platform.h"
#include "haiku_window.h"
#include "aura/windowing/monitor.h"
#include "aura/windowing/keyboard.h"
#include "aura/windowing/cursor.h"
IMPLEMENT_FACTORY(windowing_haiku){
 pfactory->add_factory_item<::windowing_haiku::windowing,::acme::windowing::windowing>();
 pfactory->add_factory_item<::windowing_haiku::window,::acme::windowing::window>();
 pfactory->add_factory_item<::windowing_haiku::display,::acme::windowing::display>();
 pfactory->add_factory_item<::windowing_haiku::graphics,::graphics::graphics>();
 pfactory->add_factory_item<::windowing::monitor>();
 pfactory->add_factory_item<::windowing::keyboard>();
 pfactory->add_factory_item<::windowing::cursor>();
}
