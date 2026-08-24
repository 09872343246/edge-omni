CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11
CFLAGS += -I../app/include
CFLAGS += -I/usr/include/libevdev-1.0
CFLAGS += -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=200809L

LDFLAGS = -levdev
LDFLAGS += -lpthread -latomic
SRC_DIR = ../app
SOURCES = $(SRC_DIR)/core/ring_buffer.c \
          $(SRC_DIR)/hal/mpu6050_hal.c \
          $(SRC_DIR)/hal/sht30_hal.c \
          $(SRC_DIR)/hal/hal_register.c
MAIN_SOURCE = $(SRC_DIR)/main.c
TEST_SRC = $(SRC_DIR)/core/ring_buffer_test.c



CORE_OBJECTS = $(CORE_SOURCES:.c=.o)
MAIN_OBJECT  = $(MAIN_SOURCE:.c=.o)
TEST_OBJ     = $(TEST_SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
.PHONY: all


all: collector
collector: $(CORE_OBJECTS) $(MAIN_OBJECT)
	$(CC) $(CORE_OBJECTS) $(MAIN_OBJECT) -o $@ $(LDFLAGS)
	@echo "=== collector 编译完成 ==="
test_ring_buffer: $(OBJECTS) $(TEST_OBJ)
	$(CC) $(OBJECTS) $(TEST_OBJ) -o $@ $(LDFLAGS)
	@echo "=== test_ring_buffer 编译完成 ==="


clean:
	rm -f $(CORE_OBJECTS) $(MAIN_OBJECT) $(TEST_OBJ) collector test_ring_buffer
	@echo "=== 清理完成 ==="
