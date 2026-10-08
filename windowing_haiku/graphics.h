// Created by camilo on 2026-10-08 02:18 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "aura/graphics/graphics/double_buffer.h"


namespace windowing_haiku
{


   class CLASS_DECL_WINDOWING_HAIKU graphics :
      virtual public ::graphics::double_buffer_graphics
   {
   public:


      void update_screen() override;
      void on_update_screen(::graphics::buffer_item *) override;


   };



}
