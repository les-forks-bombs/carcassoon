DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/sdl

SDL_SRCS := $(wildcard $(DIR)/*.c)
SDL_OBJS := $(patsubst $(PWD)/lib/%.c, $(OUT)/objs/%.o, $(SDL_SRCS))

CLEAN += $(SDL_OBJS) $(SDL_OBJS:.o=.d)
-include $(SDL_OBJS:.o=.d)

$(OUT)/bin/assets: $(DIR)/assets
	@mkdir -p $(dir $@)
	@cp -r $< $@
	$(info $(TAG)CP $<)

$(OUT)/bin/sdl$(EXT): $(SDL_OBJS) $(OUT)/bin/assets \
    $(OUT)/libcarcassonne.a \
    $(OUT)/libutils.a \
    $(OUT)/libai.a
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $(SDL_OBJS) -lai -lutils -lcarcassonne $(LFLAGS)
	@case "$(TARGET)" in \
	        (x86_64-w64-mingw64|x86_64-w64-mingw32) \
	            $(UTIL_DIR)/copy_dlls.sh $@; \
	            ;; \
	esac
	$(info $(TAB)LD $@)

CLEAN += $(OUT)/bin/sdl$(EXT) $(OUT)/bin/assets

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))