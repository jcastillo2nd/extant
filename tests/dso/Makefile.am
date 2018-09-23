AM_CPPFLAGS = -I$(top_srcdir)/include @libcheck_CFLAGS@

LDADD = $(top_builddir)/src/libextant.la @libcheck_LIBS@

TESTS = dso_tests

check_PROGRAMS = dso_tests

dso_tests_SOURCES = dso.c
