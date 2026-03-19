SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

$(LIB): $(OBJS)
	@$(AR) cr $@ $^
	@echo "    AR    $(notdir $@)"

$(OBJS): $(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $^ -o $@
	@echo "    CC    $^"

build: $(LIB)
	@$(MAKE) -C tests -f build.mk build

.PHONY: clean
clean:
	@$(RM) -f $(LIB) $(OBJS)
	@echo "    CL    $(notdir $(OBJS))"
	@echo "    CL    $(notdir $(PROG))"
	@$(MAKE) -C tests -f build.mk clean
