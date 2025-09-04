cmake_minimum_required(VERSION 3.28)

# thanks to https://github.com/albertnadal/GJKCollisionDetection

file(GLOB_RECURSE SOURCES *.cpp )
add_library(libcollision STATIC ${SOURCES})

target_include_directories(libcollision PUBLIC algorithm)
target_include_directories(libcollision PUBLIC geometry)
target_include_directories(libcollision PUBLIC math)
target_include_directories(libcollision PUBLIC structures)
