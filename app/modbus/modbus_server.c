#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <modbus.h>
#include "modbus_server.h"


#define MODBUS_PORT         502
#define MODBUS_MAX_CLIENTS  5
#define MODBUS_MAX_REGS     4



#define BLACKLIST_THRESHOLD 5
#define BLACKLIST_BAN_SEC   300


static modbus_t          *g_ctx          = NULL;
static modbus_mapping_t  *g_mb_mapping   = NULL;
static pthread_t          g_server_tid;
static volatile int       g_running      = 1;


static modbus_reg_data_t  g_reg_data;

typedef struct {
	uint32_t ip_addr;
	int      err_count;
	time_t   ban_until;
} blacklist_entry_t;


#define BLACKLIST_MAX 16
static blacklist_entry_t g_blacklist[BLACKLIST_MAX];
static pthread_mutex_t   g_bl_mutex = PTHREAD_MUTEX_INITIALIZER;



static uint32_t ip_to_u32(struct sockaddr_in *addr){
	return ntohl(addr->sin_addr.s_addr);
}
static int is_ip_banned(uint32_t ip){
	time_t now = time(NULL);
	pthread_mutex_lock(&g_bl_mutex);
	for (int i = 0; i < BLACKLIST_MAX; i++) {
		if (g_blacklist[i].ip_addr == ip) {
			if (g_blacklist[i].ban_until > now) {
				pthread_mutex_unlock(&g_bl_mutex);
				return 1;
			}
			g_blacklist[i].err_count = 0;
			g_blacklist[i].ban_until = 0;
			pthread_mutex_unlock(&g_bl_mutex);
			return 0;
		}
	}
	pthread_mutex_unlock(&g_bl_mutex);
	return 0;
}

static void record_illegal_request(uint32_t ip){
	time_t now = time(NULL);
	pthread_mutex_lock(&g_bl_mutex);
	int slot = -1;
	for (int i = 0; i < BLACKLIST_MAX; i++) {
		if (g_blacklist[i].ip_addr == ip) {
			slot = i;
			break;
		}
		if (slot == -1 && g_blacklist[i].ip_addr == 0) {
			slot = i;
		}
	}
	if (slot < 0) {
		slot = 0;
		memset(&g_blacklist[0], 0, sizeof(g_blacklist[0]));
	}
	g_blacklist[slot].ip_addr = ip;
	g_blacklist[slot].err_count++;

	if (g_blacklist[slot].err_count >= BLACKLIST_THRESHOLD) {
		g_blacklist[slot].ban_until = now + BLACKLIST_BAN_SEC;
		g_blacklist[slot].err_count = 0;
		char cmd[256];
		struct in_addr addr;
		addr.s_addr = htonl(ip);
		snprintf(cmd, sizeof(cmd),"iptables -A INPUT -s %s -p tcp --dport %d -j DROP",inet_ntoa(addr), MODBUS_PORT);
		system(cmd);
		printf("[MODBUS] IP %s banned for %d sec (iptables)\n",inet_ntoa(addr), BLACKLIST_BAN_SEC);

	}
	pthread_mutex_unlock(&g_bl_mutex);
}

static void clear_iptables_rules(void){
	char cmd[256];
	snprintf(cmd, sizeof(cmd),"iptables -D INPUT -p tcp --dport %d -j DROP 2>/dev/null",MODBUS_PORT);
	while (system(cmd) == 0) { }
}





void modbus_server_update_data(const modbus_reg_data_t *data){
	atomic_store_explicit(&g_reg_data.temperature, data->temperature, memory_order_relaxed);
	atomic_store_explicit(&g_reg_data.humidity,    data->humidity,    memory_order_relaxed);
	atomic_store_explicit(&g_reg_data.accel_x,     data->accel_x,     memory_order_relaxed);
	atomic_store_explicit(&g_reg_data.accel_y,     data->accel_y,     memory_order_relaxed);

}


