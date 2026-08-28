#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdatomic.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "include/thread_config.h"
#include "include/fsm.h"
#include "sensor_hal.h"
#include "db_manager.h"
#include "include/metrics.h"
#include "modbus_server.h"


fsm_context_t g_fsm;


void *collector_thread(void *arg){
	pthread_setname_np(pthread_self(),COLLECTOR_THREAD_NAME);
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(COLLECTOR_CPU_CORE,&cpuset);
	int ret = pthread_setaffinity_np(
		pthread_self(),
		sizeof(cpu_set_t),
		&cpuset
	);
	if(ret != 0){
		perror("pthread_setaffinity_np failed");
		return NULL;
	}
	printf("[collector]已绑定到cpu核心%d\n",COLLECTOR_CPU_CORE);


	struct sched_param param;
	param.sched_priority = COLLECTOR_PRIORITY;
	ret = pthread_setschedparam(
		pthread_self(),
		COLLECTOR_SCHED_POLICY,
		&param
	);
	if(ret != 0){
		perror("pthread_setschedparam failed");
		return NULL;
	}
	printf("[collector]已设置SCHED_FIFO,优先级%d\n",COLLECTOR_PRIORITY);



	const sensor_hal_ops_t *mpu_ops = hal_get_ops("mpu6050");
	const sensor_hal_ops_t *sht_ops = hal_get_ops("sht30");
	void *mpu_handle = NULL;
	void *sht_handle = NULL;

        sensor_data_t mpu_buf[6];
        sensor_data_t sht_buf[2];

	if (mpu_ops) {
		for (int i = 0; i <= 3 && mpu_handle == NULL; i++) {
			char path[32];
			snprintf(path, sizeof(path), "/dev/input/event%d", i);
			if (access(path, F_OK) == 0) {
				if (mpu_ops->open(path, &mpu_handle) == 0) {
					printf("[collector] MPU6050 使用 %s\n", path);
					sensor_data_t warmup[6];
					int warmup_ok = 0;
					for (int w = 0; w < 50; w++) {
						//ssize_t n = mpu_ops->read(mpu_handle, warmup, sizeof(warmup));
						struct timespec ts_mpu_start, ts_mpu_end;
						clock_gettime(CLOCK_MONOTONIC, &ts_mpu_start);
						ssize_t n = mpu_ops->read(mpu_handle, mpu_buf, sizeof(mpu_buf));
						clock_gettime(CLOCK_MONOTONIC, &ts_mpu_end);
						int latency_mpu = (ts_mpu_end.tv_sec - ts_mpu_start.tv_sec) * 1000000+ (ts_mpu_end.tv_nsec - ts_mpu_start.tv_nsec) / 1000;
						record_latency(latency_mpu);



						if (n > 0) {
							printf("[collector] MPU6050 热身成功\n");
							warmup_ok = 1;
							break;
						}
						usleep(10000);
					}
					if (!warmup_ok) {
						printf("[collector] MPU6050 热身失败，关闭 handle\n");
						mpu_ops->close(&mpu_handle);
						mpu_handle = NULL;
					} else {
						break;
					}
				}
			}
		}
		if (!mpu_handle) {
			printf("[collector] ⚠️ 未找到 MPU6050 设备\n");
		}
	}
	if (sht_ops) {
		int ret = sht_ops->open("/sys/class/hwmon/hwmon0", &sht_handle);
		if (ret < 0) {
			printf("[collector] SHT30 open 失败: %d\n", ret);
			sht_handle = NULL;
		}
	}
	int mpu_fail_streak = 0;
	int mpu_no_data_streak = 0;
	int sht_fail_streak = 0;
	int32_t latest_ax = 0;
	int32_t latest_ay = 0;


	while (1) {
		if (mpu_ops) {
			if (!mpu_handle) {
				for (int i = 0; i <= 3 && mpu_handle == NULL; i++) {
					char path[32];
					snprintf(path, sizeof(path), "/dev/input/event%d", i);
					if (access(path, F_OK) == 0) {
						if (mpu_ops->open(path, &mpu_handle) == 0) {
							printf("[collector] MPU6050 重新打开 %s\n", path);
							break;
						}
					}
				}
				if (!mpu_handle) {
					mpu_fail_streak++;
					atomic_fetch_add(&g_fsm.mpu_fail_count, 1);
					printf("[collector] MPU6050 无法重新打开 #%d\n", mpu_fail_streak);
					if (mpu_fail_streak >= 3) {
						printf("[collector] >>> 连续失败3次，触发 DEGRADED!\n");
						fsm_transition(&g_fsm, EVENT_MPU_FAIL);
					}
					goto mpu_done;
				}
			}

			ssize_t n = mpu_ops->read(mpu_handle, mpu_buf, sizeof(mpu_buf));
			if (n < 0) {
				mpu_fail_streak++;
				mpu_no_data_streak = 0;
				atomic_fetch_add(&g_fsm.mpu_fail_count, 1);
				printf("[collector] MPU6050 读失败 #%d (ret=%zd)\n", mpu_fail_streak, n);

				if (mpu_handle) {
					printf("[collector] 关闭旧 handle，尝试重新探测...\n");
					mpu_ops->close(&mpu_handle);
					mpu_handle = NULL;
				}
				for (int i = 0; i <= 3 && mpu_handle == NULL; i++) {
					char path[32];
					snprintf(path, sizeof(path), "/dev/input/event%d", i);
					if (access(path, F_OK) == 0) {
						if (mpu_ops->open(path, &mpu_handle) == 0) {
							printf("[collector] MPU6050 重新打开 %s\n", path);
							break;
						}
					}
				}




				if (mpu_fail_streak >= 3) {
					printf("[collector] >>> 连续失败3次，触发 DEGRADED!\n");
					fsm_transition(&g_fsm, EVENT_MPU_FAIL);
				}
			} else if (n > 0) {
				int count = n / sizeof(sensor_data_t);
				if (mpu_fail_streak > 0 && fsm_get_state(&g_fsm) == STATE_DEGRADED) {
					printf("[collector] MPU6050 恢复，触发 RUNNING!\n");
					fsm_transition(&g_fsm, EVENT_MPU_RESTORED);

				}
				mpu_fail_streak = 0;
				mpu_no_data_streak = 0;
				printf("[collector] MPU6050 数据: ");
				const char *name[] = {"AX","AY","AZ","GX","GY","GZ"};
				for (int i = 0; i < count; i++) {
					printf("%s=%d ", name[mpu_buf[i].channel], mpu_buf[i].value);
				}
				printf("\n");


				int ts = (int)time(NULL);
				for (int i = 0; i < count; i++) {
					db_insert_sensor(ts, "mpu6050", name[mpu_buf[i].channel], mpu_buf[i].value);
				if (mpu_buf[i].channel == 0) latest_ax = mpu_buf[i].value;

				if (mpu_buf[i].channel == 1) latest_ay = mpu_buf[i].value;

				}

			} else {
				mpu_no_data_streak++;
				if (mpu_no_data_streak >= 10) {
					mpu_fail_streak++;
					mpu_no_data_streak = 0;
					atomic_fetch_add(&g_fsm.mpu_fail_count, 1);
					printf("[collector] MPU6050 连续无数据，视为失败 #%d\n", mpu_fail_streak);
					if (mpu_handle) {
						printf("[collector] 关闭旧 handle，尝试重新探测...\n");
						mpu_ops->close(&mpu_handle);
						mpu_handle = NULL;
					}
					for (int i = 0; i <= 3 && mpu_handle == NULL; i++) {
						char path[32];
						snprintf(path, sizeof(path), "/dev/input/event%d", i);
						if (access(path, F_OK) == 0) {
							if (mpu_ops->open(path, &mpu_handle) == 0) {
								printf("[collector] MPU6050 重新打开 %s\n", path);
								break;
							}
						}
					}
					if (mpu_fail_streak >= 3) {
						printf("[collector] >>> 连续失败3次，触发 DEGRADED!\n");
						fsm_transition(&g_fsm, EVENT_MPU_FAIL);
					}
				}
			}



		}
mpu_done:
		;

		if (sht_ops && sht_handle) {
//			ssize_t n = sht_ops->read(sht_handle, sht_buf, sizeof(sht_buf));
			struct timespec ts_sht_start, ts_sht_end;
			clock_gettime(CLOCK_MONOTONIC, &ts_sht_start);
			ssize_t n = sht_ops->read(sht_handle, sht_buf, sizeof(sht_buf));
			clock_gettime(CLOCK_MONOTONIC, &ts_sht_end);
			int latency_sht = (ts_sht_end.tv_sec - ts_sht_start.tv_sec) * 1000000 + (ts_sht_end.tv_nsec - ts_sht_start.tv_nsec) / 1000;
			record_latency(latency_sht);
			if (n < 0) {
				sht_fail_streak++;
				atomic_fetch_add(&g_fsm.i2c_retry_count, 1);
				printf("[collector] SHT30 读失败 #%d (ret=%zd)\n", sht_fail_streak, n);

				if (mpu_fail_streak >= 3 && sht_fail_streak >= 3) {
					printf("[collector] >>> 所有传感器离线，可能I2C死锁!\n");
					fsm_transition(&g_fsm, EVENT_I2C_DEADLOCK);
				}
			} else if (n > 0) {
				int count = n / sizeof(sensor_data_t);
				if (sht_fail_streak > 0) {
					printf("[collector] SHT30 恢复成功\n");
				}
				sht_fail_streak = 0;
				atomic_store(&g_fsm.temp_raw, sht_buf[0].value);
				atomic_store(&g_fsm.hum_raw, sht_buf[1].value);
				printf("[collector] SHT30 数据: ");
				for (int i = 0; i < count; i++) {
					printf("[%s ch=%d val=%d] ",sht_buf[i].sensor_type == SENSOR_SHT30 ? "SHT" : "?",sht_buf[i].channel, sht_buf[i].value);
				}
				printf("\n");

				int ts = (int)time(NULL);
				for (int i = 0; i < count; i++) {
					const char *ch_name = (sht_buf[i].channel == 0) ? "temperature" : "humidity";
					db_insert_sensor(ts, "sht30", ch_name, sht_buf[i].value);
				}



				modbus_reg_data_t mb_data;
				mb_data.temperature = (uint16_t)(sht_buf[0].value / 10);
				mb_data.humidity    = (uint16_t)(sht_buf[1].value / 10);
				mb_data.accel_x     = (uint16_t)latest_ax;
				mb_data.accel_y     = (uint16_t)latest_ay;
				modbus_server_update_data(&mb_data);
			}
		}
		atomic_store(&g_system_state, (int)fsm_get_state(&g_fsm));
		struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000};
		nanosleep(&ts, NULL);
	}
	if (mpu_ops && mpu_handle) mpu_ops->close(&mpu_handle);
	if (sht_ops && sht_handle) sht_ops->close(&sht_handle);

	return NULL;
}






