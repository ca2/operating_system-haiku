//
// Created by camilo on 19/01/2021. <33TBS!!
//

//
// Created by camilo on 19/01/2021. <3-<3ThomasBS!!
//
#pragma once


#include "node_haiku/node.h"
#include "node_gtk3/node.h"


namespace operating_ambient_gtk3
{


   class CLASS_DECL_OPERATING_AMBIENT_GTK3 node :
      virtual public ::node_haiku::node,
      virtual public ::node_gtk3::node
   {
   public:


      //gpointer m_pGtkSettingsDefault;
      //string   m_strTheme;


      node();
      ~node() override;


      //virtual int node_init_check(int * pi, char *** ppz) override;

      //virtual void defer_notify_startup_complete() override;

      //virtual void start_node() override;

      virtual void initialize(::particle * pparticle) override;

      //virtual void os_calc_user_dark_mode() override;

      //virtual string os_get_user_theme() override;

      //virtual string _os_get_user_theme();

      //virtual bool os_set_user_theme(const ::scoped_string & scopedstrUserTheme) override;

      //virtual void os_process_user_theme(const ::scoped_string & scopedstrTheme) override;

      //virtual void enable_wallpaper_change_notification() override;

      //virtual string get_file_icon_path(const_char_pointer pszPath, int iSize) override;

      //virtual string get_file_content_type(const_char_pointer pszPath) override;

      //virtual bool set_wallpaper(::collection::index iScreen, const ::scoped_string & scopedstrLocalImagePath) override;

      ///virtual string get_wallpaper(::collection::index iScreen) override;

      //using ::aura::node::node_fork;

      //virtual bool windowing_message_loop_step();

      virtual ::e_status node_post(const ::procedure & procedure);

      //virtual void node_post_quit() override;

      //virtual void on_subject(::promise::topic * ptopic, ::subject::context * pcontext) override;

      //virtual void _on_change_os_user_theme(const ::scoped_string & scopedstrTheme);

      //virtual void _os_process_user_theme_color(const ::scoped_string & scopedstrTheme);

      //virtual ::node_linux::appindicator * appindicator_allocate() override;

      //virtual void appindicator_destroy(::linux::appindicator * pappindicator) override;

      //virtual void enum_display_monitors(::aura::session * psession) override;

      //virtual bool _os_calc_system_dark_mode() override;

      //virtual void os_post_quit() override;
//
//      virtual bool should_launch_on_node(::topic * ptopic);
//
//      virtual bool launch_on_node(::topic * ptopic);


   };


} // namespace operating_ambient_gtk3




