DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/carcassonne

CARCASSONNE_SRCS := $(wildcard $(DIR)/*.c)
CARCASSONNE_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(CARCASSONNE_SRCS))

CLEAN += $(CARCASSONNE_OBJS) $(CARCASSONNE_OBJS:.o=.d)
-include $(CARCASSONNE_OBJS:.o=.d)

$(OUT)/bin/assets: $(DIR)/assets
	@mkdir -p $(dir $@)
	@cp -r $< $@
	$(info $(TAB)CP $<)

OLD_LFLAGS := $(LFLAGS)
ifeq "$(TARGET)" "wasm32-unknown-emscripten"
	CLEAN += $(OUT)/bin/carcassonne.{wasm,js,data}
	LFLAGS += --preload-file $(OUT)/bin/assets@/assets
endif

$(OUT)/bin/carcassonne$(EXT): $(CARCASSONNE_OBJS) $(OUT)/bin/assets $(OUT)/libcarcassonne.a $(OUT)/libutils.a $(OUT)/libai.a  $(OUT)/libsdlrender.a
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $< -lsdlrender  -lai -lcarcassonne -lutils $(LFLAGS)
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(UTIL_DIR)/copy_dlls.sh $@; \
	            ;; \
	esac
	$(info $(TAB)LD $@)

LFLAGS := $(OLD_LFLAGS)

CLEAN += $(OUT)/bin/carcassonne$(EXT) $(OUT)/bin/assets

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))