//
//

#include <can_real_time.h>
#include "includes.h"
#include "db.h"

extern GtkWidget * main_window;
extern GtkWidget * real_time_window;
extern GtkWidget * history_display_window;

static GtkWidget * flowbox_1 = NULL;
static GtkWidget * flowbox_2 = NULL;

///////////////////////////////////////////////////////////////////////////////

sqlite3 * sqlite3_db_statis;
sqlite3 * sqlite3_real_time;

pthread_mutex_t real_time_draw_cache_mutex;
volatile int draw_real_time_draw_line_func_id;
volatile int can_read_func_should_exit;

extern char *g_db_path;
char time_part_of_new_db_name[STRING_LENGTH];
char user_part_of_new_db_name[STRING_LENGTH];

///////////////////////////////////////////////////////////////////////////////

extern int screen_width;
extern int screen_height;

double draw_area_width;							// recompute in real_time_on_draw_event_call_back()
double draw_area_height;						// recompute in real_time_on_draw_event_call_back()
double human_max_x_coordinate;				// recompute in real_time_on_draw_event_call_back()
double human_max_y_coordinate;				// recompute in real_time_on_draw_event_call_back()

double x_move_to_left_current;
double x_move_to_left_last;
double x_cairo_translate;
double x_of_this_draw_time;

int count_of_on_draw_event_called;
int count_of_on_draw_event_called_record;

///////////////////////////////////////////////////////////////////////////////

struct timeval start_time;
struct timeval last_show_entry_time;
struct timeval last_draw_time;
struct timeval this_draw_time; // .......... current ms_time

extern can_frame_t can_frame_ar[];
extern int can_frame_ar_length;

extern can_frame_real_time_t can_frame_real_time_ar[];
extern int can_frame_real_time_ar_length;
int can_node_real_time_choosed;

y_ruler_t g_ruler =
{100, 10, 0};

//GtkWidget * window;

//	GtkWidget *gtk_check_button_new_with_label ( const gchar *label );

//// #ifconfig can0 down

static void real_time_select_pf_check_button_call_back(GtkWidget * widget, void * data)
{
	int i;
	bool x = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

	((can_frame_real_time_t*) data)->frame_choosed = x;
	for (i = 0; i < ((can_frame_real_time_t*) data)->total_node; i++)
	{
		gtk_widget_set_sensitive(
				GTK_WIDGET(
						((can_frame_real_time_t* )data)->can_node_real_time_ar[i].check_button_of_cuntomer_choose_node),
				x);
	}

}
static void real_time_select_node_check_button_call_back(GtkWidget * widget, void * data)
{
	((can_node_real_time_t*) data)->node_choosed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void dialog_of_real_time_select_pf(GtkButton *button, gpointer user_data)
{

//	real_time_window;
	char str[STRING_LENGTH];
	sprintf(str, "???： %s", time_part_of_new_db_name);

	GtkWidget * dialog = gtk_dialog_new_with_buttons("select_pf", GTK_WINDOW(user_data),
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, _("_OK"), GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL,
			NULL);
	gtk_widget_set_size_request(dialog, 500, 400);

	GtkWidget * content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
//	gtk_widget_set_size_request(scrolled, (int) (screen_width * 0.1), (int) (screen_height * 0.5));
//	gtk_container_add(GTK_CONTAINER(draw_hbox3), scrolled_2);
	gtk_box_pack_start(GTK_BOX(content_area), scrolled, TRUE, TRUE, 0);

	GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);
//	gtk_box_pack_start(GTK_BOX(content_area), vbox, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(scrolled), vbox);

	int i, k;
	GtkWidget * frame;
	GtkWidget * separator;
	can_frame_t * pf;
	can_frame_real_time_t * pfr;
	can_node_t * pn;
	can_node_real_time_t * pnr;

	for (i = 0; i < can_frame_ar_length; i++)
	{
		pf = can_frame_ar + i;
		pfr = can_frame_real_time_ar + i;
		pfr->frame_choosed = FALSE;
		pf->frame_choosed = FALSE;

		separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
		gtk_box_pack_start(GTK_BOX(vbox), separator, TRUE, TRUE, 2);
//		frame = gtk_frame_new(str);
//		gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 2);
//		sprintf(str, "pf: %d", pf->pf);
		sprintf(str, "************* pf: %d, %s -> %s, id: 0X%08X *************", pf->pf, pf->out, pf->in, pf->id);
		pfr->check_button_of_cuntomer_choose_frame = gtk_check_button_new_with_label(str);
		gtk_box_pack_start(GTK_BOX(vbox), pfr->check_button_of_cuntomer_choose_frame, TRUE, TRUE, 2);
		g_signal_connect(G_OBJECT (pfr->check_button_of_cuntomer_choose_frame), "toggled",
				G_CALLBACK (real_time_select_pf_check_button_call_back), (gpointer )pfr);

		for (k = 0; k < pf->total_node; k++)
		{
			pn = pf->can_node_ar + k;
			pnr = pfr->can_node_real_time_ar + k;
			pnr->node_choosed = FALSE;
			pn->node_choosed = FALSE;
//			pn->node_name = pn->NODE_NAME_TO_USE;

			sprintf(str, "node: %s", pn->node_name);
			pnr->check_button_of_cuntomer_choose_node = gtk_check_button_new_with_label(str);
			gtk_box_pack_start(GTK_BOX(vbox), pnr->check_button_of_cuntomer_choose_node, TRUE, TRUE, 2);
			g_signal_connect(G_OBJECT (pnr->check_button_of_cuntomer_choose_node), "toggled",
					G_CALLBACK (real_time_select_node_check_button_call_back), (gpointer )pnr);

			gtk_widget_set_sensitive(GTK_WIDGET(pnr->check_button_of_cuntomer_choose_node), FALSE);
		}
		separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
		gtk_box_pack_start(GTK_BOX(vbox), separator, TRUE, TRUE, 2);
	}

	gtk_widget_show_all(scrolled);
	int response;

	loop:

	response = gtk_dialog_run(GTK_DIALOG(dialog));

//	user_part_of_new_db_name[STRING_LENGTH];
	if (response == GTK_RESPONSE_OK)
	{
		int sum = 0;
		for (i = 0; i < can_frame_ar_length; i++)
		{
//			pf = can_frame_ar + i;
			pfr = can_frame_real_time_ar + i;
			if (pfr->frame_choosed == FALSE)
			{
				continue;
			}

			for (k = 0; k < pfr->total_node; k++)
			{
//				pn = pf->can_node_ar + k;
				pnr = pfr->can_node_real_time_ar + k;
				sum += pnr->node_choosed;
			}
		}

		printf("dialog_of_real_time_select_pf: sum = %d \n", sum);
		if (sum > 0)
		{
			real_time_handle(NULL, NULL);
			gtk_widget_destroy(dialog);
		}
		else
		{
			goto loop;
		}
	}

	gtk_widget_destroy(dialog);
}

int real_time_handle(GtkWidget * do_widget, void * data)
{
	printf("real_time_handle 1 \n");
	/*
	 *		gtk_widget_set_sensitive(button_history, FALSE);
	 gtk_widget_set_visible(button_history, FALSE);
	 * 	gtk_widget_set_sensitive(button_history, TRUE);
	 gtk_widget_set_visible(button_history, TRUE);
	 *  	gtk_widget_translate_coordinates (GtkWidget  *src_widget,
	 GtkWidget  *dest_widget,
	 gint        src_x,
	 gint        src_y,
	 gint       *dest_x,
	 gint       *dest_y);
	 */

	if (history_display_window != NULL)	// if history_display_window is running
	{
		return 0;
	}

	if (init_can_frame_real_time_ar() < 0)
	{
		exit(1);
	}

	gtk_widget_set_sensitive(GTK_WIDGET(main_window), FALSE);

	pthread_mutex_init(&real_time_draw_cache_mutex, PTHREAD_MUTEX_TIMED_NP);
	int padding = 0;

	printf("real_time_handle 2 \n");
	if (real_time_window == NULL)
	{
		real_time_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//		gtk_window_set_screen(GTK_WINDOW(real_time_window), gtk_widget_get_screen(do_widget));
		gtk_window_set_title(GTK_WINDOW(real_time_window), "实时");
		gtk_container_set_border_width(GTK_CONTAINER(real_time_window), 10);
//		gtk_widget_set_size_request(window, 600, 600);
		gtk_window_maximize(real_time_window);	// title will still exist
//		gtk_window_fullscreen(GTK_WINDOW(window));	// title will not exist
//		gtk_window_set_resizable (GTK_WINDOW(window), FALSE);  // not a good choice
//		g_signal_connect(window, "destroy", G_CALLBACK (gtk_widget_destroy), window);
		g_signal_connect(real_time_window, "destroy", G_CALLBACK (real_time_window_destroy),
				(void** )&real_time_window);	//  "delete-event"

//		int window_width = gtk_widget_get_allocated_width(window);  // will be 1 before display all finished
//		int window_height = gtk_widget_get_allocated_height(window);

		GtkWidget * main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);
		gtk_container_add(GTK_CONTAINER(real_time_window), main_vbox);

		/*********************************************************************/
//		GTK_ORIENTATION_VERTICAL
		GtkWidget * hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
		gtk_box_pack_start(GTK_BOX(main_vbox), hbox1, TRUE, TRUE, padding);

		GtkWidget * button_of_exit = gtk_button_new_with_label("exit");
		gtk_box_pack_start(GTK_BOX(hbox1), button_of_exit, TRUE, TRUE, padding);
		g_signal_connect(button_of_exit, "clicked", G_CALLBACK(exit_button_clicked), (void** )&real_time_window);

		GtkWidget * scrolled_1 = gtk_scrolled_window_new(NULL, NULL);
		gtk_widget_set_size_request(scrolled_1, (int) (screen_width * 0.8), (int) (screen_height * 0.1));
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); //GTK_POLICY_ALWAYS
//		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_1), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
//		gtk_container_add(GTK_CONTAINER(hbox1), scrolled_1);
		gtk_box_pack_start(GTK_BOX(hbox1), scrolled_1, TRUE, TRUE, padding);

		flowbox_1 = gtk_flow_box_new();
		gtk_widget_set_valign(flowbox_1, GTK_ALIGN_START);
		gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox_1), 10);
		gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox_1), GTK_SELECTION_NONE);
		gtk_container_add(GTK_CONTAINER(scrolled_1), flowbox_1);

		/******************************************* draw_vbox *******************************************/

		GtkWidget * draw_hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
		gtk_box_pack_start(GTK_BOX(main_vbox), draw_hbox3, TRUE, TRUE, padding);

		GtkWidget * draw_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, padding);
		gtk_box_pack_start(GTK_BOX(draw_hbox3), draw_vbox, TRUE, TRUE, padding);

		GtkWidget *frame = gtk_frame_new("draw");
		gtk_box_pack_start(GTK_BOX(draw_vbox), frame, TRUE, TRUE, padding);

		/******************************************* draw_area *******************************************/

		GtkWidget *draw_area = gtk_drawing_area_new();
		gtk_widget_set_size_request(draw_area, (int) (screen_width * 0.6), (int) (screen_height * 0.7));
		gtk_container_add(GTK_CONTAINER(frame), draw_area);
		g_signal_connect(draw_area, "draw", G_CALLBACK(real_time_on_draw_event_call_back), NULL);

		/******************************************* scrolled_2, flowbox_2 *******************************************/

		GtkWidget *scrolled_2 = gtk_scrolled_window_new(NULL, NULL);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_widget_set_size_request(scrolled_2, (int) (screen_width * 0.15), (int) (screen_height * 0.5));
		gtk_container_add(GTK_CONTAINER(draw_hbox3), scrolled_2);

		flowbox_2 = gtk_flow_box_new();
		gtk_widget_set_valign(flowbox_2, GTK_ALIGN_START);
		gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox_2), 1);
		gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox_2), GTK_SELECTION_NONE);
		gtk_container_add(GTK_CONTAINER(scrolled_2), flowbox_2);

