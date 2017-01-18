# versioning
m4_define([libcc2540_major_version],[0])
m4_define([libcc2540_minor_version],[0])
m4_define([libcc2540_micro_version],[0])
m4_define([libcc2540_extra_version],[])
m4_define([libcc2540_version],[libcc2540_major_version.libcc2540_minor_version.libcc2540_micro_version()libcc2540_extra_version])

# API version
m4_define([libcc2540_api_version],[libcc2540_major_version.libcc2540_minor_version.libcc2540_micro_version()libcc2540_extra_version])

# Libtool versioning.
# Before making a release, the libtool version should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A

# libtool version
m4_define([libcc2540_lt_current],[0])
m4_define([libcc2540_lt_revision],[0])
m4_define([libcc2540_lt_age],[0])
m4_define([libcc2540_lt_version_info],[libcc2540_lt_current:libcc2540_lt_revision:libcc2540_lt_age])
m4_define([libcc2540_lt_current_minus_age],[m4_eval(libcc2540_lt_current - libcc2540_lt_age)])

# binary version
m4_define([libcc2540_binary_version],[libcc2540_lt_current])

AC_PREREQ(2.60)
AC_INIT([libcc2540],
	[libcc2540_version],
	[inigomartinez@gmail.com],
	[libcc2540],
	[https://github.com/inigomartinez/liblibcc2540])
AC_COPYRIGHT([Copyright (C) 2016-2017 Iñigo Martínez])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_AUX_DIR([build-aux])

AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE

AC_PREFIX_DEFAULT([/usr])

AM_INIT_AUTOMAKE([
	check-news
	foreign
	1.11
	-Wall
	-Wno-portability
	silent-rules
	serial-tests
	tar-pax
	no-dist-gzip
	dist-xz
	subdir-objects
])
AM_SILENT_RULES([yes])

LT_PREREQ([2.2.6])
LT_INIT([
	disable-static
	pic-only
])

AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CC_C99
AC_C_INLINE

# header support
AC_HEADER_STDC
AC_HEADER_STDBOOL

AC_ARG_ENABLE([test],
	AS_HELP_STRING([--disable-test], [disable test building @<:@default=enabled@:>@]),
	[], enable_test=yes)
AM_CONDITIONAL(ENABLE_TEST, test "x$enable_test" = "xyes")
AS_IF([test "x$enable_test" = "xyes"], [
	AC_DEFINE(ENABLE_TEST, [1], [Test building.])
])

AC_ARG_ENABLE([doc],
	AS_HELP_STRING([--disable-doc], [disable documentation building @<:@default=disabled@:>@]),
	[], enable_doc=no)
AM_CONDITIONAL(ENABLE_DOC, test "x$enable_doc" = "xyes")
AS_IF([test "x$enable_doc" = "xyes"], [
	AC_DEFINE(ENABLE_DOC, [1], [Documentation building.])
])

my_CFLAGS="\
-Wall \
-Wchar-subscripts \
-Wformat-security \
-Wmissing-declarations \
-Wmissing-prototypes \
-Wnested-externs \
-Wpointer-arith \
-Wshadow \
-Wsign-compare \
-Wstrict-prototypes \
-Wtype-limits \
-pedantic
"
AC_SUBST([my_CFLAGS])

# versioning
AC_SUBST([LIBCC2540_MAJOR_VERSION],[libcc2540_major_version])
AC_SUBST([LIBCC2540_MINOR_VERSION],[libcc2540_minor_version])
AC_SUBST([LIBCC2540_MICRO_VERSION],[libcc2540_micro_version])
AC_SUBST([LIBCC2540_VERSION],[libcc2540_version])

AC_SUBST([LIBCC2540_API_VERSION],[libcc2540_api_version])
AC_SUBST([LIBCC2540_BINARY_VERSION],[libcc2540_binary_version])

AC_SUBST([LIBCC2540_LT_VERSION_INFO],[libcc2540_lt_version_info])

# Silence warning: ar: 'u' modifier ignored since 'D' is the default
AC_SUBST(AR_FLAGS, [cr])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	test/Makefile
	doc/Makefile
	libcc2540.pc
])

AC_OUTPUT
AC_MSG_RESULT([
	$PACKAGE $VERSION
	=====

	prefix:			${prefix}
	sysconfdir:		${sysconfdir}
	libdir:			${libdir}
	includedir:		${includedir}

	compiler:		${CC}
	cflags:			${CFLAGS}
	ldflags:		${LDFLAGS}

	test:			${enable_test}
	doc:			${enable_doc}
])
