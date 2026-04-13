NAME := carcassonne
PROG = $(BINS_DIR)/$(NAME)

LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --cflags)
LLIBS  += carcassonne

include $(BUILD_DIR)/binary.mk
