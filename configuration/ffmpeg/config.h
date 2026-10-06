#pragma once


/* #define FF_API_AVIO_WRITE_NONCONST 1 */
extern "C"
{


#include <libavformat/version.h>


}

//#ifdef DEBIAN_LINUX
//#error "debian check"
//#define AVIO_FILE_WRITE_TYPE uint8_t
//#elif defined(MINT_LINUX)
//#error "debian check"
//#define AVIO_FILE_WRITE_TYPE uint8_t
//#else
//#error "non debian check"

#if defined(FF_API_AVIO_WRITE_NONCONST)
#if FF_API_AVIO_WRITE_NONCONST
#define AVIO_FILE_WRITE_TYPE uint8_t
#else
#define AVIO_FILE_WRITE_TYPE const uint8_t
#endif
#elif LIBAVFORMAT_VERSION_MAJOR >= 61
#define AVIO_FILE_WRITE_TYPE const uint8_t
#else
#define AVIO_FILE_WRITE_TYPE uint8_t
#endif
//#endif
