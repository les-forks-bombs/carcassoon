BINS := $(shell find . -maxdepth 1 -mindepth 1 -type d -exec basename {} \;)

$(BINS): %:
	@$(MAKE) -C $* -f build.mk build

$(addprefix clean_,$(BINS)): clean_%:
	@$(MAKE) -C $* -f build.mk clean
$(addprefix test_,$(BINS)): test_%:
	@$(MAKE) -C $* -f build.mk test

sdl carcassonne cli: libcarcassonne libai

build: $(BINS)
clean: $(addprefix clean_,$(BINS))
test: $(addprefix test_,$(BINS))

.PHONY: \
	clean build test \
	$(BINS) \
	$(addprefix clean_,$(BINS)) \
	$(addprefix test_,$(BINS))