static void handle_client(int client_socket, struct sockaddr_in *client_addr){
	uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
	uint32_t client_ip = ip_to_u32(client_addr);
	int rc;
	if (is_ip_banned(client_ip)) {
		printf("[MODBUS] Connection from banned IP %s rejected\n",inet_ntoa(client_addr->sin_addr));
		close(client_socket);
		return;
	}
	modbus_set_socket(g_ctx, client_socket);
	while (g_running) {
		rc = modbus_receive(g_ctx, query);
		if (rc <= 0) {
			break;
		}
		int function = query[7];
		if (function != MODBUS_FC_READ_HOLDING_REGISTERS) {
			printf("[MODBUS] Illegal function 0x%02X from %s\n",function, inet_ntoa(client_addr->sin_addr));
			record_illegal_request(client_ip);
			modbus_reply_exception(g_ctx, query, MODBUS_EXCEPTION_ILLEGAL_FUNCTION);
			continue;
		}
		int addr  = (query[8] << 8) | query[9];
		int count = (query[10] << 8) | query[11];
		if (addr < 0 || addr + count > MODBUS_MAX_REGS) {
			printf("[MODBUS] Illegal data address %d+%d from %s\n",addr, count, inet_ntoa(client_addr->sin_addr));
			record_illegal_request(client_ip);
			modbus_reply_exception(g_ctx, query,MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
			continue;
		}
		uint16_t regs[MODBUS_MAX_REGS];
		regs[0] = atomic_load_explicit(&g_reg_data.temperature, memory_order_relaxed);
		regs[1] = atomic_load_explicit(&g_reg_data.humidity,    memory_order_relaxed);
		regs[2] = atomic_load_explicit(&g_reg_data.accel_x,     memory_order_relaxed);
		regs[3] = atomic_load_explicit(&g_reg_data.accel_y,     memory_order_relaxed);


		for (int i = 0; i < MODBUS_MAX_REGS; i++) {
			g_mb_mapping->tab_registers[i] = regs[i];
		}
		rc = modbus_reply(g_ctx, query, rc, g_mb_mapping);
		if (rc < 0) {
			printf("[MODBUS] Reply error: %s\n", modbus_strerror(errno));
			break;
		}
	}
	close(client_socket);
	printf("[MODBUS] Client %s disconnected\n", inet_ntoa(client_addr->sin_addr));
}


static void *modbus_server_thread(void *arg){
	(void)arg;
	int server_socket;
	fd_set readfds;
	struct timeval tv;

	printf("[MODBUS] Server thread started, listening on port %d\n", MODBUS_PORT);


	server_socket = modbus_tcp_listen(g_ctx, MODBUS_MAX_CLIENTS);
	if (server_socket < 0) {
		fprintf(stderr, "[MODBUS] Listen failed: %s\n", modbus_strerror(errno));
		return NULL;
	}
	while (g_running) {
		FD_ZERO(&readfds);
		FD_SET(server_socket, &readfds);
		tv.tv_sec  = 1;
		tv.tv_usec = 0;
		int rc = select(server_socket + 1, &readfds, NULL, NULL, &tv);
		if (rc < 0) {
			if (errno == EINTR) continue;
			break;
		}

		if (rc == 0) continue;
		if (FD_ISSET(server_socket, &readfds)) {
			struct sockaddr_in client_addr;
			socklen_t addr_len = sizeof(client_addr);
			int client_socket = accept(server_socket,(struct sockaddr *)&client_addr,&addr_len);
			if (client_socket < 0) {
				continue;
			}
			printf("[MODBUS] Client connected: %s:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
			handle_client(client_socket, &client_addr);
		}
	}
	close(server_socket);
	printf("[MODBUS] Server thread stopped\n");
	return NULL;
}


int modbus_server_start(void){
	memset(&g_reg_data, 0, sizeof(g_reg_data));
	memset(g_blacklist, 0, sizeof(g_blacklist));

	g_ctx = modbus_new_tcp("0.0.0.0", MODBUS_PORT);
	if (!g_ctx) {
		fprintf(stderr, "[MODBUS] Failed to create context\n");
		return -1;
	}
	g_mb_mapping = modbus_mapping_new(0, 0, MODBUS_MAX_REGS, 0);
	if (!g_mb_mapping) {
		fprintf(stderr, "[MODBUS] Failed to create mapping\n");
		modbus_free(g_ctx);
		g_ctx = NULL;
		return -1;
	}

	g_running = 1;
	if (pthread_create(&g_server_tid, NULL, modbus_server_thread, NULL) != 0) {
		fprintf(stderr, "[MODBUS] Failed to create thread\n");
		modbus_mapping_free(g_mb_mapping);
		modbus_free(g_ctx);
		g_mb_mapping = NULL;
		g_ctx = NULL;
		return -1;
	}

	printf("[MODBUS] Server started successfully\n");
	return 0;
}



void modbus_server_stop(void){
	g_running = 0;
	pthread_join(g_server_tid, NULL);
	if (g_mb_mapping) {
		modbus_mapping_free(g_mb_mapping);
		g_mb_mapping = NULL;
	}
	if (g_ctx) {
		modbus_free(g_ctx);
		g_ctx = NULL;
	}

	clear_iptables_rules();
	printf("[MODBUS] Server stopped, iptables cleaned\n");
}


