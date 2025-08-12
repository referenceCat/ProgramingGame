cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE SOURCES *.cpp )
message(${SOURCES})
add_executable(testBuild ${SOURCES})

target_include_directories(testBuild PUBLIC ui)
target_include_directories(testBuild PUBLIC logic)
target_include_directories(testBuild PUBLIC common)

target_link_libraries(testBuild ${ALLEGRO5_LIBRARIES})
target_include_directories(testBuild PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
