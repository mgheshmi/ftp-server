# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mahdi/Desktop/ftp_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mahdi/Desktop/ftp_server/build

# Include any dependencies generated for this target.
include CMakeFiles/ftp_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ftp_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ftp_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ftp_server.dir/flags.make

CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o: CMakeFiles/ftp_server.dir/flags.make
CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o: /home/mahdi/Desktop/ftp_server/src/ftp_server.cpp
CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o: CMakeFiles/ftp_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mahdi/Desktop/ftp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o -MF CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o.d -o CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o -c /home/mahdi/Desktop/ftp_server/src/ftp_server.cpp

CMakeFiles/ftp_server.dir/src/ftp_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ftp_server.dir/src/ftp_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mahdi/Desktop/ftp_server/src/ftp_server.cpp > CMakeFiles/ftp_server.dir/src/ftp_server.cpp.i

CMakeFiles/ftp_server.dir/src/ftp_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ftp_server.dir/src/ftp_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mahdi/Desktop/ftp_server/src/ftp_server.cpp -o CMakeFiles/ftp_server.dir/src/ftp_server.cpp.s

# Object files for target ftp_server
ftp_server_OBJECTS = \
"CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o"

# External object files for target ftp_server
ftp_server_EXTERNAL_OBJECTS =

ftp_server: CMakeFiles/ftp_server.dir/src/ftp_server.cpp.o
ftp_server: CMakeFiles/ftp_server.dir/build.make
ftp_server: CMakeFiles/ftp_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/mahdi/Desktop/ftp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ftp_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ftp_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ftp_server.dir/build: ftp_server
.PHONY : CMakeFiles/ftp_server.dir/build

CMakeFiles/ftp_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ftp_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ftp_server.dir/clean

CMakeFiles/ftp_server.dir/depend:
	cd /home/mahdi/Desktop/ftp_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mahdi/Desktop/ftp_server /home/mahdi/Desktop/ftp_server /home/mahdi/Desktop/ftp_server/build /home/mahdi/Desktop/ftp_server/build /home/mahdi/Desktop/ftp_server/build/CMakeFiles/ftp_server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ftp_server.dir/depend

