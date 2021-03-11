//
//

#pragma once

#include "includes.h"

#define	CAN_WR		"can0"
#define	CAN_RD		"can1"

#define	MAX_CAN_ID_COUNT							(20)
#define	MAX_NODE_COUNT_OF_ONE_CAN_ID				(16)
//#define	NODE_NAME_TO_USE							node_name_cn
#define	ERRCODE_MAX									(32)

#define	TEST

typedef struct
{
	/*********** read from configure ***********/
	char *node_name_en;
	char *node_name_cn;

	int bit_posion_in_bytes;
	int bit_length;
	double ratio;	// usually == 1
	int offset;		// usually == 0
	char *errcode_text[ERRCODE_MAX];

	int errcode_text_count;

	char *node_name;	// == node_name_en or node_name_cn
	bool node_choosed;
	double value;
/**** display configure ****/
//	rgba_t rgba;
//	double line_wigth;
//	y_ruler_t y_ruler;
} can_node_t;

typedef struct
{
	/*********** read from configure ***********/
	char * out;
	char * in;
	u32 id;
	int p;
	int pf;	// frame_id
	int ps;
	int sa;
	double cycle_time_in_ms;

	/*********** customer select ***********/
	bool frame_choosed;

	/**********************/
	int total_node;
	can_node_t can_node_ar[MAX_NODE_COUNT_OF_ONE_CAN_ID];

	volatile unsigned char life;
	double frame_count_every_ms;
	int frame_received_total;
} can_frame_t;





int can_get_sockfd(void);
void * can_write(void *);
void * read_data(void *);

int deal_with_in_frame(struct can_frame * p_can_in_frame);

