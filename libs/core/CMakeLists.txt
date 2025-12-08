cmake_minimum_required(VERSION 3.28)

# thanks to https://github.com/albertnadal/GJKCollisionDetection

file(GLOB_RECURSE SOURCES *.cpp )
add_library(core OBJECT ${SOURCES})

find_package(nlohmann_json 3.7.0 REQUIRED)
target_link_libraries(core nlohmann_json::nlohmann_json)
target_link_libraries(core clip)

target_include_directories(core PUBLIC include)
target_include_directories(core PUBLIC src)
target_include_directories(core PUBLIC ../clip)
