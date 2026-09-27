PROJECT       := equip_ops
CXX           ?= g++
CPPFLAGS      := -Isrc
WARNINGS      := -Wall -Wextra -Wpedantic
STD           := -std=c++23
DEBUG_FLAGS   := -O0 -g3
RELEASE_FLAGS := -O3 -DNDEBUG

BUILD_DIR     := build/debug
RELEASE_DIR   := build/release
APP           := $(BUILD_DIR)/$(PROJECT)
RELEASE_APP   := $(RELEASE_DIR)/$(PROJECT)
SOURCES       := $(wildcard src/*.cpp)
TEST_SOURCES  := $(wildcard tests/unit/*Test.cpp)
TESTS         := $(patsubst tests/unit/%.cpp,$(BUILD_DIR)/tests/%,$(TEST_SOURCES))

.DEFAULT_GOAL := build

.PHONY: all build run test release sanitize format clean help

all: build

build: $(APP)

$(APP): app/main.cpp $(SOURCES)
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARNINGS) $(DEBUG_FLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

run: $(APP)
	./$(APP)

$(BUILD_DIR)/tests/%Test: tests/unit/%Test.cpp $(SOURCES)
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARNINGS) $(DEBUG_FLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

test: $(TESTS)
	@set -e; for test in $(TESTS); do $$test; done

release: $(RELEASE_APP)

$(RELEASE_APP): app/main.cpp $(SOURCES)
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARNINGS) $(RELEASE_FLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

sanitize:
	$(MAKE) clean
	$(MAKE) test DEBUG_FLAGS='-O1 -g3 -fsanitize=address,undefined -fno-omit-frame-pointer' \
		LDFLAGS='-fsanitize=address,undefined'

format:
	clang-format -i app/*.cpp src/*.cpp src/*.cppm tests/unit/*.cpp

clean:
	rm -rf build bin

help:
	@printf '%s\n' 'Targets: build, run, test, release, sanitize, format, clean'
