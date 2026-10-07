#pragma once
#include "aura/graphics/draw2d/bitmap.h"
#include "aura/graphics/draw2d/graphics.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/draw2d/brush.h"
#include "aura/graphics/draw2d/pen.h"
#include "aura/graphics/write_text/font.h"
#include "native.h"
namespace draw2d_haiku {
class bitmap : virtual public ::draw2d::bitmap {
public:
 void *m_surface=nullptr;
 ~bitmap() override;
 void destroy() override;
 void create_bitmap(::draw2d::graphics *, const ::i32_size &) override;
 void create_bitmap(::draw2d::graphics *, const ::i32_size &, ::pixmap *) override;
 ::i32 stride_for_width(::i32 w) override { return w*4; }
 ::i32_size size() const override { return m_size; }
 void set_size(const ::i32_size &,bool preserve=false) override;
 void write_pixels(const ::i32_size &,const ::i32_point &,const ::image32_t *,::i32,bool) override;
 void read_pixels();
 void commit_pixels();
};
class graphics : virtual public ::draw2d::graphics {
public:
 using ::draw2d::graphics::set;
 using ::draw2d::graphics::line;
 using ::draw2d::graphics::draw_ellipse;
 using ::draw2d::graphics::fill_ellipse;
 void *surface();
 void on_acquire_memory_graphics(bool,::image::image *,const ::i32_size &,::draw2d::domain *) override;
 void _create_memory_graphics(const ::i32_size &,::draw2d::domain *) override;
 ::i32 save_graphics_context() override;
 void restore_graphics_context(::i32) override;
 void _set(const ::geometry2d::matrix &) override;
 void intersect_clip(const ::f64_rectangle &) override;
 void reset_clip() override;
 void _draw_raw(const ::f64_rectangle &,::image::image *,const ::image::image_drawing_options &,const ::f64_point &) override;
 void _stretch_raw(const ::f64_rectangle &,::image::image *,const ::image::image_drawing_options &,const ::f64_rectangle &) override;

 void *m_shape=nullptr;
 ~graphics() override {haiku_draw_shape_destroy(m_shape);}
 using ::draw2d::graphics::_set;
 using ::draw2d::graphics::draw;
 using ::draw2d::graphics::fill;
 void draw(::draw2d::path *) override;
 void fill(::draw2d::path *) override;
 bool _set(const ::draw2d::enum_item &) override;
 bool _set(const ::f64_line &) override;
 bool _set(const ::f64_lines &) override;
 bool _set(const ::f64_rectangle &) override;
 bool _set(const ::f64_ellipse &) override;
 bool _set(const ::f64_arc &) override;
 bool _set(const ::f64_polygon_base &) override;
 void prepare_path(::draw2d::path *);

 void set(::draw2d::bitmap *) override;
 void create_bitmap_graphics(::draw2d::bitmap *, ::draw2d::domain *) override;
 void line(double,double,double,double) override;
 void line(double,double,double,double,::draw2d::pen *) override;
 void fill_rectangle(const ::f64_rectangle &) override;
 void fill_rectangle(const ::f64_rectangle &,::draw2d::brush *) override;
 void fill_rectangle(const ::f64_rectangle &,const ::color::color &) override;
 void draw_rectangle(const ::f64_rectangle &) override;
 void draw_rectangle(const ::f64_rectangle &,::draw2d::pen *) override;
 void fill_ellipse(const ::f64_rectangle &) override;
 void draw_ellipse(const ::f64_rectangle &) override;
 void TextOutRaw(double,double,const ::scoped_string &) override;
 ::f64_size get_text_extent(const ::scoped_string &) override;
 ::f64_size _get_text_extent(const ::scoped_string &) override;
};
class image : virtual public ::image::image {
protected:
 ::image_pixmap_lease _map(::image::enum_map,const ::i32_rectangle &) override;
 void _unmap(::image_pixmap_lease *) override;
};
}
