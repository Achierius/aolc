# Primitive build file, will need to replaced when:
#  - We get an actual testing suite

EMPTY :=
SPACE := $(EMPTY) $(EMPTY)
FORCE:



ASMR = nasm
ASMFLAGS = -X gnu -f elf64
CC = g++
CFLAGS_OPTM = -fno-optimize-strlen -fno-early-inlining -fno-inline -fno-asynchronous-unwind-tables -fno-rtti -fno-exceptions -fno-inline-atomics -fno-inline-functions-called-once -fno-builtin
CFLAGS_CORE = -no-pie -masm=intel -pthread -std=c++17


SRC_DIR = src
C_SRC_DIR = ${SRC_DIR}/c
ASM_SRC_DIR = ${SRC_DIR}/asm

INCLUDE_DIR = include
INTERNAL_INCLUDE_DIR = $(INCLUDE_DIR)/internal

LIBS_DIR = lib
BIN_DIR = $(BUILD_DIR)/bin
BUILD_DIR = build
TESTS_DIR = tests
BENCH_DIR = benchmarks
SUBMODULE_DIR = external

LIBNAME = aolc

STRINGH_FNS  = memcpy memmove memchr memcmp memset strcat strncat strchr \
               strrchr strcmp strncmp strcoll strcpy strncpy strerror strlen \
							 strspn strcspn strpbrk strstr strtok strxfrm \

TEST_NAMES = test_linkages $(addprefix test_,$(STRINGH_FNS))
TESTS = $(addprefix $(TESTS_DIR)/,$(addsuffix .cpp,$(TEST_NAMES)))
TESTS_O = $(addprefix $(TESTS_DIR)/,$(addsuffix .cpp,$(TEST_NAMES)))
TEST_LIBNAMES = test_$(LIBNAME).a# sys_libc.a
TEST_LIBS = $(addprefix $(LIBS_DIR)/,$(TEST_LIBNAMES))

GTEST_DIR = $(SUBMODULE_DIR)/googletest
GTEST_INCLUDE_DIR = $(GTEST_DIR)/googletest/include
GTEST_LIBS = $(GTEST_DIR)/lib/libgtest_main.a $(GTEST_DIR)/lib/libgtest.a


IMPLEMENTED_STRINGH_FNS = memcpy memset strchr \
	      									strcpy strncpy strlen

STRING_FILES_ASM = $(addprefix $(ASM_SRC_DIR)/,$(addsuffix .S,$(STRINGH_FNS)))
STRING_FILES_O = $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(STRINGH_FNS)))
TEST_STRING_FILES_O = $(addprefix $(BUILD_DIR)/test_,$(addsuffix .o,$(STRINGH_FNS)))

BENCHMARK_DIR = $(SUBMODULE_DIR)/benchmark
BENCHMARK_LIBS = $(BENCHMARK_DIR)/src/libbenchmark.a
BENCHMARK_INCLUDE_DIR = $(BENCHMARK_DIR)/include


regression: check

check: GTEST_FILTER = $(subst $(SPACE),:,$(addsuffix .*,$(IMPLEMENTED_STRINGH_FNS)))
check: $(BIN_DIR)/test_suite.o FORCE
	$< --gtest_filter=$(GTEST_FILTER) 

check-full: GTEST_FILTER = $(subst $(SPACE),:,$(addsuffix .*,$(STRINGH_FNS)))
check-full: $(BIN_DIR)/test_suite.o FORCE
	$< --gtest_filter=$(GTEST_FILTER)

check-sanity: GTEST_FILTER = $(subst $(SPACE),:,$(addsuffix .*,$(STRINGH_FNS)))
check-sanity: $(BIN_DIR)/meta_tests.o FORCE
	$< --gtest_filter=$(GTEST_FILTER)

check-%: GTEST_FILTER = $**
check-%: $(BIN_DIR)/test_suite.o FORCE
	$< --gtest_filter=$(GTEST_FILTER)

# !!! RECURSIVE MAKE !!!
$(GTEST_LIBS) $(GBENCH_LIBS):
	git submodule update --init
	(cd $(SUBMODULE_DIR) && make libs)

$(BIN_DIR)/test_suite.o: external/googletest/lib/libgtest_main.a external/googletest/lib/libgtest.a \
	                       $(TESTS) $(INTERNAL_INCLUDE_DIR)/aolc/compare_buffer_functions.h $(LIBS_DIR)/test_$(LIBNAME).a  \
												 | $(BIN_DIR)/.sent
	$(CC) $(CFLAGS_CORE) $(CFLAGS_OPTM) -O1 -I$(INTERNAL_INCLUDE_DIR) -I$(GTEST_INCLUDE_DIR) $^ -o$@

$(BIN_DIR)/meta_tests.o: external/googletest/lib/libgtest_main.a external/googletest/lib/libgtest.a \
	                       $(TESTS) $(INTERNAL_INCLUDE_DIR)/aolc/compare_buffer_functions.h $(LIBS_DIR)/glibc_standin_test_$(LIBNAME).a\
												 | $(BIN_DIR)/.sent
	$(CC) $(CFLAGS_CORE) $(CFLAGS_OPTM) -O1 -I$(INTERNAL_INCLUDE_DIR) -I$(GTEST_INCLUDE_DIR) $^ -o$@

