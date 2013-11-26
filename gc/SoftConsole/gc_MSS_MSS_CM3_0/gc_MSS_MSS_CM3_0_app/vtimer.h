#ifndef __MATT_VTIMER__
#define __MATT_VTIMER__
#include <stdbool.h>
#include <inttypes.h>

/*
 * Enable the virtual timer
 */
void VTIMER_enable();

/*
 * Disable the virtual timer
 */
void VTIMER_disable();

/*
 * Add a task
 * 
 * Returns a task id
 */
 uint32_t VTIMER_add_task(void (*fn_ptr)(void), uint32_t millisecs, bool repeat);

 /*
  * Remove a task
  */
 uint32_t VTIMER_remove_task(uint32_t task_id);
#endif