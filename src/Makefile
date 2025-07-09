# Build mode
ifeq ($(MAKECMDGOALS), release)
	BUILD_MODE = BUILD_RELEASE
else
	BUILD_MODE = BUILD_DEBUG
endif

# Platform detection
ifeq ($(OS), Windows_NT)
	PLATFORM = WINDOWS
else
	UNAMEOS = $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		PLATFORM = LINUX
	else ifeq ($(UNAMEOS), Darwin)
		PLATFORM = MACOS
	else
		$(error Unsupported platform)
	endif
endif

CC = cc

CFLAGS = -Wall -Werror -Wpedantic -Wextra -std=c99
ifeq ($(BUILD_MODE), BUILD_RELEASE)
	CFLAGS += -O3 -DNDEBUG -march=native
else
	CFLAGS += -O0 -ggdb
endif

SRC_DIR = .
BIN_DIR = ../bin
VENDOR_DIR = ../vendor

CLIENT_DIR = $(SRC_DIR)/client
CORE_DIR = $(SRC_DIR)/core
EMULATOR_DIR = $(SRC_DIR)/emulator
RENDERER_DIR = $(SRC_DIR)/renderer

SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(CLIENT_DIR)/*.c $(CORE_DIR)/*.c $(EMULATOR_DIR)/*.c $(RENDERER_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, $(BIN_DIR)/%.o, $(SRC_FILES))

ifeq ($(PLATFORM), WINDOWS)
	TARGET = $(BIN_DIR)/c8emu.exe
else
	TARGET = $(BIN_DIR)/c8emu
endif

# Third-party includes
RAYLIB_INCL = -I$(VENDOR_DIR)/raylib/include
RKLOG_INCL = -I$(VENDOR_DIR)/rklog

# Third-party linking
ifeq ($(PLATFORM), WINDOWS)
	RAYLIB_LINK = -L$(VENDOR_DIR)/raylib/lib/windows
else ifeq ($(PLATFORM), LINUX)
	RAYLIB_LINK = -L$(VENDOR_DIR)/raylib/lib/linux
else ifeq ($(PLATFORM), MACOS)
	$(error MacOS not yet supported)
endif

INCL = -I$(SRC_DIR) $(RAYLIB_INCL) $(RKLOG_INCL)
LINK = $(RAYLIB_LINK)
ifeq ($(PLATFORM), WINDOWS)
	LIBS = -lraylib -lwinmm -luser32 -lshell32 -lgdi32
else ifeq ($(PLATFORM), LINUX)
	LIBS = -lraylib -lm
else ifeq ($(PLATFORM), MACOS)
	$(error MacOS not yet supported)
endif

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
