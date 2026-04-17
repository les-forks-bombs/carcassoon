NAME := lct
PROG = $(BINS_DIR)/tests/$(NAME)_test

LLIBS = lct
LFLAGS += -lcmocka

include $(BUILD_DIR)/binary.mk
