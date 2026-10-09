#include "platform.h"
#include "icon.h"
#include <TranslationUtils.h>
#include <DataIO.h>
namespace innate_ui_haiku {
icon::~icon() { delete m_bitmap; }
void icon::_create() {
 BMemoryIO input(m_memory.data(), m_memory.size());
 auto *bitmap = BTranslationUtils::GetBitmap(&input);
 if (!bitmap) throw ::exception(error_failed, "Haiku could not decode dialog icon");
 delete m_bitmap; m_bitmap = bitmap;
}
}
