# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mfracker/csce/482/morphling/game-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mfracker/csce/482/morphling/game-server/build

# Include any dependencies generated for this target.
include networking/CMakeFiles/networking.dir/depend.make

# Include the progress variables for this target.
include networking/CMakeFiles/networking.dir/progress.make

# Include the compile flags for this target's objects.
include networking/CMakeFiles/networking.dir/flags.make

networking/CMakeFiles/networking.dir/stub.cc.o: networking/CMakeFiles/networking.dir/flags.make
networking/CMakeFiles/networking.dir/stub.cc.o: ../networking/stub.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mfracker/csce/482/morphling/game-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object networking/CMakeFiles/networking.dir/stub.cc.o"
	cd /home/mfracker/csce/482/morphling/game-server/build/networking && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networking.dir/stub.cc.o -c /home/mfracker/csce/482/morphling/game-server/networking/stub.cc

networking/CMakeFiles/networking.dir/stub.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networking.dir/stub.cc.i"
	cd /home/mfracker/csce/482/morphling/game-server/build/networking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mfracker/csce/482/morphling/game-server/networking/stub.cc > CMakeFiles/networking.dir/stub.cc.i

networking/CMakeFiles/networking.dir/stub.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networking.dir/stub.cc.s"
	cd /home/mfracker/csce/482/morphling/game-server/build/networking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mfracker/csce/482/morphling/game-server/networking/stub.cc -o CMakeFiles/networking.dir/stub.cc.s

networking/CMakeFiles/networking.dir/stub.cc.o.requires:

.PHONY : networking/CMakeFiles/networking.dir/stub.cc.o.requires

networking/CMakeFiles/networking.dir/stub.cc.o.provides: networking/CMakeFiles/networking.dir/stub.cc.o.requires
	$(MAKE) -f networking/CMakeFiles/networking.dir/build.make networking/CMakeFiles/networking.dir/stub.cc.o.provides.build
.PHONY : networking/CMakeFiles/networking.dir/stub.cc.o.provides

networking/CMakeFiles/networking.dir/stub.cc.o.provides.build: networking/CMakeFiles/networking.dir/stub.cc.o


# Object files for target networking
networking_OBJECTS = \
"CMakeFiles/networking.dir/stub.cc.o"

# External object files for target networking
networking_EXTERNAL_OBJECTS =

networking/libnetworking.a: networking/CMakeFiles/networking.dir/stub.cc.o
networking/libnetworking.a: networking/CMakeFiles/networking.dir/build.make
networking/libnetworking.a: networking/CMakeFiles/networking.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mfracker/csce/482/morphling/game-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libnetworking.a"
	cd /home/mfracker/csce/482/morphling/game-server/build/networking && $(CMAKE_COMMAND) -P CMakeFiles/networking.dir/cmake_clean_target.cmake
	cd /home/mfracker/csce/482/morphling/game-server/build/networking && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/networking.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
networking/CMakeFiles/networking.dir/build: networking/libnetworking.a

.PHONY : networking/CMakeFiles/networking.dir/build

networking/CMakeFiles/networking.dir/requires: networking/CMakeFiles/networking.dir/stub.cc.o.requires

.PHONY : networking/CMakeFiles/networking.dir/requires

networking/CMakeFiles/networking.dir/clean:
	cd /home/mfracker/csce/482/morphling/game-server/build/networking && $(CMAKE_COMMAND) -P CMakeFiles/networking.dir/cmake_clean.cmake
.PHONY : networking/CMakeFiles/networking.dir/clean

networking/CMakeFiles/networking.dir/depend:
	cd /home/mfracker/csce/482/morphling/game-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mfracker/csce/482/morphling/game-server /home/mfracker/csce/482/morphling/game-server/networking /home/mfracker/csce/482/morphling/game-server/build /home/mfracker/csce/482/morphling/game-server/build/networking /home/mfracker/csce/482/morphling/game-server/build/networking/CMakeFiles/networking.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : networking/CMakeFiles/networking.dir/depend

