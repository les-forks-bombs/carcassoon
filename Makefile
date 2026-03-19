TARGET := x86_64-redhat-linux-gnu
PROFILE := debug

MAKE_DIR := $(PWD)
OUT_DIR  := $(MAKE_DIR)/out/$(PROFILE)/$(TARGET)
# The directory where we're going to store our '.a' archive files
LIBS_DIR := $(OUT_DIR)/lib
BINS_DIR := $(OUT_DIR)/bin
OBJ_DIR  := $(OUT_DIR)/obj
INCL_DIR := $(MAKE_DIR)/lib
BUILD_DIR:= $(MAKE_DIR)/build

CC := clang

LIBARIES := $(shell find lib -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
BINARIES := $(shell find bin -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
TARGETS  := $(LIBARIES) $(BINARIES)

LIBS := -L$(LIBS_DIR) -Wl,--start-group $(addprefix -l,$(LIBARIES:lib%=%)) -Wl,--end-group


CFLAGS := --target=$(TARGET)
ifeq "$(PROFILE)" "debug"
	ifneq ($(TARGET),x86_64-w64-windows-gnu)
	    CFLAGS += -fsanitize=address
	    LDFLAGS += -fsanitize=address
	endif
endif

CFLAGS += -I$(INCL_DIR) # In order to include files (#include header files)
CFLAGS += -std=c99 -g -Wall -Wextra -Wpedantic  # General building flags


export CC MAKE_DIR OBJ_DIR LIBS_DIR BINS_DIR INCL_DIR BUILD_DIR LIBS CFLAGS TARGET

build: out/$(PROFILE)/$(TARGET)
	@$(MAKE) -C lib -f build.mk $@
	@$(MAKE) -C bin -f build.mk $@

out/$(PROFILE)/$(TARGET):
	@mkdir -p out/$(PROFILE)/
	@cp -r $(BUILD_DIR)/out $@

clean:
	@$(MAKE) -C lib -f build.mk $@
	@$(MAKE) -C bin -f build.mk $@
	@$(MAKE) -C $(BINS_DIR)/tests -f test.mk $@ || true

test: build
	@$(MAKE) -C $(BINS_DIR)/tests -f test.mk $@ || true

.PHONY: clean build test
