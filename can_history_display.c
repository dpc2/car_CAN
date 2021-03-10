/*
 * history.c
 *
 *  Created on: 2017年9月5日
 *      Author: root
 */

#include "can_history.h"
#include "can_real_time.h"
#include "can_standard.h"
#include "includes.h"

//extern sqlite3 * sqlite3_db_statis;
//extern char * db_path;

extern GtkWidget * real_time_window;
extern GtkWidget * history_display_window;
extern GtkWidget * history_select_db_window;

extern int screen_width;
extern int screen_height;

GtkWidget * vbox3_table_list;

history_db_t history_db;

GtkWidget * hbox2_draw_area;
GtkWidget * ruler_area;
GtkWidget * draw_area;
GtkWidget * spin_button_of_pix_conf;
GtkWidget * spin_button_of_x_coordinate;

void history_display_window_destroy(GtkWidget * widget, void ** data)
{
	printf("\nhistory_display_window_destroy: widget == %p, *data == %p, history_display_window == %p \n", widget,
			*data, history_display_window);

	free_all_sources();
	gtk_widget_destroyed(GTK_WIDGET(widget), (GtkWidget**) data); // also destroy all child_widget; set *pointer = NULL;
	gtk_widget_set_sensitive(GTK_WIDGET(history_select_db_window), TRUE);

	printf("history_display_window_destroy returned. \n");
}

//void history_display_window_expose_event_call_back(GtkWidget*widget, void * pointer)
//{
//	g_signal_emit_by_name(G_OBJECT(pointer), "draw");
//}

int history_display_handle(void)
{
	printf("history_display 1 \n");
//	dynamic loading of modules 动态加载模块
	GdkEventExpose *event;
//	GdkPixmap * pixmap = gdk_pixmap_new();
//	cairo_t * cairo_create(cairo_surface_t *target);
//	cairo_surface_t * gdk_window_create_similar_surface(GdkWindow *window, cairo_content_t content, int width, int height);
// CAIRO_CONTENT_COLOR_ALPHA
//	if(0)
//	{
//		cairo_surface_t * surface = gdk_window_create_similar_surface(GDK_WINDOW(do_widget), CAIRO_CONTENT_COLOR_ALPHA,
//				100, 100);
//		cairo_t * cr = cairo_create(surface);
//	}
//		gtk_widget_queue_draw (do_widget);
//		gtk_widget_queue_draw_area();
//		pthread_mutex_init(&history_db.history_draw_cache_mutex, PTHREAD_MUTEX_TIMED_NP);
//	int window_width = gtk_widget_get_allocated_width(window);  // will be 1 before display all finished
//	int window_height = gtk_widget_get_allocated_height(window);

	if (real_time_window != NULL)	// if real_time_window is running
	{
		return 0;
	}

	gtk_widget_set_sensitive(GTK_WIDGET(history_select_db_window), FALSE);
	int padding = 0;
	double draw_area_height = 0.7;

	if (history_display_window == NULL)
	{

		history_display_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//		gtk_window_set_screen(GTK_WINDOW(history_display_window), gtk_widget_get_screen(do_widget));
		gtk_window_set_title(GTK_WINDOW(history_display_window), "回看");
		gtk_container_set_border_width(GTK_CONTAINER(history_display_window), 10);
//		gtk_widget_set_size_request(window, 600, 600);
		gtk_window_maximize(history_display_window);	// title will still exist
//		gtk_window_fullscreen(GTK_WINDOW(window));	// title will not exist
//		gtk_window_set_resizable (GTK_WINDOW(window), FALSE);  // not a good choice
//		g_signal_connect(window, "destroy", G_CALLBACK (gtk_widget_destroy), window);
		g_signal_connect(history_display_window, "destroy", G_CALLBACK (history_display_window_destroy),
				(void** )&history_display_window);

		GtkWidget * main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
		gtk_container_add(GTK_CONTAINER(history_display_window), main_hbox);

		/*********************************************************************/

		GtkWidget * vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);	// GTK_ORIENTATION_HORIZONTAL
		gtk_box_pack_start(GTK_BOX(main_hbox), vbox1, TRUE, TRUE, padding);

		hbox2_draw_area = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
		gtk_box_pack_start(GTK_BOX(vbox1), hbox2_draw_area, TRUE, TRUE, padding);

		/********************************** ruler_area ************************************/

		GtkWidget * scrolled_ruler_area = gtk_scrolled_window_new(NULL, NULL);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_ruler_area), GTK_POLICY_ALWAYS,
				GTK_POLICY_AUTOMATIC); //GTK_POLICY_ALWAYS
		gtk_widget_set_size_request(scrolled_ruler_area, (int) (screen_width * 0.1),
				(int) (screen_height * draw_area_height));
		//		gtk_scrolled_window_set_placement(GTK_SCROLLED_WINDOW(scrolled_draw_area), GTK_CORNER_BOTTOM_LEFT);	// GTK_CORNER_TOP_RIGHT
		gtk_scrolled_window_set_kinetic_scrolling(GTK_SCROLLED_WINDOW(scrolled_ruler_area), TRUE); // move by touch_screen
		gtk_box_pack_start(GTK_BOX(hbox2_draw_area), scrolled_ruler_area, TRUE, TRUE, padding);

		ruler_area = gtk_drawing_area_new();
		gtk_widget_set_size_request(ruler_area, (int) (screen_width * 0.1), (int) (screen_height * 0.7));
		//		gtk_container_add(GTK_CONTAINER(frame), draw_area);
		gtk_container_add(GTK_CONTAINER(scrolled_ruler_area), ruler_area);
		g_signal_connect(ruler_area, "draw", G_CALLBACK(history_ruler_area_call_back), ruler_area); // "expose_event"

		/********************************** draw_area ************************************/

		GtkWidget * scrolled_draw_area = gtk_scrolled_window_new(NULL, NULL);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_draw_area), GTK_POLICY_ALWAYS,
				GTK_POLICY_AUTOMATIC); //GTK_POLICY_ALWAYS
		gtk_widget_set_size_request(scrolled_draw_area, (int) (screen_width * 0.7),
				(int) (screen_height * draw_area_height));
		//		gtk_scrolled_window_set_placement(GTK_SCROLLED_WINDOW(scrolled_draw_area), GTK_CORNER_BOTTOM_LEFT);	// GTK_CORNER_TOP_RIGHT
		gtk_scrolled_window_set_kinetic_scrolling(GTK_SCROLLED_WINDOW(scrolled_draw_area), TRUE); // move by touch_screen
		gtk_box_pack_start(GTK_BOX(hbox2_draw_area), scrolled_draw_area, TRUE, TRUE, padding);

		//		GtkWidget *frame = gtk_frame_new("draw");
		//		gtk_container_add(GTK_CONTAINER(scrolled_draw_area), frame);

		draw_area = gtk_drawing_area_new();
		gtk_widget_set_size_request(draw_area, (int) (history_db.history_draw_area_request_width),
				(int) (screen_height * 0.7));
