#include "platform.h"
#include "node.h"
#include "acme/exception/not_implemented.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/path_system.h"
#include "acme/filesystem/filesystem/link.h"
#include "apex/platform/node.h"
#include "apex/filesystem/file/set.h"
#include "apex/platform/system.h"
#include "acme/filesystem/filesystem/listing.h"
#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <FindDirectory.h>

void apex_haiku::node::root_ones(::file::listing_base &listing)
{
   auto add_root = [&](::file::path path, const ::scoped_string &title,
                       const ::file::path &physicalPath = {})
   {
      BDirectory folder((physicalPath.has_character() ? physicalPath : path).c_str());
      if (folder.InitCheck() != B_OK) return;
      for (const auto &existing : listing)
         if (existing == path) return;
      path.set_existent_folder();
      listing.insert_at(listing.size(), path);
      listing.m_straTitle.add(title);
   };

   BPath userHome;
   if (find_directory(B_USER_DIRECTORY, &userHome) == B_OK)
   {
      ::file::path home(userHome.Path());
      add_root(home, "Home");
      add_root(home / "Desktop", "Desktop");
      add_root(home / "Documents", "Documents");
      add_root(home / "Downloads", "Downloads");
      // Keep the user-facing protocol roots; directory_context supplies their
      // Haiku-specific physical paths and directory_system creates the folders.
      add_root("image://", "Image", directory()->image());
      add_root("music://", "Music", directory()->music());
      add_root("video://", "Video", directory()->video());
   }
   add_root("/", "File System");

   BVolumeRoster volumes;
   BVolume volume;
   while (volumes.GetNextVolume(&volume) == B_OK)
   {
      if (!volume.IsPersistent()) continue;
      BDirectory root;
      BEntry entry;
      BPath path;
      char name[B_FILE_NAME_LENGTH] = {};
      if (volume.GetRootDirectory(&root) != B_OK || root.GetEntry(&entry) != B_OK
         || entry.GetPath(&path) != B_OK) continue;
      ::file::path volumeRoot(path.Path());
      // Package/system mounts are reachable through File System; keep the
      // convenience list for user locations and additional data volumes.
      if (volumeRoot == "/boot" || volumeRoot == "/boot/system"
         || volumeRoot == (::file::path(userHome.Path()) / "config")) continue;
      if (volume.GetName(name) == B_OK)
         add_root(path.Path(), name);
      else
         add_root(path.Path(), path.Path());
   }
}
#define __BSD_VISIBLE 1
#include <unistd.h>

i32 daemonize_process(const char * _cmd_line, i32 * pprocessId);

#undef USERNAME_LENGTH // mysql one

#include <sys/stat.h>



string empty_get_file_content_type(string)
{

    return "";

}




bool __node_node_pre_init();

bool __node_node_pos_init();

//PFN_GET_FILE_CONTENT_TYPE g_pfnGetFileContentType = &empty_get_file_content_type;
//
//string haiku_get_file_content_type(string strPath)
//{
//
//   return (*g_pfnGetFileContentType)(strPath);
//
//}
//
//void set_get_file_content_type_function(PFN_GET_FILE_CONTENT_TYPE pfnGetFileContentType)
//{
//
//   g_pfnGetFileContentType = pfnGetFileContentType;
//
//}


namespace apex_haiku
{


//    node::node()
//    {
//    }
//
//
//    node::~node()
//    {
//    }
//
//
//
//namespace apex_haiku
//{


   node::node()
   {

      //m_pnodenode = this;

   }


   node::~node()
   {


   }


   string node::get_user_name()
   {

//      WCHAR wsz[1024];
//
//      DWORD dwSize = sizeof(wsz) / sizeof(WCHAR);
//
//      ::GetUserNameW(wsz, &dwSize);
//
//      return string(wsz);

      return "";

   }


