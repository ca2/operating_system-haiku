// Porting to OpenIndiana (a "SunOS") by camilo on 2026-09-28 14:00 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "directory_system.h"
#include "file_context.h"
#include "file_system.h"
#include "node.h"
#include "path_system.h"
#include "acme/operating_system/summary.h"
#include "acme/filesystem/filesystem/file_context.h"


//::user::enum_desktop _get_edesktop();


namespace acme_haiku
{


   node::node()
   {


   }


   node::~node()
   {


   }


   void node::initialize(::particle * pparticle)
   {

      //auto estatus =

      ::acme_posix::node::initialize(pparticle);

//      if (!estatus)
//      {
//
//         return estatus;
//
//      }
//
//      return estatus;

   }

//   string node::get_user_name()
//   {
//
//      WCHAR wsz[1024];
//
//      DWORD dwSize = sizeof(wsz) / sizeof(WCHAR);
//
//      ::GetUserNameW(wsz, &dwSize);
//
//      return string(wsz);
//
//   }
//#include "aura/os/haiku/_c.h"
//
//
//   bool node::_os_calc_app_dark_mode()
//   {
//
//      try
//      {
//
//         ::haiku::registry::key key;
//
//         key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");
//
//         ::u32 dw;
//
//         auto estatus = key._get("AppsUseLightTheme", dw);
//
//         if (::failed(estatus))
//         {
//
//            estatus = key._get("SystemUseLightTheme", dw);
//
//            if (::failed(estatus))
//            {
//
//               return false;
//
//            }
//
//         }
//
//         return dw == 0;
//
//      }
//      catch (...)
//      {
//
//         return false;
//
//      }
//
//   }
//
//
//   bool node::_os_calc_system_dark_mode()
//   {
//
//      try
//      {
//
//         ::haiku::registry::key key;
//
//         key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");
//
//         ::u32 dw;
//
//         auto estatus = key._get("SystemUseLightTheme", dw);
//
//         if (::failed(estatus))
//         {
//
//            estatus = key._get("AppsUseLightTheme", dw);
//
//            if (::failed(estatus))
//            {
//
//               return false;
//
//            }
//
//         }
//
//         return dw == 0;
//
//      }
//      catch (...)
//      {
//
//         return false;
//
//      }
//
//   }
//
//
//   ::color::color node::get_default_color(::u64 u)
//   {
//
//      switch (u)
//      {
//      case COLOR_3DFACE:
//         return argb(127, 192, 192, 184);
//      case COLOR_WINDOW:
//         return argb(127, 255, 255, 255);
//      case COLOR_3DLIGHT:
//         return argb(127, 218, 218, 210);
//      case COLOR_3DHIGHLIGHT:
//         return argb(127, 238, 238, 230);
//      case COLOR_3DSHADOW:
//         return argb(127, 138, 138, 130);
//      case COLOR_3DDKSHADOW:
//         return argb(127, 90, 90, 80);
//      default:
//         break;
//      }
//
//      return argb(127, 0, 0, 0);
//
//   }
//
//   
//   void node::set_console_colors(::u32 dwScreenColors, ::u32 dwPopupColors, ::u32 dwWindowAlpha)
//   {
//
//      ::haiku::registry::key key(HKEY_CURRENT_USER, "Console", true);
//
//      key._set("ScreenColors", dwScreenColors);
//      key._set("PopupColors", dwPopupColors);
//      key._set("WindowAlpha", dwWindowAlpha);
//
//   }
//
//
//
//   ::e_status node::set_system_dark_mode1(bool bSet)
//   {
//
//      ::haiku::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);
//
//      ::u32 dwSystemUseLightTheme;
//      if (bSet)
//      {
//         dwSystemUseLightTheme = 0;
//      }
//      else
//      {
//         dwSystemUseLightTheme = 1;
//      }
//
//      key._set("SystemUsesLightTheme", dwSystemUseLightTheme);
//      return ::success;
//
//   }
//
//
//   ::e_status node::set_app_dark_mode1(bool bSet)
//   {
//
//      ::haiku::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);
//
//      ::u32 dwAppsUseLightTheme;
//      if (bSet)
//      {
//         dwAppsUseLightTheme = 0;
//      }
//      else
//      {
//         dwAppsUseLightTheme = 1;
//      }
//
//      key._set("AppsUseLightTheme", dwAppsUseLightTheme);
//
//      return ::success;
//
//   }
//
//   
//   double node::get_time_zone()
//   {
//
//      double dTimeZone = 0.;
//
//#ifdef __HAIKU__
//      {
//         //time_t t = time(nullptr);
//
//         //struct tm *p = localtime(&t);
//
//         DYNAMIC_TIME_ZONE_INFORMATION i = {};
//
//         ::u32 dw = GetDynamicTimeZoneInformation(&i);
//
//         if (dw == TIME_ZONE_ID_STANDARD)
//         {
//
//            dTimeZone = -((double)(i.Bias + i.StandardBias) / 60.0);
//
//         }
//         else if (dw == TIME_ZONE_ID_DAYLIGHT)
//         {
//
//            dTimeZone = -((double)(i.Bias + i.DaylightBias) / 60.0);
//
//         }
//         else
//         {
//
//            dTimeZone = -((double)i.Bias / 60.0);
//
//         }
//
//      }
//#else
//      {
//
//         time_t t = time(nullptr);
//
//         struct tm lt = { 0 };
//
//         localtime_r(&t, &lt);
//
//         //printf("Offset to GMT is %lds.\n", lt.tm_gmtoff);
//
//         //printf("The time zone is '%s'.\n", lt.tm_zone);
//
//         dTimeZone = +((double)lt.tm_gmtoff / (60.0 * 60.0));
//
//      }
//#endif
//
//      return dTimeZone;
//
//   }
//
//
//   ::e_status node::open_folder(::file::path & pathFolder)
//   {
//
//      wstring wstrFolder(pathFolder);
//
//      int i = (int) (iptr) ::ShellExecuteW(nullptr, L"open", wstrFolder, nullptr, nullptr, SW_NORMAL);
//
//      if (i < 32)
//      {
//
//         switch (i)
//         {
//         case 0:
//            //The operating system is out of memory or resources.
//            return error_no_memory;
//         case ERROR_FILE_NOT_FOUND:
//            return error_file_not_found;
//            //The specified file was not found.
//         case ERROR_PATH_NOT_FOUND:
//            return error_path_not_found;
//            //            The specified path was not found.
//         case          ERROR_BAD_FORMAT:
//            return error_bad_format;
//            //The.exe file is invalid(non - Win32.exe or error in.exe image).
//            //case SE_ERR_ACCESSDENIED:
//            //         return error_access_denied;
//            ////The operating system denied access to the specified file.
//            //SE_ERR_ASSOCINCOMPLETE
//            //The file name association is incomplete or invalid.
//            //SE_ERR_DDEBUSY
//            //The DDE transaction could not be completed because other DDE transactions were being processed.
//            //SE_ERR_DDEFAIL
//            //The DDE transaction failed.
//            //SE_ERR_DDETIMEOUT
//            //The DDE transaction could not be completed because the request timed out.
//            //SE_ERR_DLLNOTFOUND
//            //The specified DLL was not found.
//            //SE_ERR_FNF
//            //The specified file was not found.
//            //SE_ERR_NOASSOC
//            //There is no application associated with the given file name extension.This error will also be returned if you attempt to print a file that is not printable.
//            //SE_ERR_OOM
//            //There was not enough memory to complete the operation.
//            //SE_ERR_PNF
//            //The specified path was not found.
//            //SE_ERR_SHARE
//            //A sharing violation occurred.*/
//         default:
//            return error_failed;
//         }
//
//      }
//
//      return ::success;
//
//   }
//
//
//   ::e_status node::register_dll(const ::file::path & pathDll)
//   {
//
//
//      string strPathDll;
//         
//      //#ifdef _DEBUG
//         
//      strPathDll = pathDll;
//         
//      //#else
//      //
//      //   strPathDll = m_psystem->m_pnodedir->matter() / "time" / process_platform_dir_name() /"stage/_desk_tb.dll";
//      //
//      //#endif
//         
//      string strParam;
//         
//      strParam = "/s \"" + strPathDll + "\"";
//         
//      //wstring wstrParam(strParam);
//         
//      //STARTUPINFOW si = {};
//         
//      //si.cb = sizeof(si);
//         
//      //si.wShowWindow = SW_HIDE;
//         
//      //PROCESS_INFORMATION pi = {};
//         
//      WCHAR wszSystem[2048];
//         
//      GetSystemDirectoryW(wszSystem, sizeof(wszSystem) / sizeof(WCHAR));
//         
//      wstring wstrSystem(wszSystem);
//         
//      ::file::path path(wstrSystem);
//         
//      path /= "regsvr32.exe";
//         
//      property_set set;
//         
//      set["privileged"] = true;
//         
//      if (!call_sync(path, strParam, path.folder(), ::e_display_none, 3_min, set))
//      {
//         
//         return false;
//         
//      }
//         
//      //if (CreateProcessW(wstrPath, wstrParam, nullptr, nullptr, false, 0, nullptr, wstrSystem, &si, &pi))
//      //{
//         
//      //   output_debug_string("created");
//         
//      //}
//      //else
//      //{
//         
//      //   output_debug_string("not created");
//         
//      //}
//         
//      //CloseHandle(pi.hProcess);
//         
//      //CloseHandle(pi.htask);
//         
//      return true;
//         
//   }
//
//
//   ::e_status node::start()
//   {
//
//      auto estatus = m_psystem->m_papexsystem->m_papex->thread_initialize(m_psystem->m_papexsystem);
//
//      if (!estatus)
//      {
//
//         return estatus;
//
//      }
//
//      estatus = m_psystem->on_start();
//
//      if (!estatus)
//      {
//
//         return estatus;
//
//      }
//
//      estatus = m_psystem->main();
//
//      if (!estatus)
//      {
//
//         return estatus;
//
//      }
//
//      estatus = m_psystem->inline_term();
//
//      if (!estatus)
//      {
//
//         return estatus;
//
//      }
//
//      return estatus;
//
//
//   }
//
//
//   ::e_status node::get_firefox_installation_info(string& strPathToExe, string& strInstallDirectory)
//   {
//
//#ifdef HAIKU_DESKTOP
//
//      try
//      {
//
//         ::haiku::registry::key key(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mozilla\\Mozilla Firefox");
//
//         string strCurrentVersion;
//
//         key.get("CurrentVersion", strCurrentVersion);
//
//         key.open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mozilla\\Mozilla Firefox\\" + strCurrentVersion + "\\Main");
//
//         key.get("PathToExe", strPathToExe);
//
//         key.get("Install Directory", strInstallDirectory);
//
//      }
//      catch (const ::e_status& estatus)
//      {
//
//         return estatus;
//
//      }
//
//      return ::success;
//
//#else
//
//      return ::error_failed;
//
//#endif
//
//   }
//
//
//   ::e_status node::_001InitializeShellOpen()
//   {
//
//      //ASSERT(m_atomApp == nullptr && m_atomSystemTopic == nullptr); // do once
//
//      //m_atomApp            = ::GlobalAddAtomW(::str::international::utf8_to_unicode(m_strAppName));
//
//      //m_atomSystemTopic    = ::GlobalAddAtomW(L"system");
//
//      return ::success;
//
//   }
//
//
//   ::e_status node::process_init()
//   {
//
//      
//
//      defer_initialize_winsock();
//      return success;
//
//   }
//
//
//   string node::veriwell_multimedia_music_midi_get_default_library_name()
//   {
//
//      return "music_midi_mmsystem";
//
//   }
//
//
//   string node::multimedia_audio_mixer_get_default_library_name()
//   {
//
//      return "audio_mixer_mmsystem";
//
//   }
//
//
//   string node::multimedia_audio_get_default_library_name()
//   {
//
//      string str;
//
//      if (file_exists(m_psystem->m_pnodedir->system() / "config\\system\\audio.txt"))
//      {
//
//         str = file_as_string(m_psystem->m_pnodedir->system() / "config\\system\\audio.txt");
//
//      }
//      else
//      {
//
//         ::file::path strPath;
//
//         strPath = m_psystem->m_pnodedir->appdata() / "audio.txt";
//
//         str = file_as_string(strPath);
//
//      }
//
//      if (str.has_char())
//         return "audio_" + str;
//      else
//         return "audio_mmsystem";
//
//   }
//
//
   // Twitter Automator and Denis Lakic and UpWork contribution
// enzymes: Liveedu.tv, Twitch.tv and Mixer.com streamers and viewers
// Mummi and bilbo!!
// create call to :
//   void node::install_crash_dump_reporting(const string & strModuleNameWithTheExeExtension)
   //{

//      ::haiku::registry::key k;
//
//      string strKey = "SOFTWARE\\Microsoft\\Windows\\Windows Error Reporting\\LocalDumps\\" + strModuleNameWithTheExeExtension;
//
//      if (k._open(HKEY_LOCAL_MACHINE, strKey, true))
//      {
//         ::file::path str = m_psystem->m_pnodedir->system() / "CrashDumps" / strModuleNameWithTheExeExtension;
//         wstring wstr = str;
//         RegSetValueExW(k.m_hkey, L"DumpFolder", 0, REG_EXPAND_SZ, (byte*)wstr.c_str(), ::u32((wcslen(wstr) + 1) * sizeof(wchar_t)));
//         ::u32 dw = 10;
//         RegSetValueExW(k.m_hkey, L"DumpCount", 0, REG_DWORD, (byte*)&dw, sizeof(dw));
//         dw = 2;
//         RegSetValueExW(k.m_hkey, L"DumpType", 0, REG_DWORD, (byte*)&dw, sizeof(dw));
//
//      }
//
//      output_debug_string("test01");

