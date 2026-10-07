#include "native.h"
#include <Bitmap.h>
#include <View.h>
#include <Font.h>
#include <Shape.h>
#include <AffineTransform.h>
#include <GradientLinear.h>
#include <GradientRadial.h>
#include <new>
#include <cstring>
#include <cmath>

namespace {
struct surface {
  BBitmap bitmap;
  BView *view = nullptr;
  int saved=0;
  surface(int w, int h) : bitmap(BRect(0, 0, w-1, h-1), B_BITMAP_ACCEPTS_VIEWS, B_RGBA32) {
    if (bitmap.InitCheck() != B_OK || !bitmap.Lock()) return;
    std::memset(bitmap.Bits(), 0, bitmap.BitsLength());
    view = new BView(bitmap.Bounds(), "ca2-offscreen", B_FOLLOW_NONE, B_WILL_DRAW);
    bitmap.AddChild(view);
    bitmap.Unlock();
  }
};
struct lock {
  surface *s;
  bool ok;
  explicit lock(void *p) : s(static_cast<surface *>(p)), ok(s && s->view && s->bitmap.Lock()) {}
  ~lock() { if(ok) { s->view->Sync(); s->bitmap.Unlock(); } }
};
rgb_color color(uint32_t c) { return {uint8_t(c>>16),uint8_t(c>>8),uint8_t(c),uint8_t(c>>24)}; }
void setup(surface *s, uint32_t c, int blend) {
  s->view->SetHighColor(color(c));
  s->view->SetDrawingMode(blend ? B_OP_ALPHA : B_OP_COPY);
  s->view->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_COMPOSITE);
}
BFont font(const char *family, double size, int bold, int italic) {
  BFont f(be_plain_font);
  if(family && *family) f.SetFamilyAndStyle(family,nullptr);
  f.SetSize(float(size));
  f.SetFace((bold ? B_BOLD_FACE : B_REGULAR_FACE) | (italic ? B_ITALIC_FACE : 0));
  return f;
}
}
extern "C" void *haiku_draw_surface_create(int w,int h) {
  if(w<=0 || h<=0 || w>32767 || h>32767) return nullptr;
  auto *s=new(std::nothrow) surface(w,h);
  if(s && !s->view) { delete s; return nullptr; }
  return s;
}
extern "C" void haiku_draw_surface_destroy(void *p) { delete static_cast<surface *>(p); }
extern "C" int haiku_draw_surface_read(void *p,void *data,int stride) {
  lock l(p); if(!l.ok || !data) return 0;
  l.s->view->Sync();
  int w=int(l.s->bitmap.Bounds().IntegerWidth())+1, h=int(l.s->bitmap.Bounds().IntegerHeight())+1;
  if(stride<w*4) return 0;
  const auto *src=static_cast<const uint8_t *>(l.s->bitmap.Bits());
  auto *dst=static_cast<uint8_t *>(data);
  for(int y=0;y<h;y++) for(int x=0;x<w;x++) {
    const auto *a=src+y*l.s->bitmap.BytesPerRow()+x*4; auto *b=dst+y*stride+x*4;
    b[3]=a[3]; for(int c=0;c<3;c++) b[c]=uint8_t((unsigned(a[c])*a[3]+127)/255);
  }
  return 1;
}
extern "C" int haiku_draw_surface_write(void *p,const void *data,int stride) {
  lock l(p); if(!l.ok || !data) return 0;
  l.s->view->Sync();
  int w=int(l.s->bitmap.Bounds().IntegerWidth())+1,h=int(l.s->bitmap.Bounds().IntegerHeight())+1;
  if(stride<w*4) return 0;
  const auto *src=static_cast<const uint8_t *>(data);auto *dst=static_cast<uint8_t *>(l.s->bitmap.Bits());
  for(int y=0;y<h;y++) for(int x=0;x<w;x++) {
    const auto *a=src+y*stride+x*4;auto *b=dst+y*l.s->bitmap.BytesPerRow()+x*4;
    b[3]=a[3];for(int c=0;c<3;c++) b[c]=a[3] ? uint8_t(std::fmin(255.,(unsigned(a[c])*255+a[3]/2)/a[3])) : 0;
  }
  return 1;
}
extern "C" int haiku_draw_fill(void *p,int ellipse,double a,double b,double c,double d,uint32_t rgba,int blend) {
  lock l(p);if(!l.ok)return 0;setup(l.s,rgba,blend);
  BRect r(a,b,c-1,d-1);if(ellipse)l.s->view->FillEllipse(r);else l.s->view->FillRect(r);return 1;
}
extern "C" int haiku_draw_stroke(void *p,int ellipse,double a,double b,double c,double d,uint32_t rgba,double width,int blend) {
  lock l(p);if(!l.ok)return 0;setup(l.s,rgba,blend);l.s->view->SetPenSize(width);
  BRect r(a,b,c-1,d-1);if(ellipse)l.s->view->StrokeEllipse(r);else l.s->view->StrokeRect(r);return 1;
}
extern "C" int haiku_draw_line(void *p,double a,double b,double c,double d,uint32_t rgba,double width,int blend) {
  lock l(p);if(!l.ok)return 0;setup(l.s,rgba,blend);l.s->view->SetPenSize(width);l.s->view->StrokeLine(BPoint(a,b),BPoint(c,d));return 1;
}
extern "C" int haiku_draw_text(void *p,const char *text,int length,double x,double y,const char *family,double size,int bold,int italic,uint32_t rgba,int blend) {
  lock l(p);if(!l.ok || !text || length<0)return 0;setup(l.s,rgba,blend);
  auto f=font(family,size,bold,italic);font_height h{};f.GetHeight(&h);l.s->view->SetFont(&f);
  l.s->view->DrawString(text,length,BPoint(x,y+h.ascent));return 1;
}
extern "C" int haiku_draw_text_size(const char *text,int length,const char *family,double size,int bold,int italic,double *w,double *h) {
  if(!text || length<0 || !w || !h)return 0;auto f=font(family,size,bold,italic);font_height fh{};f.GetHeight(&fh);
  *w=f.StringWidth(text,length);*h=fh.ascent+fh.descent+fh.leading;return 1;
}

