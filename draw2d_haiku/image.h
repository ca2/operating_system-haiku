// Created by camilo on 2026-10-08 01:39 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once
#include "aura/graphics/image/image.h"

namespace draw2d_haiku
{


   class CLASS_DECL_DRAW2D_HAIKU image :
   virtual public ::image::image
   {


   public:
      image();
      ~image();

   protected:
      ::image_pixmap_lease _map(::image::enum_map, const ::i32_rectangle &) override;
      void _unmap(::image_pixmap_lease *) override;
   };

}
