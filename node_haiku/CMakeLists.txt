cmake_minimum_required(VERSION 3.17)
project(node_haiku CXX)


list(APPEND library_source
    _.cpp
    _.h
    _factory.cpp
    node.h
    node.cpp
    )


add_library(${PROJECT_NAME} SHARED ${library_source})


list(APPEND library_references
   acme
   apex
   aqua
   aura
   acme_posix
   acme_haiku
   apex_posix
   aura_posix
   apex_haiku
   aura_haiku
   )


target_precompile_headers(${PROJECT_NAME} PRIVATE platform.h)


set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


if (MSVC)


   target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
   target_link_options(${PROJECT_NAME} PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
   target_link_options(${PROJECT_NAME} PRIVATE "/IMPLIB:${LIBRARY_OUTPUT_PATH}/${PROJECT_NAME}.lib")
   target_link_options(${PROJECT_NAME} PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.def")
   target_link_libraries(${PROJECT_NAME} PRIVATE ${library_references})


elseif(${__HAIKU__})


   pkg_check_modules(PKGCONFIG x11)
   target_compile_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
   target_link_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_LDFLAGS})
   target_link_libraries(${PROJECT_NAME} PRIVATE pthread ${library_references})
   target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)


endif()


target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)



