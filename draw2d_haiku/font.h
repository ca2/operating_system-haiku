#pragma once
#include "aura/graphics/write_text/font.h"
#include <Font.h>
namespace draw2d_haiku {
class font : virtual public ::write_text::font {
public:
 BFont m_bfont;
 void update_native_font(double);
 static BFont native_font(::write_text::font *,double);
};
}
