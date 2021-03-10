///*
// * can_real_time_select_pf.c
// *
// *  Created on: 2017年10月2日
// *      Author: root
// */
//
//
//#include <can_history.h>
//#include <can_real_time.h>
//#include "includes.h"
//#include "db.h"
//cc
//extern can_frame_t can_frame_ar[];
//extern int can_frame_ar_length;
//extern can_frame_real_time_t can_frame_real_time_ar[];
//extern int can_frame_real_time_ar_length;
//
//extern GtkWidget * main_window;
//extern GtkWidget * real_time_window;
//extern GtkWidget * real_time_select_pf_window;
//
//
//GtkWidget * real_time_select_pf(void)
//{
//	int padding = 10;
//	static int a = 0;
//
//	if (real_time_select_pf_window == NULL)
//	{
//		printf("\n*** history_select_db(): %d \n", a++);
//
//		gtk_widget_set_sensitive(GTK_WIDGET(main_window), FALSE);
//
//		real_time_select_pf_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//		gtk_window_set_default_size(GTK_WINDOW(real_time_select_pf_window), 400, 400);
//		gtk_window_set_title(GTK_WINDOW(real_time_select_pf_window), "选择数据库");
//		g_signal_connect(real_time_select_pf_window, "destroy", G_CALLBACK(real_time_select_pf_window_destroy),
//				&real_time_select_pf_window);
//
//		GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);
//		gtk_container_add(GTK_CONTAINER(real_time_select_pf_window), vbox);
//
//		GtkWidget * db_name_list_combo_box = gtk_combo_box_text_new_with_entry();
//		gtk_box_pack_start(GTK_BOX(vbox), db_name_list_combo_box, FALSE, TRUE, 10);
////		gchar * gtk_combo_box_text_get_active_text(GtkComboBoxText *combo_box);
//		if (add_db_name_for_combo_box(db_name_list_combo_box) < 0)
//		{
//			exit(1);
//		}
//
//		GtkWidget * ok_button = gtk_button_new_with_label("OK");
//		gtk_box_pack_start(GTK_BOX(vbox), ok_button, FALSE, TRUE, 10);
//		g_signal_connect(ok_button, "clicked", G_CALLBACK (ok_button_clicked_call_back),
//				(void* )real_time_select_pf_window);
//
////		gtk_widget_add_events(box,
////				GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_SMOOTH_SCROLL_MASK
////						| GDK_TOUCH_MASK);
////		g_signal_connect(box, "event", G_CALLBACK(event_cb), event_data);
//
//	}
//
//	if (FALSE == gtk_widget_get_visible(real_time_select_pf_window))
//	{
//		gtk_widget_show_all(real_time_select_pf_window);
//	}
//	else
//	{
////		gtk_widget_destroy(real_time_select_pf_window);
////		real_time_select_pf_window = NULL;
//	}
//
//	return real_time_select_pf_window;
//}
//
//
//
//
//
//void real_time_select_pf_window_destroy(GtkWidget * widget, void ** data)
//{
//	printf("\nreal_time_select_pf_window_destroy: widget == %p, *data == %p \n", widget, *data);
//
//	if (history_display_window != NULL)
//	{
//		if (1)
//		{
//			// work
//			gtk_widget_destroy(history_display_window);	// ......... gtk_widget_destroy(widget) == send destroy signal to widget ???
//			history_display_window = NULL;
//		}
//		else
//		{
//			// not work
//			history_display_window_destroy(history_display_window, (void**) &history_display_window);
//		}
//	}
//
//	gtk_widget_destroyed(widget, (GtkWidget**) data);	// also destroy all child_widget; set *pointer = NULL;
//	gtk_widget_set_sensitive(GTK_WIDGET(main_window), TRUE);
//	printf("real_time_select_pf_window_destroy returned. \n");
//}
