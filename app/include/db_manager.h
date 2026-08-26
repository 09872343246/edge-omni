#ifndef DB_MANAGER_H
#define DB_MANAGER_H


#include <sqlite3.h>

int db_init(void);

void db_close(void);

int db_insert_sensor(int timestamp, const char *sensor_type,const char *channel, double value);

int db_flush_batch(void);

int db_insert_alarm(int timestamp, const char *alarm_type,int level, const char *message);

int db_check_and_repair(void);

int db_backup(void);

int db_vacuum(void);


#endif
