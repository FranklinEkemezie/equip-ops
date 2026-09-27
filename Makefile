CXX			:= g++
CXXFLAGS	:= -std=c++23 -Wall -Wextra -g -I src

BIN_DIR		:= bin
BUILD_DIR	:= build

PROJECT_NAME = equip_ops

SRC_DIR		= src
TEST_DIR	= tests
APP_DIR		= app


SOURCES		= $(wildcard src/*.cpp src/**/*.cpp)
OBJECTS		= $(patsubst %.cpp,$(BIN_DIR)/%.o, $(SOURCES))

TESTS_SRC	= $(wildcard tests/*Test.cpp tests/**/*Test.cpp)
TESTS		= $(patsubst %.cpp,$(BIN_DIR)/%, $(TESTS_SRC))

LIBRARY		= $(BUILD_DIR)/lib$(PROJECT_NAME).a
LIBRARY_SO	= $(patsubst %.a,%.so, $(LIBRARY))

APP_SRC		= $(wildcard app/*.cpp app/**/*.cpp)
APP_OBJECTS	= $(patsubst %.cpp,$(BIN_DIR)/%.o, $(APP_SRC))

APP			= $(BUILD_DIR)/$(PROJECT_NAME)

$(info LIBRARY=$(LIBRARY))
$(info SOURCES=$(SOURCES))
$(info OBJECTS=$(OBJECTS))
$(info LIBRARY_SO=$(LIBRARY_SO))
$(info TESTS=$(TESTS))

all: $(LIBRARY) $(LIBRARY_SO) tests app

app: $(APP_OBJECTS)
	@echo ""
	@echo "=============================================="
	@echo " Building application: $(PROJECT_NAME)"
	@echo "=============================================="
	$(CXX) $(CXXFLAGS) $(APP_OBJECTS) $(LIBRARY) -o $(APP)
	@echo ""
	@echo "=============================================="
	@echo " Running application..."
	@echo "=============================================="
	@echo ""
	@./$(APP)

bin/src/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/tests/%: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< $(LIBRARY) -o $@

bin/app/%.o: app/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIBRARY): CXXFLAGS += -fPIC
$(LIBRARY): build $(OBJECTS)
	@echo ""
	@echo "=== Building static library [$(LIBRARY)]: "
	@echo ""
	ar rcs $@ $(OBJECTS)
	ranlib $@
	@echo ""
	@echo "========== Static Library Compiled ==========="
	@echo ""

$(LIBRARY_SO): $(LIBRARY) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJECTS)

tests: $(TESTS)
	@echo ""
	@echo "=============================================="
	@echo " Running tests"
	@echo "=============================================="
	@echo ""
	@sh ./run_tests.sh
	@echo "=============== Tests Passed ================="
	@echo ""

build:
	@echo ""
	@echo "=============================================="
	@echo " Building project: $(PROJECT_NAME)"
	@echo "=============================================="
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@echo ""


.PHONY: clean

clean:
	@rm -rf $(BIN_DIR) $(BUILD_DIR)
	@echo "Cleaned build files."
