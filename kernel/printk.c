#include "include/x86.h"
#include "include/stdarg.h"
#include "include/stdio.h"

/*
 * You may refer to lib/printfmt.c
 * to implement the printk() and vprintk().
 * You can also implement a simplier version
 * by yourself.
 */

void serial_printc(char ch);

static void
putch(int ch, void *cnt){
	serial_printc(ch);
	(*((int *)cnt))++;
}


int	vprintk(const char *fmt,va_list ap){ 
	int cnt=0;
	vprintfmt(putch,(void *)&cnt,fmt,ap);
	return cnt;
}



int	printk(const char *fmt, ...){
	va_list ap;
	int rc;
	
	va_start(ap, fmt);
	rc= vprintk(fmt,ap);
	va_end(ap);
	return rc;
}
