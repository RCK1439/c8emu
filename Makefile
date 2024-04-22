CC = gcc

CFLAGS = -Wall -Werror -Wpedantic
DBG = -ggdb -O0
REL = -O3 -DNDEBUG

SRC = src/*.c
BIN = bin
EXE = $(BIN)/c8emu.exe

INCL = -Ivendor/raylib/include
LINK = -Lvendor/raylib/lib
LIBS = -lraylib -lwinmm -lgdi32 -lshell32 -luser32

$(BIN):
	mkdir $(BIN)

.PHONY: all debug release clean

all:
	debug

debug: $(BIN) |
	$(CC) $(DBG) $(CFLAGS) $(INCL) $(LINK) -o $(EXE) $(SRC) $(LIBS)

release: $(BIN) |
	$(CC) $(REL) $(CFLAGS) $(INCL) $(LINK) -o $(EXE) $(SRC) $(LIBS)

clean:
	rm -f $(EXE) &&\
	rm -f *.dis