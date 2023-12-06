# compiler and options
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -O3

# file directories
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build

# get source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)

# get object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRC_FILES))

# define DTYPE and ZEROIZE value
DTYPE := 64 	# default DTYPE is 32-bit
ZEROIZE := 0 	# default ZEROIZE is 0

# add DTYPE and ZEROIZE into CFLAGS
CFLAGS += -DDTYPE=$(DTYPE)
CFLAGS += -DZEROIZE=$(ZEROIZE)



# build target
all: test

# test 
test: $(BUILD_DIR)/test_executable

# build test_executable
$(BUILD_DIR)/test_executable: $(OBJ_FILES) $(TEST_OBJ_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# create test directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# clean
clean:
	rm -rf $(BUILD_DIR)
	@find test -name "*.txt" -type f -delete

.PHONY: all test clean
