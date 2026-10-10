#include "platform.h"
#include "icon.h"
#include "acme/prototype/geometry2d/size.h"
#include <DataIO.h>
#include <TranslationUtils.h>
namespace nano_graphics_haiku {
icon::~icon() { delete m_bitmap; }
::i32_size icon::size() const {
  return m_bitmap ? ::i32_size(int(m_bitmap->Bounds().Width() + 1),
                               int(m_bitmap->Bounds().Height() + 1))
                  : ::i32_size();
}
void icon::load_image_file(const ::block &data) {
  BMemoryIO io(data.data(), data.size());
  auto *bitmap = BTranslationUtils::GetBitmap(&io);
  if (!bitmap)
    throw ::exception(error_failed, "Cannot decode Haiku nano icon");
  delete m_bitmap;
  m_bitmap = bitmap;
}
} // namespace nano_graphics_haiku
