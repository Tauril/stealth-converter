project(stealth-converter CXX)

cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Wextra -Wall -g3")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

include_directories(".")
add_subdirectory("src/stream")

add_executable(stealth-converter src/stealth-converter.cc
               $<TARGET_OBJECTS:STREAM>)

find_package(OpenCV REQUIRED)
if (OPENCV_FOUND)
  message("Found OpenCV library")
  target_link_libraries(stealth-converter ${OpenCV_LIBS})
endif (OPENCV_FOUND)

find_package(FFmpeg REQUIRED)
if (FFMPEG_FOUND)
  message("Found FFMPEG/LibAV libraries")
  include_directories(${FFMPEG_INCLUDE_DIR})
  target_link_libraries(stealth-converter ${FFMPEG_LIBRARIES})
endif (FFMPEG_FOUND)

target_link_libraries(stealth-converter "-lm -lavresample")
