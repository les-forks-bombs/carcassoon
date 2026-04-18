L_OBJ_DIR := $(OBJ_DIR)$(subst $(MAKE_DIR),,$(CURDIR))

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, $(L_OBJ_DIR)/%.o, $(SRCS))

# Build .o files from .c files
$(OBJS): $(L_OBJ_DIR)/%.o: %.c
	@mkdir -p $(L_OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "    CC    $<"
-include $(OBJS:.o=.d)

clean::
	@$(RM) -f $(OBJS)
	@$(RM) -f $(OBJS:.o=.d)
	@echo "    RM    $(notdir $(OBJS)) $(notdir $(OBJS:.o=.d))"

build:: $(OBJS)
test:: $(OBJS)
