include $(BUILD_DIR)/compile.mk

ARCHIVE  := $(LIBS_DIR)/$(NAME).a

$(ARCHIVE): $(OBJS)
	@$(AR) cr $@ $^
	@echo "    AR    $(notdir $@)"

build:: $(ARCHIVE)
test:: $(ARCHIVE)

.PHONY: clean
clean::
	@$(RM) -f $(ARCHIVE)
	@echo "    RM    $(notdir $(ARCHIVE))"
	@$(MAKE) -C tests -f build.mk clean