//		gtk_container_add(GTK_CONTAINER(frame), draw_area);
		gtk_container_add(GTK_CONTAINER(scrolled_draw_area), draw_area);
//		g_signal_connect(draw_area, "draw", G_CALLBACK(history_on_draw_event_call_back), NULL);

		g_signal_connect(draw_area, "draw", G_CALLBACK(history_draw_area_on_draw_event_call_back), (void* )draw_area);// "expose_event"
//		g_signal_connect(draw_area, "expose_event", G_CALLBACK(history_draw_area_on_draw_event_call_back), NULL);// "expose_event"	// ........
//		g_signal_connect(history_display_window, "expose_event", G_CALLBACK (history_draw_area_on_draw_event_call_back), (void*)draw_area);	// ...........
//
//
//	      g_signal_connect (da, "draw",
//	                        G_CALLBACK (scribble_draw), NULL);
//	      g_signal_connect (da,"configure-event",
//	                        G_CALLBACK (scribble_configure_event), NULL);
//
//	      /* Event signals */
//
//	      g_signal_connect (da, "motion-notify-event",
//	                        G_CALLBACK (scribble_motion_notify_event), NULL);
//	      g_signal_connect (da, "button-press-event",
//	                        G_CALLBACK (scribble_button_press_event), NULL);
//
//
//	      /* Ask to receive events the drawing area doesn't normally
//	       * subscribe to
//	       */
//	      gtk_widget_set_events (da, gtk_widget_get_events (da)
//	                             | GDK_LEAVE_NOTIFY_MASK
//	                             | GDK_BUTTON_PRESS_MASK
//	                             | GDK_POINTER_MOTION_MASK
//	                             | GDK_POINTER_MOTION_HINT_MASK);
//
//		GdkWindow * gdkwindow = gtk_widget_get_window(GTK_WIDGET(draw_area));
//		GdkRGBA rgba = {0, 0, 0.3, 1};
//		gdk_window_set_background_rgba (gdkwindow, &rgba);

//		gtk_widget_override_background_color (GTK_WIDGET(draw_area), GTK_STATE_FLAG_NORMAL, &rgba);

//		GDK_LEAVE_NOTIFY_MASK;
//		GDK_BUTTON_PRESS_MASK;
//		GDK_POINTER_MOTION_MASK;
//		GDK_POINTER_MOTION_HINT_MASK;

//		GdkEventButton *a;
//		GdkEventMotion *b;
//
//		GdkWindow * gdkwindow = gtk_widget_get_window(GTK_WIDGET(draw_area));
//		gdk_window_set_event_compression(gdkwindow, FALSE);

		gtk_widget_add_events(draw_area, GDK_BUTTON_PRESS_MASK);
		gtk_widget_add_events(draw_area, GDK_BUTTON_MOTION_MASK);
		gtk_widget_add_events(draw_area, GDK_POINTER_MOTION_MASK);
		gtk_widget_add_events(draw_area, GDK_TOUCH_MASK);

		g_signal_connect(draw_area, "button-press-event", G_CALLBACK(mouse_button_press_call_back), NULL);
		g_signal_connect(draw_area, "pointer-motion-event", G_CALLBACK(mouse_pointer_motion_call_back), NULL);

		/*******************************************************************************************************/
		/*******************************************************************************************************/
		/********************** spin_button_of_pix_conf ********************************************************/

		GtkWidget * hbox3_of_spin_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
		gtk_box_pack_start(GTK_BOX(vbox1), hbox3_of_spin_button, TRUE, TRUE, padding);

		GtkWidget * label_of_pix_conf = gtk_label_new("像素/秒");
		gtk_box_pack_start(GTK_BOX(hbox3_of_spin_button), label_of_pix_conf, TRUE, TRUE, padding);

//		GtkAdjustment * adjustment_of_pix_conf = gtk_adjustment_new(100, 10, 2000, 20, 1000, 0);
//		history_db.spin_button_of_pix_conf = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment_of_pix_conf), 0.005, 0);
		spin_button_of_pix_conf = gtk_spin_button_new_with_range(MIN_PIX_COUNT_OF_ONE_S, MAX_PIX_COUNT_OF_ONE_S, 10);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button_of_pix_conf), 100);
		//		gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spin_button), 1, 0 );
		gtk_box_pack_start(GTK_BOX(hbox3_of_spin_button), spin_button_of_pix_conf, TRUE, TRUE, padding);

//		g_object_bind_property(history_db.spin_button_of_pix_conf, "value_changed",
//				draw_area, "draw", G_BINDING_BIDIRECTIONAL);

		g_signal_connect(spin_button_of_pix_conf, "value_changed", G_CALLBACK(spin_button_of_pix_conf_value_changed),
				NULL);	// ..........................................

		/********************** spin_button_of_x_coordinate ************************/

		GtkWidget * label_of_x_coordinate = gtk_label_new("位置");
		gtk_box_pack_start(GTK_BOX(hbox3_of_spin_button), label_of_x_coordinate, TRUE, TRUE, padding);

		GtkAdjustment * adjustment_of_draw_area = gtk_scrolled_window_get_hadjustment(
				GTK_SCROLLED_WINDOW(scrolled_draw_area));
		spin_button_of_x_coordinate = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment_of_draw_area), 0.002, 0);
