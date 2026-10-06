#pragma once


#include "inc.h"


#include "config_memdleak.h"


#define HAVE_UNISTD_H


#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif // _THREAD_SAFE

#if !defined(_LARGEFILE64_SOURCE)
#define _LARGEFILE64_SOURCE
#endif

#define PARALLELIZATION_PTHREAD


//#define ENABLE_BINRELOC


#define HAS_FREEDESKTOP


//#ifndef __HAIKU__
//#define __HAIKU__
//#endif

//#ifndef _HAIKU
//#define _HAIKU
//#endif

//#ifndef __HAIKU__
//#define __HAIKU__
//#endif

//#ifdef RASPBIAN
//#pragma pack(4)
//#endif


#define c_cdecl


//#ifdef RASPBIAN
//#define BROAD_PRECOMPILED_HEADER 0
//#else
//#define BROAD_PRECOMPILED_HEADER 1
//#endif

using errno_t = int;

#define OPERATING_SYSTEM_NAMESPACE haiku

#define PLATFORM_COMMON_NAMESPACE posix

#define PLATFORM_FAMILY_NAMESPACE darwin

#define PLATFORM_STRING "haiku"

#define PLATFORM_COMMON_STRING "posix"

#define PLATFORM_FAMILY_STRING "darwin"

#define DEFAULT_DIR_SEPARATOR "/"

#define OPERATING_SYSTEM_NAME "haiku"

#define OPERATING_SYSTEM_NAMESPACE haiku



#ifndef __haiku__
#define __haiku__
#endif

#define _OPENGL 1

#define HAVE_TERMIOS_H 1


#define _FILE_OFFSET_BITS 64


#define BSD_STYLE_SOCKETS 1
#define HAVE_MYSQL 1
#define HAVE_OPENSSL 1


#define MEMORY_DECL

#define _POSIX_C_SOURCE 200809L

#define VARIADIC_TEMPLATE_FORMAT2

#define HAVE_PTHREAD_MUTEX_TIMEDLOCK 1

#define OPENSSL_API_COMPAT 10100


#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#define CLASS_DECL_THREAD __thread
#define __FACTORY_EXPORT extern "C"
#define __FACTORY_IMPORT extern "C"









#ifdef cplus
#define inline static
#endif

#define static_inline static inline



#ifdef __cplusplus

#define LOG_THIS_TAG (get_demangle(type_name()))
#define LOG_MEMBER_PREFIX __FUNCTION__

#define ALOG_FUNCTION __FUNCTION__
#define ALOG_LINE __LINE__
#define ALOG_FILE __FILE__


#include <initializer_list>
#include <compare>


#endif



#define AXIS_FREEIMAGE






#define _strdup strdup


#define HNULL 0
#define STR_NEWLINE "\n"


//#include "bit.h"



#define FONT_SANS "sans-serif"
#define FONT_SERIF "serif"
#define FONT_SANS_EX "sans-serif"
#define FONT_SERIF_EX "serif"
#define FONT_SANS_FX "sans-serif"
#define FONT_SERIF_FX "serif"
#define FONT_SANS_UI "sans-serif"
#define FONT_SERIF_UI "serif"


#define FONT_MONO "monospace"


