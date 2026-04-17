BINS := $(shell find . -maxdepth 1 -mindepth 1 -type d -exec basename {} \;)

libcarcassonne: libai

$(BINS): %:
	@$(MAKE) -C $* -f build.mk build

$(addprefix clean_,$(BINS)): clean_%:
	@$(MAKE) -C $* -f build.mk clean

build: $(BINS)
clean: $(addprefix clean_,$(BINS))

.PHONY: clean build $(BINS) $(addprefix clean_,$(BINS))