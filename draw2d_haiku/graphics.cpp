#include "platform.h"
#include "aura/graphics/image/image.h"
#include "graphics.h"
#include "bitmap.h"
#include "font.h"
#include "acme/prototype/geometry2d/matrix.h"
#include "acme/prototype/geometry2d/line.h"
#include "acme/prototype/geometry2d/lines.h"
#include "acme/prototype/geometry2d/ellipse.h"
#include "acme/prototype/geometry2d/arc.h"
#include "acme/prototype/geometry2d/polygon.h"
#include "aura/graphics/draw2d/domain.h"
#include "aura/graphics/draw2d/path.h"
#include "aura/graphics/draw2d/brush.h"
#include "aura/graphics/draw2d/pen.h"
#include "aura/graphics/image/drawing.h"
#include "acme/exception/interface_only.h"
#include <AffineTransform.h>
#include <GradientLinear.h>
#include <GradientRadial.h>
#include <cmath>
namespace draw2d_haiku {
static rgb_color as_rgb_color(const ::color::color &c){return {c.u8_red(),c.u8_green(),c.u8_blue(),c.u8_opacity()};}
static BRect as_brect(const ::f64_rectangle &r){return BRect(r.left,r.top,r.right-1,r.bottom-1);}
bitmap *graphics::target_bitmap(){
 defer_on_target_rectangle_update();
 auto *b=dynamic_cast<bitmap *>(m_pdraw2dbitmap.m_p);
 if(!b || !b->m_pbbitmap)throw ::exception(error_wrong_state,"No Haiku drawing bitmap");
 return b;
}
void graphics::_001ColorSelect(const ::color::color &c,bool blend){
 update_matrix();
 auto *v=target_bitmap()->m_pbview;v->SetHighColor(as_rgb_color(c));
 v->SetDrawingMode(blend?B_OP_ALPHA:B_OP_COPY);
 v->SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_COMPOSITE);
}
void graphics::set(::draw2d::bitmap *b){
 if(!dynamic_cast<bitmap *>(b))throw ::exception(error_wrong_state);
 m_pdraw2dbitmap=b;
}
void graphics::create_bitmap_graphics(::draw2d::bitmap *b,::draw2d::domain *d){set_draw2d_domain(d);set(b);}
void graphics::on_acquire_memory_graphics(bool external,::image::image *target,const ::i32_size &size,::draw2d::domain *domain){
 m_pointBitmapOrigin=target?::f64_point(target->m_point) : ::f64_point();
 if(target){auto b=target->get_bitmap_as_target(this);set(b);}
 ::draw2d::graphics::on_acquire_memory_graphics(external,target,size,domain);
}
void graphics::_create_memory_graphics(const ::i32_size &size,::draw2d::domain *domain){
 set_draw2d_domain(domain);constructø(m_pimageOwned);
 m_pimageOwned->update_as_render_target(size,domain,this);m_pimageOwned->m_pgraphicsOwned=this;
 set(m_pimageOwned->m_pdraw2dbitmap);m_pimageTarget=m_pimageOwned;set_ok_flag();
}
void graphics::line(double a,double b,double c,double d){line(a,b,c,d,m_pdraw2dpen);}
void graphics::line(double a,double b,double c,double d,::draw2d::pen *p){
 if(!p || p->m_epen==::draw2d::e_pen_null)return;
 graphics_lock lock(target_bitmap());_001ColorSelect(p->m_color,alpha_mode()==::draw2d::e_alpha_mode_blend);
 auto *v=lock.m_pbitmap->m_pbview;v->SetPenSize(p->m_dWidth);v->StrokeLine(BPoint(a,b),BPoint(c,d));
}
void graphics::fill_rectangle(const ::f64_rectangle &r){fill_rectangle(r,m_pdraw2dbrush);}
void graphics::fill_rectangle(const ::f64_rectangle &r,::draw2d::brush *b){
 if(!b || b->m_ebrush==::draw2d::e_brush_null)return;
 if(b->m_ebrush==::draw2d::e_brush_solid){fill_rectangle(r,b->m_color);return;}
 m_bshape.Clear();m_bBeginFigure=true;_set(r);paint_shape(true,b,1,false);
}


