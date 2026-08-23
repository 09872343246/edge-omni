#include <stdio.h>
#include <string.h>
#include "sensor_hal.h"

extern const sensor_hal_ops_t mpu6050_hal_ops;
extern const sensor_hal_ops_t sht30_hal_ops;


static const struct {
	const char		*name;
	const sensor_hal_ops_t	*ops;
} hal_registry[] = {
	{ "mpu6050", &mpu6050_hal_ops },
	{ "sht30",   &sht30_hal_ops   },
	{ NULL,      NULL             }
};


const sensor_hal_ops_t *hal_get_ops(const char *sensor_name){
	for (int i = 0; hal_registry[i].name != NULL; i++) {
		if (strcmp(sensor_name, hal_registry[i].name) == 0) {
			return hal_registry[i].ops;
		}
	}
	return NULL;
}
