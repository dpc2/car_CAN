/*
 * real_time_conf.c
 *
 *  Created on: 2017年9月26日
 *      Author: root
 */

#include <can_real_time.h>
#include "includes.h"
#include "can_standard.h"

can_frame_real_time_t can_frame_real_time_ar[] =
{

/********** pf-0X61 ***********/
{.id = 0X0C61D0D1, .pf = 0X61, .total_node = 8, .can_node_real_time_ar =
{
{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},

{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},

{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

/********** pf-0X62 ***********/
{.id = 0X0C62D0D1, .pf = 0X62, .total_node = 6, .can_node_real_time_ar =
{
{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},

{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH}}},


///********** pf-0X63 ***********/
//{.id = 0X0C61D0D1, .pf = 0X63, .total_node = 31, .can_node_real_time_ar =
//{
//{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//
//{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//
//{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/

/********** pf-25 ***********/
{.id = 0X0C19A7F0, .pf = 25, .total_node = 12, .can_node_real_time_ar =
{
//				{"cmd_DM_target_torgue", 0, 16, 1, 32000},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DMC_U_limit", 16, 16, 0.1, 10000},		// voltage
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"cmd_DMC_I_limit", 32, 16, 0.1, 10000},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},

//				{"cmd_DM_stop", 48, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_drive", 49, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_brake", 50, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_backword", 51, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_forward", 52, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_mode", 53, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"null_bit", 54, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"command_DM_ready", 55, 1, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"EVCU_life", 56, 8, 1, 0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

/********** pf-8 ***********/
{.id = 0X0C08A7F0, .pf = 8, .total_node = 12, .can_node_real_time_ar =
{
//				{"DM_torgue",	0,		16,	1,		32000},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_BLUE, COMMON_LINE_WIDTH},
//				{"DM_speed",	16,	16,	0.5,	0},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_YELLO, COMMON_LINE_WIDTH},
//				{"DM_in_I",		32,	16,	0.1,	10000},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},

//				{"DM_stop",		48,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_drive",	49,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_brake",	50,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_backword",51,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_forward",	52,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_mode",		53,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"null_bit",	54,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_ready",	55,	1,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DMCM_life", 	56,	8,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

/********** pf-9 ***********/
{.id = 0X0C09A7F0, .pf = 9, .total_node = 8, .can_node_real_time_ar =
{
//{"DM_rectified_U", 0, 16, 0.1, 10000},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"DM_rectified_I", 		16,	16,	0.1,	10000},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},

//				{"DM_temperature", 		32,	8,		1,		40},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_RED, COMMON_LINE_WIDTH},
//				{"DMCM_temperature",		40, 	8,		1,		40},
		{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_RED, COMMON_LINE_WIDTH},
//				{"DM_main_contactor",	48, 	1, 	1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_charge_contactor",	49, 	1, 	1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"null_bit", 				50,	6,		1,		0},
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_error_code", 		56,	8,		1,		0}
		{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

///********** pf-255 ***********/
//{
//
//},

		/********** pf-1 ***********/
		{.id = 0X0C01A2A7, .pf = 1, .total_node = 15, .can_node_real_time_ar =
		{
//				{"drive_pedal",			0,		8,		0.4,	0},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"brake_pedal",			8,		8,		0.4,	0},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},

//				{"stall_forward",			16,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},	// byte 3
//				{"stall_backward",		17,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"stall_empty",			18,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"stall_still_?",			19,	1,		1,		0},	// ...
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"stall_?",					20,	1,		1,		0},	// ...
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"stall_?",					21,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"clutch_status",			22,	2,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"stall_now",				24,	8,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},

//				{"clutch_pedal",			32,	8,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},	// byte 4
//				{"EVCU_error_code", 		40,	8,		1,		0},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},	// byte 5
//				{"EVCU_life", 				48,	8,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},	// byte 6

//				{"bus_type", 				56,	4,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"battery_type", 			60,	4,		1,		0}
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

		/********** pf-4 ***********/
		{.id = 0X0C04A2A7, .pf = 4, .total_node = 8, .can_node_real_time_ar =
		{
//				{"single_battery_max_U",	0,		12,	0.01,	0},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"battery_index",				12,	4,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"single_battery_min_U",	16,	12,	0.01,	0},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"battery_index",				28,	4,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"battery_module_max_temperature",					32,	8,		1,		40},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_RED, COMMON_LINE_WIDTH},
//				{"battery_module_average_temperature",				40,	8,		1,		40},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_RED, COMMON_LINE_WIDTH},
//				{"battery_module_SOC",		48,	8,		0.4,		0},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"battery_module_status",	56,	8,		1,			0}
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

		/********** pf-5 ***********/
		{.id = 0X0C05A2A7, .pf = 5, .total_node = 12, .can_node_real_time_ar =
		{
//				{"cmd_DM_target_torgue",	0,		16,	1,		32000},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_BLUE, COMMON_LINE_WIDTH},
//				{"cmd_DMC_U_limit", 			16,	16,	0.1,	10000},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"cmd_DMC_I_limit", 			32,	16,	0.1,	10000},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},

//				{"cmd_DM_stop",				48,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_drive",				49,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_brake",				50,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_backword",			51,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_forward",			52,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_mode",				53,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"null_bit",					54,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"command_DM_ready",			55,	1,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"EVCU_life", 					56,	8,		1,		0}
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

		/********** pf-6 ***********/
		{.id = 0X0C06A2A7, .pf = 6, .total_node = 12, .can_node_real_time_ar =
		{
//				{"cmd_DM_target_torgue", 0, 16, 1, 32000},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_BLUE, COMMON_LINE_WIDTH},
//				{"cmd_DMC_U_limit", 16, 16, 0.1, 10000},		// voltage
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"cmd_DMC_I_limit", 32, 16, 0.1, 10000},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},

//				{"cmd_DM_stop", 48, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_drive", 49, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_brake", 50, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_backword", 51, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_forward", 52, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"cmd_DM_mode", 53, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"null_bit", 54, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"command_DM_ready", 55, 1, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"EVCU_life", 56, 8, 1, 0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}},

		/********** pf-7 ***********/
		{.id = 0X0C07A2A7, .pf = 7, .total_node = 6, .can_node_real_time_ar =
		{
//				{"DMCM_contactor_U1",		0,		16,	0.1,	10000},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"DM_temperature", 			16,	8,		1,		40},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_RED, COMMON_LINE_WIDTH},
//				{"DMCM_temperature", 		24,	8,		1,		40},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_RED, COMMON_LINE_WIDTH},
//				{"DMCM_contactor_U2",		32,	16,	0.1,	10000},
				{SHOW_AS_LINE, COMMON_Y_RULER, RGBA_GREEN, COMMON_LINE_WIDTH},
//				{"DMCM_contactor_status",	48,	8,		1,		0},
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH},
//				{"DM_error_code", 			56,	8,		1,		0}	// ................................................
				{SHOW_AS_ENTRY, COMMON_Y_RULER, RGBA_WHITE, COMMON_LINE_WIDTH}}}

};

int can_frame_real_time_ar_length = sizeof(can_frame_real_time_ar) / sizeof(can_frame_real_time_t);

