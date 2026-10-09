#include "platform.h"
#include "button.h"
#include <Button.h>
#include <Message.h>
namespace innate_ui_haiku {
BView *button::new_view() {
 auto *message = new BMessage('c2bt');
 message->AddPointer("control", this);
 return new BButton(BRect(0,0,99,29), "ca2-button", m_text.c_str(), message);
}
}
