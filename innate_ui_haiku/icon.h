#pragma once
#include "_.h"
#include "apex/innate_ui/icon.h"
#include <Bitmap.h>
namespace innate_ui_haiku {
class CLASS_DECL_INNATE_UI_HAIKU icon : virtual public ::innate_ui::icon {
public:
 BBitmap *m_bitmap = nullptr;
 ~icon() override;
 void _create() override;
};
}
