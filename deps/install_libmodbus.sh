#!/bin/bash
# deps/install_libmodbus.sh

set -e  # 遇到错误立即退出，不继续执行

SRC_DIR="/root/modbus_build"
INSTALL_PREFIX="/usr/local"

echo "[*] Installing libmodbus..."

# 如果已经安装过，跳过
if [ -f "${INSTALL_PREFIX}/lib/libmodbus.a" ]; then
    echo "[*] libmodbus already installed, skipping."
    exit 0
fi

mkdir -p "${SRC_DIR}"
cd "${SRC_DIR}"

# 下载
if [ ! -f "libmodbus-3.1.10.tar.gz" ]; then
    wget https://github.com/stephane/libmodbus/releases/download/v3.1.10/libmodbus-3.1.10.tar.gz
fi

# 编译安装
tar -xzf libmodbus-3.1.10.tar.gz
cd libmodbus-3.1.10
./configure --prefix="${INSTALL_PREFIX}" --enable-static --disable-shared
make -j4
make install
ldconfig

echo "[+] libmodbus installed successfully!"
