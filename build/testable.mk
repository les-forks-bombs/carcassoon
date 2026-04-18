TESTS := tests

build:: $(ARCHIVE)
	@$(MAKE) -C $(TESTS) -f build.mk build

test:: $(ARCHIVE)
	@$(MAKE) -C $(TESTS) -f build.mk test

clean::
	@$(MAKE) -C $(TESTS) -f build.mk clean
