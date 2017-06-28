project(stealth-converter CXX)

cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Werror -Wextra -Wall")

find_package(OpenCV REQUIRED)

include_directories("src/")

add_executable(stealth-converter src/stealth-converter.cc)

target_link_libraries(stealth-converter ${OpenCV_LIBS})
