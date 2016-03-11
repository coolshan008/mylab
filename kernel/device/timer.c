#include "include/x86.h"
#include "include/assert.h"
#include "include/stdio.h"
#include "include/device.h"

/* 8253输入频率为1.193182MHz */
#define TIMER_PORT 0x40
#define FREQ_8253 1193182
#define HZ 100

void
do_timer(){
	for(int i=0;i<10;i++)
		printk("haha\n");
}


void
init_timer(void) {
	int counter= FREQ_8253 / HZ;
	outb(TIMER_PORT + 3, 0x34);
	outb(TIMER_PORT + 0, counter %256);
	outb(TIMER_PORT + 0, counter /256);
	printk("init timer\n");
	set_timer_intr_handler(do_timer);
}


