# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /apps/sylar/bin/cmake

# The command to remove a file.
RM = /apps/sylar/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/workspace/tw_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/tw_server/build

# Include any dependencies generated for this target.
include CMakeFiles/test_bytearray.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_bytearray.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_bytearray.dir/flags.make

CMakeFiles/test_bytearray.dir/tw/log.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/log.cc.o: ../tw/log.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_bytearray.dir/tw/log.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/log.cc.o -c /root/workspace/tw_server/tw/log.cc

CMakeFiles/test_bytearray.dir/tw/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/log.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/log.cc > CMakeFiles/test_bytearray.dir/tw/log.cc.i

CMakeFiles/test_bytearray.dir/tw/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/log.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/log.cc -o CMakeFiles/test_bytearray.dir/tw/log.cc.s

CMakeFiles/test_bytearray.dir/tw/util.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/util.cc.o: ../tw/util.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_bytearray.dir/tw/util.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/util.cc.o -c /root/workspace/tw_server/tw/util.cc

CMakeFiles/test_bytearray.dir/tw/util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/util.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/util.cc > CMakeFiles/test_bytearray.dir/tw/util.cc.i

CMakeFiles/test_bytearray.dir/tw/util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/util.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/util.cc -o CMakeFiles/test_bytearray.dir/tw/util.cc.s

CMakeFiles/test_bytearray.dir/tw/thread.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/thread.cc.o: ../tw/thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_bytearray.dir/tw/thread.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/thread.cc.o -c /root/workspace/tw_server/tw/thread.cc

CMakeFiles/test_bytearray.dir/tw/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/thread.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/thread.cc > CMakeFiles/test_bytearray.dir/tw/thread.cc.i

CMakeFiles/test_bytearray.dir/tw/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/thread.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/thread.cc -o CMakeFiles/test_bytearray.dir/tw/thread.cc.s

CMakeFiles/test_bytearray.dir/tw/fiber.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/fiber.cc.o: ../tw/fiber.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_bytearray.dir/tw/fiber.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/fiber.cc.o -c /root/workspace/tw_server/tw/fiber.cc

CMakeFiles/test_bytearray.dir/tw/fiber.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/fiber.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/fiber.cc > CMakeFiles/test_bytearray.dir/tw/fiber.cc.i

CMakeFiles/test_bytearray.dir/tw/fiber.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/fiber.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/fiber.cc -o CMakeFiles/test_bytearray.dir/tw/fiber.cc.s

CMakeFiles/test_bytearray.dir/tw/scheduler.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/scheduler.cc.o: ../tw/scheduler.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_bytearray.dir/tw/scheduler.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/scheduler.cc.o -c /root/workspace/tw_server/tw/scheduler.cc

CMakeFiles/test_bytearray.dir/tw/scheduler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/scheduler.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/scheduler.cc > CMakeFiles/test_bytearray.dir/tw/scheduler.cc.i

CMakeFiles/test_bytearray.dir/tw/scheduler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/scheduler.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/scheduler.cc -o CMakeFiles/test_bytearray.dir/tw/scheduler.cc.s

CMakeFiles/test_bytearray.dir/tw/iomanager.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/iomanager.cc.o: ../tw/iomanager.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test_bytearray.dir/tw/iomanager.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/iomanager.cc.o -c /root/workspace/tw_server/tw/iomanager.cc

CMakeFiles/test_bytearray.dir/tw/iomanager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/iomanager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/iomanager.cc > CMakeFiles/test_bytearray.dir/tw/iomanager.cc.i

CMakeFiles/test_bytearray.dir/tw/iomanager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/iomanager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/iomanager.cc -o CMakeFiles/test_bytearray.dir/tw/iomanager.cc.s

CMakeFiles/test_bytearray.dir/tw/timer.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/timer.cc.o: ../tw/timer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/test_bytearray.dir/tw/timer.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/timer.cc.o -c /root/workspace/tw_server/tw/timer.cc

CMakeFiles/test_bytearray.dir/tw/timer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/timer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/timer.cc > CMakeFiles/test_bytearray.dir/tw/timer.cc.i

CMakeFiles/test_bytearray.dir/tw/timer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/timer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/timer.cc -o CMakeFiles/test_bytearray.dir/tw/timer.cc.s

CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.o: ../tw/fd_manager.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.o -c /root/workspace/tw_server/tw/fd_manager.cc

CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/fd_manager.cc > CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.i

CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/fd_manager.cc -o CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.s

CMakeFiles/test_bytearray.dir/tw/hook.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/hook.cc.o: ../tw/hook.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/test_bytearray.dir/tw/hook.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/hook.cc.o -c /root/workspace/tw_server/tw/hook.cc

CMakeFiles/test_bytearray.dir/tw/hook.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/hook.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/hook.cc > CMakeFiles/test_bytearray.dir/tw/hook.cc.i

CMakeFiles/test_bytearray.dir/tw/hook.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/hook.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/hook.cc -o CMakeFiles/test_bytearray.dir/tw/hook.cc.s

