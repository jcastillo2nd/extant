AM_CPPFLAGS = -I$(top_srcdir)/include @libcheck_CFLAGS@

LDADD = $(top_builddir)/src/libextant.la @libcheck_LIBS@

TESTS = error_tests \
		common_tests

check_PROGRAMS = error_tests \
				 common_tests

error_tests_SOURCES = error.c

common_tests_SOURCES = common.c

SUBDIRS = dso \
		  log \
		  set
