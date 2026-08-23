#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include "sensor_hal.h"




typedef struct {
	char temp_path[128];
	char hum_path[128];
	int  temp_fd;
	int  hum_fd;
}sht30_hal_t;


static int read_sysfs_int(int fd, int32_t *out_val){
	char buf[32];
	ssize_t n;
	lseek(fd, 0, SEEK_SET);
	n = read(fd, buf, sizeof(buf) - 1);
	if (n <= 0) {
		return -EIO;
	}
	buf[n] = '\0';
	*out_val = (int32_t)atoi(buf);
	return 0;
}


static int sht30_hal_open(const char *dev_path, void **handle){
	sht30_hal_t *priv = calloc(1, sizeof(sht30_hal_t));
	if (!priv) {
		return -ENOMEM;
	}
	snprintf(priv->temp_path, sizeof(priv->temp_path), "%s/temp1_input", dev_path);
	snprintf(priv->hum_path,  sizeof(priv->hum_path),  "%s/humidity1_input", dev_path);
	priv->temp_fd = open(priv->temp_path, O_RDONLY);
	if (priv->temp_fd < 0) {
		free(priv);
		return -errno;
	}
	priv->hum_fd = open(priv->hum_path, O_RDONLY);
	if (priv->hum_fd < 0) {
		close(priv->temp_fd);
		free(priv);
		return -errno;
	}
	*handle = priv;
	printf("[SHT30 HAL] Opened %s\n", dev_path);
	return 0;
}
static ssize_t sht30_hal_read(void *handle, void *buf, size_t len){
	sht30_hal_t *priv = (sht30_hal_t *)handle;
	sensor_data_t *out = (sensor_data_t *)buf;
	int32_t temp_raw, hum_raw;
	int ret;
	struct timespec ts;
	if (len < 2 * sizeof(sensor_data_t)) {
		return -EINVAL;
	}
	clock_gettime(CLOCK_MONOTONIC, &ts);
	ret = read_sysfs_int(priv->temp_fd, &temp_raw);
	if (ret < 0) {
		return ret;
	}
	ret = read_sysfs_int(priv->hum_fd, &hum_raw);
	if (ret < 0) {
		return ret;
	}
	out[0].timestamp_ns = (uint64_t)ts.tv_sec * 1000000000ULL+ (uint64_t)ts.tv_nsec;
	out[0].sensor_type  = SENSOR_SHT30;
	out[0].channel      = 0;
	out[0].value        = temp_raw;
	out[1].timestamp_ns = out[0].timestamp_ns;
	out[1].sensor_type  = SENSOR_SHT30;
	out[1].channel      = 1;
	out[1].value        = hum_raw;
	return (ssize_t)(2 * sizeof(sensor_data_t));
}
static int sht30_hal_ioctl(void *handle, unsigned int cmd, void *arg){
	(void)handle;
	(void)cmd;
	(void)arg;
	return -ENOTSUP;
}
static int sht30_hal_close(void **handle){
	if (!handle || !*handle) {
		return -EINVAL;
	}
	sht30_hal_t *priv = (sht30_hal_t *)(*handle);
	close(priv->temp_fd);
	close(priv->hum_fd);
	free(priv);
	*handle = NULL;
	printf("[SHT30 HAL] Closed\n");
	return 0;
}
static int sht30_hal_get_info(void *handle, char *info_buf, size_t buf_len){
	sht30_hal_t *priv = (sht30_hal_t *)handle;
	snprintf(info_buf, buf_len,"SHT30: Temp+Humidity, temp=%s, hum=%s",priv->temp_path, priv->hum_path);
	return 0;
}
const sensor_hal_ops_t sht30_hal_ops = {
	.open     = sht30_hal_open,
	.read     = sht30_hal_read,
	.ioctl    = sht30_hal_ioctl,
	.close    = sht30_hal_close,
	.get_info = sht30_hal_get_info,
};
