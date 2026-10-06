//
// Created by camilo on 19/01/2021. <33TBS!!
//

//
// Created by camilo on 19/01/2021. <3-<3ThomasBS!!
//
#pragma once


#include "aura_haiku/node.h"
#include "node_gnome/node.h"


namespace desktop_environment_gnome
{


   class CLASS_DECL_DESKTOP_ENVIRONMENT_GNOME node :
      virtual public ::aura_haiku::node,
      virtual public ::node_gnome::node
   {
   public:


      //gpointer m_pGtkSettingsDefault;
      //string   m_strTheme;

      //::boolean         m_bDarkMode;


      node();
      ~node() override;


      //virtual int node_init_check(int * pi, char *** ppz) override;

      //virtual void defer_notify_startup_complete() override;

      //void start_node() override;

      void initialize(::particle * pparticle) override;

      //virtual void os_calc_dark_mode();

      //virtual bool _os_calc_dark_mode();

      //virtual string os_get_user_theme() override;

      //string _os_get_user_theme() override;

      //virtual string _os_get_user_theme();

      //virtual void os_process_user_theme(string strTheme);

      //virtual void _os_process_user_theme_color(string strTheme);

      //bool os_set_user_theme(const string & strUserTheme) override;

      //virtual void os_process_user_theme(string strTheme) override;

      //void enable_wallpaper_change_notification() override;

      //string get_file_icon_path(const ::string & strPath, int iSize) override;

      //string get_file_content_type(const ::string & strPath) override;

      //bool set_wallpaper(index iScreen, string strLocalImagePath) override;

      //string get_wallpaper(index iScreen) override;

      //using ::aura::node::node_fork;

      //virtual bool windowing_message_loop_step();

      //virtual ::e_status node_branch(const ::routine & routine);

      //virtual void node_post_quit() override;

      //void handle(::topic * psubject, ::context * pcontext) override;

      //virtual void _on_change_os_user_theme(string strTheme);

      //virtual void _os_process_user_theme_color(string strTheme);

      //virtual ::node_haiku::appindicator * appindicator_allocate() override;

      //virtual void appindicator_destroy(::haiku::appindicator * pappindicator) override;

      //virtual void enum_display_monitors(::aura::session * psession) override;

      //virtual bool _os_calc_system_dark_mode() override;

      //virtual void os_post_quit() override;
//
//      bool should_launch_on_node(::topic * ptopic) override;
//
//      bool launch_on_node(::topic * ptopic) override;
//

   };


} // namespace desktop_environment_gnome




