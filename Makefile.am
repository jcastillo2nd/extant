if HAVE_DOXYGEN
.PHONY: doc
endif

ACLOCAL_AMFLAGS = -I config

SUBDIRS = src
if HAVE_DOXYGEN
SUBDIRS += doc
endif
if HAVE_LIBCHECK
SUBDIRS += tests
endif

EXTRA_DIST = include
