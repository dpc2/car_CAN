/*
 * db_log_handle.c
 *
 *  Created on: 2017��5��15��
 *      Author: Lenovo
 */

#include	"can_history.h"
#include	"includes.h"
#include	"db.h"


extern sqlite3 * sqlite3_db_statis;
//extern sqlite3 * sqlite3_real_time;
char * g_db_path = "/home/x/can_data_db/";


int open_db(sqlite3 ** sqlite3_handle, const char * db_path, const char * db_name)
{
	int i;
	char * errmsg;
	char db_path_and_name[STRING_LENGTH] = {0};

	//open the database
	strcpy(db_path_and_name, db_path);
	strncpy(db_path_and_name + strlen(db_path_and_name), db_name, sizeof(db_path_and_name) - strlen(db_path_and_name) -1);
	db_path_and_name[sizeof(db_path_and_name) -1] = '0';


	if (SQLITE_OK != sqlite3_open(db_path_and_name, sqlite3_handle))
	{
		printf("0 != sqlite3_open(%s, &sqlite3_data \n", db_path_and_name);
		return -1;
	}

	if (SQLITE_OK != sqlite3_exec(*sqlite3_handle, "PRAGMA synchronous=OFF;", 0, 0, &errmsg))
	{
		printf("SQLITE_OK != sqlite3_exec(db_name, \"PRAGMA synchronous=OFF;\", 0, 0, 0) \n");
		return -1;
	}

	printf("open_db: %s is opened.\n", db_path_and_name);
	return 0;
}


int update_db_name(const char * old_db_name, const char * new_db_name)
{
	char sql[200];
	char * errmsg;

	/*********************************************************************************/

	sprintf(sql, "UPDATE db_statis SET db_name='%s' WHERE db_name='%s'", new_db_name, old_db_name);

	if (SQLITE_OK != sqlite3_exec(sqlite3_db_statis, sql, NULL, NULL, &errmsg))
	{
		printf("%s fail. errmsg: %s \n", sql, errmsg);
		sqlite3_free(errmsg);
		exit(1);
	}
	else
	{
//			printf("%s. errmsg: %s\n", new_db_name, errmsg);
	}


	return 0;
}


int delete_db(void)
{
	char str[STRING_LENGTH * 2];
	char ** result;
	int row, col;
	char * errmsg;

	//	DESC limit 1
	sprintf(str, "SELECT row FROM db_statis ORDER BY row DESC limit 1");
	if (SQLITE_OK != sqlite3_get_table(sqlite3_db_statis, str, &result, &row, &col, &errmsg))
	{
		printf("delete_db: %s fail. row = %d, errmsg: %s \n", str, row, errmsg);
		sqlite3_free(errmsg);
		sqlite3_free_table(result);
		exit(1);
	}
	if (row == 0)
	{
		printf("delete_db: %s: row = %d \n", str, row);
		return 0;
	}

	int latest_db_index = atoi(result[col + 0]);
	sqlite3_free_table(result);

//	if (latest_db_index - MAX_DB_COUNT <= 0)
//	{
//		return 0;
//	}

	sprintf(str, "SELECT db_name FROM db_statis WHERE row<%d", latest_db_index - MAX_DB_COUNT_TO_STORE);
	if (SQLITE_OK != sqlite3_get_table(sqlite3_db_statis, str, &result, &row, &col, &errmsg))
	{
		printf("delete_db: %s fail. row = %d, errmsg: %s \n", str, row, errmsg);
		sqlite3_free(errmsg);
		sqlite3_free_table(result);
		exit(1);
	}
	if (row == 0)
	{
		printf("delete_db: %s: row = %d \n", str, row);
		return 0;
	}

	int i, j;
	j = col;
	for(i = 0; i < row; i++, j += col)
	{
		sprintf(str, "rm ");
		sprintf(str + strlen(str), g_db_path);
		sprintf(str + strlen(str), result[j]);

		puts(str);
		int sys = system(str);
		printf("system(str) returned %d \n", sys);
		if(sys == 0)
		{
			sprintf(str, "DELETE FROM db_statis WHERE db_name=%s", result[j]);
			if (SQLITE_OK != sqlite3_exec(sqlite3_db_statis, str, NULL, NULL, &errmsg))
			{
				printf("delete_db: %s fail. row = %d, errmsg: %s \n", str, row, errmsg);
				sqlite3_free(errmsg);
				//		sqlite3_free_table(result);
				exit(1);
			}
		}
	}
	sqlite3_free_table(result);

	return 0;
}


