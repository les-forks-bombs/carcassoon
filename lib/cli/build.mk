NAME := cli
PROG = $(BINS_DIR)/$(NAME)

LLIBS += ai carcassonne utils

include $(BUILD_DIR)/binary.mk
