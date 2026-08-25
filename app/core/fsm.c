#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "../include/fsm.h"



static const char *event_name[] = {
    "INIT_OK",          /* 0 */
    "MPU_FAIL",         /* 1 */
    "I2C_DEADLOCK",     /* 2 */
    "BUS_RECOVERED",    /* 3 */
    "BUS_RECOVER_FAIL", /* 4 */
    "MPU_RESTORED",     /* 5 */
    "ALL_OFFLINE",      /* 6 */
    "WATCHDOG_RESET"    /* 7 */
};



const char *state_name[] = {
	"INIT",
	"RUNNING",
	"DEGRADED",
	"RECOVERING",
	"FAULT"
};

static const int transition_table[STATE_MAX][EVENT_WATCHDOG_RESET + 1] = {
/* ==================== INIT 状态 ==================== */
	[STATE_INIT] = {
		[EVENT_INIT_OK]        = STATE_RUNNING,
		[EVENT_MPU_FAIL]       = -1,
		[EVENT_I2C_DEADLOCK]   = -1,
		[EVENT_BUS_RECOVERED]  = -1,
		[EVENT_BUS_RECOVER_FAIL]= -1,
		[EVENT_MPU_RESTORED]   = -1,
		[EVENT_ALL_OFFLINE]    = STATE_FAULT,
		[EVENT_WATCHDOG_RESET] = -1
	},
/* ==================== RUNNING 状态 ==================== */

	[STATE_RUNNING] = {
		[EVENT_INIT_OK]        = -1,
		[EVENT_MPU_FAIL]       = STATE_DEGRADED,
		[EVENT_I2C_DEADLOCK]   = STATE_RECOVERING,
		[EVENT_BUS_RECOVERED]  = -1,
		[EVENT_BUS_RECOVER_FAIL]= -1,
		[EVENT_MPU_RESTORED]   = -1,
		[EVENT_ALL_OFFLINE]    = STATE_FAULT,
		[EVENT_WATCHDOG_RESET] = -1
	},

/* ==================== DEGRADED 状态 ==================== */

	[STATE_DEGRADED] = {
		[EVENT_INIT_OK]        = -1,
		[EVENT_MPU_FAIL]       = -1,
		[EVENT_I2C_DEADLOCK]   = STATE_RECOVERING,
		[EVENT_BUS_RECOVERED]  = -1,
		[EVENT_BUS_RECOVER_FAIL]= -1,
		[EVENT_MPU_RESTORED]   = STATE_RUNNING,
		[EVENT_ALL_OFFLINE]    = STATE_FAULT,
		[EVENT_WATCHDOG_RESET] = -1
	},


/* ==================== RECOVERING 状态 ==================== */
	[STATE_RECOVERING] = {
                [EVENT_INIT_OK]        = -1,
                [EVENT_MPU_FAIL]       = -1,
                [EVENT_I2C_DEADLOCK]   = -1,
                [EVENT_BUS_RECOVERED]  = STATE_RUNNING,
                [EVENT_BUS_RECOVER_FAIL]= STATE_FAULT,
                [EVENT_MPU_RESTORED]   = -1,
                [EVENT_ALL_OFFLINE]    = STATE_FAULT,
                [EVENT_WATCHDOG_RESET] = -1
        },


/* ==================== FAULT 状态 ==================== */

        [STATE_FAULT] = {
                [EVENT_INIT_OK]        = -1,
                [EVENT_MPU_FAIL]       = -1,
                [EVENT_I2C_DEADLOCK]   = -1,
                [EVENT_BUS_RECOVERED]  = -1,
                [EVENT_BUS_RECOVER_FAIL]= -1,
                [EVENT_MPU_RESTORED]   = -1,
                [EVENT_ALL_OFFLINE]    = -1,
                [EVENT_WATCHDOG_RESET] = STATE_INIT
        },
};

int fsm_init(fsm_context_t *ctx){
	if (ctx == NULL) {
		fprintf(stderr, "[FSM] Error: ctx is NULL\n");
		return -1;
	}
	memset(ctx, 0, sizeof(fsm_context_t));
	ctx->current = STATE_INIT;
	if (pthread_mutex_init(&ctx->lock, NULL) != 0) {
		fprintf(stderr, "[FSM] Error: mutex init failed\n");
		return -1;
	}
	if (pthread_cond_init(&ctx->cond, NULL) != 0) {
                fprintf(stderr, "[FSM] Error: cond init failed\n");
                pthread_mutex_destroy(&ctx->lock);
		return -1;
	}

	atomic_init(&ctx->mpu_fail_count, 0);
	atomic_init(&ctx->i2c_retry_count, 0);

	atomic_init(&ctx->temp_raw, 0);
	atomic_init(&ctx->hum_raw, 0);

	printf("[FSM] Initialized, state = %s\n", state_name[ctx->current]);
	return 0;
}


int fsm_transition(fsm_context_t *ctx, fsm_event_t event){
	int next_state;
	int ret = 0;
	if (ctx == NULL) {
		return -1;
	}
	pthread_mutex_lock(&ctx->lock);
	next_state = transition_table[ctx->current][event];
	if (next_state < 0 || next_state >= STATE_MAX) {
		printf("[FSM] Reject: %s + %s → (illegal)\n",state_name[ctx->current],event_name[event]);
		ret = -1;
	} else {
		printf("[FSM] Transition: %s + %s → %s\n",state_name[ctx->current],event_name[event],state_name[next_state]);
		ctx->current = next_state;
		pthread_cond_broadcast(&ctx->cond);
	}
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

system_state_t fsm_get_state(fsm_context_t *ctx){
	system_state_t s;
	pthread_mutex_lock(&ctx->lock);
	s = ctx->current;
	if (s < 0 || s >= STATE_MAX) {
		fprintf(stderr, "[FSM] ERROR: current=%d, memory corrupted!\n", s);
		s = STATE_FAULT;
	}
	pthread_mutex_unlock(&ctx->lock);

	return s;
}

const char *fsm_get_state_name(fsm_context_t *ctx){
	system_state_t s = fsm_get_state(ctx);
	return state_name[s];
}

int fsm_wait_for_state(fsm_context_t *ctx, system_state_t target, int timeout_ms){
	int ret = 0;
	struct timespec ts;
	if (ctx == NULL) {
		return -1;
	}
	pthread_mutex_lock(&ctx->lock);
	while (ctx->current != target) {
		if (timeout_ms < 0) {
			pthread_cond_wait(&ctx->cond, &ctx->lock);
		} else {
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec  += timeout_ms / 1000;
			ts.tv_nsec += (timeout_ms % 1000) * 1000000L;
			if (ts.tv_nsec >= 1000000000L) {
				ts.tv_sec++;
				ts.tv_nsec -= 1000000000L;
			}
			ret = pthread_cond_timedwait(&ctx->cond, &ctx->lock, &ts);
			if (ret == ETIMEDOUT) {
				printf("[FSM] Wait timeout: target=%s, current=%s\n",state_name[target], state_name[ctx->current]);
				ret = -1;
				break;
			}
		}
	}
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}
