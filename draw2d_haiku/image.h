#pragma once
#include "aura/graphics/image/image.h"
namespace draw2d_haiku {
class image : virtual public ::image::image {
protected:
 ::image_pixmap_lease _map(::image::enum_map,const ::i32_rectangle &) override;
 void _unmap(::image_pixmap_lease *) override;
};
}
