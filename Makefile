CC 		:= clang
TARGET 	:= $(shell $(CC) -dumpmachine)
PROFILE := debug
CLEAN :=
NULL :=
TAB := $(NULL)	$(NULL)
TESTS :=
DIR_STACK := 
DIR := $(PWD)
UTIL_DIR := $(DIR)/build
EXT :=
OUT  := $(PWD)/out/$(PROFILE)/$(TARGET)

CFLAGS += --target=$(TARGET)

CFLAGS += -I$(DIR)/lib
CFLAGS += -std=c99 -Wall -Wextra -Wpedantic -Wdocumentation  # General building flags
LFLAGS += -L$(OUT) -lm

LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3 --cflags)
LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-image --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-image --cflags)
LFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-ttf --libs)
CFLAGS += $(shell pkg-config --personality=$(TARGET) sdl3-ttf --cflags)

ifeq "$(PROFILE)" "debug"
	CFLAGS += -O0 -g -D DEBUG
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

RUNNER := 
ifneq (,$(filter $(TARGET),x86_64-w64-mingw64 x86_64-w64-mingw32))
    RUNNER := wine
	EXT := .exe
	LFLAGS += -lshlwapi
endif

ifneq ($(filter clean,$(MAKECMDGOALS)),)
build: clean
test: clean
endif

build: $(OUT)/bin/sdl$(EXT) $(OUT)/bin/cli$(EXT) $(OUT)/bin/carcassonne$(EXT)

cli sdl:
	$(OUT)/bin/carcassonne -m $@

include lib/build.mk

all: build $(TESTS)

TESTS_XMLS := $(addsuffix .xml,$(TESTS))
TESTS_COVE := $(addsuffix .profraw,$(TESTS))

%.xml %.profraw: %
	@CMOCKA_XML_FILE='$*.xml' \
		LLVM_PROFILE_FILE="$*.profraw" \
		CMOCKA_MESSAGE_OUTPUT=xml,stdout \
		CMOCKA_ERROR_OUTPUT=stdout \
		$(RUNNER) $<

test: $(TESTS_XMLS) $(TESTS_COVE)
CLEAN += $(TESTS_XMLS) $(TESTS_COVE)

$(OUT)/objs/%.o: lib/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	$(info $(TAB)CC $@)

public/:
	doxygen
CLEAN += public/

docs: public/

format:
	@find . -iname "*.h" -o -iname "*.c" | xargs clang-format -i

tidy: $(OUT)/compile_commands.json
	@run-clang-tidy -p $(OUT)

check:
	@find . -iname "*.h" -o -iname "*.c" | xargs clang-format --dry-run --Werror

$(OUT)/compile_commands.json:
	@mkdir -p $(OUT)
	@make clean
	@bear --output $(OUT)/compile_commands.json -- make all -j$(shell nproc)

bear: $(OUT)/compile_commands.json

CLEAN += $(OUT)/compile_commands.json

FIRST_BIN := $(firstword $(TESTS))
OTHER_BINS := $(wordlist 2, $(words $(TESTS)), $(TESTS))
OBJECT_FLAGS := $(addprefix -object , $(OTHER_BINS))

$(OUT)/coverage.profdata: $(TESTS_COVE)
	@llvm-profdata merge -sparse $(TESTS_COVE) -o $(OUT)/coverage.profdata
CLEAN += $(OUT)/coverage.profdata

coverage: $(OUT)/coverage.profdata
	@llvm-cov report $(FIRST_BIN) $(OBJECT_FLAGS) -instr-profile=$(OUT)/coverage.profdata

$(OUT)/coverage.lcov: $(OUT)/coverage.profdata
	@llvm-cov export $(FIRST_BIN) $(OBJECT_FLAGS) -instr-profile=$(OUT)/coverage.profdata -format=lcov > $(OUT)/coverage.lcov
CLEAN += $(OUT)/coverage.lcov

$(OUT)/coverage.xml: $(OUT)/coverage.lcov
	@lcov_cobertura $(OUT)/coverage.lcov --output $(OUT)/coverage.xml
CLEAN += $(OUT)/coverage.xml

coverage-xml: coverage $(OUT)/coverage.xml

clean:
	@rm -rf $(CLEAN)

req:
	@echo "/!\ Attention !"
	@echo ""
	@echo "Ce projet utilise les libraries système pour compiler avec sdl et cmocka"
	@echo "Merci de l'installer pour le target $(TARGET) afin que pkg-config"
	@echo "Puisse trouver la librarie !"
	@echo
	@echo "RHEL/Fedora: dnf install libSDL3-devel libSDL3_ttf-devel libSDL3_image-devel libcmocka-devel"
	@echo "Debian: apt-get install libsdl3-dev libsdl3-ttf-dev libsdl3-image-dev libcmocka-dev"


.PHONY: clean build test docs check format coverage bear tidy req cli sdl coverage-xml

