#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/input.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

#define MPU6050_ADDR 0x68
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_ACCEL_CONFIG 0x1C
#define MPU6050_VAL_WAKEUP 0x00
#define MPU6050_VAL_ACCEL_2G 0x00


struct mpu6050_data {
	struct i2c_client  *client;
	struct regmap *regmap;
	struct input_dev *input;
	struct hrtimer timer;
};


static int mpu6050_probe(struct i2c_client *client,const struct i2c_device_id *id){
	struct mpu6050_data *data;
	int ret;
	data = kzalloc(sizeof(*data),GFP_KERNEL);
	if(!data)
		return -ENOMEM;
	data->client = client;
	i2c_set_clientdata(client,data);
	data->regmap = devm_regmap_init_i2c(client,&mpu6050_regmap_config);
	if(IS_ERR(data->regmap)){
		ret = PTR_ERR(data->regmap);
		dev_err(&client->dev,"regmap init failed: %d\n",ret);
		goto err_free_data;
	}
	ret = regmap_write(data->regmap,MPU6050_REG_PWR_MGMT_1,MPU6050_VAL_WAKEUP);
	if(ret){
		dev_err(&client->dev,"wakeup failed: %d\n",ret);
		goto err_free_data;
	}

	ret = regmap_write(data->regmap,MPU6050_REG_ACCEL_CONFIG,MPU6050_VAL_ACCEL_2G);
        if(ret){
                dev_err(&client->dev,"accel config failed: %d\n",ret);
                goto err_free_data;
        }
	dev_info(&client->dev,"MPU6050 probed,±2g range set\n");
	return 0;


err_free_data:
	kfree(data);
	return ret;
}

static const struct regmap_config mpu6050_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = 0x7F,
};