   //}
//
//
//   int g_iMemoryCountersStartable = 0;
//
//   bool node::memcnts()
//   {
//
//      if (g_iMemoryCountersStartable && g_iMemoryCounters < 0)
//      {
//
//         g_iMemoryCounters = file_exists(m_psystem->m_pnodedir->config() / "system/memory_counters.txt") ? 1 : 0;
//
//         if (g_iMemoryCounters)
//         {
//
//            g_pmutexMemoryCounters = new ::mutex(e_create_new, false, "Global\\ca2_memory_counters");
//
//         }
//
//      }
//
//      return g_iMemoryCountersStartable && g_iMemoryCounters;
//
//   }
//
//
//   ::file::path* g_pMemoryCounters = nullptr;
//
//
//   CLASS_DCL_ACME::file::path node::memcnts_base_path()
//   {
//
//      if (g_iMemoryCountersStartable && g_pMemoryCounters == nullptr)
//      {
//
//         g_pMemoryCounters = new ::file::path();
//
//#if defined(_UWP)
//
//         string strBasePath = m_psystem->m_pnodedir->system() / "memory_counters";
//
//#else
//
//         ::file::path strModule = module_path_from_pid(getpid());
//
//         string strBasePath = m_psystem->m_pnodedir->system() / "memory_counters" / strModule.title() / __str(getpid());
//
//#endif
//
//         * g_pMemoryCounters = strBasePath;
//
//      }
//
//      return *g_pMemoryCounters;
//
//   }
//
//


//   ::e_status node::datetime_to_filetime(::filetime_t * pfiletime, const ::datetime::time& time)
//   {
//
//      SYSTEMTIME sysTime;
//
//      sysTime.wYear = (::u16)time.GetYear();
//      sysTime.wMonth = (::u16)time.GetMonth();
//      sysTime.wDay = (::u16)time.GetDay();
//      sysTime.wHour = (::u16)time.GetHour();
//      sysTime.wMinute = (::u16)time.GetMinute();
//      sysTime.wSecond = (::u16)time.GetSecond();
//      sysTime.wMilliseconds = 0;
//
//      // convert system time to local file time
//      FILETIME localTime;
//
//      DWORD dwLastError = ::GetLastError();
//
//      if (!SystemTimeToFileTime((LPSYSTEMTIME)&sysTime, &localTime))
//      {
//
//         DWORD dwLastError = ::GetLastError();
//
//         return last_error_to_status(dwLastError);
//
//      }
//
//      // convert local file time to UTC file time
//      if (!LocalFileTimeToFileTime(&localTime, (FILETIME*)pfiletime))
//      {
//
//         DWORD dwLastError = ::GetLastError();
//
//         return last_error_to_status(dwLastError);
//
//      }
//
//      return ::success;
//
//   }


//   ::e_status node::last_error_to_status(DWORD dwLastError)
//   {
//
//      if (dwLastError == 0)
//      {
//
//         return ::success;
//
//      }
//      else
//      {
//
//         return error_failed;
//
//      }
//
//
//   }


