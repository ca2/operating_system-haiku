

set(OPERATING_SYSTEM_NAME "haiku")
set(__HAIKU__ TRUE)

# Call from an executable's __implement/CMakeLists.txt after add_executable().
# The application id uses the same repo/app spelling as m_strAppId.
function(haiku_set_application_icon target app_id icon)
   if(NOT TARGET "${target}")
      message(FATAL_ERROR "haiku_set_application_icon: unknown target ${target}")
   endif()
   get_target_property(_haiku_target_type "${target}" TYPE)
   if(NOT _haiku_target_type STREQUAL "EXECUTABLE")
      message(FATAL_ERROR "haiku_set_application_icon requires an executable")
   endif()
   get_filename_component(_haiku_icon "${icon}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
   if(NOT EXISTS "${_haiku_icon}")
      message(FATAL_ERROR "haiku_set_application_icon: icon not found: ${_haiku_icon}")
   endif()
   if(NOT TARGET ca2_haiku_app_metadata)
      add_executable(ca2_haiku_app_metadata EXCLUDE_FROM_ALL
         "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/tools/app_metadata.cpp")
      target_link_libraries(ca2_haiku_app_metadata PRIVATE be translation)
   endif()
   string(REPLACE "/" "." _haiku_app_id "${app_id}")
   add_dependencies("${target}" ca2_haiku_app_metadata)
   set_property(TARGET "${target}" APPEND PROPERTY LINK_DEPENDS
      "${_haiku_icon}" "$<TARGET_FILE:ca2_haiku_app_metadata>")
   add_custom_command(TARGET "${target}" POST_BUILD
      COMMAND $<TARGET_FILE:ca2_haiku_app_metadata>
         $<TARGET_FILE:${target}> "${_haiku_icon}" "application/x-vnd.ca2.${_haiku_app_id}"
      COMMENT "Embedding the Haiku application icon for ${target}"
      VERBATIM)
endfunction()

set(USE_PKGCONFIG TRUE)
list(APPEND global_library_references network)
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


add_compile_definitions(__HAIKU__)

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


set(__SYSTEM "$ENV{__SYSTEM}")
if (NOT __SYSTEM)
   set(__SYSTEM "haiku")
endif()

if(NOT ${__TARGET_SYSTEM_ARCHITECTURE})
set(__TARGET_SYSTEM_ARCHITECTURE ${__SYSTEM_ARCHITECTURE})
endif()



message(STATUS "__SYSTEM_ARCHITECTURE is ${__SYSTEM_ARCHITECTURE}")


set(__TARGET_SYSTEM_ARCHITECTURE ${__SYSTEM_ARCHITECTURE})

message(STATUS "__TARGET_SYSTEM_ARCHITECTURE is ${__TARGET_SYSTEM_ARCHITECTURE}")


if(NOT $ENV{__SYSTEM} OR $ENV{__SYSTEM}  STREQUAL "")
message(STATUS "\$ENV{__SYSTEM} is (Empty)")
else()
message(STATUS "\$ENV{__SYSTEM} is $ENV{__SYSTEM}")
endif()

if ("${__SYSTEM}" STREQUAL "haiku")

   set(HAIKU TRUE)

   set(HAIKU_LIKE TRUE)

   #add_compile_definitions(UBUNTU_LINUX)

#   add_compile_definitions(DEBIAN_LIKE_LIBUILD_GPU_BASED_APPLICATIONSNUX)

   message(STATUS "HAIKU has been set TRUE")

   set(APPINDICATOR_PKG_MODULE "ayatana-appindicator3-0.1")

   #set(APPINDICATOR_PKG_MODULE "appindicator3-0.1")

   set(MPG123_PKG_MODULE "libmpg123")

   set(HAS_SYSTEM_UNAC FALSE)

   if(NOT ${MAIN_STORE_SLASHED_OPERATING_SYSTEM} OR ${MAIN_STORE_SLASHED_OPERATING_SYSTEM}  STREQUAL "")
	set(MAIN_STORE_SLASHED_OPERATING_SYSTEM "haiku")
   endif()
   
   if(NOT ${TOOL_RELEASE_NAME} OR ${TOOL_RELEASE_NAME}  STREQUAL "")
	set(TOOL_RELEASE_NAME "haiku")
   endif()
   
endif()


# Haiku uses app_server and the Interface Kit rather than an XDG desktop.
set(DESKTOP_ENVIRONMENT_NAME "haiku")



include("operating_system/operating_system-posix/_desktop_ambient_1.cmake")



set(default_write_text write_text_haiku)
set(default_draw2d draw2d_haiku)
set(default_node node_haiku)
set(default_acme_windowing acme_windowing_haiku)
set(default_windowing windowing_haiku)
set(default_imaging imaging_freeimage)
set(default_networking networking_bsd)
set(default_audio audio_sunaudio CACHE STRING "SunOS audio backend")
set(default_nano_graphics nano_graphics_cairo)



list(APPEND acme_libraries
        acme
        acme_posix
        acme_haiku)


list(APPEND static_acme_libraries
        static_acme
        static_acme_posix
        static_acme_haiku)


list(APPEND apex_libraries
        ${acme_libraries}
        apex
        apex_posix
        apex_haiku
)

list(APPEND aura_libraries
        ${apex_libraries}
        aura
        aura_posix
        aura_haiku
        node_haiku
)


if(${DESKTOP_AMBIENT})

   list(APPEND app_common_dependencies
           operating_ambient_haiku
           windowing_haiku
           acme_windowing_haiku
           nano_graphics_cairo
           ${aura_libraries})

else()

   list(APPEND app_common_dependencies
           nano_http_command_line
           nano_compress_command_line)

endif()



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


message("OPERATING_SYSTEM_NAME is ${OPERATING_SYSTEM_NAME}")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
#include_directories(${WORKSPACE_FOLDER})
#include_directories($ENV{HOME}/__config)
#include_directories(${WORKSPACE_FOLDER}/source)
#include_directories(${WORKSPACE_FOLDER}/source/app)
#include_directories(${WORKSPACE_FOLDER}/source/app/include)
#include_directories(${WORKSPACE_FOLDER}/source/include)
#include_directories(${WORKSPACE_FOLDER}/port/_)
include_directories(${WORKSPACE_FOLDER}/port/include)
include_directories(${WORKSPACE_FOLDER}/operating_system)
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


