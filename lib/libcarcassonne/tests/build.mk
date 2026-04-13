NAME := carcassonne
PROG = $(BINS_DIR)/tests/$(NAME)_test

LLIBS = carcassonne
LFLAGS += -lcmocka

include $(BUILD_DIR)/binary.mk
