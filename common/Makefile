NAME := libcommon.a
SOURCES := list.c string_view.c

CC := gcc
AR := ar
CFLAGS := -Wall -Wextra -Werror -std=c2x -ggdb
CPPFLAGS := -Iinclude/

OBJECTS_DIR := obj
BINARY_DIR := bin

OBJECTS := $(SOURCES:%.c=$(OBJECTS_DIR)/%.o)
HEADERS := $(OBJECTS:%.o=%.d)

all: $(BINARY_DIR)/$(NAME)

$(BINARY_DIR)/$(NAME): $(OBJECTS)
	@mkdir -p $(BINARY_DIR)
	@$(AR) rcs $@ $^
	$(info AR $@)

-include $(HEADERS)

$(OBJECTS_DIR)/%.o: %.c
	@mkdir -p $(OBJECTS_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -c -o $@ $<
	$(info CC $@)

clean:
	rm -f $(OBJECTS)
	rm -f $(HEADERS)
	rm -rf $(OBJECTS_DIR)
	rm -rf $(BINARY_DIR)
