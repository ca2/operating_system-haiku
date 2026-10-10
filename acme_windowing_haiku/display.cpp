#include "platform.h"
#include "display.h"
#include "monitor.h"
#include <InterfaceDefs.h>
#include <Message.h>
#include <cstdio>
#include <FindDirectory.h>
#include <Node.h>
#include <Path.h>
#include <Messenger.h>
#include <Screen.h>
#include <be_apps/Tracker/Background.h>
#include <vector>
#include <string>
#include <fs_attr.h>
#include <DataIO.h>
namespace haiku::acme::windowing {
void display::open_display(){monitor m;m_bDisplayOpened=m.update_native_cache();if(!m_bDisplayOpened)throw ::exception(error_failed,"Cannot open Haiku screen");_enumerate_monitors();}
bool display::is_display_opened() const{return m_bDisplayOpened;}
void display::_enumerate_monitors(){
 m_rectanglea.erase_all();
 for(int i=0;;++i){monitor m;m.m_iScreen=i;if(!m.update_native_cache())break;_on_monitor(i,m.m_rectangleNative,m.m_rectangleNativeWorkspace);}
}
::i32_size display::get_main_screen_size(){monitor m;if(!m.update_native_cache())throw ::exception(error_failed,"Cannot query Haiku screen");return m.m_rectangleNative.size();}

::string display::_get_wallpaper(::collection::index screen)
{
 if(screen != 0) return "";
 BPath desktop;
 if(find_directory(B_DESKTOP_DIRECTORY, &desktop) != B_OK) return "";
 BNode node(desktop.Path());
 attr_info info;
 if(node.GetAttrInfo(B_BACKGROUND_INFO, &info) != B_OK || info.size <= 0) return "";
 std::vector<char> buffer(info.size);
 if(node.ReadAttr(B_BACKGROUND_INFO, B_MESSAGE_TYPE, 0, buffer.data(), buffer.size()) != info.size) return "";
 BMessage background;
 BMemoryIO stream(buffer.data(), buffer.size());
 if(background.Unflatten(&stream) != B_OK) return "";
 for(int32 i = 0;; ++i) {
  int32 workspaces;
  const char *path;
  if(background.FindInt32(B_BACKGROUND_WORKSPACES, i, &workspaces) != B_OK
     && background.FindUInt32(B_BACKGROUND_WORKSPACES, i, (uint32 *)&workspaces) != B_OK) break;
  if((workspaces & (uint32(1) << current_workspace())) != 0
     && background.FindString(B_BACKGROUND_IMAGE, i, &path) == B_OK) return path;
 }
 return "";
}

void display::_set_wallpaper(::collection::index screen, const ::scoped_string &value)
{
 if(screen != 0) throw ::exception(error_bad_argument, "Haiku has one desktop background per workspace");
 ::string path(value);
 BNode image(path.c_str());
 if(image.InitCheck() != B_OK) throw ::exception(error_file_not_found, "Haiku wallpaper file is unavailable");
 BPath desktop;
 if(find_directory(B_DESKTOP_DIRECTORY, &desktop) != B_OK) throw ::exception(error_failed, "Cannot find Haiku Desktop");
 BNode node(desktop.Path());
 BMessage background;
 background.AddString(B_BACKGROUND_IMAGE, path.c_str());
 background.AddInt32(B_BACKGROUND_MODE, B_BACKGROUND_MODE_SCALED);
 background.AddPoint(B_BACKGROUND_ORIGIN, BPoint(0, 0));
 background.AddBool(B_BACKGROUND_ERASE_TEXT, true);
 // Tracker reads this workspace bitmask as B_INT32_TYPE, including -1 for all.
 background.AddInt32(B_BACKGROUND_WORKSPACES, -1);
 std::vector<char> buffer(background.FlattenedSize());
 if(background.Flatten(buffer.data(), buffer.size()) != B_OK
    || node.WriteAttr(B_BACKGROUND_INFO, B_MESSAGE_TYPE, 0, buffer.data(), buffer.size()) != (ssize_t)buffer.size())
  throw ::exception(error_failed, "Cannot save Haiku desktop background");
 BMessenger tracker("application/x-vnd.Be-TRAK");
 BMessage refresh(B_RESTORE_BACKGROUND_IMAGE);
 if(tracker.SendMessage(&refresh, (BHandler *)nullptr, 3000000) != B_OK) throw ::exception(error_failed, "Cannot refresh Haiku desktop background");
}

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

std::string haiku_appearance_name();
namespace haiku::acme::windowing {
::string display::get_desktop_theme_name(){return haiku_appearance_name().c_str();}
}
