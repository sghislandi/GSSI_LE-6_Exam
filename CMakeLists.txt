cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(GSSI-LE-6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Ofast")

MAKE_DIRECTORY(figs/Exercise2)
add_executable(Exercise2_1 src/Exercise2/Exercise2_1.cpp)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}")

MAKE_DIRECTORY(figs/Exercise1)
add_executable(Exercise1 src/Exercise1/Exercise1.cpp)
target_link_libraries(Exercise1 "${ROOT_LIBRARIES}" )

add_executable(Exercise2 src/Exercise2/Exercise2.cpp)
target_link_libraries(Exercise2 "${ROOT_LIBRARIES}" )

MAKE_DIRECTORY(figs/Exercise3)
add_executable(Exercise3 src/Exercise3/Exercise3.cpp)
target_link_libraries(Exercise3 "${ROOT_LIBRARIES}" )

MAKE_DIRECTORY(figs/Exercise4)
add_executable(Exercise4_1 src/Exercise4/Exercise4_1.cpp)
target_link_libraries(Exercise4_1 "${ROOT_LIBRARIES}" )
add_executable(Exercise4_2 src/Exercise4/Exercise4_2.cpp)
target_link_libraries(Exercise4_2 "${ROOT_LIBRARIES}" )

MAKE_DIRECTORY(figs/Exercise5)
add_executable(Exercise5_1 src/Exercise5/Exercise5_1.cpp)
target_link_libraries(Exercise5_1 "${ROOT_LIBRARIES}" )
add_executable(Exercise5_2 src/Exercise5/Exercise5_2.cpp)
target_link_libraries(Exercise5_2 "${ROOT_LIBRARIES}" )
add_executable(Exercise5_3 src/Exercise5/Exercise5_3.cpp)
target_link_libraries(Exercise5_3 "${ROOT_LIBRARIES}" )

MAKE_DIRECTORY(figs/Exercise6)
add_executable(Exercise6 src/Exercise6/Exercise6.cpp)
target_link_libraries(Exercise6 "${ROOT_LIBRARIES}" )

MAKE_DIRECTORY(figs/Exercise7)
add_executable(Exercise7 src/Exercise7/Exercise7.cpp)
target_link_libraries(Exercise7 "${ROOT_LIBRARIES}" )

MAKE_DIRECTORY(figs/Exercise8)
add_executable(Exercise8_1 src/Exercise8/Exercise8_1.cpp)
target_link_libraries(Exercise8_1 "${ROOT_LIBRARIES}" )
add_executable(Exercise8_2 src/Exercise8/Exercise8_2.cpp)
target_link_libraries(Exercise8_2 "${ROOT_LIBRARIES}" )
