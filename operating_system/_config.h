#pragma once


#define _THREAD_SAFE

#define ENABLE_BINRELOC

#define BROAD_PRECOMPILED_HEADER 1

#define _OPENGL 1

#define BSD_STYLE_SOCKETS 1
#define HAVE_MYSQL 1
#define HAVE_OPENSSL 1
#define HAVE_TERMIOS_H 1



#define OPERATING_SYSTEM_NAMESPACE haiku

#define PLATFORM_COMMON_NAMESPACE posix

#define PLATFORM_STRING "haiku"

#define PLATFORM_COMMON_STRING "posix"

#define DEFAULT_DIR_SEPARATOR "/"

#define OPERATING_SYSTEM_NAME "haiku"

#define OPERATING_SYSTEM_NAMESPACE haiku



#define PARALLELIZATION_PTHREAD 1

#define VARIADIC_TEMPLATE_FORMAT2

#define HAVE_PTHREAD_MUTEX_TIMEDLOCK 1

#define MUTEX_COND_TIMED 1


#define OPENSSL_API_COMPAT 0x10100000L


#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#define CLASS_DECL_THREAD __thread
#define __FACTORY_EXPORT extern "C"
#define __FACTORY_IMPORT extern "C"









#define INFINITE_TIMEOUT ((::u32)0xffffffff)


#ifdef cplus
#define inline static
#endif

#define static_inline static inline



#ifdef __cplusplus

#define LOG_THIS_TAG (get_demangle(typeid(*this).name()))
#define LOG_MEMBER_PREFIX __FUNCTION__

#define ALOG_FUNCTION __FUNCTION__
#define ALOG_LINE __LINE__
#define ALOG_FILE __FILE__


#include <initializer_list>

#endif



#define AXIS_FREEIMAGE


#define _MAX_PATH PATH_MAX


#define FONT_SANS "FreeSans"
#define FONT_SERIF "FreeSerif"
#define FONT_MONO "Ubuntu Mono"
#define FONT_SANS_EX "Liberation Sans"
#define FONT_SERIF_EX "Liberation Serif"
#define FONT_SANS_FX "Ubuntu"
#define FONT_SERIF_FX "Bitstream Charter"
#define FONT_SANS_UI "Ubuntu"
#define FONT_SERIF_UI "Bitstream Charter"




#define _strdup strdup


#define HNULL 0
#define STR_NEWLINE "\n"


//#include "bit.h"