   void node::initialize(::particle * pparticle)
   {

      //auto estatus =
      //
      ::acme_haiku::node::initialize(pparticle);

//      if (!estatus)
//      {
//
//         return estatus;
//
//      }

//         if (!__node_node_pre_init())
//         {
//
//            return error_failed;
//
//         }


//         if (!__node_node_pos_init())
//         {
//
//            return error_failed;
//
//         }

      //return estatus;

   }


   void node::_001InitializeShellOpen()
   {

      //ASSERT(m_atomApp == nullptr && m_atomSystemTopic == nullptr); // do once

      //m_atomApp            = ::GlobalAddAtomW(::str::international::utf8_to_unicode(m_strAppName));

      //m_atomSystemTopic    = ::GlobalAddAtomW(L"system");

      //return ::success;

   }


   void node::process_init()
   {

      //defer_initialize_winsock();
      //return success;

   }


//   string node::get_command_line()
//   {
//
//      return acmesystem()->m_papexsystem->get_command_line();
//
//   }


        void node::shutdown(bool bIfPowerOff)
        {
            /*      bool retval = true;
                  HANDLE hToken;
                  TOKEN_PRIVILEGES tkp;
                  if (!OpenProcessToken(GetCurrentProcess(),
                     TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
                     return false;
                  LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
                  tkp.PrivilegeCount = 1;
                  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                  AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES) nullptr, 0);
                  if (bIfPowerOff)
                     retval = ExitWindowsEx(EWX_POWEROFF, 0) != false;
                  else
                     retval = ExitWindowsEx(EWX_SHUTDOWN, 0) != false;

                  //reset the previlages
                  tkp.Privileges[0].Attributes = 0;
                  AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES) nullptr, 0);
                  return retval;*/

            throw not_implemented();
            //return false;

        }


        void node::reboot()
        {
            /*      HANDLE hToken;
                  TOKEN_PRIVILEGES tkp;
                  if (!OpenProcessToken(GetCurrentProcess(),
                     TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
                     return false;
                  if(!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
                  {
                     TRACELASTERROR();
                     return false;
                  }
                  tkp.PrivilegeCount = 1;
                  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                  if(!AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES) nullptr, 0))
                  {
                     TRACELASTERROR();
                     return false;
                  }
                  if (get_last_error() == ERROR_NOT_ALL_ASSIGNED)
                  {
                     return false;
                  }
                  if(!LookupPrivilegeValue(nullptr, SE_REMOTE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
                  {
                     TRACELASTERROR();
                     return false;
                  }
                  tkp.PrivilegeCount = 1;
                  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                  if(!AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES) nullptr, 0))
                  {
                     TRACELASTERROR();
                     return false;
                  }
                  if (get_last_error() == ERROR_NOT_ALL_ASSIGNED)
                  {
                     return false;
                  }


                  if(!WTSShutdownSystem(WTS_CURRENT_SERVER_HANDLE, WTS_WSD_REBOOT))
                  {
                     TRACELASTERROR();
                     return false;
                  }
            //      if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE,
            //      SHTDN_REASON_MAJOR_SOFTWARE | SHTDN_REASON_MINOR_INSTALLATION))
            //      {
            //      ::u32 dwLastError = ::get_last_error();
            //      return false;
            //      }
                  //reset the previlages
            //      tkp.Privileges[0].Attributes = 0;
            //      AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES) nullptr, 0);
            //      return true;
                  throw not_implemented();
                  return false;

               */
            throw not_implemented();
            //return false;
        }


