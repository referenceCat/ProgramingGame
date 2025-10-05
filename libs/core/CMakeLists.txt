cmake_minimum_required(VERSION 3.28)

# thanks to https://github.com/albertnadal/GJKCollisionDetection

file(GLOB_RECURSE SOURCES *.cpp )
add_library(core OBJECT ${SOURCES})

target_include_directories(core PUBLIC include)
target_include_directories(core PUBLIC src)
