#ifndef METRICS_H
#define METRICS_H

#include <stdatomic.h>
#include <stddef.h>

extern atomic_int g_i2c_retry_total;

extern atomic_int g_temperature_milli;

extern atomic_int g_humidity_milli;

extern atomic_int g_system_state;

void record_latency(int latency_us);

void calc_latency_percentile(int *out_p50, int *out_p99);

int metrics_generate(char *buf, size_t buf_size);

#endif
