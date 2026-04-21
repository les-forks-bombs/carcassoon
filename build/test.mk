PROG = $(BINS_DIR)/tests/$(NAME)
LFLAGS += -lcmocka

include $(BUILD_DIR)/binary.mk

RUNNER := 
ifneq (,$(filter $(TARGET),x86_64-w64-mingw64 x86_64-w64-mingw32))
    RUNNER := wine
endif

$(PROG).xml $(PROG).profraw: $(PROG)
	@CMOCKA_XML_FILE='$@' LLVM_PROFILE_FILE="$(PROG).profraw" CMOCKA_MESSAGE_OUTPUT=xml \
		$(RUNNER) $<
	@echo "    TEST  $(notdir $<)"

test:: $(PROG).xml

clean::
	@$(RM) -f $(PROG).{xml,profraw}
	@echo "    RM    $(notdir $(PROG).xml)"

.PHONY: test clean
