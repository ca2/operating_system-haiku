#include "platform.h"
#include "image.h"
#include "graphics.h"
#include "bitmap.h"
#include "pen.h"
#include "brush.h"
#include "region.h"
#include "font.h"
#include "internal_font.h"
#include "path.h"
#include "draw2d.h"
#include "aura/graphics/draw2d/domain.h"
#include "aura/graphics/draw2d/window_attachment.h"

//BEGIN_FACTORY(draw2d_haiku)
//FACTORY_ITEM(::draw2d_haiku::image)
//FACTORY_ITEM(::draw2d_haiku::graphics)
//FACTORY_ITEM(::draw2d_haiku::bitmap)
//FACTORY_ITEM(::draw2d_haiku::pen)
//FACTORY_ITEM(::draw2d_haiku::brush)
//FACTORY_ITEM(::draw2d_haiku::region)
//FACTORY_ITEM(::draw2d_haiku::font)
//FACTORY_ITEM(::draw2d_haiku::path)
//FACTORY_ITEM(::draw2d_haiku::draw2d)
//END_FACTORY()
//
//

__FACTORY_EXPORT void draw2d_haiku_factory(::factory::factory * pfactory)
{


   pfactory->add_factory_item < ::draw2d_haiku::image, ::image::image >();
   pfactory->add_factory_item < ::draw2d_haiku::graphics, ::draw2d::graphics >();
   pfactory->add_factory_item < ::draw2d_haiku::bitmap, ::draw2d::bitmap >();
   pfactory->add_factory_item < ::draw2d_haiku::pen, ::draw2d::pen >();
   pfactory->add_factory_item < ::draw2d_haiku::brush, ::draw2d::brush >();
   pfactory->add_factory_item < ::draw2d_haiku::region, ::draw2d::region >();
   pfactory->add_factory_item < ::draw2d_haiku::font, ::write_text::font >();
   pfactory->add_factory_item < ::draw2d_haiku::internal_font, ::write_text::internal_font >();
   pfactory->add_factory_item < ::draw2d_haiku::path, ::draw2d::path >();
   pfactory->add_factory_item < ::draw2d_haiku::draw2d, ::draw2d::draw2d >();

   pfactory->add_factory_item < ::draw2d::domain, ::acme::draw2d::domain >();

   pfactory->add_factory_item < ::draw2d::window_attachment >();


}