//		gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spin_button), 1, 0 );
		gtk_box_pack_start(GTK_BOX(hbox3_of_spin_button), spin_button_of_x_coordinate, TRUE, TRUE, padding);
		g_signal_connect(spin_button_of_x_coordinate, "value_changed",
				G_CALLBACK(spin_button_of_x_coordinate_value_changed), NULL);

//		gtk_spin_button_set_increments (GtkSpinButton *spin_button,	gdouble step, gdouble page);
//		GtkWidget * hscrollbar = gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
//		gtk_box_pack_start(GTK_BOX(vbox1), hscrollbar, TRUE, TRUE, padding);
//		GtkAdjustment*   gtk_adjustment_new             (gdouble          value,
//		                                                 gdouble          lower,
//		                                                 gdouble          upper,
//		                                                 gdouble          step_increment,
//		                                                 gdouble          page_increment,
//		                                                 gdouble          page_size);
//		GtkWidget *gtk_spin_button_new (GtkAdjustment *adjustment, gdouble climb_rate, guint digits);
//		gtk_widget_send_expose();
//		gtk_widget_queue_draw	  (GtkWidget	       *widget);
//		gtk_widget_queue_draw_area();
//		gtk_widget_set_app_paintable();
//		gdk_cairo_create();
//
////		If you want to force an area of a window to be redrawn, use gdk_window_invalidate_rect() or
////		 gdk_window_invalidate_region(). To cause the redraw to be done immediately, follow that call with a call to
////		 gdk_window_process_updates().
//		gdk_window_invalidate_rect();
//		gdk_window_invalidate_region();
//		 gdk_window_process_updates();

		/**********************************************************************************************************/
		/**********************************************************************************************************/
		/*************************************** table_list *******************************************************/

		GtkWidget * vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);	// GTK_ORIENTATION_HORIZONTAL
		gtk_box_pack_start(GTK_BOX(main_hbox), vbox2, TRUE, TRUE, padding);

		GtkWidget * refresh_button = gtk_button_new_with_label("refresh");
		gtk_box_pack_start(GTK_BOX(vbox2), refresh_button, TRUE, TRUE, padding);
		g_signal_connect(refresh_button, "clicked", G_CALLBACK(refresh_button_clicked), (void* )draw_area);

		GtkWidget *scrolled_table_list = gtk_scrolled_window_new(NULL, NULL);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_table_list), GTK_POLICY_AUTOMATIC,
				GTK_POLICY_AUTOMATIC);
		gtk_widget_set_size_request(scrolled_table_list, (int) (screen_width * 0.1), (int) (screen_height * 0.5));
		gtk_box_pack_start(GTK_BOX(vbox2), scrolled_table_list, TRUE, TRUE, padding);

		vbox3_table_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);	// GTK_ORIENTATION_HORIZONTAL
		gtk_container_add(GTK_CONTAINER(scrolled_table_list), vbox3_table_list);

//		GtkWidget * table_list_check_button_ar[history_db.history_total_frame];

		history_node_t * p;
		int i, j, k;
		for (i = 0; i < history_db.history_total_frame; i++)
		{
			for (j = 0; j < history_db.history_frame_ar[i].history_total_node; j++)
			{
				if (history_db.history_frame_ar[i].history_node_ar[j].display_type != SHOW_AS_LINE)
				{
					continue;
				}
				p = history_db.history_frame_ar[i].history_node_ar + j;

				p->check_button = gtk_check_button_new_with_label(p->node_name);

//				gtk_widget_set_sensitive(p->check_button,	TRUE && history_db.history_frame_ar[i].history_node_data_ar_length);
//				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(table_list_check_button_ar[i]),
//				TRUE && history_db.history_frame_ar[i].history_node_data_ar_length);

				g_signal_connect(G_OBJECT (p->check_button), "toggled",
						G_CALLBACK (history_check_button_toggled_call_back), (void* )p);

				gtk_box_pack_start(GTK_BOX(vbox3_table_list), p->check_button, FALSE, TRUE, padding);
			}
		}

//		flowbox_1 = gtk_flow_box_new();
//		gtk_widget_set_valign(flowbox_1, GTK_ALIGN_START);
//		gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox_1), 10);
//		gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox_1), GTK_SELECTION_NONE);
//		gtk_container_add(GTK_CONTAINER(scrolled_draw_area), flowbox_1);
//
//		GtkWidget * draw_hbox3 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
//		gtk_box_pack_start(GTK_BOX(main_vbox), draw_hbox3, TRUE, TRUE, padding);
//
//		GtkWidget * draw_vbox = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
//		gtk_box_pack_start(GTK_BOX(draw_hbox3), draw_vbox, TRUE, TRUE, padding);
//
//		/******************************************* scrolled_2, flowbox_2 *******************************************/
//
//		GtkWidget *scrolled_2 = gtk_scrolled_window_new(NULL, NULL);
//		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//		gtk_widget_set_size_request(scrolled_2, (int) (screen_width * 0.1), (int) (screen_height * 0.5));
//		gtk_container_add(GTK_CONTAINER(draw_hbox3), scrolled_2);
//
//		flowbox_2 = gtk_flow_box_new();
//		gtk_widget_set_valign(flowbox_2, GTK_ALIGN_START);
//		gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox_2), 1);
//		gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox_2), GTK_SELECTION_NONE);
//		gtk_container_add(GTK_CONTAINER(scrolled_2), flowbox_2);
//		/*************************************************************************************************************/
//
//		if (init_message_ar() < 0)
//		{
//			exit(1);
//		}
//		char string[20];
//		sprintf(string, "%d", screen_width);
//		gtk_entry_set_text(GTK_ENTRY(message_ar[0].entry), string);
//		sprintf(string, "%d", screen_height);
//		gtk_entry_set_text(GTK_ENTRY(message_ar[1].entry), string);
//		pthread_t send, receive;
//		pthread_create(&receive, NULL, (void*) can_read, NULL);
//		pthread_create(&send, NULL, can_write, NULL);
	}

	/**************************************************************************************************************************/

	history_db.history_draw_area_sigle_screen_width = -1;	// == width of hbox2_draw_area
	history_db.history_draw_area_allocated_width = -1;	// width
	history_db.history_draw_area_height = -1;
	//double history_human_max_x_coordinate;
	history_db.history_human_max_y_coordinate = -1;

	if (gtk_widget_get_visible(history_display_window) == FALSE)
	{
//		gtk_window_fullscreen(GTK_WINDOW(window));	// title will not exist
		gtk_widget_show_all(history_display_window);
	}
	else
	{
//		gtk_widget_hide(history_display_window);
		gtk_widget_show_all(history_display_window);
//		gtk_widget_destroy(window);
//		window = NULL;
	}

