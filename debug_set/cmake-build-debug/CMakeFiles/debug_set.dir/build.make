# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.2.2\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.2.2\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Maxim\CLionProjects\debug_set

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Maxim\CLionProjects\debug_set\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/debug_set.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/debug_set.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/debug_set.dir/flags.make

CMakeFiles/debug_set.dir/main.cpp.obj: CMakeFiles/debug_set.dir/flags.make
CMakeFiles/debug_set.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Maxim\CLionProjects\debug_set\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/debug_set.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\debug_set.dir\main.cpp.obj -c C:\Users\Maxim\CLionProjects\debug_set\main.cpp

CMakeFiles/debug_set.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/debug_set.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Maxim\CLionProjects\debug_set\main.cpp > CMakeFiles\debug_set.dir\main.cpp.i

CMakeFiles/debug_set.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/debug_set.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Maxim\CLionProjects\debug_set\main.cpp -o CMakeFiles\debug_set.dir\main.cpp.s

CMakeFiles/debug_set.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/debug_set.dir/main.cpp.obj.requires

CMakeFiles/debug_set.dir/main.cpp.obj.provides: CMakeFiles/debug_set.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\debug_set.dir\build.make CMakeFiles/debug_set.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/debug_set.dir/main.cpp.obj.provides

CMakeFiles/debug_set.dir/main.cpp.obj.provides.build: CMakeFiles/debug_set.dir/main.cpp.obj


# Object files for target debug_set
debug_set_OBJECTS = \
"CMakeFiles/debug_set.dir/main.cpp.obj"

# External object files for target debug_set
debug_set_EXTERNAL_OBJECTS =

debug_set.exe: CMakeFiles/debug_set.dir/main.cpp.obj
debug_set.exe: CMakeFiles/debug_set.dir/build.make
debug_set.exe: CMakeFiles/debug_set.dir/linklibs.rsp
debug_set.exe: CMakeFiles/debug_set.dir/objects1.rsp
debug_set.exe: CMakeFiles/debug_set.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Maxim\CLionProjects\debug_set\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable debug_set.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\debug_set.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/debug_set.dir/build: debug_set.exe

.PHONY : CMakeFiles/debug_set.dir/build

CMakeFiles/debug_set.dir/requires: CMakeFiles/debug_set.dir/main.cpp.obj.requires

.PHONY : CMakeFiles/debug_set.dir/requires

CMakeFiles/debug_set.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\debug_set.dir\cmake_clean.cmake
.PHONY : CMakeFiles/debug_set.dir/clean

CMakeFiles/debug_set.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Maxim\CLionProjects\debug_set C:\Users\Maxim\CLionProjects\debug_set C:\Users\Maxim\CLionProjects\debug_set\cmake-build-debug C:\Users\Maxim\CLionProjects\debug_set\cmake-build-debug C:\Users\Maxim\CLionProjects\debug_set\cmake-build-debug\CMakeFiles\debug_set.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/debug_set.dir/depend

