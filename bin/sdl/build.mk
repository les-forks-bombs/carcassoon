NAME := sdl
PROG = $(BINS_DIR)/$(NAME)

LDLIBS += `pkg-config sdl3 --libs`

include $(BUILD_DIR)/binary.mk
