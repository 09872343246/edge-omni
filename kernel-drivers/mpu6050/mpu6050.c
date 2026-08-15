#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/input.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>

#define MPU6050_ADDR              0x68
#define MPU6050_REG_PWR_MGMT_1    0x6B
#define MPU6050_REG_ACCEL_CONFIG  0x1C
#define MPU6050_VAL_WAKEUP        0x00
#define MPU6050_VAL_ACCEL_2G      0x00

#define MPU6050_REG_ACCEL_XOUT_H  0x3B
#define MPU6050_REG_GYRO_XOUT_H   0x43
#define MPU6050_SAMPLE_PERIOD_MS  10

struct mpu6050_data {
	struct i2c_client  *client;
	struct regmap      *regmap;
	struct input_dev   *input;
	struct hrtimer      timer;
};

static const struct regmap_config mpu6050_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = 0x7F,
};

static inline int mpu6050_read_reg(struct mpu6050_data *data, u8 reg, u8 *val)
{
	unsigned int tmp;
	int ret;

	ret = regmap_read(data->regmap, reg, &tmp);
	if (ret)
		dev_err(&data->client->dev,
			"read reg 0x%02X failed: %d\n", reg, ret);
	else
		*val = (u8)tmp;
	return ret;
}

static inline int mpu6050_write_reg(struct mpu6050_data *data, u8 reg, u8 val)
{
	int ret;

	ret = regmap_write(data->regmap, reg, val);
	if (ret)
		dev_err(&data->client->dev,
			"write reg 0x%02X = 0x%02X failed: %d\n", reg, val, ret);
	return ret;
}

static enum hrtimer_restart mpu6050_timer_callback(struct hrtimer *timer)
{
	struct mpu6050_data *data = container_of(timer, struct mpu6050_data, timer);

	dev_info(&data->client->dev, "MPU6050 timer tick\n");

	hrtimer_forward_now(timer, ms_to_ktime(MPU6050_SAMPLE_PERIOD_MS));
	return HRTIMER_RESTART;
}

static int mpu6050_probe(struct i2c_client *client)
{
	struct mpu6050_data *data;
	int ret;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	i2c_set_clientdata(client, data);

	data->regmap = devm_regmap_init_i2c(client, &mpu6050_regmap_config);
	if (IS_ERR(data->regmap)) {
		ret = PTR_ERR(data->regmap);
		dev_err(&client->dev, "regmap init failed: %d\n", ret);
		goto err_free_data;
	}

	ret = mpu6050_write_reg(data, MPU6050_REG_PWR_MGMT_1,
				MPU6050_VAL_WAKEUP);
	if (ret)
		goto err_free_data;

	ret = mpu6050_write_reg(data, MPU6050_REG_ACCEL_CONFIG,
				MPU6050_VAL_ACCEL_2G);
	if (ret)
		goto err_free_data;

	/* 6.18 内核：用 hrtimer_setup 替代 hrtimer_init + function 赋值 */
	hrtimer_setup(&data->timer, mpu6050_timer_callback,
		      CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hrtimer_start(&data->timer, ms_to_ktime(MPU6050_SAMPLE_PERIOD_MS),
		      HRTIMER_MODE_REL);

	dev_info(&client->dev, "MPU6050 probed, +-2g range set\n");
	return 0;

err_free_data:
	kfree(data);
	return ret;
}

static void mpu6050_remove(struct i2c_client *client)
{
	struct mpu6050_data *data = i2c_get_clientdata(client);

	hrtimer_cancel(&data->timer);
	kfree(data);
	dev_info(&client->dev, "MPU6050 removed\n");
}

static const struct i2c_device_id mpu6050_id[] = {
	{ "mpu6050", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mpu6050_id);

static const struct of_device_id mpu6050_of_match[] = {
	{ .compatible = "invensense,mpu6050" },
	{ }
};
MODULE_DEVICE_TABLE(of, mpu6050_of_match);

static struct i2c_driver mpu6050_driver = {
	.driver = {
		.name           = "mpu6050",
		.of_match_table = mpu6050_of_match,
	},
	.probe    = mpu6050_probe,
	.remove   = mpu6050_remove,
	.id_table = mpu6050_id,
};

module_i2c_driver(mpu6050_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("MPU6050 Input driver for Orange Pi Zero (6.18 kernel)");
