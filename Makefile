# Primitive build file, will need to replaced when:
#  - We get an actual testing suite

ASMC = nasm
ASMFLAGS = -X gnu -f elf64
CC = gcc
CFLAGS = -no-pie -fno-asynchronous-unwind-tables -fno-exceptions -masm=intel

SRC_DIR = src
LIBS_DIR = lib
BUILD_DIR = build
TESTS_DIR = tests
INCLUDE_DIR = include
C_SRC_DIR = ${SRC_DIR}/c
ASM_SRC_DIR = ${SRC_DIR}/asm

C_STUB_FILE = string_stubs.c
LIBNAME = aolc

TEST_NAMES = test_linkages test_strlen test_strcpy test_strncpy test_memcpy test_memcpy
TESTS = $(addprefix $(TESTS_DIR)/,$(addsuffix .c,$(TEST_NAMES)))
TEST_LIBNAMES = $(LIBNAME).a sys_libc.a
TEST_LIBS = $(addprefix $(LIBS_DIR)/,$(TEST_LIBNAMES))

STRING_FUNCTIONS = memcpy memmove memchr memcmp memset strcat strncat strchr \
                   strrchr strcmp strncmp strcoll strcpy strncpy strerror strlen \
									 strspn strcspn strpbrk strstr strtok strxfrm \
									 hello_world
STRING_FILES_ASM = $(addprefix $(ASM_SRC_DIR)/,$(addsuffix .S,$(STRING_FUNCTIONS)))
STRING_FILES_O = $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(STRING_FUNCTIONS)))

libs: $(TEST_LIBS)

check: FORCE tests

demo: $(LIBS_DIR)/$(LIBNAME).a
	$(CC) $(CFLAGS) $(C_SRC_DIR)/demo.c $(LIBS_DIR)/$(LIBNAME).a -o demo
	@./demo
	@rm demo

tests: $(TEST_LIBS)
	@echo "Executing tests"
	for test in $(TESTS) ; do \
		echo " > Performing test $$test..." ; \
		$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $$test $(TEST_LIBS) -o $(BUILD_DIR)/test.o ; \
		$(CC) -S $(CFLAGS) -I$(INCLUDE_DIR) $$test -o $(BUILD_DIR)/test.S ; \
		$(BUILD_DIR)/test.o ; \
		echo "              test $$test passed" ; \
	done
	@echo "All tests passed"

clean:
	rm $(BUILD_DIR)/*
	rm ./aolc.a

$(BUILD_DIR)/_sys_string.o:
	${CC} ${CFLAGS} -I$(INCLUDE_DIR) -c $(C_SRC_DIR)/_sys_string.c -o $(BUILD_DIR)/_sys_string.o

$(STRING_FILES_O): $(BUILD_DIR)/%.o: $(ASM_SRC_DIR)/%.S
	@echo " > Compiling assembly for $@..."
	$(ASMC) $(ASMFLAGS) $^ -o $(BUILD_DIR)/$*.o

$(LIBS_DIR)/$(LIBNAME).a: $(STRING_FILES_O)
	@mkdir -p ./$(LIBS_DIR)
	@mkdir -p ./$(BUILD_DIR)
	ar rvs $@ $(STRING_FILES_O)

$(LIBS_DIR)/sys_libc.a: $(BUILD_DIR)/_sys_string.o
	@mkdir -p ./$(LIBS_DIR)
	@mkdir -p ./$(BUILD_DIR)
	ar rvs $@ $(BUILD_DIR)/_sys_string.o

FORCE:
