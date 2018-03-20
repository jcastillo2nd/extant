AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libextant.la

TESTS = xtnt_list_delete_tests \
	xtnt_list_insert_tests \
	xtnt_list_replace_tests \
	xtnt_list_search_tests

check_PROGRAMS = xtnt_list_delete_tests \
		 xtnt_list_insert_tests \
		 xtnt_list_replace_tests \
		 xtnt_list_search_tests

xtnt_list_delete_tests_SOURCES = xtnt_list_delete_tests.c

xtnt_list_insert_tests_SOURCES = xtnt_list_insert_tests.c

xtnt_list_replace_tests_SOURCES = xtnt_list_replace_tests.c

xtnt_list_search_tests_SOURCES = xtnt_list_search_tests.c
