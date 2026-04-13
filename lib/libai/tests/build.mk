NAME := ai
PROG = $(BINS_DIR)/tests/$(NAME)_test

LLIBS = cmocka ai

include $(BUILD_DIR)/binary.mk
