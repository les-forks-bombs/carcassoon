DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/carcassonne

CARCASSONNE_SRCS := $(wildcard $(DIR)/*.c)
CARCASSONNE_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(CARCASSONNE_SRCS))

CLEAN += $(CARCASSONNE_OBJS) $(CARCASSONNE_OBJS:.o=.d)
-include $(CARCASSONNE_OBJS:.o=.d)

$(OUT)/bin/carcassonne$(EXT): $(CARCASSONNE_OBJS) $(OUT)/libcarcassonne.a $(OUT)/libutils.a $(OUT)/libai.a
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -lai -lcarcassonne -lutils
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(UTIL_DIR)/copy_dlls.sh $@; \
	            ;; \
	esac
	$(info $(TAB)LD $@)

CLEAN += $(OUT)/bin/carcassonne$(EXT)

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))