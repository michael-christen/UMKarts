#include <inttypes.h>
#include <stdbool.h>

#include "vtimer.h"
#include "drivers/mss_timer/mss_timer.h"

static const int MAX_NUM_TASKS = 100;

struct vtimer_task {
	void (*fn_ptr)(void);
	uint32_t id;

	uint32_t millisecs;
	bool repeat;
	timer_task* next;
};

struct vtimer {
	vtimer_task tasks[MAX_NUM_TASKS];
	bool enabled;
};

void VTIMER_init() {
	// Going to start with one shot mode for now
	// relys on tasks being quick!
	MSS_TIM2_init(MSS_TIMER_ONE_SHOT_MODE);
}

void VTIMER_enable() {
	MSS_TIM2_enable_irq();
}