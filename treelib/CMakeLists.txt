cmake_minimum_required(VERSION 3.1...3.19)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(treelib
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -Wextra")

add_library(treelib INTERFACE)
target_include_directories(treelib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(treelib INTERFACE cxx_std_17)

if(BUILD_TESTS)
	include(CTest)
  	enable_testing()
    add_subdirectory(tests)
endif()