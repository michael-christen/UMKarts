#ifndef __ATOMICS_H_
#define __ATOMICS_H_

static inline unsigned char atomic_cmpxchg_4(int * mem, int new, int old) {
	unsigned char failure;
	int old_mem_val;
	asm(
		"mov.w %[failure], #1\n"
		"ldr %[old_mem_val], [%[mem]]\n"
		"cmp   %[old_mem_val],  %[old_val]\n"
		"it    eq\n"
		"strexeq %[failure], %[new_val], [%[mem]]"
		: [failure] "=&l" (failure), [old_mem_val] "=&l" (old_mem_val)
		: [mem] "l" (mem), [new_val] "l" (new), [old_val] "l" (old)
		: "memory"
	);
	return failure;
}

static inline unsigned char atomic_cmpxchg_unsigned_char(unsigned char * mem, unsigned char new, unsigned char old) {
	unsigned char failure;
	unsigned char old_mem_val;
	asm(
		"ldrexb %[old_mem_val], [%[mem]]\n"
		"cmp   %[old_mem_val],  %[old_val]\n"
		"it    eq\n"
		"strexbeq %[res], %[new_val], [%[mem]]"
		: [res] "=&l" (failure), [old_mem_val] "=&l" (old_mem_val)
		: [mem] "l" (mem), [new_val] "l" (new), [old_val] "l" (old)
		: "memory"
	);
	return failure;
}

static inline unsigned char atomic_lock_test_and_set_1(unsigned char *mem) {
	return atomic_cmpxchg_unsigned_char(mem, 0, 1);
}

#endif
