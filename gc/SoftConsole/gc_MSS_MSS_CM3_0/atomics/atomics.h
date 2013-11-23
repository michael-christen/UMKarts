#ifndef __ATOMICS_H_
#define __ATOMICS_H_

static inline int ldrex(int *mem) {
	int result;
	asm("ldrex %0, %1"
	                      :"=r"(result)
	                      :"m"(mem)
	                      :"memory");
	return result;
}

static inline int strex(int *mem, int val) {
	int old_value;
	asm("strex %0, %1, [%2]" : "=r" (old_value), "=r" (val) : "r" (mem) : "memory");
	return old_value;
}

#endif
