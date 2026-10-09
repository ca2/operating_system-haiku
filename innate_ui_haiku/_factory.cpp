#include "platform.h"
#include "window.h"
#include "button.h"
#include "still.h"
#include "dialog.h"
#include "innate_ui.h"
#include "menu.h"
IMPLEMENT_FACTORY(innate_ui_haiku) {
 pfactory->add_factory_item<::innate_ui_haiku::window, ::innate_ui::window>();
 pfactory->add_factory_item<::innate_ui_haiku::button, ::innate_ui::button>();
 pfactory->add_factory_item<::innate_ui_haiku::still, ::innate_ui::still>();
 pfactory->add_factory_item<::innate_ui_haiku::dialog, ::innate_ui::dialog>();
 pfactory->add_factory_item<::innate_ui_haiku::innate_ui, ::innate_ui::innate_ui>();
 pfactory->add_factory_item<::innate_ui_haiku::menu, ::innate_ui::menu>();
}
