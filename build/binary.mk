OUT_OBJ_DIR := $(OBJ_DIR)/$(shell realpath -s --relative-to="$(MAKE_DIR)" "$(shell pwd)")
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, $(OUT_OBJ_DIR)/%.o, $(SRCS))

ifeq ($(TARGET),x86_64-w64-windows-gnu)
    PROG := $(PROG).exe
endif

$(PROG): $(OBJS)
	@$(CC) $(LFLAGS) -o $@ $^ $(LDLIBS)
	@[ $(TARGET) = x86_64-w64-windows-gnu ]; \
		$(BUILD_DIR)/copy_dlls.sh $(PROG)
	@echo "    LD    $(notdir $@)"

$(OBJS): $(OUT_OBJ_DIR)/%.o: %.c
	@mkdir -p $(OUT_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $^ -o $@
	@echo "    CC    $^"

build: $(PROG)

.PHONY: clean
clean:
	@$(RM) -f $(OBJS) $(PROG)
	@$(RM) -rf $(PROG){,.exe}
	@echo "    RM    $(notdir $(OBJS))"
	@echo "    RM    $(notdir $(PROG))"
