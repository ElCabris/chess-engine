TARGET = chess 

SRC_DIR = src
INC_DIR = include
OBJ_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -Wpedantic -Wstrict-aliasing -Wsequence-point -Wconversion -Wsign-conversion -Wfloat-conversion -Wuseless-cast -Wuninitialized -Wnull-dereference -Wformat=2 -Wformat-security -Wformat-truncation -Wformat-overflow -Wmissing-format-attribute -Waddress -Wcast-align -Wcast-qual -Wshadow -Wswitch-enum -Wswitch-default -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wnested-externs -Wdeclaration-after-statement -Wold-style-definition -Wunused-result -Warray-bounds -Wrestrict -Wwrite-strings -Wlogical-op -Wmissing-include-dirs -Wunused-macros -Winit-self -Wundef -I$(INC_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
