#ifndef FSM_H
#define FSM_H

#include <pthread.h>
#include <stdatomic.h>

typedef enum {
	STATE_INIT		= 0,
	STATE_RUNNING		= 1,
	STATE_DEGRADED		= 2,
	STATE_RECOVERING	= 3,
	STATE_FAULT		= 4,
	STATE_MAX		= 5
} system_state_t;


extern const char *state_name[];


typedef enum {
	EVENT_INIT_OK,
	EVENT_MPU_FAIL,
	EVENT_I2C_DEADLOCK,
	EVENT_BUS_RECOVERED,
	EVENT_BUS_RECOVER_FAIL,
	EVENT_MPU_RESTORED,
	EVENT_ALL_OFFLINE,
	EVENT_WATCHDOG_RESET
} fsm_event_t;

typedef struct {
	system_state_t current;
	pthread_mutex_t lock;
	pthread_cond_t  cond;
	atomic_int mpu_fail_count;
	atomic_int i2c_retry_count;
	atomic_int temp_raw;
	atomic_int hum_raw;

} fsm_context_t;

int fsm_init(fsm_context_t *ctx);


int fsm_transition(fsm_context_t *ctx, fsm_event_t event);

system_state_t fsm_get_state(fsm_context_t *ctx);

const char *fsm_get_state_name(fsm_context_t *ctx);


int fsm_wait_for_state(fsm_context_t *ctx, system_state_t target, int timeout_ms);


#endif
