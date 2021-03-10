/*
 * history_select_db.c
 *
 *  Created on: 2017年9月13日
 *      Author: root
 */

#include <can_history.h>
#include <can_real_time.h>
#include "includes.h"
#include "db.h"

extern can_frame_t can_frame_ar[];
extern int can_frame_ar_length;
extern can_frame_real_time_t can_frame_real_time_ar[];
extern int can_frame_real_time_ar_length;

extern sqlite3 * sqlite3_db_statis;

extern char *g_db_path;

extern GtkWidget * main_window;
extern GtkWidget * history_select_db_window;
extern GtkWidget * history_display_window;

extern history_db_t history_db;

GtkWidget * db_name_list_combo_box;

GtkWidget * history_select_db_handle(void)
{
	int padding = 10;
	static int a = 0;

	if (history_select_db_window == NULL)
	{
		printf("\n*** history_select_db(): %d \n", a++);

		gtk_widget_set_sensitive(GTK_WIDGET(main_window), FALSE);

		history_select_db_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_default_size(GTK_WINDOW(history_select_db_window), 400, 400);
		gtk_window_set_title(GTK_WINDOW(history_select_db_window), "选择数据库");
		g_signal_connect(history_select_db_window, "destroy", G_CALLBACK(history_select_db_window_destroy),
				&history_select_db_window);

		GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);
		gtk_container_add(GTK_CONTAINER(history_select_db_window), vbox);

		db_name_list_combo_box = gtk_combo_box_text_new_with_entry();
		gtk_box_pack_start(GTK_BOX(vbox), db_name_list_combo_box, FALSE, TRUE, 10);
//		gchar * gtk_combo_box_text_get_active_text(GtkComboBoxText *combo_box);
		if (add_db_name_for_combo_box(db_name_list_combo_box) < 0)
		{
			exit(1);
		}

		GtkWidget * ok_button = gtk_button_new_with_label("OK");
		gtk_box_pack_start(GTK_BOX(vbox), ok_button, FALSE, TRUE, 10);
		g_signal_connect(ok_button, "clicked", G_CALLBACK (ok_button_clicked_call_back),
				(void* )history_select_db_window);

//		gtk_widget_add_events(box,
//				GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_SMOOTH_SCROLL_MASK
//						| GDK_TOUCH_MASK);
//		g_signal_connect(box, "event", G_CALLBACK(event_cb), event_data);

	}

	if (FALSE == gtk_widget_get_visible(history_select_db_window))
	{
		gtk_widget_show_all(history_select_db_window);
	}
	else
	{
//		gtk_widget_destroy(history_select_db_window);
//		history_select_db_window = NULL;
	}

	return history_select_db_window;
}

int add_db_name_for_combo_box(GtkWidget *combo_box)
{
	char str[512];
	char **result;
	int row, col;
	char *errmsg;

	sprintf(str, "select * from db_statis ORDER BY row DESC limit %d", MAX_DB_COUNT_TO_STORE);

	if (SQLITE_OK != sqlite3_get_table(sqlite3_db_statis, str, &result, &row, &col, &errmsg))
	{
		printf("select * from db_statis fail. row = %d, %s \n", row, errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	if (row == 0)
	{
		printf("select * from db_statis: row = %d \n", row);
		return 0;
	}

//	sprintf(sql_str, "create table if not exists  db_name_table("
//			"row				integer			primary key		autoincrement,"
//			"db_name			varchar(50)		default 0,"
//			"start_second			integer			default 0,"
//			"start_millisecond	integer			default 0)");

	int i;
	int j;
	for (i = 0, j = col; i < row; i++, j += col)
	{
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), result[j + 1]);
	}

	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);
	sqlite3_free_table(result);
	return row;
}

void ok_button_clicked_call_back(GtkWidget * widget, void * data)
{
	memset(&history_db, 0, sizeof(history_db_t));
//	free_all_sources();

	strncpy(history_db.db_name, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(db_name_list_combo_box)),
	STRING_LENGTH);

	if(strlen(history_db.db_name) == 0)
	{
		return;
	}
	printf("ok_button_clicked_call_back: history_db.db_name: %s \n", history_db.db_name);

	get_all_source();

//	printf("ok_button_clicked_call_back 2: \n");

	history_display_handle();
	gtk_widget_set_sensitive(GTK_WIDGET(data), FALSE);
//	gtk_widget_realize(widget);
}

