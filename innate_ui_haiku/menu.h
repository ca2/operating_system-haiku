#pragma once
#include "_.h"
#include "apex/innate_ui/menu.h"
#include <PopUpMenu.h>
namespace innate_ui_haiku
{

class CLASS_DECL_INNATE_UI_HAIKU menu : virtual public ::innate_ui::menu {
public:
 BPopUpMenu *m_menu;
 BPoint m_screenPoint;
 menu();
 ~menu() override;
 void add_item(const ::scoped_string &, int) override;
 void add_separator() override;
 void set_item_enabled(::i32, bool);
 void set_default_menu_item_command_id(::i32) override;
 void erase_menu_item_by_command_id(::i32) override;
 void track_popup_menu(const ::operating_system::window &, const ::function<void(::i32)> &) override;
};
}
