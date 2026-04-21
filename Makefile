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
	CFLAGS += -O0 -g
	ifeq (,$(filter $(TARGET),x86_64-w64-mingw64 x86_64-w64-mingw32))
	    CFLAGS += -fsanitize=address
	    LFLAGS += -fsanitize=address
		CFLAGS  += -fprofile-instr-generate -fcoverage-mapping
		LFLAGS += -fprofile-instr-generate -fcoverage-mapping
	endif
endif

ifeq "$(PROFILE)" "release"
	CFLAGS += -O3
	LFLAGS += -s
endif

export CC MAKE_DIR OBJ_DIR LIBS_DIR BINS_DIR INCL_DIR BUILD_DIR CFLAGS LFLAGS TARGET


build test clean: out/$(PROFILE)/$(TARGET)
	@$(MAKE) -C lib -f build.mk $@

ifneq ($(filter clean,$(MAKECMDGOALS)),)
build: clean
test: clean
endif


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

bear: out/$(PROFILE)/$(TARGET)
	@bear --output ./out/$(PROFILE)/$(TARGET)/compile_commands.json -- make clean build


TEST_BINS = $(wildcard $(BINS_DIR)/tests/*_test)
FIRST_BIN = $(word 1, $(TEST_BINS))
OTHER_BINS = $(wordlist 2, $(words $(TEST_BINS)), $(TEST_BINS))
OBJECT_FLAGS = $(foreach bin,$(OTHER_BINS),-object $(bin))

coverage: test
	llvm-profdata merge -sparse $(wildcard $(BINS_DIR)/tests/*.profraw) -o $(OUT_DIR)/coverage.profdata
	llvm-cov report $(FIRST_BIN) $(OBJECT_FLAGS) -instr-profile=$(OUT_DIR)/coverage.profdata
	llvm-cov export $(FIRST_BIN) $(OBJECT_FLAGS) -instr-profile=$(OUT_DIR)/coverage.profdata -format=lcov > $(OUT_DIR)/coverage.lcov
	lcov_cobertura $(OUT_DIR)/coverage.lcov --output $(OUT_DIR)/coverage.xml

.PHONY: clean build test docs check lint req cli sdl
