# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([RaquelTeachingTool], [0.1.36])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_SRCDIR([TokeniserTest.cpp])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ltcl':
AC_CHECK_LIB([tcl], [main])
# FIXME: Replace `main' with a function in `-ltk':
AC_CHECK_LIB([tk], [main])

# Checks for libraries.
# AC_PROG_LIBTOOL

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([fcntl.h limits.h stddef.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strcasecmp strncasecmp strtol])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
