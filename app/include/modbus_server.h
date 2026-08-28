#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H

#include <stdint.h>
#include <stdatomic.h>


typedef struct {
	_Atomic uint16_t temperature;
	_Atomic uint16_t humidity;
	_Atomic uint16_t accel_x;
	_Atomic uint16_t accel_y;
} modbus_reg_data_t;


int modbus_server_start(void);

void modbus_server_stop(void);

void modbus_server_update_data(const modbus_reg_data_t *data);

#endif
