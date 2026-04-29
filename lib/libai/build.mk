DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/libai

LIBAI_SRCS := $(wildcard $(DIR)/*.c)
LIBAI_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(LIBAI_SRCS))

CLEAN += $(LIBAI_OBJS) $(LIBAI_OBJS:.o=.d)
-include $(LIBAI_OBJS:.o=.d)

$(OUT)/libai.a: $(LIBAI_OBJS)
	@mkdir -p $(dir $@)
	@ar rcs $@ $^
	$(info $(TAB)AR $@)
CLEAN += $(OUT)/libai.a

include $(DIR)/tests/build.mk

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))