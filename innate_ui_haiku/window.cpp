#include "platform.h"
#include "window.h"
#include "button.h"
#include "apex/innate_ui/innate_ui.h"
#include "acme/platform/system.h"
#include "acme/windowing/windowing.h"
#include "acme/operating_system/window.h"
#include <Application.h>
#include <Screen.h>
#include <Message.h>
#include <Control.h>
namespace innate_ui_haiku {
constexpr uint32 clicked = 'c2bt';
class native_window : public BWindow {
public:
 window *owner;
 ::pointer<button> find_button(window *parent, void *address) {
  for (auto &child : parent->m_childa) {
   auto *native = dynamic_cast<window *>(child.m_p);
   if (!native || !native->m_nativeView) continue;
   auto *candidate = dynamic_cast<button *>(native);
   if (candidate == address) return candidate;
   auto nested = find_button(native, address);
   if (nested) return nested;
  }
  return nullptr;
 }
 native_window(window *p) : BWindow(BRect(100,100,499,299), "ca2", B_TITLED_WINDOW,
   B_ASYNCHRONOUS_CONTROLS), owner(p) {}
 bool QuitRequested() override { Hide(); return false; }
 void MessageReceived(BMessage *message) override {
  if (message->what != clicked) { BWindow::MessageReceived(message); return; }
  void *control = nullptr;
  if (message->FindPointer("control", &control) == B_OK && control) {
   // A queued click may outlive a removed control. Only resolve live children.
   auto target = find_button(owner, control);
   if (target) target->main_post([target]() {
    if (target->m_nativeView) target->call_on_click();
   });
  }
 }
 void FrameResized(float width, float height) override {
  BWindow::FrameResized(width, height);
  ::pointer<window> target = owner;
  target->main_post([target]() { target->on_size(); });
 }
};
window::~window() { try { destroy_window(); } catch (...) {} }
BView *window::new_view() {
 return new BView(BRect(0,0,99,29), "ca2-child", B_FOLLOW_NONE, B_WILL_DRAW);
}
void window::create() {
 if (m_nativeWindow) return;
 if (!be_app) throw ::exception(error_wrong_state, "Haiku BApplication must be initialized first");
 m_nativeWindow = new native_window(this);
 m_nativeView = new BView(m_nativeWindow->Bounds(), "ca2-content", B_FOLLOW_ALL, B_WILL_DRAW);
 m_nativeView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
 m_nativeView->SetLowUIColor(B_PANEL_BACKGROUND_COLOR);
 m_nativeView->SetHighUIColor(B_PANEL_TEXT_COLOR);
 m_nativeWindow->AddChild(m_nativeView);
 set_text(m_text);
 // Dialog creation is posted in a temporary callback. Retain the native
 // window after that callback releases its local dialog pointer.
 innate_ui()->add_top_level_window(this);
}
void window::create_child(::innate_ui::window *parent) {
 auto *nativeParent = dynamic_cast<window *>(parent);
 if (!nativeParent || !nativeParent->m_nativeWindow || !nativeParent->m_nativeView)
  throw ::exception(error_wrong_state, "Native Haiku parent window required");
 if (m_nativeView) throw ::exception(error_wrong_state, "Control already created");
 if (!nativeParent->m_nativeWindow->Lock()) throw ::exception(error_failed);
 m_pwindowParent = parent;
 m_nativeWindow = nativeParent->m_nativeWindow;
 m_nativeView = new_view();
 nativeParent->m_nativeView->AddChild(m_nativeView);
 if (auto *control = dynamic_cast<BControl *>(m_nativeView)) control->SetTarget(m_nativeWindow);
 m_nativeWindow->Unlock();
 parent->m_childa.add(this);
}
void window::destroy_window() {
 if (!m_nativeWindow) return;
 if (!m_nativeWindow->Lock()) return;
 if (m_pwindowParent) {
  if (m_nativeView) { m_nativeView->RemoveSelf(); delete m_nativeView; }
  m_nativeView = nullptr;
  m_nativeWindow->Unlock();
  m_nativeWindow = nullptr;
 } else {
  for (auto &child : m_childa) child->destroy_window();
  m_childa.clear();
  auto *native = m_nativeWindow;
  m_nativeWindow = nullptr; m_nativeView = nullptr;
  native->Quit();
 }
}
void window::set_text(const ::scoped_string &text) {
 m_text = text;
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 if (!m_pwindowParent) m_nativeWindow->SetTitle(m_text.c_str());
 else if (auto *control = dynamic_cast<BControl *>(m_nativeView)) control->SetLabel(m_text.c_str());
 m_nativeWindow->Unlock();
}
void window::show() {
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 if (m_pwindowParent) { if (m_nativeView->IsHidden()) m_nativeView->Show(); }
 else if (m_nativeWindow->IsHidden()) m_nativeWindow->Show();
 m_nativeWindow->Unlock();
}
void window::hide() {
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 if (m_pwindowParent) { if (!m_nativeView->IsHidden()) m_nativeView->Hide(); }
 else if (!m_nativeWindow->IsHidden()) m_nativeWindow->Hide();
 m_nativeWindow->Unlock();
}
void window::show_front(::user::activation_token *) {
 show();
 if (m_nativeWindow && m_nativeWindow->Lock()) { m_nativeWindow->Activate(); m_nativeWindow->Unlock(); }
}
void window::center() {
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 auto screen = BScreen(m_nativeWindow).Frame(); auto frame = m_nativeWindow->Frame();
 m_nativeWindow->MoveTo(screen.left+(screen.Width()-frame.Width())/2,
                       screen.top+(screen.Height()-frame.Height())/2);
 m_nativeWindow->Unlock();
}
void window::set_position(const ::i32_point &p) {
 m_pointWindow = p;
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 if (m_pwindowParent) m_nativeView->MoveTo(p.x,p.y); else m_nativeWindow->MoveTo(p.x,p.y);
 m_nativeWindow->Unlock();
}
void window::set_size(const ::i32_size &s) {
 if (s.cx <= 0 || s.cy <= 0) return;
 m_sizeWindow = s;
 if (!m_nativeWindow || !m_nativeWindow->Lock()) return;
 if (m_pwindowParent) m_nativeView->ResizeTo(s.cx-1,s.cy-1); else m_nativeWindow->ResizeTo(s.cx-1,s.cy-1);
 m_nativeWindow->Unlock();
}
void window::adjust_for_client_size(const ::i32_size &s) { set_size(s); }
::operating_system::window window::operating_system_window() const {
 return ::operating_system::window(
  ::operating_system::window_opaque_t((::u64)m_nativeWindow, 0, 0), const_cast<window *>(this));
}
}
