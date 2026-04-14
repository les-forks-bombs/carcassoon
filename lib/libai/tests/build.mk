NAME := ai
PROG = $(BINS_DIR)/tests/$(NAME)_test

LLIBS = ai
LFLAGS += -lcmocka

include $(BUILD_DIR)/binary.mk
