cmake_minimum_required(VERSION 3.7)
project(Zirro)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )
set(SOURCE_FILES main.cpp bgsubcnt.cpp zirro.cpp)
add_executable(Zirro ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( Zirro ${OpenCV_LIBS} )