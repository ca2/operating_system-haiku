#include "platform.h"
#include "acme/platform/application.h"
#include "acme/platform/system.h"
#include "acme/user/user/interaction.h"
#include "acme/operating_system/window.h"
#include "windowing.h"
namespace haiku::acme::windowing {

static void invoke_procedure(void *p){try{(*static_cast<::procedure *>(p))();}catch(const ::exception &e){warning()<<"Haiku main procedure: "<<e.get_message();}catch(...){warning()<<"Haiku main procedure failed";}}
static void dispose(void *p){delete static_cast<::procedure *>(p);}
void windowing::initialize_windowing(){::string appId=application()->m_strAppId;appId.replace_with(".","/");::string signature="application/x-vnd.ca2."+appId;if(!haiku_app_initialize(signature.c_str()))throw ::exception(error_failed,"Cannot initialize Haiku BApplication");}
void windowing::main_post(const ::procedure &p){if(!haiku_app_post(invoke_procedure,new ::procedure(p),dispose,0))throw ::exception(error_failed);}
void windowing::main_send(const ::procedure &p){if(!haiku_app_post(invoke_procedure,new ::procedure(p),dispose,1))throw ::exception(error_failed);}
void windowing::run(){::set_main_user_thread();initialize_windowing();m_papplication->prepare_application();m_papplication->post_request(nullptr);haiku_app_run();}

}
