AM_CPPFLAGS = -I$(top_srcdir)/include @libcheck_CFLAGS@

LDADD = $(top_builddir)/src/libextant.la @libcheck_LIBS@

TESTS = common_tests \
		array_tests \
		list_tests \
		queue_tests \
		stack_tests

check_PROGRAMS = common_tests \
				 array_tests \
				 list_tests \
				 queue_tests \
				 stack_tests

common_tests_SOURCES = common.c

array_tests_SOURCES = array.c

list_tests_SOURCES = list.c

queue_tests_SOURCES = queue.c

stack_tests_SOURCES = stack.c