//	g_source_remove(source_id);
//	g_io_channel_shutdown(io_channel, TRUE, NULL);	// 关闭 iochannel 操作会把文件描述符关闭
//	g_io_channel_unref(io_channel);

//	refresh_button_clicked(NULL, NULL);
	return 0;
}

//int creat_check_buttons(GtkWidget * container)
//{
//	int i;
//	char str[300];
//
//	for (i = 0; i < history_db.history_total_frame; i++)
//	{
//		sprintf(str, history_db.history_frame_ar[i].table_name);
//		sprintf(str + strlen(str), "--%s", history_db.history_frame_ar[i].can_id);
//
//		history_db.history_frame_ar[i].check_button = gtk_check_button_new_with_label(str);
//		if (history_db.history_frame_ar[i].history_node_data_ar_length <= 0)
//		{
//			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(history_db.history_frame_ar[i].check_button), FALSE);
//		}
//		else
//		{
//			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(history_db.history_frame_ar[i].check_button), TRUE);
//		}
////		gtk_widget_set_sensitive(check_button_pointer_ar[i], FALSE);
//		g_signal_connect(G_OBJECT (history_db.history_frame_ar[i].check_button), "toggled",
//				G_CALLBACK (history_check_button_toggled), (gpointer ) &history_db.history_frame_ar[i]);
//
//		gtk_box_pack_start(GTK_BOX(container), history_db.history_frame_ar[i].check_button, TRUE, TRUE, 5);
//	}

//	for (i = 0; i < can_frame_ar_length; i++)
//	{
//		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_pointer_ar[i]), FALSE);
//		gtk_widget_set_sensitive(check_button_pointer_ar[i], FALSE);
//	}
//
//	return 0;
//}

int refresh_button_clicked(GtkWidget *widget, gpointer data)
{

//	pthread_mutex_lock(&history_db.history_draw_cache_mutex);
//	gtk_widget_set_sensitive(p->entry, FALSE);
//	/****************************** get db_name from combo_box to display, open db, get db/table_data *********************************/
//	char db_name[STRING_LENGTH];
//	strncpy(db_name, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(db_name_list_combo_box)), STRING_LENGTH);
//	if (strcmp(history_db.db_name, db_name) == 0 && strlen(db_name) != 0)  // the same db_name && db_name != NULL
//	{
//		printf("db is not changed. db_name: %s, history_db.db_name: %s \n", db_name, history_db.db_name);
//	}
//	else
//	{
//		/************** new db_name, get_all_sources ********************/
//
//	}
	/*************************************** compute standard_source ********************************************/
	GtkWidget * da = draw_area;
	history_db.history_draw_area_sigle_screen_width = gtk_widget_get_allocated_width((GtkWidget*) hbox2_draw_area);

	// history_draw_area_allocated_width: just need call once, will == history_db.history_draw_area_request_width
	history_db.history_draw_area_allocated_width = gtk_widget_get_allocated_width((GtkWidget*) da);

	/* useless */
//	history_human_max_x_coordinate = history_draw_area_allocated_width - DRAW_AREA_X_MARGIN;
	if (history_db.history_draw_area_height != gtk_widget_get_allocated_height((GtkWidget*) da))
	{
		history_db.history_draw_area_height = gtk_widget_get_allocated_height((GtkWidget*) da);
		history_db.history_human_max_y_coordinate = history_db.history_draw_area_height - DRAW_AREA_Y_MARGIN;
		compute_y_coordinate_of_data();
	}

	static int a = 0;
	printf("\nrefresh_button_clicked: %d,  history_draw_area_allocated_width: %f  \n", ++a,
			history_db.history_draw_area_allocated_width);

	/*********************************** refresh_source ***********************************/

	if (history_db.pix_count_of_one_s != gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_of_pix_conf))
			|| history_db.value_of_spin_button_of_x_coordinate
					!= gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_of_x_coordinate)))
	{
		history_db.pix_count_of_one_s = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_of_pix_conf));

		history_db.pix_count_of_one_ms = history_db.pix_count_of_one_s / 1000;

		history_db.x_max_for_current_pix_conf = history_db.total_ms
				* history_db.pix_count_of_one_ms+ DRAW_AREA_X_MARGIN;
		if (history_db.x_max_for_current_pix_conf > history_db.history_draw_area_allocated_width)
		{
			printf(
					"history_draw_x_and_time: history_db.x_max_for_current_pix_conf(%f) > history_draw_area_allocated_width(%f) \n",
					history_db.x_max_for_current_pix_conf, history_db.history_draw_area_allocated_width);
			exit(1);
		}

		// ....... > max?
		history_db.value_of_spin_button_of_x_coordinate = gtk_spin_button_get_value(
				GTK_SPIN_BUTTON(spin_button_of_x_coordinate));
		history_db.left_coordinate_of_current_screen = history_db.value_of_spin_button_of_x_coordinate
				- DRAW_AREA_X_MARGIN;
		// ....... > max?
		history_db.right_coordinate_of_current_screen = history_db.left_coordinate_of_current_screen
				+ history_db.history_draw_area_sigle_screen_width;
		// ....... > max?
		history_db.left_ms_of_current_screen = history_db.left_coordinate_of_current_screen
				/ history_db.pix_count_of_one_ms;
		// ....... > max?
		history_db.right_ms_of_current_screen = history_db.right_coordinate_of_current_screen
				/ history_db.pix_count_of_one_ms;

		//		if(history_db.current_ms_end_to_draw > history_db.stop_time.tv_sec * 1000 + history_db.stop_time.tv_usec)
		//		{
		//			history_db.current_ms_end_to_draw = history_db.stop_time.tv_sec * 1000 + history_db.stop_time.tv_usec;
		//		}

		/*************************************** compute node_data_ar[j].x which need to display ********************************************/
		refresh_source_of_all_node_data_ar_to_display();
	}

	/***************************************** draw line ******************************************/
	if (history_db.history_total_frame > 0)
	{
		GtkWidget * da = draw_area;
		GdkWindow * gdkwindow = gtk_widget_get_window((GtkWidget*) da);
//			gdk_window_set_event_compression(gdkwindow, FALSE);
		cairo_t * cr = gdk_cairo_create(gdkwindow);

		cairo_set_source_rgba(cr, 0, 0, 0, 1);
		cairo_paint(cr);
		cairo_select_font_face(cr, "AdobeHeitiStd", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, 12);

		history_draw_x_and_time(cr);
		history_draw_line(cr);

		history_ruler_area_call_back(ruler_area, NULL);

		cairo_destroy(cr);
	}

