CC = gcc

CFLAGS = -Wall -Werror -Wpedantic -std=c99
ifeq ($(MAKECMDGOALS), release)
	CLFAGS += -O3 -DNDEBUG
else
	CFLAGS += -O0 -ggdb
endif

SRC_DIR = .
BIN_DIR = ../bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, $(BIN_DIR)/%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)/c8emu

RAYLIB_INCL = -I../vendor/raylib/include
RAYLIB_LINK = -L../vendor/raylib/lib

LIBS = -lraylib -lm

.PHONY: all release clean

all: $(TARGET)

release: all

clean:
	rm -f $(TARGET) $(OBJ_FILES)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(RAYLIB_LINK) $(LIBS)

$(BIN_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(RAYLIB_INCL)
