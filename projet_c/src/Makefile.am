bin_PROGRAMS = quad

AM_CFLAGS = -DHAVE_QUAD $(GTK_CFLAGS)
quad_LDFLAGS = $(GTK_LIBS)

quad_SOURCES = \
	file_bit.c file_bit.h\
	image_common.h\
	load_ppm.c load_ppm.h\
	load_qtc.c load_qtc.h\
	main.c main.h\
	picture.c picture.h\
	quadcomp.c quadcomp.h\
	quadpic.c quadpic.h\
	quadtree.c quadtree.h\
	save_ppm.c save_ppm.h\
	save_qtc.c save_qtc.h\
	util.c util.h\
	ui_main.c ui_main.h\
	ui_util.c ui_util.h

#unquad_CFLAGS = -DHAVE_UNQUAD $(GTK_CFLAGS)
#unquad_LDFLAGS = $(GTK_LIBS)

#unquad_SOURCES = \
#	load_ppm.c load_ppm.h\
#	main.c main.h\
#	picture.c picture.h

mostlyclean-generic:	
	-rm -f *~ \#* .*~ .\#*