//	pthread_mutex_unlock(&history_db.history_draw_cache_mutex);
	return 0;
}

int history_ruler_area_call_back(GtkWidget * widget, void * data)
{
	static int a = 0;
	printf("\nhistory_ruler_area_call_back %d: \n", ++a);

	GtkWidget * da = widget;
	int ruler_area_width = gtk_widget_get_allocated_width((GtkWidget*) da);
	int ruler_area_height = gtk_widget_get_allocated_height((GtkWidget*) da);
//	history_ruler_human_max_x_coordinate = history_draw_area_allocated_width - DRAW_AREA_X_MARGIN;
	int ruler_human_max_y_coordinate = ruler_area_height - RULER_AREA_Y_MARGIN;

	/***************************************** background *****************************************************/

	cairo_t * cr = gdk_cairo_create(gtk_widget_get_window(da));
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_paint(cr);

	/************************************************* y ******************************************************/

	char str[300] =
	{0};
	double step_y;

	int i, j, k;
	history_node_t * p;
	for (i = 0; i < history_db.history_total_frame; i++)
	{
		for (j = 0; j < history_db.history_frame_ar[i].history_total_node; j++)
		{
			p = history_db.history_frame_ar[i].history_node_ar + j;
			if (p->display_type != SHOW_AS_LINE)
			{
				continue;
			}

			y_ruler_t ruler = p->ruler;
			step_y = ruler_human_max_y_coordinate / (ruler.step_total + 1);

			cairo_set_source_rgba(cr, p->rgba.red, p->rgba.green, p->rgba.blue, p->rgba.alpha);

			cairo_move_to(cr, ruler_area_width,
					get_y_cairo_coordinate_from_y_human_pix_value(ruler_human_max_y_coordinate, -SHORT_LINE_LENGTH));
			cairo_line_to(cr, ruler_area_width,
					get_y_cairo_coordinate_from_y_human_pix_value(ruler_human_max_y_coordinate,
							ruler_human_max_y_coordinate));

			for (k = 0; k <= ruler.step_total; k++)
			{
				cairo_move_to(cr, ruler_area_width,
						get_y_cairo_coordinate_from_y_human_pix_value(ruler_human_max_y_coordinate, step_y * k));
				cairo_line_to(cr, ruler_area_width - SHORT_LINE_LENGTH,
						get_y_cairo_coordinate_from_y_human_pix_value(ruler_human_max_y_coordinate, step_y * k));

				sprintf(str, "%d", ruler.step_number * k);
				cairo_move_to(cr, ruler_area_width - DRAW_AREA_X_MARGIN + 2,
						get_y_cairo_coordinate_from_y_human_pix_value(ruler_human_max_y_coordinate, step_y * k - 4));
				cairo_show_text(cr, str);
			}
			cairo_stroke(cr);

			/**** draw once ****/
			break;
		}

		/**** draw once ****/
		break;
	}

	cairo_destroy(cr);
//	gtk_widget_queue_draw(widget);
	return TRUE;
}