//int sqlite3_db_statis_get_db_name(void)
//{
//	char sql_str[512];
//	char **result;
//	int row, col;
//	char *errmsg;
//
//	sprintf(sql_str, "select db_name from db_name_table ORDER BY row DESC limit %d", DB_HISTORY_COUNT);
//	sqlite3_get_table(sqlite3_db_statis, sql_str, &result, &row, &col, &errmsg);
//
//	if (row > 0)
//	{
////		strcpy(data, result[col + 0]);
//	}
//
//	sqlite3_free_table(result);
//
//	return 0;
//}

//int db_get_data(int id, char *data)
//{
//	char sql_str[512];
//	char **result;
//	int row, col;
//	int load_cnts = 0;
//	char *errmsg;
//
//	sprintf(sql_str, "select version from software_version where type=%d limit 1", id);
//
////	sqlite3_get_table(sqlite3_history, sql_str, &result, &row, &col, &errmsg);
//
//	if (row > 0)
//	{
//		load_cnts = row;
//
//		strcpy(data, result[col + 0]);
//	}
//
//	sqlite3_free_table(result);
//	return load_cnts;
//}

//int get_net_modbus_item_conf_info_from_document(fn_link_t *p_fn_link)
//{
//    int column_count_of_fn = 5, row;
//
//    FILE *fp;
//    if((fp = fopen("/usr/Energy_Ctrl/net_modbus_item_conf.txt", "r")) == NULL)
//    {
//        send_log_to_file_to_display(0, (u8*)"open /usr/Energy_Ctrl/net_modbus_item_conf.txt failure \n", 0);
//        exit(0);
//    }
//
//    /*
//     if(fp != STDIN_FILENO)
//     {
//     if(dup2(fp, STDIN_FILENO) < 0 )
//     {
//     printf("dup2(fp, STDIN_FILENO) < 0  \n");
//     exit(-1);
//     }
//     }
//     abc();
//     */
//
//    int net_modbus_item_conf_document_info[column_count_of_fn];
//
//    fn_node_t ** pp_fn_node = &p_fn_link->fn_node_head;
//    fn_node_t * p_current_node;
//    item_t * p_current_item = NULL;
//
//    int item_ar_index = 0, i;
//    int last_fn_order_number, fn_order_number, last_fn, fn, global_item_id, fn_private_index;
//    last_fn_order_number = fn_order_number = last_fn = fn = -2;
//
//    char printtologbuf[400];
//
//    while(1)
//    {
//        for(i = 0; i < column_count_of_fn; i++)
//        {
//            if(fscanf(fp, "%d", &net_modbus_item_conf_document_info[i]) == EOF)
//            {
//                return 0;
//            }
//        }
//
//        row = net_modbus_item_conf_document_info[0];
//        fn_order_number = net_modbus_item_conf_document_info[1];
//        fn = net_modbus_item_conf_document_info[2];
//        global_item_id = net_modbus_item_conf_document_info[3];
//        fn_private_index = net_modbus_item_conf_document_info[4];
//
//        // printf("row: %d, fn_order_number: %d, fn: %d, global_item_id: %d, fn_private_index: %d   \n",
//        //        row, fn_order_number, fn, global_item_id, fn_private_index);
//
//        if(row < 1)
//        {
//            sprintf(printtologbuf, "error: get row == %d from net_modbus_item_conf.txt \n", row);
//            send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//            return -1;
//        }
//        if(fn > sizeof(g_item_amount_config_of_all_fn) / sizeof(int) || fn < 1)
//        {
//            sprintf(printtologbuf, "error: get fn == %d from net_modbus_item_conf.txt \n", fn);
//            send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//            return -1;
//        }
//        if(fn_order_number < 1)
//        {
//            sprintf(printtologbuf, "error: get fn_order_number == %d from net_modbus_item_conf.txt , ignore. \n", fn_order_number);
//            send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//            return -1;
//        }
//
//        if(fn_order_number != last_fn_order_number)
//        {
//            // create a new fn_node
//            p_current_node = (fn_node_t*)malloc(sizeof(fn_node_t));
//            if(NULL == p_current_node)
//            {
//                sprintf(printtologbuf, "malloc()error 1! \n");
//                send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//                return -1;
//            }
//
//            p_current_node->fn_order_number = fn_order_number;
//            p_current_node->fn = fn;
//            p_current_node->fn_item_count = g_item_amount_config_of_all_fn[fn - 1];
//            p_current_node->type = g_type_config_of_all_fn[fn - 1];
//            p_current_node->item_ar = (item_t*)calloc(p_current_node->fn_item_count, sizeof(item_t));
//            if(NULL == p_current_node->item_ar)
//            {
//                sprintf(printtologbuf, "malloc()error 2! \n");
//                send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//                return -1;
//            }
//
//            (*pp_fn_node) = p_current_node;
//            pp_fn_node = &(*pp_fn_node)->next_fn;
//            (*pp_fn_node) = p_fn_link->fn_node_head;
//            p_fn_link->fn_node_total++;
//            item_ar_index = 0;
//            //   p_current_item = &p_current_fn_node->item_ar[total_item_of_each_fn];
//            last_fn_order_number = fn_order_number;
//            last_fn = fn;
//        }else // if(fn_order_number == last_fn_order_number )
//        {
//            if(item_ar_index >= (*pp_fn_node)->fn_item_count)
//            {
//                sprintf(printtologbuf, "item_ar_index (= %d) >= p_current_fn_node->fn_item_total \n", item_ar_index);
//                send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//                return -1;
//            }
//            if(item_ar_index >= MAX_ITEM_AMOUNT_OF_ANY_FN)
//            {
//                sprintf(printtologbuf, "item_ar_index (= %d) > MAX_ITEM_COUNT_OF_ANY_FN  \n", item_ar_index);
//                send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//                return -1;
//            }
//            if(last_fn != fn)
//            {
//                sprintf(printtologbuf, "error: last_fn = %d,  fn = %d \n", last_fn, fn);
//                send_log_to_file_to_display(0, (u8*)printtologbuf, 0);
//                return -1;
//            }
//
//        }
//        // modefy  item_ar of the current fn_node
//        p_current_item = &p_current_node->item_ar[item_ar_index];
//
//        p_current_item->fn_private_index = fn_private_index;
//        p_current_item->global_item_id = global_item_id;
//        //strncpy(p_current_item->item_name, result[index++], FN_AND_ITEM_NAME_LENGTH - 1);
//        //p_current_item->item_name[strlen(p_current_item->item_name)] = '\0';
//        item_ar_index++;
//    }
//
//    return 0;
//}