        void node::terminate_processes_by_title(const ::scoped_string & scopedstrName)
        {

            throw not_implemented();

            //return;

            /*      ::u32 dwPid;
                  while(get_pid_by_title(strName, dwPid))
                  {
                     HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                        PROCESS_VM_READ,
                        false, dwPid );
                     TerminateProcess(hProcess, (::u32) -1);
                     Clos_contexteHandle(hProcess);
                     ::EnumWindows((WNDENUMPROC)
                     CKillProcessHelper::TerminateAppEnum,
                     (LPARAM) dwId);
                     // Wait on the handle. If it signals, great.

                     //If it times out, then you kill it.

                     if(WaitForSingleObject(hProcess, 5000)
                     !=WAIT_OBJECT_0)
                     bResult = TerminateProcess(hProcess,0);
                     else
                     bResult = true;
                     Clos_contexteHandle(hProcess);
                     return bResult == true;*/

            //  }
        }


//   bool node::path_pid(::u32 & dwPid, const ::scoped_string & scopedstrName)
//   {
//
//      u32_array dwa;
//      get_all_processes(dwa);
//      for(i32 i = 0; i < dwa.get_count(); i++)
//      {
//         if(get_process_path(dwa[i]).compare_ci(strName) == 0)
//         {
//            dwPid = dwa[i];
//            return true;
//         }
//      }
//      return false;
//   }
//
//
//   bool node::title_pid(::u32 & dwPid,  const ::scoped_string & scopedstrName)
//   {
//
//      u32_array dwa;
//
//      get_all_processes(dwa);
//
//      for(i32 i = 0; i < dwa.get_count(); i++)
//      {
//
//         if(get_process_path(dwa[i]).title().compare_ci(strName) == 0)
//         {
//
//            dwPid = dwa[i];
//
//            return true;
//
//         }
//
//      }
//
//      return false;
//
//   }
//
//
//   ::file::path node::get_process_path(::u32 dwPid)
//   {
//
//      throw not_implemented();
//
//      return "";
//
//   }
//
//
//   void node::get_all_processes(u32_array & dwa )
//   {
//
//      throw not_implemented();
//
//      //return;
//
//      /*
//            dwa.set_size(0);
//            ::u32 cbNeeded = 0;
//            while(cbNeeded == natural(dwa.get_count()))
//            {
//               dwa.set_size(dwa.get_count() + 1024);
//               if(!EnumProcesses(
//                  dwa.get_data(),
//                  (::u32) (dwa.get_count() * sizeof(::u32)),
//                  &cbNeeded))
//               {
//                  return;
//               }
//               dwa.set_size(cbNeeded / sizeof(::u32));
//            }*/
//   }
//
////   string node::get_module_path(HMODULE hmodule)
////   {
////      throw not_implemented();
////      return "";
////      /*
////      string strPath;
////      ::u32 dwSize = 1;
////      while(natural(strPath.get_length() + 1) == dwSize)
////      {
////         dwSize = ::GetModuleFileName(
////            hmodule,
////            strPath.get_string_buffer(dwSize + 1024),
////            (dwSize + 1024));
////         strPath.ReleaseBuffer();
////      }
////      return strPath;*/
////   }


        ::payload node::connection_settings_get_auto_detect()
        {
            //throw not_implemented();
            return false;

            /*
                  registry::Key key1;

                  key1.OpenKey(HKEY_CURRENT_USER, "Software\\Micros_contextoft\\Windows\\CurrentVersion\\Internet Settings\\Connections", false);

                  memory mem;

                  key1.QueryValue("DefaultConnectionSettings", mem);

                  bool bAutoDetect = (((byte *) mem.get_data())[8] & 0x08) != 0;

                  return bAutoDetect;
            */
        }



        ::payload node::connection_settings_get_auto_config_url()
        {

            //throw not_implemented();
            return "";
            /*
                  registry::Key key;

                  key.OpenKey(HKEY_CURRENT_USER, "Software\\Micros_contextoft\\Windows\\CurrentVersion\\Internet Settings", false);

                  string strUrl;

                  key.QueryValue("AutoConfigURL", strUrl);

                  return strUrl;
            */
        }


        void node::local_machine_set_run(const ::scoped_string & scopedstrKey, const ::file::path & pathExecutable, const ::scoped_string & scopedstrArguments, bool bSet)
        {


            throw not_implemented();
//      throw not_implemented();
//      return false;

            /*
                  registry::Key keyKar(HKEY_LOcaL_MACHINE, "SOFTWARE\\Micros_contextoft\\Windows\\CurrentVersion\\Run", true);


                  keyKar.SetValue(pszKey, pszCommand);


                  return true;
            */
        }


