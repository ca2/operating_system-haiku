#include "platform.h"
#include "haiku_windowing.h"
#include "acme/platform/application.h"
#include "acme/platform/system.h"
#include "acme/user/user/interaction.h"
#include "acme/operating_system/window.h"
namespace haiku::acme::windowing {
static void invoke_procedure(void *p){try{(*static_cast<::procedure *>(p))();}catch(const ::exception &e){warning()<<"Haiku main procedure: "<<e.get_message();}catch(...){warning()<<"Haiku main procedure failed";}}
static void dispose(void *p){delete static_cast<::procedure *>(p);}
void windowing::initialize_windowing(){::string signature="application/x-vnd.ca2."+application()->m_strAppId;signature.replace_with(".","/");if(!haiku_app_initialize(signature.c_str()))throw ::exception(error_failed,"Cannot initialize Haiku BApplication");}
void windowing::main_post(const ::procedure &p){if(!haiku_app_post(invoke_procedure,new ::procedure(p),dispose,0))throw ::exception(error_failed);}
void windowing::main_send(const ::procedure &p){if(!haiku_app_post(invoke_procedure,new ::procedure(p),dispose,1))throw ::exception(error_failed);}
void windowing::run(){initialize_windowing();m_papplication->prepare_application();m_papplication->post_request(nullptr);haiku_app_run();}
void display::_enumerate_monitors(){int x=0,y=0,w=0,h=0;haiku_screen_bounds(&x,&y,&w,&h);_on_monitor(0,{x,y,x+w,y+h},{x,y,x+w,y+h});}
::i32_size display::get_main_screen_size(){int x=0,y=0,w=0,h=0;haiku_screen_bounds(&x,&y,&w,&h);return {w,h};}
window::~window(){if(m_native)haiku_window_destroy(m_native);}
static void dispatch_native_window_event(void *p,const haiku_window_event *e){try{static_cast<window *>(p)->native_event(*e);}catch(const ::exception &error){warning()<<"Haiku window event: "<<error.get_message();}}
void window::_create_window(){auto r=m_rectangle;if(r.is_empty())r={100,100,740,580};::string title=m_pacmeuserinteraction?m_pacmeuserinteraction->get_title():"ca2";m_native=haiku_window_create(this,dispatch_native_window_event,title.c_str(),r.left,r.top,r.width(),r.height());if(!m_native)throw ::exception(error_failed,"Cannot create Haiku BWindow");haiku_window_show(m_native,1);m_nativeVisible=true;}
void window::native_event(const haiku_window_event &e){if(e.kind==1)on_window_size({e.width,e.height});else if(e.kind==2)on_window_position({e.x,e.y});else if(e.kind==3)on_window_close();}
void window::destroy_window(){auto p=m_native;m_native=nullptr;m_nativeVisible=false;haiku_window_destroy(p);}
void window::set_window_text(const ::scoped_string &title){::string s(title);haiku_window_title(m_native,s.c_str());}
::i32_rectangle window::get_window_rectangle(){int x=0,y=0,w=0,h=0;haiku_window_bounds(m_native,&x,&y,&w,&h);return {x,y,x+w,y+h};}
void window::set_position(const ::i32_point &p){auto r=get_window_rectangle();haiku_window_frame(m_native,p.x,p.y,r.width(),r.height());}
void window::set_size(const ::i32_size &s){auto r=get_window_rectangle();haiku_window_frame(m_native,r.left,r.top,s.cx,s.cy);}
void window::set_active_window(){haiku_window_activate(m_native);}
::operating_system::window window::operating_system_window() const{return ::operating_system::window(::operating_system::window_opaque_t((::u64)m_native,0,0),const_cast<window *>(this));}
}
