# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /home/afonso/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/171.4073.41/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/afonso/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/171.4073.41/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug"

# Include any dependencies generated for this target.
include generator/CMakeFiles/generator.dir/depend.make

# Include the progress variables for this target.
include generator/CMakeFiles/generator.dir/progress.make

# Include the compile flags for this target's objects.
include generator/CMakeFiles/generator.dir/flags.make

generator/CMakeFiles/generator.dir/generator.c.o: generator/CMakeFiles/generator.dir/flags.make
generator/CMakeFiles/generator.dir/generator.c.o: ../generator/generator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object generator/CMakeFiles/generator.dir/generator.c.o"
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/generator.dir/generator.c.o   -c "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/generator/generator.c"

generator/CMakeFiles/generator.dir/generator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/generator.dir/generator.c.i"
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/generator/generator.c" > CMakeFiles/generator.dir/generator.c.i

generator/CMakeFiles/generator.dir/generator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/generator.dir/generator.c.s"
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/generator/generator.c" -o CMakeFiles/generator.dir/generator.c.s

generator/CMakeFiles/generator.dir/generator.c.o.requires:

.PHONY : generator/CMakeFiles/generator.dir/generator.c.o.requires

generator/CMakeFiles/generator.dir/generator.c.o.provides: generator/CMakeFiles/generator.dir/generator.c.o.requires
	$(MAKE) -f generator/CMakeFiles/generator.dir/build.make generator/CMakeFiles/generator.dir/generator.c.o.provides.build
.PHONY : generator/CMakeFiles/generator.dir/generator.c.o.provides

generator/CMakeFiles/generator.dir/generator.c.o.provides.build: generator/CMakeFiles/generator.dir/generator.c.o


generator/CMakeFiles/generator.dir/model.c.o: generator/CMakeFiles/generator.dir/flags.make
generator/CMakeFiles/generator.dir/model.c.o: ../generator/model.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object generator/CMakeFiles/generator.dir/model.c.o"
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/generator.dir/model.c.o   -c "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/generator/model.c"

generator/CMakeFiles/generator.dir/model.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/generator.dir/model.c.i"
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/generator/model.c" > CMakeFiles/generator.dir/model.c.i

generator/CMakeFiles/generator.dir/model.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/generator.dir/model.c.s"
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/generator/model.c" -o CMakeFiles/generator.dir/model.c.s

generator/CMakeFiles/generator.dir/model.c.o.requires:

.PHONY : generator/CMakeFiles/generator.dir/model.c.o.requires

generator/CMakeFiles/generator.dir/model.c.o.provides: generator/CMakeFiles/generator.dir/model.c.o.requires
	$(MAKE) -f generator/CMakeFiles/generator.dir/build.make generator/CMakeFiles/generator.dir/model.c.o.provides.build
.PHONY : generator/CMakeFiles/generator.dir/model.c.o.provides

generator/CMakeFiles/generator.dir/model.c.o.provides.build: generator/CMakeFiles/generator.dir/model.c.o


# Object files for target generator
generator_OBJECTS = \
"CMakeFiles/generator.dir/generator.c.o" \
"CMakeFiles/generator.dir/model.c.o"

# External object files for target generator
generator_EXTERNAL_OBJECTS =

generator/generator: generator/CMakeFiles/generator.dir/generator.c.o
generator/generator: generator/CMakeFiles/generator.dir/model.c.o
generator/generator: generator/CMakeFiles/generator.dir/build.make
generator/generator: generator/CMakeFiles/generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable generator"
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
generator/CMakeFiles/generator.dir/build: generator/generator

.PHONY : generator/CMakeFiles/generator.dir/build

generator/CMakeFiles/generator.dir/requires: generator/CMakeFiles/generator.dir/generator.c.o.requires
generator/CMakeFiles/generator.dir/requires: generator/CMakeFiles/generator.dir/model.c.o.requires

.PHONY : generator/CMakeFiles/generator.dir/requires

generator/CMakeFiles/generator.dir/clean:
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" && $(CMAKE_COMMAND) -P CMakeFiles/generator.dir/cmake_clean.cmake
.PHONY : generator/CMakeFiles/generator.dir/clean

generator/CMakeFiles/generator.dir/depend:
	cd "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment" "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/generator" "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug" "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator" "/home/afonso/Documentos/Licenciatura em Engenharia Informática/Computação Gráfica/assignment/cmake-build-debug/generator/CMakeFiles/generator.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : generator/CMakeFiles/generator.dir/depend

