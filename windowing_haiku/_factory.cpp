#include "platform.h"
#include "windowing.h"
#include "window.h"
#include "display.h"
#include "graphics.h"
#include "monitor.h"
#include "text_composition_client.h"
#include "shell.h"
#include "icon.h"
#include "aura/windowing/keyboard.h"
#include "aura/windowing/cursor.h"
IMPLEMENT_FACTORY(windowing_haiku){
 pfactory->add_factory_item<::windowing_haiku::windowing,::acme::windowing::windowing>();
 pfactory->add_factory_item<::windowing_haiku::window,::acme::windowing::window>();
 pfactory->add_factory_item<::windowing_haiku::display,::acme::windowing::display>();
 pfactory->add_factory_item<::windowing_haiku::graphics,::graphics::graphics>();
 pfactory->add_factory_item<::windowing_haiku::monitor,::windowing::monitor>();
 pfactory->add_factory_item<::windowing::keyboard>();
 pfactory->add_factory_item<::windowing::cursor>();
 pfactory->add_factory_item<::windowing_haiku::text_composition_client,::user::text_composition_client>();
 pfactory->add_factory_item<::windowing_haiku::shell,::user::shell>();
 pfactory->add_factory_item<::windowing_haiku::icon,::windowing::icon>();
}