void graphics::fill_rectangle(const ::f64_rectangle &r,const ::color::color &c)
{
	
	graphics_lock lock(target_bitmap());
	
	_001ColorSelect(c,alpha_mode()==::draw2d::e_alpha_mode_blend);
	
	lock.m_pbitmap->m_pbview->FillRect(as_brect(r));
	
}


void graphics::draw_rectangle(const ::f64_rectangle &r){draw_rectangle(r,m_pdraw2dpen);}
void graphics::draw_rectangle(const ::f64_rectangle &r,::draw2d::pen *p){
 if(!p || p->m_epen==::draw2d::e_pen_null)return;
 graphics_lock lock(target_bitmap());_001ColorSelect(p->m_color,alpha_mode()==::draw2d::e_alpha_mode_blend);
 lock.m_pbitmap->m_pbview->SetPenSize(p->m_dWidth);lock.m_pbitmap->m_pbview->StrokeRect(as_brect(r));
}

	void graphics::fill_ellipse(const ::f64_rectangle &r)
	{
 
		fprintf(stderr,
			"ELLIPSE target=%g,%g origin=%g,%g rect=%g,%g,%g,%g\n",
			m_pointTarget.x,m_pointTarget.y,m_pointBitmapOrigin.x,m_pointBitmapOrigin.y,r.left,r.top,r.right,r.bottom);
		
		auto *pbrush=m_pdraw2dbrush.m_p;
		
		if(!pbrush || pbrush->m_ebrush==::draw2d::e_brush_null)
			return;
 
		if(pbrush->m_ebrush!=::draw2d::e_brush_solid)
		{
			
			m_bshape.Clear();
			
			m_bBeginFigure=true;
			
			arc_shape(r.left,r.top,r.right,r.bottom,0,2*MATH_PI);
			
			m_bshape.Close();
			
			paint_shape(true,pbrush,1,false);
			
			return;
			
		}
 
		graphics_lock lock(target_bitmap());
		
		_001ColorSelect(b->m_color,alpha_mode()==::draw2d::e_alpha_mode_blend);
 
		auto *view=lock.m_pbitmap->m_pbview;
		
		auto transform=view->Transform();
		
		auto rect=as_brect(r);
		
		BPoint corners[2]={rect.LeftTop(),rect.RightBottom()};
		
		transform.Apply(corners,2);
		
		view->SetTransform(BAffineTransform());
		
		view->FillEllipse(BRect(corners[0],corners[1]));
		
		view->SetTransform(transform);
 
		auto tr=lock.m_pbitmap->m_pbview->Transform();
		
		double tx=0,ty=0;tr.
		
		GetTranslation(&tx,&ty);
		
		fprintf(stderr,"NATIVE ellipse transform=%g,%g color=%d,%d,%d,%d\n",tx,ty,b->m_color.u8_red(),b->m_color.u8_green(),b->m_color.u8_blue(),b->m_color.u8_opacity());

	}
