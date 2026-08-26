#include "db_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


static sqlite3 *db = NULL;

#define BATCH_SIZE		10
#define BATCH_TIMEOUT_MS	50


typedef struct {
	int timestamp;
	char sensor_type[16];
	char channel[16];
	double value;
} SensorRecord;


static SensorRecord batch_buffer[BATCH_SIZE];
static int batch_count = 0;
static struct timespec last_flush_time;


static int exec_sql(const char *sql){
	char *errmsg = NULL;
	int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "[DB] SQL错误: %s\n", errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}



int db_init(void){
	int rc;
	rc = sqlite3_open("/var/lib/sensor/data.db", &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "[DB] 无法打开数据库: %s\n", sqlite3_errmsg(db));
		return -1;
	}


	rc = exec_sql(
		"CREATE TABLE IF NOT EXISTS sensor_data ("
		"    id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"    timestamp INTEGER,"
		"    sensor_type TEXT,"
		"    channel TEXT,"
		"    value REAL"
		");");
	if (rc != 0) return -1;


	rc = exec_sql(
		"CREATE TABLE IF NOT EXISTS alarm_log ("
		"    id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"    timestamp INTEGER,"
		"    alarm_type TEXT,"
		"    level INTEGER,"
		"    message TEXT"
		");");
	if (rc != 0) return -1;



	rc = exec_sql(
		"CREATE TABLE IF NOT EXISTS system_log ("
		"    id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"    timestamp INTEGER,"
		"    level TEXT,"
		"    message TEXT"
		");");
	if (rc != 0) return -1;


	rc = exec_sql("PRAGMA journal_mode=WAL;");
	if (rc != 0) return -1;

	rc = exec_sql("PRAGMA synchronous=NORMAL;");
	if (rc != 0) return -1;

	batch_count = 0;
	clock_gettime(CLOCK_MONOTONIC, &last_flush_time);


	printf("[DB] 数据库初始化完成（WAL模式已开启）\n");
	return 0;
}


void db_close(void){
	if (db) {
		db_flush_batch();
		sqlite3_close(db);
		db = NULL;
	}
}


int db_insert_sensor(int timestamp, const char *sensor_type,const char *channel, double value){
	if (batch_count >= BATCH_SIZE) {
		db_flush_batch();
	}


	batch_buffer[batch_count].timestamp = timestamp;
	strncpy(batch_buffer[batch_count].sensor_type, sensor_type, 15);
	batch_buffer[batch_count].sensor_type[15] = '\0';
	strncpy(batch_buffer[batch_count].channel, channel, 15);
	batch_buffer[batch_count].channel[15] = '\0';
	batch_buffer[batch_count].value = value;
	batch_count++;


	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	long elapsed_ms = (now.tv_sec - last_flush_time.tv_sec) * 1000 + (now.tv_nsec - last_flush_time.tv_nsec) / 1000000;
	if (elapsed_ms >= BATCH_TIMEOUT_MS) {
		return db_flush_batch();
	}
	return 0;
}


int db_flush_batch(void){
	if (batch_count == 0 || db == NULL) return 0;
	sqlite3_stmt *stmt;
	const char *sql = "INSERT INTO sensor_data (timestamp, sensor_type, channel, value) ""VALUES (?, ?, ?, ?);";
	exec_sql("BEGIN TRANSACTION;");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	for (int i = 0; i < batch_count; i++) {
		sqlite3_bind_int(stmt, 1, batch_buffer[i].timestamp);
		sqlite3_bind_text(stmt, 2, batch_buffer[i].sensor_type, -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, batch_buffer[i].channel, -1, SQLITE_STATIC);
		sqlite3_bind_double(stmt, 4, batch_buffer[i].value);

		sqlite3_step(stmt);
		sqlite3_reset(stmt);
	}

	sqlite3_finalize(stmt);
	exec_sql("COMMIT;");



	printf("[DB] 批量写入 %d 条传感器数据\n", batch_count);


        batch_count = 0;
        clock_gettime(CLOCK_MONOTONIC, &last_flush_time);

	return 0;
}


int db_insert_alarm(int timestamp, const char *alarm_type,int level, const char *message){
	if (db == NULL) return -1;
	sqlite3_stmt *stmt;
	const char *sql = "INSERT INTO alarm_log (timestamp, alarm_type, level, message) "
			"VALUES (?, ?, ?, ?);";
	exec_sql("BEGIN IMMEDIATE;");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, timestamp);

	sqlite3_bind_text(stmt, 2, alarm_type, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 3, level);
	sqlite3_bind_text(stmt, 4, message, -1, SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	exec_sql("COMMIT;");

	sqlite3_wal_checkpoint_v2(db, NULL, SQLITE_CHECKPOINT_FULL, NULL, NULL);
	printf("[DB] 【关键报警已落盘】%s\n", message);
	return 0;
}



int db_check_and_repair(void){
	sqlite3_stmt *stmt;
	int rc;

	rc = sqlite3_prepare_v2(db, "PRAGMA integrity_check;", -1, &stmt, NULL);
	if (rc != SQLITE_OK) return -1;

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char *result = (const char *)sqlite3_column_text(stmt, 0);
		if (strcmp(result, "ok") == 0) {
			printf("[DB] 完整性检查通过\n");
			sqlite3_finalize(stmt);
			return 0;
		}
	fprintf(stderr, "[DB] 数据库损坏: %s\n", result);
	}
	sqlite3_finalize(stmt);

	const char *backup_path = "/var/lib/sensor/data.db.bak";
	if (access(backup_path, F_OK) == 0) {
		printf("[DB] 发现备份，正在恢复...\n");
		sqlite3_close(db);
		if (rename(backup_path, "/var/lib/sensor/data.db") == 0) {
			rc = sqlite3_open("/var/lib/sensor/data.db", &db);
			if (rc == SQLITE_OK) {
				printf("[DB] 恢复成功\n");
				return 0;
			}
		}
	}

	fprintf(stderr, "[DB] 数据库损坏且无备份！\n");
	return -1;
}




int db_backup(void){
	if (db == NULL) return -1;
	sqlite3 *backup_db;
	sqlite3_open("/var/lib/sensor/data.db.bak", &backup_db);
	sqlite3_backup *backup = sqlite3_backup_init(backup_db, "main", db, "main");
	if (backup) {
		sqlite3_backup_step(backup, -1);
		sqlite3_backup_finish(backup);
	}
	sqlite3_close(backup_db);
	printf("[DB] 备份完成\n");
	return 0;
}


int db_vacuum(void){
	return exec_sql("VACUUM;");
}































