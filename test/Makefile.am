AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DLIBEXECDIR=\""$(libexecdir)"\" \
	-I${top_srcdir}/src \
	-I${top_builddir}/src

AM_CFLAGS = ${my_CFLAGS}

LDADD = ${top_builddir}/src/libcc2540.la

TEST_COMMON = \
	test-common.h \
	test-common.c

noinst_PROGRAMS = \
	test-cc2540 \
	test-gap-dev-init \
	test-gap-dev-addr-set \
	test-gap-dev-disc \
	test-gap-dev-disc-end \
	test-gap-disc-set \
	test-gap-adv-set \
	test-gap-param-set \
	test-gap-param-get

test_gap_dev_init_SOURCES = ${TEST_COMMON} test-gap-dev-init.c
test_gap_dev_addr_set_SOURCES = ${TEST_COMMON} test-gap-dev-addr-set.c
test_gap_dev_disc_SOURCES = ${TEST_COMMON} test-gap-dev-disc.c
test_gap_dev_disc_end_SOURCES = ${TEST_COMMON} test-gap-dev-disc-end.c
test_gap_disc_set_SOURCES = ${TEST_COMMON} test-gap-disc-set.c
test_gap_adv_set_SOURCES = ${TEST_COMMON} test-gap-adv-set.c
