#ifndef __ATOMICS_H_
#define __ATOMICS_H_

static inline int ldrex(int *mem) {
	int result;
	asm volatile("ldrex %0, [%1]"
	                      :"=&r"(result)
	                      :"r"(mem)
	                      :"memory");
	return result;
}

static inline int strex(int *mem, int val) {
	int old_value;
	asm volatile("strex %0, %1, [%2]" : "=&r"(old_value) : "r"(mem), "r"(val) : "memory");
	return old_value;
}

static inline unsigned char ldrexb(unsigned char *mem) {
	unsigned char result;
	asm volatile("ldrexb %0, [%1]"
	                      :"=&r"(result)
	                      :"r"(mem)
	                      :"memory");
	return result;
}

static inline unsigned char strexb(unsigned char *mem, unsigned char val) {
	unsigned char old_value;
	asm volatile ("strexb %0, %1, [%2]" : "=r"(old_value), "=r"(val) : "r"(mem) : "memory");
	return old_value;
}

static inline unsigned char atomic_lock_test_and_set_1(unsigned char *mem, unsigned char val) {
	unsigned char old_value;
	old_value = strexb(mem, val);
	return  (old_value == 0);
}

#endif
