NAME := cli
PROG = $(BINS_DIR)/$(NAME)

LLIBS += ai carcassonne

include $(BUILD_DIR)/binary.mk
