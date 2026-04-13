NAME := sdl
PROG = $(BINS_DIR)/$(NAME)

LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --cflags)

include $(BUILD_DIR)/binary.mk
