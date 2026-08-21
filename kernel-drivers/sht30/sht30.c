#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/hwmon.h>
#include <linux/workqueue.h>

struct sht30_data {
	struct i2c_client *client;
	struct delayed_work dwork;
	struct device *hwmon_dev;
	s32 temp;
	s32 humidity;
	bool valid;
};

static u8 sht30_crc8(const u8 *data, int len)
{
	const u8 polynomial = 0x31;
	u8 crc = 0xFF;
	int i, j;

	for (i = 0; i < len; i++) {
		crc ^= data[i];
		for (j = 0; j < 8; j++) {
			if (crc & 0x80)
				crc = (crc << 1) ^ polynomial;
			else
				crc <<= 1;
		}
	}
	return crc;
}

static int sht30_send_cmd(struct i2c_client *client, u16 cmd)
{
	u8 buf[2] = {cmd >> 8, cmd & 0xFF};
	int ret;

	ret = i2c_master_send(client, buf, 2);
	if (ret < 0)
		return ret;
	if (ret != 2)
		return -EIO;
	return 0;
}

static int sht30_read_data(struct i2c_client *client, u8 *buf, int len)
{
	int ret;

	ret = i2c_master_recv(client, buf, len);
	if (ret < 0)
		return ret;
	if (ret != len)
		return -EIO;
	return 0;
}

static void sht30_work_handler(struct work_struct *work)
{
	struct sht30_data *data = container_of(work, struct sht30_data, dwork.work);
	struct device *dev = &data->client->dev;
	u8 buf[6];
	u16 temp_raw, hum_raw;
	int ret;

	ret = sht30_send_cmd(data->client, 0x2400);
	if (ret) {
		dev_err(dev, "Failed to send measure command: %d\n", ret);
		goto reschedule;
	}

	msleep(15);

	ret = sht30_read_data(data->client, buf, 6);
	if (ret) {
		dev_err(dev, "Failed to read data: %d\n", ret);
		goto reschedule;
	}

	if (sht30_crc8(buf, 2) != buf[2]) {
		dev_err(dev, "Temperature CRC error!\n");
		goto reschedule;
	}
	if (sht30_crc8(buf + 3, 2) != buf[5]) {
		dev_err(dev, "Humidity CRC error!\n");
		goto reschedule;
	}

	temp_raw = (buf[0] << 8) | buf[1];
	hum_raw = (buf[3] << 8) | buf[4];

	data->temp = ((21965 * (s32)temp_raw) >> 13) - 45000;
	data->humidity = ((12500 * (s32)hum_raw) >> 13);
	data->valid = true;

	dev_dbg(dev, "Temp: %d mC, Humidity: %d milli%%\n",
		data->temp, data->humidity);

reschedule:
	schedule_delayed_work(&data->dwork, msecs_to_jiffies(1000));
}

static ssize_t temp1_input_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	struct sht30_data *data = dev_get_drvdata(dev);

	if (!data->valid)
		return -ENODATA;

	return sprintf(buf, "%d\n", data->temp);
}

static ssize_t humidity1_input_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct sht30_data *data = dev_get_drvdata(dev);

	if (!data->valid)
		return -ENODATA;

	return sprintf(buf, "%d\n", data->humidity);
}

static DEVICE_ATTR(temp1_input, 0444, temp1_input_show, NULL);
static DEVICE_ATTR(humidity1_input, 0444, humidity1_input_show, NULL);

static struct attribute *sht30_attrs[] = {
	&dev_attr_temp1_input.attr,
	&dev_attr_humidity1_input.attr,
	NULL,
};

static struct attribute_group sht30_attr_group = {
	.attrs = sht30_attrs,
};

static const struct attribute_group *sht30_attr_groups[] = {
	&sht30_attr_group,
	NULL,
};

static int sht30_probe(struct i2c_client *client)
{
	struct sht30_data *data;
	struct device *dev = &client->dev;
	int ret;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	data->valid = false;

	ret = sht30_send_cmd(client, 0x30A2);
	if (ret) {
		dev_err(dev, "Soft reset failed: %d\n", ret);
		return ret;
	}
	msleep(2);

	INIT_DELAYED_WORK(&data->dwork, sht30_work_handler);
	schedule_delayed_work(&data->dwork, msecs_to_jiffies(1000));

	data->hwmon_dev = hwmon_device_register_with_groups(dev, "sht30", data,
							    sht30_attr_groups);
	if (IS_ERR(data->hwmon_dev)) {
		ret = PTR_ERR(data->hwmon_dev);
		dev_err(dev, "Failed to register hwmon: %d\n", ret);
		cancel_delayed_work_sync(&data->dwork);
		return ret;
	}

	i2c_set_clientdata(client, data);
	dev_info(dev, "SHT30 probed successfully\n");
	return 0;
}

static void sht30_remove(struct i2c_client *client)
{
	struct sht30_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	cancel_delayed_work_sync(&data->dwork);
}

static const struct i2c_device_id sht30_id[] = {
	{"sht30", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, sht30_id);

static const struct of_device_id sht30_of_match[] = {
	{.compatible = "sensirion,sht30"},
	{}
};
MODULE_DEVICE_TABLE(of, sht30_of_match);

static struct i2c_driver sht30_driver = {
	.driver = {
		.name = "sht30",
		.of_match_table = sht30_of_match,
	},
	.probe = sht30_probe,
	.remove = sht30_remove,
	.id_table = sht30_id,
};

module_i2c_driver(sht30_driver);

MODULE_AUTHOR("ChongZhou");
MODULE_DESCRIPTION("SHT30 temperature and humidity sensor driver");
MODULE_LICENSE("GPL");
