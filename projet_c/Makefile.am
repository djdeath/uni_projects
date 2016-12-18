SUBDIRS = doc src

giftuidocdir = $(prefix)/doc/giftui
giftuidoc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	NEWS

EXTRA_DIST = $(giftuidoc_DATA)\
	INSTALL\
	autogen.sh

mostlyclean-generic:	
	-rm -f *~ \#* .*~ .\#*
