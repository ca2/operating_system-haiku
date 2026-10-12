// Windows baseline retained for future Haiku porting.
// // Created by camilo on 2023-04-15 19:31 <3ThomasBorregaardSorensen!!
// #include "platform.h"
// #include "node.h"
// #include "main_window.h"
// #include "application.h"
// #include "acme/constant/user_message.h"
// #include "acme/parallelization/mutex.h"
// #include "acme/_operating_system.h"
// 
// 
// namespace accessibility_windows
// {
// 
// 
//    main_window::main_window()
//    {
// 
//       m_hwnd = nullptr;
// 
//    }
// 
// 
//    main_window::~main_window()
//    {
// 
//    }
// 
// 
//    
//    BOOL hwnd_is_main_window(HWND handle)
//    {
// 
//       return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
// 
//    }
// 
// 
//    struct process_identifier_hwnd
//    {
// 
//       DWORD       m_dwProcessId;
// 
//       HWND        m_hwnd;
// 
//    };
// 
// 
//    BOOL CALLBACK enum_windows_callback(HWND hwnd, LPARAM lParam)
//    {
//       
//       process_identifier_hwnd & processidentifierhwnd = *(process_identifier_hwnd *)lParam;
// 
//       DWORD dwProcessId = 0;
//       
//       ::GetWindowThreadProcessId(hwnd, &dwProcessId);
//    
//       if (processidentifierhwnd.m_dwProcessId != dwProcessId || !hwnd_is_main_window(hwnd))
//       {
//       
//          return TRUE;
// 
//       }
//    
//       processidentifierhwnd.m_hwnd = hwnd;
// 
//       return FALSE;
// 
//    }
// 
// 
//    HWND process_identifier_main_window(DWORD dwProcessId)
//    {
// 
//       process_identifier_hwnd processidentifierhwnd;
//       
//       processidentifierhwnd.m_dwProcessId = dwProcessId;
//       
//       processidentifierhwnd.m_hwnd = 0;
//       
//       EnumWindows(enum_windows_callback, (LPARAM)&processidentifierhwnd);
// 
//       return processidentifierhwnd.m_hwnd;
// 
//    }
// 
// 
//    void main_window::from_application(::accessibility::application * papplication)
//    {
// 
//       auto processidentifier = papplication->m_processidentifier;
// 
//       DWORD dwProcess = (DWORD) processidentifier;
// 
//       m_hwnd = process_identifier_main_window(dwProcess);
// 
//    }
// 
// 
//    bool main_window::is_window()
//    {
// 
//       return ::IsWindow(m_hwnd);
// 
//    }
//    
//    void main_window::post_close()
//    {
// 
//       ::PostMessage(m_hwnd, WM_CLOSE, 0, 0);
// 
//    }
// 
// 
//    void main_window::post_application_exit()
//    {
// 
//       ::PostMessage(m_hwnd, ::user::e_message_application_exit, 0, 0);
// 
//    }
// 
// 
//    ::string main_window::get_window_text(const ::iptr_array & idPath)
//    {
// 
//       ::collection::index i = 0;
// 
//       HWND hwnd = m_hwnd;
// 
//       while (i < idPath.size())
//       {
// 
//          hwnd = ::GetDlgItem(hwnd, (int) idPath[i]);
// 
//          if (!hwnd)
//          {
// 
//             throw ::exception(error_not_found);
// 
//          }
// 
//          i++;
// 
//       }
// 
//       auto operatingsystemwindow = ::as_operating_system_window(hwnd);
// 
//       return ::windows::get_window_text_timeout(operatingsystemwindow);
// 
//    }
// 
// 
//    ::string main_window::indexed_get_window_text(const ::iptr_array & idPath)
//    {
// 
//       ::collection::index i = 0;
// 
//       HWND hwnd = m_hwnd;
// 
//       auto operatingsystemwindow = ::as_operating_system_window(hwnd);
// 
//       while (i < idPath.size())
//       {
// 
//          operatingsystemwindow = ::windows::child_at(operatingsystemwindow, idPath[i]);
// 
//          if (operatingsystemwindow.is_null())
//          {
// 
//             throw ::exception(error_not_found);
// 
//          }
// 
//          i++;
// 
//       }
// 
//       return ::windows::get_window_text_timeout(operatingsystemwindow);
// 
//    }
// 
// 
//    void main_window::activate()
//    {
// 
//       ::SetForegroundWindow(m_hwnd);
// 
//       ::BringWindowToTop(m_hwnd);
// 
//    }
// 
// } // namespace accessibility_windows
// 
// 
// 
// 

#include "platform.h"
#include "main_window.h"
#include "application.h"
#include <Message.h>
#include <Roster.h>
namespace accessibility_haiku {
namespace {
constexpr bigtime_t timeout=500000;
BMessage query(const BMessenger &target, BMessage &request) {
 BMessage reply;
 if(target.SendMessage(&request,&reply,timeout,timeout)!=B_OK)
  throw ::exception(error_failed,"Haiku accessibility scripting request failed");
 int32 error=0;
 if(reply.FindInt32("error",&error)==B_OK && error!=B_OK)
  throw ::exception(error_not_supported,"The Haiku application does not support this scripting property");
 return reply;
}
}
main_window::main_window() {}
main_window::~main_window() {}
void main_window::from_application(::accessibility::application *app) {
 if(!app) throw ::exception(error_bad_argument);
 m_team=(team_id)app->m_processidentifier;
 BMessenger target(nullptr,m_team);
 BMessage request(B_GET_PROPERTY);request.AddSpecifier("Window",int32(0));
 auto reply=query(target,request);
 if(reply.FindMessenger("result",&m_window)!=B_OK)
  throw ::exception(error_not_found,"No scriptable main window was found");
}
bool main_window::is_window() {return m_window.IsValid();}
void main_window::post_close() {
 if(m_window.SendMessage(B_QUIT_REQUESTED)!=B_OK) throw ::exception(error_failed);
}
void main_window::post_application_exit() {
 BMessenger target(nullptr,m_team);
 if(target.SendMessage(B_QUIT_REQUESTED)!=B_OK) throw ::exception(error_failed);
}
void main_window::activate() {
 if(be_roster->ActivateApp(m_team)!=B_OK) throw ::exception(error_failed);
}
::string main_window::get_window_text(const ::iptr_array &path) {
 if(path.has_element()) throw ::exception(error_not_supported,"Windows child-control identifiers have no Haiku mapping yet");
 BMessage request(B_GET_PROPERTY);request.AddSpecifier("Title");
 auto reply=query(m_window,request);const char *title=nullptr;
 if(reply.FindString("result",&title)!=B_OK) throw ::exception(error_not_supported);
 return title;
}
::string main_window::indexed_get_window_text(const ::iptr_array &path) {
 return get_window_text(path);
}
}
