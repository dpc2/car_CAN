//
//
#include <can_real_time.h>
#include <can_standard.h>
#include "includes.h"

volatile int can_sockfd;

extern pthread_mutex_t real_time_draw_cache_mutex;

extern sqlite3 * sqlite3_real_time;

extern struct timeval start_time;
extern volatile int can_read_func_should_exit;
extern double draw_area_width;
//extern double human_max_x_coordinate;
extern double human_max_y_coordinate;

extern can_frame_real_time_t can_frame_real_time_ar[];
extern int can_frame_real_time_ar_length;

extern can_frame_t can_frame_ar[];
extern int can_frame_ar_length;

//u32	MASK32 = 0XFFFFFFFF;
#define	MASK32	(0XFFFFFFFF)
u32 bit_mask[32] = 	// 1, 3, 7, 15......(2^n - 1), compute MASK32t runtime
		{
		MASK32 >> 31, MASK32 >> 30,
		MASK32 >> 29, MASK32 >> 28, MASK32 >> 27, MASK32 >> 26, MASK32 >> 25,
		MASK32 >> 24, MASK32 >> 23, MASK32 >> 22, MASK32 >> 21, MASK32 >> 20,
		MASK32 >> 19, MASK32 >> 18, MASK32 >> 17, MASK32 >> 16, MASK32 >> 15,
		MASK32 >> 14, MASK32 >> 13, MASK32 >> 12, MASK32 >> 11, MASK32 >> 10,
		MASK32 >> 9, MASK32 >> 8, MASK32 >> 7, MASK32 >> 6, MASK32 >> 5,
		MASK32 >> 4, MASK32 >> 3, MASK32 >> 2, MASK32 >> 1, MASK32 >> 0};

// ip link show
// ifconfig -a
// ip link set can0   type can     tq 500     prop-seg 6      phase-seg1 7      phase-seg2 2      sjw 1
//// ip link set can0 type can     tq 500     prop-seg 6      phase-seg1 7      phase-seg2 2      sjw 1

// ip link set can0 type can bitrate 512000  // 12500
// ip -details link show can0
// ifconfig can0 up
// ip -details -stat link show can0      // statistics

//> ip link set can0 up type can bitrate 125000:
//> ip: either "dev" is duplicate, or "type" is garbage
//This is because of your "ip" is too old and does not support CAN devices setup.
//http://git.kernel.org/?p=network/iproute2/iproute2.git;a=summary
//Get & build the latest "ip" from
//git://git.kernel.org/pub/scm/network/iproute2/iproute2.git

//> ifconfig can0 up:
//> ifconfig: SIOCGIFFLAGS: No such device
//Please check for CAN driver stuff in the kernel log (e.g. with dmesg)
//search for "CAN device driver interface"
/*********************************************************************************/
/* controller area network (CAN) kernel definitions */

///* special address description flags for the CAN_ID */
//#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
//#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
//#define CAN_ERR_FLAG 0x20000000U /* error real_time frame */
//
///* valid bits in CAN ID for frame formats */
//#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
//#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
//#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */
/*********************************************************************************/
/*
 * Controller Area Network Identifier structure
 *
 * bit 0-28	: CAN identifier (11/29 bit)
 * bit 29	: error real_time frame flag (0 = data frame, 1 = error real_time)
 * bit 30	: remote transmission request flag (1 = rtr frame)
 * bit 31	: frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
 */
