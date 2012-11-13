AC_PREREQ([2.67])
AC_INIT([Soma], [1.0], [mbrush@codebrainz.ca])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall silent-rules])
AM_SILENT_RULES([yes])
LT_INIT
AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_VALAC([0.15.1])
PKG_CHECK_MODULES([GTK3], [gtk+-3.0])
AC_CONFIG_FILES([
  Makefile
  soma/Makefile
  src/Makefile
])
AC_OUTPUT
