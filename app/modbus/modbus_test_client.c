#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <modbus.h>
#include <errno.h>

int main(int argc, char *argv[]){

	const char *ip = (argc > 1) ? argv[1] : "127.0.0.1";
	int port = (argc > 2) ? atoi(argv[2]) : 502;

	printf("[test] 连接 %s:%d ...\n", ip, port);
	modbus_t *ctx = modbus_new_tcp(ip, port);

	if (!ctx) {
		fprintf(stderr, "[test] 创建 modbus 上下文失败\n");
		return 1;
	}
	modbus_set_response_timeout(ctx, 3, 0);
	modbus_set_byte_timeout(ctx, 3, 0);

	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "[test] 连接失败: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return 1;
	}
	printf("[test] 连接成功!\n");




	uint16_t tab_reg[4];
	printf("\n[test] === 测试1：读取保持寄存器 (addr=0, count=4) ===\n");
	int rc = modbus_read_registers(ctx, 0, 4, tab_reg);
	if (rc == -1) {
		fprintf(stderr, "[test] 读取失败: %s\n", modbus_strerror(errno));

	} else {
	printf("[test] 读取成功! 收到 %d 个寄存器:\n", rc);
	printf("  [0] 温度×100: %d  →  %.2f ℃\n", tab_reg[0], tab_reg[0] / 100.0);
	printf("  [1] 湿度×100: %d  →  %.2f %%\n", tab_reg[1], tab_reg[1] / 100.0);
	printf("  [2] 加速度X:   %d\n", tab_reg[2]);
	printf("  [3] 加速度Y:   %d\n", tab_reg[3]);
	}



	printf("\n[test] === 测试2：非法地址 (addr=10, count=1) ===\n");
	rc = modbus_read_registers(ctx, 10, 1, tab_reg);
	if (rc == -1) {
		printf("[test] 收到异常响应 ✅（预期行为）: %s\n", modbus_strerror(errno));
	} else {
		printf("[test] 异常! 服务器不应允许这个地址\n");
	}


	printf("\n[test] === 测试3：连续5次非法功能码，触发黑名单 ===\n");
	uint8_t raw_req[12];
	int tid = 1;
	for (int i = 0; i < 5; i++) {

		raw_req[0] = tid >> 8;
		raw_req[1] = tid & 0xFF;
		raw_req[2] = 0;
		raw_req[3] = 0;
		raw_req[4] = 0;
		raw_req[5] = 2;
		raw_req[6] = 1;
		raw_req[7] = 0x08;

		int sent = modbus_send_raw_request(ctx, raw_req + 6, 2);

		if (sent > 0) {
			uint8_t rsp[MODBUS_TCP_MAX_ADU_LENGTH];
			rc = modbus_receive_confirmation(ctx, rsp);
			if (rc > 0) {
				printf("[test] 第%d次非法请求: 收到异常响应\n", i+1);
			}
		}
		tid++;
		usleep(100000);
	}

	modbus_close(ctx);
	modbus_free(ctx);


	printf("\n[test] === 测试4：验证黑名单（等待2秒后重连）===\n");
	sleep(2);

	ctx = modbus_new_tcp(ip, port);

	modbus_set_response_timeout(ctx, 2, 0);

	if (modbus_connect(ctx) == -1) {

		printf("[test] 重新连接失败 ✅: %s\n", modbus_strerror(errno));
		printf("[test] ✅ 黑名单生效! IP 已被 iptables 封禁 300 秒\n");
	} else {
		printf("[test] 重新连接成功（可能封禁未生效或已解封）\n");
		modbus_close(ctx);
	}
	modbus_free(ctx);

	return 0;
}
