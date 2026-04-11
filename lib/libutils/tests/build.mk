NAME := utils
PROG = $(BINS_DIR)/tests/$(NAME)_test

LDLIBS = -lcmocka -lutils

include $(BUILD_DIR)/binary.mk