        void node::local_machine_set_run_once(const ::scoped_string & scopedstrKey, const ::file::path & pathExecutable, const ::scoped_string & scopedstrArguments, bool bSet)
        {


            throw not_implemented();
            //return false;
            /*    registry::Key keyKar(HKEY_LOcaL_MACHINE, "SOFTWARE\\Micros_contextoft\\Windows\\CurrentVersion\\RunOnce", true);


                  keyKar.SetValue(pszKey, pszCommand);
            */

            ///return false;

        }


        void node::current_user_set_run(const ::scoped_string & scopedstrKey, const ::file::path & pathExecutable, const ::scoped_string & scopedstrArguments, bool bSet)
        {

            throw not_implemented();
            //return false;

            /*
                  registry::Key keyKar(HKEY_CURRENT_USER, "SOFTWARE\\Micros_contextoft\\Windows\\CurrentVersion\\Run", true);


                  keyKar.SetValue(pszKey, pszCommand);
            */

            //return false;

        }


        void node::current_user_set_run_once(const ::scoped_string & scopedstrKey, const ::file::path & pathExecutable, const ::scoped_string & scopedstrArguments, bool bSet)
        {

            throw not_implemented();
            //return false;

            /*
                  registry::Key keyKar(HKEY_CURRENT_USER, "SOFTWARE\\Micros_contextoft\\Windows\\CurrentVersion\\RunOnce", true);


                  keyKar.SetValue(pszKey, pszCommand);

            */
            //return false;

        }


        void node::defer_register_ca2_plugin_for_mozilla()
        {
            //  throw not_implemented();
            //return false;

            /*
                  registry::Key keyPlugins;

                  if(keyPlugins.OpenKey(HKEY_LOcaL_MACHINE, "SOFTWARE\\MozillaPlugins", true))
                  {

                     registry::Key keyPlugin;

                     if(keyPlugin.OpenKey(keyPlugins, "@ca2.cc/npca2", true))
                     {

                        keyPlugin.SetValue("Description", "ca2 plugin for NPAPI");
                        keyPlugin.SetValue("Path", ::apex::get_system()->m_strCa2Module("npca2.dll"));
                        keyPlugin.SetValue("ProductName", "ca2 plugin for NPAPI");
                        keyPlugin.SetValue("Vendor", "ca2 Desenvolvimento de Software Ltda.");
                        keyPlugin.SetValue("Version", get_application()->file_as_string(pcontext->m_papexcontext->dir().ca2("appdata/x86/ca2_build.txt")));

                        registry::Key keyApplicationca2;

                        if(keyApplicationca2.OpenKey(keyPlugin, "application/ca2", true))
                        {

                           keyApplicationca2.SetValue("Description", "ca2 Document");

                        }

                     }

                  }

                  return true;
            */
        }


        void node::file_extension_get_open_with_list_keys(string_array_base & straKey, const ::scoped_string & scopedstrExtension)
        {

            throw not_implemented();
            //return false;

            /*
                  string strExt;

                  strExt = ".";
                  strExt += strExtension;

                  string strOpenWithKey;
                  strOpenWithKey = strExt + "\\OpenWithList";

                  registry::Key key;

                  key.OpenKey(HKEY_CLASSES_ROOT, strOpenWithKey, false);

                  key.EnumKey(straKey);

                  return true;
            */
        }


        void node::file_extension_get_open_with_list_commands(string_array_base & straCommand, const ::scoped_string & scopedstrExtension)
        {

            string_array_base straKey;

            //if(!
            //
            file_extension_get_open_with_list_keys(straKey, scopedstrExtension);
//      {
//
//         //return false;
//
//         throw exception(error_failed);
//
//      }

            //return true;

        }


