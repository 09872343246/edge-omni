#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include "sensor_hal.h"



typedef struct ring_buffer ring_buffer_t;

ring_buffer_t *ring_buffer_create(uint32_t capacity);

void ring_buffer_destroy(ring_buffer_t *rb);

bool ring_buffer_write(ring_buffer_t *rb, const sensor_data_t *data);


bool ring_buffer_read(ring_buffer_t *rb, sensor_data_t *data);

uint32_t ring_buffer_count(ring_buffer_t *rb);

uint32_t ring_buffer_capacity(ring_buffer_t *rb);

#endif /* RING_BUFFER_H */

