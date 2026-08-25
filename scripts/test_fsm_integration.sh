#!/bin/bash
# ============================================
# Edge-Omni 五态状态机集成测试脚本 - Day 12
# ============================================

set -e

COLLECTOR="../build/collector"
METRICS_URL="http://localhost:8080/metrics"
LOG_FILE="/tmp/fsm_test.log"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

COLLECTOR_PID=""

# 【trap】捕获任何退出信号，确保清理
# 比喻：医院的"无论手术成功还是失败，最后都要关手术室门"
cleanup() {
    echo ""
    echo "[清理] 终止 collector 进程..."
    if [ -n "$COLLECTOR_PID" ] && ps -p $COLLECTOR_PID > /dev/null 2>&1; then
        kill $COLLECTOR_PID 2>/dev/null
        sleep 1
        # 如果还在，强制杀死
        if ps -p $COLLECTOR_PID > /dev/null 2>&1; then
            kill -9 $COLLECTOR_PID 2>/dev/null
        fi
    fi
    # 兜底：强制清理所有 collector 进程
    pkill -9 -f "collector" 2>/dev/null || true
    sleep 1
    echo "[清理] 完成"
}
trap cleanup EXIT INT TERM

get_state() {
    curl -s $METRICS_URL | grep "^system_state " | awk '{print $2}'
}

get_state_name() {
    curl -s $METRICS_URL | grep "system_state_name" | grep -oP 'state="\K\w+'
}

wait_for_state() {
    local target=$1
    local timeout=$2
    local elapsed=0
    echo "    [等待] 目标状态: $target, 超时: ${timeout}s"
    while [ $elapsed -lt $timeout ]; do
        local current=$(get_state)
        if [ "$current" = "$target" ]; then
            echo "    [OK] 已进入状态 $target ($(get_state_name))"
            return 0
        fi
        sleep 1
        elapsed=$((elapsed + 1))
        echo "    [轮询] 当前状态: $current, 已等待 ${elapsed}s..."
    done
    echo "    [FAIL] 超时！目标 $target 未达成"
    return 1
}

# ============================================
# 测试开始
# ============================================

echo "========================================"
echo "  Edge-Omni FSM 集成测试"
echo "========================================"

# 0. 清理环境
echo "[0/5] 清理环境..."
pkill -9 -f "collector" 2>/dev/null || true
sleep 1
while ss -tlnp 2>/dev/null | grep -q ":8080"; do
    echo "    等待 8080 端口释放..."
    sleep 1
done
echo "    环境清理完成"

# 1. 确保驱动已加载
echo "[1/5] 检查并加载驱动..."
if ! lsmod | grep -q mpu6050; then
    echo "    mpu6050 未加载，正在加载..."
    sudo insmod ../kernel-drivers/mpu6050/mpu6050.ko
    sleep 5
else
    echo "    mpu6050 已加载，等待设备稳定..."
    sleep 2
fi
echo "    mpu6050 驱动已就绪"

# 2. 启动 collector
echo "[2/5] 启动 collector..."
if [ ! -f "$COLLECTOR" ]; then
    echo "错误: 找不到 $COLLECTOR，请先编译"
    exit 1
fi

if [ "$EUID" -eq 0 ]; then
    nohup $COLLECTOR > $LOG_FILE 2>&1 &
else
    nohup sudo $COLLECTOR > $LOG_FILE 2>&1 &
fi
COLLECTOR_PID=$!
echo "    collector PID: $COLLECTOR_PID"

# 3. 等待 RUNNING
echo "[3/5] 等待系统进入 RUNNING..."
sleep 2
wait_for_state 1 10
if [ $? -ne 0 ]; then
    echo -e "${RED}[FAIL] 系统未能进入 RUNNING${NC}"
    exit 1
fi
echo -e "${GREEN}[PASS] 系统已就绪，状态: RUNNING${NC}"

# 4. 模拟故障
echo "[4/5] 模拟 MPU6050 故障（rmmod mpu6050）..."
sudo rmmod mpu6050 || true
echo "    [等待] 观察是否进入 DEGRADED..."
wait_for_state 2 30
if [ $? -ne 0 ]; then
    echo -e "${RED}[FAIL] 未能进入 DEGRADED${NC}"
    exit 1
fi
echo -e "${GREEN}[PASS] 故障检测生效，状态: DEGRADED${NC}"

# 5. 恢复
echo "[5/5] 恢复 MPU6050（insmod mpu6050.ko）..."
sudo insmod ../kernel-drivers/mpu6050/mpu6050.ko || {
    echo "错误: 加载驱动失败"
    exit 1
}
echo "    [等待] 观察是否恢复 RUNNING..."
wait_for_state 1 30
if [ $? -ne 0 ]; then
    echo -e "${RED}[FAIL] 未能恢复 RUNNING${NC}"
    exit 1
fi
echo -e "${GREEN}[PASS] 自动恢复成功，状态: RUNNING${NC}"

echo ""
echo "========================================"
echo -e "${GREEN}  全部测试通过！${NC}"
echo "========================================"
echo "日志文件: $LOG_FILE"
echo "测试流程: RUNNING → DEGRADED → RUNNING ✓"