int history_draw_area_on_draw_event_call_back(GtkWidget *widget, GtkWidget * data)
{
	static int a = 0;
	printf("history_draw_area_call_back: %d,          widget == %p, data == %p \n", ++a, widget, (GtkWidget*) data);//

	if (history_db.history_total_frame <= 0)
	{
		printf("***** history_draw_area_call_back: history_total_frame: %d \n", history_db.history_total_frame);
		return 0;
	}

//	real_time_ruler.pixel_count_of__integer_one = human_max_y_coordinate / (real_time_ruler.step_number * (real_time_ruler.step_total + 1));

	/*************************************** compute standard_source ********************************************/

	history_db.history_draw_area_sigle_screen_width = gtk_widget_get_allocated_width((GtkWidget*) hbox2_draw_area);

	// history_draw_area_allocated_width: just need call once, will == history_db.history_draw_area_request_width
	history_db.history_draw_area_allocated_width = gtk_widget_get_allocated_width((GtkWidget*) widget);

	/* useless */
//	history_human_max_x_coordinate = history_draw_area_allocated_width - DRAW_AREA_X_MARGIN;
	if (history_db.history_draw_area_height != gtk_widget_get_allocated_height((GtkWidget*) widget))
	{
		history_db.history_draw_area_height = gtk_widget_get_allocated_height((GtkWidget*) widget);
		history_db.history_human_max_y_coordinate = history_db.history_draw_area_height - DRAW_AREA_Y_MARGIN;
		compute_y_coordinate_of_data();
	}

	refresh_button_clicked(NULL, NULL);

//	GtkWidget * da = (GtkWidget*) widget;
//	GdkWindow * gdkwindow = gtk_widget_get_window(GTK_WIDGET(da));
//	cairo_t * cr = gdk_cairo_create(gtk_widget_get_window(GTK_WIDGET(da)));
//
//	cairo_set_source_rgba(cr, 0, 0, 0, 1);
//	cairo_paint(cr);

//	cairo_translate(cr, 0, -300);
//	gtk_widget_draw(da, cr);
//	cairo_set_source_rgba(cr, 0, 0, 1, 1);
//	cairo_set_line_width(cr, 5);
//	cairo_move_to(cr, 100, 100);
//	cairo_line_to(cr, screen_width2, 100);
////	screen_width2 = 500;
//	cairo_set_font_size(cr, 12);
//
//	char str[50];
//	int i;
//	for (i = 50; i < screen_width2; i += 50)
//	{
//		sprintf(str, "%d ", i);
//		cairo_move_to(cr, i, 100);
//		cairo_show_text(cr, str);
//	}
//	  GtkAllocation allocation;
//	  cairo_surface_t * surface;
//	  if (surface)
//	    cairo_surface_destroy (surface);
//	gtk_widget_get_allocation(widget, &allocation);
//
//	cairo_surface_t * surface = gdk_window_create_similar_surface(
//			gtk_widget_get_window(da),
//			CAIRO_CONTENT_COLOR,
//			allocation.width,
//			allocation.height);
//
//	cairo_t *cr = cairo_create(surface);
//	cairo_set_source_rgba(cr, 0, 0, 0, 1);
//	cairo_surface_destroy(surface);
//	cairo_paint(cr);

//	pthread_mutex_lock(&history_db.history_draw_cache_mutex);
//	history_draw_x_and_time_and_line(cr);
//	history_ruler_area_call_back(ruler_area, NULL);
//	pthread_mutex_unlock(&history_db.history_draw_cache_mutex);

//	cairo_destroy(cr);

	//	gtk_widget_send_expose(GTK_WIDGET(da), "draw");

	/*
	 * If you want to force an area of a window to be redrawn,
	 * use gdk_window_invalidate_rect() or gdk_window_invalidate_region().
	 * To cause the redraw to be done immediately, follow that call with a call to gdk_window_process_updates();
	 */

	/* cause call_back_func run 100 times/s */
//	GdkRGBA rgba =	{0, 0, 0.3, 1};
//	gdk_window_set_background_rgba(gdkwindow, &rgba);
//	gdk_window_set_background_pattern(gdkwindow, cairo_pattern_t * pattern);
//	GdkRectangle rect;
//	gtk_widget_get_allocation(da, &rect);
//	gdk_window_invalidate_rect(gdkwindow, &rect, FALSE);
//	gdk_window_process_updates(gdkwindow, FALSE);
//	gtk_widget_queue_draw(da);
	return TRUE;
}

int refresh_source_of_all_node_data_ar_to_display(void)
{
	/* find current_head_index_to_draw
	 * just compute time.tv_sec for a simple way
	 */

	int i, j, k;
	history_node_t * p;
	for (i = 0; i < history_db.history_total_frame; i++)
	{
		for (j = 0; j < history_db.history_frame_ar[i].history_total_node; j++)
		{
			p = history_db.history_frame_ar[i].history_node_ar + j;
			refresh_source_of_one_node_data_ar_to_display(p);
		}
	}

	return 0;
}

int refresh_source_of_one_node_data_ar_to_display(history_node_t * p)
{
	if (p->display_type == SHOW_AS_ENTRY)
	{

	}

	if (p->display_type != SHOW_AS_LINE || p->line_active == FALSE)
	{
		return 0;
	}

	if (p->history_node_data_ar[0].ms > history_db.right_ms_of_current_screen
			|| p->history_node_data_ar[p->history_node_data_ar_length - 1].ms < history_db.left_ms_of_current_screen)
	{
		p->current_head_index_to_draw = -1;
		p->current_end_index_to_draw = -1;
		printf("refresh_source_of_one_node_data_ar_to_display: node_name: %s, no data in current screen \n",
				p->node_name);
		return 0;
	}

	/**************** seek current_head_index_to_draw *********************/
	int k;
	for (k = 0; k < p->history_node_data_ar_length; k++)
	{
		if (p->history_node_data_ar[k].ms >= history_db.left_ms_of_current_screen)
		{
			p->current_head_index_to_draw = (k == 0 ? 0 : --k);
			break;
		}
	}

	/**************** seek current_end_index_to_draw *********************/
	for (; k < p->history_node_data_ar_length; k++)
	{
		if (p->history_node_data_ar[k].ms > history_db.right_ms_of_current_screen)
		{
			p->current_end_index_to_draw = k;
			break;
		}
	}
	if (k == p->history_node_data_ar_length)
	{
		p->current_end_index_to_draw = p->history_node_data_ar_length - 1;
	}

	/**************** compute x *********************/
	for (k = p->current_head_index_to_draw; k <= p->current_end_index_to_draw; k++)
	{
		/* DRAW_AREA_X_MARGIN == x of start_time */
		p->history_node_data_ar[k].x = p->history_node_data_ar[k].ms
				* history_db.pix_count_of_one_ms+ DRAW_AREA_X_MARGIN;
		//			printf("history_db.history_frame_ar[%d].row_ar[%d].x: %f, history_db.history_frame_ar[%d].row_ar[%d].y: %f \n",
		//					i, j, history_db.history_frame_ar[i].row_ar[j].x,
		//					i, j, history_db.history_frame_ar[i].row_ar[j].y);
	}

	printf("refresh_source_of_one_node_data_ar_to_display: node_name: %s, "
			"length: %d, head_index_to_draw: %d, end_index_to_draw: %d \n",
					p->node_name,
					p->history_node_data_ar_length,
					p->current_head_index_to_draw,
					p->current_end_index_to_draw);
	return 0;
}

int draw_entry_value_on_draw_area(void)
{

	return 0;
}

