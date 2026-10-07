#include "native.h"
#include <Application.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
static int failures=0;
static void test(bool ok,const char *name){printf("%s: %s\n",name,ok?"passed":"FAILED");if(!ok)++failures;}
int main(){
 BApplication app("application/x-vnd.ca2-draw2d-haiku-test");
 void *s=haiku_draw_surface_create(8,8);uint32_t p[64]{};
 test(s && haiku_draw_fill(s,0,0,0,8,8,0x80ff0000,1) && haiku_draw_surface_read(s,p,32) && p[0]==0x80800000 && p[63]==0x80800000,"Premultiplied alpha readback");
 test(haiku_draw_surface_write(s,p,32) && haiku_draw_surface_read(s,p,32) && p[0]==0x80800000,"Alpha round-trip");
 double w=0,h=0;test(haiku_draw_text_size("Haiku",5,nullptr,16,0,0,&w,&h) && w>0 && h>0,"Native font metrics");
 haiku_draw_fill(s,0,0,0,8,8,0xff000000,0);
 int state=haiku_draw_save(s);haiku_draw_transform(s,1,0,0,1,2,2);haiku_draw_fill(s,0,0,0,2,2,0xffffffff,0);haiku_draw_restore(s,state);haiku_draw_surface_read(s,p,32);
 test(p[0]==0xff000000 && p[18]==0xffffffff && p[27]==0xffffffff,"Transform and saved state");
 haiku_draw_fill(s,0,0,0,8,8,0xff000000,0);state=haiku_draw_save(s);haiku_draw_clip(s,2,2,4,4,0);haiku_draw_fill(s,0,0,0,8,8,0xffffffff,0);haiku_draw_restore(s,state);haiku_draw_surface_read(s,p,32);
 test(p[0]==0xff000000 && p[18]==0xffffffff && p[36]==0xff000000,"Clip rectangle");
 void *shape=haiku_draw_shape_create();haiku_draw_shape_line(shape,0,0,8,0);haiku_draw_shape_line(shape,8,0,8,8);haiku_draw_shape_line(shape,8,8,0,8);haiku_draw_shape_close(shape);
 haiku_draw_brush brush{};brush.kind=1;brush.color1=0xffff0000;brush.color2=0xff0000ff;brush.x2=8;
 test(haiku_draw_shape(s,shape,1,&brush,1,0,0) && haiku_draw_surface_read(s,p,32) && p[0]!=p[7],"Native vector path and gradient");
 void *source=haiku_draw_surface_create(8,8);haiku_draw_fill(source,0,0,0,8,8,0xffff0000,0);haiku_draw_fill(s,0,0,0,8,8,0xff0000ff,0);
 test(haiku_draw_blit(s,source,0,0,8,8,0,0,8,8,1,0.5) && haiku_draw_surface_read(s,p,32) && ((p[0]>>16)&255)>=126 && (p[0]&255)>=126,"Bitmap alpha compositing");
 haiku_draw_surface_destroy(source);haiku_draw_shape_destroy(shape);haiku_draw_surface_destroy(s);
 test(haiku_draw_surface_create(0,8)==nullptr,"Invalid bitmap dimensions");return failures?1:0;
}
