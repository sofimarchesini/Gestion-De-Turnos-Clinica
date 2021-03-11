SOURCE_DIR := .
BUILD_DIR := build
TARGET := main

# TDA := none

SOURCES := $(shell find $(SOURCE_DIR)/ -type f -name "*.c")
OBJS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

LDLIBS := -lm
LDFLAGS := -g
CFLAGS := -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror

all: $(BUILD_DIR)/$(TARGET)

test:
	valgrind --leak-check=full --show-leak-kinds=all -s $(BUILD_DIR)/main

# coverage: 
# 	./cover.sh $(TDA).c *.c

clean:
	rm -rf -- build

rebuild: clean all

$(BUILD_DIR)/%.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

-include $(OBJECTS:%.o=%.d)

MKDIR_P ?= mkdir -p

.PHONY: all clean rebuild