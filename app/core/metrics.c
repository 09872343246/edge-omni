#include "metrics.h"
#include <stdio.h>
#include <string.h>

atomic_int g_i2c_retry_total   = 0;
atomic_int g_temperature_milli = 0;
atomic_int g_humidity_milli    = 0;
atomic_int g_system_state      = 1;



#define LATENCY_WINDOW_SIZE 1024

static atomic_int g_latency_window[LATENCY_WINDOW_SIZE];
static atomic_int g_latency_write_idx = 0;
static atomic_int g_latency_count     = 0;


void record_latency(int latency_us){
	int idx = atomic_fetch_add(&g_latency_write_idx, 1) % LATENCY_WINDOW_SIZE;
	atomic_store(&g_latency_window[idx], latency_us);
	int old_count = atomic_fetch_add(&g_latency_count, 1);
	if (old_count >= LATENCY_WINDOW_SIZE) {
		atomic_store(&g_latency_count, LATENCY_WINDOW_SIZE);
	}
}

void calc_latency_percentile(int *out_p50, int *out_p99){
	int local[LATENCY_WINDOW_SIZE];
	int count;
	int i, j;

	count = atomic_load(&g_latency_count);
	if (count > LATENCY_WINDOW_SIZE) {
		count = LATENCY_WINDOW_SIZE;
	}

	if (count == 0) {
		*out_p50 = 0;
		*out_p99 = 0;
		return;
	}
	for (i = 0; i < count; i++) {
		local[i] = atomic_load(&g_latency_window[i]);
	}


	for (i = 0; i < count - 1; i++) {
		for (j = i + 1; j < count; j++) {
			if (local[i] > local[j]) {
				int tmp = local[i];
				local[i] = local[j];
				local[j] = tmp;
			}
		}
	}
	*out_p50 = local[count * 50 / 100];
	*out_p99 = local[count * 99 / 100];
}


int metrics_generate(char *buf, size_t buf_size){
	int p50, p99;
	int temp, humi, retry, state;


	retry = atomic_load(&g_i2c_retry_total);
	temp  = atomic_load(&g_temperature_milli);
	humi  = atomic_load(&g_humidity_milli);
	state = atomic_load(&g_system_state);


	calc_latency_percentile(&p50, &p99);


	return snprintf(buf, buf_size,
		"# HELP i2c_retry_total Total I2C retry count\n"
		"# TYPE i2c_retry_total counter\n"
		"i2c_retry_total{driver=\"mpu6050\"} %d\n"
		"\n"
		"# HELP sensor_temperature_celsius Current temperature\n"
		"# TYPE sensor_temperature_celsius gauge\n"
		"sensor_temperature_celsius{id=\"sht30\"} %.1f\n"
		"\n"
		"# HELP sensor_humidity_percent Current humidity\n"
		"# TYPE sensor_humidity_percent gauge\n"
		"sensor_humidity_percent{id=\"sht30\"} %.1f\n"
		"\n"
		"# HELP task_latency_us Task latency in microseconds\n"
		"# TYPE task_latency_us summary\n"
		"task_latency_us{quantile=\"0.50\"} %d\n"
		"task_latency_us{quantile=\"0.99\"} %d\n"
		"\n"
		"# HELP system_state Current system state (0=INIT,1=RUNNING,2=DEGRADED,3=RECOVERING,4=FAULT)\n"
		"# TYPE system_state gauge\n"
		"system_state{state=\"running\"} %d\n"
		"system_state{state=\"degraded\"} %d\n"
		"system_state{state=\"fault\"} %d\n",
		retry,
		temp / 1000.0,
		humi / 1000.0,
		p50,
		p99,
		(state == 1) ? 1 : 0,
		(state == 2) ? 1 : 0,
		(state == 4) ? 1 : 0
	);
}