//		real_time_label = (GtkWidget*) malloc(sizeof(void*) * can_frame_ar_length);
//		real_time_entry = (GtkWidget*) malloc(sizeof(void*) * can_frame_ar_length);
//		real_time_check_button = (GtkWidget*) malloc(sizeof(void*) * can_frame_ar_length);

		int i, k;
//		can_frame_t * pf;
		can_frame_real_time_t * pfr;

//		can_node_t * pn;
		can_node_real_time_t * pnr;

		for (i = 0; i < can_frame_ar_length; i++)
		{
//			pf = can_frame_ar + i;
			pfr = can_frame_real_time_ar + i;

			if (pfr->frame_choosed == FALSE)
			{
				continue;
			}

			for (k = 0; k < pfr->total_node; k++)
			{
//				pd = pf->can_node_ar + k;
				pnr = pfr->can_node_real_time_ar + k;

				if (pnr->node_choosed == FALSE)
				{
					continue;
				}

				if (pnr->display_type == SHOW_AS_ENTRY)
				{
					pnr->label = gtk_label_new(pnr->node_name);
					gtk_container_add(GTK_CONTAINER(flowbox_1), pnr->label);
					pnr->entry = gtk_entry_new();
					gtk_widget_set_sensitive(pnr->entry, FALSE);
					gtk_container_add(GTK_CONTAINER(flowbox_1), pnr->entry);
					//void gtk_entry_set_text(GtkEntry *entry, const gchar *text);
				}

				if (pnr->display_type == SHOW_AS_LINE)
				{
					pnr->line_active_check_button = gtk_check_button_new_with_label(pnr->node_name);
					gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pnr->line_active_check_button), pnr->line_active);
					g_signal_connect(G_OBJECT (pnr->line_active_check_button), "toggled",
							G_CALLBACK (real_time_check_button_toggled_call_back), (gpointer )pnr);

					gtk_container_add(GTK_CONTAINER(flowbox_2), pnr->line_active_check_button);
				}
			}
		}

		/*************************************************************************************************************/

		draw_real_time_draw_line_func_id = g_timeout_add(MILLISECOND_OF_DRAW_LINES_DELAY, real_time_draw_line,
				(void*) draw_area);

//		char string[20];
//		sprintf(string, "%d", screen_width);
//		gtk_entry_set_text(GTK_ENTRY(real_time_ar[0].entry), string);
//		sprintf(string, "%d", screen_height);
//		gtk_entry_set_text(GTK_ENTRY(real_time_ar[1].entry), string);

		pthread_t send, receive;
		can_get_sockfd();
		pthread_create(&receive, NULL, (void*) can_read, NULL);
//		pthread_create(&send, NULL, can_write, NULL);

	}

	/**************************************************************************************************************************/
	/**************************************************************************************************************************/
	/**************************************************************************************************************************/
	/**************************************************************************************************************************/

	if (gtk_widget_get_visible(real_time_window) == FALSE)
	{
//		gtk_window_fullscreen(GTK_WINDOW(window));	// title will not exist
		gtk_widget_show_all(real_time_window);
	}
	else
	{
//		gtk_widget_hide(real_time_window);
		gtk_widget_show_all(real_time_window);
//		gtk_widget_destroy(window);
//		window = NULL;
	}

//	g_source_remove(source_id);
//	g_io_channel_shutdown(io_channel, TRUE, NULL);	// 关闭 iochannel 操作会把文件描述符关闭
//	g_io_channel_unref(io_channel);

	return 0;
}

//	g_timeout_add_full(G_PRIORITY_DEFAULT, MICRO_SECONDS_OF_LOOP_DELAY, draw_cb, (void*)draw_area, NULL);
/*
 pthread_t bbbbb_pid;
 pthread_create(&bbbbb_pid, NULL, bbbbb, (void*) draw_area);
 pthread_mutex_init(&draw_source_mutex,  PTHREAD_MUTEX_TIMED_NP);
 int pthread_mutex_lock(pthread_mutex_t *mutex)
 int pthread_mutex_unlock(pthread_mutex_t *mutex)
 int pthread_mutex_trylock(pthread_mutex_t *mutex)
 pthread_mutex_trylock()语义与pthread_mutex_lock()类似，不同的是在锁已经被占据时返回EBUSY而不是挂起等待。
 */
// gboolean (*GIOFunc) (GIOChannel *source, GIOCondition condition, gpointer data);
/*
 * 5. 当停止读取的事件发生时，回调函数应该做如下工作：
 * (1). 退出事件循环： g_source_remove (source_id);
 * (2). 关闭 IO_Channel： g_io_channel_shutdown (io_channel, TRUE, NULL);
 * (3). 释放 IO_Channel： g_io_channel_unref (io_channel);
 关闭 iochannel 操作会把文件描述符关闭。
 6. 其他：
 打开和关闭 iochannel 的顺序不可变。
 要是只想暂时不读文件妙算符，可以只退出事件循环。
 不保证退出事件循环后到来的数据是否会在内核中缓存，不保证这段时间内的数据是否全部被缓存，
 所以当你退出事件循环再加入时要自己检查数据是否是你所需要的。(不保证是因为我没有做过试验)
 iochannel不是什么新技术，它的基础是 select / poll，对比一下 g_io_add_watch 提供的事件选项和
 select / poll 提供的就清楚了。关于 select / poll 请 man 2 select_tut或者 man 2 poll。
 上面提到的方法对其他文件描述符都适用，我之前是把它用在socket上。
 */

