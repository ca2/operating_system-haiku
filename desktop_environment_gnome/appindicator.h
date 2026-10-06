//
// Created by camilo on 20/01/2021. ThomasBS!!
//
#pragma once


#include "node_gnome/appindicator.h"


namespace desktop_environment_gnome
{


   class appindicator :
      virtual public ::node_gnome::appindicator
   {
   public:


      appindicator();
      ~appindicator() override;


      bool create(const char * pszId, const char * pszIcon, const char * pszFolder, application_menu * papplicationmenu, application_menu_callback * papplicationmenucallback) override;



   };


} // namespace desktop_environment_gnome



