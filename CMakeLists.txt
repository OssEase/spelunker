# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

cmake_minimum_required(VERSION 3.9)
project(spelunker_top)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Find includes in corresponding build directories.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost 1.54 COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/src")

#if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#    add_definitions(-DDEBUG)
#endif()

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(gui)
