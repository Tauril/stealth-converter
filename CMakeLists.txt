project(stealth-converter CXX)

cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Wextra -Wall")

find_package(OpenCV REQUIRED)

include_directories("src/")
add_subdirectory("src/stream-reader/")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("lib/")

add_executable(stealth-converter src/stealth-converter.cc
               $<TARGET_OBJECTS:STREAM_READER>)

target_link_libraries(stealth-converter ${OpenCV_LIBS} ${FFMPEG_LIBS})
