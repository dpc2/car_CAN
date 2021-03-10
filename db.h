//
//

#pragma once
#include "includes.h"

#define	MAX_DB_COUNT_TO_STORE	(100)

typedef struct
{
	sqlite3 * sqlite3_statis;


}conf_db_t;



void * pthread_net_modbus_handle(void *arg);

int sqlite3_creat_net_modbus_log_table(sqlite3 *modbus_db);

int delete_row_of_net_modbus_log_table(sqlite3 *net_modbus_db);
int system_reboot(int reboot_because_error);
void beep_once(int beep_length);

int update_db_name(const char * old_db_name, const char * new_db_name);

