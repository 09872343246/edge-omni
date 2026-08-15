#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/input.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

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
	struct work_struct  work;
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

static void mpu6050_work_handler(struct work_struct *work)
{
	struct mpu6050_data *data = container_of(work, struct mpu6050_data, work);
	u8 buf[14];
	s16 accel_x, accel_y, accel_z;
	s16 gyro_x, gyro_y, gyro_z;
	int ret;

	ret = regmap_bulk_read(data->regmap, MPU6050_REG_ACCEL_XOUT_H, buf, 14);
	if (ret) {
		dev_err(&data->client->dev, "bulk read failed: %d\n", ret);
		return;
	}

	accel_x = (s16)((buf[0] << 8) | buf[1]);
	accel_y = (s16)((buf[2] << 8) | buf[3]);
	accel_z = (s16)((buf[4] << 8) | buf[5]);
	gyro_x  = (s16)((buf[8]  << 8) | buf[9]);
	gyro_y  = (s16)((buf[10] << 8) | buf[11]);
	gyro_z  = (s16)((buf[12] << 8) | buf[13]);

	input_report_abs(data->input, ABS_X,  accel_x);
	input_report_abs(data->input, ABS_Y,  accel_y);
	input_report_abs(data->input, ABS_Z,  accel_z);
	input_report_abs(data->input, ABS_RX, gyro_x);
	input_report_abs(data->input, ABS_RY, gyro_y);
	input_report_abs(data->input, ABS_RZ, gyro_z);
	input_sync(data->input);
}

static enum hrtimer_restart mpu6050_timer_callback(struct hrtimer *timer)
{
	struct mpu6050_data *data = container_of(timer, struct mpu6050_data, timer);

	schedule_work(&data->work);  /* 调度 workqueue */

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

	/* ---- 创建 Input 设备 ---- */
	data->input = devm_input_allocate_device(&client->dev);
	if (!data->input) {
		ret = -ENOMEM;
		goto err_free_data;
	}

	data->input->name = "MPU6050";
	data->input->id.bustype = BUS_I2C;

	input_set_abs_params(data->input, ABS_X,  -32768, 32767, 0, 0);
	input_set_abs_params(data->input, ABS_Y,  -32768, 32767, 0, 0);
	input_set_abs_params(data->input, ABS_Z,  -32768, 32767, 0, 0);
	input_set_abs_params(data->input, ABS_RX, -32768, 32767, 0, 0);
	input_set_abs_params(data->input, ABS_RY, -32768, 32767, 0, 0);
	input_set_abs_params(data->input, ABS_RZ, -32768, 32767, 0, 0);

	ret = input_register_device(data->input);
	if (ret) {
		dev_err(&client->dev, "input register failed: %d\n", ret);
		goto err_free_data;
	}

	/* ---- 初始化 workqueue ---- */
	INIT_WORK(&data->work, mpu6050_work_handler);

	/* ---- 启动 hrtimer ---- */
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
	cancel_work_sync(&data->work);  /* 等当前 work 执行完 */
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

