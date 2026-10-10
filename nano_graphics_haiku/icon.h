#pragma once
#include "acme/nano/graphics/icon.h"
#include <Bitmap.h>
namespace nano_graphics_haiku {
class icon : virtual public ::nano::graphics::icon {
public:
  BBitmap *m_bitmap = nullptr;
  ~icon() override;
  ::i32_size size() const override;
  void load_image_file(const ::block &data) override;
};
} // namespace nano_graphics_haiku
