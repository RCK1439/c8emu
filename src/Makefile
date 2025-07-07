CC = cc

CFLAGS = -Wall -Werror -Wpedantic -Wextra -std=c99
ifeq ($(MAKECMDGOALS), release)
	CFLAGS += -O3 -DNDEBUG -march=native
else
	CFLAGS += -O0 -ggdb
endif

SRC_DIR = .
BIN_DIR = ../bin

CLIENT_DIR = $(SRC_DIR)/client
CORE_DIR = $(SRC_DIR)/core
EMULATOR_DIR = $(SRC_DIR)/emulator

SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(CLIENT_DIR)/*.c $(CORE_DIR)/*.c $(EMULATOR_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, $(BIN_DIR)/%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)/c8emu

RAYLIB_INCL = -I../vendor/raylib/include
RAYLIB_LINK = -L../vendor/raylib/lib

RKLOG_INCL = -I../vendor/rklog

INCL = -I$(SRC_DIR) $(RAYLIB_INCL) $(RKLOG_INCL)
LINK = $(RAYLIB_LINK)
LIBS = -lraylib -lm

.PHONY: all release clean

all: $(TARGET)

release: all

clean:
	rm -f $(TARGET) $(OBJ_FILES)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LINK) $(LIBS)

$(BIN_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCL)
