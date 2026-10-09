#include "platform.h"
#include "menu.h"
#include "acme_windowing_haiku/native.h"
#include <MenuItem.h>
#include <Message.h>
namespace innate_ui_haiku {
menu::menu() : m_menu(new BPopUpMenu("Window", false, false)) {
 int x=0,y=0;haiku_mouse_position(&x,&y);m_screenPoint=BPoint(x,y);
}
menu::~menu() { delete m_menu; }
void menu::add_item(const ::scoped_string &text, int id) {
 ::string label(text);
 auto *message=new BMessage('c2mi');message->AddInt32("command",id);
 m_menu->AddItem(new BMenuItem(label.c_str(),message));
}
void menu::add_separator() { m_menu->AddSeparatorItem(); }
void menu::set_item_enabled(::i32 id, bool enabled) {
 for(int32 i=0;i<m_menu->CountItems();++i) {
  auto *item=m_menu->ItemAt(i);int32 command=0;
  if(item->Message() && item->Message()->FindInt32("command",&command)==B_OK && command==id) {
   item->SetEnabled(enabled);return;
  }
 }
}
void menu::set_default_menu_item_command_id(::i32 id) {
 for(int32 i=0;i<m_menu->CountItems();++i) {
  auto *item=m_menu->ItemAt(i);int32 command=0;
  if(item->Message() && item->Message()->FindInt32("command",&command)==B_OK)
   item->SetMarked(command==id);
 }
}
void menu::erase_menu_item_by_command_id(::i32 id) {
 for(int32 i=0;i<m_menu->CountItems();++i) {
  auto *item=m_menu->ItemAt(i);int32 command=0;
  if(item->Message() && item->Message()->FindInt32("command",&command)==B_OK && command==id) {
   m_menu->RemoveItem(item);delete item;return;
  }
 }
}
void menu::track_popup_menu(const ::operating_system::window &, const ::function<void(::i32)> &callback) {
 // Synchronous Go returns the selection; messages are handled by the framework callback.
 auto *item=m_menu->Go(m_screenPoint,false,false,false);
 int32 command=0;
 if(item && item->Message() && item->Message()->FindInt32("command",&command)==B_OK)
  callback(command);
}
}
