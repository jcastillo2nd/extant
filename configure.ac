AC_PREREQ([2.69])
AC_INIT(extant, 0.1)

AC_LANG([C])

AC_CONFIG_MACRO_DIRS([config])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/extant.c])
AC_CONFIG_HEADERS([src/config.h])

AC_PROG_LIBTOOL
AC_PROG_CC

AC_TYPE_LONG_DOUBLE
AC_TYPE_LONG_LONG_INT
AC_PROG_CC_C99

LT_INIT

AM_INIT_AUTOMAKE([foreign subdir-objects])

# BUILD_SUBDIRS
BUILD_TESTS=""

# Doxygen Check
AC_PATH_PROG([DOXYGEN], [doxygen], [no])
AS_IF([test "x${DOXYGEN}" != "xno"],
      [AM_CONDITIONAL([HAVE_DOXYGEN], [true])],
      [AC_MSG_WARN([Doxygen not found. Docs not available.])])

# pthread
AC_SEARCH_LIBS([pthread_create], [pthread], [],
               [AC_MSG_FAILURE([Threads library missing])])

# with libltdl
AC_ARG_WITH([libltdl],
            AS_HELP_STRING([--with-libltdl], [Enforce libltdl DSO library]))

# check libltdl
AS_IF([test "x${with_libltdl}" != "xno"],
      [AC_CHECK_LIB([ltdl],
                    [lt_dlinit])])

AS_IF([test "x${with_libltdl}" = "xyes"] && [test "${ac_cv_lib_ltdl_lt_dlinit}" = "no"],
      [AC_MSG_ERROR([ltdl was specified but not found])])

AS_IF([test "${ac_cv_lib_ltdl_lt_dlinit}" = "yes"],
      [dso_handler="ltdl"])

# check dlfcn fallback
AS_IF([test "${dso_handler}" != "ltdl"],
      [AC_CHECK_LIB([dl], [dlopen])]
      [AS_IF([test "${ac_cv_lib_dl_dlopen}" = "no"],
             [dso_handler=[""]]
             [AC_MSG_NOTICE([libltdl nor libdl available. No DSO support.])])]
      [AS_IF([test "${ac_cv_lib_dl_dlopen}" = "yes"],
             [dso_handler="dlfcn"])])

# DSO variables and defines
AS_IF([test -n "${dso_handler}"],
      [AM_CONDITIONAL([DSO_DLFCN], [test "${dso_handler}" = "dlfcn"])]
      [AM_CONDITIONAL([DSO_LTDL], [test "${dso_handler}" = "ltdl"])]
      [AC_DEFINE([XTNT_HAVE_DSO], [1], [A DSO Handler was detected])])

# Options
AC_ARG_WITH([tree-max-height],
            AS_HELP_STRING([--with-tree-max-height],
                           [Set AVL/RB tree max height [(default: 16)]]),
            [tree_max_height=$withval],
            [tree_max_height=16,
             AC_DEFINE_UNQUOTED([XTNT_TREE_MAX_HEIGHT], [$tree_max_height], [AVL/RB tree max height (Advanced usage)])])

AS_IF([test -n "${with_tree_max_height}"],
      [AS_IF([test "$tree_max_height" -eq "$tree_max_height"],
             [AS_IF([test "$tree_max_height" -gt 7],
                    [AC_DEFINE_UNQUOTED([XTNT_TREE_MAX_HEIGHT], [$tree_max_height], [AVL/RB tree max height (Advanced usage)])
                     AC_MSG_NOTICE([XTNT_TREE_MAX_HEIGHT set to $with_tree_max_height (Advanced usage)])],
                    [AC_MSG_ERROR([tree max height must be an integer of 8 or greater])])])])

# Convenience defines
AC_SUBST([START_YEAR], [2016])
AC_DEFINE_UNQUOTED([START_YEAR], [$START_YEAR], [Year of project inception])
AC_SUBST([BUILD_YEAR], [`date +%Y`])
AC_DEFINE_UNQUOTED([BUILD_YEAR], [$BUILD_YEAR], [Year of build])
AC_SUBST([RELEASE_BUILD], [`date +%Y%m%d`])
AC_DEFINE_UNQUOTED([BUILD_RELEASE], [$BUILD_RELEASE], [Date of build for release])

PKG_CHECK_MODULES([libcheck],
                  [check],
                  [LIBCHECK="yes"])
AS_IF([test "x${LIBCHECK}" != "xyes"],
      [AC_MSG_WARN([libcheck not found. Unit tests not available.])],
      [AM_CONDITIONAL([HAVE_LIBCHECK], [true])])

# Program Tests 
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([tests/dso/Makefile])
AC_CONFIG_FILES([tests/log/Makefile])
AC_CONFIG_FILES([tests/set/Makefile])

# Program Docs
AC_CONFIG_FILES([doc/Makefile])
AC_OUTPUT([doc/Doxyfile])

# Program Source
AC_CONFIG_FILES([src/Makefile])

# Package
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
