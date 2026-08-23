#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "ring_buffer.h"


#define TEST_COUNT      1000000
#define RING_CAPACITY   2048


static ring_buffer_t *g_rb;
static _Atomic uint64_t g_write_ok = 0;
static _Atomic uint64_t g_write_fail = 0;
static _Atomic uint64_t g_read_ok = 0;
static _Atomic uint64_t g_read_fail = 0;
static _Atomic uint64_t g_error_count = 0;


static void *producer_thread(void *arg){
	(void)arg;
	for (uint64_t i = 0; i < TEST_COUNT; i++) {
                sensor_data_t data;
		data.timestamp_ns = i;
		data.sensor_type  = 0;
		data.channel      = i % 6;
		data.value        = (int32_t)i;
		while (!ring_buffer_write(g_rb, &data)) {
			atomic_fetch_add(&g_write_fail, 1);
			struct timespec ts = {.tv_sec = 0, .tv_nsec = 1000};
			nanosleep(&ts, NULL);
		}
		atomic_fetch_add(&g_write_ok, 1);
	}
	printf("[Producer] Done. Written: %lu, Failed: %lu\n",(unsigned long)g_write_ok, (unsigned long)g_write_fail);
	return NULL;
}
static void *consumer_thread(void *arg){
	(void)arg;

	uint64_t expected = 0;
	for (uint64_t i = 0; i < TEST_COUNT; i++) {
		sensor_data_t data;
		while (!ring_buffer_read(g_rb, &data)) {
			atomic_fetch_add(&g_read_fail, 1);
			struct timespec ts = {.tv_sec = 0, .tv_nsec = 1000};
			nanosleep(&ts, NULL);
		}
		atomic_fetch_add(&g_read_ok, 1);
		if (data.timestamp_ns != expected) {
			printf("[Consumer] ERROR at %lu: expected %lu, got %lu\n",(unsigned long)i, (unsigned long)expected,(unsigned long)data.timestamp_ns);
			atomic_fetch_add(&g_error_count, 1);
		}
		if (data.value != (int32_t)expected) {
			printf("[Consumer] VALUE ERROR at %lu\n", (unsigned long)i);
			atomic_fetch_add(&g_error_count, 1);
		}
		expected++;
	}
	printf("[Consumer] Done. Read: %lu, Failed: %lu, Errors: %lu\n",(unsigned long)g_read_ok, (unsigned long)g_read_fail,(unsigned long)g_error_count);
	return NULL;
}

int main(void){

	pthread_t producer, consumer;
	int ret;
	printf("=== Ring Buffer SPSC Test ===\n");
	printf("Buffer capacity: %u\n", RING_CAPACITY);
	printf("Test count: %d\n", TEST_COUNT);

	g_rb = ring_buffer_create(RING_CAPACITY);
	if (!g_rb) {
		printf("Failed to create ring buffer\n");
		return 1;
	}
	printf("Ring buffer created. Capacity: %u\n", ring_buffer_capacity(g_rb));

	ret = pthread_create(&consumer, NULL, consumer_thread, NULL);
	if (ret != 0) {
		perror("pthread_create consumer");
		return 1;
	}
	ret = pthread_create(&producer, NULL, producer_thread, NULL);
	if (ret != 0) {
                perror("pthread_create consumer");
                return 1;
        }
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
	printf("\n=== Result ===\n");
	printf("Written: %lu, Read: %lu\n",(unsigned long)g_write_ok, (unsigned long)g_read_ok);
	printf("Write failures (buffer full): %lu\n", (unsigned long)g_write_fail);
	printf("Read failures (buffer empty): %lu\n", (unsigned long)g_read_fail);
	printf("Data errors: %lu\n", (unsigned long)g_error_count);
	if (g_error_count == 0 && g_write_ok == TEST_COUNT && g_read_ok == TEST_COUNT) {
		printf("\n✅ TEST PASSED: 100%% reliable, zero data loss!\n");
	}else{
		printf("\n❌ TEST FAILED\n");
	}
	ring_buffer_destroy(g_rb);
	return (g_error_count == 0) ? 0 : 1;
}




































