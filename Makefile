# Primitive build file, will need to replaced when:
#  - We get an actual testing suite
#  - We start compiling asm into static libs

INCLUDE_DIR = include
C_SRC_DIR = src/c
ASM_SRC_DIR = src/asm
BUILD_DIR = build

C_STUB_FILE = string_stubs.c
C_EXPORT_FILE = string.h

TESTS_DIR = tests
TEST_NAMES = test_linkages
TESTS = $(addprefix $(TESTS_DIR)/,$(addsuffix .c,$(TEST_NAMES)))

GCC_FLAGS = $(C_SRC_DIR)/$(C_STUB_FILE) -I$(C_EXPORT_FILE)

test:
	@echo "Executing tests"
	@mkdir -p ./$(BUILD_DIR)
	@for test in $(TESTS) ; do \
		echo " > Performing test $$test..." ; \
		gcc $(GCC_FLAGS) $$test -o$(BUILD_DIR)/test.o ; \
		./$(BUILD_DIR)/test.o ; \
		rm ./$(BUILD_DIR)/test.o ; \
		echo "              test $$test passed" ; \
	done
	@echo "All tests passed"

clean:
	rm -r $(BUILD_DIR)
