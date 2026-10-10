// Match ThemeManager's named UI color palettes without applying a theme.
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <InterfaceDefs.h>
#include <Path.h>
#include <regex>
#include <string>

namespace {
struct theme_color { const char *name; color_which role; };
const theme_color colors[] = {
 {"be:c:PanBg", B_PANEL_BACKGROUND_COLOR}, {"be:c:PanTx", B_PANEL_TEXT_COLOR},
 {"be:c:DocBg", B_DOCUMENT_BACKGROUND_COLOR}, {"be:c:DocTx", B_DOCUMENT_TEXT_COLOR},
 {"be:c:CtlBg", B_CONTROL_BACKGROUND_COLOR}, {"be:c:CtlTx", B_CONTROL_TEXT_COLOR},
 {"be:c:CtlBr", B_CONTROL_BORDER_COLOR}, {"be:c:CtlHg", B_CONTROL_HIGHLIGHT_COLOR},
 {"h:c:CtlMk", B_CONTROL_MARK_COLOR}, {"be:c:NavBs", B_NAVIGATION_BASE_COLOR},
 {"be:c:NavPl", B_NAVIGATION_PULSE_COLOR}, {"be:c:Shine", B_SHINE_COLOR},
 {"be:c:Shadow", B_SHADOW_COLOR}, {"be:c:TipBg", B_TOOL_TIP_BACKGROUND_COLOR},
 {"be:c:TipTx", B_TOOL_TIP_TEXT_COLOR}, {"be:c:MenBg", B_MENU_BACKGROUND_COLOR},
 {"be:c:MenSBg", B_MENU_SELECTED_BACKGROUND_COLOR}, {"be:c:MenTx", B_MENU_ITEM_TEXT_COLOR},
 {"be:c:MenSTx", B_MENU_SELECTED_ITEM_TEXT_COLOR}, {"be:c:MenSBr", B_MENU_SELECTED_BORDER_COLOR},
 {"be:c:Success", B_SUCCESS_COLOR}, {"be:c:Failure", B_FAILURE_COLOR},
 {"h:c:WinTabBg", B_WINDOW_TAB_COLOR}, {"h:c:WinTabTx", B_WINDOW_TEXT_COLOR},
 {"h:c:InWinTabBg", B_WINDOW_INACTIVE_TAB_COLOR}, {"h:c:InWinTabTx", B_WINDOW_INACTIVE_TEXT_COLOR},
 {"h:c:WinBr", B_WINDOW_BORDER_COLOR}, {"h:c:InWinBr", B_WINDOW_INACTIVE_BORDER_COLOR},
 {"h:c:LstBg", B_LIST_BACKGROUND_COLOR}, {"h:c:LstSBg", B_LIST_SELECTED_BACKGROUND_COLOR},
 {"h:c:LstTx", B_LIST_ITEM_TEXT_COLOR}, {"h:c:LstSTx", B_LIST_SELECTED_ITEM_TEXT_COLOR},
 {"h:c:SbThb", B_SCROLL_BAR_THUMB_COLOR}
};

int matching_palette(const std::string &text, std::string &name, int *matchingColors=nullptr) {
 static const std::regex nameField(R"theme(^\s*z:theme:name\s*=\s*string\("((?:\\.|[^"\\])*)")theme", std::regex::multiline);
 static const std::regex paletteField(R"theme(^\s*ui_settings\s*=\s*BMessage\([^\n]*\)\s*\{)theme", std::regex::multiline);
 static const std::regex colorField(R"theme(^\s*([a-zA-Z0-9_:]+)\s*=\s*rgb_color\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))theme", std::regex::multiline);
 std::smatch match;
 if (!std::regex_search(text, match, nameField)) return 0;
 name.clear();
 auto encoded = match[1].str();
 for (size_t i=0; i<encoded.size(); ++i) {
  if (encoded[i]=='\\' && i+1<encoded.size()) ++i;
  name += encoded[i];
 }
 if (name.empty() || !std::regex_search(text, match, paletteField)) return 0;
 auto start = size_t(match.position()+match.length());
 auto end = text.find('}', start);
 if (end==std::string::npos) return 0;
 auto palette = text.substr(start, end-start);
 int count=0;
 int matching=0;
 for (const auto &color : colors) {
  bool found=false;
  for (std::sregex_iterator it(palette.begin(),palette.end(),colorField), last; it!=last; ++it) {
   if ((*it)[1].str()!=color.name) continue;
   if (found) return 0;
   found=true;
   unsigned rgba[4];
   for(int c=0;c<4;++c) {
    auto field=(*it)[c+2].str();
    if(field.size()>3)return 0;
    rgba[c]=std::stoul(field);
    if(rgba[c]>255)return 0;
   }
   auto active=ui_color(color.role);
   if(rgba[0]==active.red && rgba[1]==active.green && rgba[2]==active.blue && rgba[3]==active.alpha) ++matching;
   ++count;
  }
 }
 // A tiny partial palette cannot reliably identify a whole appearance.
 if(matchingColors) *matchingColors=matching;
 else if(matching!=count) return 0;
 return count>=6 ? count : 0;
}
}

std::string haiku_appearance_name() {
 std::string bestName;
 int bestCount=0;
 int bestMatching=0;
 bool ambiguous=false;
 const directory_which locations[]={B_SYSTEM_DATA_DIRECTORY, B_SYSTEM_NONPACKAGED_DATA_DIRECTORY, B_USER_DATA_DIRECTORY, B_USER_SETTINGS_DIRECTORY};
 for(auto location:locations) {
  BPath path;
  if(find_directory(location,&path)!=B_OK || path.Append("UIThemes")!=B_OK) continue;
  BDirectory directory(path.Path());
  BEntry entry;
  while(directory.GetNextEntry(&entry)==B_OK) {
   if(!entry.IsDirectory()) continue;
   BPath themePath(&entry);
   themePath.Append("Theme");
   BFile file(themePath.Path(),B_READ_ONLY);
   off_t size;
   if(file.GetSize(&size)!=B_OK || size<=0 || size>1024*1024) continue;
   std::string text(size,'\0');
   if(file.Read(text.data(),size)!=size) continue;
   std::string name;
   int matching=0;
   int count=matching_palette(text,name,&matching);
   if(count==0 || (matching!=count && (count<12 || matching*10<count*9))) continue;
   int comparison=matching*bestCount-bestMatching*count;
   if(bestCount==0 || comparison>0 || (comparison==0 && count>bestCount)) {
    bestCount=count;bestMatching=matching;bestName=name;ambiguous=false;
   }
   else if(comparison==0 && count==bestCount && name!=bestName) ambiguous=true;
  }
 }
 if(bestCount==0 || ambiguous) return "Custom appearance";
 return bestMatching==bestCount ? bestName : "Custom appearance based on " + bestName;
}
