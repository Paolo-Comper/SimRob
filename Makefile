# Makefile per compilare progetto CMake con Ninja

BUILD_DIR := build
GENERATOR := Ninja

.PHONY: all run clean debug release

SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))src

all:
	@cmake -S . -B $(BUILD_DIR) -G $(GENERATOR)
	@cmake --build $(BUILD_DIR)

run: all
	@echo "+--------------------------------------+"
	@echo "|        Running the program...        |"
	@echo "+--------------------------------------+"
	@./$(BUILD_DIR)/SimRob

debug:
	@cmake -S . -B $(BUILD_DIR) -G $(GENERATOR) -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD_DIR)

release:
	@cmake -S . -B $(BUILD_DIR) -G $(GENERATOR) -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(BUILD_DIR)/.[!.]* $(BUILD_DIR)/..?*
	@clear

# Regola per formattare il codice C++ con clang-format
format:
	@find $(SRC_DIR) -name "*.cpp" -exec clang-format -i {} \;
	@echo "Formattazione completata!"