// Create on 2021-03-21 20:00 <3ThomasBS_
#include "platform.h"
#include "acme_directory.h"
#include "acme_file.h"


namespace acme_haiku
{

   
   acme_directory::acme_directory()
   {

      m_pplatformdir = this;

   }


   acme_directory::~acme_directory()
   {


   }


   string acme_directory::dir_root()
   {

      return home() / ".config/ca2";

   }


   ::file::path acme_directory::get_memory_map_base_folder_path() 
   {

      return home() / ".config/ca2/memory_map";

   }


//   ::file::path acme_directory::home()
//   {
//
//      return getenv("HOME");
//
//   }


   ::file::path acme_directory::program_data()
   {

      return home() / "application";

   }


   ::file::path acme_directory::roaming()
   {

      return home() / ".config";

   }


   ::file::path acme_directory::appdata()
   {

      return ca2roaming() / "appdata" / this->appid();

   }


   ::file::path acme_directory::public_system()
   {

      return public_root() / "system";

   }


   ::file::path acme_directory::system()
   {

      return ca2roaming() / "system";

   }


   ::file::path acme_directory::config()
   {

      return ca2roaming() / "config";

   }


   ::file::path acme_directory::local()
   {

      return ca2roaming() / "local";

   }


   ::file::path acme_directory::sensitive()
   {

   #ifdef __HAIKU__

      return "C:\\sensitive\\sensitive";

   #else

      return "/sensitive/sensitive";

   #endif

   }







   string acme_directory::system_short_name()
   {

      ::file::path pathSystemShortName = localconfig() / "system_short_name.txt";

      return m_pacmefile->as_string(pathSystemShortName).trimmed();

   }


   ::file::path acme_directory::relative(::file::path path)
   {

      path.find_replace(":", "");

      path.case_insensitive_ends_eat(".exe");

      return path;

   }

   #ifdef _UWP


   ::file::path acme_directory::app_relative()
   {

      return "";

   }


   #else


   ::string acme_directory::appid()
   {

      ::file::path path = m_pacmefile->module();

      path = file_path_folder(path);

      return path;

   }


   #endif



   ::file::path acme_directory::inplace_install(string strAppId, string strPlatform, string strConfiguration)
   {

   #ifdef HAIKU_DESKTOP

      ::file::path path;

      string strFolder;

      strsize iFind = strAppId.find('/');

      if (strPlatform.compare_ci("win32") == 0 || strPlatform.compare_ci("x86") == 0)
      {

         path = program_files_x86();

      }
      else
      {
         path = program_files_x86();


         path = program_files();

      }

      if (iFind < 0)
      {

         path /= strAppId;

      }
      else
      {

         path /= strAppId.Left(iFind);

         path /= strAppId.Mid(iFind + 1);

      }

      return path;

   #elif defined(ANDROID)

      return pacmedir->roaming();

   #else

      return acmefile()->module().folder(4);

   #endif


   }


   ::file::path acme_directory::inplace_matter_install(string strAppId, string strPlatform, string strConfiguration)
   {

   #ifdef HAIKU_DESKTOP

      ::file::path path;

      string strFolder;

      strsize iFind = strAppId.find('/');

      path = ca2roaming();

      path /= "_matter";

      return path;

   #elif defined(ANDROID)

      return pacmedir->roaming();

   #else

      return acmefile()->module().folder(4);

   #endif


   }


   ::file::path acme_directory::install()
   {

      if (m_pathInstallFolder.is_empty())
      {

         return default_install();

      }

      return m_pathInstallFolder;

   }


   ::file::path acme_directory::default_install()
   {

   #ifdef ANDROID

      return pacmedir->roaming();

   #elif defined(__APPLE__)

      return m_psystem->m_pacmepath->app_module().folder(3);

   #else

      return acmefile()->module().folder(4);

   #endif

   }


   ::file::path acme_directory::beforeca2()
   {

      return file_path_folder(install());

   }


   ::file::path acme_directory::program_files_x86()
   {

      ::file::path path("/opt/ca2");

      return path;

   }


   ::file::path acme_directory::program_files()
   {

      ::file::path path("/opt/ca2");

      return path;

   }

   ::file::path acme_directory::stage(string strAppId, string strPlatform, string strConfiguration)
   {

      return inplace_install(strAppId, strPlatform, strConfiguration) / "time" / strPlatform / strConfiguration;

   }


   ::file::path acme_directory::home()
   {

      return getenv("HOME");

   }


   void acme_directory::set_path_install_folder(const ::string & strPath)
   {

      m_pathInstallFolder = strPath;

   }


   ::file::path acme_directory::bookmark()
   {

      return localconfig() / "bookmark";

   }





   ::file::path acme_directory::sys_temp()
   {

      return appdata() / "time";

   }


   //::string acme_directory::dir_root()
   //{

   //   return "";

   //}


   //::file::path acme_directory::home()
   //{

   //   return "";

   //}


   //::file::path acme_directory::program_data()
   //{

   //   return "";

   //}


   ::file::path acme_directory::ca2appdata()
   {

      return ca2roaming() / "appdata";

   }



   ::file::path acme_directory::public_root()
   {

      return program_data() / "ca2";

   }

   ::file::path acme_directory::ca2roaming()
   {

      return roaming() / "ca2";

   }


   ::file::path acme_directory::localconfig()
   {

      return ca2roaming() / "localconfig";

   }


   ::file::path acme_directory::module()
   {

      return "";

   }

   //
   //::file::path acme_directory::base_module()
   //{
   //
   //   return "";
   //
   //}
   //

   //::file::path acme_directory::ca2_module()
   //{
   //
   //   return "";
   //
   //}
   //
   ::file::path acme_directory::archive()
   {

      return "";

   }



   ::file::path acme_directory::tool()
   {

      return "";

   }


   //::file::path acme_directory::roaming()
   //{

   //   return "";

   //}


   ::file::path acme_directory::pathfind(const string& pszEnv, const string& pszTopic, const string& pszMode)
   {

      ::file::path_array stra;

      stra.add_tokens(pszEnv, ":", false);

      string strCandidate;

      for (i32 i = 0; i < stra.get_count(); i++)
      {

         strCandidate = stra[i] / pszTopic;

         //if (m_pcontext->m_papexcontext->file().exists(strCandidate))
         if (file_exists(strCandidate))
         {

            return strCandidate;

         }

      }

      return "";

   }


   //::file::path acme_directory::get_memory_map_base_folder_path()
   //{

   //   return "";

   //}


   ::file::path acme_directory::user_appdata_local()
   {

      //return _shell_get_special_folder_path(CSIDL_LOCAL_APPDATA);

      return home() / ".config";

   }


} // namespace acme_haiku



