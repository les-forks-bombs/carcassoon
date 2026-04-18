NAME := sdl
PROG = $(BINS_DIR)/$(NAME)

LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --cflags)
LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-image --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-image --cflags)
LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-ttf --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-ttf --cflags)

LLIBS += carcassonne ai

include $(BUILD_DIR)/binary.mk

build:: ./assets
	@cp -r ./assets $(BINS_DIR)/
