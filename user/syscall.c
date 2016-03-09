//System call

//#include "syscall.h"
#include "lib.h"

#define T_SYSCALL   0x80

static inline int32_t syscall(int num, int check, uint32_t a1,uint32_t a2,uint32_t a3, uint32_t a4, uint32_t a5)
{
	int32_t ret = 0;
	//Generic system call: pass system call number in AX
	//up to five parameters in DX,CX,BX,DI,SI
	//Interrupt kernel with T_SYSCALL
	//
	//The "volatile" tells the assembler not to optimize
	//this instruction away just because we don't use the
	//return value
	//
	//The last clause tells the assembler that this can potentially
	//change the condition and arbitrary memory locations.

	asm volatile("int $0x80":"=a"(ret):"a"(num),"b"(a1),"c"(a2),"d"(a3),"S"(a4),"D"(a5));

	return ret;
}

int sys_cputs(const char *s,uint32_t len)
{
	return syscall(SYS_cputs,0,1,(int)s,len,0,0);
}

int cputs(const char *s,uint32_t len)
{
	return sys_cputs(s,len);
}
