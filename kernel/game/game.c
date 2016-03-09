#include "./include/string.h"
#include "./include/x86.h"
#include "./include/video.h"
#include "./include/stdio.h"



void main_loop(){
	printk("game start!\n");
	int i=0;
	while(1){
		if(i==1000)
			printk("haha\n");
		i=(i+1)%10000;
	}
}
