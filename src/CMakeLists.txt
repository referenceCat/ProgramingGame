cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE SOURCES *.cpp )
message(${SOURCES})
add_executable(testBuild ${SOURCES})

target_include_directories(testBuild PUBLIC ui)
target_include_directories(testBuild PUBLIC logic)
target_include_directories(testBuild PUBLIC common)
target_include_directories(testBuild PUBLIC collision)


target_link_libraries(testBuild ${ALLEGRO5_LIBRARIES})
target_include_directories(testBuild PUBLIC ${ALLEGRO5_INCLUDE_DIRS})

find_package(nlohmann_json 3.7.0 REQUIRED)
target_link_libraries(testBuild nlohmann_json::nlohmann_json)

target_link_libraries(testBuild libcollision)
target_include_directories(testBuild PUBLIC ../libs/libcollision)