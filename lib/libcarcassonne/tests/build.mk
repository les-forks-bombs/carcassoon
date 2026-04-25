NAME := carcassonne_test
LLIBS = carcassonne utils

include $(BUILD_DIR)/test.mk

build::
	@cp -r ./fixtures $(BINS_DIR)/tests

clean::
	rm -r $(BINS_DIR)/tests/fixtures || true