        void node::file_association_set_default_icon(const ::scoped_string & scopedstrExtension, const ::scoped_string & scopedstrExtensionNamingClass, const ::file::path & pathIconPath)
        {

            throw not_implemented();
            //return false;

            /*
                  string strExtensionNamingClass(pszExtensionNamingClass);

                  registry::Key keyLink3(HKEY_CLASSES_ROOT, strExtensionNamingClass, true);
                  keyLink3.SetValue("DefaultIcon", pszIconPath);


                  return false;
            */
        }


        void node::file_association_set_shell_open_command(const ::scoped_string & scopedstrExtension, const ::scoped_string & scopedstrExtensionNamingClass,  const ::file::path & pathExecutable, const ::scoped_string & scopedstrParam)
        {

            //return false;

            throw not_implemented();

            //return false;

            /*
                  string strExt;

                  strExt = ".";
                  strExt += strExtension;

                  string strExtensionNamingClass(pszExtensionNamingClass);

                  registry::Key key(HKEY_CLASSES_ROOT, strExt, true);
                  key.SetValue(nullptr, strExtensionNamingClass);

                  registry::Key keyLink3(HKEY_CLASSES_ROOT, strExtensionNamingClass + "\\shell", true);
                  keyLink3.SetValue(nullptr, "open");

                  registry::Key keyLink2(keyLink3, "open", true);
                  keyLink2.SetValue(nullptr, "&Abrir");

                  registry::Key keyLink1(keyLink2, "command", true);

                  string strFormat;
                  strFormat.Format("\"%s\" \"%%L\" %s", pszCommand, pszParam);
                  keyLink1.SetValue(nullptr, strFormat);

                  return true;
            */
        }


        void node::file_association_get_shell_open_command(const ::scoped_string & scopedstrExtension, string & strExtensionNamingClass, string & strCommand, string & strParam)
        {

            throw not_implemented();

            //return false;

            /*
                  string strExt;

                  strExt = ".";
                  strExt += strExtension;

                  registry::Key key(HKEY_CLASSES_ROOT, strExt, false);
                  if(!key.QueryValue(nullptr, strExtensionNamingClass))
                     return false;

                  registry::Key keyLink(HKEY_CLASSES_ROOT, strExtensionNamingClass + "\\shell\\open\\command", false);

                  string strFormat;
                  if(keyLink.QueryValue(nullptr, strFormat))
                  {

                     const ::scoped_string & scopedstr = strFormat;

                     try
                     {

                        strCommand = ::str::consume_quoted_value(psz);
                        ::str::consume_spaces(psz);
                        ::str::consume(psz, "\"%L\"");
                        strParam = psz;

                     }
                     catch(...)
                     {
                     }


                  }

                  return true;
            */
        }


//   bool node::open_in_ie(const char * pcsz)
//   {
//
//      throw not_implemented();
//      return false;
//
//      /*    registry reg;
//            string str;
//            string str2;
//            string strCommand;
//            registry::Key key;
//            if(key.OpenKey(HKEY_CLASSES_ROOT, ".html", false))
//            {
//               if(reg.RegQueryValue(key.m_hkey, "", str))
//               {
//                  if(key.OpenKey(HKEY_CLASSES_ROOT, ".html\\shell\\opennew\\command", false))
//                  {
//                     string str;
//                     if(reg.RegQueryValue(HKEY_CLASSES_ROOT, str, str2))
//                     {
//                        string strCommand(str2);
//                        strCommand.replace("%1", pcsz);
//
//                        WinExec(strCommand,e_display_normal);
//                     }
//                  }
//                  else
//                  {
//                     if(key.OpenKey(HKEY_CLASSES_ROOT, str, false))
//                     {
//                        str += "\\shell\\opennew\\command";
//                        if(key.OpenKey(HKEY_CLASSES_ROOT, str, false))
//                        {
//                           if(reg.RegQueryValue(key.m_hkey, "", str2))
//                           {
//                              string strCommand(str2);
//                              strCommand.replace("%1", pcsz);
//
//                              WinExec(strCommand,e_display_normal);
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//
//            return true;
//      */
//
//   }