int init_can_frame_real_time_ar(void)
{
	printf("***************************** init_real_time_ar start **************************** \n");
//	get data from message_set.txt
//	message_count = sizeof(real_time_ar) / sizeof(can_frame_real_time_t);
	if (can_frame_ar_length <= 0)
	{
		printf("can_frame_ar_length == %d \n", can_frame_ar_length);
		exit(1);
	}
	if (can_frame_ar_length != can_frame_real_time_ar_length)
	{
		printf("can_frame_ar_length(%d) != can_frame_real_time_ar_length(%d) \n", can_frame_ar_length,
				can_frame_real_time_ar_length);
		exit(1);
	}

	x_move_to_left_current = 0;
	x_move_to_left_last = 0;
	x_cairo_translate = 0;
	x_of_this_draw_time = 0;

	count_of_on_draw_event_called = 0;
	count_of_on_draw_event_called_record = -1;

	/****************************************/

	int i, k, m;
	can_node_t * p;
	can_node_real_time_t * q;

	for (i = 0; i < can_frame_real_time_ar_length; i++)
	{
		if (can_frame_real_time_ar[i].id != can_frame_ar[i].id)
		{
			printf("can_frame_real_time_ar[%d].id != can_frame_ar[%d].id \n", i, i);
			exit(1);
		}
		if (can_frame_real_time_ar[i].pf != can_frame_ar[i].pf)
		{
			printf("can_frame_real_time_ar[%d].pf != can_frame_ar[%d].pf \n", i, i);
			exit(1);
		}
		if (can_frame_real_time_ar[i].total_node != can_frame_ar[i].total_node)
		{
			printf("can_frame_real_time_ar[%d].total_node != can_frame_ar[%d].total_node \n", i, i);
			exit(1);
		}

		/****************/
//		can_frame_real_time_ar[i].pf = can_frame_ar[i].pf;
//		can_frame_real_time_ar[i].frame_choosed = TRUE; // ...................................................................
		/****************/

		can_frame_ar[i].frame_choosed = can_frame_real_time_ar[i].frame_choosed;
		if (can_frame_real_time_ar[i].frame_choosed == FALSE)
		{
			continue;
		}

		can_frame_ar[i].frame_received_total = 0;
		can_frame_ar[i].life = 0XFF;
		can_frame_ar[i].frame_count_every_ms = 1 / can_frame_ar[i].cycle_time_in_ms;

		for (k = 0; k < can_frame_real_time_ar[i].total_node; k++)
		{
			p = can_frame_ar[i].can_node_ar + k;
			q = can_frame_real_time_ar[i].can_node_real_time_ar + k;

//			p->node_name = p->NODE_NAME_TO_USE;
			q->node_name = p->node_name;
			puts(q->node_name);

			/****************/
//			q->node_choosed = TRUE;	// ...................................................................................
			/****************/

			if (q->node_choosed == FALSE)
			{
				continue;
			}

			if (q->cr != NULL)
			{
				cairo_destroy(q->cr);
				q->cr = NULL;
			}

			if (q->display_type != SHOW_AS_LINE)
			{
//				q->line_active = FALSE;
				continue;
			}

			/****************/
//			q->line_active = TRUE;	// ..............................
			/****************/

			/* L >= (T + T_delay) * P
			 * T = draw_area_width * MS_COUNT_OF_ONE_PIX
			 * draw_area_width is variable, screen_width is stable
			 * screen_width > draw_area_width  ->  use screen_width is ok
			 * T = screen_width * MS_COUNT_OF_ONE_PIX is ok
			 * add with 1000ms
			 */
			q->cairo_data_ar_length = (MS_COUNT_OF_ONE_PIX * screen_width + MILLISECOND_OF_DRAW_LINES_DELAY + 1000)
					* can_frame_ar[i].frame_count_every_ms * 2;

			if (q->cairo_data_ar_length % 2 != 0)
			{
				q->cairo_data_ar_length++;
			}
			printf("init_real_time_ar: real_time_ar[%d].cairo_data_ar_length == %d (KB: %.2f),  "
					"frame_received_count_every_ms == %f \n", i, q->cairo_data_ar_length,
					(double) sizeof(cairo_path_data_t) * q->cairo_data_ar_length / 1024,
					can_frame_ar[i].frame_count_every_ms);

			q->cairo_data_ar = (cairo_path_data_t*) malloc(sizeof(cairo_path_data_t) * q->cairo_data_ar_length);

			if (q->cairo_data_ar == NULL)
			{
				printf("real_time_ar[%d].cairo_data_ar == NULL \n", i);
				return -1;
			}
			printf("real_time_ar[%d].cairo_data_ar == %p \n", i, q->cairo_data_ar);
			memset(q->cairo_data_ar, 0, sizeof(cairo_path_data_t) * q->cairo_data_ar_length);

			/************************ init cairo_data_ar[] ***************************/

			cairo_path_data_t cairo_data_header;
			cairo_data_header.header.type = CAIRO_PATH_LINE_TO;
			cairo_data_header.header.length = 2;
			for (m = 0; m < q->cairo_data_ar_length; m += 2)
			{
				memcpy(q->cairo_data_ar + k, &cairo_data_header, sizeof(cairo_path_data_t));
			}

			q->cairo_data_ar_total_point_data = 0;
			q->cairo_data_ar_head_header_index = 0;
			q->cairo_data_ar_tail_header_index = 0;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		for (k = 0; k < p->cairo_data_ar_length; k += 2)
//		{
//			printf("draw_draw_area: cairo_data_ar ***** total_point: %d, k: %d *** type: %d, length: %d *** x: %f, y: %f \n",
//					p->cairo_data_ar_total_point_data,
//					k,
//					p->cairo_data_ar[k].header.type,
//					p->cairo_data_ar[k].header.length,
//					p->cairo_data_ar[k + 1].point.x,
//					p->cairo_data_ar[k + 1].point.y);
//		}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			q->cairo_data_ar2 = (cairo_path_data_t*) malloc(sizeof(cairo_path_data_t) * q->cairo_data_ar_length);
			if (q->cairo_data_ar2 == NULL)
			{
				printf("p->in_frame_data_ar2 == NULL \n");
				return -1;
			}
			printf("real_time_ar[%d].cairo_data_ar2 == %p \n", i, q->cairo_data_ar2);
			memset(q->cairo_data_ar2, 0, sizeof(cairo_path_data_t) * q->cairo_data_ar_length);
		}

	}

	/************************************************************************************************/
	/************************************************************************************************/
	gettimeofday(&start_time, NULL);
	last_draw_time = start_time;
	last_show_entry_time = start_time;
	can_read_func_should_exit = 0;

	time_t lt = time(0);
	struct tm *ptr = localtime(&lt);
	printf("time = %02d-%02d-%02d--%02d-%02d-%02d *** \n", (ptr->tm_year + 1900), ptr->tm_mon, ptr->tm_mday,
			ptr->tm_hour, ptr->tm_min, ptr->tm_sec);

	/*********************** create new database, store new database name and start_time *************************/

//	memset(user_part_of_new_db_name, 0, sizeof(user_part_of_new_db_name));
	sprintf(time_part_of_new_db_name, "%02d-%02d-%02d--%02d-%02d-%02d", (ptr->tm_year + 1900), ptr->tm_mon,
			ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);

	if (open_db(&sqlite3_real_time, g_db_path, time_part_of_new_db_name) < 0)	// ................................
	{
		exit(1);
	}

	sqlite3_db_statis_store_new_db_statis(time_part_of_new_db_name, &start_time);

	delete_db();

	/************************ create data tables ************************/

	if (real_time_creat_all_tables_and_update_statis() < 0)
	{
		exit(1);
	}

	printf("***************************** init_real_time_ar return **************************** \n");
	return 0;
}

int sqlite3_db_statis_store_new_db_statis(const char * new_db_name, struct timeval * start_time)
{
	char sql[200];
	char * errmsg;

	sprintf(sql, "INSERT INTO db_statis (db_name, start_second, start_millisecond)  VALUES ('%s', %ld, %d)",
			new_db_name, start_time->tv_sec, start_time->tv_usec);

	if (SQLITE_OK != sqlite3_exec(sqlite3_db_statis, sql, NULL, NULL, &errmsg))
	{
		printf("%s fail. errmsg: %s \n", sql, errmsg);
		sqlite3_free(errmsg);
		exit(1);
	}

	printf("sqlite3_db_statis_store_new_db_statis ok.     %s.\n", sql);
	return 0;
}

int real_time_creat_all_tables_and_update_statis(void)
{
	int i, k;
	char str[2000];
	char * errmsg;

	/********************* create table_statis ***************************/

	sprintf(str, "create table if not exists table_statis ("
			"row  						integer  	primary key  autoincrement,"
			"table_name					varchar(50)	default 0,"
			"pf							integer  	default 0,"
			"frame_received_total	integer  	default 0,"
			"frame_received_one_s	real			default 0)");

//		puts(str);
	if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, str, NULL, NULL, &errmsg))
	{
		printf("error: %s. errmsg: %s \n", str, errmsg);
		sqlite3_free(errmsg);
		return -1;
	}

	int sum = 0;
	can_node_real_time_t * p;
	/************************ create data tables ************************/
	for (i = 0; i < can_frame_real_time_ar_length; i++)
	{
		if (can_frame_real_time_ar[i].frame_choosed == FALSE)
		{
			continue;
		}
		for (k = 0, sum = 0; k < can_frame_real_time_ar[i].total_node; k++)
		{
			p = can_frame_real_time_ar[i].can_node_real_time_ar + k;
			if (p->node_choosed == TRUE)
			{
				sum++;
			}
		}
		if (sum == 0)
		{
			continue;
		}

		sprintf(str, "create table if not exists pf_%d ("
				"row  integer  primary key  autoincrement"
				", second  integer  default 0"
				", millisecond  integer  default 0", can_frame_ar[i].pf);

		for (k = 0; k < can_frame_real_time_ar[i].total_node; k++)
		{
			p = can_frame_real_time_ar[i].can_node_real_time_ar + k;

			if (p->node_choosed == FALSE)
			{
				continue;
			}
			sprintf(str + strlen(str), ", %s  real  default 0", p->node_name);
		}
		sprintf(str + strlen(str), ")");

//		puts(str);
		if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, str, NULL, NULL, &errmsg))
		{
			printf("create data-tables error: %s. errmsg: %s \n", str, errmsg);
			sqlite3_free(errmsg);
			return -1;
		}

		/********************* update table_statis ***************************/
		sprintf(str, "INSERT INTO table_statis (table_name, pf) VALUES('pf_%d', %d)", can_frame_ar[i].pf,
				can_frame_ar[i].pf);

		if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, str, NULL, NULL, &errmsg))
		{
			printf("update table_statis error: %s. errmsg: %s \n", str, errmsg);
			sqlite3_free(errmsg);
			return -1;
		}
	}

