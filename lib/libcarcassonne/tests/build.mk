NAME := carcassonne
PROG = $(BINS_DIR)/tests/$(NAME)_test

LDLIBS = -lcmocka -lcarcassonne

include $(BUILD_DIR)/binary.mk