   string node::audio_get_default_implementation_name()
   {

      auto implementation = file()->safe_get_string("appconfig://audio.txt");
      implementation.trim();
      if (implementation == "audio_sunaudio" || implementation == "sunaudio")
         return "sunaudio";
      if (implementation == "audio_oss" || implementation == "oss")
         return "oss";
      if (implementation.has_character())
         warning() << "Unknown appconfig://audio.txt backend: " << implementation;
#ifdef CA2_HAIKU_AUDIO_IMPLEMENTATION
      return CA2_HAIKU_AUDIO_IMPLEMENTATION;
#else
      return "sunaudio";
#endif

   }


   void node::shell_open(const ::file::path & path, const ::scoped_string & strParams, const ::file::path & pathFolder)
   {

      string str(path);

      fork([this, str]()
           {

              ::system("xdg-open \"" + str + "\" & ");

           });

   }


   //::pointer <::operating_system::summary > node::operating_system_summary()
   //{

      //auto psummary = create_newø < ::operating_system::summary >();


      ////::particle::initialize(pparticle);

      //::string strOs;
      //::string strVer;
      ////}

      //// freedesktop.org and systemd
      //if (file()->exists("/etc/os-release"))
      //{

         //auto set = file()->parse_standard_configuration("/etc/os-release");