int history_draw_x_and_time(cairo_t *cr)
{
	static int a = 0;
	printf("***** history_draw_x_and_time: %d \n", ++a);

	/************************************ draw every s *********************************************/

	//	history_db.current_x_coordinate_from_start + history_draw_area_sigle_screen_width;
	int start_second = (int) (history_db.left_ms_of_current_screen / 1000);
	double start_x = history_db.pix_count_of_one_s * start_second + DRAW_AREA_X_MARGIN;
	int second_number_to_display = start_second;

	//	int end_second = (int)(history_db.right_ms_of_current_screen / 1000) + 1;
	// add history_db.pix_count_of_one_s for full line
	double end_x = start_x + history_db.history_draw_area_sigle_screen_width + history_db.pix_count_of_one_s;

	double x = start_x;
	double y = get_y_cairo_coordinate_from_y_human_pix_value(history_db.history_human_max_y_coordinate, 0);

	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_set_line_width(cr, 1.5);
	cairo_select_font_face(cr, "AdobeHeitiStd", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 18);

	cairo_move_to(cr, start_x, y);
	cairo_line_to(cr, end_x, y);
	cairo_stroke(cr);

	static char str[100];

	while (x <= end_x)
	{
		cairo_move_to(cr, x, y);
		cairo_line_to(cr, x,
				get_y_cairo_coordinate_from_y_human_pix_value(history_db.history_human_max_y_coordinate,
						-SHORT_LINE_LENGTH));

		sprintf(str, "%d", second_number_to_display);
		cairo_move_to(cr, x - 5,
				get_y_cairo_coordinate_from_y_human_pix_value(history_db.history_human_max_y_coordinate,
						-SHORT_LINE_LENGTH * 3));
		cairo_show_text(cr, str);

		x += history_db.pix_count_of_one_s;
		second_number_to_display++;
	}
	cairo_stroke(cr);

	/************************************ draw every 0.1 s *********************************************/

	if (history_db.pix_count_of_one_s >= 500)
	{
		double step = history_db.pix_count_of_one_s / 10;
		int k = 0;
		x = start_x;

		cairo_set_source_rgba(cr, 1, 1, 1, 1);
		cairo_set_line_width(cr, 1);
		cairo_select_font_face(cr, "AdobeHeitiStd", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, 10);

		while (x <= end_x)
		{
			k++;
			x += step;
			if (k % 10 == 0)
			{
				continue;
			}

			cairo_move_to(cr, x, y);
			cairo_line_to(cr, x,
					get_y_cairo_coordinate_from_y_human_pix_value(history_db.history_human_max_y_coordinate,
							-SHORT_LINE_LENGTH / 2));

			sprintf(str, "%d", k % 10);
			cairo_move_to(cr, x,
					get_y_cairo_coordinate_from_y_human_pix_value(history_db.history_human_max_y_coordinate,
							-SHORT_LINE_LENGTH * 1.5));
			cairo_show_text(cr, str);
		}
		cairo_stroke(cr);
	}

	//	printf("******************************************* history_draw_x_and_time \n");
	return 0;
}

int history_draw_line(cairo_t *cr)
{
	static int a = 0;
	printf("***** history_draw_line: %d \n", ++a);
//	pthread_mutex_lock(&history_db.history_draw_cache_mutex);

	/********************************* draw data ****************************************/

	int i, j, k;
	history_node_t * p;
	for (i = 0; i < history_db.history_total_frame; i++)
	{
		for (j = 0; j < history_db.history_frame_ar[i].history_total_node; j++)
		{
			p = history_db.history_frame_ar[i].history_node_ar + j;

			if (p->display_type == SHOW_AS_ENTRY)
			{
				draw_entry_value_on_draw_area();
			}

			if (p->display_type != SHOW_AS_LINE)
			{
				continue;
			}
			if (p->line_active == FALSE)
			{
				continue;
			}
			if (p->current_head_index_to_draw < 0)
			{
				continue;
			}

			cairo_set_source_rgba(cr, p->rgba.red, p->rgba.green, p->rgba.blue, p->rgba.alpha);
			cairo_set_line_width(cr, p->line_width);

			for (k = p->current_head_index_to_draw; k <= p->current_end_index_to_draw; k++)
			{
				/* DRAW_AREA_X_MARGIN == x of start_time */
//				p->history_node_data_ar[k].x = p->history_node_data_ar[k].ms
//						* history_db.pix_count_of_one_ms+ DRAW_AREA_X_MARGIN;
				//			printf("history_db.history_frame_ar[%d].row_ar[%d].x: %f, history_db.history_frame_ar[%d].row_ar[%d].y: %f \n",
				//					i, j, history_db.history_frame_ar[i].row_ar[j].x,
				//					i, j, history_db.history_frame_ar[i].row_ar[j].y);
				cairo_line_to(cr, p->history_node_data_ar[k].x, p->history_node_data_ar[k].y);
				cairo_stroke(cr);

				cairo_arc(cr, p->history_node_data_ar[k].x, p->history_node_data_ar[k].y, 1, 0, 2 * M_PI);
				cairo_fill(cr);

				cairo_move_to(cr, p->history_node_data_ar[k].x, p->history_node_data_ar[k].y);
			}

			cairo_stroke(cr);
		}
	}

//	printf("******************************************* history_draw_line \n");
//	pthread_mutex_unlock(&history_db.history_draw_cache_mutex);
	return 0;
}

