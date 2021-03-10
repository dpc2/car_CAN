/*
 * history.h
 *
 *  Created on: 2017年9月6日
 *      Author: root
 */

#pragma once
#include <can_real_time.h>
#include "includes.h"
#include "can_standard.h"

#define ROW_LIMIT_BY_SECOND		(8200)

typedef struct
{
	struct timeval time;
	double ms;				// ms = time.tv_sec * 1000 + time.tv_usec / 1000 - ms of start_time
	double value;
	double x;
	double y;
} history_node_data_t;

typedef struct
{
	char node_name[STRING_LENGTH];

	display_type_e display_type;
	GtkWidget * label;
	GtkWidget * entry;
	double entry_value;

	bool line_active;
	GtkWidget * check_button;

//	cairo_t * cr;
	rgba_t rgba;
	double line_width;
	y_ruler_t ruler;

	int history_node_data_ar_length;
	history_node_data_t * history_node_data_ar;

	int current_head_index_to_draw;
	int current_end_index_to_draw;
} history_node_t;									// col of table

typedef struct
{
	char table_name[STRING_LENGTH];
	int pf;
//	u32 can_id;
	int frame_received_total;
	double frame_received_one_s;

	int history_total_node;
	history_node_t history_node_ar[MAX_NODE_COUNT_OF_ONE_CAN_ID];
//	pthread_mutex_t history_draw_cache_mutex;
} history_frame_t;

typedef struct
{
	char db_name[STRING_LENGTH];
	sqlite3 * sqlite3_history;

	struct timeval start_time;						// x_coordinate of start_time == 0
	struct timeval end_time;
	double total_ms;

	double history_draw_area_height;
	//double history_human_max_x_coordinate;
	double history_human_max_y_coordinate;
	//= gtk_widget_get_allocated_height((GtkWidget*) widget);

	double history_draw_area_request_width;	// == MAX_PIX_COUNT_OF_ONE_S * (end_time - start_time)
	double history_draw_area_allocated_width;	// width
	double history_draw_area_sigle_screen_width;	// == width of hbox2_draw_area

	double pix_count_of_one_s;						// == value_of_spin_button_of_pix_conf
	double pix_count_of_one_ms;					// == pix_count_of_one_s / 1000
	double x_max_for_current_pix_conf;										// == pix_count_of_one_s * (end_time - start_time)

	double value_of_spin_button_of_x_coordinate;
	/* left-coordinate of current screen, == value of spin_button_of_x_coordinate - DRAW_AREA_X_MARGIN */
	double left_coordinate_of_current_screen;
	/* right-coordinate of current screen, == current_x_coordinate_from_start + history_draw_area_sigle_screen_width */
	double right_coordinate_of_current_screen;
	/* ms of left-coordinate of current screen, == left-coordinate of current screen / pix_count_of_one_ms */
	double left_ms_of_current_screen;
	/* ms of right-coordinate of current screen, == right-coordinate of current screen / pix_count_of_one_ms */
	double right_ms_of_current_screen;

	int history_total_frame;
	history_frame_t history_frame_ar[MAX_CAN_ID_COUNT];
} history_db_t;

typedef struct
{
	GtkWidget * check_button;

} check_button_t;

GtkWidget * history_select_db_handle(void);

int add_db_name_for_combo_box(GtkWidget *combo_box);
void ok_button_clicked_call_back(GtkWidget * widget, void * data);
int get_all_source(void);
int get_db_statis(void);
int get_table_statis(void);	// get table_name and count
//int sqlite3_history_get_conf_info_from_local_table_statis(void);
int sqlite3_history_get_node_name_and_count(void);
int sqlite3_history_get_node_data(void);

void history_select_db_window_destroy(GtkWidget * widget, void ** data);
int free_all_sources(void);

/********************************/

int history_display_handle(void);

int history_draw_area_on_draw_event_call_back(GtkWidget * widget, GtkWidget * data);
int compute_y_coordinate_of_data(void);
int refresh_button_clicked(GtkWidget *widget, gpointer data);

int refresh_source_of_all_node_data_ar_to_display(void);
int refresh_source_of_one_node_data_ar_to_display(history_node_t * p);
int draw_entry_value_on_draw_area(void);
int history_draw_x_and_time(cairo_t *cr);
int history_draw_line(cairo_t *cr);

int history_ruler_area_call_back(GtkWidget * widget, void * data);

void history_check_button_toggled_call_back(GtkWidget * widget, void * data);
void spin_button_of_x_coordinate_value_changed(GtkWidget *widget, void * data);
void spin_button_of_pix_conf_value_changed(GtkWidget *widget, void * data);
int mouse_button_press_call_back(GtkWidget *widget, GdkEventButton * event, void * data);
int mouse_pointer_motion_call_back(GtkWidget *widget, GdkEventMotion * event, void * data);

void history_display_window_destroy(GtkWidget * widget, void ** data);


history_node_t * seek_row_ar_index(history_node_t * p, int l, double v);
