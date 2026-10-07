#include "platform.h"
#include "backend.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "aura/graphics/draw2d/path.h"
#include "aura/graphics/draw2d/region.h"
#include "aura/graphics/draw2d/palette.h"
#include "aura/graphics/draw2d/domain.h"
IMPLEMENT_FACTORY(draw2d_haiku) {
 pfactory->add_factory_item<::draw2d_haiku::graphics,::draw2d::graphics>();
 pfactory->add_factory_item<::draw2d_haiku::bitmap,::draw2d::bitmap>();
 pfactory->add_factory_item<::draw2d_haiku::image,::image::image>();
 pfactory->add_factory_item<::draw2d::pen>();
 pfactory->add_factory_item<::draw2d::brush>();
 pfactory->add_factory_item<::write_text::font>();
 pfactory->add_factory_item<::draw2d::path>();
 pfactory->add_factory_item<::draw2d::region>();
 pfactory->add_factory_item<::draw2d::palette>();
 pfactory->add_factory_item<::draw2d::draw2d>();
 pfactory->add_factory_item<::draw2d::domain,::acme::draw2d::domain>();
}
