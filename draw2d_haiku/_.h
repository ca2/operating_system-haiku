#pragma once
#include "aura/_.h"
#if defined(_draw2d_haiku_project)
#define CLASS_DECL_DRAW2D_HAIKU CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DRAW2D_HAIKU CLASS_DECL_IMPORT
#endif
