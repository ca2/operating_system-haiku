// Created by camilo on 2026-10-08 02:23 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once
#include "aura/_.h"



#if defined(_windowing_haiku_project)
#define CLASS_DECL_WINDOWING_HAIKU CLASS_DECL_EXPORT
#else
#define CLASS_DECL_WINDOWING_HAIKU CLASS_DECL_IMPORT
#endif


namespace windowing_haiku
{

   class windowing;
   class display;
   class monitor;
   class window;
   class graphics;


}
