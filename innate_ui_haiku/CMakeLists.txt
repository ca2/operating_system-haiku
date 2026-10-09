cmake_minimum_required(VERSION 3.17)
project(innate_ui_haiku CXX)
add_library(${PROJECT_NAME} SHARED
 _.cpp _factory.cpp platform.cpp window.cpp button.cpp still.cpp dialog.cpp innate_ui.cpp menu.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
 ${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE _innate_ui_haiku_project)
target_link_libraries(${PROJECT_NAME} PRIVATE acme apex acme_windowing_haiku be)
target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)
