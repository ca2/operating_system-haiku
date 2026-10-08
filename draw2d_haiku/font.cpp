// Native Haiku font implementation.
#include "platform.h"
#include "aura/graphics/draw2d/graphics.h"
#include "font.h"
namespace draw2d_haiku {
font::font() {}
font::~font() {}
void font::destroy() {
    m_pbfont.destroy();
    ::write_text::font::destroy();
}
void font::update(::draw2d::graphics *graphics) {
    m_pbfont = auto_pointer<BFont>(transfer_t{}, new BFont(be_plain_font));
    auto family = family_name();
    if (family.has_character()) m_pbfont->SetFamilyAndStyle(family.c_str(), nullptr);
    double size = m_fontsize.as_f64();
    if (m_fontsize.eunit() == ::e_unit_point) size *= (graphics ? graphics->get_dpix() : 96.) / 72.;
    m_pbfont->SetSize(float(size));
    m_pbfont->SetFace((m_fontweight.as_i32() >= 700 ? B_BOLD_FACE : B_REGULAR_FACE) | (m_bItalic ? B_ITALIC_FACE : 0));
}
}
