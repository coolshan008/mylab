#include "./include/string.h"
#include "./include/x86.h"
#include "./include/video.h"
#include "./include/device.h"
#include "./include/i8259.h"
#include "./include/irq.h"
#include "./include/stdio.h"
#include "./include/kvm.h"
#include "./include/process.h"
#include "./include/game.h"


void init_mmu();


void
init_proc(){
	init_mmu();
	PCBPool_init();
}


int main(){
	printk("start kernel!!!!!!!!!\n");
	init_segment();
	init_idt();
	init_intr();
	init_serial();
	init_timer();
	init_keyboard();
	init_proc();
	blue_screen();
	//enable_interrupt();
	/*
	 * enable interrupt by iret
	 */
	enter_process_userspace();
	//main_loop();
	assert(0);
}
