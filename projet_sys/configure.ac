dnl ###########################################################################
dnl Initialize autoconf...
dnl ###########################################################################

AC_PREREQ(2.52)

AC_INIT(mptc, 0.0.1)
AC_CONFIG_HEADER(src/mptc_config.h)
AC_CONFIG_SRCDIR([src/sv_main.h])

dnl ###########################################################################
dnl Initialize automake...
dnl ###########################################################################

AM_INIT_AUTOMAKE
#AM_CONFIG_HEADER(src/config.h)
#AM_MAINTAINER_MODE

dnl ###########################################################################
dnl Check for the necessary build tools.
dnl ###########################################################################

AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
#AC_CANONICAL_BUILD

dnl ###########################################################################
dnl Checks for header files.
dnl ###########################################################################

AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h ctype.h errno.h fcntl.h limits.h locale.h\
		  netdb.h netinet/in.h stdarg.h sys/poll.h ss/select.h\
	 	  sys/socket.h sys/time.h sys/types.h time.h winsock.h\
		  winsock2.h])

dnl ###########################################################################
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ###########################################################################

AC_C_CONST
AC_TYPE_SIZE_T

dnl ###########################################################################
dnl  Checks for library functions.
dnl ###########################################################################

AC_CHECK_FUNCS([fcntl getopt io.h memcpy memcmp poll signal strcpy strstr])


dnl ###########################################################################
dnl  Checks for types.
dnl ###########################################################################

AC_CHECK_TYPE(in_port_t)

dnl ###########################################################################
dnl Checks for GTK+.
dnl ###########################################################################

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.2.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

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
dnl GCC debug options.
dnl ###########################################################################

CFLAGS="-Wall"
if test x"$debug" = xyes; then
   AC_DEFINE(HAVE_DEBUG, 1, HAVE_DEBUG)
   CFLAGS="-g $CFLAGS"
fi

dnl ###########################################################################
dnl Finish generation of the build environment
dnl ###########################################################################

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
