
bin_PROGRAMS = sv_mptc cl_mptc

AM_CFLAGS = $(GTK_CFLAGS)

sv_mptc_LDFLAGS = -lm

sv_mptc_SOURCES = \
	config.c config.h\
	data.h\
	event.c event.h\
	hash_close.c hash_close.h\
	hash_func.c hash_func.h\
	hash_open.c hash_open.h\
	hash_table.c hash_table.h\
	list.c list.h\
	sv_client.c sv_client.h\
	sv_console.c sv_console.h\
	sv_dict.c sv_dict.h\
	sv_game.c sv_game.h\
	sv_main.c sv_main.h\
	sv_server.c sv_server.h\
	sv_team.c sv_team.h\
	sv_word.c sv_word.h\
	network.c network.h\
	util.c util.h

cl_mptc_LDFLAGS = $(GTK_LIBS)

cl_mptc_SOURCES = \
	config.c config.h\
	cl_main.c cl_main.h\
	cl_parse.c cl_parse.h\
	cl_ui_chat.c cl_ui_chat.h\
	cl_ui_conn.c cl_ui_conn.h\
	cl_ui_list.c cl_ui_list.h\
	cl_ui_main.c cl_ui_main.h\
	cl_ui_menu.c cl_ui_menu.h\
	cl_ui_play.c cl_ui_play.h\
	cl_ui_setu.c cl_ui_setu.h\
	cl_ui_util.c cl_ui_util.h\
	cl_util.c cl_util.h\
	cl_word.c cl_word.h\
	data.h\
	hash_close.c hash_close.h\
	hash_func.c hash_func.h\
	hash_open.c hash_open.h\
	hash_table.c hash_table.h\
	list.c list.h\
	network.c network.h\
	util.c util.h

mostlyclean-generic:
	-rm -f *~ \#* .*~ .\#*