//	/********************* create local_table_statis ***************************/
//
//	sprintf(str, "create table if not exists local_table_statis ("
//			"row						integer			primary key		autoincrement,"
//			"table_name		varchar(50)		default 0,"
//			"can_id					integer			default 0,"
//			"rgba_red				real				default 0,"
//			"rgba_green				real				default 0,"
//			"rgba_blue				real				default 0,"
//			"rgba_alpha				real				default 0,"
//			"line_width				real				default 0,"
//			"ruler_step_number	integer			default 0,"
//			"ruler_step_total		integer			default 0)");
//
//	if (creat_table(sqlite3_real_time, str) < 0)
//	{
//		exit(1);
//	}
//
//	/********************* update local_table_statis ***************************/
//
//	for (i = 0; i < can_frame_ar_length; i++)
//	{
//		sprintf(str, "INSERT INTO local_table_statis ("
//				"can_id,"
//				"table_name,"
//				"rgba_red,"
//				"rgba_green,"
//				"rgba_blue,"
//				"rgba_alpha,"
//				"line_width,"
//				"ruler_step_number,"
//				"ruler_step_total)"
//
//				"VALUES(%d,'%s', %f, %f, %f, %f, %f, %d, %d)", real_time_ar[i].can_id, real_time_ar[i].name,
//				real_time_ar[i].rgba.red, real_time_ar[i].rgba.green, real_time_ar[i].rgba.blue,
//				real_time_ar[i].rgba.alpha, real_time_ar[i].line_wigth, real_time_ar[i].p_y_ruler->step_number,
//				real_time_ar[i].p_y_ruler->step_total);
//
//		if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, str, NULL, NULL, &errmsg))
//		{
//			printf("update local_table_statis fail. errmsg: %s \n", errmsg);
//			exit(1);
//		}
//	}

	return 0;
}

void real_time_check_button_toggled_call_back(GtkCheckButton *check_button, can_node_real_time_t *p)// GtkWidget * check_button, GTK_CHECK_BUTTON(obj)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button)))		// GTK_CHECK_BUTTON(obj)
	{
		p->line_active = TRUE;
	}
	else
	{
		p->line_active = FALSE;
	}
}

int real_time_on_draw_event_call_back(GtkWidget *widget, gpointer data)
{
//	gtk_widget_draw(GtkWidget * widget, cairo_t * cr);

	count_of_on_draw_event_called++;
	printf("count_of_on_draw_event_called: %d \n", count_of_on_draw_event_called);

	draw_area_width = gtk_widget_get_allocated_width((GtkWidget*) widget);
	draw_area_height = gtk_widget_get_allocated_height((GtkWidget*) widget);
	human_max_x_coordinate = draw_area_width - DRAW_AREA_X_MARGIN;
	human_max_y_coordinate = draw_area_height - DRAW_AREA_Y_MARGIN;

	int i, k;
	can_node_real_time_t * p;
	for (i = 0; i < can_frame_real_time_ar_length; i++)
	{
		if (can_frame_real_time_ar[i].frame_choosed == FALSE)
		{
			continue;
		}
		for (k = 0; k < can_frame_real_time_ar[i].total_node; k++)
		{
			p = can_frame_real_time_ar[i].can_node_real_time_ar + k;
			if (p->node_choosed == FALSE)
			{
				continue;
			}
			p->y_ruler.pixel_count_of_single_integer = human_max_y_coordinate
					/ (p->y_ruler.step_number * (p->y_ruler.step_total + 1));
		}
	}

	if (MILLISECOND_OF_DRAW_LINES_DELAY > draw_area_width * MS_COUNT_OF_ONE_PIX)
	{
		printf(
				"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX MILLISECOND_OF_DRAW_LINES_DELAY = %d > draw_area_width * MS_COUNT_OF_ONE_PIX \n",
				MILLISECOND_OF_DRAW_LINES_DELAY);
//			exit(1);
	}

//	cairo_t * cr = gdk_cairo_create(gtk_widget_get_window(widget));
//
//	cairo_set_source_rgba(cr, 0, 0, 0, 1);
//	cairo_paint(cr);
//
//	cairo_set_source_rgba(cr, 0, 0, 1, 1);
//	cairo_set_line_width(cr, 5);
//	cairo_move_to(cr, 100, 100);
//
////	screen_width2 = 500;
//	cairo_line_to(cr, screen_width2, 100);
//
//	cairo_set_font_size(cr, 24);
//	int i;
//	char str[50];
//	for(i = 0; i < screen_width2; i+=500)
//	{
//		sprintf(str, "%d ", i);
//		cairo_move_to(cr, i, 100);
//		cairo_show_text(cr, str);
//	}
//
//	draw_test(cr);
////	cairo_translate(cr, 100, 0);
////	draw_test(cr);
//
//	cairo_destroy(cr);
//	printf("draw_back_ground \n");
//	gtk_widget_queue_draw(widget);
	return 0;
//	void gdk_cairo_set_source_rgba(cairo_t *cr, const GdkRGBA *rgba);
}

