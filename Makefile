# To build the program (which is called mytop by default), simply type:
#   make
#
# To clean up and remove the compiled binary and other generated files, type:
#   make clean
#
# To build and run the unit tests, type:
#   make test
#
# To build AND run the shell, type:
#   make run
#

# The name of your binary.
NAME = mytop

# We default PROC_ROOT to ./proc for tests. This gets overridden when building
# the binary to the real location (/proc).
PROC_ROOT = ./proc

# If running on Alamode machines (which use a slow, distributed file system),
# put the fake proc folder in /dev/shm, which is an in-memory file system.
ifeq ($(shell hostname | cut -c1-5), bb136)
	PROC_BASE := $(shell mktemp -u --tmpdir=/dev/shm/)
else
	PROC_BASE := $(shell mktemp -u)
endif

# Flags passed to the preprocessor.
CPPFLAGS += -Wall -DPROC_ROOT='"$(PROC_ROOT)"' -MMD -MP -Isrc -std=c++14
TEST_CPPFLAGS = $(CPPFLAGS) -isystem $(GTEST_DIR)/include

# ALL .cpp files.
ALL_SRCS = $(shell find src -name '*.cpp')

# All .cpp files (excluding tests) under src/info. We handle the info/
# directory specially, since we need to build it two different ways. Tests
# define PROC_ROOT as ./proc, while the real binary sets PROC_ROOT to the actual
# value (/proc).
INFO_IMPL_SRCS = $(shell find src/info -name '*.cpp' -not -name '*_tests.cpp')
INFO_IMPL_OBJS = $(INFO_IMPL_SRCS:src/%.cpp=bin/%.o)
INFO_IMPL_DEPS = $(INFO_IMPL_SRCS:src/%.cpp=bin/%.d)
INFO_TEST_OBJS = $(INFO_IMPL_SRCS:src/%.cpp=bin/%_.o)
INFO_TEST_DEPS = $(INFO_IMPL_SRCS:src/%.cpp=bin/%_.d)

# The .cpp files under the root of src/. These are the only classes not compiled
# into the tests, since testing ncurses code is beyond the scope of what I think
# is reasonable for this class.
MAIN_SRCS = $(wildcard src/*.cpp)
MAIN_OBJS = $(MAIN_SRCS:src/%.cpp=bin/%.o)
MAIN_DEPS = $(MAIN_SRCS:src/%.cpp=bin/%.d)

# All other .cpp files, excluding any tests.
OTHER_SRCS = $(shell find src \
		-name '*.cpp' \
		-path 'src/*/*' \
		-not -path '*info*' \
		-not -name '*_tests.cpp')
OTHER_OBJS = $(OTHER_SRCS:src/%.cpp=bin/%.o)
OTHER_DEPS = $(OTHER_SRCS:src/%.cpp=bin/%.d)

# All test classes. This makes the assumption that test classes are always in a
# subdirectory of src.
TEST_SRCS = $(shell find src -name '*_tests.cpp' -path 'src/*/*')
TEST_OBJS = $(TEST_SRCS:src/%.cpp=bin/%_.o)
TEST_DEPS = $(TEST_SRCS:src/%.cpp=bin/%_.d)

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = googletest/googletest

# All Google Test headers and source files.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

TEST_FILTER = '*'

# Default target. Build your 'mytop' program, using the real /proc filesystem.
$(NAME): PROC_ROOT = /proc
$(NAME): $(MAIN_OBJS) $(OTHER_OBJS) $(INFO_IMPL_OBJS)
	$(CXX) $(CPP_FLAGS) $^ -o $(NAME) -l ncurses

# Build and run the unit tests.
test: bin/all_tests | proc
	./bin/all_tests --gtest_filter=$(TEST_FILTER)

# Build and run the program. Only useful on Linux machines
run: $(NAME)
	./$(NAME)

# Remove all generated files.
clean:
	rm -rf $(NAME)* bin/

# Ensure the bin/ directories are created.
$(ALL_SRCS): | bin

# Mirror the directory structure of src/ under bin/
bin:
	mkdir -p $(shell find src -type d | sed "s/src/bin/")

# Ask the user to initialize the googletest submodule if needed
$(GTEST_DIR):
	# You need to initialize the googletest Git submodule first:
	#
	#   git submodule update --init
	#
	# IMPORTANT:
	#   If you use HTTPS with Git, update the URL in the .gitmodules file
	#   in this directory before running this command.
	#
	exit 1

$(GTEST_HEADERS): | $(GTEST_DIR)

# Build objects using the actual PROC_ROOT.
bin/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $< -c -o $@

# Build objects using the fake PROC_ROOT and test flags.
bin/%_.o: src/%.cpp $(GTEST_HEADERS)
	$(CXX) $(TEST_CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Build gtest_main.a.
bin/gtest-all.o: $(GTEST_SRCS_) | bin
	$(CXX) $(TEST_CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

bin/gtest_main.o: $(GTEST_SRCS_) | bin
	$(CXX) $(TEST_CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

bin/gtest_main.a: bin/gtest-all.o bin/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Build the unit tests.
bin/all_tests: $(INFO_TEST_OBJS) $(OTHER_OBJS) $(TEST_OBJS) bin/gtest_main.a
	$(CXX) $(TEST_CPPFLAGS) $(CXXFLAGS) -pthread $^ -o $@

# Set up a fake /proc filesystem for testing.
proc:
	mkdir -p $(PROC_BASE)
	tar -xjf proc.tar.bz2 -C $(PROC_BASE)
	rm -f proc && ln -s $(addprefix $(PROC_BASE),/proc) proc

# Auto dependency management.
-include $(MAIN_DEPS)
-include $(INFO_IMPL_DEPS)
-include $(INFO_TEST_DEPS)
-include $(OTHER_DEPS)
-include $(TEST_DEPS)
