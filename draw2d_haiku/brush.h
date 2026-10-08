
// Created by camilo on 2026-10-08 01:36 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "aura/graphics/draw2d/brush.h"
#include "operating_system-haiku/draw2d_haiku/object.h"


namespace draw2d_haiku
{


   class CLASS_DECL_DRAW2D_HAIKU brush :
      virtual public ::draw2d::brush,
      virtual public ::draw2d_haiku::object
   {
   public:


      brush();
      ~brush() override;


   };



}
