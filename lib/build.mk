BINS := $(shell find . -maxdepth 1 -mindepth 1 -type d -exec basename {} \;)

libcarcassonne: libai
sdl carcassonne cli: libcarcassonne

$(BINS): %:
	@$(MAKE) -C $* -f build.mk build

$(addprefix clean_,$(BINS)): clean_%:
	@$(MAKE) -C $* -f build.mk clean
$(addprefix test_,$(BINS)): test_%:
	@$(MAKE) -C $* -f build.mk test

build: $(BINS)
clean: $(addprefix clean_,$(BINS))
test: $(addprefix test_,$(BINS))

.PHONY: clean build test $(BINS) $(addprefix clean_,$(BINS)) $(addprefix test_,$(BINS))