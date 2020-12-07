# Primitive build file, will need to replaced when:
#  - We get an actual testing suite
#  - We start compiling asm into static libs

ASMC = nasm
ASMFLAGS = -f elf64
CC = gcc
CFLAGS = -no-pie

SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests
INCLUDE_DIR = include
C_SRC_DIR = ${SRC_DIR}/c
ASM_SRC_DIR = ${SRC_DIR}/asm

C_STUB_FILE = string_stubs.c
LIBNAME = aolc

TEST_NAMES = test_linkages
TESTS = $(addprefix $(TESTS_DIR)/,$(addsuffix .c,$(TEST_NAMES)))

#STRING_FUNCTIONS = hello_world ok
STRING_FUNCTIONS = memcpy memmove memchr memcmp memset strcat strncat strchr \
                   strrchr strcmp strncmp strcoll strcpy strncpy strerror strlen \
									 strspn strcspn strpbrk strstr strtok strxfrm \
									 hello_world
STRING_FILES_ASM = $(addprefix $(ASM_SRC_DIR)/,$(addsuffix .S,$(STRING_FUNCTIONS)))
STRING_FILES_O = $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(STRING_FUNCTIONS)))

$(STRING_FILES_O): $(BUILD_DIR)/%.o: $(ASM_SRC_DIR)/%.S
	@echo " > Compiling assembly for $@..."
	$(ASMC) $(ASMFLAGS) $^ -o $(BUILD_DIR)/$*.o

$(LIBNAME).a: $(STRING_FILES_O)
	ar rvs $(LIBNAME).a $(STRING_FILES_O)

demo: $(LIBNAME).a
	$(CC) $(CFLAGS) $(C_SRC_DIR)/demo.c $(LIBNAME).a -o demo
	@./demo
	@rm demo

test: $(LIBNAME).a
	@echo "Executing tests"
	@mkdir -p ./$(BUILD_DIR)
	for test in $(TESTS) ; do \
		echo " > Performing test $$test..." ; \
		$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $$test $(LIBNAME).a -o $(BUILD_DIR)/test.o ; \
		$(BUILD_DIR)/test.o ; \
		rm $(BUILD_DIR)/test.o ; \
		echo "              test $$test passed" ; \
	done
	@echo "All tests passed"

clean:
	rm $(BUILD_DIR)/*
	rm ./aolc.a
