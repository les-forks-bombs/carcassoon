NAME := carcassonne
PROG = $(BINS_DIR)/$(NAME)

LFLAGS += `pkg-config sdl3 --libs`
CFLAGS += `pkg-config sdl3 --cflags`
LLIBS  += carcassonne

include $(BUILD_DIR)/binary.mk
