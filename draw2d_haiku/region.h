// Created by camilo on 2026-10-08 01:33 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "aura/graphics/draw2d/region.h"
#include "operating_system-haiku/draw2d_haiku/object.h"


namespace draw2d_haiku
{

   class CLASS_DECL_DRAW2D_HAIKU region :
      virtual public ::draw2d::region,
      virtual public ::draw2d_haiku::object
   {
   public:


      region();
      ~region() override;

   };


}
