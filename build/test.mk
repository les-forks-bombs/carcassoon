TEST_CASES := $(shell find . -mindepth 1 -name "*_test" -exec basename {} \;) \
	$(shell find . -mindepth 1 -name "*_test.exe" -exec basename {} \;)

test: $(addsuffix .xml,$(TEST_CASES))

%_test.xml: %_test
	@CMOCKA_XML_FILE='./$*_test.xml' CMOCKA_MESSAGE_OUTPUT=xml \
		./$*_test
	@echo "    TEST  $(notdir $*)"

%_test.exe.xml: %_test.exe
	@CMOCKA_XML_FILE='./$*_test.exe.xml' CMOCKA_MESSAGE_OUTPUT=xml \
		wine ./$*_test.exe
	@echo "    TEST  $(notdir $*)"

clean:
	@rm *_test{,.exe}.xml 2>/dev/null || true

.PHONY: test clean