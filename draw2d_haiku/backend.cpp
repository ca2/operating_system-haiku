#include "platform.h"
#include "backend.h"
#include "acme/prototype/geometry2d/matrix.h"
#include "aura/graphics/image/drawing.h"
#include "acme/prototype/geometry2d/line.h"
#include "acme/prototype/geometry2d/lines.h"
#include "acme/prototype/geometry2d/ellipse.h"
#include "acme/prototype/geometry2d/arc.h"
#include "acme/prototype/geometry2d/polygon.h"
#include "aura/graphics/draw2d/domain.h"
#include "aura/graphics/draw2d/path.h"
#include "aura/graphics/image/image_pixmap_lease.h"
#include "acme/graphics/image/pixmap.h"
#include "acme/exception/interface_only.h"
namespace draw2d_haiku {
static haiku_draw_brush native_brush(::draw2d::brush *);
static uint32_t argb(const ::color::color &c) { return (uint32_t(c.u8_opacity())<<24)|(uint32_t(c.u8_red())<<16)|(uint32_t(c.u8_green())<<8)|c.u8_blue(); }
static void check(int ok) { if(!ok) throw ::exception(error_failed,"Haiku native drawing failed"); }
bitmap::~bitmap() { destroy(); }
void bitmap::destroy() { haiku_draw_surface_destroy(m_surface);m_surface=nullptr;::draw2d::bitmap::destroy(); }
void bitmap::create_bitmap(::draw2d::graphics *g,const ::i32_size &size) { create_bitmap(g,size,nullptr); }
void bitmap::create_bitmap(::draw2d::graphics *,const ::i32_size &size,::pixmap *pixels) {
 destroy();m_surface=haiku_draw_surface_create(size.cx,size.cy);check(m_surface!=nullptr);m_size=size;
 m_memoryDraw2dBitmap.set_size(size.cx*4*size.cy);
 memory_set(m_memoryDraw2dBitmap.data(),0,m_memoryDraw2dBitmap.size());
 if(pixels && pixels->m_pimage32Raw) check(haiku_draw_surface_write(m_surface,pixels->m_pimage32Raw,pixels->m_iScan));
}
void bitmap::read_pixels() { check(haiku_draw_surface_read(m_surface,m_memoryDraw2dBitmap.data(),m_size.cx*4)); }
void bitmap::commit_pixels() { check(haiku_draw_surface_write(m_surface,m_memoryDraw2dBitmap.data(),m_size.cx*4)); }
void *graphics::surface() {
 auto *b=dynamic_cast<bitmap *>(m_pdraw2dbitmap.m_p);
 if(!b || !b->m_surface) throw ::exception(error_wrong_state,"No Haiku drawing surface");return b->m_surface;
}
void graphics::set(::draw2d::bitmap *b) { if(!dynamic_cast<bitmap *>(b))throw ::exception(error_wrong_state);m_pdraw2dbitmap=b; }
void graphics::create_bitmap_graphics(::draw2d::bitmap *b,::draw2d::domain *d) {set_draw2d_domain(d);set(b);}
void graphics::line(double a,double b,double c,double d) {line(a,b,c,d,m_pdraw2dpen);}
void graphics::line(double a,double b,double c,double d,::draw2d::pen *p) {
 if(!p || p->m_epen==::draw2d::e_pen_null)return;
 check(haiku_draw_line(surface(),a,b,c,d,argb(p->m_color),p->m_dWidth,alpha_mode()==::draw2d::e_alpha_mode_blend));
}
void graphics::fill_rectangle(const ::f64_rectangle &r) {fill_rectangle(r,m_pdraw2dbrush);}
void graphics::fill_rectangle(const ::f64_rectangle &r,::draw2d::brush *b) {
 if(!b || b->m_ebrush==::draw2d::e_brush_null)return;
 if(b->m_ebrush==::draw2d::e_brush_solid){fill_rectangle(r,b->m_color);return;}
 haiku_draw_shape_destroy(m_shape);m_shape=haiku_draw_shape_create();check(m_shape!=nullptr);_set(r);
 auto brush=native_brush(b);check(haiku_draw_shape(surface(),m_shape,1,&brush,1,alpha_mode()==::draw2d::e_alpha_mode_blend,0));
}
void graphics::fill_rectangle(const ::f64_rectangle &r,const ::color::color &c) {check(haiku_draw_fill(surface(),0,r.left,r.top,r.right,r.bottom,argb(c),alpha_mode()==::draw2d::e_alpha_mode_blend));}
void graphics::draw_rectangle(const ::f64_rectangle &r) {draw_rectangle(r,m_pdraw2dpen);}
void graphics::draw_rectangle(const ::f64_rectangle &r,::draw2d::pen *p) {
 if(!p || p->m_epen==::draw2d::e_pen_null)return;
 check(haiku_draw_stroke(surface(),0,r.left,r.top,r.right,r.bottom,argb(p->m_color),p->m_dWidth,alpha_mode()==::draw2d::e_alpha_mode_blend));
}
void graphics::fill_ellipse(const ::f64_rectangle &r) {
 auto *b=m_pdraw2dbrush.m_p;if(!b || b->m_ebrush==::draw2d::e_brush_null)return;
 if(b->m_ebrush!=::draw2d::e_brush_solid)throw ::interface_only();
 check(haiku_draw_fill(surface(),1,r.left,r.top,r.right,r.bottom,argb(b->m_color),alpha_mode()==::draw2d::e_alpha_mode_blend));
}
void graphics::draw_ellipse(const ::f64_rectangle &r) {
 auto *p=m_pdraw2dpen.m_p;if(!p || p->m_epen==::draw2d::e_pen_null)return;
 check(haiku_draw_stroke(surface(),1,r.left,r.top,r.right,r.bottom,argb(p->m_color),p->m_dWidth,alpha_mode()==::draw2d::e_alpha_mode_blend));
}
void graphics::TextOutRaw(double x,double y,const ::scoped_string &text) {
 auto *f=m_pwritetextfont.m_p;::string s(text),family=f?f->family_name():"";
 auto c=m_pdraw2dbrush?m_pdraw2dbrush->m_color : ::argb(255,0,0,0);
 double size=f?f->m_fontsize.as_f64():12.;if(f && f->m_fontsize.eunit()==::e_unit_point)size*=get_dpix()/72.;
 check(haiku_draw_text(surface(),s.c_str(),s.size(),x,y,family.c_str(),size,f && f->m_fontweight.as_i32()>=700,f && f->m_bItalic,argb(c),alpha_mode()==::draw2d::e_alpha_mode_blend));
}
::f64_size graphics::get_text_extent(const ::scoped_string &text) {
 auto *f=m_pwritetextfont.m_p;::string s(text),family=f?f->family_name():"";double w=0,h=0;
 double size=f?f->m_fontsize.as_f64():12.;if(f && f->m_fontsize.eunit()==::e_unit_point)size*=get_dpix()/72.;
 check(haiku_draw_text_size(s.c_str(),s.size(),family.c_str(),size,f && f->m_fontweight.as_i32()>=700,f && f->m_bItalic,&w,&h));return {w,h};
}
::f64_size graphics::_get_text_extent(const ::scoped_string &s){return get_text_extent(s);}
::image_pixmap_lease image::_map(::image::enum_map mode,const ::i32_rectangle &r) {
 if(!m_pdraw2dbitmap)return ::image::image::_map(mode,r);
 auto *b=dynamic_cast<bitmap *>(m_pdraw2dbitmap.m_p);if(!b)throw ::exception(error_wrong_state);
 _tidy_map(r);b->read_pixels();auto p=create_newø<::pixmap>();
 p->m_memoryPixmap.reference_data(b->m_memoryDraw2dBitmap.data(),b->m_memoryDraw2dBitmap.size());
 p->m_pimage32Raw=(::image32_t *)b->m_memoryDraw2dBitmap.data();p->m_iScan=b->m_size.cx*4;p->m_bTopLeft=true;p->m_size=m_size;p->m_sizeRaw=b->m_size;
 p->pixmap_map(r.is_set()?r : ::i32_rectangle(m_point,m_size));return {this,p};
}
void image::_unmap(::image_pixmap_lease *lease) {
 ::image::image::_unmap(lease);if(auto *b=dynamic_cast<bitmap *>(m_pdraw2dbitmap.m_p))b->commit_pixels();
}
static haiku_draw_brush native_brush(::draw2d::brush *b){
 haiku_draw_brush n{};if(!b)throw ::exception(error_null_pointer);
 if(b->m_ebrush==::draw2d::e_brush_solid)n.color1=argb(b->m_color);
 else if(b->m_ebrush==::draw2d::e_brush_linear_gradient_point_color){n.kind=1;n.x1=b->m_point1.x;n.y1=b->m_point1.y;n.x2=b->m_point2.x;n.y2=b->m_point2.y;n.color1=argb(b->m_color1);n.color2=argb(b->m_color2);}
 else if(b->m_ebrush==::draw2d::e_brush_radial_gradient_color){
  if(b->m_size.cx!=b->m_size.cy)throw ::interface_only("Elliptical gradient pending");
  n.kind=2;n.x1=b->m_point.x;n.y1=b->m_point.y;n.radius=b->m_size.cx/2;n.color1=argb(b->m_color1);n.color2=argb(b->m_color2);
 }else throw ::interface_only("Haiku brush type pending");return n;
}
void graphics::prepare_path(::draw2d::path *p){
 if(!p)throw ::exception(error_null_pointer);haiku_draw_shape_destroy(m_shape);m_shape=haiku_draw_shape_create();check(m_shape!=nullptr);
 for(auto &item:p->m_itema)if(!::draw2d::graphics::_set(item))throw ::interface_only("Haiku path item pending");
}
void graphics::fill(::draw2d::path *p){
 if(!m_pdraw2dbrush || m_pdraw2dbrush->m_ebrush==::draw2d::e_brush_null)return;
 prepare_path(p);auto b=native_brush(m_pdraw2dbrush);check(haiku_draw_shape(surface(),m_shape,1,&b,1,alpha_mode()==::draw2d::e_alpha_mode_blend,p->m_efillmode==::draw2d::e_fill_mode_alternate));
}
void graphics::draw(::draw2d::path *p){
 auto *pen=m_pdraw2dpen.m_p;if(!pen || pen->m_epen==::draw2d::e_pen_null)return;prepare_path(p);
 haiku_draw_brush b{};b.color1=argb(pen->m_color);if(pen->m_pdraw2dbrush)b=native_brush(pen->m_pdraw2dbrush);
 check(haiku_draw_shape(surface(),m_shape,0,&b,pen->m_dWidth,alpha_mode()==::draw2d::e_alpha_mode_blend,0));
}
bool graphics::_set(const ::draw2d::enum_item &item){
 if(item==::draw2d::e_item_begin_figure || item==::draw2d::e_item_end_figure)haiku_draw_shape_begin(m_shape);
 else if(item==::draw2d::e_item_close_figure)haiku_draw_shape_close(m_shape);else return false;return true;
}
bool graphics::_set(const ::f64_line &v){haiku_draw_shape_line(m_shape,v.m_p1.x,v.m_p1.y,v.m_p2.x,v.m_p2.y);return true;}
bool graphics::_set(const ::f64_lines &v){for(::collection::index i=1;i<v.get_count();i++)haiku_draw_shape_line(m_shape,v[i-1].x,v[i-1].y,v[i].x,v[i].y);return true;}
bool graphics::_set(const ::f64_polygon_base &v){for(::collection::index i=1;i<v.get_count();i++)haiku_draw_shape_line(m_shape,v[i-1].x,v[i-1].y,v[i].x,v[i].y);haiku_draw_shape_close(m_shape);return true;}
bool graphics::_set(const ::f64_rectangle &r){haiku_draw_shape_begin(m_shape);haiku_draw_shape_line(m_shape,r.left,r.top,r.right,r.top);haiku_draw_shape_line(m_shape,r.right,r.top,r.right,r.bottom);haiku_draw_shape_line(m_shape,r.right,r.bottom,r.left,r.bottom);haiku_draw_shape_line(m_shape,r.left,r.bottom,r.left,r.top);haiku_draw_shape_close(m_shape);return true;}
bool graphics::_set(const ::f64_ellipse &r){haiku_draw_shape_begin(m_shape);haiku_draw_shape_arc(m_shape,r.left,r.top,r.right,r.bottom,0,2*MATH_PI);haiku_draw_shape_close(m_shape);return true;}
bool graphics::_set(const ::f64_arc &r){haiku_draw_shape_arc(m_shape,r.left,r.top,r.right,r.bottom,r.m_angleBeg.radian(),r.m_angleExt.radian());return true;}

::i32 graphics::save_graphics_context(){int state=haiku_draw_save(surface());check(state>0);return state;}
void graphics::restore_graphics_context(::i32 state){check(haiku_draw_restore(surface(),state));}
void graphics::_set(const ::geometry2d::matrix &m){check(haiku_draw_transform(surface(),m.a1,m.a2,m.b1,m.b2,m.c1,m.c2));}
void graphics::intersect_clip(const ::f64_rectangle &r){check(haiku_draw_clip(surface(),r.left,r.top,r.right,r.bottom,0));}
void graphics::reset_clip(){check(haiku_draw_clip(surface(),0,0,0,0,1));}
void graphics::_draw_raw(const ::f64_rectangle &dst,::image::image *src,const ::image::image_drawing_options &options,const ::f64_point &pt){_stretch_raw(dst,src,options,::f64_rectangle(pt,dst.size()));}
void graphics::_stretch_raw(const ::f64_rectangle &dst,::image::image *src,const ::image::image_drawing_options &options,const ::f64_rectangle &source){
 if(!src)throw ::exception(error_null_pointer);auto b=src->get_bitmap_as_source(this);auto *native=dynamic_cast<bitmap *>(b.m_p);
 if(!native)throw ::exception(error_wrong_state,"Source image has no Haiku bitmap");
 check(haiku_draw_blit(surface(),native->m_surface,dst.left,dst.top,dst.right,dst.bottom,source.left,source.top,source.right,source.bottom,alpha_mode()==::draw2d::e_alpha_mode_blend,options.opacity().f64_opacity()));
}

}
