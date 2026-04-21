include $(BUILD_DIR)/compile.mk

ifneq (,$(filter $(TARGET),x86_64-w64-mingw64 x86_64-w64-mingw32))
    PROG := $(PROG).exe
endif

$(PROG): $(OBJS) $(LLIBS:%=$(LIBS_DIR)/lib%.a)
	@$(CC)  -o $@ $(OBJS) $(LFLAGS) $(LLIBS:%=-l%)
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(BUILD_DIR)/copy_dlls.sh $(PROG); \
	            ;; \
	esac
	@echo "    LD    $(notdir $@)"

build:: $(PROG)
test:: $(PROG)

.PHONY: clean
clean::
	@$(RM) -f $(PROG){,.exe}
	@echo "    RM    $(notdir $(PROG))"