        void node::enable_service(const ::scoped_string & scopedstrServiceName, const ::scoped_string & scopedstrDisplayName, const ::scoped_string & scopedstrCommand, const ::scoped_string & scopedstrUser, const ::scoped_string & scopedstrPass)
        {

            throw not_implemented();

            //throw not_implemented();
            //return true;

            /*
                  if(papp->m_strAppName.is_empty()
                  || papp->m_strAppName.compare_ci("bergedge") == 0
                  || !papp->is_serviceable())
                     return false;

                  SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);

                  string strcalling = papp->m_strModulePath + " : app=" + papp->m_strAppId + " service usehos_contexttlogin";

                  if(hdlSCM == 0)
                  {
                     //::get_last_error()
                     return false;
                  }

                  SC_HANDLE hdlServ = ::CreateService(
                     hdlSCM,                    // SCManager database
                     "apex-" + papp->m_strAppName,               // name of service
                     "ccwarehouse ca2 account " + papp->m_strAppName,        // service name to display
                     STANDARD_RIGHTS_REQUIRED,  // desired access
                     SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type
                     SERVICE_AUTO_START,      // start type
                     SERVICE_ERROR_NORMAL,      // error control type
                     strcalling,                   // service's binary Path name
                     0,                      // no load ordering group
                     0,                      // no tag identifier
                     0,                      // no dependencies
                     0,                      // LocalSystem account
                     0);                     // no password

                  if (!hdlServ)
                  {
                     Clos_contexteServiceHandle(hdlSCM);
                     //Ret = ::get_last_error();
                     return false;
                  }

                  Clos_contexteServiceHandle(hdlServ);
                  Clos_contexteServiceHandle(hdlSCM);

                  return true;
            */

        }


        void node::disable_service(const ::scoped_string & scopedstrServiceName)
        {

            throw not_implemented();

//      throw not_implemented();
            //return false;

            /*
                  if(papp->m_strAppName.is_empty()
                  || papp->m_strAppName.compare_ci("bergedge") == 0
                  || !papp->is_serviceable())
                     return false;

                  SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

                  if(hdlSCM == 0)
                  {
                     //::get_last_error();
                     return false;
                  }

                  SC_HANDLE hdlServ = ::OpenService(
                     hdlSCM,                    // SCManager database
                     "apex-" + papp->m_strAppName,               // name of service
                     DELETE);                     // no password

                  if (!hdlServ)
                  {
                     // Ret = ::get_last_error();
                     Clos_contexteServiceHandle(hdlSCM);
                     return false;
                  }

                  ::DeleteService(hdlServ);

                  Clos_contexteServiceHandle(hdlServ);

                  Clos_contexteServiceHandle(hdlSCM);

                  return false;
            */
        }


        void node::start_service(const ::scoped_string & scopedstrServiceName)
        {

            throw not_implemented();

            //throw not_implemented();
            //return false;

            /*
                  if(papp->m_strAppName.is_empty()
                  || papp->m_strAppName.compare_ci("bergedge") == 0
                  || !papp->is_serviceable())
                     return false;

                  SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

                  if(hdlSCM == 0)
                  {
                     //::get_last_error();
                     return false;
                  }

                  SC_HANDLE hdlServ = ::OpenService(
                     hdlSCM,                    // SCManager database
                     "apex-" + papp->m_strAppName,               // name of service
                     SERVICE_START);                     // no password


                  if (!hdlServ)
                  {
                     Clos_contexteServiceHandle(hdlSCM);
                     //Ret = ::get_last_error();
                     return false;
                  }

                  bool bOk = StartService(hdlServ, 0, nullptr) != false;

                  Clos_contexteServiceHandle(hdlServ);
                  Clos_contexteServiceHandle(hdlSCM);

                  return bOk != false;
                  */
        }


