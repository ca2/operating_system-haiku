#include "platform.h"
#include "graphics.h"
#include "window.h"
namespace windowing_haiku {
void graphics::update_screen(){if(m_pwindow)m_pwindow->window_update_screen();}
void graphics::on_update_screen(::graphics::buffer_item *item){
 auto *w=dynamic_cast<window *>(m_pwindow.m_p);
 if(w && item)w->present_buffer_item(item);
}
}
