#include "platform.h"
#include "monitor.h"
#include "native.h"
namespace haiku::acme::windowing {
bool monitor::update_native_cache(){
 int r[4]{},w[4]{};
 if(!haiku_screen_info(m_iScreen,r,w))return false;
 m_rectangleNative={r[0],r[1],r[2],r[3]};
 m_rectangleNativeWorkspace={w[0],w[1],w[2],w[3]};return true;
}
}
