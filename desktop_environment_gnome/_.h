#pragma once


#include "aura_haiku/_.h"
#include "node_gnome/_.h"


#if defined(_DESKTOP_ENVIRONMENT_GNOME_LIBRARY)
   #define CLASS_DECL_DESKTOP_ENVIRONMENT_GNOME  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_DESKTOP_ENVIRONMENT_GNOME  CLASS_DECL_IMPORT
#endif


namespace desktop_environment_gnome
{


   class node;


} // namespace node_gnome



