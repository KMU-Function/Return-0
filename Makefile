CC = gcc
<<<<<<< HEAD
CFLAGS = -g -Wall -Wextra -Wpedantic -std=c99 
=======
CFLAGS = -g -Wall -Wextra -Wpedantic -std=c99
>>>>>>> 5681f8b32aade6f77052bd23b958c750b0823056
OBJS := bigint.o rng.o arith.o array.o
SRCS := arith.c array.c bigint.c rng.c

OS := $(shell uname -s)

<<<<<<< HEAD
DTYPE := 32 # default DTYPE is 32-bit
ZEROIZE := 0 

CFLAGS += -DDTYPE=$(DTYPE)
CFLAGS += -DZEROIZE=$(ZEROIZE)


=======
>>>>>>> 5681f8b32aade6f77052bd23b958c750b0823056
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

<<<<<<< HEAD

=======
>>>>>>> 5681f8b32aade6f77052bd23b958c750b0823056
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