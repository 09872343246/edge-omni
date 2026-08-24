#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "include/thread_config.h"

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


	while(1){
		printf("[collector] 采集中... (运行在CPU%d)\n", sched_getcpu());
		struct timespec ts = {.tv_sec = 0, .tv_nsec = 1000000000};
		nanosleep(&ts, NULL);
	}
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t collector_tid;
	printf("=== Edge-Omni 启动 ===\n");
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
	printf("采集线程已创建，TID=%lu\n", (unsigned long)collector_tid);
	cpu_set_t main_cpuset;
	CPU_ZERO(&main_cpuset);
	CPU_SET(MAIN_CPU_CORE, &main_cpuset);
	pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &main_cpuset);
	printf("[main] 已绑定到 CPU核心%d\n", MAIN_CPU_CORE);
	pthread_join(collector_tid, NULL);
	return 0;
}
