int real_time_draw_line(void * draw_area)
{
	static int a = 0;
	a++;
	printf("real_time_draw_line: %d \n", a);

	static int i, k;
	static int path_data_size = sizeof(cairo_path_data_t);
	static cairo_t *cr_background = NULL;

	static can_node_t * p;
	static can_node_real_time_t * q;
	/**************************************************************************************************/
	/**************************************************************************************************/
	/*********************** check if on_draw_event was called again **********************************/

	if (count_of_on_draw_event_called_record != count_of_on_draw_event_called)
	{
		printf("count_of_on_draw_event_called_record != count_of_on_draw_event_called \n");
		/***********************************************/
		cairo_destroy(cr_background);
		cr_background = gdk_cairo_create(gtk_widget_get_window((GtkWidget*) draw_area));
		cairo_set_source_rgba(cr_background, 0, 0, 0, 1);
		cairo_select_font_face(cr_background, "AdobeHeitiStd", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr_background, 12);
//		printf("cairo_destroy(cr_background) called. \n");

		/***********************************************/
		for (i = 0; i < can_frame_real_time_ar_length; i++)
		{
			if (can_frame_real_time_ar[i].frame_choosed == FALSE)
			{
				continue;
			}
			for (k = 0; k < can_frame_real_time_ar[i].total_node; k++)
			{
				q = can_frame_real_time_ar[i].can_node_real_time_ar + k;
				if (q->node_choosed == FALSE)
				{
					continue;
				}

				if (q->cr != NULL)
				{
					cairo_destroy(q->cr);
					q->cr = NULL;
				}
				q->cr = gdk_cairo_create(gtk_widget_get_window((GtkWidget*) draw_area));
				cairo_set_source_rgba(q->cr, q->rgba.red, q->rgba.green, q->rgba.blue, q->rgba.alpha);
				cairo_set_line_width(q->cr, q->line_width);
				cairo_translate(q->cr, -x_move_to_left_last, 0);
//			cairo_move_to(q->cr, q->last_x, q->last_y);

				printf("real_time_draw_line reset_cr:   real_time_ar[%d].cr = %p  \n", i, q->cr);
			}
		}

		count_of_on_draw_event_called_record = count_of_on_draw_event_called;
		printf("real_time_draw_line reset_cr:   draw_back_ground_times_record  = %d  \n",
				count_of_on_draw_event_called_record);

	}
//	printf("real_time_draw_line: a \n");
	/***************************************************************************************************************/
	/***************************************************************************************************************/

	pthread_mutex_lock(&real_time_draw_cache_mutex);
	/**********************************************************************************************************/
	/******************************************* draw background **********************************************/

	real_time_draw_background_and_ruler(cr_background);

	gettimeofday(&this_draw_time, NULL);

// pix_move_to_left == x_of_this_draw_time;
	x_move_to_left_current = ((this_draw_time.tv_sec - start_time.tv_sec) * 1000
			+ (this_draw_time.tv_usec - start_time.tv_usec) / 1000) * PIX_COUNT_OF_ONE_MS;

	x_cairo_translate = x_move_to_left_current - x_move_to_left_last;
	x_move_to_left_last = x_move_to_left_current;

	x_of_this_draw_time = x_move_to_left_current + draw_area_width;

//	printf("real_time_draw_line: a.1 \n");
	static char str[50];
	int header_index, sum;
	for (i = 0; i < can_frame_real_time_ar_length; i++)
	{
		if (can_frame_real_time_ar[i].frame_choosed == FALSE)
		{
			continue;
		}
		for (k = 0; k < can_frame_real_time_ar[i].total_node; k++)
		{
			p = can_frame_ar[i].can_node_ar + k;
			q = can_frame_real_time_ar[i].can_node_real_time_ar + k;
			if (q->node_choosed == FALSE)
			{
				continue;
			}

			if (q->display_type == SHOW_AS_LINE)
			{

				/* whether real_time_ar[i] should be draw or not, the x_cairo_translate of  */
//			printf("real_time_draw_line: a.2 \n");
				cairo_translate(q->cr, -x_cairo_translate, 0);
//			printf("real_time_draw_line: a.3 \n");

				/*
				 if (q->line_active == FALSE)
				 {
				 continue;
				 }
				 */
				/************************** seek head_data *************************/
				for (header_index = q->cairo_data_ar_head_header_index, sum = 0;
						sum < q->cairo_data_ar_total_point_data; sum++)
				{
#ifdef PRINT
//			printf("real_time_draw_line: compute: real_time_ar[%d], q->x = %f, q->y = %f \n", i, q->x, q->y);
#endif
					/*********** seek head ************/
					if (q->cairo_data_ar[header_index + 1].point.x
							< x_of_this_draw_time - (draw_area_width - DRAW_AREA_X_MARGIN))
					{
//				header_index = (header_index == q->cairo_data_ar_length - 2) ? 0 : header_index + 2;
						header_index += 2;
						if (header_index >= q->cairo_data_ar_length)
						{
							header_index = 0;
						}
					}
					else	// if (q->cairo_data_ar[data_index].point.x - x_move_to_left >= 0)
					{
						break;
					}
				}

				/*******************************************************************************************************/
				/* if always point.x - x_move_to_left < 0, head_data_index will not be set, total_node should be set == 0
				 * so, should do this:get_set_size_request(scrolled_1, (int) (screen_width * 0.8), (int) (screen_height * 0.7));
				 g
				 * set head_position == tail_position */
				if (sum == q->cairo_data_ar_total_point_data)
				{
//					printf("real_time_draw_line: pf: %d, node_name: %s, sum == p->cairo_data_ar_total_point_data == %d, "
//									"x_move_to_left_current: %lf \n",
//									can_frame_real_time_ar[i].pf,
//									p->node_name, sum,
//									x_move_to_left_current);

					for (header_index = q->cairo_data_ar_head_header_index, sum = 0;
							sum < q->cairo_data_ar_total_point_data; sum++)
					{
						if (q->cairo_data_ar[header_index + 1].point.x
								< x_of_this_draw_time - (draw_area_width - DRAW_AREA_X_MARGIN))
						{
							printf("q->cairo_data_ar[%d].point.x: %lf \n", header_index + 1,
									q->cairo_data_ar[header_index + 1].point.x);

							header_index += 2;
							if (header_index >= q->cairo_data_ar_length)
							{
								header_index = 0;
							}
						}
					}

					q->cairo_data_ar_head_header_index = 0;
					q->cairo_data_ar_tail_header_index = 0;
					q->cairo_data_ar_total_point_data = 0;
				}
				else
				{
					q->cairo_data_ar_head_header_index = header_index;
					q->cairo_data_ar_total_point_data -= sum;
				}
				/*******************************************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		int k;																																	//
//		for (k = 0; k < q->cairo_data_ar_length; k += 2)																			//
//		{																																		    //
//			printf("draw_draw_area: cairo_data_ar: total: %d, head: %d, tail: %d, "											//
//					"k: %d, type: %d, length: %d, x: %f, y: %f \n", q->cairo_data_ar_total_point_data,					//
//					q->cairo_data_ar_head_header_index, q->cairo_data_ar_tail_header_index, k,								//
//					q->cairo_data_ar[k].header.type, q->cairo_data_ar[k].header.length,										//
//					q->cairo_data_ar[k + 1].point.x, q->cairo_data_ar[k + 1].point.y);										//
//		}																																			//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if (q->line_active == TRUE)
				{
//					draw_line(q);
					draw_line2(q);
				}
			}
			else
				if (q->display_type == SHOW_AS_ENTRY)
				{
//					sprintf(str, "%.3f", q->entry_value);
//					gtk_entry_set_text(GTK_ENTRY(q->entry), str);
//					printf("entry: %s, (int)(q->entry_value): %d \n", p->errcode_text[(int)(q->entry_value)], (int)(q->entry_value));
				gtk_entry_set_text(GTK_ENTRY(q->entry), p->errcode_text[(int)(q->entry_value)]);
			}
			else
				if (q->display_type == SHOW_AS_TEXT_IN_DRAW_AREA)
				{

				}
				else
					if (q->display_type == SHOW_AS_NOTHING)
					{
						// do nothing
					}
					else
					{
						printf("p->display_type == ??? \n");
						exit(1);
					}
	}
}

//	printf("real_time_draw_line: b \n");
//	/**********************************************************************************************************/
//	/******************************************* draw background **********************************************/
//
//	real_time_draw_background_and_ruler(cr_background);

#ifdef PRINT
//		printf("draw_draw_area: max_x_coordinate=%f, real_time_ar[%d].draw_source.total=%d \n",
//				max_x_coordinate, i, p->draw_source.total);
#endif

//	printf("real_time_draw_line: c \n");
/**********************************************************************************************************/
/**********************************************************************************************************/
/*********************************************** show entry ***********************************************/
/*
 static unsigned long int delay;
 delay = (this_draw_time.tv_sec - last_show_entry_time.tv_sec) * 1000
 + (this_draw_time.tv_usec - last_show_entry_time.tv_usec) / 1000;

 if (delay > MILLISECOND_OF_SHOW_ENTRY_DELAY)
 {
 for (i = 0; i < can_frame_ar_length; i++)
 {
 if (can_frame_real_time_ar[i].frame_choosed == FALSE)
 {
 continue;
 }
 for (k = 0; k < can_frame_real_time_ar[i].total_node; k++)
 {
 p = can_frame_real_time_ar[i].can_node_real_time_ar + k;
 if (p->node_choosed == FALSE)
 {
 continue;
 }

 if (q->display_type == SHOW_AS_ENTRY)
 {
 sprintf(str, "%.3f", q->entry_value);
 gtk_entry_set_text(GTK_ENTRY(q->entry), str);
 }
 }

 last_show_entry_time.tv_sec = this_draw_time.tv_sec;
 last_show_entry_time.tv_usec = this_draw_time.tv_usec;
 }
 }
 last_draw_time = this_draw_time;
 */
pthread_mutex_unlock(&real_time_draw_cache_mutex);

#ifdef PRINT
printf("draw_cb return  \n\n\n");
#endif

//	gtk_widget_queue_draw(draw_area);
//	printf("real_time_draw_line: return \n");
return TRUE;

//	cairo_append_path();
//	cairo_translate(cr, 100, 100);
//	cairo_image_surface_create_from_png("plaveckycastle.png");
//	cairo_pattern_set_extend (cairo_get_source ( cr), CAIRO_EXTEND_REPEAT) ;
//	cairo_translate(cr, 130, 30);
//	cairo_scale(cr, 0.7, 0.7);
//	cairo_save(cr);
//	cairo_restore(cr);

//	cairo_stroke(cr);
//	cairo_stroke_preserve(cr);
//	cairo_fill（cr）;
//	cairo_fill_preserve（cr）;

//	cairo_show_page (cairo_t *cr);
//	cairo_clip();	//  cai jian
//	cairo_clip_preserve();
//	MAX(a, b)
//	MIN(a, b);
/********************************/
}

int real_time_draw_background_and_ruler(cairo_t * cr)
{
/***************************************** background *****************************************************/

cairo_set_source_rgba(cr, 0, 0, 0, 1);
cairo_paint(cr);

/************************************************* x ******************************************************/

cairo_set_source_rgba(cr, 1, 1, 1, 1);
//	cr

//	get_x_cairo_coordinate_from_x_human_pix_value();
cairo_move_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(0),
		get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, 0));
cairo_line_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(human_max_x_coordinate),
		get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, 0));

/* this_draw_time.tv_sec - start_time.tv_sec == n, n == 0, 1, 2, 3 ... */
int second_number_to_display;
double human_x_coordinate_of_second_number_to_display;
//	second_gone = (this_draw_time.tv_sec - start_time.tv_sec) * 1000 + (this_draw_time.tv_usec - start_time.tv_usec) / 1000;
//	second_gone = second_gone / 1000;

if (this_draw_time.tv_usec >= start_time.tv_usec)
{
	human_x_coordinate_of_second_number_to_display = human_max_x_coordinate
			- ((this_draw_time.tv_usec - start_time.tv_usec) / 1000) * PIX_COUNT_OF_ONE_MS;
	second_number_to_display = this_draw_time.tv_sec - start_time.tv_sec;
}
else	// if(this_draw_time.tv_usec < start_time.tv_usec), at least tv_sec has added 1
{
	human_x_coordinate_of_second_number_to_display = human_max_x_coordinate
			+ ((start_time.tv_usec - this_draw_time.tv_usec) / 1000) * PIX_COUNT_OF_ONE_MS - PIX_COUNT_OF_ONE_S;
	second_number_to_display = this_draw_time.tv_sec - start_time.tv_sec - 1;
}

char str[50];

while (human_x_coordinate_of_second_number_to_display > 0)
{
	cairo_move_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(human_x_coordinate_of_second_number_to_display),
			get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, 0));
	cairo_line_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(human_x_coordinate_of_second_number_to_display),
			get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, -SHORT_LINE_LENGTH));

	cairo_move_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(human_x_coordinate_of_second_number_to_display + 5),
			get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, -SHORT_LINE_LENGTH * 2));
	sprintf(str, "%d", second_number_to_display);
	cairo_show_text(cr, str);

	human_x_coordinate_of_second_number_to_display -= PIX_COUNT_OF_ONE_S;
	second_number_to_display--;
}

/**********************************************************************************************************/
/**********************************************************************************************************/
/************************************************* y ******************************************************/

cairo_move_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(0),
		get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, -SHORT_LINE_LENGTH));
cairo_line_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(0),
		get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, human_max_y_coordinate));

int number;
double step_y;
//	double x1 = -DRAW_AREA_X_MARGIN;
double x = 0;
double y;
step_y = human_max_y_coordinate / (g_ruler.step_total + 1);

int i;
for (i = 0; i <= g_ruler.step_total; i++)
{
	cairo_move_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(x),
			get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, step_y * i));
	cairo_line_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(-SHORT_LINE_LENGTH),
			get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, step_y * i));

	cairo_move_to(cr, get_x_cairo_coordinate_from_x_human_pix_value(-DRAW_AREA_X_MARGIN + 2),
			get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate, step_y * i - 4));
	sprintf(str, "%d", g_ruler.step_number * i);
	cairo_show_text(cr, str);

}

cairo_stroke(cr);

return 0;
}

