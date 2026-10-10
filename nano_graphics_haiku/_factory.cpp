#include "platform.h"
#include "acme/nano/graphics/font_family.h"
#include "context.h"
#include "icon.h"
IMPLEMENT_FACTORY(nano_graphics_haiku) {
  pfactory->add_factory_item<::nano::graphics::font_family>();
  pfactory->add_factory_item<nano_graphics_haiku::context,
                             ::nano::graphics::context>();
  pfactory
      ->add_factory_item<nano_graphics_haiku::icon, ::nano::graphics::icon>();
  pfactory->add_factory_item<::nano::graphics::brush>();
  pfactory->add_factory_item<::nano::graphics::pen>();
  pfactory->add_factory_item<::nano::graphics::font>();
  pfactory->add_factory_item<::nano::graphics::graphics>();
}
