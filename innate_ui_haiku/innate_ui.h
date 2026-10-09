#pragma once
#include "apex/innate_ui/innate_ui.h"
namespace innate_ui_haiku {
class CLASS_DECL_INNATE_UI_HAIKU innate_ui : virtual public ::innate_ui::innate_ui {
public:
 void main_post(const ::procedure &) override;
 void main_send(const ::procedure &) override;
};
}
