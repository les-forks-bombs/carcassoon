BINS := $(shell find . -maxdepth 1 -mindepth 1 -type d -exec basename {} \;)

build_libcarcassonne: build_libai

build_%: %/
	@$(MAKE) -C $* -f build.mk build
clean_%: %/
	@$(MAKE) -C $* -f build.mk clean

build: $(addprefix build_,$(BINS))
clean: $(addprefix clean_,$(BINS))