void graphics::draw_ellipse(const ::f64_rectangle &r){
 if(m_pdraw2dpen)fprintf(stderr,"PEN ellipse kind=%d width=%g rgba=%d,%d,%d,%d\n",int(m_pdraw2dpen->m_epen),m_pdraw2dpen->m_dWidth,m_pdraw2dpen->m_color.u8_red(),m_pdraw2dpen->m_color.u8_green(),m_pdraw2dpen->m_color.u8_blue(),m_pdraw2dpen->m_color.u8_opacity());
 auto *p=m_pdraw2dpen.m_p;if(!p || p->m_epen==::draw2d::e_pen_null)return;
 graphics_lock lock(target_bitmap());_001ColorSelect(p->m_color,alpha_mode()==::draw2d::e_alpha_mode_blend);
 auto *view=lock.m_pbitmap->m_pbview;auto transform=view->Transform();auto rect=as_brect(r);BPoint corners[2]={rect.LeftTop(),rect.RightBottom()};transform.Apply(corners,2);view->SetTransform(BAffineTransform());view->SetPenSize(p->m_dWidth);view->StrokeEllipse(BRect(corners[0],corners[1]));view->SetTransform(transform);
 auto *v=lock.m_pbitmap->m_pbview;v->Sync();double tx=0,ty=0;v->Transform().GetTranslation(&tx,&ty);int xx=int(tx+12),yy=int(ty);if(xx>=0 && yy>=0 && xx<lock.m_pbitmap->m_size.cx && yy<lock.m_pbitmap->m_size.cy){auto *bits=static_cast<unsigned char *>(lock.m_pbitmap->m_pbbitmap->Bits());fprintf(stderr,"STROKE pixel at %d,%d = %08x\n",xx,yy,*reinterpret_cast<unsigned *>(bits+yy*lock.m_pbitmap->m_pbbitmap->BytesPerRow()+xx*4));}
}
void graphics::TextOutRaw(double x,double y,const ::scoped_string &text){
 ::string s(text);auto f=font::native_font(m_pwritetextfont,get_dpix());font_height h{};f.GetHeight(&h);
 graphics_lock lock(target_bitmap());_001ColorSelect(m_pdraw2dbrush?m_pdraw2dbrush->m_color : ::argb(255,0,0,0),alpha_mode()==::draw2d::e_alpha_mode_blend);
 auto *v=lock.m_pbitmap->m_pbview;v->SetFont(&f);v->DrawString(s.c_str(),s.size(),BPoint(x,y+h.ascent));
}
::f64_size graphics::get_text_extent(const ::scoped_string &text){

 ::string s(text);auto f=font::native_font(m_pwritetextfont,get_dpix());font_height h{};f.GetHeight(&h);
 return {f.StringWidth(s.c_str(),s.size()),h.ascent+h.descent+h.leading};
}
::f64_size graphics::_get_text_extent(const ::scoped_string &s){return get_text_extent(s);}
void graphics::prepare_path(::draw2d::path *p){
 if(!p)throw ::exception(error_null_pointer);m_bshape.Clear();m_bBeginFigure=true;
 for(auto &item:p->m_itema)if(!::draw2d::graphics::_set(item))throw ::interface_only("Haiku path item pending");
}
void graphics::paint_shape(bool fill,const ::draw2d::brush *b,double width,bool alternate){
 if(!b)throw ::exception(error_null_pointer);graphics_lock lock(target_bitmap());
 _001ColorSelect(b->m_color,alpha_mode()==::draw2d::e_alpha_mode_blend);
 auto *v=lock.m_pbitmap->m_pbview;v->SetFillRule(alternate?B_EVEN_ODD:B_NONZERO);v->SetPenSize(width);
 if(b->m_ebrush==::draw2d::e_brush_solid){if(fill)v->FillShape(&m_bshape);else v->StrokeShape(&m_bshape);}
 else if(b->m_ebrush==::draw2d::e_brush_linear_gradient_point_color){
  BGradientLinear g(BPoint(b->m_point1.x,b->m_point1.y),BPoint(b->m_point2.x,b->m_point2.y));
  g.AddColor(as_rgb_color(b->m_color1),0);g.AddColor(as_rgb_color(b->m_color2),255);
  if(fill)v->FillShape(&m_bshape,g);else v->StrokeShape(&m_bshape,g);
 }else if(b->m_ebrush==::draw2d::e_brush_radial_gradient_color){
  if(b->m_size.cx!=b->m_size.cy)throw ::interface_only("Elliptical gradient pending");
  BGradientRadial g(BPoint(b->m_point.x,b->m_point.y),b->m_size.cx/2);
  g.AddColor(as_rgb_color(b->m_color1),0);g.AddColor(as_rgb_color(b->m_color2),255);
  if(fill)v->FillShape(&m_bshape,g);else v->StrokeShape(&m_bshape,g);
 }else throw ::interface_only("Haiku brush type pending");
}
void graphics::fill(::draw2d::path *p){
 if(!m_pdraw2dbrush || m_pdraw2dbrush->m_ebrush==::draw2d::e_brush_null)return;
 prepare_path(p);paint_shape(true,m_pdraw2dbrush,1,p->m_efillmode==::draw2d::e_fill_mode_alternate);
}
void graphics::draw(::draw2d::path *p){
 auto *pen=m_pdraw2dpen.m_p;if(!pen || pen->m_epen==::draw2d::e_pen_null)return;prepare_path(p);
 if(pen->m_pdraw2dbrush)paint_shape(false,pen->m_pdraw2dbrush,pen->m_dWidth,false);
 else {graphics_lock lock(target_bitmap());_001ColorSelect(pen->m_color,alpha_mode()==::draw2d::e_alpha_mode_blend);
  auto *v=lock.m_pbitmap->m_pbview;v->SetPenSize(pen->m_dWidth);v->StrokeShape(&m_bshape);}
}
void graphics::move_shape(double x,double y){
 BPoint p(x,y);if(m_bBeginFigure || m_bshape.CurrentPosition()!=p)m_bshape.MoveTo(p);m_bBeginFigure=false;
}
void graphics::arc_shape(double l,double t,double r,double b,double start,double extent){
 double cx=(l+r)/2,cy=(t+b)/2,rx=(r-l)/2,ry=(b-t)/2;
 int steps=int(std::ceil(std::fabs(extent)/(MATH_PI/2)));if(steps<1)return;double step=extent/steps;
 move_shape(cx+rx*std::cos(start),cy+ry*std::sin(start));
 for(int i=0;i<steps;i++){
  double a=start+i*step,z=a+step,k=4./3.*std::tan(step/4);
  BPoint points[3]={BPoint(cx+rx*(std::cos(a)-k*std::sin(a)),cy+ry*(std::sin(a)+k*std::cos(a))),BPoint(cx+rx*(std::cos(z)+k*std::sin(z)),cy+ry*(std::sin(z)-k*std::cos(z))),BPoint(cx+rx*std::cos(z),cy+ry*std::sin(z))};
  m_bshape.BezierTo(points);
 }
}
bool graphics::_set(const ::draw2d::enum_item &i){
 if(i==::draw2d::e_item_begin_figure || i==::draw2d::e_item_end_figure)m_bBeginFigure=true;
 else if(i==::draw2d::e_item_close_figure){m_bshape.Close();m_bBeginFigure=true;}else return false;return true;
}
bool graphics::_set(const ::f64_line &v){move_shape(v.m_p1.x,v.m_p1.y);m_bshape.LineTo(BPoint(v.m_p2.x,v.m_p2.y));return true;}
bool graphics::_set(const ::f64_lines &v){for(::collection::index i=1;i<v.get_count();i++){move_shape(v[i-1].x,v[i-1].y);m_bshape.LineTo(BPoint(v[i].x,v[i].y));}return true;}
bool graphics::_set(const ::f64_polygon_base &v){for(::collection::index i=1;i<v.get_count();i++){move_shape(v[i-1].x,v[i-1].y);m_bshape.LineTo(BPoint(v[i].x,v[i].y));}m_bshape.Close();m_bBeginFigure=true;return true;}
bool graphics::_set(const ::f64_rectangle &r){
 m_bBeginFigure=true;move_shape(r.left,r.top);m_bshape.LineTo(BPoint(r.right,r.top));m_bshape.LineTo(BPoint(r.right,r.bottom));m_bshape.LineTo(BPoint(r.left,r.bottom));m_bshape.Close();m_bBeginFigure=true;return true;
}
bool graphics::_set(const ::f64_ellipse &r){m_bBeginFigure=true;arc_shape(r.left,r.top,r.right,r.bottom,0,2*MATH_PI);m_bshape.Close();m_bBeginFigure=true;return true;}
bool graphics::_set(const ::f64_arc &r){arc_shape(r.left,r.top,r.right,r.bottom,r.m_angleBeg.radian(),r.m_angleExt.radian());return true;}
::i32 graphics::save_graphics_context(){graphics_lock lock(target_bitmap());lock.m_pbitmap->m_pbview->PushState();return ++lock.m_pbitmap->m_iSavedState;}
void graphics::restore_graphics_context(::i32 state){
 graphics_lock lock(target_bitmap());auto *b=lock.m_pbitmap;
 if(state<1 || state>b->m_iSavedState)throw ::exception(error_bad_argument);
 while(b->m_iSavedState>=state){b->m_pbview->PopState();--b->m_iSavedState;}
}