namespace {
struct shape { BShape path; bool begin=true; };
void move(shape *s,BPoint p) { if(s->begin || s->path.CurrentPosition()!=p)s->path.MoveTo(p);s->begin=false; }
}
extern "C" void *haiku_draw_shape_create(){return new(std::nothrow) shape;}
extern "C" void haiku_draw_shape_destroy(void *p){delete static_cast<shape *>(p);}
extern "C" void haiku_draw_shape_begin(void *p){static_cast<shape *>(p)->begin=true;}
extern "C" void haiku_draw_shape_close(void *p){auto *s=static_cast<shape *>(p);s->path.Close();s->begin=true;}
extern "C" void haiku_draw_shape_line(void *p,double a,double b,double c,double d){auto *s=static_cast<shape *>(p);move(s,BPoint(a,b));s->path.LineTo(BPoint(c,d));}
extern "C" void haiku_draw_shape_arc(void *p,double l,double t,double r,double b,double start,double extent){
 auto *s=static_cast<shape *>(p);double cx=(l+r)/2,cy=(t+b)/2,rx=(r-l)/2,ry=(b-t)/2;
 int steps=int(std::ceil(std::fabs(extent)/(3.141592653589793/2)));if(steps<1)return;double step=extent/steps;
 move(s,BPoint(cx+rx*std::cos(start),cy+ry*std::sin(start)));
 for(int i=0;i<steps;i++){
  double a=start+i*step,z=a+step,k=4./3.*std::tan(step/4);
  BPoint points[3]={BPoint(cx+rx*(std::cos(a)-k*std::sin(a)),cy+ry*(std::sin(a)+k*std::cos(a))),BPoint(cx+rx*(std::cos(z)+k*std::sin(z)),cy+ry*(std::sin(z)-k*std::cos(z))),BPoint(cx+rx*std::cos(z),cy+ry*std::sin(z))};
  s->path.BezierTo(points);
 }
}
extern "C" int haiku_draw_shape(void *p,void *q,int fill,const haiku_draw_brush *brush,double width,int blend,int alternate){
 lock l(p);if(!l.ok || !q || !brush)return 0;auto *s=static_cast<shape *>(q);setup(l.s,brush->color1,blend);
 l.s->view->SetFillRule(alternate?B_EVEN_ODD:B_NONZERO);l.s->view->SetPenSize(width);
 if(brush->kind==0){if(fill)l.s->view->FillShape(&s->path);else l.s->view->StrokeShape(&s->path);}
 else if(brush->kind==1){BGradientLinear g(BPoint(brush->x1,brush->y1),BPoint(brush->x2,brush->y2));g.AddColor(color(brush->color1),0);g.AddColor(color(brush->color2),255);if(fill)l.s->view->FillShape(&s->path,g);else l.s->view->StrokeShape(&s->path,g);}
 else if(brush->kind==2){BGradientRadial g(BPoint(brush->x1,brush->y1),brush->radius);g.AddColor(color(brush->color1),0);g.AddColor(color(brush->color2),255);if(fill)l.s->view->FillShape(&s->path,g);else l.s->view->StrokeShape(&s->path,g);}
 else return 0;return 1;
}

