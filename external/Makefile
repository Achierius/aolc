GTEST_REPO_NAME = googletest
GTEST_DIR = $(GTEST_REPO_NAME)
GTEST_LIB_DIR = $(GTEST_DIR)/lib
GTEST_LIBNAMES = libgmock.a libgmock_main.a libgtestl.a libgtest_main.a
GTEST_LIBS = $(addprefix $(GTEST_LIB_DIR)/,$(GTEST_LIBNAMES))
GTEST_MAKE = $(GTEST_DIR)/Makefile

GBENCH_DIR = benchmark
GBENCH_LIB_DIR = $(GBENCH_DIR)/src
GBENCH_LIBNAMES = libbenchmark.a libbenchmark_main.a
GBENCH_LIBS = $(addprefix $(GBENCH_LIB_DIR)/,$(GBENCH_LIBNAMES))
GBENCH_TEST_LINK = $(GBENCH_DIR)/$(GTEST_REPO_NAME)
GBENCH_MAKE = $(GBENCH_DIR)/Makefile

$(GTEST_MAKE):
	(cd $(GTEST_DIR) && cmake .)

$(GTEST_LIBS): $(GTEST_MAKE)
	(cd $(GTEST_DIR) && make)

$(GBENCH_TEST_LINK):
	ln -sr $(GTEST_DIR) -t $(GBENCH_DIR)

$(GBENCH_MAKE): $(GBENCH_TEST_LINK)
	(cd $(GBENCH_DIR) && cmake .)

$(GBENCH_LIBS): $(GBENCH_MAKE) $(GTEST_LIBS)
	(cd $(GBENCH_DIR) && make)

libs: $(GTEST_LIBS) $(GBENCH_LIBS)

clean:
	rm $(GBENCH_TEST_LINK)
	(cd $(GTEST_DIR) && make clean)
	(cd $(GBENCH_DIR) && make clean)
