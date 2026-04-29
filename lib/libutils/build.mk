DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/libutils

LIBUTILS_SRCS := $(wildcard $(DIR)/*.c)
LIBUTILS_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(LIBUTILS_SRCS))

CLEAN += $(LIBUTILS_OBJS) $(LIBUTILS_OBJS:.o=.d)
-include $(LIBUTILS_OBJS:.o=.d)

$(OUT)/libutils.a: $(LIBUTILS_OBJS)
	@mkdir -p $(dir $@)
	@ar rcs $@ $^
	$(info $(TAB)AR $@)
CLEAN += $(OUT)/libutils.a

include $(DIR)/tests/build.mk

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))