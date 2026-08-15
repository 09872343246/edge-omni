savedcmd_mpu6050.mod := printf '%s\n'   mpu6050.o | awk '!x[$$0]++ { print("./"$$0) }' > mpu6050.mod
