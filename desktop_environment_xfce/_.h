#pragma once


#include "aura_haiku/_.h"
#include "node_xfce/_.h"


#if defined(_DESKTOP_ENVIRONMENT_XFCE_LIBRARY)
   #define CLASS_DECL_DESKTOP_ENVIRONMENT_XFCE  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_DESKTOP_ENVIRONMENT_XFCE  CLASS_DECL_IMPORT
#endif


namespace desktop_environment_xfce
{


   class node;


} // namespace node_xfce



