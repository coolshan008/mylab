#include "include/x86.h"
#include "include/assert.h"
#include "include/string.h"
#include "include/stdio.h"

/* This source file involves some hardware details. Please refer to 
 *  _ ____   ___    __    __  __                         _ 
 * (_)___ \ / _ \  / /   |  \/  |                       | |
 *  _  __) | (_) |/ /_   | \  / | __ _ _ __  _   _  __ _| |
 * | ||__ < > _ <| '_ \  | |\/| |/ _` | '_ \| | | |/ _` | |
 * | |___) | (_) | (_) | | |  | | (_| | | | | |_| | (_| | |
 * |_|____/ \___/ \___/  |_|  |_|\__,_|_| |_|\__,_|\__,_|_|
 */                                                               


static uint8_t active[NR_SEGMENTS];


/* One TSS will be enough for all processes in ring 3. It will be used in Lab3. */
static TSS tss; 


inline static void
set_tss(SegDesc *ptr) {
	tss.ss0 = SELECTOR_KERNEL(SEG_KERNEL_DATA);		// only one ring 0 stack segment

	uint32_t base = (uint32_t)&tss;
	uint32_t limit = sizeof(TSS) - 1;
	ptr->limit_15_0  = limit & 0xffff;
	ptr->base_15_0   = base & 0xffff;
	ptr->base_23_16  = (base >> 16) & 0xff;
	ptr->type = STS_T32A;
	ptr->segment_type = 0;
	ptr->privilege_level = DPL_USER;
	ptr->present = 1;
	ptr->limit_19_16 = limit >> 16;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 0;
	ptr->base_31_24  = base >> 24;
}

/*inline static void set_tss_esp0(uint32_t esp) {
	tss.esp0 = esp;
}*/
void set_tss_esp0(uint32_t esp) {
	tss.esp0 = esp;
}
//static may be drop

/* GDT in the kernel's memory, whose virtual memory is greater than 0xC0000000. */
static SegDesc gdt[NR_SEGMENTS];

static void
set_segment(SegDesc *ptr, uint32_t pl, uint32_t type) {
	ptr->limit_15_0  = (KERNEL_SIZE >> 12)&0xffff;
	//ptr->limit_15_0  = 0xFFFF;
	ptr->base_15_0   = 0x0;
	ptr->base_23_16  = 0x0;
	ptr->type = type;
	ptr->segment_type = 1;
	ptr->privilege_level = pl;
	ptr->present = 1;
	//ptr->limit_19_16 = 0xF;
	ptr->limit_19_16 = (KERNEL_SIZE >> 28) & 0xf;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 1;
	ptr->base_31_24  = 0x0;
}

/* This is similar with the one in the bootloader. However the
   previous one cannot be accessed in user process, because its virtual
   address below 0xC0000000, and is not in the process' address space. */

uint32_t
new_segment(uint32_t type, uint32_t base, uint32_t limit, uint32_t dpl){
	printk("new_segment here working!\n");
	for(uint32_t i=1;i<NR_SEGMENTS;i++){
		if(active[i]==false){
			SEG(&(gdt[i]),type,base,limit,dpl);
			active[i]=true;
			return i;//return the index of gdt
		}
	}
	printk("No SegDesc available!\n");
	assert(0);
}


void
init_segment(void) {
	memset(gdt, 0, sizeof(gdt));
	memset(active, 0, sizeof(active));
	active[0]=active[1]=active[2]=active[3]=true;//these segments are being used
	set_segment(&gdt[SEG_KERNEL_CODE], DPL_KERNEL, SEG_EXECUTABLE | SEG_READABLE);
	set_segment(&gdt[SEG_KERNEL_DATA], DPL_KERNEL, SEG_WRITABLE );
	//set_segment(&gdt[SEG_USER_CODE], DPL_USER, SEG_EXECUTABLE | SEG_READABLE);
	//set_segment(&gdt[SEG_USER_DATA], DPL_USER, SEG_WRITABLE );

	write_gdt(gdt, sizeof(gdt));

	set_tss(&gdt[SEG_TSS]);
	write_tr( SELECTOR_USER(SEG_TSS) );
}

