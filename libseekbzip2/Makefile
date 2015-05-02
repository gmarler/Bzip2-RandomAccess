UNAME=/bin/uname

OSNAME := $(UNAME) -s

# TODO: Need a way to differentiate between whether we need a 32- or 64-bit
# version of this library created, to match the bitness of the Perl we'll be
# linking the XS routine(s) to.
CC     := $(if $(filter $(OSNAME),SunOS),cc -m64,gcc)
CFLAGS = -g -Isrc # -DNDEBUG -DTESTING
CFLAGS += $(if $(filter $(OSNAME),SunOS),-xcode=pic32,-fPIC)
# LDFLAGS += $(if $(filter $(OSNAME),SunOS),-Lbuild/ -Rbuild/,-Lbuild/)
SHARED_LIB_FLAG := $(if $(filter $(OSNAME),SunOS),-G,-shared)

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libseekbzip2.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

#$(TARGET): CFLAGS += -xcode=pic32
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) $(SHARED_LIB_FLAG) -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin

# Unit Tests
.PHONY: tests
# tests: CFLAGS += -Lbuild/ -Rbuild/ -lseekbzip2
# TODO: is -ldl just for Linux, or for Solaris too?
tests: LDFLAGS += -Lbuild/ $(if $(filter $(OSNAME),SunOS),-Rbuild/,)
tests: LDLIBS = -lseekbzip2 -ldl
tests: $(TESTS) $(SO_TARGET)
	ksh ./tests/runtests.sh

PROGS=bzip-table seek-bunzip

all: $(SO_TARGET) tests

bzip-table : bzip-table.o micro-bunzip.o
seek-bunzip : seek-bunzip.o micro-bunzip.o

micro-bunzip : micro-bunzip.c
	$(CC) $(CFLAGS) -DMICRO_BUNZIP_MAIN $< -o $@

test: bzip-table seek-bunzip
	# Generate 10 megabytes of random data
	dd if=/dev/random of=test_random.dat bs=1024k count=4
	# Compress but keep original
	bzip2 --force --keep test_random.dat
	# Run bzip2recover to build a seperate bz2 file for each block
	bzip2recover test_random.dat.bz2
	# Build table
	./bzip-table < test_random.dat.bz2 > test_random.dat.bz2t
	# Grab each block using seek-bunzip and compare to recovered block
	nl test_random.dat.bz2t | while read num pos size; \
	do \
		bzip2 -d -c < rec0000$${num}test_random.dat.bz2 > test_expected; \
		./seek-bunzip $${pos} < test_random.dat.bz2 > test_actual; \
		cmp test_expected test_actual; \
		if [ $$? -ne 0 ]; then \
			echo "FAILED: Blocks do not match"; \
			exit 1; \
		fi; \
	done
	@echo "SUCCESS!"

clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/test.log
	find . -name '*.gc*' -exec rm {} \;


