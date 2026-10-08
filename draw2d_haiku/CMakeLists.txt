cmake_minimum_required(VERSION 3.17)
project(draw2d_haiku CXX)
add_library(${PROJECT_NAME} SHARED _factory.cpp bitmap.cpp graphics.cpp image.cpp font.cpp brush.cpp pen.cpp path.cpp region.cpp draw2d.cpp object.cpp internal_font.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -fpermissive)
target_link_libraries(${PROJECT_NAME} PRIVATE acme apex aqua aura be)
target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)
# Do not add this directory with -I: image.h would shadow Haiku's image.h.
