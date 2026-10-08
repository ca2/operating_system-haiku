#pragma once
#include "acme/prototype/geometry2d/rectangle.h"
namespace haiku::acme::windowing {
class monitor {
public:
 int m_iScreen=0;
 ::i32_rectangle m_rectangleNative;
 ::i32_rectangle m_rectangleNativeWorkspace;
 bool update_native_cache();
};
}
