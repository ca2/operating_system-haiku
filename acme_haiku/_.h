#pragma once


#include "operating_system-posix/acme_posix/_.h"


#if defined(_acme_haiku_project)
#define CLASS_DECL_ACME_HAIKU  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_ACME_HAIKU  CLASS_DECL_IMPORT
#endif



//CLASS_DECL_ACME_HAIKU ::user::enum_desktop get_edesktop();