//
//void can_write(void)
//{
//
//	AF_CAN;
//
//	int i;
//	int id_count = 5;
//	int canw_sockfd, nbytes;
//	struct sockaddr_can addr;
//	struct ifreq ifr;
//	struct can_frame out_frame[id_count];
//
//	PF_CAN, SOCK_RAW, CAN_RAW;
//	canw_sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
//
//	printf("canw_sockfd = %d \n", canw_sockfd);
//	if(canw_sockfd < 0)
//	{
//		printf("errno: %d \n", errno);
//		perror("err: ");
//		exit(-1);
//	}
//
//	strcpy(ifr.ifr_name, CAN_WR);
//	puts(ifr.ifr_name);
//	ioctl(canw_sockfd, SIOCGIFINDEX, &ifr);
//
//	addr.can_family = AF_CAN;
//	addr.can_ifindex = ifr.ifr_ifindex;
//	bind(canw_sockfd, (struct sockaddr *) &addr, sizeof(addr));
//
//	memset(&out_frame, 0, sizeof(out_frame));
//	for(i = 0; i < id_count; i++)
//	{
//		out_frame[i].can_id = i;  // 0XDE;
//		out_frame[i].can_id |= CAN_EFF_FLAG;		// make EFF frame
//		out_frame[i].can_dlc = 1;
//		out_frame[i].data[0] = i;
//	}
//
//	i = 0;
//	while (1)
//	{
//		nbytes = write(canw_sockfd, &out_frame[i], sizeof(struct can_frame));
//
//		if(nbytes > 0)
//		{
//			printf("send: ID = 0x%X = %u, DLC = %d, data = 0x%X \n", out_frame[i].can_id, out_frame[i].can_id,
//					out_frame[i].can_dlc, out_frame[i].data[0]);
//		}
//		else
//		{
//			printf("write fail.  \n");
//		}
//
////		lt2 = time(0);
////		if(lt2 - lt >= 1)
////		{
////			lt = lt2;
////			printf("send... \n");
////		}
//
//		i++;
//		if(i >= id_count)
//		{
//			i = 0;
//		}
//		sleep(2);
//	}
//}
int can_get_sockfd(void)
{
	struct sockaddr_can addr;
	struct ifreq ifr;

	can_sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if (can_sockfd < 0)
	{
		printf("************************************** errno: %d \n", errno);
		perror("err: \n");
		exit(1);
	}
	printf("************************************** canr_sockfd = %d \n", can_sockfd);

	strcpy(ifr.ifr_name, CAN_RD);
	ioctl(can_sockfd, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	int result = bind(can_sockfd, (struct sockaddr *) &addr, sizeof(addr));
	if (result != 0)
	{
		printf("************************************** bind() return %d , errno = %d \n", result, errno);
#ifdef TEST
		//
#else
		exit(1);
#endif
	}
	printf("************************************** bind() return %d , errno = %d \n", result, errno);

	EBADF;
	struct can_filter rfilter;

	/* set some can_id to receive data from them */
	/* if setsockopt() is not used, read() will receive all frame of all can_id
	 * if setsockopt() is used, read() will only receive frame of can_id in can_filter structure
	 * the can_id in can_filter just has business with bits 0~28
	 */

#ifndef TEST
	int i;
	for (i = 0; i < can_frame_ar_length; i++)
	{
		can_frame_ar[i].frame_choosed = can_frame_real_time_ar[i].frame_choosed;	// ........................
		if (can_frame_ar[i].frame_choosed == FALSE)
		{
			continue;
		}

		rfilter.can_id = can_frame_ar[i].id;
		rfilter.can_mask = CAN_EFF_MASK;
		setsockopt(can_sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
	}
#endif

//	setsockopt(can_sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);  // refuse all in_frame

//	puts(ifr.ifr_name);

	return can_sockfd;
}

void * read_data(void *arg)
{
	pthread_detach(pthread_self());

	int i;
	int nbytes;
	struct can_frame in_frame;
	int sum = 0;

	struct timeval timeout;
	fd_set read_fds;
	int result;

	sleep(2);
	int aa = 1;

	while (1)
	{
		if (can_read_func_should_exit == 1)
		{
			can_read_func_should_exit = 0;
			return 0;
		}

#ifdef TEST
		static int which_can_id = -1;
		if (++which_can_id >= can_frame_real_time_ar_length)
		{
			which_can_id = 0;
		}
		if (!can_frame_ar[which_can_id].frame_choosed)
		{
//			printf("can_read 1: can_frame_ar[%d].frame_choosed: %d, pf: %d \n", s, can_frame_ar[s].frame_choosed, can_frame_ar[s].pf);
			continue;
		}
		in_frame.can_id = can_frame_ar[which_can_id].id;
//		printf("can_read 2: can_frame_ar[%d].frame_choosed: %d, id: %d \n", s, can_frame_ar[s].frame_choosed, can_frame_ar[s].pf);

		usleep(FRAME_RECEIVE_TEST_USLEEP / (double) can_frame_real_time_ar_length);
		nbytes = sizeof(struct can_frame);
		static int v = 10;
		v += random() % 3 - 1;
		v = v > 1000 ? 1000 : v;
		v = v < 0 ? 0 : v;

		memset(in_frame.data, v, sizeof(in_frame.data));

#else
		while (1)
		{
			if (can_read_func_should_exit == 1)
			{
				can_read_func_should_exit = 0;
				return 0;
			}

			timeout.tv_sec = 2;
			timeout.tv_usec = 0;
			FD_ZERO(&read_fds);    //用select函数之前先把集合清零
			FD_SET(can_sockfd, &read_fds);    //把要检测的句柄——标准输入（0），加入到集合里。

			result = select(can_sockfd + 1, &read_fds, NULL, NULL, &timeout);
			if (result == 0)
			{
				printf("********************** select() timeout \n");
				continue;
				//				return -1;
			}
			else
				if (result == -1)
				{
					perror("select() select error. \n");
					continue;
//					return -1;
				}
				else
				{
					printf("select() return %d \n", result);
					break;
				}
		}

		nbytes = read(can_sockfd, &in_frame, sizeof(in_frame));
#endif

		if (nbytes > 0)
		{
			sum++;
#ifdef PRINT
			printf("************************************** received: ID = 0x%X = %u, DLC = %d, data = ",
					in_frame.can_id, in_frame.can_id, in_frame.can_dlc);
#endif

			for (i = 0; i < in_frame.can_dlc; i++)
			{
#ifdef PRINT
				printf("%02X, ", in_frame.data[i]);
#endif
			}
#ifdef PRINT
			printf("\n");
#endif

			if (deal_with_in_frame(&in_frame) < 0)
			{
				exit(1);
			}
		}
		else
		{
			printf("************************************** read fail. \n");
		}
	}

	can_read_func_should_exit = 0;
	return 0;
}

int deal_with_in_frame(struct can_frame * p_can_in_frame)
{
	static int i, k;
	static char * errmsg;
	static struct timeval lt;
	static can_node_t * p;
	static can_node_real_time_t * q;
	static double x;
	static double y_human_pix_value;
	static char sql[1000] =
	{0};
	static char sql2[1000] =
	{0};

	union
	{
		unsigned char a[8];
		unsigned long int b;
	} u;

	/********** check id received, find the name of this id **********/

	p_can_in_frame->can_id = p_can_in_frame->can_id & CAN_EFF_MASK;

	for (i = 0; i < can_frame_ar_length; i++)
	{
		if (p_can_in_frame->can_id == can_frame_ar[i].id)
		{
			break;
		}
	}
	if (i == can_frame_ar_length)
	{
		printf("deal_with_in_frame: error: i == can_frame_ar_length, received id: %X \n", p_can_in_frame->can_id);
		exit(1);
	}

	if (can_frame_ar[i].frame_choosed == false)
	{
		printf("deal_with_in_frame: error: can_frame_ar[%d].frame_choosed == FALSE,"
				"this id(%X) shoule have been filtered \n", i, can_frame_ar[i].id);
		return 0;
	}

	/********** life??? ***********/
	can_frame_ar[i].life++;
	can_frame_ar[i].frame_received_total++;

	/*****************************************************************************************************************/
	memcpy(u.a, p_can_in_frame->data, 8);

	gettimeofday(&lt, NULL);
	x = ((lt.tv_sec - start_time.tv_sec) * 1000 + (lt.tv_usec - start_time.tv_usec) / 1000) * PIX_COUNT_OF_ONE_MS
			+ draw_area_width;

	sprintf(sql, "INSERT INTO pf_%d (second, millisecond", can_frame_ar[i].pf);
	sprintf(sql2, "  VALUES (%ld, %d", lt.tv_sec, lt.tv_usec);

	for (k = 0; k < can_frame_ar[i].total_node; k++)
	{
		if (can_frame_real_time_ar[i].can_node_real_time_ar[k].node_choosed == false)
		{
			continue;
		}
		p = can_frame_ar[i].can_node_ar + k;
		q = can_frame_real_time_ar[i].can_node_real_time_ar + k;

		p->value = (u.b >> p->bit_posion_in_bytes) & bit_mask[p->bit_length - 1];
		p->value = (p->value - p->offset) * p->ratio;
		sprintf(sql + strlen(sql), ", %s", p->node_name);
		sprintf(sql2 + strlen(sql2), ", %lf", p->value);
		printf("node_name: %s, value: %02lf \n", p->node_name, p->value);

		q->cairo_path_data.point.x = x;

		q->this_value = p->value;
		y_human_pix_value = get_y_human_pix_value_from_y_real_value_with_y_ruler(q->this_value, &q->y_ruler);
		q->cairo_path_data.point.y = get_y_cairo_coordinate_from_y_human_pix_value(human_max_y_coordinate,
				y_human_pix_value);

		/************************** store into database ***********************************************/
//			static int a = 0;
//			a++;
//			printf("a: %d      ");
//			if (a % 200 == 0)
//			{
//				if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, "PRAGMA synchronous=OFF;", 0, 0, &errmsg))
//				{
//					printf("SQLITE_OK != sqlite3_exec(db_name, \"PRAGMA synchronous=OFF;\", 0, 0, 0) \n");
//					sqlite3_free(errmsg);
//					return -1;
//				}
//			}
//			else
//				if (a % 100 == 0)
//				{
//					if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, "PRAGMA synchronous=FULL;", 0, 0, &errmsg))
//					{
//						printf("SQLITE_OK != sqlite3_exec(db_name, \"PRAGMA synchronous=FULL;\", 0, 0, 0) \n");
//						sqlite3_free(errmsg);
//						return -1;
//					}
//				}
//
//			static struct timeval db_time1;
//			static struct timeval db_time2;
//			gettimeofday(&db_time1, NULL);
		/*sprintf(sql, "INSERT INTO pf_%d (second, millisecond)  VALUES (%ld, %d) ",
		 can_frame_ar[i].pf, lt.tv_sec, lt.tv_usec);
		 if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, sql, 0, 0, &errmsg))
		 {
		 if (NULL != errmsg)
		 {
		 printf("%s error. errmsg: %s \n", sql, errmsg);
		 beep_once(100);
		 sqlite3_free(errmsg);
		 }
		 }
		 */
		//	gettimeofday(&db_time2, NULL);
		//	printf("db_time2 -db_time1: %d \n",
		//			(db_time2.tv_sec - db_time1.tv_sec) * 1000 + (db_time2.tv_usec - db_time1.tv_usec) / 1000);
	}
	sprintf(sql + strlen(sql), ")");
	sprintf(sql2 + strlen(sql2), ")");
	sprintf(sql + strlen(sql), sql2);

	if (SQLITE_OK != sqlite3_exec(sqlite3_real_time, sql, 0, 0, &errmsg))
	{
		printf("%s error. errmsg: %s \n", sql, errmsg);
		beep_once(100);
		sqlite3_free(errmsg);
		exit(1);
	}
	else
	{
//		printf("\nYYY  %s. errmsg: %s \n", sql, errmsg);
	}
//	printf("deal_with_in_frame: point.x = %f , point.y == %f \n", cairo_path_data.point.x, cairo_path_data.point.y);

	/**************************** insert into entry_value and node_array ****************************/

	pthread_mutex_lock(&real_time_draw_cache_mutex);
	update_real_time_draw_source(i);
	pthread_mutex_unlock(&real_time_draw_cache_mutex);
//	printf("deal_with_in_frame: real_time_ar[%d].show_in_draw_area == TRUE \n", i);

//	p_read_total_node->in_frame_ar[p_read_total_node->in_frame_count].id = p_can_in_frame->can_id;
//	p_read_total_node->in_frame_ar[p_read_total_node->in_frame_count].value = p_can_in_frame->data;
//	p_read_total_node->in_frame_ar[p_read_total_node->in_frame_count].receive_time = time(0);
//	p_read_total_node->in_frame_count++;

//	p_read_total_node->in_frame_ar[p_read_total_node->in_frame_count].id = p_can_in_frame->can_id;
//	p_read_total_node->in_frame_ar[p_read_total_node->in_frame_count].value = 100;
//	p_read_total_node->in_frame_ar[p_read_total_node->in_frame_count].receive_time = time(0);

	printf("******************************** deal_with_in_frame return success. id: %X \n", p_can_in_frame->can_id);
	return 0;
}

int update_real_time_draw_source(int i)
{
//	printf("insert_into_draw_source: in.  \n");
	static int size = sizeof(cairo_path_data_t);
	static int k;
	static can_node_t * pn;
	static can_node_real_time_t * pnr;

	can_frame_t * pf = can_frame_ar + i;
	can_frame_real_time_t * pfr = can_frame_real_time_ar + i;

	for (k = 0; k < pfr->total_node; k++)
	{
		pn = pf->can_node_ar + k;
		pnr = pfr->can_node_real_time_ar + k;

		if (pnr->node_choosed == FALSE)
		{
			continue;
		}
		if (pnr->display_type == SHOW_AS_ENTRY)
		{
#ifdef TEST
			pnr->entry_value = (int)(pnr->this_value) % pn->errcode_text_count + 1;
#else
			if (pnr->this_value < 0 || pnr->this_value > pn->errcode_text_count)
			{
				printf("\n\n pnr->this_value(= %f) < 0 || pnr->this_value > pn->errcode_text_count \n\n",
						pnr->this_value);
				exit(1);
			}
			else
			{
				pnr->entry_value = pnr->this_value;
			}
#endif
		}
		else
			if (pnr->display_type == SHOW_AS_LINE)
			{

//		if (q->line_active == FALSE)
//		{
//			continue;
//		}

				/*
				 if (q->cairo_data_ar_total_point_data >= q->cairo_data_ar_length / 2)
				 {
				 printf("XXXXXXX update_real_time_draw_source: total_node == %d >= array_length / 2, node will lost to draw \n",
				 q->cairo_data_ar_total_point_data);
				 beep_once(1000);exit(1);
				 return -1;
				 }
				 */

//		if (q->cairo_data_ar_total_point_data == 0)
//		{
//			/* head and tail should have the same position, head == tail */
//			printf("update_real_time_draw_source: pf: %d, node_name: %s, "
//					"total_point_data: %d, head_index: %d, tail_index: %d \n",
//					p->pf,
//					q->node_name,
//					q->cairo_data_ar_total_point_data,
//					q->cairo_data_ar_head_header_index,
//					q->cairo_data_ar_tail_header_index);
//			q->cairo_data_ar_head_header_index = 0;
//			q->cairo_data_ar_tail_header_index = 0;
//		}
				pnr->cairo_data_ar_tail_header_index += 2;
				if (pnr->cairo_data_ar_tail_header_index >= pnr->cairo_data_ar_length)
				{
					pnr->cairo_data_ar_tail_header_index = 0;
				}

//	memcpy(p_real_time->cairo_data_ar + p_real_time->cairo_data_ar_tail_header_index, &p_real_time->cairo_data_header, size);

				// tail_point_index = (p->cairo_data_ar_tail_header_index + 1)
				memcpy(pnr->cairo_data_ar + (pnr->cairo_data_ar_tail_header_index + 1), &pnr->cairo_path_data, size);
				pnr->cairo_data_ar_total_point_data++;
				if (pnr->cairo_data_ar_total_point_data > pnr->cairo_data_ar_length / 2)
				{
					pnr->cairo_data_ar_total_point_data = pnr->cairo_data_ar_length / 2;

					pnr->cairo_data_ar_head_header_index += 2;
					// q->cairo_data_ar_head_header_index -> max == q->cairo_data_ar_length - 2
					if (pnr->cairo_data_ar_head_header_index >= pnr->cairo_data_ar_length)
					{
						pnr->cairo_data_ar_head_header_index = 0;
					}
				}
//	printf("insert_into_frame_node_array: tail_index---x = %f , tail_index---y == %f \n",
//			p_real_time->cairo_data_ar[p_real_time->cairo_data_ar_tail_header_index].point.x,
//			p_real_time->cairo_data_ar[p_real_time->cairo_data_ar_tail_header_index].point.y);

#ifdef PRINT
				printf("update_real_time_draw_source: total_data = %d \n", pnr->cairo_data_ar_total_point_data);
#endif
//		printf("update_real_time_draw_source: pf: %d, pnr->data_name: %s, total_point_data = %d \n",
//				pfr->pf, pnr->data_name, pnr->cairo_data_ar_total_point_data);

			}
	}

	return 0;
}

int gtk_can_read_and_more(GIOChannel *io_channel, GIOCondition condition, gpointer data)
{
	int i;
	int nbytes;
	struct can_frame in_frame;
	int sum = 0;
	time_t lt;

	int can_sockfd = *((int *) data);

	if (condition == G_IO_ERR)
	{
		return -1;
	}

	if (condition == G_IO_IN)
	{
		lt = time(0);

		while (1)
		{
			nbytes = read(can_sockfd, &in_frame, sizeof(in_frame));

			if (nbytes < 0)			// no data
			{
				printf("************************************** read() < 0. \n");
				return -1;
			}
			else
				if (nbytes == 0)	// error
				{
					printf("************************************** read() == 0. \n");
					return -1;
				}
				else						//	if(nbytes > 0)
				{
					sum++;
					printf("************************************** received: ID = 0x%X = %u, DLC = %d, data = ",
							in_frame.can_id, in_frame.can_id, in_frame.can_dlc);

					for (i = 0; i < in_frame.can_dlc; i++)
					{
						printf("%02X, ", in_frame.data[i]);
					}
//				printf("sum = %d  seconds = %d  \n\n", sum, time(0) - start_time);

					/***********************************/

				}
		}
	}

	return 0;
}

