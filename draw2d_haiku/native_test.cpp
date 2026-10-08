#include "platform.h"
#include "bitmap.h"
#include "graphics.h"
#include "image.h"
#include "aura/graphics/image/drawing.h"
#include "font.h"
#include "aura/graphics/draw2d/brush.h"
#include "aura/graphics/draw2d/pen.h"
#include "aura/graphics/draw2d/path.h"
#include "acme/prototype/geometry2d/matrix.h"
#include <Application.h>
#include <stdio.h>
extern "C" void app_simple_application_main_create_system();
extern "C" void haiku_enumerate_font_families(void *,void (*)(void *,const char *));
static int failures=0;
static void test(bool ok,const char *name){printf("%s: %s\n",name,ok?"passed":"FAILED");if(!ok)++failures;}
int main(){
 app_simple_application_main_create_system();
 BApplication app("application/x-vnd.ca2-draw2d-haiku-test");
 draw2d_haiku::bitmap b;b.create_bitmap(nullptr,{8,8});
 draw2d_haiku::graphics g;g.set(&b);g.set_alpha_mode(::draw2d::e_alpha_mode_blend);
 g.fill_rectangle({0.,0.,8.,8.},::argb(128,255,0,0));b.read_pixels();
 auto p=(const uint32_t *)b.m_memoryDraw2dBitmap.data();
 test(p[0]==0x80800000 && p[63]==0x80800000,"Premultiplied alpha readback");
 b.commit_pixels();b.read_pixels();test(p[0]==0x80800000,"Alpha round-trip");
 auto extent=g.get_text_extent("Haiku");test(extent.cx>0 && extent.cy>0,"Native font metrics");
 g.set_alpha_mode(::draw2d::e_alpha_mode_set);g.fill_rectangle({0.,0.,8.,8.},::argb(255,0,0,0));
 auto state=g.save_graphics_context();::geometry2d::matrix m;m.c1=2;m.c2=2;g.set(m);
 g.fill_rectangle({0.,0.,2.,2.},::argb(255,255,255,255));g.restore_graphics_context(state);b.read_pixels();
 test(p[0]==0xff000000 && p[18]==0xffffffff,"Transform and saved state");
 g.fill_rectangle({0.,0.,8.,8.},::argb(255,0,0,0));state=g.save_graphics_context();
 g.intersect_clip({2.,2.,4.,4.});g.fill_rectangle({0.,0.,8.,8.},::argb(255,255,255,255));g.restore_graphics_context(state);b.read_pixels();
 test(p[0]==0xff000000 && p[18]==0xffffffff && p[36]==0xff000000,"Clip rectangle");
 ::draw2d::brush brush;brush.m_ebrush=::draw2d::e_brush_linear_gradient_point_color;
 brush.m_point1={0.,0.};brush.m_point2={8.,0.};brush.m_color1=::argb(255,255,0,0);brush.m_color2=::argb(255,0,0,255);
 g.fill_rectangle({0.,0.,8.,8.},&brush);b.read_pixels();test(p[0]!=p[7],"Native path and gradient");
 draw2d_haiku::bitmap b2;b2.create_bitmap(nullptr,{8,8});draw2d_haiku::graphics g2;g2.set(&b2);
 g2.set_alpha_mode(::draw2d::e_alpha_mode_set);g2.fill_rectangle({0.,0.,8.,8.},::argb(255,255,0,0));
 draw2d_haiku::image source;source.create_as_descriptor({8,8},nullptr);source.m_pdraw2dbitmap=&b2;source.m_bWasMappedAfterLastGraphicsAcquisition=false;
 g.set_alpha_mode(::draw2d::e_alpha_mode_set);g.fill_rectangle({0.,0.,8.,8.},::argb(255,0,0,255));
 g.set_alpha_mode(::draw2d::e_alpha_mode_blend);::image::image_drawing_options options;
 options.opacity(0.5);g._stretch_raw({0.,0.,8.,8.},&source,options,{0.,0.,8.,8.});b.read_pixels();
 test(((p[0]>>16)&255)>=126 && (p[0]&255)>=126,"Bitmap alpha compositing");
 g.set_alpha_mode(::draw2d::e_alpha_mode_set);g.fill_rectangle({0.,0.,8.,8.},::argb(255,0,0,0));
 ::draw2d::brush ellipseBrush;ellipseBrush.create_solid(::argb(255,255,255,255));g.m_pdraw2dbrush=&ellipseBrush;
 state=g.save_graphics_context();m.c1=4;m.c2=4;g.set(m);g.fill_ellipse({0.,0.,3.,3.});g.restore_graphics_context(state);b.read_pixels();
 printf("Ellipse pixels origin=%08x translated=%08x\n",p[9],p[45]);test(p[9]==0xff000000 && p[45]!=0xff000000,"Ellipse child translation");g.m_pdraw2dbrush.release();
 ::draw2d::pen ellipsePen;ellipsePen.create_solid(1.,::argb(255,255,255,255));g.m_pdraw2dpen=&ellipsePen;
 m.c1=0;m.c2=0;g.set(m);g.fill_rectangle({0.,0.,8.,8.},::argb(255,0,0,0));
 state=g.save_graphics_context();m.c1=4;m.c2=4;g.set(m);g.draw_ellipse({0.,0.,3.,3.});g.restore_graphics_context(state);b.read_pixels();
 printf("Stroke pixels origin=%08x translated=%08x\n",p[1],p[37]);test(p[1]==0xff000000 && p[37]!=0xff000000,"Ellipse stroke translation");g.m_pdraw2dpen.release();
 int families=0;haiku_enumerate_font_families(&families,[](void *c,const char *){++*static_cast<int *>(c);});
 test(families>0,"Native font enumeration");printf("Font families: %d\n",families);
 source.m_pdraw2dbitmap.release();g2.m_pdraw2dbitmap.release();
 g.m_pdraw2dbitmap.release(); // b is a stack object.
 return failures?1:0;
}
