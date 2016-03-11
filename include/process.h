#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "include/x86.h"
#include "include/list.h"


//#define STK_SZ  8192
#define KSTACK_SIZE 4096
#define PCB_Pool_NUM 128

#define TASK_RUNNING         1
#define TASK_READY           2
#define TASK_INTERRUPTIBLE   3
#define TASK_UNINTERRUPTIBLE 4
#define TASK_STOPPED         0
//#define PG_ALIGN __attribute((aligned(PG_SIZE)))



typedef struct PCB
{
	void *tf;
	//void *idt;
	uint8_t kstack[KSTACK_SIZE];
	//uint32_t idtr;
	int state;
	int pid;
	//uint8_t time_count;
	ListHead list;
}PCB;


PCB *current ;
PCB *pcb_of_idle ;


PCB PCBPool[PCB_Pool_NUM];
void TrapFrame_init(TrapFrame *);
void PCBPool_init();
PCB* new_PCB();


int enter_process_userspace(void);

void print_stack(TrapFrame *tf);
#endif
