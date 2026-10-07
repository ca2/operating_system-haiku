#pragma once
#include <stdint.h>
// No Haiku Kit headers here: keep native SDK types out of ca2 translation units.
struct haiku_draw_brush { int kind; uint32_t color1, color2; double x1,y1,x2,y2,radius; };
extern "C" {
void *haiku_draw_shape_create();
void haiku_draw_shape_destroy(void *shape);
void haiku_draw_shape_begin(void *shape);
void haiku_draw_shape_close(void *shape);
void haiku_draw_shape_line(void *shape,double x1,double y1,double x2,double y2);
void haiku_draw_shape_arc(void *shape,double left,double top,double right,double bottom,double start,double extent);
int haiku_draw_shape(void *surface,void *shape,int fill,const haiku_draw_brush *brush,double width,int blend,int alternate);

int haiku_draw_save(void *surface);
int haiku_draw_restore(void *surface,int state);
int haiku_draw_transform(void *surface,double a,double b,double c,double d,double x,double y);
int haiku_draw_clip(void *surface,double left,double top,double right,double bottom,int reset);
int haiku_draw_blit(void *target,void *source,double dl,double dt,double dr,double db,double sl,double st,double sr,double sb,int blend,double opacity);
void *haiku_draw_surface_create(int width, int height);
void haiku_draw_surface_destroy(void *surface);
int haiku_draw_surface_read(void *surface, void *premultiplied_bgra, int stride);
int haiku_draw_surface_write(void *surface, const void *premultiplied_bgra, int stride);
int haiku_draw_fill(void *surface, int ellipse, double left, double top, double right, double bottom, uint32_t argb, int blend);
int haiku_draw_stroke(void *surface, int ellipse, double left, double top, double right, double bottom, uint32_t argb, double width, int blend);
int haiku_draw_line(void *surface, double x1, double y1, double x2, double y2, uint32_t argb, double width, int blend);
int haiku_draw_text(void *surface, const char *text, int length, double x, double y, const char *family, double size, int bold, int italic, uint32_t argb, int blend);
int haiku_draw_text_size(const char *text, int length, const char *family, double size, int bold, int italic, double *width, double *height);
}