demo: $(LIBS_DIR)/$(LIBNAME).a
	$(CC) $(CFLAGS_CORE) $(CFLAGS_OPTM) -O1 -I$(INCLUDE_DIR) $(C_SRC_DIR)/demo.c $(LIBS_DIR)/$(LIBNAME).a -o$(BUILD_DIR)/demo
	@./$(BUILD_DIR)/demo

lc = $(subst a,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

uc = $(subst a,A,$(subst b,B,$(subst c,C,$(subst d,D,$(subst e,E,$(subst f,F,$(subst g,G,$(subst h,H,$(subst i,I,$(subst j,J,$(subst k,K,$(subst l,L,$(subst m,M,$(subst n,N,$(subst o,O,$(subst p,P,$(subst q,Q,$(subst r,R,$(subst s,S,$(subst t,T,$(subst u,U,$(subst v,V,$(subst w,W,$(subst x,X,$(subst y,Y,$(subst z,Z,$1))))))))))))))))))))))))))


bench-all: $(addprefix bench-,$(STRINGH_FNS)) | FORCE

bench-regression: $(addprefix bench-,$(IMPLEMENTED_STRINGH_FNS)) | FORCE

# requires that the system 'python' defaults to python3
bench-%: COMPARE_PY = $(BENCHMARK_DIR)/tools/compare.py
bench-%: $(BUILD_DIR)/bench_glibc-%.json $(BUILD_DIR)/bench_aolc-%.json $(BENCHMARK_LIBS) | FORCE
	@pip3 install -r $(BENCHMARK_DIR)/requirements.txt
	@echo "\e[4mComparative Benchmark: \e[31m$(call uc,$*)\e[0m"
	$(COMPARE_PY) benchmarks $(word 1,$^) $(word 2,$^)

$(BIN_DIR)/bench_glibc-%.o: $(BENCH_DIR)/bench_%.cpp $(LIBS_DIR)/test_$(LIBNAME).a $(BENCHMARK_LIBS) | $(BIN_DIR)/.sent
	$(CC) -D__BENCH_GLIBC__ $(CFLAGS_CORE) $(CFLAGS_OPTM) -O0 -I$(INTERNAL_INCLUDE_DIR) -I$(BENCHMARK_INCLUDE_DIR) $^ -o$@

$(BIN_DIR)/bench_aolc-%.o: $(BENCH_DIR)/bench_%.cpp $(LIBS_DIR)/test_$(LIBNAME).a $(BENCHMARK_LIBS) | $(BIN_DIR)/.sent
	$(CC) -D__BENCH_AOLC__ $(CFLAGS_CORE) $(CFLAGS_OPTM) -O0 -I$(INTERNAL_INCLUDE_DIR) -I$(BENCHMARK_INCLUDE_DIR) $^ -o$@

$(BUILD_DIR)/bench_glibc-%.json: $(BIN_DIR)/bench_glibc-%.o | $(BUILD_DIR)/.sent
	@$< --benchmark_out_format=json --benchmark_out=$(BUILD_DIR)/bench_glibc-$*.json 1> /dev/null

$(BUILD_DIR)/bench_aolc-%.json: $(BIN_DIR)/bench_aolc-%.o | $(BUILD_DIR)/.sent
	@$< --benchmark_out_format=json --benchmark_out=$(BUILD_DIR)/bench_aolc-$*.json 1> /dev/null


clean:
	rm -r $(BUILD_DIR)/*
	rm $(LIBS_DIR)/*

clean-all: clean
	rmdir $(BUILD_DIR)
	rmdir $(LIBS_DIR)
	(cd $(SUBMODULE_DIR) && make clean)


libs: $(TEST_LIBS) $(LIBS_DIR)/$(LIBNAME).a

$(LIBS_DIR)/$(LIBNAME).a: $(STRING_FILES_O) | $(LIBS_DIR)/.sent
	@mkdir -p ./$(LIBS_DIR)
	@mkdir -p ./$(BUILD_DIR)
	ar rvs $@ $(STRING_FILES_O)

$(LIBS_DIR)/test_$(LIBNAME).a: $(TEST_STRING_FILES_O) | $(LIBS_DIR)/.sent
	@mkdir -p ./$(LIBS_DIR)
	@mkdir -p ./$(BUILD_DIR)
	ar rvs $@ $(TEST_STRING_FILES_O)

$(LIBS_DIR)/glibc_standin_test_$(LIBNAME).a: $(BUILD_DIR)/glibc_standin_test_string.o | $(LIBS_DIR)/.sent
	ar rvs $@ $<

$(BUILD_DIR)/glibc_standin_test_string.o: CC = gcc -c
$(BUILD_DIR)/glibc_standin_test_string.o: $(C_SRC_DIR)/sys_libc_standin.c
	$(CC) $(CFLAGS_OPTM) -I$(INTERNAL_INCLUDE_DIR) $< -o $@

$(STRING_FILES_O): $(BUILD_DIR)/%.o: $(ASM_SRC_DIR)/%.S | $(BUILD_DIR)/.sent
	@echo " > Compiling assembly for $@..."
	$(ASMR) $(ASMFLAGS) -d OVERRIDE_LIBC_NAMES $^ -o $(BUILD_DIR)/$*.o

$(TEST_STRING_FILES_O): $(BUILD_DIR)/test_%.o: $(ASM_SRC_DIR)/%.S | $(BUILD_DIR)/.sent
	@echo " > Compiling assembly for $@..."
	$(ASMR) $(ASMFLAGS) $< -o $(BUILD_DIR)/test_$*.o


.PRECIOUS: %/.sent
%/.sent:
	mkdir -p ${@D}
	touch $@
