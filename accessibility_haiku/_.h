#pragma once


#include "acme/_.h"
// Windows backend dependency removed: #include "acme_windows/_.h"


#if defined(_accessibility_haiku_project)
#define CLASS_DECL_ACCESSIBILITY_HAIKU  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_ACCESSIBILITY_HAIKU  CLASS_DECL_IMPORT
#endif


namespace accessibility_haiku
{


   class accessibility;


   class application;

   class main_window;


} // namespace acme_windows