void graphics::_set(const ::geometry2d::matrix &m)
{

   if(!m_pdraw2dbitmap)
      return;
 
   graphics_lock lock(target_bitmap());
 
   lock.m_pbitmap->m_pbview->SetTransform(BAffineTransform(m.a1,m.a2,m.b1,m.b2,m.c1.x,m.c2);
 
}


void graphics::intersect_clip(const ::f64_rectangle &r){graphics_lock lock(target_bitmap());auto *v=lock.m_pbitmap->m_pbview;auto tr=v->Transform();auto rect=as_brect(r);BPoint pts[4]={rect.LeftTop(),rect.RightTop(),rect.RightBottom(),rect.LeftBottom()};tr.Apply(pts,4);rect=BRect(pts[0],pts[0]);for(int i=1;i<4;i++){rect.left=std::fmin(rect.left,pts[i].x);rect.top=std::fmin(rect.top,pts[i].y);rect.right=std::fmax(rect.right,pts[i].x);rect.bottom=std::fmax(rect.bottom,pts[i].y);}v->SetTransform(BAffineTransform());/* clip diagnostic */v->SetTransform(tr);}
void graphics::reset_clip(){if(!m_pdraw2dbitmap)return;graphics_lock lock(target_bitmap());lock.m_pbitmap->m_pbview->ConstrainClippingRegion(nullptr);}
void graphics::_draw_raw(const ::f64_rectangle &dst,::image::image *src,const ::image::image_drawing_options &o,const ::f64_point &p){_stretch_raw(dst,src,o,::f64_rectangle(p,dst.size()));}
void graphics::_stretch_raw(const ::f64_rectangle &dst,::image::image *src,const ::image::image_drawing_options &o,const ::f64_rectangle &source){
 if(!src)throw ::exception(error_null_pointer);auto b=src->get_bitmap_as_source(this);auto *native=dynamic_cast<bitmap *>(b.m_p);
 if(!native)throw ::exception(error_wrong_state);
 BBitmap copy(native->m_pbbitmap->Bounds(),B_RGBA32);if(copy.InitCheck()!=B_OK)throw ::exception(error_failed);
 {graphics_lock lock(native);native->m_pbview->Sync();
  int w=native->m_size.cx,h=native->m_size.cy;
  for(int y=0;y<h;y++){
   auto *s=static_cast<const uint8_t *>(native->m_pbbitmap->Bits())+y*native->m_pbbitmap->BytesPerRow();
   auto *t=static_cast<uint8_t *>(copy.Bits())+y*copy.BytesPerRow();memory_copy(t,s,w*4);
   for(int x=0;x<w;x++)t[x*4+3]=uint8_t(std::fmax(0.,std::fmin(255.,t[x*4+3]*o.opacity().f64_opacity())));
  }
 }
 graphics_lock lock(target_bitmap());_001ColorSelect(::argb(255,255,255,255),alpha_mode()==::draw2d::e_alpha_mode_blend);
 lock.m_pbitmap->m_pbview->DrawBitmap(&copy,as_brect(source),as_brect(dst),B_FILTER_BITMAP_BILINEAR);
}
}

void draw2d_haiku::graphics::_add_shape(const ::f64_rectangle &r){if(!m_bBuildingClip){m_bshape.Clear();m_bBeginFigure=true;m_bBuildingClip=true;}_set(r);}
void draw2d_haiku::graphics::_add_shape(const ::f64_ellipse &r){if(!m_bBuildingClip){m_bshape.Clear();m_bBeginFigure=true;m_bBuildingClip=true;}_set(r);}
void draw2d_haiku::graphics::_add_shape(const ::f64_polygon_base &r){if(!m_bBuildingClip){m_bshape.Clear();m_bBeginFigure=true;m_bBuildingClip=true;}_set(r);}
void draw2d_haiku::graphics::_intersect_clip(){graphics_lock lock(target_bitmap());/* shape clip diagnostic */m_bshape.Clear();m_bBuildingClip=false;}