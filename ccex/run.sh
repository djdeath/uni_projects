#!/bin/sh

CC=gcc
CFLAGS="-g -Wall"
CCEX=ccex/ccex
CCEX3A=ccex3a/ccex3a

TMP3A=/tmp/3a.tmp

for i in $@ ; do
    
    $CCEX $i > $TMP3A
    if test $? -ne 0 ; then
	rm $TMP3A
	exit $?
    fi
    asmfile=`basename $i`.s
    
    $CCEX3A $TMP3A > $asmfile
    if test $? -ne 0 ; then
	rm -f $TMP3A $asmfile
	exit $?
    fi
    
    rm -f $TMP3A
    $CC $CFLAGS $asmfile lib/*.o -o cex.out
    rm -f $asmfile
done

exit 0
