# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mac/Documents/c++/limitorderbook

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mac/Documents/c++/limitorderbook

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "No interactive CMake dialog available..."
	/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/mac/Documents/c++/limitorderbook/CMakeFiles /Users/mac/Documents/c++/limitorderbook//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/mac/Documents/c++/limitorderbook/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named LimitOrderBook_lib

# Build rule for target.
LimitOrderBook_lib: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 LimitOrderBook_lib
.PHONY : LimitOrderBook_lib

# fast build rule for target.
LimitOrderBook_lib/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/build
.PHONY : LimitOrderBook_lib/fast

#=============================================================================
# Target rules for targets named LimitOrderBook

# Build rule for target.
LimitOrderBook: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 LimitOrderBook
.PHONY : LimitOrderBook

# fast build rule for target.
LimitOrderBook/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook.dir/build.make CMakeFiles/LimitOrderBook.dir/build
.PHONY : LimitOrderBook/fast

src/Limit/limit.o: src/Limit/limit.cpp.o
.PHONY : src/Limit/limit.o

# target to build an object file
src/Limit/limit.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/Limit/limit.cpp.o
.PHONY : src/Limit/limit.cpp.o

src/Limit/limit.i: src/Limit/limit.cpp.i
.PHONY : src/Limit/limit.i

# target to preprocess a source file
src/Limit/limit.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/Limit/limit.cpp.i
.PHONY : src/Limit/limit.cpp.i

src/Limit/limit.s: src/Limit/limit.cpp.s
.PHONY : src/Limit/limit.s

# target to generate assembly for a file
src/Limit/limit.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/Limit/limit.cpp.s
.PHONY : src/Limit/limit.cpp.s

src/Order/order.o: src/Order/order.cpp.o
.PHONY : src/Order/order.o

# target to build an object file
src/Order/order.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/Order/order.cpp.o
.PHONY : src/Order/order.cpp.o

src/Order/order.i: src/Order/order.cpp.i
.PHONY : src/Order/order.i

# target to preprocess a source file
src/Order/order.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/Order/order.cpp.i
.PHONY : src/Order/order.cpp.i

src/Order/order.s: src/Order/order.cpp.s
.PHONY : src/Order/order.s

# target to generate assembly for a file
src/Order/order.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/Order/order.cpp.s
.PHONY : src/Order/order.cpp.s

src/OrderBook/order_book.o: src/OrderBook/order_book.cpp.o
.PHONY : src/OrderBook/order_book.o

# target to build an object file
src/OrderBook/order_book.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/OrderBook/order_book.cpp.o
.PHONY : src/OrderBook/order_book.cpp.o

src/OrderBook/order_book.i: src/OrderBook/order_book.cpp.i
.PHONY : src/OrderBook/order_book.i

# target to preprocess a source file
src/OrderBook/order_book.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/OrderBook/order_book.cpp.i
.PHONY : src/OrderBook/order_book.cpp.i

src/OrderBook/order_book.s: src/OrderBook/order_book.cpp.s
.PHONY : src/OrderBook/order_book.s

# target to generate assembly for a file
src/OrderBook/order_book.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/OrderBook/order_book.cpp.s
.PHONY : src/OrderBook/order_book.cpp.s

src/OrderGenerator/order_generator.o: src/OrderGenerator/order_generator.cpp.o
.PHONY : src/OrderGenerator/order_generator.o

# target to build an object file
src/OrderGenerator/order_generator.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/OrderGenerator/order_generator.cpp.o
.PHONY : src/OrderGenerator/order_generator.cpp.o

src/OrderGenerator/order_generator.i: src/OrderGenerator/order_generator.cpp.i
.PHONY : src/OrderGenerator/order_generator.i

# target to preprocess a source file
src/OrderGenerator/order_generator.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/OrderGenerator/order_generator.cpp.i
.PHONY : src/OrderGenerator/order_generator.cpp.i

src/OrderGenerator/order_generator.s: src/OrderGenerator/order_generator.cpp.s
.PHONY : src/OrderGenerator/order_generator.s

# target to generate assembly for a file
src/OrderGenerator/order_generator.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook_lib.dir/build.make CMakeFiles/LimitOrderBook_lib.dir/src/OrderGenerator/order_generator.cpp.s
.PHONY : src/OrderGenerator/order_generator.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook.dir/build.make CMakeFiles/LimitOrderBook.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook.dir/build.make CMakeFiles/LimitOrderBook.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/LimitOrderBook.dir/build.make CMakeFiles/LimitOrderBook.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... LimitOrderBook"
	@echo "... LimitOrderBook_lib"
	@echo "... src/Limit/limit.o"
	@echo "... src/Limit/limit.i"
	@echo "... src/Limit/limit.s"
	@echo "... src/Order/order.o"
	@echo "... src/Order/order.i"
	@echo "... src/Order/order.s"
	@echo "... src/OrderBook/order_book.o"
	@echo "... src/OrderBook/order_book.i"
	@echo "... src/OrderBook/order_book.s"
	@echo "... src/OrderGenerator/order_generator.o"
	@echo "... src/OrderGenerator/order_generator.i"
	@echo "... src/OrderGenerator/order_generator.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

