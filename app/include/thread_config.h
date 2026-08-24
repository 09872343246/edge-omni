#ifndef THREAD_CONFIG_H
#define THREAD_CONFIG_H


#include <sched.h>


#define COLLECTOR_CPU_CORE 0
#define MAIN_CPU_CORE 1


#define COLLECTOR_SCHED_POLICY	SCHED_FIFO
#define COLLECTOR_PRIORITY 	99
#define MAIN_SCHED_POLICY	SCHED_OTHER
#define MAIN_PRIORITY            0

#define COLLECTOR_THREAD_NAME	"collector"
#define MAIN_THREAD_NAME	"main_web"


#endif
