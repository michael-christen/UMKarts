#include <inttypes.h>
#include <stdbool.h>

#include "vtimer.h"
#include "drivers/mss_timer/mss_timer.h"

static const int MAX_NUM_TASKS = 100;

// @TODO: Only disable irqs which a task
static inline disable_irqs() {
	asm("cpsid i");
}

/// @TODO: Only disable irqs which a task
static inline reenable_irqs() {
	asm("cpsie i");
}

struct vtimer_task {
	void (*fn_ptr)(void);
	uint32_t id;

	uint32_t millisecs;
	bool repeat;
	timer_task* next;
};

struct vtimer {
	vtimer_task tasks[MAX_NUM_TASKS];
	uint32_t num_tasks;
	bool modified;
};

static volatile vtimer virtual_timer;

void VTIMER_init() {
	// Going to start with one shot mode for now
	// relys on tasks being quick!
	MSS_TIM2_init(MSS_TIMER_ONE_SHOT_MODE);
}

void VTIMER_enable() {
	MSS_TIM2_enable_irq();
}

void VTIMER_disable() {
	MSS_TIM2_disable_irq();
}

uint32_t VTIMER_add_task(void (*fn_ptr)(void), uint32_t millisecs, bool repeat) {
	if (virtual_timer.num_tasks == MAX_NUM_TASKS) return -1;
	disable_irqs();
	do {
		virtual_timer.modified = false;
		reenable_irqs();
		// search for spot to insert
		disable_irqs()
	} while (!virtual_timer.modified);
}

static void VTIMER_start() {
	VTIMER_enable();
	disable_irqs();

}