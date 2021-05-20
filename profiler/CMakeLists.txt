# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/profiler/*.hpp")
set(HEADER_LIST profiler.hpp)
set(SOURCE_LIST profiler.cpp)

add_executable(run_profiler ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(run_profiler PRIVATE treelib)
target_compile_features(run_profiler PUBLIC cxx_std_17)