extern "C" int haiku_draw_save(void *p){lock l(p);if(!l.ok)return 0;l.s->view->PushState();return ++l.s->saved;}
extern "C" int haiku_draw_restore(void *p,int state){lock l(p);if(!l.ok || state<1 || state>l.s->saved)return 0;while(l.s->saved>=state){l.s->view->PopState();--l.s->saved;}return 1;}
extern "C" int haiku_draw_transform(void *p,double a,double b,double c,double d,double x,double y){lock l(p);if(!l.ok)return 0;l.s->view->SetTransform(BAffineTransform(a,b,c,d,x,y));return 1;}
extern "C" int haiku_draw_clip(void *p,double a,double b,double c,double d,int reset){lock l(p);if(!l.ok)return 0;if(reset)l.s->view->ConstrainClippingRegion(nullptr);else l.s->view->ClipToRect(BRect(a,b,c-1,d-1));return 1;}
extern "C" int haiku_draw_blit(void *p,void *q,double dl,double dt,double dr,double db,double sl,double st,double sr,double sb,int blend,double opacity){
 auto *src=static_cast<surface *>(q);if(!src)return 0;
 BBitmap copy(src->bitmap.Bounds(),B_RGBA32);if(copy.InitCheck()!=B_OK)return 0;
 {lock source(q);if(!source.ok)return 0;src->view->Sync();
  int w=src->bitmap.Bounds().IntegerWidth()+1,h=src->bitmap.Bounds().IntegerHeight()+1;
  for(int y=0;y<h;y++){auto *s=static_cast<const uint8_t *>(src->bitmap.Bits())+y*src->bitmap.BytesPerRow();auto *t=static_cast<uint8_t *>(copy.Bits())+y*copy.BytesPerRow();std::memcpy(t,s,w*4);for(int x=0;x<w;x++)t[x*4+3]=uint8_t(std::fmax(0.,std::fmin(255.,t[x*4+3]*opacity)));}
 }
 lock target(p);if(!target.ok)return 0;setup(target.s,0xffffffff,blend);
 target.s->view->DrawBitmap(&copy,BRect(sl,st,sr-1,sb-1),BRect(dl,dt,dr-1,db-1),B_FILTER_BITMAP_BILINEAR);return 1;
}