int compute_y_coordinate_of_data(void)
{

	static int a = 0;
	a++;
	printf("compute_y_coordinate_of_data: %d \n", a);

#ifdef PRINT
	printf("go_button_clicked: 8 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
#endif

	/*************************************** compute every row_ar[j].x/y ********************************************/

	int i, j, k;
	double temp_y_value;
	history_node_t * p;
	for (i = 0; i < history_db.history_total_frame; i++)
	{
		for (j = 0; j < history_db.history_frame_ar[i].history_total_node; j++)
		{
			p = history_db.history_frame_ar[i].history_node_ar + j;

			if (p->display_type == SHOW_AS_ENTRY)
			{

			}

			if (p->display_type != SHOW_AS_LINE)
			{
				continue;
			}
			p->ruler.pixel_count_of_single_integer = history_db.history_human_max_y_coordinate
					/ (p->ruler.step_number * (p->ruler.step_total + 1));

			for (k = 0; k < p->history_node_data_ar_length; k++)
			{
				temp_y_value = get_y_human_pix_value_from_y_real_value_with_y_ruler(p->history_node_data_ar[k].value,
						&p->ruler);

				p->history_node_data_ar[k].y = get_y_cairo_coordinate_from_y_human_pix_value(
						history_db.history_human_max_y_coordinate, temp_y_value);
			}
		}
	}

	return 0;
}

int free_all_sources(void)
{
	memset(history_db.db_name, 0, sizeof(history_db.db_name));
	sqlite3_close(history_db.sqlite3_history);
	history_db.sqlite3_history = NULL;

//	cairo_destroy(history_db.cr);
//	history_db.cr = NULL;

	history_db.start_time;
	history_db.end_time;
	history_db.x_max_for_current_pix_conf = 0;

//	history_db.spin_button_of_pix_conf;
//	history_db.value_of_spin_button_of_pix_conf;
//	double pix_of_one_second = history_db.value_of_spin_button_of_pix_conf;
//	history_db.pix_count_of_one_ms;

	int i, j, k;
	history_node_t * p;
	for (i = 0; i < history_db.history_total_frame; i++)
	{
		for (j = 0; j < history_db.history_frame_ar[i].history_total_node; j++)
		{
			p = history_db.history_frame_ar[i].history_node_ar + j;

			if (p->history_node_data_ar != NULL)
			{
				free(p->history_node_data_ar);
				p->history_node_data_ar = NULL;
			}
		}
	}

	history_db.history_total_frame = 0;
//	history_db.history_draw_cache_mutex;

	printf("free_all_sources returned. \n");
	return 0;
}

int mouse_button_press_call_back(GtkWidget *widget, GdkEventButton * event, void * data)
{
//	if(history_db.cr == NULL)
//	{
//		return 0;
//	}

	printf("mouse_press: event->x: %f, event->y: %f \n", event->x, event->y);

	static double x;
	static double y;
	static int index = 0;

	x = event->x;
	y = event->y;

	cairo_t * cr = gdk_cairo_create(gtk_widget_get_window(GTK_WIDGET(widget)));
	cairo_set_source_rgba(cr, 1, 1, 1, 1);

	cairo_set_line_width(cr, 0.5);
	cairo_move_to(cr, x, get_y_cairo_coordinate_from_y_human_pix_value(history_db.history_human_max_y_coordinate, 0));

	cairo_line_to(cr, x,
			get_y_cairo_coordinate_from_y_human_pix_value(history_db.history_human_max_y_coordinate,
					history_db.history_human_max_y_coordinate));

	cairo_stroke(cr);

	history_db.pix_count_of_one_ms;

	static double x1, y1;
	static char str[100];

	cairo_set_line_width(cr, 1);

	int i, j, k;
	history_node_t * p;
	for (i = 0; i < history_db.history_total_frame; i++)
	{
		for (j = 0; j < history_db.history_frame_ar[i].history_total_node; j++)
		{
			p = history_db.history_frame_ar[i].history_node_ar + j;

			if (p->display_type == SHOW_AS_ENTRY)
			{
				draw_entry_value_on_draw_area();
			}

			if (p->display_type != SHOW_AS_LINE)
			{
				continue;
			}
			if (p->line_active == FALSE)
			{
				continue;
			}
			if (p->history_node_data_ar_length <= 0)
			{
				continue;
			}

			if (x < p->history_node_data_ar[p->current_head_index_to_draw].x
					|| x > p->history_node_data_ar[p->current_end_index_to_draw].x)
			{
				continue;
			}

			for (k = p->current_head_index_to_draw; k <= p->current_end_index_to_draw; k++)
			{
				if (p->history_node_data_ar[k].x > x)
				{
//				if (j == 0)
//				{
//					x1 = x;
//					y1 = history_db.history_frame_ar[i].row_ar[0].y;
//					break;
//				}

					x1 = p->history_node_data_ar[k - 1].x;
					y1 = p->history_node_data_ar[k - 1].y;
					break;
				}
			}

			cairo_move_to(cr, x1, y1);
//		cairo_rectangle(cr, x1 - 1, y1 - 1, 2, 2);
			cairo_arc(cr, x1, y1, 2, 0, 2 * M_PI);
			cairo_fill(cr);

			cairo_move_to(cr, x1, y1 - 5);
			sprintf(str, "%.2lf", p->history_node_data_ar[k - 1].value);
			cairo_show_text(cr, str);
		}
	}

	if (++index >= 100)
	{
		index = 0;
	}

	cairo_destroy(cr);
	return TRUE;
}

int mouse_pointer_motion_call_back(GtkWidget *widget, GdkEventMotion * event, void * data)
{
	printf("mouse_motion: event->x: %f, event->y: %f \n", event->x, event->y);

	return TRUE;
}

void spin_button_of_x_coordinate_value_changed(GtkWidget *widget, void * data)
{
	static int a = 0;
	printf("spin_button_of_x_coordinate_value_changed: %d \n", ++a);

	if (draw_area == NULL)
	{
		return;
	}

	refresh_button_clicked(NULL, NULL);
	//	g_signal_emit_by_name(draw_area, "draw");
	return;
}

void spin_button_of_pix_conf_value_changed(GtkWidget *widget, void * data)
{
	static int a = 0;
	printf("spin_button_of_pix_conf_value_changed: %d \n", ++a);

	if (draw_area == NULL)
	{
		return;
	}

	refresh_button_clicked(NULL, NULL);
//	g_signal_emit_by_name(draw_area, "draw");
	return;
}

void history_check_button_toggled_call_back(GtkWidget * widget, void * data)
{

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))		// GTK_CHECK_BUTTON(obj)
	{
		((history_node_t*) data)->line_active = TRUE;
		refresh_source_of_one_node_data_ar_to_display((history_node_t*) data);
	}
	else
	{
		((history_node_t*) data)->line_active = FALSE;
	}

	refresh_button_clicked(NULL, NULL);
	return;
}

