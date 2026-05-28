DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/libsdlrender

LIBSDLRENDER_SRCS := $(wildcard $(DIR)/*.c)
LIBSDLRENDER_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(LIBSDLRENDER_SRCS))

CLEAN += $(LIBSDLRENDER_OBJS) $(LIBSDLRENDER_OBJS:.o=.d)
-include $(LIBSDLRENDER_OBJS:.o=.d)

$(OUT)/libsdlrender.a: $(LIBSDLRENDER_OBJS)
	@mkdir -p $(dir $@)
	@ar rcs $@ $^
	$(info $(TAB)AR $@)

CLEAN += $(OUT)/libsdlrender.a

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))