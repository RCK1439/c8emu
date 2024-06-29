CC = gcc

CFLAGS = -Wall -Werror -Wpedantic -std=c99
debug: LDFLAGS = -O0 -ggdb
release: LDFLAGS = -O3 -DNDEBUG

BIN_DIR = bin
OBJ_DIR = obj
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE = $(BIN_DIR)/c8emu.exe

INCL = -Ivendor/raylib/include
LINK = -Lvendor/raylib/lib
LIBS = -lraylib -lwinmm -lgdi32 -luser32 -lshell32

.PHONY: all debug release

all: debug

debug: $(SOURCES) $(EXECUTABLE)

release: $(SOURCES) $(EXECUTABLE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXECUTABLE): $(BIN_DIR) $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LINK) $(LIBS)

$(OBJ_DIR)/%.o: src/%.c $(OBJ_DIR)
	$(CC) $(LDFLAGS) $(CFLAGS) $< -c -o $@ $(INCL)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(wildcard *.dis)
