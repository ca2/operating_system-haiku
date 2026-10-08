#pragma once
#include "acme/windowing/windowing.h"
#include "native.h"
namespace haiku::acme::windowing {
class windowing : virtual public ::acme::windowing::windowing {
public:
 void initialize_windowing() override;
 void run() override;
 void main_post(const ::procedure &) override;
 void main_send(const ::procedure &) override;
};
}
