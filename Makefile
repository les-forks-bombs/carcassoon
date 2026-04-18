CC 		:= clang
TARGET 	:= $(shell $(CC) -dumpmachine)
PROFILE := debug

# Directory configuration
MAKE_DIR := $(CURDIR)
OUT_DIR  := $(MAKE_DIR)/out/$(PROFILE)/$(TARGET)
LIBS_DIR := $(OUT_DIR)/lib
BINS_DIR := $(OUT_DIR)/bin
OBJ_DIR  := $(OUT_DIR)/obj
INCL_DIR := $(MAKE_DIR)/lib
BUILD_DIR:= $(MAKE_DIR)/build

CFLAGS += --target=$(TARGET)
LFLAGS += --target=$(TARGET)

CFLAGS += -I$(INCL_DIR) # In order to include files (#include header files)
CFLAGS += -std=c99 -Wall -Wextra -Wpedantic -Wdocumentation  # General building flags
LFLAGS += -L$(LIBS_DIR) -lm

ifeq "$(PROFILE)" "debug"
	ifeq (,$(filter $(TARGET),x86_64-w64-mingw64 x86_64-w64-mingw32))
	    CFLAGS += -fsanitize=address
	    LFLAGS += -fsanitize=address
	endif

	CFLAGS += -O0 -g
endif

ifeq "$(PROFILE)" "release"
	CFLAGS += -O3
	LFLAGS += -s
endif

export CC MAKE_DIR OBJ_DIR LIBS_DIR BINS_DIR INCL_DIR BUILD_DIR CFLAGS LFLAGS TARGET


build test clean: out/$(PROFILE)/$(TARGET)
	@$(MAKE) -C lib -f build.mk $@

out/$(PROFILE)/$(TARGET):
	@mkdir -p out/$(PROFILE)/
	@cp -r $(BUILD_DIR)/out $@

cli sdl: build
	$(BINS_DIR)/carcassonne -m $@

req:
	@echo "/!\ Attention !"
	@echo ""
	@echo "Ce projet utilise les libraries système pour compiler avec sdl et cmocka"
	@echo "Merci de l'installer pour le target $(TARGET) afin que pkg-config"
	@echo "Puisse trouver la librarie !"
	@echo
	@echo "RHEL/Fedora: dnf install libSDL3-devel libSDL3_ttf-devel libSDL3_image-devel libcmocka-devel"
	@echo "Debian: apt-get install libsdl3-dev libsdl3-ttf-dev libsdl3-image-dev libcmocka-dev"

docs:
	doxygen

lint:
	@find . -iname "*.h" -o -iname "*.c" | xargs clang-format -i

check:
	@find . -iname "*.h" -o -iname "*.c" | xargs clang-format --dry-run --Werror

.PHONY: clean build test docs check lint req cli sdl

