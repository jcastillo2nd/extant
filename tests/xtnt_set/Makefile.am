SUBDIRS = xtnt_array  \
	  xtnt_list  \
	  xtnt_queue \
	  xtnt_stack \
	  xtnt_tree

AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libextant.la

TESTS = xtnt_common_tests

check_PROGRAMS = xtnt_common_tests

xtnt_node_tests_SOURCES = xtnt_common_tests.c
