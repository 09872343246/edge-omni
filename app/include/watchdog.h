#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdatomic.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern _Atomic uint64_t g_sensor_heartbeat;
extern _Atomic uint64_t g_http_heartbeat;

extern _Atomic int g_watchdog_dead_thread;
extern _Atomic int g_watchdog_monitor_alive;

void *watchdog_monitor_thread(void *arg);

extern int g_watchdog_test_mode;

#define WATCHDOG_FEED_SENSOR() atomic_fetch_add(&g_sensor_heartbeat, 1)
#define WATCHDOG_FEED_HTTP()   atomic_fetch_add(&g_http_heartbeat, 1)

#ifdef __cplusplus
}
#endif

#endif
