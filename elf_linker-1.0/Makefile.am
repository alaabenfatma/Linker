ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -Werror -g
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
AM_CFLAGS+=-D BIG_ENDIAN_SIMULATOR
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG

LDADD=-lpthread

COMMON=debug.h debug.c util.h util.c brain.h  elf_dumper.c brain.c crawler.c fuse.c
SUBDIRS=
SUBDIRS+=. Examples_loader

bin_PROGRAMS=
bin_PROGRAMS+=ARM_runner_example


EXTRA_DIST=filtre.pl
SIMULATOR_INTERFACE=csapp.h csapp.c scanner.h scanner.l  brain.h gdb_protocol.h gdb_protocol.c arm_simulator_interface.h arm_simulator_interface.c

ARM_runner_example_SOURCES=$(COMMON) $(SIMULATOR_INTERFACE) ARM_runner_example.c

EXTRA_DIST+=sample_run.sh check.sh

