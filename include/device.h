#ifndef __DEVICE_H__
#define __DEVICE_H__

void init_timer(void);
void init_keyboard(void);
void set_timer_intr_handler(void (*)(int));
void set_keyboard_intr_handler(void (*)(int));
void init_serial(void);
/*
 * 设置时钟中断的处理函数
 *
 */

#endif
