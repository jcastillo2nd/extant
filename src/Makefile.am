AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libextant.la

libextant_la_SOURCES = extant.c \
					   common.c \
					   set/array.c \
					   set/common.c \
					   set/list.c \
					   set/node.c \
					   set/queue.c \
					   set/stack.c \
					   log/log.c

if DSO_LTDL
libextant_la_SOURCES += dso/dso.c dso/ltdl.c
endif
if DSO_DLFCN
libextant_la_SOURCES += dso/dso.c dso/dlfcn.c
endif