        void node::stop_service(const ::scoped_string & scopedstrServiceName)
        {

            throw not_implemented();

            //throw not_implemented();
            //return false;

            /*
                  if(papp->m_strAppName.is_empty()
                  || papp->m_strAppName.compare_ci("bergedge") == 0
                  || !papp->is_serviceable())
                     return false;

                  SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

                  if(hdlSCM == 0)
                  {
                     //::get_last_error();
                     return false;
                  }

                  SC_HANDLE hdlServ = ::OpenService(
                     hdlSCM,                    // SCManager database
                     "apex-" + papp->m_strAppName,               // name of service
                     SERVICE_STOP);                     // no password

                  if (!hdlServ)
                  {
                     // Ret = ::get_last_error();
                     Clos_contexteServiceHandle(hdlSCM);
                     return false;
                  }

                  SERVICE_STATUS ss;

                  __memset(&ss, 0, sizeof(ss));

                  bool bOk = ::ControlService(hdlServ, SERVICE_CONTROL_STOP, &ss) != false;

                  ::DeleteService(hdlServ);

                  Clos_contexteServiceHandle(hdlServ);

                  Clos_contexteServiceHandle(hdlSCM);

                  return bOk != false;
            */
        }


        void node::raise_exception( ::u32 dwExceptionCode, ::u32 dwExceptionFlags)
        {

            throw not_implemented();
            return;
            /*
                  RaiseException( dwExceptionCode, dwExceptionFlags, 0, nullptr );
                  */
        }


        bool node::is_remote_session()
        {

//      throw not_implemented();
            return false;
            /*
                  return GetSystemMetrics(SM_REMOTESESSION) != false;
            */
        }


//   void node::post_to_all_threads(const ::id & id, wparam wparam, lparam lparam)
//   {
//
//      return;
//
//   }


//   int node::get_pid()
//   {
//
//      return ::getpid();
//
//   }


        void node::initialize_wallpaper_fileset(::file::set * pfileset, bool bAddSearch)
        {

            if (bAddSearch)
            {

                ::file::path pathBaseFolder;

                pathBaseFolder = "/usr/local/share/backgrounds";

                ::file::path pathFolder;

                string strCurrentDesktop;

                strCurrentDesktop = string(getenv("XDG_CURRENT_DESKTOP")).lowered();

                pathFolder = pathBaseFolder / strCurrentDesktop;

                pfileset->add_search(pathFolder, true);

            }

            //return true;

        }


        //#elif defined(__HAIKU__)
        //            }
        //
        //#elif defined(MACos_context)
        //   //string strDir;
        //   //strDir = pcontext->m_papexcontext->dir().path(getenv("HOME"), "Pictures");
        //   //imagefileset.add_search(strDir);
        //   string strDir;
        //   strDir = "/Library/Desktop Pictures";
        //   imagefileset.add_search(strDir, true);
        //            }
        //
        //#else

        void node::get_default_browser(string & strId, ::file::path & pathParam, string & strParam)
        {

            auto str = this->get_output("/bin/sh -c \"xdg-settings get default-web-browser\"");

            str.trim();

            if(str.case_insensitive_ends(".desktop"))
            {

                ::file::path pathDesktop = "/usr/share/applications";

                pathDesktop /= str;

                if(file_exists(pathDesktop))
                {

                    ::file::path pathTarget;

                    auto pfilelink = path_system()->resolve_link(pathTarget, ::file::e_link_target);

                    pathParam = pfilelink->m_pathTarget;

                }

            }

            //string str = ::apex::get_system()->process().get_output("xdg-settings get default-web-browser");

            if(str.case_insensitive_find_index("chrome") >= 0)
            {

                strId = "chrome";

                if(pathParam.is_empty())
                {

                    pathParam = "google-chrome";

                }

            }
            else
            {

                strId = "firefox";


                pathParam = "firefox";

            }

            //return true;

        }


