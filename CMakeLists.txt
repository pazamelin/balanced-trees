cmake_minimum_required(VERSION 3.1...3.19)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(balanced_trees)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "build tests executables" ON)
option(BUILD_PROFILER "build profiler executable" ON)

message(STATUS "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_TESTS : ${BUILD_TESTS}")
message(STATUS "BUILD_PROFILER : ${BUILD_PROFILER}")

if (BUILD_PROFILER)
    add_subdirectory(profiler)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    add_subdirectory(treelib)
endif()