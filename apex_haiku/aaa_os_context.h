#pragma once


#include "apex_posix/os_context.h"


namespace apex_haiku
{


   class CLASS_DECL_APEX os_context :
      virtual public ::apex_posix::os_context
   {
   public:


      os_context();
      ~os_context() override;


      //string get_command_line() override;


      void reboot() override;
      void shutdown(bool bPowerOff) override;

      void terminate_processes_by_title(const ::string & strName) override;


      virtual bool haiku_can_exec(const char *file);


      //virtual ::file::path get_module_path(HMODULE hmodule) override;


      //virtual bool path_pid(::u32 & dwPid, const ::string & strName) override;
      //virtual bool title_pid(::u32 & dwPid, const ::string & strName) override;
      //virtual void get_all_processes(u32_array & dwa) override;
      //virtual ::file::path get_process_path(::u32 dwPid) override;
      //virtual int get_pid() override;

      virtual ::payload connection_settings_get_auto_detect() override;
      virtual ::payload connection_settings_get_auto_config_url() override;


      void local_machine_set_run(const ::string & strKey, const ::file::path & pathExecutable, const ::string& strArguments, bool bSet) override;
      void local_machine_set_run_once(const ::string & strKey, const ::file::path & pathExecutable, const ::string& strArguments, bool bSet) override;
      void current_user_set_run(const ::string & strKey, const ::file::path & pathExecutable, const ::string& strArguments, bool bSet) override;
      void current_user_set_run_once(const ::string & strKey, const ::file::path & pathExecutable, const ::string& strArguments, bool bSet) override;
      void defer_register_ca2_plugin_for_mozilla() override;

      void file_extension_get_open_with_list_keys(string_array & straKey, const ::string & strExtension) override;
      void file_extension_get_open_with_list_commands(string_array & straCommand, const ::string & strExtension) override;

      void file_association_set_default_icon(const ::string & strExtension, const ::string & strExtensionNamingClass, const ::string & strIconPath) override;
      void file_association_set_shell_open_command(const ::string & strExtension, const ::string & strExtensionNamingClass, const ::string & strCommand, const ::string & strParam) override;
      void file_association_get_shell_open_command(const ::string & strExtension, string & strExtensionNamingClass, string & strCommand, string & strParam) override;


      //virtual bool open_in_ie(const char * pcsz);


      void enable_service(const ::string & strServiceName, const ::string & strDisplayName, const ::string & strCommand, const ::string & strUser = "", const ::string & strPass = "") override;

      void disable_service(const ::string & strServiceName) override;

      void start_service(const ::string & strServiceName) override;

      void stop_service(const ::string & strServiceName) override;

      void raise_exception( ::u32 dwExceptionCode, ::u32 dwExceptionFlags);

      virtual bool is_remote_session() override;

      //virtual void post_to_all_threads(const ::id & id, wparam wparam, lparam lparam);


      virtual void initialize_wallpaper_fileset(::file::set * pfileset, bool bAddSearch) override;

      virtual void get_default_browser(string & strId, ::file::path & path, string & strParam) override;

      virtual void file_open(const ::file::path & path, const ::string & strParams = "", const ::file::path & pathFolder = "") override;

      //virtual void list_process(::file::path_array & patha, u32_array & iaPid) override;


   };


} // namespace apex_haiku



//typedef string GET_FILE_CONTENT_TYPE(string strPath);

//typedef GET_FILE_CONTENT_TYPE * PFN_GET_FILE_CONTENT_TYPE;

//void set_get_file_content_type_function(PFN_GET_FILE_CONTENT_TYPE pfnGetFileContentType);