int draw_line(can_node_real_time_t * p)
{

static int path_data_size = sizeof(cairo_path_data_t);
//	cairo_translate(p->cr, -x_cairo_translate, 0);
/**********************************************************************************************************/
/*********************************************** draw line ************************************************/
//	void cairo_append_path (cairo_t *cr, const cairo_path_t	*path);
int t1, t2;

if (p->cairo_data_ar_total_point_data == 0)
{
	/* head and tail should have the same position */
#ifdef PRINT
	//			printf("draw_draw_area: compute: real_time_ar[%d].draw_source.total == 0,  head - tail = %d \n", i,
	//					p->draw_source.head - p->draw_source.tail);
#endif
	return 0;
}

if (p->cairo_data_ar_head_header_index <= p->cairo_data_ar_tail_header_index)
{
	// tail_point_index = (p->cairo_data_ar_tail_header_index + 1)
	t1 = (p->cairo_data_ar_tail_header_index + 1) - p->cairo_data_ar_head_header_index + 1;
	memcpy(p->cairo_data_ar2, p->cairo_data_ar + p->cairo_data_ar_head_header_index, path_data_size * t1);
}
else	// if(header_index > tail_index)
{
	t1 = (p->cairo_data_ar_length - 1) - p->cairo_data_ar_head_header_index + 1;
	memcpy(p->cairo_data_ar2, p->cairo_data_ar + p->cairo_data_ar_head_header_index, path_data_size * t1);

	// tail_point_index = (p->cairo_data_ar_tail_header_index + 1)
	t2 = (p->cairo_data_ar_tail_header_index + 1) - 0 + 1;
	memcpy(p->cairo_data_ar2 + t1, p->cairo_data_ar, path_data_size * t2);
}

//	cairo_translate(p->cr, -x_cairo_translate, 0);

p->cairo_path.status = CAIRO_STATUS_SUCCESS;
p->cairo_path.data = p->cairo_data_ar2;
p->cairo_path.num_data = p->cairo_data_ar_total_point_data * 2;
cairo_append_path(p->cr, &p->cairo_path);
cairo_stroke(p->cr);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				int k;
//				for (k = 0; k < p->cairo_data_ar_length; k += 2)
//				{
//					printf("draw_draw_area: cairo_data_ar2: total: %d, k: %d, type: %d, length: %d, x: %f, y: %f \n",
//							p->cairo_data_ar_total_point_data,
//							k,
//							p->cairo_data_ar2[k].header.type,
//							p->cairo_data_ar2[k].header.length,
//							p->cairo_data_ar2[k + 1].point.x,
//							p->cairo_data_ar2[k + 1].point.y);
//				}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

return 0;
}

int draw_line2(can_node_real_time_t * p)
{

static int path_data_size = sizeof(cairo_path_data_t);
//	cairo_translate(p->cr, -x_cairo_translate, 0);
/**********************************************************************************************************/
/*********************************************** draw line ************************************************/
//	void cairo_append_path (cairo_t *cr, const cairo_path_t	*path);
int t1, t2;

if (p->cairo_data_ar_total_point_data == 0)
{
	/* head and tail should have the same position */
#ifdef PRINT
	//			printf("draw_draw_area: compute: real_time_ar[%d].draw_source.total == 0,  head - tail = %d \n", i,
	//					p->draw_source.head - p->draw_source.tail);
#endif
	return 0;
}

if (p->cairo_data_ar_head_header_index <= p->cairo_data_ar_tail_header_index)
{
	// tail_point_index = (p->cairo_data_ar_tail_header_index + 1)
	t1 = (p->cairo_data_ar_tail_header_index + 1) - p->cairo_data_ar_head_header_index + 1;
	memcpy(p->cairo_data_ar2, p->cairo_data_ar + p->cairo_data_ar_head_header_index, path_data_size * t1);
}
else	// if(header_index > tail_index)
{
	t1 = (p->cairo_data_ar_length - 1) - p->cairo_data_ar_head_header_index + 1;
	memcpy(p->cairo_data_ar2, p->cairo_data_ar + p->cairo_data_ar_head_header_index, path_data_size * t1);

	// tail_point_index = (p->cairo_data_ar_tail_header_index + 1)
	t2 = (p->cairo_data_ar_tail_header_index + 1) - 0 + 1;
	memcpy(p->cairo_data_ar2 + t1, p->cairo_data_ar, path_data_size * t2);
}

int k = 0;
cairo_move_to(p->cr, p->cairo_data_ar2[k + 1].point.x, p->cairo_data_ar2[k + 1].point.y);
while (1)
{
	if (k == p->cairo_data_ar_total_point_data * 2 - 2)
	{
		break;
	}
	k += 2;

	cairo_line_to(p->cr, p->cairo_data_ar2[k + 1].point.x, p->cairo_data_ar2[k + 1].point.y);
	cairo_move_to(p->cr, p->cairo_data_ar2[k + 1].point.x, p->cairo_data_ar2[k + 1].point.y);
}
cairo_stroke(p->cr);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				int k;
//				for (k = 0; k < p->cairo_data_ar_length; k += 2)
//				{
//					printf("draw_draw_area: cairo_data_ar2: total: %d, k: %d, type: %d, length: %d, x: %f, y: %f \n",
//							p->cairo_data_ar_total_point_data,
//							k,
//							p->cairo_data_ar2[k].header.type,
//							p->cairo_data_ar2[k].header.length,
//							p->cairo_data_ar2[k + 1].point.x,
//							p->cairo_data_ar2[k + 1].point.y);
//				}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

return 0;
}

inline double get_x_cairo_coordinate_from_x_human_pix_value(double human_x_coordinate)
{
//	draw_area_width;
return DRAW_AREA_X_MARGIN + human_x_coordinate;
}

inline double get_y_human_pix_value_from_y_real_value_with_y_ruler(double y_value, y_ruler_t * p_ruler)
{
if (p_ruler == NULL)
{
	printf("p_ruler == NULL \n");
	return y_value;
}
else
{
	return y_value * p_ruler->pixel_count_of_single_integer;
}
}

inline double get_y_cairo_coordinate_from_y_human_pix_value(double human_max_y_coordinate_of_draw_area,
	double human_y_coordinate)
{
return human_max_y_coordinate_of_draw_area - human_y_coordinate;
}

void real_time_window_destroy(GtkWidget*widget, void ** data)
{
printf("real_time_window_destroy start. \n");
//	printf("widget: %p, data: %p \n", widget, data);
//	printf("real_time_window: %p, &real_time_window: %p \n", real_time_window, &real_time_window);

g_source_remove(draw_real_time_draw_line_func_id);
printf("real_time_window_destroy 0.1 \n");
can_read_func_should_exit = 1;
while (can_read_func_should_exit == 1)
{
	printf("real_time_window_destroy 0.2 \n");
	sleep(1);
	continue;
}
pthread_mutex_destroy(&real_time_draw_cache_mutex);
printf("real_time_window_destroy 1 \n");

int i, k;
int str_length = STRING_LENGTH * 5;
char str[str_length];
//	char **result;
//	int row, col;
char *errmsg;

struct timeval stop_time;
gettimeofday(&stop_time, NULL);

/********************* update table_statis-stop_time ***************************/
//	ALTER DATABASE database_name MODIFY NAME=new_database_name
sprintf(str, "UPDATE db_statis SET stop_second=%ld, stop_millisecond = %ld WHERE start_second=%ld", stop_time.tv_sec,
		stop_time.tv_usec, start_time.tv_sec);

if (SQLITE_OK != sqlite3_exec(sqlite3_db_statis, str, NULL, NULL, &errmsg))
{
	printf("error %s. errmsg: %s \n", str, errmsg);
	sqlite3_free(errmsg);
	exit(1);
}

//	if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, "PRAGMA synchronous=FULL;", 0, 0, &errmsg))
//	{
//		printf("SQLITE_OK != sqlite3_exec(db_name, \"PRAGMA synchronous=FULL;\", 0, 0, 0) \n");
//		exit(1);
//	}

dialog_of_user_db_name(NULL, widget);

/* rename database */
if (strlen(user_part_of_new_db_name) != 0)
{
	char new_db_name[str_length];		// new_db_name = time_part_of_new_db_name + user_part_of_new_db_name

	strncpy(new_db_name, time_part_of_new_db_name, STRING_LENGTH);
	new_db_name[sizeof(new_db_name) - 1] = '0';

	strncpy(new_db_name + strlen(new_db_name), "---", sizeof(new_db_name) - strlen(new_db_name) - 1);
	new_db_name[sizeof(new_db_name) - 1] = '0';

	strncpy(new_db_name + strlen(new_db_name), user_part_of_new_db_name, sizeof(new_db_name) - strlen(new_db_name) - 1);
	new_db_name[sizeof(new_db_name) - 1] = '0';

	/* rename database in folder of db_path
	 * use strncpy() is better
	 */
	sprintf(str, "mv ");
	sprintf(str + strlen(str), g_db_path);
	sprintf(str + strlen(str), time_part_of_new_db_name);

	sprintf(str + strlen(str), "  ");
	sprintf(str + strlen(str), g_db_path);
	sprintf(str + strlen(str), new_db_name);

	puts(str);
	int sys = system(str);
	printf("system(str) returned %d \n", sys);

	/* rename database in db_static.table_name_static */
	update_db_name(time_part_of_new_db_name, new_db_name);
}
printf("real_time_window_destroy 2 \n");

can_node_real_time_t * p;
for (i = 0; i < can_frame_real_time_ar_length; i++)
{
	if (can_frame_real_time_ar[i].frame_choosed == FALSE)
	{
		continue;
	}

	for (k = 0; k < can_frame_real_time_ar[i].total_node; k++)
	{
		p = can_frame_real_time_ar[i].can_node_real_time_ar + k;
		if (p->node_choosed == FALSE)
		{
			continue;
		}

		cairo_destroy(p->cr);
		p->cr = NULL;

		if (p->cairo_data_ar != NULL)
		{
			free(p->cairo_data_ar);
			p->cairo_data_ar = NULL;
		}
		if (p->cairo_data_ar2 != NULL)
		{
			free(p->cairo_data_ar2);
			p->cairo_data_ar2 = NULL;
		}
//		if (real_time_ar[i].frame_received_total == 0)
//		{
//			/****************************** get start time *********************************/
//			sprintf(str, "DROP TABLE * from db_name_table where db_name=%s", history_db.db_name);
//
//			if (SQLITE_OK != sqlite3_exec(sqlite3_db_statis, str, NULL, NULL, &errmsg))
//			{
//				printf("select * from db_name_table where db_name=%s fail. row = %d, %s \n", history_db.db_name, row,
//						errmsg);
//				sqlite3_free(errmsg);
//				exit(1);
//			}
//		}

	}
}

sqlite3_close(sqlite3_real_time);
sqlite3_real_time = NULL;

gtk_widget_set_sensitive(GTK_WIDGET(main_window), TRUE);
gtk_widget_destroyed(widget, (GtkWidget**) data);		// also destroy all child_widget; set *pointer = NULL;

printf("real_time_window_destroy returned. \n");
return;
}

