# link testing lib
find_package(Catch2 REQUIRED)

# using C++17 in the tests
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# link the treelib and Catch2
link_libraries(treelib Catch2::Catch2WithMain)

# add tests as executables first
add_executable(test_avl avl.cpp)
add_executable(test_stress stress.cpp)

# register tests' targets for ctest and make test to run it
add_test(NAME test_avl COMMAND test_avl)
add_test(NAME test_stress COMMAND test_stress)

