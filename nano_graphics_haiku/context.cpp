#include "platform.h"
#include "context.h"
#include "acme/nano/graphics/font_family.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "icon.h"
#include <DataIO.h>
#include <TranslationUtils.h>
#include <Region.h>
namespace nano_graphics_haiku {
static rgb_color native_color(const ::color::color &c) {
  return {c.u8_red(), c.u8_green(), c.u8_blue(), c.u8_opacity()};
}
static BRect native_rect(const ::f64_rectangle &r) {
  return BRect(r.left, r.top, r.right - 1, r.bottom - 1);
}
context::~context() {
  if (m_bitmap) {
    m_bitmap->Unlock();
    delete m_bitmap;
  }
}
void context::create(const ::i32_size &s) {
  if (m_bitmap) {
    m_bitmap->Unlock();
    delete m_bitmap;
  }
  m_bitmap = new BBitmap(BRect(0, 0, s.cx - 1, s.cy - 1),
                         B_BITMAP_ACCEPTS_VIEWS, B_RGBA32);
  if (m_bitmap->InitCheck() != B_OK)
    throw ::exception(error_failed, "Cannot create Haiku nano bitmap");
  m_view = new BView(m_bitmap->Bounds(), "nano", B_FOLLOW_NONE, B_WILL_DRAW);
  m_bitmap->AddChild(m_view);
  m_bitmap->Lock();
  m_size = s;
}
void context::attach(void *d, const ::i32_size &s, int) {
  m_view = static_cast<BView *>(d);
  m_size = s;
}
void *context::detach(int) {
  auto *v = m_view;
  m_view = nullptr;
  return v;
}
void context::set_brush(::nano::graphics::brush *b) { m_brush = b; }
void context::set_pen(::nano::graphics::pen *p) { m_pen = p; }
void context::set_font(::nano::graphics::font *f) {
  m_font = f;
  if (!m_view)
    return;
  auto font = native_font();
  m_view->SetFont(&font);
}
void context::set_blend_mode(::nano::graphics::enum_blend_mode) {
  if (m_view) {
    m_view->SetDrawingMode(B_OP_ALPHA);
    m_view->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
  }
}
void context::set_smoothing_mode(::nano::graphics::enum_smoothing_mode) {}
void context::set_text_rendering_hint(
    ::nano::graphics::enum_text_rendering_hint) {}
void context::clear(const ::color::color &c) {
  if (m_view) {
    m_view->SetHighColor(native_color(c));
    m_view->FillRect(m_view->Bounds());
  }
}
void context::rectangle(const ::f64_rectangle &r) {
  if (!m_view)
    return;
  if (m_brush) {
    m_view->SetHighColor(native_color(m_brush->m_color));
    m_view->FillRect(native_rect(r));
  }
  if (m_pen) {
    m_view->SetHighColor(native_color(m_pen->m_color));
    m_view->SetPenSize(m_pen->m_fWidth);
    m_view->StrokeRect(native_rect(r));
  }
}
void context::ellipse(const ::f64_rectangle &r) {
  if (!m_view)
    return;
  if (m_brush) {
    m_view->SetHighColor(native_color(m_brush->m_color));
    m_view->FillEllipse(native_rect(r));
  }
  if (m_pen) {
    m_view->SetHighColor(native_color(m_pen->m_color));
    m_view->SetPenSize(m_pen->m_fWidth);
    m_view->StrokeEllipse(native_rect(r));
  }
}
void context::line(const ::f64_point &a, const ::f64_point &b) {
  if (m_view && m_pen) {
    m_view->SetHighColor(native_color(m_pen->m_color));
    m_view->SetPenSize(m_pen->m_fWidth);
    m_view->StrokeLine(BPoint(a.x, a.y), BPoint(b.x, b.y));
  }
}
void context::translate(double x, double y) {
  if (m_view)
    m_view->SetOrigin(m_view->Origin() + BPoint(x, y));
}
BFont context::native_font() const {
  BFont font;
  if (m_view)
    m_view->GetFont(&font);
  else
    font = *be_plain_font;
  if (m_font) {
    if (m_font->m_pfontfamily) {
      auto family = m_font->m_pfontfamily->m_strFamilyName;
      if (family.has_character()) font.SetFamilyAndStyle(family.c_str(), nullptr);
      else if (m_font->m_pfontfamily->m_efont == e_font_monospace) font = *be_fixed_font;
    }
    font.SetFace((m_font->m_bBold ? B_BOLD_FACE : 0) | (m_font->m_bItalic ? B_ITALIC_FACE : 0));
    font.SetSize(m_font->m_fFontSize);
  }
  return font;
}
::f64_size context::get_text_extents(const ::scoped_string &t) {
  ::string text(t);
  auto font = native_font();
  font_height h;
  font.GetHeight(&h);
  return {font.StringWidth(text.c_str()), h.ascent + h.descent + h.leading};
}
void context::draw_text123(const ::scoped_string &t, const ::f64_rectangle &r,
                          const ::e_draw_text &draw, const ::e_align &align) {
  if (!m_view || r.width() <= 0 || r.height() <= 0) return;
  ::string text(t);
  ::string_array_base lines;
  if (draw & e_draw_text_single_line) {
    text.replace_with(" ", "\r");
    text.replace_with(" ", "\n");
    lines.add(text);
  } else {
    character_count paragraphStart = 0;
    while (paragraphStart <= text.length()) {
      auto paragraphEnd = paragraphStart;
      while (paragraphEnd < text.length() && text[paragraphEnd] != '\n') ++paragraphEnd;
      ::string paragraph = text.substr(paragraphStart, paragraphEnd - paragraphStart);
      paragraph.ends_eat("\r");
      if (!(draw & e_draw_text_word_break) || paragraph.is_empty()) lines.add(paragraph);
      else {
        character_count start = 0;
        while (start < paragraph.length()) {
          auto end = start;
          character_count lastBreak = -1;
          while (end < paragraph.length()) {
            auto next = end + 1;
            // Never split a UTF-8 code point, including in long file paths.
            while (next < paragraph.length() && (::u8(paragraph[next]) & 0xc0) == 0x80) ++next;
            if (end > start && get_text_extents(paragraph.substr(start, next - start)).cx > r.width()) break;
            if (paragraph[end] == ' ' || paragraph[end] == '\t') lastBreak = end;
            end = next;
          }
          auto lineEnd = end;
          if (end < paragraph.length() && lastBreak > start) lineEnd = lastBreak;
          ::string line = paragraph.substr(start, lineEnd - start);
          line.trim_right();
          lines.add(line);
          start = lineEnd;
          while (start < paragraph.length() && (paragraph[start] == ' ' || paragraph[start] == '\t')) ++start;
        }
      }
      if (paragraphEnd == text.length()) break;
      paragraphStart = paragraphEnd + 1;
    }
  }
  auto font = native_font();
  m_view->SetFont(&font);
  font_height height;
  font.GetHeight(&height);
  auto lineHeight = height.ascent + height.descent + height.leading;
  auto blockHeight = lines.size() * lineHeight;
  auto top = r.top;
  if (align & e_align_vertical_center) top += (r.height() - blockHeight) / 2;
  else if (align & e_align_bottom) top = r.bottom - blockHeight;
  m_view->PushState();
  BRegion clip;
  clip.Set(native_rect(r));
  m_view->ConstrainClippingRegion(&clip);
  for (const auto &line : lines) {
    if (top + height.ascent > r.bottom) break;
    ::f64_rectangle lineRectangle(r.left, top, r.right, top + lineHeight);
    _draw_text(line, lineRectangle, e_draw_text_single_line, e_align_top_left | (align & (e_align_horizontal_center | e_align_right)));
    top += lineHeight;
  }
  m_view->PopState();
}
void context::_draw_text(const ::scoped_string &t, const ::f64_rectangle &r,
                         const ::e_draw_text &, const ::e_align &a) {
  if (!m_view)
    return;
  ::string text(t);
  auto size = get_text_extents(t);
  BFont font;
  m_view->GetFont(&font);
  font_height h;
  font.GetHeight(&h);
  float x = r.left, y = r.top + h.ascent;
  if (a & e_align_horizontal_center)
    x += (r.width() - size.cx) / 2;
  else if (a & e_align_right)
    x = r.right - size.cx;
  if (a & e_align_vertical_center)
    y = r.top + (r.height() - size.cy) / 2 + h.ascent;
  else if (a & e_align_bottom)
    y = r.bottom - h.descent;
  if (m_brush)
    m_view->SetHighColor(native_color(m_brush->m_color));
  m_view->DrawString(text.c_str(), BPoint(x, y));
}
void context::draw_icon(double x, double y, double w, double h,
                        ::nano::graphics::icon *i) {
  auto *icon = dynamic_cast<nano_graphics_haiku::icon *>(i);
  if (m_view && icon && icon->m_bitmap)
    m_view->DrawBitmap(icon->m_bitmap, icon->m_bitmap->Bounds(),
                       BRect(x, y, x + w - 1, y + h - 1));
}
} // namespace nano_graphics_haiku