         //psummary->m_strDistro = set["ID"];
         //psummary->m_strDistroBranch = set["VARIANT_ID"];
         //psummary->m_strDesktopEnvironment = psummary->m_strDistroBranch;
         //psummary->m_strDistroRelease = set["VERSION_ID"];
         //psummary->m_strDistroFamily = set["ID_LIKE"];

         //strsize iDot = psummary->m_strDistroRelease.find_index('.');

         //if(iDot > 0)
         //{

            //psummary->m_strDistroRelease = psummary->m_strDistroRelease.left(iDot);

         //}

         //psummary->m_strDistro.make_lower();
         //psummary->m_strDistroBranch.make_lower();
         //psummary->m_strDesktopEnvironment.make_lower();
         //psummary->m_strDistroRelease.make_lower();
         //psummary->m_strDistroFamily.make_lower();

      //}


      //auto strLowerCaseCurrentDesktop = this->get_environment_variable("XDG_CURRENT_DESKTOP").lowered();

      ////# echo "lower case xdg_current_desktop is $__SYSTEM_LOWER_CASE_CURRENT_DESKTOP"
      //if (strLowerCaseCurrentDesktop.equals("gnome"))
      //{
         ////      if contains
         ////      $__SYSTEM_LOWER_CASE_CURRENT_DESKTOP
         ////      "gnome";
         ////      then
         ////
         ////# echo "lower case xdg_current_desktop contains gnome"

         //psummary->m_strDesktopEnvironment = "gnome";

      //}
      //else if (strLowerCaseCurrentDesktop.equals("kde"))
      //{
         ////      elif
         ////      contains
         ////      $__SYSTEM_LOWER_CASE_CURRENT_DESKTOP
         ////      "kde";
         ////      then
         ////
         ////# echo "lower case xdg_current_desktop contains gnome"

         //psummary->m_strDesktopEnvironment = "kde";

