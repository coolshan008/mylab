//#include "syscall.h"
#include "include/x86.h"
#include "include/string.h"
#include "include/process.h"
#include "include/stdio.h"


enum
{
	SYS_cputs = 0,
	SYS_cgetc,
	SYS_getpid,
	SYS_pcb_destroy,
	SYS_page_alloc,
	SYS_page_map,
	SYS_page_unmap,
	SYS_exofork,
	SYS_set_status,
	SYS_set_trapframe,
	SYS_set_pgfault_upcall,
	SYS_yield,
	SYS_ipc_try_send,
	SYS_ipc_recv,
	NSYSCALLS
};

void serial_printc(char ch);

static void sys_cputs(const char *s,size_t len)
{
	for(int i=0;i<len;i++)
		serial_printc(s[i]);
}


int syscall(uint32_t syscallno,uint32_t a1,uint32_t a2,\
		uint32_t a3,uint32_t a4,int32_t a5)
{
   /*
	  根据系统调用号，调用不同的系统调用处理程序
	*/
	switch(syscallno){
		case SYS_cputs:if(a1==1)sys_cputs((char *)a2,a3);return a3;
		default:assert(0);return 0;
	}
}