int get_all_source(void)
{
	printf("*** get_all_source: db_name: %s \n", history_db.db_name);

	if (history_db.sqlite3_history != NULL)
	{
		printf("history_db.sqlite3_history != NULL \n");
		sqlite3_close(history_db.sqlite3_history);
	}
//	printf("get_all_source: 2 \n");

//	char db_full_name[100];
//	strncpy(db_full_name, db_path, sizeof(db_full_name));
//
//	strncpy(db_full_name + strlen(db_full_name), history_db.db_name, sizeof(db_full_name) - strlen(db_full_name) - 1);
//	db_full_name[sizeof(db_full_name) - 1] = 0;
	//	puts(db_full_name);

//	printf("get_all_source: 3 \n");

	if (open_db(&history_db.sqlite3_history, g_db_path, history_db.db_name) < 0)
	{
		exit(1);
	}

	if (get_db_statis() < 0)
	{
		exit(1);
	}

//	printf("get_all_source: 4 \n");

	if (get_table_statis() < 0)
	{
		exit(1);
	}

//	printf("get_all_source: 5 \n");

	if (sqlite3_history_get_node_name_and_count() < 0)
	{
		exit(1);
	}

//	printf("get_all_source: 6 \n");

	if (sqlite3_history_get_node_data() < 0)
	{
		exit(1);
	}

	printf("*** get_all_source: return \n");

	return 0;
}

