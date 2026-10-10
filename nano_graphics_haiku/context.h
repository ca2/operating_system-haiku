#pragma once
#include "acme/nano/graphics/brush.h"
#include "acme/nano/graphics/context.h"
#include "acme/nano/graphics/font.h"
#include "acme/nano/graphics/graphics.h"
#include "acme/nano/graphics/icon.h"
#include "acme/nano/graphics/pen.h"
#include <Bitmap.h>
#include <View.h>
namespace nano_graphics_haiku {
class context : virtual public ::nano::graphics::context {
public:
  BView *m_view = nullptr;
  BBitmap *m_bitmap = nullptr;
  ::pointer<::nano::graphics::brush> m_brush;
  ::pointer<::nano::graphics::pen> m_pen;
  ::pointer<::nano::graphics::font> m_font;
  ~context() override;
  void create(const ::i32_size &size) override;
  void attach(void *data, const ::i32_size &size, int type) override;
  void *detach(int type) override;
  void set_brush(::nano::graphics::brush *brush) override;
  void set_pen(::nano::graphics::pen *pen) override;
  void set_font(::nano::graphics::font *font) override;
  void set_blend_mode(::nano::graphics::enum_blend_mode mode) override;
  void set_smoothing_mode(::nano::graphics::enum_smoothing_mode mode) override;
  void set_text_rendering_hint(
      ::nano::graphics::enum_text_rendering_hint hint) override;
  void clear(const ::color::color &color) override;
  void rectangle(const ::f64_rectangle &r) override;
  void ellipse(const ::f64_rectangle &r) override;
  void line(const ::f64_point &a, const ::f64_point &b) override;
  void translate(double x, double y) override;
  ::f64_size get_text_extents(const ::scoped_string &text) override;
  BFont native_font() const;
  void draw_text123(const ::scoped_string &text, const ::f64_rectangle &r,
                    const ::e_draw_text &draw, const ::e_align &align) override;
  void _draw_text(const ::scoped_string &text, const ::f64_rectangle &r,
                  const ::e_draw_text &draw, const ::e_align &align) override;
  void draw_icon(double x, double y, double w, double h,
                 ::nano::graphics::icon *icon) override;
};
} // namespace nano_graphics_haiku
