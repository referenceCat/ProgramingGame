cmake_minimum_required(VERSION 3.28)

add_executable(DevProject main.cpp logic/GameWorld.cpp common/common.cpp logic/ManipulatorArm.cpp)

target_include_directories(DevProject PUBLIC logic})
target_include_directories(DevProject PUBLIC common})
target_link_libraries(DevProject ${ALLEGRO5_LIBRARIES})
target_include_directories(DevProject PUBLIC ${ALLEGRO5_INCLUDE_DIRS})