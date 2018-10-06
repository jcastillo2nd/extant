AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libextant.la

libextant_la_SOURCES = extant.c \
					   common.c \
					   log/log.c \
					   set/array.c \
					   set/list.c \
					   set/common.c \
					   set/queue.c \
					   set/stack.c

if DSO_LTDL
libextant_la_SOURCES += dso/dso.c dso/ltdl.c
endif
if DSO_DLFCN
libextant_la_SOURCES += dso/dso.c dso/dlfcn.c
endif
