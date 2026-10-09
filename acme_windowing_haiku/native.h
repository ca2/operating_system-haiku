#pragma once
#include <stdint.h>
struct haiku_window_event { int kind; int x,y,width,height; char text[64] = {}; };
extern "C" {
int haiku_app_initialize(const char *signature);
int haiku_app_is_main_thread();
void haiku_app_run();
void haiku_app_quit();
int haiku_app_post(void (*call)(void *),void *context,void (*dispose)(void *),int wait);
void *haiku_window_create(void *owner,void (*event)(void *,const haiku_window_event *),const char *title,int x,int y,int width,int height);
void haiku_window_destroy(void *window);
void haiku_window_show(void *window,int show);
void haiku_window_title(void *window,const char *title);
void haiku_window_frame(void *window,int x,int y,int width,int height);
void haiku_window_activate(void *window);
void haiku_window_focus(void *window);
int haiku_window_has_focus(void *window);
void haiku_window_bounds(void *window,int *x,int *y,int *width,int *height);
void haiku_window_present(void *window,const void *premultiplied_bgra,int width,int height,int stride);
void haiku_screen_bounds(int *x,int *y,int *width,int *height);
}
extern "C" void haiku_mouse_position(int *x,int *y);
extern "C" int haiku_screen_info(int index,int *bounds,int *workspace);
extern "C" int haiku_window_is_active(void *window);
