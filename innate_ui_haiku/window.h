#pragma once
#include "apex/innate_ui/window.h"
#include <Window.h>
#include <View.h>
namespace innate_ui_haiku {
class CLASS_DECL_INNATE_UI_HAIKU window : virtual public ::innate_ui::window {
public:
 BWindow *m_nativeWindow = nullptr;
 BView *m_nativeView = nullptr;
 string m_text;
 ~window() override;
 virtual BView *new_view();
 void create() override;
 void create_child(::innate_ui::window *) override;
 void destroy_window() override;
 void set_text(const ::scoped_string &) override;
 void show() override;
 void hide() override;
 void show_front(::user::activation_token *) override;
 void center() override;
 void set_position(const ::i32_point &) override;
 void set_size(const ::i32_size &) override;
 void adjust_for_client_size(const ::i32_size &) override;
 ::operating_system::window operating_system_window() const override;
};
}