void exit_button_clicked(GtkWidget * widget, void ** data)
{
printf("exit_button_clicked 1. \n");
//	printf("*data: %p, data: %p \n", *data, data);
//	printf("real_time_window: %p, &real_time_window: %p \n", real_time_window, &real_time_window);

//	real_time_window_destroy(*data, data);	// real_time_window
//	real_time_window_destroy(real_time_window, (void**)&real_time_window);

//	g_signal_emit_by_name(real_time_window, "destroy");
//	g_signal_emit_by_name(real_time_window, "delete-event");
//	g_signal_emit_by_name(main_window, "delete-event");	// "destroy"
printf("exit_button_clicked 2. \n");

gtk_widget_destroy(*data);	// ......... gtk_widget_destroy(widget) == send destroy signal to widget ???
printf("exit_button_clicked return. \n");
return; // TRUE;
}

void dialog_of_user_db_name(GtkButton *button, gpointer user_data)
{

//	real_time_window;
char str[STRING_LENGTH * 2];
sprintf(str, "数据已默认保存在： %s", time_part_of_new_db_name);

GtkWidget * dialog = gtk_dialog_new_with_buttons(str, GTK_WINDOW(user_data),
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, _("_OK"), GTK_RESPONSE_OK,
//			"Cancel",
//			GTK_RESPONSE_CANCEL,
		NULL);
gtk_widget_set_size_request(dialog, 600, 300);

GtkWidget * content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);
gtk_box_pack_start(GTK_BOX(content_area), vbox, TRUE, TRUE, 0);

sprintf(str, "可以为默认的数据库名添加后缀,只能包含字母和数字：");
GtkWidget * label = gtk_label_new(str);
gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);
GtkWidget * entry = gtk_entry_new();
gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, TRUE, 0);

gtk_widget_show_all(vbox);
int response = gtk_dialog_run(GTK_DIALOG(dialog));

//	user_part_of_new_db_name[STRING_LENGTH];
if (response == GTK_RESPONSE_OK)
{
	strncpy(user_part_of_new_db_name, gtk_entry_get_text(GTK_ENTRY(entry)), STRING_LENGTH);
}

gtk_widget_destroy(dialog);
}

int draw_test(cairo_t *cr)
{
//	cairo_path_t *	cairo_copy_path (cairo_t *cr);
//	void cairo_append_path (cairo_t *cr, const cairo_path_t	*path);

cairo_set_source_rgba(cr, 0, 0, 1, 1);
cairo_set_line_width(cr, 5);
cairo_rectangle(cr, 3, 3, 100, 100);
cairo_stroke(cr);

/* draw circle */
cairo_set_source_rgba(cr, 0.17, 0.63, 0.12, 1);
cairo_set_line_width(cr, 7);
cairo_arc(cr, 53, 53, 43, 0, 2 * G_PI);
cairo_stroke(cr);

/***************************/

cairo_set_source_rgba(cr, 0.6, 0.6, 0.6, 1);
cairo_rectangle(cr, 0, 0, 50, 50);
cairo_stroke_preserve(cr);
cairo_set_source_rgba(cr, 1, 1, 1, 1);
cairo_fill(cr);

cairo_translate(cr, 100, 0);

cairo_set_source_rgba(cr, 0.6, 0.6, 0.6, 1);
cairo_rectangle(cr, 0, 0, 50, 50);
cairo_stroke_preserve(cr);
cairo_set_source_rgba(cr, 1, 1, 1, 1);
cairo_fill(cr);

int i;
int a = 20000;
//	cairo_path_data_t data_header;
//	cairo_path_data_t data_ar[10];
//	cairo_path_data_t *cairo_data_ar2;
cairo_path_t *path;

cairo_translate(cr, 100 - a, 0);
cairo_set_source_rgba(cr, 0, 1, 1, 1);
cairo_set_line_width(cr, 5);

//	cairo_move_to(cr, 100, 100);
cairo_line_to(cr, 100 + a, 100);
cairo_line_to(cr, 200 + a, 100);
cairo_line_to(cr, 300 + a, 200);
cairo_line_to(cr, 300 + a, 300);
cairo_stroke_preserve(cr);
//	cairo_stroke(cr);

path = cairo_copy_path(cr);

printf("cairo_path.status: %d, cairo_path.num_data: %d \n", path->status, path->num_data);
for (i = 0; i < path->num_data; i += 2)
{
	printf("type: %d, length: %d, x: %f, y: %f  \n", path->data[i].header.type, path->data[i].header.length,
			path->data[i + 1].point.x, path->data[i + 1].point.y);
}
printf("**********************************************\n");

return 0;
}

