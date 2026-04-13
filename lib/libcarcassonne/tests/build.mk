NAME := carcassonne
PROG = $(BINS_DIR)/tests/$(NAME)_test

LLIBS = cmocka carcassonne

include $(BUILD_DIR)/binary.mk
