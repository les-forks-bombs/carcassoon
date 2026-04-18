OBJ_DIR := $(OBJ_DIR)$(subst $(MAKE_DIR),,$(CURDIR))

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

# Build .o files from .c files
$(OBJS): $(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "    CC    $<"
-include $(OBJS:.o=.d)

build:: $(OBJS)
test:: $(OBJS)
