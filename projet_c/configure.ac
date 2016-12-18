dnl ###########################################################################
dnl Initialize autoconf...
dnl ###########################################################################

AC_PREREQ(2.52)

AC_INIT(imgcompress, 0.0.1)
AC_CONFIG_SRCDIR([src/main.h])

dnl ###########################################################################
dnl Initialize automake...
dnl ###########################################################################

AM_INIT_AUTOMAKE([dist-bzip2])
AM_CONFIG_HEADER(src/config.h)
#AM_MAINTAINER_MODE

dnl ###########################################################################
dnl Check for the necessary build tools.
dnl ###########################################################################

AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl ###########################################################################
dnl Check Big/Little ENDIAN Processors
dnl ###########################################################################

AC_C_BIGENDIAN(AC_DEFINE([ENDIAN_BIG],[1],[Define if the machine is Big-Endian]),\
	       AC_DEFINE([ENDIAN_LITTLE],[1],[Define if the machine is Little-Endian]))

dnl ###########################################################################
dnl Checks for header files.
dnl ###########################################################################

AC_HEADER_STDC
#AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([alloca.h argz.h errno.h fcntl.h getopt.h libintl.h limits.h\
		 locale.h malloc.h math.h nl_types.h pwd.h stdarg.h stddef.h\
		 sys/param.h sys/stat.h sys/types.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
#AC_C_INLINE
AC_TYPE_SIZE_T

dnl ###########################################################################
dnl  Checks for library functions.
dnl ###########################################################################

AC_FUNC_ALLOCA
AC_FUNC_MMAP
AC_STRUCT_ST_BLOCKS
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify getcwd memcpy\
	       strcasecmp strchr strdup strcpy strncpy strstr])

dnl ###########################################################################
dnl Checks for GTK+.
dnl ###########################################################################

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.2.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

# NLS
#ALL_LINGUAS="fr"
#AM_GNU_GETTEXT

dnl ###########################################################################
dnl Handle the configure switches...
dnl ###########################################################################

AC_ARG_ENABLE(debug, [  --enable-debug          Compile with debug support (optional, using -g option with gcc)],
		      debug=$enableval, debug=no)

if test x$prefix = xNONE; then
    prefix="/usr/local"
    AC_SUBST(prefix)
fi

dnl ###########################################################################
dnl GCC debug options
dnl ###########################################################################

CFLAGS="-Wall -ansi"
if test x"$debug" = xyes; then
   AC_DEFINE([HAVE_DEBUG], [1], [Compile program with debug support])
   CFLAGS="-g $CFLAGS"
fi

dnl ###########################################################################
dnl Finish generation of the build environment
dnl ###########################################################################

AC_CONFIG_FILES([
Makefile
doc/Makefile
src/Makefile
])

AC_OUTPUT

