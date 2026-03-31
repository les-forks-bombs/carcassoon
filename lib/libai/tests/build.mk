NAME := ai
PROG = $(BINS_DIR)/tests/$(NAME)_test

LDLIBS = -lcmocka -lai

include $(BUILD_DIR)/binary.mk
