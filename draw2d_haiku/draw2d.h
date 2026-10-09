#pragma once
#include "aura/graphics/draw2d/draw2d.h"
namespace draw2d_haiku {
class draw2d : virtual public ::draw2d::draw2d {
public:
 void initialize(::particle *pparticle) override;
 bool graphics_context_does_full_redraw() override {return true;}
};
}
