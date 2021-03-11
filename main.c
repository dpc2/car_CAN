//
//

#include <can_history.h>
#include "includes.h"
#include "can_real_time.h"

extern char * g_db_path;
extern sqlite3 * sqlite3_db_statis;

GtkWidget * main_window;
GtkWidget * real_time_window;
GtkWidget * history_select_db_window;
GtkWidget * history_display_window;

int screen_width;
int screen_height;

extern can_frame_t can_frame_ar[];
extern int can_frame_ar_length;
// startx /usr/bin/xclock -- /usr/bin/X :0

int main(int argc, char *argv[])
{
//	gdk_pixbuf_get_colorspace();
//	can_id_conf_ar_length = sizeof(can_id_conf_ar) / sizeof(can_id_conf_t);
//	printf("can_id_conf_ar_length : %d \n", can_id_conf_ar_length);

	int i, k;
	can_frame_t *pf;
	can_node_t *pn;
	for (i = 0; i < can_frame_ar_length; i++)
	{
		pf = can_frame_ar + i;
//		pfr = can_frame_real_time_ar + i;
		for (k = 0; k < pf->total_node; k++)
		{
//			pn = pf->can_node_ar + k;
			pn = pf->can_node_ar + k;
			pn->node_name = pn->node_name_cn;
		}
	}

	if (open_db(&sqlite3_db_statis, g_db_path, "db_statis") < 0)
	{
		exit(1);
	}

	char * errmsg;
	char str[500];
	/* db_time is useless */
	sprintf(str, "create table if not exists  db_statis("
			"row							integer			primary key	autoincrement,"
			"db_name						varchar(100)	default 0,"
			"start_second  			integer  		default 0,"
			"start_millisecond  		integer  		default 0,"
			"stop_second  				integer  		default 0,"
			"stop_millisecond  		integer 			default 0)");

	if (SQLITE_OK != sqlite3_exec(sqlite3_db_statis, str, NULL, NULL, &errmsg))
	{
		printf("error: %s. errmsg: %s \n", str, errmsg);
		sqlite3_free(errmsg);
		return -1;
	}

	gtk_init(&argc, &argv);
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(main_window), "can");
	gtk_widget_set_size_request(main_window, 600, 400);
//	gtk_window_maximize(window);	// title will still exist
//	gtk_window_fullscreen(GTK_WINDOW(main_window));	// title will not exist
	gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
//	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);    // not a good choice
//	gtk_window_set_decorated(window, FALSE);  // hide the window border
	g_signal_connect(main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	//	gdk_display_beep();
	GdkScreen *gdk_screen = gdk_screen_get_default();
	screen_width = gdk_screen_get_width(gdk_screen);
	screen_height = gdk_screen_get_height(gdk_screen);
	//  free gdk_screen .............................................................
	printf("screen_width = %d\n", screen_width);
	printf("screen_height= %d\n", screen_height);

	int padding = 10;
	GtkWidget * main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);// GTK_ORIENTATION_HORIZONTAL     GTK_ORIENTATION_VERTICAL
	gtk_container_add(GTK_CONTAINER(main_window), main_hbox);

	/*************************************************** vbox1 *******************************************************/

	GtkWidget * vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);	// GTK_ORIENTATION_HORIZONTAL
	gtk_box_pack_start(GTK_BOX(main_hbox), vbox1, TRUE, TRUE, padding);

	GtkWidget * button_real_time = gtk_button_new_with_label("实时");
	gtk_box_pack_end(GTK_BOX(vbox1), button_real_time, FALSE, TRUE, padding);
	g_signal_connect(button_real_time, "clicked", G_CALLBACK (dialog_of_real_time_select_pf), main_window);

	/*************************************************** vbox2 *******************************************************/

	GtkWidget * vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);
	gtk_box_pack_start(GTK_BOX(main_hbox), vbox2, TRUE, TRUE, padding);

	GtkWidget * button_of_history_select_db = gtk_button_new_with_label("回看");
	gtk_box_pack_end(GTK_BOX(vbox2), button_of_history_select_db, FALSE, TRUE, padding);
	g_signal_connect(button_of_history_select_db, "clicked", G_CALLBACK(history_select_db_handle), NULL);

//	GtkWidget * button_history = gtk_button_new_with_label("history");
////	gtk_widget_set_sensitive(button_history, FALSE);
//	gtk_box_pack_end(GTK_BOX(vbox2), button_history, FALSE, TRUE, 10);
//	g_signal_connect(button_history, "clicked", G_CALLBACK (history_main), window);

//	do_sizegroup(window);
//	display(window);
//	dialog(main_window);
//	do_infobar();
//	do_event_axes(window);
//	cairo_drawing_area(window);
//	do_button_box (window);
//	paned_main(argc, argv);
//	cairo_move(argc, argv);
//	cairo_svg(argc, argv);
//	cairo_pdf(argc, argv);
//	cairo_image(argc, argv);

	gtk_widget_show_all(main_window);
	gtk_main();
	return 0;
//	GdkEventMask x;
//	GdkEventButton y;
}

void main_window_destroy(GtkWidget * widget, void ** data)
{
	printf("main_window_destroy.\n");

	gtk_widget_destroyed(GTK_WIDGET(widget), (GtkWidget**) data);// also destroy all child_widget; set *pointer = NULL;
//	gtk_widget_set_sensitive(GTK_WIDGET(history_select_db_window), TRUE);

	printf("main_window_destroy returned. \n");
}

