#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "sensor_hal.h"
#include "ring_buffer.h"



struct ring_buffer {
	_Atomic 	uint32_t write_idx;
	_Atomic 	uint32_t read_idx;
	uint32_t	mask;
	sensor_data_t	*buffer;
};



ring_buffer_t *ring_buffer_create(uint32_t capacity){
	if (capacity == 0 || (capacity & (capacity - 1)) != 0) {
		return NULL;
	}
	ring_buffer_t *rb = malloc(sizeof(ring_buffer_t));
	if (!rb) {
		return NULL;
	}
	rb->buffer = malloc(capacity * sizeof(sensor_data_t));
	if (!rb->buffer) {
		free(rb);
		return NULL;
	}
	atomic_init(&rb->write_idx, 0);
	atomic_init(&rb->read_idx, 0);

	rb->mask = capacity - 1;
	return rb;
}

void ring_buffer_destroy(ring_buffer_t *rb){
	if (!rb) {
		return;
	}
	free(rb->buffer);
	free(rb);
}


bool ring_buffer_write(ring_buffer_t *rb, const sensor_data_t *data){
	uint32_t w = atomic_load_explicit(&rb->write_idx, memory_order_relaxed);
	uint32_t r = atomic_load_explicit(&rb->read_idx, memory_order_acquire);
	if (((w + 1) & rb->mask) == r) {
		return false;
	}
	rb->buffer[w & rb->mask] = *data;
	atomic_store_explicit(&rb->write_idx, w + 1, memory_order_release);
	return true;
}

bool ring_buffer_read(ring_buffer_t *rb, sensor_data_t *data){
	uint32_t r = atomic_load_explicit(&rb->read_idx, memory_order_relaxed);
	uint32_t w = atomic_load_explicit(&rb->write_idx, memory_order_acquire);
	if (r == w) {
		return false;
	}
	*data = rb->buffer[r & rb->mask];
	atomic_store_explicit(&rb->read_idx, r + 1, memory_order_release);
	return true;
}



uint32_t ring_buffer_count(ring_buffer_t *rb){
	uint32_t w = atomic_load_explicit(&rb->write_idx, memory_order_relaxed);
	uint32_t r = atomic_load_explicit(&rb->read_idx, memory_order_relaxed);
	return (w - r) & rb->mask;
}

uint32_t ring_buffer_capacity(ring_buffer_t *rb){
	return rb->mask + 1;
}




















































