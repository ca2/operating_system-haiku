#pragma once
#include "window.h"
#include "apex/innate_ui/still.h"
namespace innate_ui_haiku {
class CLASS_DECL_INNATE_UI_HAIKU still : virtual public window, virtual public ::innate_ui::still {
public:
 BView *new_view() override;
 void set_text(const ::scoped_string &) override;
 void set_font_size(::f64) override;
 void set_font_weight(::i32) override;
 void layout() override;
 void create_icon_still(::innate_ui::window *) override;
};
}
