CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -std=c99 
OBJS := bigint.o rng.o arith.o array.o
SRCS := arith.c array.c bigint.c rng.c

OS := $(shell uname -s)

DTYPE := 32 # default DTYPE is 32-bit
ZEROIZE := 0 

CFLAGS += -DDTYPE=$(DTYPE)
CFLAGS += -DZEROIZE=$(ZEROIZE)


# detecting OS
ifeq ($(OS), Darwin) # macOS
TARGET := main.out
else ifeq ($(OS), Windows_NT)
TARGET := main.exe
else ifeq ($(OS), Linux)
TARGET := main
else
$(info Current OS is $(OS))
$(error $(OS) is not supported in this library)
endif

.PHONY: all 
all: $(TARGET)
OBJS += main.o
SRCS += main.c
$(TARGET) : $(OBJS)
	$(CC) -o $@ $^


# .PHONY: test
# test:
# $(info build for testing...)
# OBJS += test.o
# SRCS += test.c
# TARGET := test
# $(TARGET) : $(OBJS)
# 	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)