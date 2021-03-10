


all:
	gcc -o can	\
	can_gw.c	\
	can_history_display.c	\
	can_history_select_db.c	\
	can_real_time.c	\
	can_real_time_select_pf.c	\
	can_standard.c	\
	db.c	\
	main.c	\
	gw_conf/can_real_time_conf.c	\
	gw_conf/can_standard_conf.c	\
	sqlite3/shell.c	\
	sqlite3/sqlite3.c	\
	-I. -Igw_conf -Isqlite3	`pkg-config --cflags --libs gtk+-3.0` -ldl
	
	
	
	
	
a:
	-I/usr/include/gtk-3.0	\
	-I/usr/include/glib-2.0	\
	-I/usr/lib/x86_64-linux-gnu/glib-2.0/include	\
	-I/usr/include/pango-1.0	\
	-I/usr/include/harfbuzz	\
	-I/usr/include/cairo	\
	-I/usr/include/gdk-pixbuf-2.0	\
	-I/usr/include/atk-1.0	\
	-L/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu/glib-2.0	\
	-lgtk-3	-lglib-2.0 -lpango-1.0 -l harfbuzz -l cairo -lgdk_pixbuf-2.0 -latk-1.0 -lgdk-3 -lgio-2.0
