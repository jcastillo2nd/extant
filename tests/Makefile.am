SUBDIRS = dso \
		  log \
		  set

AM_CPPFLAGS = -I$(top_srcdir)/include @libcheck_CFLAGS@

LDADD = $(top_builddir)/src/libextant.la @libcheck_LIBS@

TESTS = common_tests

check_PROGRAMS = common_tests

common_tests_SOURCES = common.c

EXTRA_DIST = dso \
			 log \
			 set
