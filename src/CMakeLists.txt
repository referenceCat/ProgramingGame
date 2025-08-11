cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE SOURCES *.cpp )
message(${SOURCES})
add_executable(DevProject ${SOURCES})

target_include_directories(DevProject PUBLIC logic})
target_include_directories(DevProject PUBLIC common})
target_link_libraries(DevProject ${ALLEGRO5_LIBRARIES})
target_include_directories(DevProject PUBLIC ${ALLEGRO5_INCLUDE_DIRS})