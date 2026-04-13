OBJ_DIR := $(OBJ_DIR)/$(shell realpath -s --relative-to="$(MAKE_DIR)" "$(shell pwd)")
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

ifneq (,$(filter $(TARGET),x86_64-w64-mingw64 x86_64-w64-mingw32))
    PROG := $(PROG).exe
endif

$(PROG): $(OBJS) $(LLIBS:%=$(LIBS_DIR)/lib%.a)
	@$(CC)  -o $@  $^ $(LFLAGS) $(LLIBS:%=-l%)
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(BUILD_DIR)/copy_dlls.sh $(PROG); \
	            ;; \
	esac
	@echo "    LD    $(notdir $@)"



$(OBJS): $(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "    CC    $<"

-include $(OBJS:.o=.d)

build: $(PROG)

.PHONY: clean
clean:
	@$(RM) -f $(OBJS) $(PROG)
	@$(RM) -rf $(PROG){,.exe}
	@echo "    RM    $(notdir $(OBJS))"
	@echo "    RM    $(notdir $(PROG))"
