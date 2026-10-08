// Created by camilo on 2026-10-08 01:29 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "aura/graphics/write_text/font.h"
#include <Font.h>


namespace draw2d_haiku
{


   class CLASS_DECL_DRAW2D_HAIKU font :
      virtual public ::write_text::font
   {
   public:


      auto_pointer < BFont > m_pbfont;



      font();

      ~font() override;

      void destroy() override;

      void update(::draw2d::graphics * pdraw2dgraphics) override;
      //void update_native_font(double);

      //static BFont native_font(::write_text::font *,double);


   };


}