      //}
      //else if (strLowerCaseCurrentDesktop.equals("lxde"))
      //{
         ////      elif
         ////      contains
         ////      $__SYSTEM_LOWER_CASE_CURRENT_DESKTOP
         ////      "lxde";
         ////      then
         ////
         ////# echo "lower case xdg_current_desktop contains lxde"

         //psummary->m_strDesktopEnvironment = "lxde";

      //}

      //psummary->m_strSlashedStore=psummary->m_strDistro + "/" + psummary->m_strDistroBranch + "/" + psummary->m_strDistroRelease;

      //psummary->m_strUnderscoreOperatingSystem = psummary->m_strSlashedStore;

      //psummary->m_strSlashedIntegration = psummary->m_strSlashedStore;

      //psummary->m_strUnderscoreOperatingSystem.find_replace("/", "_");

      //this->set_environment_variable("__SYSTEM_DISTRO", psummary->m_strDistro);
      //this->set_environment_variable("__SYSTEM_DISTRO_FAMILY", psummary->m_strDistroFamily);
      //this->set_environment_variable("__SYSTEM_DISTRO_BRANCH", psummary->m_strDistroBranch);
      //this->set_environment_variable("__SYSTEM_DISTRO_RELEASE", psummary->m_strDistroRelease);
      //this->set_environment_variable("__SYSTEM_DESKTOP_ENVIRONMENT", psummary->m_strDesktopEnvironment);
      //this->set_environment_variable("__SYSTEM_SLASHED_STORE", psummary->m_strSlashedStore);
      //this->set_environment_variable("__SYSTEM_SLASHED_INTEGRATION", psummary->m_strSlashedIntegration);
      //this->set_environment_variable("__SYSTEM_UNDERSCORE_OPERATING_SYSTEM", psummary->m_strUnderscoreOperatingSystem);
      //this->set_environment_variable("__SYSTEM_SUDO_INSTALL", psummary->m_strSudoInstall);
      //this->set_environment_variable("__SYSTEM_TERMINAL", psummary->m_strTerminal);

      //return psummary;

   //}


