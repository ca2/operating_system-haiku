#pragma once


#include "node_haiku/_.h"
#include "operating_system-posix/node_gtk3/_.h"


#if defined(_operating_ambient_gtk3_project)
   #define CLASS_DECL_OPERATING_AMBIENT_GTK3  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_OPERATING_AMBIENT_GTK3  CLASS_DECL_IMPORT
#endif


namespace operating_ambient_gtk3
{


   class node;


} // namespace operating_ambient_gtk3



