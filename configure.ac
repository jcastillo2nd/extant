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
BUILD_DOC=""
BUILD_TESTS=""

# Doxygen Check
AC_PATH_PROG([DOXYGEN], [doxygen], [no])
AM_CONDITIONAL([HAS_DOXYGEN], [test "x$DOXYGEN" != "xno"])
AS_IF([test "x${HAS_DOXYGEN}" = "xyes"],[BUILD_DOC="doc"],[AC_MSG_WARN([Doxygen not found. Docs not available.])])

# pthread
AC_SEARCH_LIBS([pthread_create], [pthread], [],
               [AC_MSG_FAILURE([Threads library missing])])

## Options
# Tree Max Height determines total nodes supported in AVL/RB Trees
AC_ARG_WITH([tree-max-height],
            AS_HELP_STRING([--with-tree-max-height],
                           [Set AVL/RB tree max height [(default: 16)]]),
            [tree_max_height=$withval],
            [tree_max_height=16,
             AC_DEFINE_UNQUOTED([XTNT_TREE_MAX_HEIGHT], [$tree_max_height], [AVL/RB tree max height (Advanced usage)])])
#            [AC_SUBST([XTNT_TREE_MAX_HEIGHT],
#                      [$withval])],
#            [AC_SUBST([with_tree_max_height],
#                      [16])])

#AC_MSG_NOTICE ([[Received with_tree_max_height as $tree_with_max_height]])

AS_IF([test -n "${with_tree_max_height}"],
      [AS_IF([test "$tree_max_height" -eq "$tree_max_height"],
             [AS_IF([test "$tree_max_height" -gt 7],
                    [AC_DEFINE_UNQUOTED([XTNT_TREE_MAX_HEIGHT], [$tree_max_height], [AVL/RB tree max height (Advanced usage)])
                     AC_MSG_NOTICE([XTNT_TREE_MAX_HEIGHT set to $with_tree_max_height (Advanced usage)])],
                    [AC_MSG_ERROR([tree max height must be an integer of 8 or greater])])])])

#AS_IF([test -n "${with_tree_max_height}"] [test "$tree_max_height" -eq "$tree_max_height"] && [test "$tree_max_height" -gt 7],
#      [AC_DEFINE_UNQUOTED([XTNT_TREE_MAX_HEIGHT], [$tree_max_height], [AVL/RB tree max height (Advanced usage)])
#       AC_MSG_NOTICE([XTNT_TREE_MAX_HEIGHT set to $with_tree_max_height (Advanced usage)])],
#      [AC_MSG_ERROR([tree max height must be an integer of 8 or greater])])

# Convenience defines
AC_SUBST([START_YEAR], [2016])
AC_DEFINE_UNQUOTED([START_YEAR], [$START_YEAR], [Year of project inception])
AC_SUBST([BUILD_YEAR], [`date +%Y`])
AC_DEFINE_UNQUOTED([BUILD_YEAR], [$BUILD_YEAR], [Year of build])
AC_SUBST([RELEASE_BUILD], [`date +%Y%m%d`])
AC_DEFINE_UNQUOTED([BUILD_RELEASE], [$BUILD_RELEASE], [Date of build for release])

PKG_CHECK_MODULES([libcheck],
                  [check],
                  [HAS_CHECK="yes"],
                  [HAS_CHECK="no"])
AS_IF([test "x${HAS_CHECK}" = "xyes"],[BUILD_TESTS="tests"],[AC_MSG_WARN([check framework not found. Unit testing not available.])])

# Program Tests 
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([tests/set/Makefile])

# Program Docs
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([doc/Doxyfile])

# Program Source
AC_CONFIG_FILES([src/Makefile])

# Setup SUBDIRS in Makefile.am
AC_SUBST([BUILD_SUBDIRS], [m4_join([ ], [${BUILD_TESTS}], [$BUILD_DOC])])
AC_SUBST([EXTRAS], [m4_join([ ], [${BUILD_DOC}])])

# Package
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
