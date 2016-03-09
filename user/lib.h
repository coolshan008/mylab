#ifndef _LIB_H_
#define _LIB_H_

#include "types.h"
//#include "debug.h"
//#include "kernel.h"
//syscall.c

enum
{
	SYS_cputs = 0,
	SYS_cgetc,
	SYS_getenvid,
	SYS_env_destroy,
	SYS_page_alloc,
	SYS_page_map,
	SYS_page_unmap,
	SYS_exofork,
	SYS_env_set_status,
	SYS_env_set_pgfault,
	SYS_yield,
	SYS_ipc_try_send,
	SYS_ipc_recv,
	NSYSCALLS
};

int cputs(const char *string, uint32_t len);
int yield(void);


#endif
