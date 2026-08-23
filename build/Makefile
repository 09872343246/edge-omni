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
TEST_SRC = $(SRC_DIR)/core/ring_buffer_test.c
OBJECTS = $(SOURCES:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)
all: test_ring_buffer
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
test_ring_buffer: $(OBJECTS) $(TEST_OBJ)
	$(CC) $(OBJECTS) $(TEST_OBJ) -o $@ $(LDFLAGS)
clean:
	rm -f $(OBJECTS) $(TEST_OBJ) test_ring_buffer
.PHONY: all clean
