DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/cli

CLI_SRCS := $(wildcard $(DIR)/*.c)
CLI_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(CLI_SRCS))

CLEAN += $(CLI_OBJS) $(CLI_OBJS:.o=.d)
-include $(CLI_OBJS:.o=.d)

$(OUT)/bin/cli$(EXT): $(CLI_OBJS) \
    $(OUT)/libcarcassonne.a \
    $(OUT)/libutils.a \
    $(OUT)/libai.a
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -lcarcassonne -lutils -lai
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(UTIL_DIR)/copy_dlls.sh $@; \
	            ;; \
	esac
	$(info $(TAB)LD $@)

CLEAN += $(OUT)/bin/cli$(EXT)

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))