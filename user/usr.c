#include "user/lib.h"



int main(void)
{
   /*
	创建一个简单的应用程序，尽量不要定义未初始化的全局变量
	系统调用的封装函数为cputs，在调用这个函数时，需要循环
	调用，以保证程序一直在运行
   */
	char ha[10]="hello";
	cputs(ha,5);
	while(1);
	return 0;
}
