DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/tests

LIBAI_TEST_SRCS := $(wildcard $(DIR)/*.c)
LIBAI_TEST_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(LIBAI_TEST_SRCS))

CLEAN += $(LIBAI_TEST_OBJS) $(LIBAI_TEST_OBJS:.o=.d)
-include $(LIBAI_TEST_OBJS:.o=.d)

$(OUT)/bin/libai_test$(EXT): $(LIBAI_TEST_OBJS) \
    $(OUT)/libcarcassonne.a \
    $(OUT)/libutils.a \
    $(OUT)/libai.a
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(TESTS_LFLAGS) $(LFLAGS) -o $@ $^ -lcmocka
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(UTIL_DIR)/copy_dlls.sh $@; \
	            ;; \
	esac
	$(info $(TAB)LD $@)

CLEAN += $(OUT)/bin/libai_test$(EXT)
TESTS += $(OUT)/bin/libai_test$(EXT)

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))