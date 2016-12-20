EXTRA_DIST =
CLEAN_FILES =
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libcc2540.pc
EXTRA_DIST += libcc2540.pc.in
CLEAN_FILES += libcc2540.pc

SUBDIRS = src

if ENABLE_TEST
SUBDIRS += test
endif

if ENABLE_DOC
SUBDIRS += doc
endif
