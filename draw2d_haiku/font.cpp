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
    m_pbfont->SetFamilyAndStyle("Noto Sans", "Regular");
    auto family = family_name();
    if (family == "sans-serif") family = "Noto Sans";
    else if (family == "serif") family = "Noto Serif";
    else if (family == "monospace") family = "Noto Sans Mono";
    if (family.has_character()) m_pbfont->SetFamilyAndStyle(family.c_str(), nullptr);
    double size = m_fontsize.as_f64();
    if (m_fontsize.eunit() == ::e_unit_point) size *= (graphics ? graphics->get_dpix() : 96.) / 72.;
    m_pbfont->SetSize(float(size));
    m_pbfont->SetFace((m_fontweight.as_i32() >= 700 ? B_BOLD_FACE : B_REGULAR_FACE) | (m_bItalic ? B_ITALIC_FACE : 0));
    font_height height{};
    m_pbfont->GetHeight(&height);
    m_textmetric2.m_dAscent = height.ascent;
    m_textmetric2.m_dDescent = height.descent;
    m_textmetric2.m_dInternalLeading = 0;
    m_textmetric2.m_dExternalLeading = height.leading;
    m_textmetric2.m_dHeight = height.ascent + height.descent + height.leading;
    m_textmetric2.m_dWeight = m_fontweight.as_i32();
    m_textmetric2.m_bItalic = m_bItalic;
}
}
