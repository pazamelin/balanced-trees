# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/139/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/139/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pazamelin/github/nis/nis_lab2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pazamelin/github/nis/nis_lab2/cmake-build-debug

# Include any dependencies generated for this target.
include treelib/tests/CMakeFiles/test_stress.dir/depend.make

# Include the progress variables for this target.
include treelib/tests/CMakeFiles/test_stress.dir/progress.make

# Include the compile flags for this target's objects.
include treelib/tests/CMakeFiles/test_stress.dir/flags.make

treelib/tests/CMakeFiles/test_stress.dir/stress.cpp.o: treelib/tests/CMakeFiles/test_stress.dir/flags.make
treelib/tests/CMakeFiles/test_stress.dir/stress.cpp.o: ../treelib/tests/stress.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pazamelin/github/nis/nis_lab2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object treelib/tests/CMakeFiles/test_stress.dir/stress.cpp.o"
	cd /home/pazamelin/github/nis/nis_lab2/cmake-build-debug/treelib/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_stress.dir/stress.cpp.o -c /home/pazamelin/github/nis/nis_lab2/treelib/tests/stress.cpp

treelib/tests/CMakeFiles/test_stress.dir/stress.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_stress.dir/stress.cpp.i"
	cd /home/pazamelin/github/nis/nis_lab2/cmake-build-debug/treelib/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pazamelin/github/nis/nis_lab2/treelib/tests/stress.cpp > CMakeFiles/test_stress.dir/stress.cpp.i

treelib/tests/CMakeFiles/test_stress.dir/stress.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_stress.dir/stress.cpp.s"
	cd /home/pazamelin/github/nis/nis_lab2/cmake-build-debug/treelib/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pazamelin/github/nis/nis_lab2/treelib/tests/stress.cpp -o CMakeFiles/test_stress.dir/stress.cpp.s

# Object files for target test_stress
test_stress_OBJECTS = \
"CMakeFiles/test_stress.dir/stress.cpp.o"

# External object files for target test_stress
test_stress_EXTERNAL_OBJECTS =

treelib/tests/test_stress: treelib/tests/CMakeFiles/test_stress.dir/stress.cpp.o
treelib/tests/test_stress: treelib/tests/CMakeFiles/test_stress.dir/build.make
treelib/tests/test_stress: /usr/local/lib/libCatch2Main.a
treelib/tests/test_stress: /usr/local/lib/libCatch2.a
treelib/tests/test_stress: treelib/tests/CMakeFiles/test_stress.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pazamelin/github/nis/nis_lab2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_stress"
	cd /home/pazamelin/github/nis/nis_lab2/cmake-build-debug/treelib/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_stress.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
treelib/tests/CMakeFiles/test_stress.dir/build: treelib/tests/test_stress

.PHONY : treelib/tests/CMakeFiles/test_stress.dir/build

treelib/tests/CMakeFiles/test_stress.dir/clean:
	cd /home/pazamelin/github/nis/nis_lab2/cmake-build-debug/treelib/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_stress.dir/cmake_clean.cmake
.PHONY : treelib/tests/CMakeFiles/test_stress.dir/clean

treelib/tests/CMakeFiles/test_stress.dir/depend:
	cd /home/pazamelin/github/nis/nis_lab2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pazamelin/github/nis/nis_lab2 /home/pazamelin/github/nis/nis_lab2/treelib/tests /home/pazamelin/github/nis/nis_lab2/cmake-build-debug /home/pazamelin/github/nis/nis_lab2/cmake-build-debug/treelib/tests /home/pazamelin/github/nis/nis_lab2/cmake-build-debug/treelib/tests/CMakeFiles/test_stress.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : treelib/tests/CMakeFiles/test_stress.dir/depend

