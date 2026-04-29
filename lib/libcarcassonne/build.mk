DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/libcarcassonne

LIBCARCASSONNE_SRCS := $(wildcard $(DIR)/*.c)
LIBCARCASSONNE_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(LIBCARCASSONNE_SRCS))

CLEAN += $(LIBCARCASSONNE_OBJS) $(LIBCARCASSONNE_OBJS:.o=.d)
-include $(LIBCARCASSONNE_OBJS:.o=.d)

$(OUT)/libcarcassonne.a: $(LIBCARCASSONNE_OBJS)
	@mkdir -p $(dir $@)
	@ar rcs $@ $^
	$(info $(TAB)AR $@)

CLEAN += $(OUT)/libcarcassonne.a

include $(DIR)/tests/build.mk

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))