int get_db_statis(void)
{
	char str[500];
	char **result;
	int row, col;
	char *errmsg;

	sprintf(str, "select * from db_statis where db_name='%s'", history_db.db_name);

	//	if (SQLITE_OK != sqlite3_get_table(history_db.sqlite3_history, str, &result, &row, &col, &errmsg))
	if (SQLITE_OK != sqlite3_get_table(sqlite3_db_statis, str, &result, &row, &col, &errmsg))
	{
		printf("error: %s fail. row = %d, errmsg: %s \n", str, row, errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	if (row == 0)
	{
		printf("???: %s: row = %d, errmsg: %s \n", str, row, errmsg);
		return -1;
	}

	//	history_db.pix_count_of_one_ms = atof(result[col + 1]);
	history_db.start_time.tv_sec = atoi(result[col + 2]);
	history_db.start_time.tv_usec = atoi(result[col + 3]);
	history_db.end_time.tv_sec = atoi(result[col + 4]);
	history_db.end_time.tv_usec = atoi(result[col + 5]);

#ifdef ROW_LIMIT_BY_SECOND
	if (history_db.end_time.tv_sec - history_db.start_time.tv_sec > ROW_LIMIT_BY_SECOND)
	{
		history_db.end_time.tv_sec = history_db.start_time.tv_sec + ROW_LIMIT_BY_SECOND;
	}
#endif

	history_db.total_ms = (history_db.end_time.tv_sec - history_db.start_time.tv_sec) * 1000
			+ (history_db.end_time.tv_usec - history_db.start_time.tv_usec) / 1000;

	// add 3 * MAX_PIX_COUNT_OF_ONE_S for safe
	history_db.history_draw_area_request_width = (history_db.total_ms / 1000) * MAX_PIX_COUNT_OF_ONE_S
			+ 3 * MAX_PIX_COUNT_OF_ONE_S;

	history_db.pix_count_of_one_s = history_db.pix_count_of_one_ms * 1000;

	/* need set = -1 */
	history_db.pix_count_of_one_ms = -1;
	/* need set = -1 */
	history_db.pix_count_of_one_s = -1;
	/* need set = -1 */
	history_db.value_of_spin_button_of_x_coordinate = -1;

	printf("\nget_db_statis: "
			"start_time: %ld.%ld \n"
			"stop_time: %ld.%ld \n"
			"stop_time - start_time: %ld.%ld \n"
			"pix_count_of_one_ms: %f \n"
			"history_db.x_max: %f \n"
			"history_draw_area_request_width: %f \n", history_db.start_time.tv_sec, history_db.start_time.tv_usec,
			history_db.end_time.tv_sec, history_db.end_time.tv_usec,
			history_db.end_time.tv_sec - history_db.start_time.tv_sec,
			history_db.end_time.tv_usec - history_db.start_time.tv_usec, history_db.pix_count_of_one_ms,
			history_db.x_max_for_current_pix_conf, history_db.history_draw_area_request_width);

	sqlite3_free_table(result);
	return 0;
}

int get_table_statis(void)
{
	char str[500];
	char **result;
	int row, col;
	char *errmsg;

	sprintf(str, "select * from table_statis");

//	if (SQLITE_OK != sqlite3_get_table(history_db.sqlite3_history, str, &result, &row, &col, &errmsg))
	if (SQLITE_OK != sqlite3_get_table(history_db.sqlite3_history, str, &result, &row, &col, &errmsg))
	{
		printf("error: %s. row = %d, errmsg: %s \n", str, row, errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	if (row == 0)
	{
		printf("error: %s: row = %d, errmsg: %s \n", str, row, errmsg);
		return -1;
	}

	history_db.history_total_frame = row;
	if (history_db.history_total_frame > MAX_CAN_ID_COUNT)
	{
		printf("history_db.history_total_frame > MAX_CAN_ID_COUNT \n");
		return -1;
	}

	int i, k;
	for (i = 0, k = col; k < (row + 1) * col && i < history_db.history_total_frame; k += col, i++)
	{
		strncpy(history_db.history_frame_ar[i].table_name, result[k + 1], STRING_LENGTH);
		history_db.history_frame_ar[i].table_name[STRING_LENGTH - 1] = 0;

		history_db.history_frame_ar[i].pf = atoi(result[k + 2]);
		history_db.history_frame_ar[i].frame_received_total = atoi(result[k + 3]);
		history_db.history_frame_ar[i].frame_received_one_s = atof(result[k + 4]);

//		sqlite3_history_get_table_data(char *table_name);
	}

	sqlite3_free_table(result);
	return 0;
}

int sqlite3_history_get_node_name_and_count(void)
{
	int i, j, k, m, n;
	char str[200];
	char **result;
	int row, col;
	char *errmsg;
	//	sqlite3_exec();
	/****************************** get data *********************************/

	history_frame_t * p;
	for (i = 0; i < history_db.history_total_frame; i++)
	{
		p = history_db.history_frame_ar + i;
		for (m = 0; m < can_frame_ar_length; m++)
		{
			if (can_frame_ar[m].pf == p->pf)
			{
				break;
			}
		}
		if (m == can_frame_ar_length)
		{
			printf("m(==%d) >= can_frame_ar_length. pf == %d \n", m, p->pf);
			exit(1);
		}
//		strncpy(p->table_name,  can_frame_ar[m].);
		sprintf(p->table_name, "pf_%d", p->pf);
		puts(p->table_name);

		sprintf(str, "select * from %s ORDER BY row LIMIT 1", p->table_name);
		//		puts(str);
		if (SQLITE_OK != sqlite3_get_table(history_db.sqlite3_history, str, &result, &row, &col, &errmsg))
		{
			printf("error: %s. errmsg: %s \n", str, p->table_name, errmsg);
			sqlite3_free(errmsg);
			return -1;
		}
		if (row <= 0)
		{
			printf("%s, row = %d, errmsg: %s\n", str, row, errmsg);
			p->history_total_node = 0;
			continue;
		}

		/*******************************************************/

		p->history_total_node = col - 3;		// 3: row, second, millisecond
		if (p->history_total_node <= 0)
		{
			printf("p->history_total_node <= 0, pf: %d \n", p->pf);
			exit(1);
		}
		if (p->history_total_node > MAX_NODE_COUNT_OF_ONE_CAN_ID)
		{
			printf("p->history_total_node > MAX_NODE_COUNT_OF_ONE_CAN_ID, pf: %d \n", p->pf);
			exit(1);
		}

//		can_frame_ar[m].total_node = sizeof(can_frame_ar[m].can_node_ar) / sizeof(can_node_t);
//		printf("can_frame_ar[%d].total_node == %d \n", m, can_frame_ar[m].total_node);

		for (j = 0, k = 3; k < col; k++, j++)
		{
			strncpy(p->history_node_ar[j].node_name, result[k], STRING_LENGTH);
			p->history_node_ar[j].node_name[STRING_LENGTH - 1] = 0;

			puts(p->history_node_ar[j].node_name);
//			printf("strlen(p->history_node_ar[%d].node_name) == %d \n", j, strlen(p->history_node_ar[j].node_name));

			for (n = 0; n < can_frame_ar[m].total_node; n++)
			{
				if (strcmp(can_frame_ar[m].can_node_ar[n].node_name, p->history_node_ar[j].node_name) == 0)
				{
					p->history_node_ar[j].display_type =
							can_frame_real_time_ar[m].can_node_real_time_ar[n].display_type;

					p->history_node_ar[j].ruler = can_frame_real_time_ar[m].can_node_real_time_ar[n].y_ruler;
					p->history_node_ar[j].line_width = can_frame_real_time_ar[m].can_node_real_time_ar[n].line_width;
					p->history_node_ar[j].rgba = can_frame_real_time_ar[m].can_node_real_time_ar[n].rgba;
				}
			}
		}

		sqlite3_free_table(result);
	}

	return 0;
}

//
//int sqlite3_history_get_conf_info_from_local_table_statis(void)
//{
//
//	int i;
//	char str[500];
//	char **result;
//	int row, col;
//	char *errmsg;
//
//	sprintf(str, "select * from local_table_statis");
//
//	//	if (SQLITE_OK != sqlite3_get_table(history_db.sqlite3_history, str, &result, &row, &col, &errmsg))
//	if (SQLITE_OK != sqlite3_get_table(history_db.sqlite3_history, str, &result, &row, &col, &errmsg))
//	{
//		printf("get start time fail. row = %d, errmsg: %s \n", row, errmsg);
//		sqlite3_free(errmsg);
//		exit(1);
//	}
//	if (row <= 0)
//	{
//		printf("get start time: row = %d, errmsg: %s \n", row, errmsg);
//		return -1;	// .................................................................................................
//	}
//	history_db.history_frame_ar_length = row;
//
//	if (history_db.history_frame_ar != NULL)
//	{
//		free(history_db.history_frame_ar);
//		history_db.history_frame_ar = NULL;
//	}
//
//	history_db.history_frame_ar = (history_table_t*) malloc(sizeof(history_table_t) * history_db.history_frame_ar_length);
//	if (history_db.history_frame_ar == NULL)
//	{
//		printf("(history_table_t*) malloc(sizeof(history_table_t) * history_db.history_frame_ar_length) == NULL \n");
//		return -1;
//	}
//	memset(history_db.history_frame_ar, 0, sizeof(history_table_t) * history_db.history_frame_ar_length);
//
//	int k;
//	for (i = 0, k = col; k < (row + 1) * col && i < history_db.history_frame_ar_length; k += col, i++)
//	{
//		strncpy(history_db.history_frame_ar[i].table_name, result[k + 1], STRING_LENGTH);
//		history_db.history_frame_ar[i].can_id = atoi(result[k + 2]);
//		history_db.history_frame_ar[i].rgba.red = atof(result[k + 3]);
//		history_db.history_frame_ar[i].rgba.green = atof(result[k + 4]);
//		history_db.history_frame_ar[i].rgba.blue = atof(result[k + 5]);
//		history_db.history_frame_ar[i].rgba.alpha = atof(result[k + 6]);
//		history_db.history_frame_ar[i].line_width = atof(result[k + 7]);
//		history_db.history_frame_ar[i].history_ruler.step_number = atoi(result[k + 8]);
//		history_db.history_frame_ar[i].history_ruler.step_total = atoi(result[k + 9]);
//
//	}
//
//	for (i = 0; i < history_db.history_frame_ar_length; i++)
//	{
//		printf("history_db.history_frame_ar[%d]: \n"
//				"table_name: %s \n"
//				"can_id: %d \n"
//				"ruler.step_number: %d \n"
//				"ruler.step_total: %d \n", i, history_db.history_frame_ar[i].table_name, history_db.history_frame_ar[i].can_id,
//				history_db.history_frame_ar[i].history_ruler.step_number, history_db.history_frame_ar[i].history_ruler.step_total);
//	}
//
//	return 0;
//}

int sqlite3_history_get_node_data(void)
{
	int i, j, k, m, n;
	char str[200];
	char **result;
	int row, col;
	char *errmsg;
	//	sqlite3_exec();
	/****************************** get data *********************************/

	history_frame_t * pf;
	history_node_t * pn;

	for (i = 0; i < history_db.history_total_frame; i++)
	{
		pf = history_db.history_frame_ar + i;

		for (m = 0; m < pf->history_total_node; m++)
		{
			pn = pf->history_node_ar + m;

#ifdef ROW_LIMIT_BY_SECOND
			// WHERE row<7000
			sprintf(str, "select second, millisecond, %s from %s WHERE row<%d ORDER BY row ", pn->node_name,
					pf->table_name, ROW_LIMIT_BY_SECOND * FRAME_COUNT_OF_ONE_S);
#else
			sprintf(str, "select second, millisecond, %s from %s  ORDER BY row ", pn->node_name, pf->table_name);
#endif
			//		puts(str);
			if (SQLITE_OK != sqlite3_get_table(history_db.sqlite3_history, str, &result, &row, &col, &errmsg))
			{
				printf("error: %s. errmsg: %s \n", str, errmsg);
				sqlite3_free(errmsg);
				return -1;
			}
			if (row <= 0)
			{
				printf("%s, row = %d, errmsg: %s\n", str, row, errmsg);
				pf->history_total_node = 0;
				continue;
			}

			pn->history_node_data_ar_length = row;
			pn->history_node_data_ar = (history_node_data_t*) malloc(
					sizeof(history_node_data_t) * pn->history_node_data_ar_length);
			if (pn->history_node_data_ar == NULL)
			{
				printf("pn->history_node_data_ar == NULL, pn->node_name: %s \n", pn->node_name);
				return -1;
			}
			printf("sqlite3_history_get_node_data: pf: %d, node_name: %s, node_data_ar_length: %d, size: %dK \n\n",
					pf->pf, pn->node_name, pn->history_node_data_ar_length,
					sizeof(history_node_data_t) * pn->history_node_data_ar_length / 1024);

			for (j = 0, k = col; k < (row + 1) * col; k += col, j++)
			{
				pn->history_node_data_ar[j].time.tv_sec = atoi(result[k + 0]);
				pn->history_node_data_ar[j].time.tv_usec = atoi(result[k + 1]);
				pn->history_node_data_ar[j].value = atof(result[k + 2]);

				pn->history_node_data_ar[j].ms =
						(pn->history_node_data_ar[j].time.tv_sec - history_db.start_time.tv_sec) * 1000
								+ (pn->history_node_data_ar[j].time.tv_usec - history_db.start_time.tv_usec) / 1000;

//				printf("*** [%d].value: %.2lf ", j, pn->history_node_data_ar[j].value);
			}
			printf("\n\n");
			sqlite3_free_table(result);
		}
	}

//		p->history_total_node =
//		p->history_node_ar = (history_node_data_t*) malloc(sizeof(history_node_data_t) * p->history_total_node);
//		if (p->history_node_ar == NULL)
//		{
//			printf("history_db.history_frame_ar[%d].history_node_ar \n", i);
//			return -1;
//		}
//		memset(p->history_node_ar, 0, sizeof(history_node_data_t) * p->history_total_node);
//		p->show_in_draw_area = 1;
//		for (j = 0, k = col; k < (row + 1) * col; k += col, j++)
//		{
//			p->history_node_ar[j].time.tv_sec = atoi(result[k + 1]);	// value
//			p->history_node_ar[j].time.tv_usec = atoi(result[k + 2]);	// x
//			p->history_node_ar[j].value = atof(result[k + 3]);	// y
//
//			p->history_node_ar[j].ms = (p->history_node_ar[j].time.tv_sec - history_db.start_time.tv_sec) * 1000
//					+ (p->history_node_ar[j].time.tv_usec - history_db.start_time.tv_usec) / 1000;
//
////			printf("history_db.history_frame_ar[%d].history_node_ar[%d]: time.tv_sec = %ld, time.tv_usec = %d, value = %f  \n", i, j,
////					p->history_node_ar[j].time.tv_sec, p->history_node_ar[j].time.tv_usec,
////					p->history_node_ar[j].value);
//
//		}
//		if (j != row)
//		{
//			printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX j != row: i = %d, j = %d, row = %d  \n", i, j, row);
//			exit(1);
//		}

	return 0;
}

void history_select_db_window_destroy(GtkWidget * widget, void ** data)
{
	printf("\nhistory_select_db_window_destroy: widget == %p, *data == %p \n", widget, *data);

	if (history_display_window != NULL)
	{
		if (1)
		{
			// work
			gtk_widget_destroy(history_display_window);	// ......... gtk_widget_destroy(widget) == send destroy signal to widget ???
			history_display_window = NULL;
		}
		else
		{
			// not work
			history_display_window_destroy(history_display_window, (void**) &history_display_window);
		}
	}

	gtk_widget_destroyed(widget, (GtkWidget**) data);	// also destroy all child_widget; set *pointer = NULL;
	gtk_widget_set_sensitive(GTK_WIDGET(main_window), TRUE);
	printf("history_select_db_window_destroy returned. \n");
}
