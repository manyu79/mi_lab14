# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/abelani/mi_lab14

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/abelani/mi_lab14/build

# Include any dependencies generated for this target.
include src/pXRelayTest/CMakeFiles/pXRelayTest.dir/depend.make

# Include the progress variables for this target.
include src/pXRelayTest/CMakeFiles/pXRelayTest.dir/progress.make

# Include the compile flags for this target's objects.
include src/pXRelayTest/CMakeFiles/pXRelayTest.dir/flags.make

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/flags.make
src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o: ../src/pXRelayTest/Relayer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/abelani/mi_lab14/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pXRelayTest.dir/Relayer.cpp.o -c /Users/abelani/mi_lab14/src/pXRelayTest/Relayer.cpp

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pXRelayTest.dir/Relayer.cpp.i"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/abelani/mi_lab14/src/pXRelayTest/Relayer.cpp > CMakeFiles/pXRelayTest.dir/Relayer.cpp.i

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pXRelayTest.dir/Relayer.cpp.s"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/abelani/mi_lab14/src/pXRelayTest/Relayer.cpp -o CMakeFiles/pXRelayTest.dir/Relayer.cpp.s

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.requires:
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.requires

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.provides: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.requires
	$(MAKE) -f src/pXRelayTest/CMakeFiles/pXRelayTest.dir/build.make src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.provides.build
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.provides

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.provides.build: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/flags.make
src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o: ../src/pXRelayTest/Relayer_Info.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/abelani/mi_lab14/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o -c /Users/abelani/mi_lab14/src/pXRelayTest/Relayer_Info.cpp

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.i"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/abelani/mi_lab14/src/pXRelayTest/Relayer_Info.cpp > CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.i

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.s"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/abelani/mi_lab14/src/pXRelayTest/Relayer_Info.cpp -o CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.s

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.requires:
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.requires

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.provides: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.requires
	$(MAKE) -f src/pXRelayTest/CMakeFiles/pXRelayTest.dir/build.make src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.provides.build
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.provides

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.provides.build: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/flags.make
src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o: ../src/pXRelayTest/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/abelani/mi_lab14/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pXRelayTest.dir/main.cpp.o -c /Users/abelani/mi_lab14/src/pXRelayTest/main.cpp

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pXRelayTest.dir/main.cpp.i"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/abelani/mi_lab14/src/pXRelayTest/main.cpp > CMakeFiles/pXRelayTest.dir/main.cpp.i

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pXRelayTest.dir/main.cpp.s"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/abelani/mi_lab14/src/pXRelayTest/main.cpp -o CMakeFiles/pXRelayTest.dir/main.cpp.s

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.requires:
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.requires

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.provides: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.requires
	$(MAKE) -f src/pXRelayTest/CMakeFiles/pXRelayTest.dir/build.make src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.provides.build
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.provides

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.provides.build: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o

# Object files for target pXRelayTest
pXRelayTest_OBJECTS = \
"CMakeFiles/pXRelayTest.dir/Relayer.cpp.o" \
"CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o" \
"CMakeFiles/pXRelayTest.dir/main.cpp.o"

# External object files for target pXRelayTest
pXRelayTest_EXTERNAL_OBJECTS =

../bin/pXRelayTest: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o
../bin/pXRelayTest: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o
../bin/pXRelayTest: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o
../bin/pXRelayTest: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/build.make
../bin/pXRelayTest: /Users/abelani/moos-ivp/MOOS/MOOSCore/lib/libMOOS.a
../bin/pXRelayTest: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/pXRelayTest"
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pXRelayTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/pXRelayTest/CMakeFiles/pXRelayTest.dir/build: ../bin/pXRelayTest
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/build

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/requires: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer.cpp.o.requires
src/pXRelayTest/CMakeFiles/pXRelayTest.dir/requires: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/Relayer_Info.cpp.o.requires
src/pXRelayTest/CMakeFiles/pXRelayTest.dir/requires: src/pXRelayTest/CMakeFiles/pXRelayTest.dir/main.cpp.o.requires
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/requires

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/clean:
	cd /Users/abelani/mi_lab14/build/src/pXRelayTest && $(CMAKE_COMMAND) -P CMakeFiles/pXRelayTest.dir/cmake_clean.cmake
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/clean

src/pXRelayTest/CMakeFiles/pXRelayTest.dir/depend:
	cd /Users/abelani/mi_lab14/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/abelani/mi_lab14 /Users/abelani/mi_lab14/src/pXRelayTest /Users/abelani/mi_lab14/build /Users/abelani/mi_lab14/build/src/pXRelayTest /Users/abelani/mi_lab14/build/src/pXRelayTest/CMakeFiles/pXRelayTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/pXRelayTest/CMakeFiles/pXRelayTest.dir/depend

