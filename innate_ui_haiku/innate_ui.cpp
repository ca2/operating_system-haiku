#include "platform.h"
#include "innate_ui.h"
#include "acme/platform/system.h"
#include "acme/windowing/windowing.h"
namespace innate_ui_haiku {
void innate_ui::main_post(const ::procedure &p) { system()->acme_windowing()->main_post(p); }
void innate_ui::main_send(const ::procedure &p) { system()->acme_windowing()->main_send(p); }
}
