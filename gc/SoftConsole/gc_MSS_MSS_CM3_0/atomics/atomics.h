#ifndef __ATOMICS_H_
#define __ATOMICS_H_

static inline unsigned char atomic_cmpxchg_4(int * mem, int new_val, int old) {
	unsigned char failure;
	int old_mem_val;
	asm(
		"mov.w %[failure], #1\n" /* Default the failure to 1 (fails) */
    "cpsid i\n" /* Disable interrupts */
		"ldr %[old_mem_val], [%[mem]]\n" /* Load from memory */
		"cmp   %[old_mem_val],  %[old_val]\n" /* Make sure that it's the old value */
		"itt   eq\n" /* If equal, then save */
		"streq %[new_val], [%[mem]]\n"
    "moveq.w %[failure], #0\n" /* And if equal, clear failure */
    "dmb st\n"   /* Force write to occur NOW */
    "cpsie i" /* Enable interrupts */
		: [failure] "=&l" (failure), [old_mem_val] "=&l" (old_mem_val)
		: [mem] "l" (mem), [new_val] "l" (new_val), [old_val] "l" (old)
		: "memory"
	);
	return failure;
}

static inline unsigned char atomic_cmpxchg_1(unsigned char * mem, unsigned char new_val, unsigned char old) {
	unsigned char failure;
	unsigned char old_mem_val;
	asm(
    "mov.b %[failure], #1\n"
    "cpsid i\n" /* Disable interrupts */
		"ldrb %[old_mem_val], [%[mem]]\n"
		"cmp   %[old_mem_val],  %[old_val]\n"
		"itt   eq\n"
		"strbeq %[new_val], [%[mem]]\n"
    "moveq  %[failure], #0\n"
    "dmb st\n"
    "cpsie i"
		: [failure] "=&l" (failure), [old_mem_val] "=&l" (old_mem_val)
		: [mem] "l" (mem), [new_val] "l" (new_val), [old_val] "l" (old)
		: "memory"
	);
	return failure;
}

static inline unsigned char atomic_lock_test_and_set_1(unsigned char *mem) {
	return atomic_cmpxchg_1(mem, 1, 0);
}

static inline void atomic_lock_release(unsigned char *mem) {
  asm("cpsid i");
  *mem = 0;
  asm(
    "dmb st\n"
    "cpsie i"
  );
}

#endif
