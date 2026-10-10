#include "platform.h"
#include "display.h"
#include "monitor.h"
#include <InterfaceDefs.h>
#include <Message.h>
#include <cstdio>
namespace haiku::acme::windowing {
void display::open_display(){monitor m;m_bDisplayOpened=m.update_native_cache();if(!m_bDisplayOpened)throw ::exception(error_failed,"Cannot open Haiku screen");_enumerate_monitors();}
bool display::is_display_opened() const{return m_bDisplayOpened;}
void display::_enumerate_monitors(){
 m_rectanglea.erase_all();
 for(int i=0;;++i){monitor m;m.m_iScreen=i;if(!m.update_native_cache())break;_on_monitor(i,m.m_rectangleNative,m.m_rectangleNativeWorkspace);}
}
::i32_size display::get_main_screen_size(){monitor m;if(!m.update_native_cache())throw ::exception(error_failed,"Cannot query Haiku screen");return m.m_rectangleNative.size();}

namespace {
// Capture Appearance colors, excluding the deprecated desktop background role.
const color_which theme_colors[] = {
 B_PANEL_BACKGROUND_COLOR, B_PANEL_TEXT_COLOR, B_DOCUMENT_BACKGROUND_COLOR,
 B_DOCUMENT_TEXT_COLOR, B_CONTROL_BACKGROUND_COLOR, B_CONTROL_TEXT_COLOR,
 B_CONTROL_BORDER_COLOR, B_CONTROL_HIGHLIGHT_COLOR, B_CONTROL_MARK_COLOR,
 B_NAVIGATION_BASE_COLOR, B_NAVIGATION_PULSE_COLOR, B_SHINE_COLOR, B_SHADOW_COLOR,
 B_LINK_TEXT_COLOR, B_LINK_HOVER_COLOR, B_LINK_VISITED_COLOR, B_LINK_ACTIVE_COLOR,
 B_MENU_BACKGROUND_COLOR, B_MENU_SELECTED_BACKGROUND_COLOR, B_MENU_ITEM_TEXT_COLOR,
 B_MENU_SELECTED_ITEM_TEXT_COLOR, B_MENU_SELECTED_BORDER_COLOR,
 B_LIST_BACKGROUND_COLOR, B_LIST_SELECTED_BACKGROUND_COLOR, B_LIST_ITEM_TEXT_COLOR,
 B_LIST_SELECTED_ITEM_TEXT_COLOR, B_SCROLL_BAR_THUMB_COLOR,
 B_TOOL_TIP_BACKGROUND_COLOR, B_TOOL_TIP_TEXT_COLOR, B_STATUS_BAR_COLOR,
 B_SUCCESS_COLOR, B_FAILURE_COLOR, B_WINDOW_TAB_COLOR, B_WINDOW_TEXT_COLOR,
 B_WINDOW_INACTIVE_TAB_COLOR, B_WINDOW_INACTIVE_TEXT_COLOR,
 B_WINDOW_BORDER_COLOR, B_WINDOW_INACTIVE_BORDER_COLOR
};
int hex_digit(char c) {
 if(c>='0' && c<='9')return c-'0';
 if(c>='a' && c<='f')return c-'a'+10;
 if(c>='A' && c<='F')return c-'A'+10;
 return -1;
}
}

bool display::is_dark_mode_through_theming(){return true;}
::string display::theming_ui_name(){return "Haiku Appearance";}

::string display::impl_get_desktop_theme()
{
 ::string theme = "haiku-ui-colors-v1:";
 for(auto which : theme_colors) {
  auto color=ui_color(which);
  char encoded[9];
  std::snprintf(encoded,sizeof(encoded),"%02x%02x%02x%02x",color.red,color.green,color.blue,color.alpha);
  theme+=encoded;
 }
 return theme;
}

void display::impl_set_desktop_theme(const ::scoped_string &value)
{
 ::string theme(value);
 if(!theme.begins_eat("haiku-ui-colors-v1:")
    || theme.length()!=sizeof(theme_colors)/sizeof(theme_colors[0])*8)
  throw ::exception(error_bad_argument,"Invalid Haiku Appearance association");
 BMessage colors;
 size_t offset=0;
 for(auto which : theme_colors) {
  unsigned char rgba[4];
  for(int i=0;i<4;++i) {
   int high=hex_digit(theme[offset++]), low=hex_digit(theme[offset++]);
   if(high<0 || low<0)throw ::exception(error_bad_argument,"Invalid Haiku Appearance color");
   rgba[i]=static_cast<unsigned char>(high*16+low);
  }
  rgb_color color={rgba[0],rgba[1],rgba[2],rgba[3]};
  if(colors.AddColor(ui_color_name(which),color)!=B_OK)
   throw ::exception(error_failed,"Cannot restore Haiku Appearance colors");
 }
 // Apply all roles together so applications receive one coherent palette change.
 set_ui_colors(&colors);
}
}