/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/*********** draw x_axis ******************/
//			struct timespec lt;
//			lt.tv_nsec = 2;
//
//			/*
//			 * 微秒 microsecond
//			 *
//			 *
//			 */
//			struct timeval ms;
//			//get start time
//			gettimeofday(&ms, NULL);
//			printf("draw_draw_area: time(0) = %ld, ms.tv_sec = %ld, ms.tv_usec = %ld \n", time(0), ms.tv_sec, ms.tv_usec);
//
//			struct tm* ptm = localtime(0);
//	cairo_paint(cr);
//	cairo_destroy(cr);
//	 cairo_scale();
//	cairo_mask_surface(cr, surface, 10, 10);
//	cairo_arc(cr, 150, 210, 20, 0, 2 * G_PI);
//	cairo_stroke_preserve(cr);
//	gdk_window_invalidate_rect();
//	void g_signal_emit_by_name (gponter instance, const gchar *signal_name, ...);
//	cairo_translate (cr, 0, height);
//		cairo_set_source_rgba(cr,
//		((double) (random() % 1000)) / 1000,
//		((double) (random() % 1000)) / 1000,
//		((double) (random() % 1000)) / 1000, 1);
/* draw horizontal line */
//else
//{
//	static cairo_t *cr = NULL;
//	static cairo_t *cr2 = NULL;
//
//	if (draw_back_ground_times_record != draw_back_ground_times)
//	{
//		cairo_destroy(cr);
//		cr = gdk_cairo_create(gtk_widget_get_window((GtkWidget*) draw_area));
//		width = gtk_widget_get_allocated_width((GtkWidget*) draw_area);
//		height = gtk_widget_get_allocated_height((GtkWidget*) draw_area);
//
//		draw_back_ground_times_record = draw_back_ground_times;
//		printf("draw_cb,   draw_back_ground_times_record  = %d  \n",
//				draw_back_ground_times_record);
//
//		cairo_destroy(cr2);
//		cr2 = gdk_cairo_create(gtk_widget_get_window((GtkWidget*) draw_area));
//		cairo_set_source_rgba(cr2, 0, 0, 1, 1);
//		cairo_set_line_width(cr2, 1);
//		cairo_move_to(cr2, 500, 300);
//	}
//	printf("draw_draw_area   cr = %p  \n", cr);
//
//	int x = 4;
//
//	if (x == 1)
//	{
//		static int a = 0;
//		static int b = 0;
//		cairo_move_to(cr, a, b);
//		cairo_move_to(cr, 0, 0);
//		a = random() % width;
//		b = random() % height;
//			cairo_set_source_rgba(cr, 1, 1, 1, 0.5);
//			cairo_set_source_rgba(cr, ((double) (random() % 1000)) / 1000, ((double) (random() % 1000)) / 1000, ((double) (random() % 1000)) / 1000, 1);
//
//			cairo_set_line_width(cr, 1);
//			cairo_line_to(cr, a, b);
//			cairo_stroke(cr);
//		cairo_set_source_rgba(cr2, ((double) (random() % 1000)) / 1000,
//				((double) (random() % 1000)) / 1000, ((double) (random() % 1000)) / 1000, 1);
//
//		cairo_move_to(cr2, 500, 300);
//		cairo_line_to(cr2, a, b);
//		cairo_stroke(cr2);
//			cairo_stroke_preserve(cr2);
//			cairo_translate(cr, 150, 100);
//			cairo_rotate(cr2, M_PI / 2);
//			cairo_stroke(cr2);
//		gdk_window_process_all_updates();
//		sleep(1);
//		cairo_set_source_rgb(cr2, 0.6, 0.6, 0.6);
//		cairo_rectangle(cr2, a, b, 80, 50);
//		cairo_stroke_preserve(cr2);
//		cairo_set_source_rgb(cr2, 1, 1, 1);
//		cairo_fill(cr2);
//
//		cairo_translate(cr2, 150, 100);
//		cairo_rotate(cr2, M_PI / 2);
//
//		cairo_set_source_rgb(cr2, 0, 0, 0.6);
//		cairo_rectangle(cr2, a, b, 80, 50);
//		cairo_stroke_preserve(cr2);
//		cairo_set_source_rgb(cr2, 1, 1, 1);
//		cairo_fill(cr2);
//
//		printf("draw_cb 2  \n");
//
//	}
//	else
//		if (x == 2)
//		{
//			static int a = 500;	      		//width / 2;
//			static int b = 400;  		   	// height / 2;
//			cairo_move_to(cr, a, b);
//
//			switch (random() % 3)
//			{
//			case 0:
//				break;
//
//			case 1:
//				a--;
//				if (a == 0)
//				{
//					a++;
//				}
//				break;
//
//			case 2:
//				a++;
//				if (a == width)
//				{
//					a--;
//				}
//				break;
//			}
//
//			switch (random() % 3)
//			{
//			case 0:
//				break;
//
//			case 1:
//				b--;
//				if (b == 0)
//				{
//					b++;
//				}
//				break;
//
//			case 2:
//				b++;
//				if (b == height)
//				{
//					b--;
//				}
//				break;
//			}
//
//			cairo_set_source_rgba(cr, 1, 1, 1, 1);
//			cairo_set_line_width(cr, 1);
//			cairo_line_to(cr, a, b);
//			cairo_stroke(cr);
//
//			//		gdk_window_process_all_updates();
//			//		sleep(1);
//			printf("draw_cb 3  \n");
//		}
//		else
//			if (x == 3)
//			{
//
//				cairo_move_to(cr, 500, 300);
//				cairo_set_source_rgba(cr, 1, 1, 1, 1);
//				cairo_set_line_width(cr, 0.5);
//				cairo_translate(cr, width / 2, height / 2);
//				cairo_arc(cr, 0, 0, 120, 0, 2 * M_PI);
//				cairo_stroke(cr);
//
//				/******************************/
//
//				cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL,
//						CAIRO_FONT_WEIGHT_BOLD);
//				cairo_set_font_size(cr, 33);
//				cairo_show_text(cr, "If there's a soulmate for everyone");
//				cairo_text_path(cr, "Hello");	// 空心字
//				cairo_stroke(cr);
//				/******************************/
//
//				int i;
//
//				cairo_save(cr);
//				for (i = 0; i < 36; i++)
//				{
//					cairo_rotate(cr, i * M_PI / 36);  // 旋转
//					cairo_scale(cr, 0.3, 1);		// 缩放
//					cairo_arc(cr, 0, 0, 120, 0, 2 * M_PI); // hua yuan
//					cairo_restore(cr);
//					cairo_stroke(cr);
//					cairo_save(cr);
//				}
//
////			cairo_destroy(cr);
////			return FALSE;
//			}
//			else
//				if (x == 4)
//				{
//					static int add = -10;
//					cairo_set_source_rgba(cr, 0, 0, 0, 1);
//					cairo_paint(cr);
//					static int step = 700;
//					cairo_set_source_rgba(cr, 1, 1, 1, 1);
//					cairo_set_line_width(cr, 5);
//					cairo_move_to(cr, step, 300);
//					cairo_line_to(cr, step, 500);
//					cairo_stroke(cr);
//
//					step += add;
//					if (step < 0)
//					{
//						add = 10;
//					}
//					else
//						if (step > 700)
//						{
//							add = -10;
//						}
//				}
//
//}
//
//int bbbb(int argc, char *argv[])
//{
//	gtk_init(&argc, &argv);
//
//	GtkWidget *window;
//
//	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
//
//	b_da = gtk_drawing_area_new();
//	gtk_widget_set_size_request(b_da, WINDOW_WIDTH, WINDOW_HEIGHT);
//	gtk_container_add(GTK_CONTAINER(window), b_da);
//	g_signal_connect(b_da, "draw", G_CALLBACK(draw_back_ground), NULL);
//
//	pthread_t bbbbb_pid;
//	pthread_create(&bbbbb_pid, NULL, bbbbb, (void*) b_da);
//
////	gtk_widget_set_app_paintable(window, TRUE);
//	gtk_widget_show_all(window);
//	gtk_main();
//	return 0;
//
////	g_signal_stop_emission_by_name(da, "draw");
////	g_signal_connect(da, "draw", G_CALLBACK(draw_back_ground), NULL);
//}
//int g_io_channer(void)
//{
//	io_channel = g_io_channel_unix_new(canr_sockfd);
//	g_io_channel_set_encoding(io_channel, NULL, &err);
//
//	/*
//	 guint g_io_add_watch (GIOChannel *channel, GIOCondition condition, GIOFunc func, gpointer user_data);
//	 其中，GIOCondition包括G_IO_IN, G_IO_OUT, G_IO_PRI,G_IO_ERR, G_IO_HUP, G_IO_NVAL。
//	 可以通过对它们的或运算来同时指定多个事件，当然回调函数应该判断是哪个的事件引起回调。
//	 */
//	g_io_add_watch(io_channel, G_IO_IN | G_IO_ERR, gtk_can_read, NULL);
//
//	return 0;
//}
//gtk_input_add_full();
//	gint gdk_input_add(gint source,
//					GdkInputCondition condition,
//					GdkInputFunction function,
//					gpointer data);
/* 警告：不建议使用‘gtk_hscrollbar_new’ ：Use 'gtk_scrollbar_new' instead [-Wdeprecated-declarations] */
//		GtkWidget * hscrollbar = gtk_hscrollbar_new(NULL);		// ........................................
//		GtkWidget * hscrollbar = gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL, NULL);  // !!!!!!!!!!!
//		gtk_container_add(GTK_CONTAINER(draw_vbox), hscrollbar);  // ..............................
//		gtk_box_pack_start(GTK_BOX(draw_vbox), hscrollbar, TRUE, TRUE, padding);
//		GtkWidget * line_label1 = gtk_label_new("line_label1");
//		gtk_container_add(GTK_CONTAINER(line_vbox), line_label1);
//		GtkWidget * label_hbox2 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
//		gtk_box_pack_start(GTK_BOX(main_vbox), label_hbox2, TRUE, TRUE, padding);
//
//		for(i = 0; i < ITEM_COUNT_OF_ONE_HBOX; i++)
//		{
//			display_source.item_ar2[i].label = gtk_label_new(item_label_names2[i]);
//			gtk_box_pack_start(GTK_BOX(label_hbox2), display_source.item_ar2[i].label, TRUE, TRUE, padding);
//			display_source.item_ar2[i].entry = gtk_entry_new();
//			gtk_box_pack_start(GTK_BOX(label_hbox2), display_source.item_ar2[i].entry, TRUE, TRUE, padding);
//		}
//GtkWidget * label_hbox1 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
//gtk_box_pack_start(GTK_BOX(main_vbox), label_hbox1, TRUE, TRUE, padding)
//	cairo_surface_t *surface = gdk_window_create_similar_surface(
//			gtk_widget_get_window(draw_area),
//			CAIRO_CONTENT_COLOR, /* CAIRO_CONTENT_COLOR_ALPHA */
//			gtk_widget_get_allocated_width(draw_area),
//			gtk_widget_get_allocated_height(draw_area));
//
//	cairo_t *cr = cairo_create(surface);
////			cairo_set_source_surface(cr, surface, 0, 0);
//	cairo_set_source_rgb(cr, 0, 0, 1);
//	cairo_paint(cr);
////			gdk_window_process_all_updates();
//		void gtk_scrolled_window_add_with_viewport( GtkScrolledWindow *scrolled_window, GtkWidget *child);
//	int can_sockfd;
//	GError **err;
//	GIOChannel* io_channel;
//	int source_id;
//
//	can_sockfd = can_get_sockfd();
//	io_channel = g_io_channel_unix_new(can_sockfd);
//	g_io_channel_set_encoding(io_channel, NULL, &err);
//
//	/*
//	 guint g_io_add_watch (GIOChannel *channel, GIOCondition condition, GIOFunc func, gpointer user_data);
//	 其中，GIOCondition包括G_IO_IN, G_IO_OUT, G_IO_PRI,G_IO_ERR, G_IO_HUP, G_IO_NVAL。
//	 可以通过对它们的或运算来同时指定多个事件，当然回调函数应该判断是哪个的事件引起回调。
//	 */
//	source_id = g_io_add_watch(io_channel, G_IO_IN | G_IO_ERR, gtk_can_read_and_more, &can_sockfd);
//{ /* make link */
//		p->draw_source.head = (struct in_frame_data_t *) malloc(
//				sizeof(struct in_frame_data_node_t));
//
//		p = &p->draw_source.head_node->next;
//
//		for (k = 1; k < p->draw_source_array_length; k++)
//		{
//			*p = (struct in_frame_data_node_t *) malloc(sizeof(struct in_frame_data_node_t));
//			if (*p == NULL)
//			{
//				printf("*p == NULL \n");
//				return -1;
//			}
//			p = &(*p)->next;
//		}
//		*p = p->draw_source.head_node;		// make a circle
//		p->draw_source.tail_node = p->draw_source.head_node;
//		p->draw_source.total_node = 0;
//}
