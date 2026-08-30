#include "watchdog.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>


extern _Atomic uint64_t g_sensor_heartbeat;
extern _Atomic uint64_t g_http_heartbeat;

_Atomic int g_watchdog_dead_thread  = 0;
_Atomic int g_watchdog_monitor_alive = 0;

static int g_watchdog_fd = -1;

#define DEAD_THRESHOLD_SEC	5
#define FEED_INTERVAL_SEC	1
void *watchdog_monitor_thread(void *arg){
	(void)arg;
	g_watchdog_fd = open("/dev/watchdog", O_WRONLY);
	if (g_watchdog_fd < 0) {
		perror("[WATCHDOG] open /dev/watchdog failed");
		return NULL;
	}


	printf("[WATCHDOG] DEBUG MODE: /dev/watchdog NOT opened. "
               "System will NOT reset. Printing feed instead.\n");



	printf("[WATCHDOG] Monitor thread started (tid=%lu)\n",(unsigned long)pthread_self());
	printf("[WATCHDOG] Hardware: sunxi-wdt,check every %ds, dead threshold=%ds\n",FEED_INTERVAL_SEC, DEAD_THRESHOLD_SEC);
	uint64_t last_sensor_hb = 0;
	uint64_t last_http_hb   = 0;


	int sensor_dead_count = 0;
	int http_dead_count   = 0;


	while (1) {
		uint64_t curr_sensor_hb = atomic_load(&g_sensor_heartbeat);
		uint64_t curr_http_hb   = atomic_load(&g_http_heartbeat);
		if (curr_sensor_hb == last_sensor_hb) {
			sensor_dead_count++;
		} else {
			sensor_dead_count = 0;
			last_sensor_hb = curr_sensor_hb;
		}
		if (curr_http_hb == last_http_hb) {
			http_dead_count++;
		}else{
			http_dead_count = 0;
			last_http_hb = curr_http_hb;
		}


		bool is_any_dead = (sensor_dead_count >= DEAD_THRESHOLD_SEC) || (http_dead_count >= DEAD_THRESHOLD_SEC);
		if (is_any_dead) {
			if (sensor_dead_count >= DEAD_THRESHOLD_SEC) {
				atomic_store(&g_watchdog_dead_thread, 1);
				printf("[WATCHDOG] FATAL: Collect thread DEAD ""for %ds! Stop feeding -> SYSTEM RESET!\n",DEAD_THRESHOLD_SEC);
			}
			if (http_dead_count >= DEAD_THRESHOLD_SEC) {
				atomic_store(&g_watchdog_dead_thread, 2);
				printf("[WATCHDOG] FATAL: HTTP thread DEAD ""for %ds! Stop feeding -> SYSTEM RESET!\n",DEAD_THRESHOLD_SEC);
			}

			atomic_store(&g_watchdog_monitor_alive, 0);
			extern int g_watchdog_test_mode;

			if (g_watchdog_test_mode) {
				printf("[WATCHDOG] TEST MODE: Detected dead thread, "
                                      "but /tmp/watchdog_test_mode exists. "
                                       "Continue feeding (NO RESET).\n");
			} else {
				break;
			}
//			break;

		}
		ssize_t ret = write(g_watchdog_fd, "feed", 4);

		printf("[WATCHDOG] feed (sensor_hb=%" PRIu64 ", http_hb=%" PRIu64 ")\n",
       curr_sensor_hb, curr_http_hb);

		if (ret < 0) {
			perror("[WATCHDOG] write to watchdog failed");
		}
		sleep(FEED_INTERVAL_SEC);
	}
	printf("[WATCHDOG] Monitor thread EXITED. ""System will reset in ~16s (hardware watchdog timeout).\n");

	return NULL;
}
