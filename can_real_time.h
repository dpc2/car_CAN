#pragma once

#include "includes.h"
#include <can_standard.h>

/*
 * IN_FRAME_ARRAY_LENGTH >= (MICRO_SECONDS_OF_DRAW_LINES_DELAY + (draw_area_width - X_MARGIN) * MS_COUNT_OF_ONE_PIX) * FRAME_COUNT_OF_ONE_MS
 *
 */

#define	BIT_LENGTH_OF_ABLITY_TO_DRAW_LINE	(8)

#define	COMMON_Y_RULER			{1000, 10, 0}
//#define	COMMON_STEP_NUNBER	(5000)
//#define	COMMON_STEP_TOTAL		(10)

#define	RGBA_WHITE				{1, 1, 1, 1}
#define	RGBA_RED					{1, 0, 0, 1}
#define	RGBA_GREEN				{0, 1, 0, 1}
#define	RGBA_BLUE				{0, 0, 1, 1}
#define	RGBA_YELLO				{1, 1, 0, 1}

#define	COMMON_LINE_WIDTH		(0.5)

typedef enum
{
	SHOW_AS_ENTRY,
	SHOW_AS_LINE,
	SHOW_AS_TEXT_IN_DRAW_AREA,
//	SHOW_AS_COMMAND,
	SHOW_AS_NOTHING
} display_type_e;

typedef struct
{
	int step_number;
	int step_total;	// not very big
	/* pixel_count_of_single_integer = human_max_y_coordinate / (y_ruler.step_number * (y_ruler.step_total + 1)) */
	double pixel_count_of_single_integer;
} y_ruler_t;

typedef struct
{
	double red;
	double green;
	double blue;
	double alpha;
//	GdkRGBA;
} rgba_t;

typedef struct
{
	/*********** read from configure ***********/
	display_type_e display_type;
	y_ruler_t y_ruler;
	rgba_t rgba;
	double line_width;
//	char ** errcode_text;

	/*********** customer select ***********/
	GtkWidget * check_button_of_cuntomer_choose_node;
	bool node_choosed;
//	bool entry_active;

	double last_value;
	double this_value;

	/********** entry_area ************/
	char * node_name;

	GtkWidget * label;
	GtkWidget * entry;
	double entry_value;

	/********** real_time draw_source ************/
	GtkWidget * line_active_check_button;
	volatile bool line_active;
	cairo_t * cr;

	cairo_path_data_t cairo_path_data;

	cairo_path_data_t * cairo_data_ar;
	cairo_path_data_t * cairo_data_ar2;
	cairo_path_t cairo_path;

	int cairo_data_ar_length;
	volatile int cairo_data_ar_total_point_data;		// total_data == num_header == num_point
	volatile int cairo_data_ar_head_header_index;
	volatile int cairo_data_ar_tail_header_index;

/********************************/
/********** history draw_source ************/

} can_node_real_time_t;

typedef struct
{
	/*********** read from configure ***********/
	u32 id;
	int pf;	// frame_id

	/*********** customer select ***********/
	GtkWidget * check_button_of_customer_choose_frame;
	bool frame_choosed;

	/*******************************************/
	int total_node;
	can_node_real_time_t can_node_real_time_ar[MAX_NODE_COUNT_OF_ONE_CAN_ID];
} can_frame_real_time_t;

//typedef struct
//{
//	u32 can_id;
//	/*name is id name & database table name */can
//	char name[STRING_LENGTH];
//	int frame_received_total;
//	double frame_received_count_every_ms;
//
//	/**********************/
//	volatile int show_in_entry_area;	// 1:yes; 0: no
//	GtkWidget * real_time_label;
//	GtkWidget * real_time_entry;
//	double entry_value;   // == draw_source.node.value
//
//	/**********************/
//	volatile int show_in_draw_area;	// 1:yes; 0: no
//	GtkWidget * real_time_check_button;
//
//	cairo_t * cr;
////	rgba_t rgba;
////	double line_wigth;
////	y_ruler_t * p_y_ruler;
//
//	/********** draw_area ************/
//	cairo_path_data_t cairo_data_header;
//	cairo_path_data_t * cairo_data_ar;
//	cairo_path_data_t * cairo_data_ar2;
//	cairo_path_t cairo_path;
//
//	int cairo_data_ar_length;
//	volatile int cairo_data_ar_total_point_data;		// total_data == num_header == num_point
//	volatile int cairo_data_ar_head_header_index;
//	volatile int cairo_data_ar_tail_header_index;
//
//}can_frame_real_time_t;

void dialog_of_real_time_select_pf(GtkButton *button, void *data);

int init_can_frame_real_time_ar(void);
int sqlite3_db_statis_store_new_db_statis(const char *new_db_name, struct timeval *start_time);
int delete_db(void);
int real_time_creat_all_tables_and_update_statis(void);

int update_real_time_draw_source(int i);

double get_x_cairo_coordinate_from_x_human_pix_value(double human_x_coordinate);
//inline double get_x_cairo_coordinate_from_time(double human_x_coordinate);
double get_y_human_pix_value_from_y_real_value_with_y_ruler(double y_value, y_ruler_t *p_ruler);
double get_y_cairo_coordinate_from_y_human_pix_value(double human_max_y_coordinate_of_draw_area,
		double human_y_coordinate);

int real_time_handle(GtkWidget *do_widget, void *data);
int real_time_on_draw_event_call_back(GtkWidget *widget, gpointer data);
int real_time_draw_background_and_ruler(cairo_t *cr_background);
int real_time_draw_line(void *draw_area);
int draw_line(can_node_real_time_t *p);
int draw_line2(can_node_real_time_t *p);

void real_time_check_button_toggled_call_back(GtkCheckButton * check_button, can_node_real_time_t *p);

void real_time_window_destroy(GtkWidget *widget, void **data);	// GdkEvent * event,
void exit_button_clicked(GtkWidget *widget, void **data);
void dialog_of_user_db_name(GtkButton *button, gpointer user_data);

/* useless */
int draw_test(cairo_t * cr);
int gtk_can_read_and_more(GIOChannel * io_channel, GIOCondition condition, gpointer data);

