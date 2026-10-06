//
// Created by camilo on 19/01/2021. --<33ThomasBS!!
//
#include "platform.h"
#include "node.h"
#include "xfce_shared.h"
#include "apex/platform/system.h"
//#include "windowing_x11/windowing_x11.h"
#include "acme/windowing/windowing.h"


void gdk_branch(const ::procedure & procedure);


namespace operating_ambient_gtk3
{


   CLASS_DECL_ACME void _os_process_user_theme(const ::scoped_string & scopedstrTheme);


   node::node()
   {

      //m_pNodeDesktopEnvironmentXfce = this;

      //defer_initialize_x11();

      m_pGtkSettingsDefault = nullptr;

   }


   node::~node()
   {

      if(m_pGtkSettingsDefault)
      {

         g_object_unref(m_pGtkSettingsDefault);

      }

   }



//   void node::defer_notify_startup_complete()
//   {
//
//      auto psystem = system();
//
//      string strApplicationServerName = psystem->get_application_server_name();
//
//      gdk_notify_startup_complete_with_id (strApplicationServerName);
//
//      gdk_notify_startup_complete();
//
//   }


//   ::e_status node::start_node()
//   {
//
//      return estatus;
//
//
//   }


   void node::initialize(::particle * pparticle)
   {

      //::node_xfce::g_defer_init();

      ::aura_haiku::node::initialize(pparticle);

//      if(!estatus)
//      {
//
//         return estatus;
//
//      }

      ::node_gtk3::node::initialize(pparticle);

//      if(!estatus)
//      {
//
//         return estatus;
//
//      }
//
//      return estatus;

   }


//   void node::os_calc_user_dark_mode()
//   {
//
//      ::linux::aura::node::os_calc_user_dark_mode();
//
//   }


//   bool node::windowing_message_loop_step()
//   {
//
//      
//
//      if(::is_null(psession))
//      {
//
//         return false;
//
//      }
//
//      auto paurasession = psession;
//
//      if(::is_null(paurasession))
//      {
//
//         return false;
//
//      }
//
//      auto puser = paurasession->user();
//
//      if(::is_null(puser))
//      {
//
//         return false;
//
//      }
//
//      auto pwindowing = system()->windowing();
//
//      if(::is_null(pwindowing))
//      {
//
//         return false;
//
//      }
//
//      bool bRet = pwindowing->message_loop_step();
//
//      return bRet;
//
//   }


//   string node::os_get_user_theme()
//   {
//
//      return m_strTheme;
//
//   }


//   void node::os_set_user_theme(const ::scoped_string & scopedstrUserTheme)
//   {
//
//      // https://ubuntuforums.org/showthread.php?t=2140488
//      // gsettings set org.xfce.desktop.interface gtk-theme your_theme
//
//      // indirect wall-changer sourceforge.net contribution
//
//      auto psystem = system();
//
//      auto pnode = psystem->node();
//
//      auto edesktop = pnode->get_edesktop();
//
//      switch (edesktop)
//      {
//
//      case ::user::e_operating_ambient_gnome:
//      case ::user::e_operating_ambient_ubuntu_gnome:
//      case ::user::e_operating_ambient_unity_gnome:
//      {
//
//      bool bOk1 = ::node_xfce::gsettings_set("org.gnome.desktop.interface", "gtk-theme", strUserTheme);
//
//      bool bOk2 = true;
//
//      //if(::file::system_short_name().case_insensitive_contains("manjaro"))
//      {
//
//         bOk2 = ::node_xfce::gsettings_set("org.xfce.desktop.wm.preferences", "theme", strUserTheme);
//
//      }
//
//      sleep(300_ms);
//
//      ::node_xfce::gsettings_sync();
//
//      sleep(300_ms);
//
//      return
//      bOk1 &&bOk2;
//
//      }
//
//      case ::user::e_operating_ambient_mate:
//
//      //return ::user::gsettings_set("org.mate.background", "picture-filename", strLocalImagePath);
//
//      case ::user::e_operating_ambient_lxde:
//
//      //call_async("pcmanfm", "-w " + strLocalImagePath, nullptr, e_display_none, false);
//
//      break;
//
//      case ::user::e_operating_ambient_xfce:
//      {
//      //        Q_FOREACH(QString entry, Global::getOutputOfCommand("xfconf-query", QStringList() << "-c" << "xfce4-desktop" << "-point" << "/backdrop" << "-l").split("\n")){
//      //          if(entry.contains("image-path") || entry.contains("last-image")){
//      //            QProcess::startDetached("xfconf-query", QStringList() << "-c" << "xfce4-desktop" << "-point" << entry << "-s" << image);
//      //      }
//      //}
//
//      }
//
//      //break;
//
//      default:
//
//      informationf("Failed to change wallpaper. If your Desktop Environment is not listed at \"Preferences->Integration-> Current Desktop Environment\", then it is not supported.");
//      return false;
//
//      }
//
//      return true;
//
//   }


//   void node::os_process_user_theme(const ::scoped_string & scopedstrTheme)
//   {
//
//      _os_process_user_theme(strTheme);
//
//   }

//
//   bool node::set_wallpaper(::collection::index iScreen, const ::scoped_string & scopedstrLocalImagePath)
//   {
//
//      // wall-changer sourceforge.net contribution
//
//      auto psystem = system();
//
//      auto pnode =  psystem->node();
//
//      auto edesktop = pnode->get_edesktop();
//
//      switch (edesktop)
//      {
//
//         case ::user::e_operating_ambient_gnome:
//         case ::user::e_operating_ambient_ubuntu_gnome:
//         case ::user::e_operating_ambient_unity_gnome:
//
//            return ::node_xfce::gsettings_set("org.gnome.desktop.background", "picture-uri", "file://" + strLocalImagePath);
//
//         case ::user::e_operating_ambient_mate:
//
//            return ::node_xfce::gsettings_set("org.mate.background", "picture-filename", strLocalImagePath);
//
//         case ::user::e_operating_ambient_lxde:
//
//            call_async("pcmanfm", "-w " + strLocalImagePath, nullptr, e_display_none, false);
//
//            break;
//
//         case ::user::e_operating_ambient_xfce:
//         {
//            //        Q_FOREACH(QString entry, Global::getOutputOfCommand("xfconf-query", QStringList() << "-c" << "xfce4-desktop" << "-point" << "/backdrop" << "-l").split("\n")){
//            //          if(entry.contains("image-path") || entry.contains("last-image")){
//            //            QProcess::startDetached("xfconf-query", QStringList() << "-c" << "xfce4-desktop" << "-point" << entry << "-s" << image);
//            //      }
//            //}
//
//         }
//
//            //break;
//
//         default:
//
//            informationf("Failed to change wallpaper. If your Desktop Environment is not listed at \"Preferences->Integration-> Current Desktop Environment\", then it is not supported.");
//            return false;
//
//      }
//
//      return true;
//
//   }
//
//
//   void node::enable_wallpaper_change_notification()
//   {
//
//      auto psystem = system();
//
//      auto pnode = psystem->node();
//
//      auto edesktop = pnode->get_edesktop();
//
//      switch (edesktop)
//      {
//
//         case ::user::e_operating_ambient_gnome:
//         case ::user::e_operating_ambient_ubuntu_gnome:
//         case ::user::e_operating_ambient_unity_gnome:
//
//            ::node_xfce::g_enable_wallpaper_change_notification("org.gnome.desktop.background", "picture-uri");
//
//            break;
//
//         case ::user::e_operating_ambient_mate:
//
//            ::node_xfce::g_enable_wallpaper_change_notification("org.mate.background", "picture-filename");
//
//            break;
//
//         case ::user::e_operating_ambient_lxde:
//
//            //call_async("pcmanfm", "-w " + strLocalImagePath, nullptr, e_display_none, false);
//
//            break;
//
//         case ::user::e_operating_ambient_xfce:
//         {
//            //        Q_FOREACH(QString entry, Global::getOutputOfCommand("xfconf-query", QStringList() << "-c" << "xfce4-desktop" << "-point" << "/backdrop" << "-l").split("\n")){
//            //          if(entry.contains("image-path") || entry.contains("last-image")){
//            //            QProcess::startDetached("xfconf-query", QStringList() << "-c" << "xfce4-desktop" << "-point" << entry << "-s" << image);
//            //      }
//            //}
//
//         }
//
//         break;
//         default:
//
//            informationf("Failed to get wallpaper setting. If your Desktop Environment is not listed at \"Preferences->Integration-> Current Desktop Environment\", then it is not supported.");
//            //return "";
//
//      }
//
//   }
//
//
//   string node::get_file_icon_path(const_char_pointer pszPath, int iSize)
//   {
//
//      string str = ::node_xfce::node::get_file_icon_path(scopedstrPath, iSize);
//
//      return str;
//
//      //return ::linux_g_direct_get_file_icon_path(scopedstrPath, iSize);
//
//   }
//
//
//   string node::get_file_content_type(const_char_pointer pszPath)
//   {
//
//      string str = ::node_xfce::node::get_file_content_type(scopedstrPath);
//
//      return str;
//
//      //return ::linux_g_direct_get_file_content_type(scopedstrPath);
//
//   }
//
//
//   string node::get_wallpaper(::collection::index iScreen)
//   {
//
//      return "";
//
//   }
//

