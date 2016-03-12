#include "include/x86.h"
#include "include/stdio.h"
#include "include/assert.h"
#include "include/syscall.h"
//#include "game.h"

/*
 * 切换上下文应该注意改tss的ss0 和esp0
 */


static void (*do_timer)(void);
static void (*do_keyboard)(int);
uint32_t get_string_from_user(TrapFrame *tf, uint32_t ptr);

void
set_timer_intr_handler( void (*ptr)(void) ) {
	do_timer = ptr;
}
void
set_keyboard_intr_handler( void (*ptr)(int) ) {
	do_keyboard = ptr;
}


void print_stack(TrapFrame *tf);

/* TrapFrame的定义在include/memory.h
 * 请仔细理解这段程序的含义，这些内容将在后续的实验中被反复使用。 */
void
irq_handle(struct TrapFrame *tf) {
	//print_stack(tf);
	if(tf->irq < 1000) {
		if(tf->irq == -1) {
			print_stack(tf);
			printk("%s, %d: Unhandled exception!\n", __FUNCTION__, __LINE__);
		}
		else if(tf->irq == 0x80){
			//printk("0x80!\n");
			tf->eax = syscall(tf->eax,tf->ebx,get_string_from_user(tf,tf->ecx),tf->edx,tf->esi,tf->edi);
			//print_stack(tf);
			return;
		}
		else {
			printk("%s, %d: Unexpected exception #%d!\n", __FUNCTION__, __LINE__, tf->irq);
		}
		assert(0);
	}

	else if (tf->irq == 1000) {
		//printk("timer\n");
		do_timer();
		//print_stack(tf);
		//printk("==========\n");
	}
	else if (tf->irq == 1001) {
		uint32_t code = inb(0x60);
		uint32_t val = inb(0x61);
		outb(0x61, val | 0x80);
		outb(0x61, val);

		printk("%s, %d: key code = %x\n", __FUNCTION__, __LINE__, code);
		do_keyboard(code);
	} else {
		assert(0);
	}
	//print_stack(tf);
}

