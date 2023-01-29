# Name of this program.
NAME    := mz 

# Sources
SOURCE_FILES := generate.c grid.c out_ascii.c path.c \
                permutation.c pq.c random.c union_find.c
SOURCES := $(patsubst %,src/%,$(SOURCE_FILES))
OBJECTS := $(patsubst %,build/%,$(SOURCE_FILES:.c=.o))

# Tests
SOURCES_TEST   := $(shell LC_ALL=C find test -name test-*.c)
OBJECTS_TEST   := $(patsubst test/%,build/%,$(SOURCES_TEST:.c=.test))
# Performance tests
SOURCES_PERF_TEST   := $(shell LC_ALL=C find test -name perf-test-*.c)
OBJECTS_PERF_TEST   := $(patsubst test/%,build/%,$(SOURCES_PERF_TEST:.c=.test))

# Use available processors
CPUS ?= $(shell sysctl -n hw.ncpu || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

# Compiler flags.
CFLAGS         := -std=c18 -pedantic-errors -Wall -Wimplicit-fallthrough -Wextra -Wpedantic -Iinclude -I/usr/local/include -fPIC -march=native
CFLAGS_TEST    :=
LIBRARIES      :=
VERSION        := -DGIT_VERSION="\"$(shell git describe --abbrev=8 --dirty --long --always)\""

# Debug symbols (off by default)
ifneq ("$(DEBUG)", "")
	CFLAGS += -g
endif
# Profiling (off by default)
ifneq ("$(PROFILE)", "")
	CFLAGS += -DPROFILING=1
endif
# Optimization (on by deafult)
ifeq ("$(NOOPT)", "")
	CFLAGS += -O2
endif

# Installation folder prefix.
PREFIX?=/usr/local


#
# Targets
#

all: build/$(NAME)


# program 

build/$(NAME): $(OBJECTS) build/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBRARIES)

build/%.o: src/%.c | build
	$(CC) -o $@ -c $< $(VERSION) $(CFLAGS)

build:
	@mkdir -p build


# tests

build/%.test: test/%.c | build/$(NAME)
	$(CC) -o $@ $< $(CFLAGS) $(CFLAGS_TEST) $(LIBRARIES)

test: tests
	@./scripts/test.sh

tests: $(OBJECTS_TEST)


# performance tests

perftest: perftests
	@./scripts/runperftests.sh

perftests: $(OBJECTS_PERF_TEST)


# installation

install: all
	mkdir -p $(PREFIX)/bin
	install -m 755 build/$(NAME) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(NAME)


# cleaning

clean:
	rm -rf build/

.PHONY: all test tests perftest perftests install uninstall clean
