#pragma once
#include "aura/graphics/graphics/double_buffer.h"
namespace windowing_haiku {
class graphics : virtual public ::graphics::double_buffer_graphics {
public:
 void update_screen() override;
 void on_update_screen(::graphics::buffer_item *) override;
};
}
