AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DLIBEXECDIR=\""$(libexecdir)"\" \
	-I${top_srcdir}/src \
	-I${top_builddir}/src

AM_CFLAGS = ${my_CFLAGS}

LDADD = ${top_builddir}/src/libcc2540.la

noinst_PROGRAMS = \
	test-cc2540
