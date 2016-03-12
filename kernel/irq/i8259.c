#include "include/x86.h"
#include "include/stdio.h"

#define PORT_PIC_MASTER 0x20
#define PORT_PIC_SLAVE  0xA0
#define IRQ_OFFSET 0x20
#define IRQ_SLAVE       2

/* 初始化8259中断控制器：
 * 硬件中断IRQ从32号开始，自动发送EOI */
void
init_intr(void) {
	printk("init i8259\n");
	outb(PORT_PIC_MASTER + 1, 0xFF);
	outb(PORT_PIC_SLAVE + 1 , 0xFF);
	outb(PORT_PIC_MASTER, 0x11);
	outb(PORT_PIC_MASTER + 1, 32);
	outb(PORT_PIC_MASTER + 1, 1 << 2);
	outb(PORT_PIC_MASTER + 1, 0x3);
	//outb(PORT_PIC_MASTER + 1, 0x2);
	outb(PORT_PIC_SLAVE, 0x11);
	outb(PORT_PIC_SLAVE + 1, 32 + 8);
	outb(PORT_PIC_SLAVE + 1, 2);
	outb(PORT_PIC_SLAVE + 1, 0x3);
	//outb(PORT_PIC_SLAVE + 1, 0x1);

	outb(PORT_PIC_MASTER, 0x68);
	outb(PORT_PIC_MASTER, 0x0A);
	outb(PORT_PIC_SLAVE, 0x68);
	outb(PORT_PIC_SLAVE, 0x0A);
}
