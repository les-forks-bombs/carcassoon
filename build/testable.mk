TESTS := tests

build:: $(LIB)
	@$(MAKE) -C $(TESTS) -f build.mk build

test:: $(LIB)
	@$(MAKE) -C $(TESTS) -f build.mk test

clean::
	@$(MAKE) -C $(TESTS) -f build.mk clean
