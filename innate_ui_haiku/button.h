#pragma once
#include "window.h"
#include "apex/innate_ui/button.h"
namespace innate_ui_haiku {
class CLASS_DECL_INNATE_UI_HAIKU button : virtual public window, virtual public ::innate_ui::button {
public: BView *new_view() override;
};
}