int system_reboot(int reboot_because_error)
{
	int i;
	int console_fd;
	int f = 4000;    //f = 4000;
	int ff = 1190000 / f;

	if ((console_fd = open("/dev/console", O_WRONLY)) == -1)
	{
		printf("Failed to open console.\n");
		perror("open");
	}

	for (i = 180; i > 0; i--)
	{
		if (reboot_because_error != 0)
		{
			printf(
					"computer will restart in %ds (error occurred)! enter \"CTRL + C\" if you don't want it to reboot! \n",
					i);
			ioctl(console_fd, KIOCSOUND, ff); // KDMKTONE
			usleep(300 * 1000);
			ioctl(console_fd, KIOCSOUND, 0);
			usleep(700 * 1000);
		}
		else
		{
			printf("computer will restart in %ds (no error)! enter \"CTRL + C\" if you don't want it to reboot! \n", i);
			sleep(1);
		}
	}
	close(console_fd);
	sync();
	sleep(5);
	system("reboot");
	//system("gnome-terminal -e top");
	//int xx = reboot(RB_AUTOBOOT);
	return 0;
}

void beep_once(int beep_length)
{
	return;
//	beep_length = 0;

	int console_fd;
	int f = 3000;    // f = 4000;
	int ff = 1190000 / f;

	if ((console_fd = open("/dev/console", O_WRONLY)) == -1)
	{
		printf("Failed to open console.\n");
		perror("open");
	}
	ioctl(console_fd, KIOCSOUND, ff); // KDMKTONE
	usleep(beep_length * 1000);
	ioctl(console_fd, KIOCSOUND, 0);
	close(console_fd);
	return;
}
//
//int init_net_modbus_log_db(message_t * p)
//{
//    if(log_handle->db_path_and_name == NULL || log_handle->log_table_name == NULL)
//    {
//        printf("log_handle->db_path_and_name or log_table_name == NULL \n");
//        return -1;
//    }
//    if(strlen(log_handle->log_table_name) > 50)
//    {
//        printf("error: strlen(log_handle->log_table_name) > 50 \n");
//        return -1;
//    }
//    if(log_handle->max_row_of_log_table < 1)
//    {
//        printf("error: log_handle->max_row_of_log_table < 1 \n");
//        return -1;
//    }
//
//    if(sqlite3_open(log_handle->db_path_and_name, &log_handle->sqlite3_log) != SQLITE_OK)
//    {
//        printf("sqlite3_open fail! \n");
//        return -1;
//    }
//
//    char *errmsg = NULL;
//    int i, j;
//    // NOT NULL DEFAULT CURRENT_TIMESTAMP  ON UPDATE CURRENT_TIMESTAMP
//
//    sprintf(log_handle->str, "create table if not exists %s", log_handle->log_table_name);
//    sprintf(log_handle->str + strlen(log_handle->str), " ("
//            "row            integer     primary key autoincrement,"
//            "log_index      int         default 0,"
//            "log            varchar(400),"
//            "update_time    varchar(50)   default 0 )");
//
//    if( SQLITE_OK != sqlite3_exec(log_handle->sqlite3_log, log_handle->str, NULL, NULL, &errmsg))
//    {
//        printf("error: sqlite3_creat_net_modbus_log_table!\n %s", errmsg);
//        sqlite3_free(errmsg);
//        return -1;
//    }
//    sqlite3_free(errmsg);
//    //////////////////////////////////////////////////////
//    sprintf(log_handle->str, "select row, log_index from %s", log_handle->log_table_name);
//    sprintf(log_handle->str + strlen(log_handle->str), " ORDER BY log_index DESC limit 1 ");
//
//    int row, col;
//    char **result = NULL;
//
//    if( SQLITE_OK != sqlite3_get_table(log_handle->sqlite3_log, log_handle->str, &result, &row, &col, &errmsg))
//    {
//        printf("error: sqlite3_get_table check table is exist or not!\n %s", errmsg);
//        sqlite3_free(errmsg);
//        sqlite3_free_table(result);
//        return -1;
//    }
//
//    printf("\nrow = %d    col = %d \n", row, col);
//
//    if(row != 0)
//    {
//        for(i = 0; i < (row + 1) * col; i++)
//            printf("//%s ", result[i]);
//        printf("\n");
//        log_handle->current_row_to_update = atoi(result[col]);
//        log_handle->log_serial_number = atoi(result[col + 1]);
//        sqlite3_free(errmsg);
//        sqlite3_free_table(result);
//        return 0;
//    }else
//    {
//        log_handle->current_row_to_update = 0;
//        log_handle->log_serial_number = 0;
//        sqlite3_free(errmsg);
//        sqlite3_free_table(result);
//
//    }
//
//    /////////////////////////////////////////////////////
//
//    sprintf(log_handle->str, "insert into %s ", log_handle->log_table_name);
//    sprintf(log_handle->str + strlen(log_handle->str), "(log) values( '' )");
//
//    for(i = 0; i < log_handle->max_row_of_log_table; i++)
//    {
//        if( SQLITE_OK != sqlite3_exec(log_handle->sqlite3_log, log_handle->str, NULL, NULL, &errmsg))
//        {
//            printf("error: sqlite3_creat_net_modbus_log_table!\n %s", errmsg);
//            sqlite3_free(errmsg);
//            return -1;
//        }
//        sqlite3_free(errmsg);
//    }
//
//    /*
//     if( SQLITE_OK != sqlite3_exec(modbus_db, ".import /usr/Energy_Ctrl/net_modbus_item_conf.txt net_modbus_item_conf", NULL, NULL, &errmsg ))
//     {
//     perror( "ERR: .import /usr/Energy_Ctrl/net_modbus_item_conf.txt net_modbus_item_conf!\n" );
//     exit( -1 );
//     }
//     */
//
//    sqlite3_free(errmsg);
//    return 0;
//}
//
//int insert_log_to_log_db(log_db_t * log_handle, char * log, time_t update_time)
//{
//    if(++log_handle->current_row_to_update > log_handle->max_row_of_log_table)
//    {
//        log_handle->current_row_to_update = 1;
//    }
//    log_handle->log_serial_number++;
//
//    char *errmsg = NULL;
//    log_handle->update_time = update_time == 0 ? time(0) : update_time;
//    log_handle->ptr = localtime(&log_handle->update_time);
//    sprintf(log_handle->str, "update %s", log_handle->log_table_name);
//    sprintf(log_handle->str + strlen(log_handle->str), " set log = '%s", log);
//    sprintf(log_handle->str + strlen(log_handle->str), "', update_time = '%d-%d-%d---%02d:%02d:%02d', log_index = %ld where row = %d ", log_handle->ptr->tm_year + 1900, log_handle->ptr->tm_mon,
//            log_handle->ptr->tm_mday, log_handle->ptr->tm_hour, log_handle->ptr->tm_min, log_handle->ptr->tm_sec, log_handle->log_serial_number, log_handle->current_row_to_update);
//    if( SQLITE_OK != sqlite3_exec(log_handle->sqlite3_log, log_handle->str, NULL, NULL, &errmsg))
//    {
//        printf("insert_log_to_log_db fail! \n %s", errmsg);
//        sqlite3_free(errmsg);
//        return -1;
//    }
//    sqlite3_free(errmsg);
//
//    int temp_row = log_handle->current_row_to_update + 1;
//    if(temp_row > log_handle->max_row_of_log_table)
//    {
//        temp_row = 1;
//    }
//    sprintf(log_handle->str, "update %s", log_handle->log_table_name);
//    sprintf(log_handle->str + strlen(log_handle->str), " set log = '', update_time = '', log_index = 0 where row = %d", temp_row);
//    sqlite3_exec(log_handle->sqlite3_log, log_handle->str, NULL, NULL, &errmsg);
//
//    sqlite3_free(errmsg);
//    return 0;
//}
//
//int delete_row_of_net_modbus_log_table(sqlite3 *net_modbus_db)
//{
//	char str[300];
//	char ** result;
//	int row;
//	int col;
//
//	if (SQLITE_OK != sqlite3_get_table(net_modbus_db, "SELECT row FROM net_modbus_log ORDER BY row DESC limit 1", &result,
//					&row, &col, 0))
//	{
//		//sqlite3_close(net_modbus_db);
//		sprintf(str, "SELECT row FROM net_modbus_log ORDER BY row DESC error.\n");
//		return -1;
//	}
//	if (row == 0)
//		return 0;
//
//	int max_row = atoi(result[col]);
//	sqlite3_free_table(result);
//
//	int lowest_row_except = max_row - MAX_ROW_OF_NET_MODBUS_LOG_DB_TABLE;
//	sprintf(str, "delete from net_modbus_log where row < %d ", lowest_row_except);
//
//	if (SQLITE_OK != sqlite3_exec(net_modbus_db, str, NULL, NULL, 0))
//	{
//		sprintf(str, "delete from net_modbus_log where row < %d fail! max_row = %d \n", lowest_row_except, max_row);
//		return -1;
//	}
//	sprintf(str, "delete from net_modbus_log where row < %d succeed! max_row = %d \n", lowest_row_except, max_row);
//
//	return 0;
//}
