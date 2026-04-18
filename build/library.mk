include $(BUILD_DIR)/compile.mk

ARCHIVE  := $(LIBS_DIR)/$(NAME).a

$(ARCHIVE): $(OBJS)
	@$(AR) cr $@ $^
	@echo "    AR    $(notdir $@)"

build:: $(ARCHIVE)
test:: $(ARCHIVE)

.PHONY: clean
clean::
	@$(RM) -f $(ARCHIVE) $(OBJS)
	@echo "    RM    $(notdir $(ARCHIVE))"
	@echo "    RM    $(notdir $(OBJS))"
	@$(MAKE) -C tests -f build.mk clean
