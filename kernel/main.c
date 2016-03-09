#include "./include/string.h"
#include "./include/x86.h"
#include "./include/video.h"
#include "./include/device.h"
#include "./include/i8259.h"
#include "./include/irq.h"
#include "./include/stdio.h"
#include "./include/game.h"



int main(){
	printk("start kernel!!!!!!!!!\n");
	init_serial();
	init_timer();
	init_idt();
	init_intr();
	init_keyboard();
	blue_screen();
	enable_interrupt();
	main_loop();
	assert(0);
}
