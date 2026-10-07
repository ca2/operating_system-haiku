#include "platform.h"
#include "haiku_window.h"
#include "aura/user/user/interaction.h"
#include "aura/graphics/graphics/buffer_item.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/image_pixmap_lease.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/system.h"
namespace windowing_haiku {
void window::_create_window(){
 auto *ui=user_interaction();if(ui)m_rectangle={ui->const_layout().sketch().origin(),ui->const_layout().sketch().size()};
 ::haiku::acme::windowing::window::_create_window();
 if(ui)ui->send_message(::user::e_message_create,0,0);
 create_graphics_thread();
}
void window::destroy_window(){::haiku::acme::windowing::window::destroy_window();::windowing::window::on_destroy();}
void window::main_send(const ::procedure &p){system()->acme_windowing()->main_send(p);}
void window::main_post(const ::procedure &p){system()->acme_windowing()->main_post(p);}
void window::set_window_text(const ::scoped_string &p){::haiku::acme::windowing::window::set_window_text(p);}
::i32_rectangle window::get_window_rectangle(){return ::haiku::acme::windowing::window::get_window_rectangle();}
void window::set_position(const ::i32_point &p){::haiku::acme::windowing::window::set_position(p);}
void window::set_size(const ::i32_size &p){::haiku::acme::windowing::window::set_size(p);}
void window::set_active_window(){::haiku::acme::windowing::window::set_active_window();}
::operating_system::window window::operating_system_window() const{return ::haiku::acme::windowing::window::operating_system_window();}
bool window::_strict_set_window_position_unlocked(::i32 x,::i32 y,::i32 w,::i32 h,bool noMove,bool noSize){auto r=get_window_rectangle();haiku_window_frame(m_native,noMove?r.left:x,noMove?r.top:y,noSize?r.width():w,noSize?r.height():h);return true;}
void window::window_update_screen(){
 if(!m_native || !m_pgraphicsgraphics)return;
 synchronous_lock graphicsLock(m_pgraphicsgraphics->synchronization(),DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
 auto *item=m_pgraphicsgraphics->get_screen_item();if(!item || !item->m_pimageBufferItem)return;
 synchronous_lock imageLock(item->m_pmutex,DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
 auto pixels=item->m_pimageBufferItem->map();if(!pixels || !pixels->m_pimage32Raw)return;
 haiku_window_present(m_native,pixels->m_pimage32Raw,pixels->m_sizeRaw.cx,pixels->m_sizeRaw.cy,pixels->m_iScan);
}
}
