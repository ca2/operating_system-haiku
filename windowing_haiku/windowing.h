#pragma once
#include "aura/windowing/windowing.h"
#include "acme_windowing_haiku/windowing.h"
namespace windowing_haiku {
class windowing : virtual public ::windowing::windowing,virtual public ::haiku::acme::windowing::windowing {
public:
 void initialize_windowing() override;
 void run() override;
 void release_mouse_capture(::thread *,::acme::windowing::window *) override;
 bool defer_release_mouse_capture(::thread *,::acme::windowing::window *) override;
 void main_send(const ::procedure &p) override;
 void main_post(const ::procedure &p) override;
};
}
