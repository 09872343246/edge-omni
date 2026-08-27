#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <stdatomic.h>
#include "sensor_hal.h"
#include "metrics.h"

typedef struct {
	int			fd;
	struct libevdev 	*evdev;
	sensor_data_t		cache[6];
	int			cache_count;
}mpu6050_hal_t;

static const int axis_map[] = {
	[ABS_X]  = 0,
	[ABS_Y]  = 1,
	[ABS_Z]  = 2,
	[ABS_RX] = 3,
	[ABS_RY] = 4,
	[ABS_RZ] = 5,
};


static int mpu6050_hal_open(const char *dev_path, void **handle){
	int fd = open(dev_path, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		perror("mpu6050_hal_open: open failed");
		return -errno;
	}
	struct libevdev *evdev = libevdev_new();
	if (!evdev) {
		close(fd);
		return -ENOMEM;
	}
	if (libevdev_set_fd(evdev, fd) < 0) {
		libevdev_free(evdev);
		close(fd);
		return -ENODEV;
	}
	mpu6050_hal_t *priv = calloc(1, sizeof(mpu6050_hal_t));
	if (!priv) {
		libevdev_free(evdev);
		close(fd);
		return -ENOMEM;
	}
	priv->fd = fd;
	priv->evdev = evdev;
	priv->cache_count = 0;
	*handle = priv;
	printf("[MPU6050 HAL] Opened %s, name=%s\n",dev_path, libevdev_get_name(evdev));
	return 0;
}

static ssize_t mpu6050_hal_read(void *handle, void *buf, size_t len){
	mpu6050_hal_t *priv = (mpu6050_hal_t *)handle;
	sensor_data_t *out  = (sensor_data_t *)buf;
	size_t max_count    = len / sizeof(sensor_data_t);
	size_t returned     = 0;
	while (priv->cache_count > 0 && returned < max_count) {
		out[returned] = priv->cache[6 - priv->cache_count];
		priv->cache_count--;
		returned++;
	}
	while (returned < max_count) {
		struct input_event ev;
		int rc = libevdev_next_event(priv->evdev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
		if (rc < 0) {
			if (rc == -EAGAIN) {
				break;
			}
			atomic_fetch_add(&g_i2c_retry_total, 1);
			return rc;
		}
		if (ev.type != EV_ABS) {
			continue;
		}
		if (ev.code >= sizeof(axis_map)/sizeof(axis_map[0])) {
			continue;
		}
		int channel = axis_map[ev.code];
		if (channel < 0) {
			continue;
		}
		sensor_data_t point;
		point.timestamp_ns = (uint64_t)ev.time.tv_sec * 1000000000ULL+ (uint64_t)ev.time.tv_usec * 1000ULL;
		point.sensor_type  = SENSOR_MPU6050;
		point.channel      = channel;
		point.value        = ev.value;
		priv->cache[channel] = point;
		priv->cache_count++;

		if (priv->cache_count >= 6) {
			for (int i = 0; i < 6 && returned < max_count; i++) {
				out[returned] = priv->cache[i];
				returned++;
			}
			priv->cache_count = 0;
		}
	}
	return (ssize_t)(returned * sizeof(sensor_data_t));
}





static int mpu6050_hal_ioctl(void *handle, unsigned int cmd, void *arg){
	(void)handle;
	(void)cmd;
	(void)arg;
	return -ENOTSUP;
}


static int mpu6050_hal_close(void **handle){
	if (!handle || !*handle) {
		return -EINVAL;
	}
	mpu6050_hal_t *priv = (mpu6050_hal_t *)(*handle);
	libevdev_free(priv->evdev);
	close(priv->fd);
	free(priv);
	*handle = NULL;
	printf("[MPU6050 HAL] Closed\n");
	return 0;
}


static int mpu6050_hal_get_info(void *handle, char *info_buf, size_t buf_len){
	mpu6050_hal_t *priv = (mpu6050_hal_t *)handle;
	snprintf(info_buf, buf_len,"MPU6050: 6-axis IMU, driver=%s, bus=I2C1, addr=0x68",libevdev_get_name(priv->evdev));
	return 0;
}



const sensor_hal_ops_t mpu6050_hal_ops = {
	.open     = mpu6050_hal_open,
	.read     = mpu6050_hal_read,
	.ioctl    = mpu6050_hal_ioctl,
	.close    = mpu6050_hal_close,
	.get_info = mpu6050_hal_get_info,
};




