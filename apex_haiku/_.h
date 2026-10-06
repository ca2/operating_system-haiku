#pragma once


#include "apex/_.h"
#include "operating_system-posix/apex_posix/_.h"
#include "operating_system-haiku/acme_haiku/_.h"


#if defined(_APEX_HAIKU_LIBRARY)
#define CLASS_DECL_APEX_HAIKU  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APEX_HAIKU  CLASS_DECL_IMPORT
#endif


namespace apex_haiku
{


   //class dir_context;
   //class dir_system;

   //class file_context;
   //class file_system;

   class node;


} // namespace apex_haiku



