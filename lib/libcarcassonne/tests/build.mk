DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/tests

LIBCARCASSONNE_TEST_SRCS := $(wildcard $(DIR)/*.c)
LIBCARCASSONNE_TEST_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(LIBCARCASSONNE_TEST_SRCS))

CLEAN += $(LIBCARCASSONNE_TEST_OBJS) $(LIBCARCASSONNE_TEST_OBJS:.o=.d)
-include $(LIBCARCASSONNE_TEST_OBJS:.o=.d)

$(OUT)/bin/libcarcassonne_test$(EXT): $(LIBCARCASSONNE_TEST_OBJS) \
    $(OUT)/libcarcassonne.a \
    $(OUT)/libutils.a
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -lcmocka
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(UTIL_DIR)/copy_dlls.sh $@; \
	            ;; \
	esac
	$(info $(TAB)LD $@)

CLEAN += $(OUT)/bin/libcarcassonne_test$(EXT)
TESTS += $(OUT)/bin/libcarcassonne_test$(EXT)

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))