#include "platform.h"
#include "font.h"
namespace draw2d_haiku {
void font::update_native_font(double dpi){
 m_bfont=BFont(be_plain_font);
 auto family=family_name();
 if(family.has_character())m_bfont.SetFamilyAndStyle(family.c_str(),nullptr);
 double size=m_fontsize.as_f64();
 if(m_fontsize.eunit()==::e_unit_point)size*=dpi/72.;
 m_bfont.SetSize(float(size));
 m_bfont.SetFace((m_fontweight.as_i32()>=700?B_BOLD_FACE:B_REGULAR_FACE)|(m_bItalic?B_ITALIC_FACE:0));
}
BFont font::native_font(::write_text::font *p,double dpi){
 if(!p)return BFont(be_plain_font);
 auto *f=dynamic_cast<font *>(p);
 if(!f)throw ::exception(error_wrong_state,"Font is not a Haiku font");
 f->update_native_font(dpi);return f->m_bfont;
}
}
