/*
 * Created by camilo on 2026-10-07 19:39 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
 */
#include "platform.h"
#include "graphics.h"


namespace draw2d_haiku
{

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


} // namespace draw2d_haiku



