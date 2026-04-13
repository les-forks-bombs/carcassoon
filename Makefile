CC 		:= clang
TARGET 	?= $(shell $(CC) -dumpmachine)
PROFILE := debug

MAKE_DIR := $(PWD)
OUT_DIR  := $(MAKE_DIR)/out/$(PROFILE)/$(TARGET)
# The directory where we're going to store our '.a' archive files
LIBS_DIR := $(OUT_DIR)/lib
BINS_DIR := $(OUT_DIR)/bin
OBJ_DIR  := $(OUT_DIR)/obj
INCL_DIR := $(MAKE_DIR)/lib
BUILD_DIR:= $(MAKE_DIR)/build

LIBARIES := $(shell find lib -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
BINARIES := $(shell find bin -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
TARGETS  := $(LIBARIES) $(BINARIES)

CFLAGS := --target=$(TARGET)
LFLAGS := --target=$(TARGET)
CC	   := $(CC)

CFLAGS += -I$(INCL_DIR) # In order to include files (#include header files)
CFLAGS += -std=c99 -g -Wall -Wextra -Wpedantic  # General building flags
LFLAGS += -L$(LIBS_DIR)
# LFLAGS += -Wl,--start-group $(addprefix -l,$(LIBARIES:lib%=%)) -Wl,--end-group

ifeq "$(PROFILE)" "debug"
	ifeq (,$(filter $(TARGET),x86_64-w64-mingw64 x86_64-w64-mingw32))
	    CFLAGS += -fsanitize=address
	    LFLAGS += -fsanitize=address
	endif
endif

ifeq "$(PROFILE)" "release"
	CFLAGS += -O3
endif

export CC MAKE_DIR OBJ_DIR LIBS_DIR BINS_DIR INCL_DIR BUILD_DIR CFLAGS LFLAGS TARGET

build: out/$(PROFILE)/$(TARGET)
	@$(MAKE) -C lib -f build.mk $@
	@$(MAKE) -C bin -f build.mk $@

cli sdl: build
	$(BINS_DIR)/$@

out/$(PROFILE)/$(TARGET):
	@mkdir -p out/$(PROFILE)/
	@cp -r $(BUILD_DIR)/out $@

clean:
	@$(MAKE) -C lib -f build.mk $@
	@$(MAKE) -C bin -f build.mk $@
	@$(MAKE) -C $(BINS_DIR)/tests -f test.mk $@ || true

test: build
	@$(MAKE) -C $(BINS_DIR)/tests -f test.mk $@ || true

req:
	@echo "/!\ Attention !"
	@echo ""
	@echo "Ce projet utilise les libraries système pour compiler avec sdl"
	@echo "Merci de l'installer pour le target $(TARGET) afin que pkg-config"
	@echo "Puisse trouver la librarie !"
	@echo
	@echo "RHEL/Fedora: dnf install SDL3-devel (ou mingw64-SDL3 pour windows)"
	@echo "Debian: apt-get install libsdl3-dev"

docs:
	doxygen

.PHONY: clean build test docs
