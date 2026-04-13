NAME := utils
PROG = $(BINS_DIR)/tests/$(NAME)_test

LIBS = cmocka utils

include $(BUILD_DIR)/binary.mk
