#!/bin/bash
set -e
echo "=== Edge-Omni CPU 战斗模式启动 ==="
echo "[1/4] 解除实时调度限制..."
echo -1 > /proc/sys/kernel/sched_rt_runtime_us
if [ $? -eq 0 ]; then
	echo "      ✓ 实时调度限制已解除"
else
	echo "      ✗ 解除失败，请检查是否以 root 运行"
	exit 1
fi


echo "[2/4] 配置 CPU0 性能模式..."
if [ -d /sys/devices/system/cpu/cpu0/cpufreq ]; then
	echo performance > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
	CUR_FREQ=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq)
	echo "      ✓ CPU0 已设为 performance 模式，当前频率: ${CUR_FREQ} Hz"
else
	echo "      ! CPU0 不支持 cpufreq，跳过"
fi


echo "[3/4] 配置 CPU1 性能模式..."
if [ -d /sys/devices/system/cpu/cpu1/cpufreq ]; then
	echo performance > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
	CUR_FREQ=$(cat /sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq)
	echo "      ✓ CPU1 已设为 performance 模式，当前频率: ${CUR_FREQ} Hz"
else
	echo "      ! CPU1 不支持 cpufreq，跳过"
fi


echo "[4/4] 配置中断亲和性..."
if [ -f /proc/irq/default_smp_affinity ]; then
	echo 3 > /proc/irq/default_smp_affinity
	echo "      ✓ 中断已均匀分配至 CPU0 和 CPU1"
else
	echo "      ! default_smp_affinity 不存在，跳过"
fi


echo "=== Edge-Omni CPU 战斗模式配置完成 ==="
echo "      采集线程可以安全使用 SCHED_FIFO 优先级 99"
echo "      CPU 频率已锁定，中断已分散"
