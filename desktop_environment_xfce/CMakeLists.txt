cmake_minimum_required(VERSION 3.17)
project(desktop_environment_xfce CXX)


list(APPEND library_source
   _.cpp
   _.h
        _desktop_environment_xfce.h
        #appindicator.cpp
        #appindicator.h
        #application.cpp
        #application.h
        #clipboard_data.h
        #copydesk.cpp
        #copydesk.h
        #display.cpp
        #display.h
        #enum_display_monitors.cpp
   _factory.cpp
   platform.cpp
   platform.h
        #gdk.cpp
        #gdk.h
        #xfce_direct.cpp
        #gtk_clipboard.cpp
        #monitor.cpp
        #monitor.h
   node.cpp
   node.h
        #notify_icon.cpp
        #windowing.cpp
        #windowing.h
   )


list(APPEND library_references
   acme
   acme_posix
   acme_haiku
   apex
   apex_posix
   apex_haiku
   aqua
   aura
   aura_posix
   aura_haiku
   windowing_x11
   node_gtk
   node_xfce
   )



add_library(desktop_environment_xfce SHARED ${library_source})

target_precompile_headers(desktop_environment_xfce PRIVATE platform.h)

set_target_properties(desktop_environment_xfce PROPERTIES LINKER_LANGUAGE CXX)


if (MSVC)
   target_compile_options(desktop_environment_xfce PRIVATE "/MD$<$<CONFIG:Debug>:d>")
   target_link_options(desktop_environment_xfce PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
   target_link_options(desktop_environment_xfce PRIVATE "/SUBSYSTEM:WINDOWS")
   target_link_libraries(desktop_environment_xfce PRIVATE ${library_references})
elseif(__HAIKU__ OR __HAIKU__)
   pkg_check_modules(PKGCONFIG glib-2.0 gtk+-3.0 gdk-3.0 appindicator3-0.1)
   target_compile_options(desktop_environment_xfce PRIVATE -fpermissive)
      target_compile_options(desktop_environment_xfce PRIVATE ${PKGCONFIG_CFLAGS})
   target_link_options(desktop_environment_xfce PRIVATE ${PKGCONFIG_LDFLAGS})
   #target_link_options(desktop_environment_xfce PRIVATE -Wl,-rpath=$ORIGIN)
   target_link_libraries(desktop_environment_xfce PRIVATE ${library_references} ${PKGCONFIG_LIBRARIES})
   target_link_options(desktop_environment_xfce PRIVATE -Wl,-z,defs)
endif ()


list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../../operating_system-darwin)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../../operating_system-posix)


target_include_directories(${PROJECT_NAME} PRIVATE ${library_include_directories})
target_include_directories(static_${PROJECT_NAME} PRIVATE ${library_include_directories})


target_compile_definitions(desktop_environment_xfce PRIVATE _desktop_environment_xfce_LIBRARY)



