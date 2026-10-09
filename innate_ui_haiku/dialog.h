#pragma once
#include "window.h"
#include "apex/innate_ui/dialog.h"
namespace innate_ui_haiku {
class CLASS_DECL_INNATE_UI_HAIKU dialog : virtual public window, virtual public ::innate_ui::dialog {
public: void create() override { window::create(); }
};
}
