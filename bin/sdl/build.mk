NAME := sdl
PROG = $(BINS_DIR)/$(NAME)

LFLAGS += `pkg-config sdl3 --libs`
CFLAGS += `pkg-config sdl3 --cflags`

include $(BUILD_DIR)/binary.mk
