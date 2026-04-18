NAME := cli
PROG = $(BINS_DIR)/$(NAME)

LLIBS += carcassonne

include $(BUILD_DIR)/binary.mk