        void node::file_open(const ::file::path & path, const ::scoped_string & scopedstrParams, const ::file::path & pathFolder)
        {

            string strTarget;

            strTarget = m_papplication->defer_process_path(path);

            if(haiku_can_exec(strTarget))
            {

                //int iPid;

                //daemonize_process(strTarget, &iPid);

                //::system("cd /; setsid \"" + strTarget + "\" </dev/null &>/dev/null");

                //i32 daemonize_process(const char * _cmd_line, i32 * pprocessId)

                // 2018-01-29 call_async("/bin/bash", "-c \"" + strTarget + "\"", strFolder, SW_SHOWDEFAULT, false);

                this->call_async(strTarget, scopedstrParams, pathFolder, e_display_default, false);

//         char * pszCommandLine = strdup(strTarget + " " + strParams);

//         pid_t pid = ::fork();
//
//         if(pid == 0)
//         {
//
//            int iExitCode = ::system(pszCommandLine);
//
//            free(pszCommandLine);
//
//            exit(iExitCode);
//
//         }
//         else if(pid < 0)
//         {
//
//            free(pszCommandLine);
//
//            return false;
//
//         }

                //return true;

            }
            else
            {

                //call_async("/bin/bash", "-i -c 'xdg-open \"" + strTarget + "\"'", strFolder, SW_SHOWDEFAULT, false);

                //::system("nohup /bin/bash -l -c 'xdg-open \"" + strTarget + "\"' > /dev/null 2>&1&");

                //::system("nohup xdg-open \"" + strTarget + "\" > /dev/null 2>&1&");

                this->user_post([this, strTarget]()
                                  {

                                      string strUri = strTarget;

                                      if(!strUri.contains("://"))
                                      {

                                          strUri = "file://" + strUri;

                                      }

                                      string strError;

                                      int iBufferSize = 4096;

                                      char * pszError = strError.get_buffer(iBufferSize);

                                      int iBool = this->os_launch_uri(strUri, pszError, iBufferSize);

                                      strError.release_buffer();

                                      if(!iBool)
                                      {

                                          informationf("Error launching file : \"%s\" , %s", strUri.c_str(), strError.c_str());

                                      }

                                  });

            }

            //return true;

        }


//   void node::list_process(::file::path_array & patha, u32_array & uaPid)
//   {
//
//      ::output_debug_string("haiku::node::list_process");
//
//      ::file::listing listing;
//
//      listing.set_folder_listing("/proc/");
//
//      m_psystem->m_pacmedirectory->enumerate(listing);
//
//      auto psystem = m_psystem;
//
//      auto pnode = psystem->node();
//
//      for(auto & strPid : listing)
//      {
//
//         int iPid = atoi(strPid.title());
//
//         if(iPid > 0)
//         {
//
//            ::file::path path = pnode->module_path_from_pid(iPid);
//
//            if(path.has_char())
//            {
//
//               patha.add(path);
//
//               uaPid.add(iPid);
//
//            }
//
//         }
//
//      }
//
//   }


        bool node::haiku_can_exec(const char *file)
        {

            struct stat st;

            string str(file);

            if(str.case_insensitive_begins_eat("\""))
            {

                character_count iFind = str.find_index("\"");

                if(iFind < 0)
                {

                    return false;

                }

                str = str.left(iFind);

            }
            else if(str.case_insensitive_begins_eat("\'"))
            {

                character_count iFind = str.find_index("\'");

                if(iFind < 0)
                {

                    return false;

                }

                str = str.left(iFind);

            }
            else
            {

                character_count iFind = str.find_index(" ");

                if(iFind > 0)
                {

                    str = str.left(iFind);

                }


            }

            if(str == "sudo")
            {

                return true;

            }

            zero(st);

            if (stat(str, &st) < 0)
            {

                return false;

            }

            if ((st.st_mode & S_IEXEC) != 0)
            {

                string strContentType = this->get_file_content_type(str);

                if(strContentType == "application/x-shellscript")
                {

                    return true;

                }
                else if(strContentType == "application/x-sharedlib")
                {

                    return true;

                }


                return false;

            }

            return false;

        }


} // namespace apex_haiku



