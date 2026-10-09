#include "platform.h"
#include "still.h"
#include <StringView.h>
#include <Font.h>
namespace innate_ui_haiku {
BView *still::new_view() {
 auto *label = new BStringView(BRect(0,0,99,29), "ca2-label", m_text.c_str());
 label->SetViewColor(B_TRANSPARENT_COLOR);
 return label;
}
void still::set_text(const ::scoped_string &text) {
 m_text = text;
 if (m_nativeWindow && m_nativeWindow->Lock()) {
  static_cast<BStringView *>(m_nativeView)->SetText(m_text.c_str());
  m_nativeWindow->Unlock();
 }
}
void still::set_font_size(::f64 size) { m_dFontSizeEm = size; layout(); }
void still::set_font_weight(::i32 weight) { m_iFontWeight = weight; layout(); }
void still::layout() {
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 BFont font(be_plain_font);
 font.SetSize(font.Size()*m_dFontSizeEm);
 font.SetFace(m_iFontWeight >= 700 ? B_BOLD_FACE : B_REGULAR_FACE);
 m_nativeView->SetFont(&font);
 float width=0,height=0; m_nativeView->GetPreferredSize(&width,&height);
 m_iLayoutWidth = int(width+1); m_iLayoutHeight = int(height+1);
 m_nativeView->ResizeToPreferred();
 m_nativeWindow->Unlock();
}
void still::create_icon_still(::innate_ui::window *parent) {
 // Initial port provides a text placeholder; bitmap icons are a follow-up.
 create_child(parent);
}
}
