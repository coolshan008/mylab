#ifndef _SYSCALL_H_
#define _SYSCALL_H_



int syscall(uint32_t syscallno, uint32_t a1, uint32_t a2, \
		uint32_t a3, uint32_t a4, uint32_t a5);
#endif
