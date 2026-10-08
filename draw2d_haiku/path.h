// Created by camilo on 2026-10-08 01:41 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "object.h"
#include "aura/graphics/draw2d/path.h"
#include "operating_system-haiku/draw2d_haiku/object.h"


namespace draw2d_haiku
{


   class CLASS_DECL_DRAW2D_HAIKU path :
      virtual public ::draw2d::path,
      virtual public ::draw2d_haiku::object
   {
   public:


      path();
      ~path() override;


   };


} // namespace draw2d_haiku



