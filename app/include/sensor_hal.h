#ifndef SENSOR_HAL_H
#define SENSOR_HAL_H

#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>


typedef struct {
	uint64_t timestamp_ns;
	uint8_t  sensor_type;
	uint8_t  channel;
	int32_t  value;
}sensor_data_t;

#define SENSOR_MPU6050	0
#define SENSOR_SHT30	1


typedef struct {
	int	(*open)(const char *dev_path, void **handle);
	ssize_t	(*read)(void *handle, void *buf, size_t len);
	int	(*ioctl)(void *handle, unsigned int cmd, void *arg);
	int	(*close)(void **handle);
	int	(*get_info)(void *handle, char *info_buf, size_t buf_len);
} sensor_hal_ops_t;

const sensor_hal_ops_t *hal_get_ops(const char *sensor_name);

#endif /* SENSOR_HAL_H */
