CC 		:= clang
TARGET 	:= $(shell $(CC) -dumpmachine)

ifeq "$(TARGET)" "wasm32-unknown-emscripten"
	CC := emcc
endif

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

TESTS_LFLAGS := 
CFLAGS += --target=$(TARGET)
LFLAGS += --target=$(TARGET)

CFLAGS += -I$(DIR)/lib
CFLAGS += -std=c99 -Wall -Wextra -Wpedantic -Wdocumentation  # General building flags
LFLAGS += -L$(OUT) -lm

PKG_CONF := PKG_CONFIG_PATH=/usr/$(TARGET)/lib/pkgconfig pkg-config

LFLAGS += $(shell $(PKG_CONF) --personality=$(TARGET) sdl3 --libs)
CFLAGS += $(shell $(PKG_CONF) --personality=$(TARGET) sdl3 --cflags)
LFLAGS += $(shell $(PKG_CONF) --personality=$(TARGET) sdl3-image --libs)
CFLAGS += $(shell $(PKG_CONF) --personality=$(TARGET) sdl3-image --cflags)
LFLAGS += $(shell $(PKG_CONF) --personality=$(TARGET) sdl3-ttf --libs)
CFLAGS += $(shell $(PKG_CONF) --personality=$(TARGET) sdl3-ttf --cflags)

ifeq "$(PROFILE)" "debug"
	CFLAGS += -O0 -g -D DEBUG
	ifneq "$(TARGET)" "x86_64-w64-mingw32"
	    CFLAGS += -fsanitize=address
	    LFLAGS += -fsanitize=address
		CFLAGS  += -fprofile-instr-generate -fcoverage-mapping
		LFLAGS += -fprofile-instr-generate -fcoverage-mapping
	endif
endif

RUNNER := 
BINARY := $(OUT)/bin/carcassonne$(EXT)

ifeq "$(TARGET)" "wasm32-unknown-emscripten"
	RUNNER := node
	EXT := .js
	LFLAGS += -sEXIT_RUNTIME=1 -sALLOW_MEMORY_GROWTH -s USE_LIBPNG=1 -s USE_ZLIB=1 -s SHARED_MEMORY=0 -s USE_PTHREADS=0 -s USE_FREETYPE=1 -s USE_HARFBUZZ=1 -gsource-map
	TESTS_LFLAGS += -s NODERAWFS=1
	BINARY := $(OUT)/bin/index.html
endif

ifeq "$(PROFILE)" "release"
	CFLAGS += -O3 -flto=thin
	LFLAGS += -flto=thin
	LFLAGS += -fuse-ld=lld

	ifeq "$(TARGET)" "wasm32-unknown-emscripten"
		LFLAGS += -g
	else
		LFLAGS += -s
	endif
endif

ifeq "$(TARGET)" "x86_64-w64-mingw32"
    RUNNER := wine
	EXT := .exe
	LFLAGS += -lshlwapi -mwindows
	BINARY := $(OUT)/bin/carcassonne$(EXT)
endif


$(info Using CC: $(CC))
$(info Using CFLAGS: $(CFLAGS))
$(info Using LFLAGS: $(LFLAGS))

ifneq ($(filter clean,$(MAKECMDGOALS)),)
build: clean
test: clean
endif

build: $(BINARY)
cli sdl: $(OUT)/bin/carcassonne$(EXT)
	$(RUNNER) $(BINARY) -m $@

include lib/build.mk

all: build $(TESTS)

TESTS_XMLS := $(addsuffix .xml,$(TESTS))
TESTS_COVE := $(addsuffix .profraw,$(TESTS))

%.xml %.profraw: %
	@cd $(dir $<) && \
		CMOCKA_XML_FILE='$(notdir $*).xml' \
		LLVM_PROFILE_FILE="$(notdir $*).profraw" \
		CMOCKA_MESSAGE_OUTPUT=xml,stdout \
		CMOCKA_ERROR_OUTPUT=stdout \
		$(RUNNER) $<

test: $(TESTS_XMLS) $(TESTS_COVE)
CLEAN += $(TESTS_XMLS) $(TESTS_COVE)

$(OUT)/objs/%.o: lib/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	$(info $(TAB)CC $@)


$(OUT)/AppDir: $(OUT)/bin/carcassonne
	@mkdir -p $(OUT)/AppDir/usr/lib
	@cp -r $(OUT)/bin $(OUT)/AppDir/usr/

	@cp $(UTIL_DIR)/eu.telecomnancy.pp2i.carcassonne.desktop $(OUT)/AppDir/
	@cp $(UTIL_DIR)/eu.telecomnancy.pp2i.carcassonne.png $(OUT)/AppDir/
	@cp $(UTIL_DIR)/AppRun $(OUT)/AppDir/

	@ldd $(OUT)/bin/carcassonne | grep "=> /" | awk '{print $$3}' | while read -r lib_path; do \
		case "$$lib_path" in \
			*libc.so* | *libm.so* | *libdl.so* | *libpthread.so* | \
			*libresolv.so* | *librt.so* | *ld-linux* | *libstdc++* | *libgcc_s*) \				;; \
			*) \
				echo "Copying dependency: $$lib_path"; \
				cp "$$lib_path" $(OUT)/AppDir/usr/lib \
				;; \
		esac; \
	done

APPIMAGE := Carcassonne-x86_64.AppImage

$(OUT)/$(APPIMAGE): $(OUT)/AppDir
	cd $(OUT); appimagetool $(OUT)/AppDir/

CLEAN += $(OUT)/AppDir $(OUT)/$(APPIMAGE)

appimage: $(OUT)/$(APPIMAGE)

public/docs:
	mkdir -p $@
	doxygen
CLEAN += public/docs

docs: public/docs

format:
	@find . -iname "*.h" -o -iname "*.c" | xargs clang-format -i

tidy: $(OUT)/compile_commands.json
	@run-clang-tidy -quiet -p $(OUT)

check:
	@find . -iname "*.h" -o -iname "*.c" | xargs clang-format --dry-run --Werror

$(OUT)/compile_commands.json:
	@mkdir -p $(OUT)
	@make clean
	+@bear --output $(OUT)/compile_commands.json -- make all -j$(shell nproc)

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