	::pointer < ::operating_system::summary > node::operating_system_summary()
	{
	
	   auto psummary = create_newø < ::operating_system::summary >();
	
	
	   //
	   // -------------------------------------------------------------------
	   // Kernel identity
	   // -------------------------------------------------------------------
	   //
	
	   auto strUnameSystem = _uname_system();
	   
	   ::string strUnameRelease = _uname_release();
	
	   auto strUnameVersion =
	      this->get_posix_shell_command_output("uname -v");
	
	   auto strSystemArchitecture =
	      this->get_posix_shell_command_output("uname -m");
	
	
	   strUnameSystem.trim();
	
	   strUnameRelease.trim();
	
	   strUnameVersion.trim();

         psummary->m_strKernelVersion = strUnameVersion;
         if (strUnameVersion.case_insensitive_contains("illumos"))
         {
            psummary->m_strKernel = "illumos";
            psummary->m_strKernelName = "illumos";
         }
         else
         {
            psummary->m_strKernel = "haiku";
            psummary->m_strKernelName = strUnameSystem;
         }
	
	   strSystemArchitecture.trim();
	
	
	   //
	   // SunOS is the kernel/platform family.
	   //
	
	   psummary->m_strSystemFamily = "haiku";
	
	   psummary->m_strSystemFamilyName = "SunOS";
	
	
	   //
	   // -------------------------------------------------------------------
	   // First try os-release.
	   //
	   // Some illumos distributions may provide it, and if so it gives us
	   // the same structured information used by the Linux implementation.
	   // -------------------------------------------------------------------
	   //
	
	   if(file_system()->exists("/etc/os-release"))
	   {
	
	      auto set =
	         file()->get_standard_configuration("/etc/os-release");
	
	
	      auto strId = set["ID"].as_string();
	
	      strId.make_lower();
	
	
	      if(strId.has_character())
	      {
	
	         psummary->m_strSystem = strId;
	
	      }
	
	
	      psummary->m_strName =
	         set["PRETTY_NAME"];
	
	      psummary->m_strFriendlyName =
	         set["PRETTY_NAME"];
	
	      psummary->m_strSystemName =
	         set["NAME"];
	
	      psummary->m_strSystemRelease =
	         set["VERSION_ID"];
	
	      psummary->m_strSystemReleaseName =
	         set["VERSION"];
	
	      psummary->m_strSystemBranch =
	         set["VARIANT_ID"];
	
	      psummary->m_strSystemBranchName =
	         set["VARIANT"];
	
	
	   }
	
	
	   //
	   // -------------------------------------------------------------------
	   // Traditional Solaris/illumos release identification.
	   // -------------------------------------------------------------------
	   //
	
	   ::string strEtcRelease;
	
	
	   if(file_system()->exists("/etc/release"))
	   {
	
	      strEtcRelease =
	         file_system()->as_string("/etc/release");
	
	      strEtcRelease.trim();
	
	
	      auto strReleaseLower = strEtcRelease.lowered();
	
	
	      if(strReleaseLower.begins_eat("openindiana"))
	      {
	
	         psummary->m_strSystem = "openindiana";
	
	         psummary->m_strSystemName = "OpenIndiana";
	
	         psummary->m_strSystemFamily = "illumos";
	
	         psummary->m_strSystemFamilyName = "illumos";
	         
	         strReleaseLower.trim();
	
	         if(strReleaseLower.begins_eat("hipster"))
	         {
	
	            psummary->m_strSystemName = "OpenIndiana Hipster";
	
	         }
	         
	         strReleaseLower.trim();
	         
	         psummary->m_strSystemRelease = strReleaseLower.get_word(" ");
	         
	         //~ if(strReleaseLower.contains("hipster"))
	         //~ {
	
	            //~ psummary->m_strSystemBranch = "hipster";
	
	            //~ psummary->m_strSystemBranchName = "Hipster";
	
	         //~ }
	
	      }
	      else if(strReleaseLower.contains("omnios"))
	      {
	
	         psummary->m_strSystem = "omnios";
	
	         psummary->m_strSystemName = "OmniOS";
	
	         psummary->m_strSystemFamily = "illumos";
	
	         psummary->m_strSystemFamilyName = "illumos";
	
	      }
	      else if(strReleaseLower.contains("smartos"))
	      {
	
	         psummary->m_strSystem = "smartos";
	
	         psummary->m_strSystemName = "SmartOS";
	
	         psummary->m_strSystemFamily = "illumos";
	
	         psummary->m_strSystemFamilyName = "illumos";
	
	      }
	      else if(strReleaseLower.contains("tribblix"))
	      {
	
	         psummary->m_strSystem = "tribblix";
	
	         psummary->m_strSystemName = "Tribblix";
	
	         psummary->m_strSystemFamily = "illumos";
	
	         psummary->m_strSystemFamilyName = "illumos";
	
	      }
	      else if(strReleaseLower.contains("illumos"))
	      {
	
	         psummary->m_strSystem = "illumos";
	
	         psummary->m_strSystemName = "illumos";
	
	         psummary->m_strSystemFamily = "illumos";
	
	         psummary->m_strSystemFamilyName = "illumos";
	
	      }
	      else if(strReleaseLower.contains("solaris"))
	      {
	
	         psummary->m_strSystem = "solaris";
	
	         psummary->m_strSystemName = "Oracle Solaris";
	
	         psummary->m_strSystemFamily = "solaris";
	
	         psummary->m_strSystemFamilyName = "Solaris";
	
	      }
	
	   }
	
	
	   //
	   // -------------------------------------------------------------------
	   // Generic SunOS fallback.
	   // -------------------------------------------------------------------
	   //
	
	   if(psummary->m_strSystem.is_empty())
	   {
	
	      psummary->m_strSystem = "haiku";
	
	   }
	
	
	   if(psummary->m_strSystemName.is_empty())
	   {
	
	      psummary->m_strSystemName = strUnameSystem;
	
	   }
	
	
	   if(psummary->m_strSystemRelease.is_empty())
	   {
	
	      psummary->m_strSystemRelease = strUnameRelease;
	
	   }
	
	
	   if(psummary->m_strSystemReleaseName.is_empty())
	   {
	
	      psummary->m_strSystemReleaseName =
	         psummary->m_strSystemRelease;
	
	   }
	
	
	   if(psummary->m_strName.is_empty())
	   {
	
	      if(strEtcRelease.has_character())
	      {
	
	         psummary->m_strName = strEtcRelease;
	
	      }
	      else
	      {
	
	         psummary->m_strName =
	            psummary->m_strSystemName
	            + " "
	            + psummary->m_strSystemReleaseName;
	
	      }
	
	   }
	
	
	   if(psummary->m_strFriendlyName.is_empty())
	   {
	
	      psummary->m_strFriendlyName =
	         psummary->m_strName;
	
	   }
	
	
	   //
	   // uname -v is useful as the SunOS/illumos kernel build/version.
	   //
	
	   //if(psummary->m_strSystemBranch.is_empty())
	   //{
	
	     // psummary->m_strSystemBranch =
	       //  strUnameVersion;
	
	   //}
	
	
	   //~ if(psummary->m_strSystemBranchName.is_empty())
	   //~ {
	
	      //~ psummary->m_strSystemBranchName =
	         //~ psummary->m_strSystemBranch;
	
	   //~ }
	
	
	   //
	   // -------------------------------------------------------------------
	   // Desktop environment / operating ambient
	   // -------------------------------------------------------------------
	   //
	
	   auto strCurrentDesktop =
	      get_environment_variable("XDG_CURRENT_DESKTOP");
	
	   strCurrentDesktop.make_lower();
	
	
	   if(strCurrentDesktop.contains("mate"))
	   {
	
	      psummary->m_strAmbient = "mate";
	
	      psummary->m_strAmbientName = "MATE";
	
	   }
	   else if(strCurrentDesktop.contains("gnome"))
	   {
	
	      psummary->m_strAmbient = "gnome";
	
	      psummary->m_strAmbientName = "GNOME";
	
	   }
	   else if(strCurrentDesktop.contains("kde"))
	   {
	
	      psummary->m_strAmbient = "kde";
	
	      psummary->m_strAmbientName = "KDE";
	
	   }
	   else if(strCurrentDesktop.contains("xfce"))
	   {
	
	      psummary->m_strAmbient = "xfce";
	
	      psummary->m_strAmbientName = "Xfce";
	
	   }
	   else if(strCurrentDesktop.contains("lxqt"))
	   {
	
	      psummary->m_strAmbient = "lxqt";
	
	      psummary->m_strAmbientName = "LXQt";
	
	   }
	   else if(strCurrentDesktop.contains("lxde"))
	   {
	
	      psummary->m_strAmbient = "lxde";
	
	      psummary->m_strAmbientName = "LXDE";
	
	   }
	
	
	   if(psummary->m_strAmbient.is_empty())
	   {
	
	      auto strDesktopSession =
	         get_environment_variable("DESKTOP_SESSION");
	
	      strDesktopSession.make_lower();
	
	
	      if(strDesktopSession.contains("mate"))
	      {
	
	         psummary->m_strAmbient = "mate";
	
	         psummary->m_strAmbientName = "MATE";
	
	      }
	      else if(strDesktopSession.contains("gnome"))
	      {
	
	         psummary->m_strAmbient = "gnome";
	
	         psummary->m_strAmbientName = "GNOME";
	
	      }
	      else if(strDesktopSession.contains("kde"))
	      {
	
	         psummary->m_strAmbient = "kde";
	
	         psummary->m_strAmbientName = "KDE";
	
	      }
	      else if(strDesktopSession.contains("xfce"))
	      {
	
	         psummary->m_strAmbient = "xfce";
	
	         psummary->m_strAmbientName = "Xfce";
	
	      }
	
	   }
	   
	   
	   if(psummary->m_strSystemBranch.is_empty())
	   {
	
	      psummary->m_strSystemBranch = psummary->m_strAmbient;
	
	   }

	
	   if(psummary->m_strSystemBranchName.is_empty())
	   {
	
	      psummary->m_strSystemBranchName = psummary->m_strSystemBranch;
	
	   }
	

	   //
	   // -------------------------------------------------------------------
	   // Package manager
	   // -------------------------------------------------------------------
	   //
	   // Don't assume all illumos distributions use the same userland
	   // package manager.
	   // -------------------------------------------------------------------
	   //
	
	   if(this->has_posix_shell_command("pkg"))
	   {
	
	      //
	      // IPS.
	      //
	      // OpenIndiana and several Solaris/illumos systems use this.
	      //
	
	      if(this->has_posix_shell_command("pfexec"))
	      {
	
	         psummary->m_strSudoInstall =
	            "pfexec pkg install";
	
	      }
	      else if(this->has_posix_shell_command("sudo"))
	      {
	
	         psummary->m_strSudoInstall =
	            "sudo pkg install";
	
	      }
	      else
	      {
	
	         psummary->m_strSudoInstall =
	            "pkg install";
	
	      }
	
	
	      //
	      // IPS packages don't map directly to Linux .deb/.rpm files.
	      //
	      // Leave the extension empty unless ca2 starts producing p5p
	      // archives explicitly.
	      //
	
	      psummary->m_strStandardPackageFileExtension = "";
	
	   }
	   else if(this->has_posix_shell_command("pkgin"))
	   {
	
	      //
	      // pkgsrc/pkgin is common on some illumos systems.
	      //
	
	      if(this->has_posix_shell_command("sudo"))
	      {
	
	         psummary->m_strSudoInstall =
	            "sudo pkgin -y install";
	
	      }
	      else
	      {
	
	         psummary->m_strSudoInstall =
	            "pkgin -y install";
	
	      }
	
	
	      psummary->m_strStandardPackageFileExtension =
	         "tgz";
	
	   }
	
	
	   //
	   // -------------------------------------------------------------------
	   // Architecture
	   // -------------------------------------------------------------------
	   //
	
	   psummary->m_strSystemArchitecture =
	      strSystemArchitecture;
	
	
	   //
	   // Keep this separate from uname -m if you later need a package
	   // architecture such as amd64 versus i86pc.
	   //
	
	   if(strSystemArchitecture.case_insensitive_equals("i86pc"))
	   {
	
	      psummary->m_strPackagePlatform = "amd64";
	
	   }
	   else if(
	      strSystemArchitecture.case_insensitive_equals("amd64")
	      || strSystemArchitecture.case_insensitive_equals("x86_64"))
	   {
	
	      psummary->m_strPackagePlatform = "amd64";
	
	   }
	   else if(
	      strSystemArchitecture.case_insensitive_equals("aarch64"))
	   {
	
	      psummary->m_strPackagePlatform = "aarch64";
	
	   }
	   else
	   {
	
	      psummary->m_strPackagePlatform =
	         strSystemArchitecture;
	
	   }
	
	
	   //
	   // -------------------------------------------------------------------
	   // Terminal
	   // -------------------------------------------------------------------
	   //
	
	   if(psummary->m_strAmbient == "mate")
	   {
	
	      psummary->m_strTerminal = "mate-terminal";
         if (this->has_posix_shell_command("mate-session"))
         {
            try
            {
            auto strVersion = this->get_posix_shell_command_output("LC_ALL=C mate-session --version 2>&1 || true");
            strVersion.trim();
            // mate-session reports its name followed by the desktop version.
            for (::character_count i = 0; i < strVersion.length(); ++i)
            {
               if (strVersion[i] >= '0' && strVersion[i] <= '9')
               {
                  psummary->m_strAmbientVersion = strVersion.substr(i);
                  break;
               }
            }
            }
            catch (...)
            {
               warning("MATE version query failed; continuing without the optional desktop version.");
            }
         }
	
	   }
	   else if(psummary->m_strAmbient == "kde")
	   {
	
	      psummary->m_strTerminal = "konsole";
	
	   }
	   else if(psummary->m_strAmbient == "xfce")
	   {
	
	      psummary->m_strTerminal = "xfce4-terminal";
	
	   }
	   else if(this->has_posix_shell_command("gnome-terminal"))
	   {
	
	      psummary->m_strTerminal = "gnome-terminal";
	
	   }
	   else if(this->has_posix_shell_command("xterm"))
	   {
	
	      psummary->m_strTerminal = "xterm";
	
	   }
	
	
	   //
	   // -------------------------------------------------------------------
	   // Combined system identifier
	   // -------------------------------------------------------------------
	   //
	
	   psummary->m_strSystemAmbientReleaseArchitecture =
	      psummary->m_strSystem
	      + "/"
	      + psummary->m_strSystemBranch
	      + "/"
	      + psummary->m_strSystemRelease
	      + "/"
	      + psummary->m_strSystemArchitecture;
	
	
	   psummary->m_strSystemAmbientReleaseArchitecture.trim("/");
	
	
	   //
	   // -------------------------------------------------------------------
	   // Numeric release
	   // -------------------------------------------------------------------
	   //
	
	   ::string strRelease =
	      psummary->m_strSystemRelease;
	
	
	   ::string_array_base straRelease;
	
	   straRelease.explode(".", strRelease);
	
	
	   if(straRelease.get_size() >= 1)
	   {
	
	      psummary->m_iMajor =
	         ::as_i32(straRelease[0]);
	
	
	      if(straRelease.get_size() >= 2)
	      {
	
	         psummary->m_iMinor =
	            ::as_i32(straRelease[1]);
	
	      }
	
	   }
	
	
	   //
	   // -------------------------------------------------------------------
	   // PATH prefix
	   //
	   // Same ca2 POSIX tool-directory logic as the Linux implementation.
	   // -------------------------------------------------------------------
	   //
	
	   ::string_array_base straPrefixPaths;
	
	
	   ::file::path pathToolFolderBin;
	
	   ::file::path pathToolFolder =
	      path_system()->tool_folder_path();
	
	
	   if(pathToolFolder.has_character())
	   {
	
	      pathToolFolderBin =
	         pathToolFolder / "bin";
	
	   }
	
	
	   ::file::path pathToolPosixBinFolder =
	      pathToolFolder / "posix/bin";
	
	
	   ::file::path pathToolBinArchFolder =
	      pathToolFolder / "bin" / strSystemArchitecture;
	
	
	   ::file::path pathHomeCodeOperatingSystemBin;
	
	   ::file::path pathHome =
	      directory_system()->home();
	
	
	   if(pathHome.has_character())
	   {
	
	      pathHomeCodeOperatingSystemBin =
	         pathHome / "code/operating_system/bin";
	
	   }
	
	
	   ::string strPath =
	      get_environment_variable("PATH");
	
	
	   ::string_array_base straPath;
	
	   straPath.explode(":", strPath);
	
	
	   if(pathToolBinArchFolder.has_character()
	      && !straPath.contains(pathToolBinArchFolder))
	   {
	
	      straPrefixPaths.add(pathToolBinArchFolder);
	
	   }
	
	
	   if(pathToolFolderBin.has_character()
	      && !straPath.contains(pathToolFolderBin))
	   {
	
	      straPrefixPaths.add(pathToolFolderBin);
	
	   }
	
	
	   if(pathToolPosixBinFolder.has_character()
	      && !straPath.contains(pathToolPosixBinFolder))
	   {
	
	      straPrefixPaths.add(pathToolPosixBinFolder);
	
	   }
	
	
	   if(pathHomeCodeOperatingSystemBin.has_character()
	      && !straPath.contains(pathHomeCodeOperatingSystemBin))
	   {
	
	      straPrefixPaths.add(pathHomeCodeOperatingSystemBin);
	
	   }
	
	
	   if(straPrefixPaths.has_element())
	   {
	
	      psummary->m_strPathPrefix =
	         straPrefixPaths.implode(":");
	
	   }
	
	
	   return psummary;
	
	}



} // namespace acme_haiku



