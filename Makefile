# Primitive build file, will need to replaced when:
#  - We get an actual testing suite

ASMC = nasm
ASMFLAGS = -X gnu -f elf64
CC = g++
CFLAGS = -no-pie -fno-asynchronous-unwind-tables -fno-exceptions -masm=intel -std=c++17

SRC_DIR = src
LIBS_DIR = lib
BUILD_DIR = build
TESTS_DIR = tests
INCLUDE_DIR = include
SUBMODULE_DIR = external
C_SRC_DIR = ${SRC_DIR}/c
ASM_SRC_DIR = ${SRC_DIR}/asm

C_STUB_FILE = string_stubs.c
LIBNAME = aolc

TEST_NAMES = test_linkages test_strlen test_strcpy test_strncpy test_memcpy test_memset
TESTS = $(addprefix $(TESTS_DIR)/,$(addsuffix .cpp,$(TEST_NAMES)))
TESTS_O = $(addprefix $(TESTS_DIR)/,$(addsuffix .cpp,$(TEST_NAMES)))
TEST_LIBNAMES = test_$(LIBNAME).a sys_libc.a
TEST_LIBS = $(addprefix $(LIBS_DIR)/,$(TEST_LIBNAMES))

STRING_FUNCTIONS = memcpy memmove memchr memcmp memset strcat strncat strchr \
                   strrchr strcmp strncmp strcoll strcpy strncpy strerror strlen \
									 strspn strcspn strpbrk strstr strtok strxfrm \
									 hello_world \

STRING_FILES_ASM = $(addprefix $(ASM_SRC_DIR)/,$(addsuffix .S,$(STRING_FUNCTIONS)))
STRING_FILES_O = $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(STRING_FUNCTIONS)))
TEST_STRING_FILES_O = $(addprefix $(BUILD_DIR)/test_,$(addsuffix .o,$(STRING_FUNCTIONS)))

libs: $(TEST_LIBS) $(LIBS_DIR)/$(LIBNAME).a

check: FORCE $(BUILD_DIR)/tests.o

demo: $(LIBS_DIR)/$(LIBNAME).a
	$(CC) $(CFLAGS) $(C_SRC_DIR)/demo.c $(LIBS_DIR)/$(LIBNAME).a -o demo -pthread
	@./demo
	@rm demo

external/googletest/lib/libgtest_main.a external/googletest/lib/libgtest.a:
	git submodule update --init
	(cd $(SUBMODULE_DIR) && make libs)

$(BUILD_DIR)/tests.o: external/googletest/lib/libgtest_main.a external/googletest/lib/libgtest.a \
	                    $(TESTS) $(TESTS_DIR)/compare_buffer_functions.cpp $(LIBS_DIR)/test_$(LIBNAME).a 
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -Iexternal/googletest/googletest/include $^ -o$@ -pthread
	./$@

clean:
	rm $(BUILD_DIR)/*
	rm $(LIBS_DIR)/*

deep-clean:
	rm $(BUILD_DIR)/*
	rm $(LIBS_DIR)/*
	(cd $(SUBMODULE_DIR) && make clean)

$(BUILD_DIR)/_sys_string.o:
	${CC} ${CFLAGS} -I$(INCLUDE_DIR) -c $(C_SRC_DIR)/_sys_string.c -o $(BUILD_DIR)/_sys_string.o -pthread

$(STRING_FILES_O): $(BUILD_DIR)/%.o: $(ASM_SRC_DIR)/%.S
	@echo " > Compiling assembly for $@..."
	$(ASMC) $(ASMFLAGS) -d OVERRIDE_LIBC_NAMES $^ -o $(BUILD_DIR)/$*.o

$(TEST_STRING_FILES_O): $(BUILD_DIR)/test_%.o: $(ASM_SRC_DIR)/%.S
	@echo " > Compiling assembly for $@..."
	$(ASMC) $(ASMFLAGS) $^ -o $(BUILD_DIR)/test_$*.o

$(LIBS_DIR)/$(LIBNAME).a: $(STRING_FILES_O)
	@mkdir -p ./$(LIBS_DIR)
	@mkdir -p ./$(BUILD_DIR)
	ar rvs $@ $(STRING_FILES_O)

$(LIBS_DIR)/test_$(LIBNAME).a: $(TEST_STRING_FILES_O)
	@mkdir -p ./$(LIBS_DIR)
	@mkdir -p ./$(BUILD_DIR)
	ar rvs $@ $(TEST_STRING_FILES_O)

$(LIBS_DIR)/sys_libc.a: $(BUILD_DIR)/_sys_string.o
	@mkdir -p ./$(LIBS_DIR)
	@mkdir -p ./$(BUILD_DIR)
	ar rvs $@ $(BUILD_DIR)/_sys_string.o

FORCE:

