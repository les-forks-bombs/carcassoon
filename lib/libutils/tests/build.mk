DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/tests

LIBUTILS_TEST_SRCS := $(wildcard $(DIR)/*.c)
LIBUTILS_TEST_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(LIBUTILS_TEST_SRCS))

CLEAN += $(LIBUTILS_TEST_OBJS) $(LIBUTILS_TEST_OBJS:.o=.d)
-include $(LIBUTILS_TEST_OBJS:.o=.d)

$(OUT)/bin/libutils_test$(EXT): $(LIBUTILS_TEST_OBJS) \
    $(OUT)/libutils.a 
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -lcmocka
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(UTIL_DIR)/copy_dlls.sh $@; \
	            ;; \
	esac
	$(info $(TAB)LD $@)

CLEAN += $(OUT)/bin/libutils_test$(EXT)
TESTS += $(OUT)/bin/libutils_test$(EXT)

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))