#pragma once


// GDK, GTK :: C

extern "C"
{


   void haiku_g_direct_init();
   void haiku_g_direct_term();
   //void * haiku_g_direct_app_indicator_new(const char * pszId, const char * pszIcon, const char * pszFolder, struct user_notify_icon_bridge * pbridge);
   void haiku_g_direct_step(void * pind);
   //void haiku_g_direct_app_indicator_term(void * pind);


} // extern "C"


//struct user_notify_icon_bridge
//{
//public:
//
//
//   virtual int _get_notification_area_action_count() = 0;
//   virtual bool _get_notification_area_action_info(char ** ppszName, char ** ppszId, char ** ppszLabel, char ** ppszAccelerator, char ** ppszDescription, int iIndex) = 0;
//   virtual void call_notification_area_action(const char * pszId) = 0;
//
//
//};


#include "gnome_shared.h"




