#!/bin/sh

ACLOCAL_FLAGS=""
AUTOCONF_FLAGS=""
AUTOMAKE_FLAGS="-a"

if [ -f configure.ac ]; then
  if grep "A[MC]_CONFIG_HEADER" configure.ac >/dev/null; then
    if [ ! -f src/config.h.in ]; then
      echo "Running autoheader"
      autoheader || exit 1
    fi
  fi
  if grep "AM_PROG_LIBTOOL" configure.ac >/dev/null; then
    echo "Running libtoolize"
    libtoolize || exit 1
  fi
  echo "Running aclocal $ACLOCAL_FLAGS"
  aclocal $ACLOCAL_FLAGS || exit 1
  
  echo "Running autoconf $AUTOCONF_FLAGS"
  autoconf $AUTOCONF_FLAGS || exit 1
  
  echo "Running automake $AUTOMAKE_FLAGS"
  automake $AUTOMAKE_FLAGS || exit 1
fi

echo "Now type \`./configure' to configure and then \`make' to compile."
