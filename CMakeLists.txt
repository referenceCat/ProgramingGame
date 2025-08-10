cmake_minimum_required(VERSION 3.28)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(DevProject CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5)

add_subdirectory(src)