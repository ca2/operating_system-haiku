// Created by camilo on 2026-10-08 01:35 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "object.h"
#include "aura/graphics/draw2d/pen.h"
#include "operating_system-haiku/draw2d_haiku/object.h"


namespace draw2d_haiku
{


   class CLASS_DECL_DRAW2D_HAIKU pen :
      virtual public ::draw2d::pen,
      virtual public ::draw2d_haiku::object
   {
   public:


      pen();

      ~pen() override;


   };


}