   ::e_status node::node_post(const ::procedure & procedure)
   {

      //system()->acme_windowing()->_user_post(procedure);

      system()->acme_windowing()->post(procedure);

      return ::success;

   }


//   void node::node_post_quit()
//   {
//
//      os_post_quit();
//
//   }


//   void node::on_subject(::promise::topic * ptopic, ::subject::context * pcontext)
//   {
//
//      if(ptopic->id() == ::id_os_user_theme)
//      {
//
//         _on_change_os_user_theme();
//
//      }
//
//
//   }


//   ::nlinux::appindicator * node::appindicator_allocate()
//   {
//
//      return ___new ::node_xfce::appindicator();
//
//   }
//
//
//   void node::appindicator_destroy(::linux::appindicator * pappindicator)
//   {
//
//      //::linux::appindicator_destroy(pappindicator);
//
//      delete pappindicator;
//
//   }


//   void node::enum_display_monitors(::aura::session * psession)
//   {
//
//      node_fork(__routine([psession]
//                           {
//
//                              synchronous_lock sl(user_synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
//
//                              //xdisplay d(x11_get_display());
//
//                              GdkDisplay *pdisplay = gdk_display_get_default();
//
//                              if (pdisplay == nullptr)
//                              {
//
//                                 return;
//
//                              }
//
//                              synchronous_lock slSession(psession->synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
//
//                              ::collection::count iMonitorCount = gdk_display_get_n_monitors(pdisplay);
//
//                              psession->m_rectaWorkspace.set_size(iMonitorCount);
//
//                              psession->m_rectaMonitor.set_size(iMonitorCount);
//
//                              for (::collection::index iMonitor = 0; iMonitor < iMonitorCount; iMonitor++)
//                              {
//
//                                 GdkMonitor *pmonitor = gdk_display_get_monitor(pdisplay, iMonitor);
//
//                                 auto &rectWorkspace = psession->m_rectaWorkspace[iMonitor];
//
//                                 auto &rectMonitor = psession->m_rectaMonitor[iMonitor];
//
//                                 if (pmonitor == nullptr)
//                                 {
//
//                                    rectWorkspace.Null();
//
//                                    rectMonitor.Null();
//
//                                    continue;
//
//                                 }
//
//                                 GdkRectangle rect;
//
//                                 zero(rect);
//
//                                 gdk_monitor_get_workarea(pmonitor, &rect);
//
//                                 __copy(rectWorkspace, rect);
//
//                                 zero(rect);
//
//                                 gdk_monitor_get_geometry(pmonitor, &rect);
//
//                                 __copy(rectMonitor, rect);
//
//                              }
//
//                           }));
//
//   }


//   void node::os_post_quit()
//   {
//
//   }


//   void * node::node_wrap_window(void * pvoidDisplay, long long window)
//   {
//
//      Display * pdisplay = (Display *) pvoidDisplay;
//
//      GdkDisplay * pd = gdk_x11_lookup_xdisplay (pdisplay);
//
//      auto pwindow = gdk_x11_window_foreign_new_for_display(GDK_DISPLAY(pd), (Window) window);
//
//      return pwindow;
//
////   }
//
//   bool node::should_launch_on_node(::topic * ptopic)
//   {
//
//      if(::is_null(ptopic))
//      {
//
//         return false;
//
//      }
//
//      if(ptopic->id() == id_os_dark_mode)
//      {
//
//         return false;
//
//      }
//
//      return false;
//
//   }
//
//
//   bool node::launch_on_node(::subject::topic * ptopic)
//   {
//
//      auto bOk = ::node_xfce::node::launch_on_node(psubject);
//
//      return bOk;
//
//   }
//


} // namespace operating_ambient_gtk3



