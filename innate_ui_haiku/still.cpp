#include "platform.h"
#include "still.h"
#include <StringView.h>
#include <Font.h>
#include <Message.h>
namespace innate_ui_haiku {
static void panel_colors(BView *view) {
 view->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
 view->SetLowUIColor(B_PANEL_BACKGROUND_COLOR);
 view->SetHighUIColor(B_PANEL_TEXT_COLOR);
}
class themed_label : public BStringView {
public:
 themed_label(const char *text) : BStringView(BRect(0,0,99,29), "ca2-label", text) {
  panel_colors(this);
 }
 void AttachedToWindow() override {
  BStringView::AttachedToWindow();
  panel_colors(this);
 }
 void MessageReceived(BMessage *message) override {
  BStringView::MessageReceived(message);
  if (message->what == B_COLORS_UPDATED) { panel_colors(this); Invalidate(); }
 }
};
class icon_view : public BView {
public:
 BBitmap *bitmap = nullptr;
 icon_view() : BView(BRect(0,0,47,47), "ca2-icon", B_FOLLOW_NONE, B_WILL_DRAW) {
  panel_colors(this);
 }
 void MessageReceived(BMessage *message) override {
  BView::MessageReceived(message);
  if (message->what == B_COLORS_UPDATED) { panel_colors(this); Invalidate(); }
 }
 void Draw(BRect) override {
  // Alpha pixels must blend over the current panel, not the default gray.
  PushState();
  SetDrawingMode(B_OP_COPY);
  SetHighColor(LowColor());
  FillRect(Bounds());
  PopState();
  if (!bitmap) return;
  SetDrawingMode(B_OP_ALPHA);
  SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
  DrawBitmap(bitmap, bitmap->Bounds(), Bounds(), B_FILTER_BITMAP_BILINEAR);
 }
};
BView *still::new_view() {
 if (m_iconStill) return new icon_view();
 return new themed_label(m_text.c_str());
}
void still::set_text(const ::scoped_string &text) {
 m_text = text;
 if (m_nativeWindow && m_nativeWindow->Lock()) {
  if (auto *label = dynamic_cast<BStringView *>(m_nativeView)) label->SetText(m_text.c_str());
  m_nativeWindow->Unlock();
 }
}
void still::set_font_size(::f64 size) { m_dFontSizeEm = size; layout(); }
void still::set_font_weight(::i32 weight) { m_iFontWeight = weight; layout(); }
void still::layout() {
 if (m_iconStill) return;
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
 m_iconStill = true;
 create_child(parent);
}
void still::set_icon(::innate_ui::icon *source) {
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 m_icon = dynamic_cast<icon *>(source);
 if (auto *view = dynamic_cast<icon_view *>(m_nativeView)) {
  view->bitmap = m_icon ? m_icon->m_bitmap : nullptr;
  view->Invalidate();
 }
 m_nativeWindow->Unlock();
}
}
