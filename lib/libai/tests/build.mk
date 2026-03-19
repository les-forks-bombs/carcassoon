NAME := ai
PROG = $(BINS_DIR)/tests/$(NAME)_test

LIBS = -L$(LIBS_DIR) -lcmocka -lai

include $(BUILD_DIR)/binary.mk
