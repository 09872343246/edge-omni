CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11
CFLAGS += -I../app/include
CFLAGS += -I/usr/include/libevdev-1.0
CFLAGS += -I/usr/local/include/modbus
CFLAGS += -D_POSIX_C_SOURCE=200809L


LDFLAGS = -levdev -lpthread -latomic -lsqlite3
LDFLAGS += -L/usr/local/lib -lmodbus


SRC_DIR = ../app
CORE_SOURCES =	$(SRC_DIR)/core/ring_buffer.c \
		$(SRC_DIR)/core/fsm.c \
		$(SRC_DIR)/core/db_manager.c \
		$(SRC_DIR)/core/metrics.c \
		$(SRC_DIR)/hal/mpu6050_hal.c \
		$(SRC_DIR)/hal/sht30_hal.c \
		$(SRC_DIR)/hal/hal_register.c \
		$(SRC_DIR)/modbus/modbus_server.c

MAIN_SOURCE = $(SRC_DIR)/main.c
TEST_SRC = $(SRC_DIR)/core/ring_buffer_test.c



CORE_OBJECTS = $(CORE_SOURCES:.c=.o)
MAIN_OBJECT  = $(MAIN_SOURCE:.c=.o)
TEST_OBJECT  = $(TEST_SRC:.c=.o)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

ASAN_CFLAGS  = -fsanitize=address -fno-omit-frame-pointer -g
ASAN_LDFLAGS = -fsanitize=address



.PHONY: all clean


all: collector
collector: $(CORE_OBJECTS) $(MAIN_OBJECT)
	$(CC) $(CORE_OBJECTS) $(MAIN_OBJECT) -o $@ $(LDFLAGS)
	@echo "=== collector 编译完成 ==="
collector_asan: $(CORE_OBJECTS) $(MAIN_OBJECT)
	$(CC) $(CORE_OBJECTS) $(MAIN_OBJECT) -o $@ $(LDFLAGS) $(ASAN_LDFLAGS)
	@echo "=== collector_asan (带内存检测) 编译完成 ==="

test_ring_buffer: $(CORE_OBJECTS) $(TEST_OBJECT)
	$(CC) $(CORE_OBJECTS) $(TEST_OBJECT) -o $@ $(LDFLAGS)
	@echo "=== test_ring_buffer 编译完成 ==="


clean:
	rm -f $(CORE_OBJECTS) $(MAIN_OBJECT) $(TEST_OBJECT) collector test_ring_buffer
	@echo "=== 清理完成 ==="
