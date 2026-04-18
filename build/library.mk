LIB  := $(LIBS_DIR)/$(NAME).a
OUT_OBJ_DIR := $(OBJ_DIR)/$(shell realpath -s --relative-to="$(MAKE_DIR)" "$(shell pwd)")
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, $(OUT_OBJ_DIR)/%.o, $(SRCS))

$(LIB): $(OBJS)
	@$(AR) cr $@ $^
	@echo "    AR    $(notdir $@)"

$(OBJS): $(OUT_OBJ_DIR)/%.o: %.c
	@mkdir -p $(OUT_OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "    CC    $<"

-include $(OBJS:.o=.d)

build:: $(LIB)
test:: $(LIB)

.PHONY: clean
clean::
	@$(RM) -f $(LIB) $(OBJS)
	@echo "    RM    $(notdir $(OBJS))"
	@echo "    RM    $(notdir $(LIB))"
	@$(MAKE) -C tests -f build.mk clean
