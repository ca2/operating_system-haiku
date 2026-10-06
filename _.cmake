

set(OPERATING_SYSTEM_NAME "sunos")
set(__SUNOS__ TRUE)
set(USE_PKGCONFIG TRUE)
set(INCLUDE_DRAW2D_CAIRO TRUE)


find_package(PkgConfig REQUIRED)

# OpenIndiana installs FFmpeg 6 development metadata outside pkgconf's
# default search directories. Keep explicit user search paths first.
set(_sunos_ffmpeg_pkgconfig_dir "/usr/lib/amd64/pkgconfig/ffmpeg-6")
if(EXISTS "${_sunos_ffmpeg_pkgconfig_dir}/libswresample.pc"
   AND EXISTS "${_sunos_ffmpeg_pkgconfig_dir}/libavutil.pc")
   set(_sunos_pkgconfig_path "$ENV{PKG_CONFIG_PATH}")
   string(REPLACE ":" ";" _sunos_pkgconfig_dirs "${_sunos_pkgconfig_path}")
   if(NOT "${_sunos_ffmpeg_pkgconfig_dir}" IN_LIST _sunos_pkgconfig_dirs)
      if(_sunos_pkgconfig_path STREQUAL "")
         set(ENV{PKG_CONFIG_PATH} "${_sunos_ffmpeg_pkgconfig_dir}")
      else()
         set(ENV{PKG_CONFIG_PATH} "${_sunos_pkgconfig_path}:${_sunos_ffmpeg_pkgconfig_dir}")
      endif()
   endif()
endif()


add_compile_definitions(__SUNOS__)

set(default_write_text write_text_pango)
set(default_draw2d draw2d_cairo)
set(default_imaging imaging_freeimage)
set(default_networking networking_bsd)
set(default_audio audio_sunaudio CACHE STRING "SunOS audio backend")
# Alternative default (both modules are built):
# set(default_audio audio_oss CACHE STRING "SunOS audio backend" FORCE)
set_property(CACHE default_audio PROPERTY STRINGS audio_sunaudio audio_oss)

#set(LINK_STATIC_OPTION "-static")
set(LINK_STATIC_OPTION "")

#set(__SYSTEM_ARCHITECTURE "i86pc")





execute_process(COMMAND uname -m OUTPUT_VARIABLE __SYSTEM_ARCHITECTURE)
string(STRIP ${__SYSTEM_ARCHITECTURE} __SYSTEM_ARCHITECTURE)

execute_process(COMMAND uname -r OUTPUT_VARIABLE __SYSTEM_RELEASE)
set(OPERATING_SYSTEM_RELEASE ${__SYSTEM_RELEASE})


set(__SYSTEM $ENV{__SYSTEM})


message(STATUS "__SYSTEM_ARCHITECTURE is ${__SYSTEM_ARCHITECTURE}")


set(__TARGET_SYSTEM_ARCHITECTURE ${__SYSTEM_ARCHITECTURE})

message(STATUS "__TARGET_SYSTEM_ARCHITECTURE is ${__TARGET_SYSTEM_ARCHITECTURE}")


message(STATUS "\$ENV{__SYSTEM} is $ENV{__SYSTEM}")


if (${__SYSTEM} STREQUAL "openindiana")

   set(OPENINDIANA TRUE)

   set(SUNOS_LIKE TRUE)

   #add_compile_definitions(UBUNTU_LINUX)

#   add_compile_definitions(DEBIAN_LIKE_LIBUILD_GPU_BASED_APPLICATIONSNUX)

   message(STATUS "UBUNTU has been set TRUE")

   set(APPINDICATOR_PKG_MODULE "ayatana-appindicator3-0.1")

   #set(APPINDICATOR_PKG_MODULE "appindicator3-0.1")

   set(MPG123_PKG_MODULE "libmpg123")

   set(HAS_SYSTEM_UNAC FALSE)


endif()


include("operating_system/operating_system-posix/_desktop_ambient_1.cmake")
include("operating_system/operating_system-posix/_desktop_ambient_2.cmake")

#set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/time-${OPERATING_SYSTEM_NAME}/x64/basis)
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/output")
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/time-${OPERATING_SYSTEM_NAME}/x64/basis)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/output")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output")

link_directories(${LIBRARY_OUTPUT_PATH})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/operating_system/storage-${OPERATING_SYSTEM_NAME}/library/${TARGET_ARCH}/basis)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/operating_system/storage-${OPERATING_SYSTEM_NAME}/third/library/${TARGET_ARCH}/basis)


#include_directories(${WORKSPACE_FOLDER})
#include_directories($ENV{HOME}/__config)
#include_directories(${WORKSPACE_FOLDER}/source)
#include_directories(${WORKSPACE_FOLDER}/source/app)
#include_directories(${WORKSPACE_FOLDER}/source/app/include)
#include_directories(${WORKSPACE_FOLDER}/source/include)
#include_directories(${WORKSPACE_FOLDER}/port/_)
#include_directories(${WORKSPACE_FOLDER}/port/include)
#include_directories(${WORKSPACE_FOLDER}/operating_system)
if (${OPERATING_SYSTEM_POSIX})
   include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-posix)
   include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-posix/include)
endif ()
include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-${OPERATING_SYSTEM_NAME})
include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-${OPERATING_SYSTEM_NAME}/include)
include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-${OPERATING_SYSTEM_NAME}/include/configuration)
include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-${OPERATING_SYSTEM_NAME}/configuration)
include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-${OPERATING_SYSTEM_NAME}/include/configuration_selection/${CMAKE_BUILD_TYPE})
include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-${OPERATING_SYSTEM_NAME}/operating_system/${SLASHED_OPERATING_SYSTEM})
include_directories(${WORKSPACE_FOLDER}/operating_system/operating_system-${OPERATING_SYSTEM_NAME}/operating_system/${DISTRO})

set(INCLUDE_DRAW2D_CAIRO TRUE)
set(INCLUDE_IMAGING_FREEIMAGE TRUE)
set(USE_PORT_LIBFLUIDSYNTH TRUE)


set(STORE_FOLDER $ENV{HOME}/store/${SLASHED_OPERATING_SYSTEM})



if("${APPINDICATOR_PKG_MODULE}" STREQUAL "")
   message(STATUS "APPINDICATOR_PKG_MODULE is (Empty)")
else ()
   message(STATUS "APPINDICATOR_PKG_MODULE is ${APPINDICATOR_PKG_MODULE}")
endif()


