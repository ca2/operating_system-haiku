#include "platform.h"
#include "draw2d.h"
#include "acme/platform/system.h"
#include "acme/graphics/image/color_indexes.h"

namespace draw2d_haiku
{
   void draw2d::initialize(::particle *pparticle)
   {
      // Haiku B_RGBA32 and this backend's CPU pixel buffers use BGRA bytes.
      // NanoSVG and other image producers must use that same channel order.
      set_common_system_image_color_indexes({2, 1, 0, 3});
      ::system()->m_bDefaultRedLower = false;
      ::draw2d::draw2d::initialize(pparticle);
   }
}