static void send_http_response(int client_fd, int status_code,const char *content_type,const char *body, int body_len){
	char header[512];
	const char *status_str;
	status_str = (status_code == 200) ? "200 OK" : "404 Not Found";
	int header_len = snprintf(header, sizeof(header),
		"HTTP/1.1 %s\r\n"
		"Content-Type: %s\r\n"
		"Content-Length: %d\r\n"
		"Connection: close\r\n"
		"\r\n",
		status_str, content_type, body_len);
	int sent = 0;
	while (sent < header_len) {
		int n = send(client_fd, header + sent, header_len - sent, 0);
		if (n < 0) {
			perror("[web] send header failed");
			return;
		}
		sent += n;
	}
	sent = 0;
        while (sent < body_len) {
                int n = send(client_fd, body + sent, body_len - sent, 0);
                if (n < 0) {
                        perror("[web] send body failed");
                        return;
                }
                sent += n;
        }
}


void *web_thread(void *arg){
	pthread_setname_np(pthread_self(), MAIN_THREAD_NAME);
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(MAIN_CPU_CORE, &cpuset);
	int ret = pthread_setaffinity_np(
		pthread_self(),
		sizeof(cpu_set_t),
		&cpuset
	);

	if (ret != 0) {
		perror("web_thread: pthread_setaffinity_np failed");
		return NULL;
	}
	printf("[web] 已绑定到 CPU核心%d\n", MAIN_CPU_CORE);
	struct sched_param param;
	param.sched_priority = MAIN_PRIORITY;
	ret = pthread_setschedparam(
		pthread_self(),
		MAIN_SCHED_POLICY,
		&param
	);
        if (ret != 0) {
                perror("web_thread: pthread_setschedparam failed");
                return NULL;
        }
	printf("[web] 已设置 SCHED_OTHER,优先级 %d\n", MAIN_PRIORITY);


	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		perror("[web] socket创建失败");
		return NULL;
	}
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("[web] bind失败（端口8080可能被占用）");
		close(server_fd);
		return NULL;
	}
	if (listen(server_fd, 5) < 0) {
		perror("[web] listen失败");
		close(server_fd);
		return NULL;
	}
	printf("[web] HTTP服务器启动,监听 0.0.0.0:8080\n");
	printf("[web] 测试命令: curl http://localhost:8080/\n");



	while (1) {

/*		system_state_t state = fsm_get_state(&g_fsm);
		if (state != prev_state) {
			switch (state) {
				case STATE_INIT:
					printf("[web] >>> 状态变化: INIT (初始化中)\n");
					break;

	                        case STATE_RUNNING:
        	                        printf("[web] >>> 状态变化: RUNNING (全功能恢复!)\n");
                			break;

           			case STATE_DEGRADED:
                	                printf("[web] >>> 状态变化: DEGRADED (降级模式,仅温湿度)\n");
				        break;

                        	case STATE_RECOVERING:
                                	printf("[web] >>> 状态变化: RECOVERING (I2C总线恢复中...)\n");
                                	break;

               			case STATE_FAULT:
					printf("[web] >>> 状态变化: FAULT (系统故障,即将重启)\n");
					break;
				default:
					break;
			}
			prev_state = state;
		}
		struct timespec ts = {.tv_sec = 0, .tv_nsec = 1000};
		nanosleep(&ts, NULL);
	}
	return NULL;

*/
		struct sockaddr_in client_addr;
		socklen_t addr_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
		if (client_fd < 0) {
			continue;
		}

		struct timeval tv;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

		char req[512] = {0};
		int n = recv(client_fd, req, sizeof(req) - 1, 0);
		if (n <= 0) {
			printf("[web] recv failed or client closed (n=%d, errno=%d)\n", n, errno);
			close(client_fd);
			continue;
		}
		req[n] = '\0';
		char method[16] = {0}, path[128] = {0};
		if (sscanf(req, "%15s %127s", method, path) != 2) {
			printf("[web] 无法解析请求，关闭连接\n");
			close(client_fd);
			continue;
		}

		printf("[web] 收到请求: %s %s\n", method, path);

		if (strcmp(path, "/metrics") == 0) {
			system_state_t state = fsm_get_state(&g_fsm);
			const char *state_str = fsm_get_state_name(&g_fsm);

			int mpu_fails = atomic_fetch_add(&g_fsm.mpu_fail_count,0);
			int i2c_retries = atomic_fetch_add(&g_fsm.i2c_retry_count,0);

			int p50, p99;
			calc_latency_percentile(&p50, &p99);

			int temp_raw = atomic_fetch_add(&g_fsm.temp_raw, 0);
			int hum_raw  = atomic_fetch_add(&g_fsm.hum_raw, 0);



			char body[8192];
			int body_len = snprintf(body, sizeof(body),
				"# HELP system_state Current system state (0=INIT,1=RUNNING,2=DEGRADED,3=RECOVERING,4=FAULT)\n"
				"# TYPE system_state gauge\n"
				"system_state %d\n"
				"\n"
				"# HELP system_state_name Human readable state name\n"
				"# TYPE system_state_name gauge\n"
				"system_state_name{state=\"%s\"} 1\n"
				"\n"
				"# HELP mpu_fail_total Total MPU6050 read failures\n"
				"# TYPE mpu_fail_total counter\n"
				"mpu_fail_total %d\n"
				"\n"
				"# HELP i2c_retry_total Total I2C retry attempts\n"
				"# TYPE i2c_retry_total counter\n"
				"i2c_retry_total %d\n"
				"\n"
				"# HELP sensor_temperature_celsius Temperature in celsius\n"
				"# TYPE sensor_temperature_celsius gauge\n"
				"sensor_temperature_celsius{id=\"sht30\"} %.1f\n"
				"\n"
				"# HELP sensor_humidity_percent Relative humidity in percent\n"
				"# TYPE sensor_humidity_percent gauge\n"
				"sensor_humidity_percent{id=\"sht30\"} %.1f\n"
				"\n"
				"# HELP task_latency_us Task latency in microseconds\n"
				"# TYPE task_latency_us summary\n"
				"task_latency_us{quantile=\"0.50\"} %d\n"
				"task_latency_us{quantile=\"0.99\"} %d\n",
				state,
				state_str,
				mpu_fails,
				i2c_retries,
				temp_raw / 1000.0, hum_raw / 1000.0,
				p50,
				p99
			);
			if (body_len < 0 || body_len >= (int)sizeof(body)) {
				 body_len = sizeof(body) - 1;
			}
			send_http_response(client_fd,200,"text/plain; charset=utf-8",body, body_len);

		} else if (strcmp(path, "/") == 0) {
			system_state_t state = fsm_get_state(&g_fsm);
			const char *state_str = state_name[state];

			const char *color;
			int temp_raw = atomic_fetch_add(&g_fsm.temp_raw, 0);
			int hum_raw  = atomic_fetch_add(&g_fsm.hum_raw, 0);
			double temp = temp_raw / 1000.0;
			double hum  = hum_raw / 1000.0;
			switch (state) {
				case STATE_RUNNING:	color = "green";	break;
				case STATE_DEGRADED:	color = "orange";	break;
				case STATE_RECOVERING:	color = "blue";	break;
				case STATE_FAULT:	color = "red";	break;					default:		color = "gray";	break;
				}

			char body[4096];
			int body_len = snprintf(body, sizeof(body),
				"<!DOCTYPE html>\n"
				"<html>\n"
				"<head>\n"
				"  <meta charset=\"UTF-8\">\n"
				"  <title>Edge-Omni 监控看板</title>\n"
				"  <style>\n"
				"    body {font-family: monospace; margin: 40px; background: #1a1a2e; color: #eee; }\n"
				"    .card { background: #16213e; padding: 20px; border-radius: 10px; margin: 20px 0; }\n"
				"    .state { font-size: 48px; font-weight: bold; transition: color 0.5s; }\n"
				"    .green { color: #00ff88; }\n"
				"    .orange { color: #ffaa00; }\n"
				"    .blue { color: #00aaff; }\n"
				"    .red { color: #ff4444; }\n"
				"    .gray { color: #888888; }\n"
				"    .metric { font-size: 20px; margin: 8px 0; color: #aaa; }\n"
				"    .value { color: #fff; font-weight: bold; }\n"
				"    a { color: #00aaff; }\n"
				"    #timestamp { color: #666; font-size: 14px; margin-top: 10px; }\n"
				"  </style>\n"
				"</head>\n"
				"<body>\n"
				"  <h1>Edge-Omni 系统状态看板</h1>\n"
				"  <div class=\"card\">\n"
				"    <div>当前状态</div>\n"
				"    <div id=\"state-display\" class=\"state %s\">%s</div>\n"
				"    <div id=\"timestamp\">最后更新: 刚刚</div>\n"
				"  </div>\n"
				"  <div class=\"card\">\n"
				"    <div class=\"sensor\" style=\"font-size:28px;color:#00ff88;margin:5px 0;\">温度: <span id=\"temp-val\">%.1f</span> C</div>\n"
				"    <div class=\"sensor\" style=\"font-size:28px;color:#00aaff;margin:5px 0;\">湿度: <span id=\"hum-val\">%.1f</span> %%</div>\n"
				"    <div class=\"metric\">MPU6050 失败次数: <span id=\"mpu-fails\" class=\"value\">%d</span></div>\n"
				"    <div class=\"metric\">I2C 重试次数: <span id=\"i2c-retries\" class=\"value\">%d</span></div>\n"
				"  </div>\n"
				"  <div class=\"card\">\n"
				"    <a href=\"/metrics\">查看 Prometheus 原始数据 (/metrics)</a>\n"
				"  </div>\n"
				"\n"
				"  <script>\n"
				"    \n"
				"    setInterval(async function() {\n"
				"      try {\n"
				"        \n"
				"        const response = await fetch('/metrics');\n"
				"        const text = await response.text();\n"
				"        \n"
				"        \n"
				"        const stateMatch = text.match(/system_state (\\d)/);\n"
				"        const nameMatch = text.match(/system_state_name\\{state=\"(\\w+)\"\\} 1/);\n"
				"        const mpuMatch = text.match(/mpu_fail_total (\\d+)/);\n"
				"        const i2cMatch = text.match(/i2c_retry_total (\\d+)/);\n"
				"        const tempMatch = text.match(/sensor_temperature_celsius\\{id=\"sht30\"\\} ([\\d.]+)/);\n"
				"        const humMatch  = text.match(/sensor_humidity_percent\\{id=\"sht30\"\\} ([\\d.]+)/);\n"

				"        \n"
				"        if (stateMatch && nameMatch) {\n"
				"          const stateNum = parseInt(stateMatch[1]);\n"
				"          const stateName = nameMatch[1];\n"
				"          \n"
				"          \n"
				"          const display = document.getElementById('state-display');\n"
				"          display.textContent = stateName;\n"
				"          \n"
				"          \n"
				"          display.className = 'state';\n"
				"          const colors = ['gray', 'green', 'orange', 'blue', 'red'];\n"
				"          if (colors[stateNum]) {\n"
				"            display.classList.add(colors[stateNum]);\n"
				"          }\n"
				"          \n"
				"          \n"
				"          const now = new Date().toLocaleTimeString('zh-CN');\n"
				"          document.getElementById('timestamp').textContent = '最后更新: ' + now;\n"
				"        }\n"
				"        \n"
				"        \n"
				"        if (mpuMatch) document.getElementById('mpu-fails').textContent = mpuMatch[1];\n"
				"        if (tempMatch) document.getElementById('temp-val').textContent = tempMatch[1];\n"
				"        if (humMatch)  document.getElementById('hum-val').textContent = humMatch[1];\n"
				"        if (i2cMatch) document.getElementById('i2c-retries').textContent = i2cMatch[1];\n"
				"        \n"
				"      } catch (e) {\n"
				"        console.error('刷新失败:', e);\n"
				"      }\n"
				"    }, 1000);\n"
				"  </script>\n"
				"</body>\n"
				"</html>\n",
				color, state_str,temp,hum,
				atomic_fetch_add(&g_fsm.mpu_fail_count, 0),
				atomic_fetch_add(&g_fsm.i2c_retry_count, 0)
			);


			send_http_response(client_fd, 200,"text/html; charset=utf-8",body, body_len);
		} else {
			const char *body = "404 Not Found\n";
			send_http_response(client_fd, 404, "text/plain", body, strlen(body));
		}

		shutdown(client_fd, SHUT_WR);
		close(client_fd);
	}
	close(server_fd);
	return NULL;



}
int main(int argc, char *argv[]){
	signal(SIGPIPE, SIG_IGN);

	printf("=== Edge-Omni 启动 ===\n");
	printf("[main] 初始化状态机...\n");
	if (fsm_init(&g_fsm) != 0) {
		fprintf(stderr, "[main] 状态机初始化失败,退出!\n");
		exit(EXIT_FAILURE);
	}

	printf("[main] 初始化数据库...\n");
	if (db_init() != 0) {
		fprintf(stderr, "[main] 数据库初始化失败,退出!\n");
		exit(EXIT_FAILURE);
	}
	if (db_check_and_repair() != 0) {
		fprintf(stderr, "[main] 数据库损坏且无法修复,退出!\n");
		exit(EXIT_FAILURE);
	}
	printf("[main] 数据库就绪\n");


	fsm_transition(&g_fsm, EVENT_INIT_OK);
	printf("[main] 当前系统状态: %s\n", fsm_get_state_name(&g_fsm));
	//db_insert_alarm((int)time(NULL), "test_alarm", 1, "系统启动测试报警");


	pthread_t collector_tid;
	printf("创建采集线程...\n");
	int ret = pthread_create(
		&collector_tid,
		NULL,
		collector_thread,
		NULL
	);

	if (ret != 0) {
		perror("pthread_create failed");
		exit(EXIT_FAILURE);
	}
	printf("采集线程已创建,TID=%lu\n", (unsigned long)collector_tid);
	cpu_set_t main_cpuset;
	CPU_ZERO(&main_cpuset);
	CPU_SET(MAIN_CPU_CORE, &main_cpuset);
	pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &main_cpuset);
	printf("[main] 已绑定到 CPU核心%d\n", MAIN_CPU_CORE);


	pthread_t web_tid;
	printf("创建 Web 线程...\n");
	ret = pthread_create(
		&web_tid,
		NULL,
		web_thread,
		NULL
	);
	if (ret != 0) {
		perror("pthread_create web_thread failed");
		exit(EXIT_FAILURE);
	}
	printf("Web 线程已创建,TID=%lu\n", (unsigned long)web_tid);

	printf("[main] 启动 Modbus TCP 服务器...\n");
	if (modbus_server_start() != 0) {
		fprintf(stderr, "[main] Modbus 服务器启动失败!\n");
		exit(EXIT_FAILURE);
	}
	printf("[main] Modbus TCP 服务器已启动,监听端口502\n");


	pthread_join(collector_tid, NULL);
	pthread_join(web_tid, NULL);
	modbus_server_stop();


	db_close();

	return 0;
}
















