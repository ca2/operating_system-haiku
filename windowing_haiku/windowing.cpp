#include "platform.h"
#include "windowing.h"
namespace windowing_haiku {
void windowing::initialize_windowing(){::haiku::acme::windowing::windowing::initialize_windowing();}
void windowing::run(){::haiku::acme::windowing::windowing::run();}
void windowing::main_send(const ::procedure &p){::haiku::acme::windowing::windowing::main_send(p);}
void windowing::main_post(const ::procedure &p){::haiku::acme::windowing::windowing::main_post(p);}
}
namespace windowing_haiku {
void windowing::release_mouse_capture(::thread *,::acme::windowing::window *w){if(m_pacmewindowingwindowMouseCapture==w)m_pacmewindowingwindowMouseCapture.release();}
bool windowing::defer_release_mouse_capture(::thread *t,::acme::windowing::window *w){if(m_pacmewindowingwindowMouseCapture!=w)return false;release_mouse_capture(t,w);return true;}
}
