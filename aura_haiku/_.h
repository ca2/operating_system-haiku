#pragma once


#include "aura/_.h"
#include "operating_system-posix/aura_posix/_.h"
#include "operating_system-haiku/apex_haiku/_.h"


#if defined(_AURA_HAIKU_LIBRARY)
#define CLASS_DECL_AURA_HAIKU  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_AURA_HAIKU  CLASS_DECL_IMPORT
#endif




