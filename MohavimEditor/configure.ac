# Configuration for Mohavim - a modernized text editor
#
# Copyright (C) 2025 Gabriel A. Matos
#
# Mohavim is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.

AC_INIT([Mohavim], [7.2], [gabriellramone@gmail.com], [mohavim])
AC_CONFIG_SRCDIR([mohavim.c])
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([1.14])
AM_MAINTAINER_MODE([enable])
AC_CONFIG_HEADERS([config.h])

AC_PREREQ([2.69])

dnl Checks for programs.
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_LN_S
AC_SEARCH_LIBS([strerror], [cposix])

dnl Checks for header files.
AC_CHECK_HEADERS(limits.h termios.h sys/param.h dirent.h)

dnl Checks for functions.
AC_CHECK_FUNCS_ONCE(chmod chown fchmod fchown)

dnl Checks for available flags.
AX_CHECK_COMPILE_FLAG([-Wall], [CFLAGS="$CFLAGS -Wall"], [], [])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
