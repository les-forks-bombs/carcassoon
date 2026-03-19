NAME := carcassonne
PROG = $(BINS_DIR)/tests/$(NAME)_test

LIBS = -L$(LIBS_DIR) -lcmocka -lcarcassonne

include $(BUILD_DIR)/binary.mk