CMakeFiles/test_bytearray.dir/tw/address.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/address.cc.o: ../tw/address.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/test_bytearray.dir/tw/address.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/address.cc.o -c /root/workspace/tw_server/tw/address.cc

CMakeFiles/test_bytearray.dir/tw/address.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/address.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/address.cc > CMakeFiles/test_bytearray.dir/tw/address.cc.i

CMakeFiles/test_bytearray.dir/tw/address.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/address.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/address.cc -o CMakeFiles/test_bytearray.dir/tw/address.cc.s

CMakeFiles/test_bytearray.dir/tw/socket.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/socket.cc.o: ../tw/socket.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/test_bytearray.dir/tw/socket.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/socket.cc.o -c /root/workspace/tw_server/tw/socket.cc

CMakeFiles/test_bytearray.dir/tw/socket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/socket.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/socket.cc > CMakeFiles/test_bytearray.dir/tw/socket.cc.i

CMakeFiles/test_bytearray.dir/tw/socket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/socket.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/socket.cc -o CMakeFiles/test_bytearray.dir/tw/socket.cc.s

CMakeFiles/test_bytearray.dir/tw/bytearray.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/bytearray.cc.o: ../tw/bytearray.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/test_bytearray.dir/tw/bytearray.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/bytearray.cc.o -c /root/workspace/tw_server/tw/bytearray.cc

CMakeFiles/test_bytearray.dir/tw/bytearray.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/bytearray.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/bytearray.cc > CMakeFiles/test_bytearray.dir/tw/bytearray.cc.i

CMakeFiles/test_bytearray.dir/tw/bytearray.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/bytearray.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/bytearray.cc -o CMakeFiles/test_bytearray.dir/tw/bytearray.cc.s

CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.o: ../tw/tcp_server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.o -c /root/workspace/tw_server/tw/tcp_server.cc

CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/tcp_server.cc > CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.i

CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/tcp_server.cc -o CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.s

CMakeFiles/test_bytearray.dir/tw/http/http.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/tw/http/http.cc.o: ../tw/http/http.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/test_bytearray.dir/tw/http/http.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/tw/http/http.cc.o -c /root/workspace/tw_server/tw/http/http.cc

CMakeFiles/test_bytearray.dir/tw/http/http.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/tw/http/http.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/tw/http/http.cc > CMakeFiles/test_bytearray.dir/tw/http/http.cc.i

CMakeFiles/test_bytearray.dir/tw/http/http.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/tw/http/http.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/tw/http/http.cc -o CMakeFiles/test_bytearray.dir/tw/http/http.cc.s

CMakeFiles/test_bytearray.dir/test_bytearray.cc.o: CMakeFiles/test_bytearray.dir/flags.make
CMakeFiles/test_bytearray.dir/test_bytearray.cc.o: ../test_bytearray.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/test_bytearray.dir/test_bytearray.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_bytearray.dir/test_bytearray.cc.o -c /root/workspace/tw_server/test_bytearray.cc

CMakeFiles/test_bytearray.dir/test_bytearray.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_bytearray.dir/test_bytearray.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/tw_server/test_bytearray.cc > CMakeFiles/test_bytearray.dir/test_bytearray.cc.i

CMakeFiles/test_bytearray.dir/test_bytearray.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_bytearray.dir/test_bytearray.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/tw_server/test_bytearray.cc -o CMakeFiles/test_bytearray.dir/test_bytearray.cc.s

# Object files for target test_bytearray
test_bytearray_OBJECTS = \
"CMakeFiles/test_bytearray.dir/tw/log.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/util.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/thread.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/fiber.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/scheduler.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/iomanager.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/timer.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/hook.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/address.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/socket.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/bytearray.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.o" \
"CMakeFiles/test_bytearray.dir/tw/http/http.cc.o" \
"CMakeFiles/test_bytearray.dir/test_bytearray.cc.o"

# External object files for target test_bytearray
test_bytearray_EXTERNAL_OBJECTS =

test_bytearray: CMakeFiles/test_bytearray.dir/tw/log.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/util.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/thread.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/fiber.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/scheduler.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/iomanager.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/timer.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/fd_manager.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/hook.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/address.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/socket.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/bytearray.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/tcp_server.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/tw/http/http.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/test_bytearray.cc.o
test_bytearray: CMakeFiles/test_bytearray.dir/build.make
test_bytearray: CMakeFiles/test_bytearray.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/tw_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable test_bytearray"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_bytearray.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_bytearray.dir/build: test_bytearray

.PHONY : CMakeFiles/test_bytearray.dir/build

CMakeFiles/test_bytearray.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_bytearray.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_bytearray.dir/clean

CMakeFiles/test_bytearray.dir/depend:
	cd /root/workspace/tw_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/tw_server /root/workspace/tw_server /root/workspace/tw_server/build /root/workspace/tw_server/build /root/workspace/tw_server/build/CMakeFiles/test_bytearray.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_bytearray.dir/depend

