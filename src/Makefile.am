EXTRA_DIST =
CLEANFILES =

AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DLIBEXECDIR=\""$(libexecdir)"\" \
	-I${top_srcdir}/src

AM_CFLAGS = ${my_CFLAGS} \
	-fvisibility=hidden \
	-ffunction-sections \
	-fdata-sections

AM_LDFLAGS = \
	-Wl,--gc-sections \
	-Wl,--as-needed

lib_LTLIBRARIES = libcc2540.la

INST_H_FILES = \
	cc2540.h

libcc2540_la_SOURCES = \
	$(INST_H_FILES) \
	cc2540-private.h \
	cc2540.c

libcc2540_la_LDFLAGS = $(AM_LDFLAGS) \
	-version-info $(LIBCC2540_LT_VERSION_INFO) \
	-Wl,--version-script=libcc2540.sym
libcc2540_la_DEPENDENCIES = libcc2540.sym

libcc2540includedir = $(includedir)/cc2540
libcc2540include_HEADERS = $(INST_H_FILES)
