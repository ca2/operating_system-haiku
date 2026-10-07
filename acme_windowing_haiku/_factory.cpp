#include "platform.h"
#include "haiku_windowing.h"
IMPLEMENT_FACTORY(acme_windowing_haiku){
 pfactory->add_factory_item<::haiku::acme::windowing::windowing,::acme::windowing::windowing>();
 pfactory->add_factory_item<::haiku::acme::windowing::window,::acme::windowing::window>();
 pfactory->add_factory_item<::haiku::acme::windowing::display,::acme::windowing::display>();
}
