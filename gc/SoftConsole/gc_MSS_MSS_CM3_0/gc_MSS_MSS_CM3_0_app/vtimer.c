#include <inttypes.h>
#include <stdbool.h>

#include "vtimer.h"
#include "drivers/mss_rtc/mss_rtc.h"

static const int MAX_NUM_TASKS = 100;
// Its actually 32.768, but to make the math a little easier we round
static const int MS_TO_COUNT = 33;

// @TODO: Only disable irqs which have a task
static inline disable_irqs() {
	asm("cpsid i");
}

/// @TODO: Only disable irqs which have a task
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
	virtual_timer.num_tasks = 0;
	virtual_timer.modified = false;
	MSS_RTC_init();
	MSS_RTC_configure(MSS_RTC_NO_COUNTER_RESET);
	// reset to 0, should never overflow now
	MSS_RTC_get_rtc_count(0);
}

void VTIMER_enable() {
	MSS_RTC_start();
}

void VTIMER_disable() {
	MSS_RTC_stop();
}

uint32_t VTIMER_add_task(void (*fn_ptr)(void), uint32_t millisecs, bool repeat) {
	// Grab the time first to figure out the offset
	uint32_t task_id;
	uint64_t curr_count = MSS_RTC_get_rtc_count();
	uint64_t offset_count = curr_count + (millisecs * MS_TO_COUNT);

	// There are too many tasks in the queue
	if (virtual_timer.num_tasks == MAX_NUM_TASKS) return -1;

	disable_irqs();
	do {
		virtual_timer.modified = false;
		reenable_irqs();
		// @TODO: search for spot to insert
		disable_irqs();
	} while (!virtual_timer.modified);
	if (offset_count <= MSS_RTC_get_rtc_count()) {
		fn_ptr();
		task_id = 0;
	} else {
		// @TODO: insert task
	}
	reenable_irqs();
	return task_id;
}

static void VTIMER_start() {
	VTIMER_enable();
	disable_irqs();

}
