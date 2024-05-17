CC = gcc

CFLAGS = -Wall -Werror -Wpedantic -std=c99
LDFLAGS = # TODO: Make release and debug specific flags

BIN_DIR = bin
OBJ_DIR = obj
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE = bin/c8emu.exe

INCL = -Ivendor/raylib/include
LINK = -Lvendor/raylib/lib
LIBS = -lraylib -lwinmm -lgdi32 -luser32 -lshell32

all: $(SOURCES) $(EXECUTABLE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXECUTABLE): $(BIN_DIR) $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LINK) $(LIBS)

$(OBJ_DIR)/%.o: src/%.c $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -c -o $@ $(INCL)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(wildcard *.dis)
