#include "./include/string.h"
#include "./include/x86.h"
#include "./include/video.h"
#include "./include/device.h"
#include "./include/i8259.h"
#include "./include/irq.h"
#include "./include/stdio.h"



int main(){
	init_serial();
	init_timer();
	init_idt();
	init_intr();
	init_keyboard();
	blue_screen();
	printk("game start!\n");
	while(1)
		printk("haha\n");
	return 0;
}
