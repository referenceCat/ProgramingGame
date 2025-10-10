cmake_minimum_required(VERSION 3.28)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set (CMAKE_CXX_STANDARD 20)

project(ProgramingGame CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5 allegro_image-5)

# Disable clip examples and tests
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

# In case that you have ${PNG_LIBRARY} set to support copy/paste images on Linux
#set(CLIP_X11_PNG_LIBRARY "${PNG_LIBRARY}")

add_subdirectory(libs/clip)
add_subdirectory(libs/core)
add_subdirectory(libs/libcollision)
add_subdirectory(src)