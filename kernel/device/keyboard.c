#include "./include/x86.h"
#include "./include/kbdreg.h"
#include "./include/device.h"


int kb_dreg;
uint8_t unused = false;//keyboard state untrack



void do_keyboard(int code){
	kb_dreg = code;
	unused = true;
}


void init_keyboard(){
	set_keyboard_intr_handler(do_keyboard);
}
