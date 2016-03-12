#include "include/stdio.h"
#include "include/assert.h"
#include "include/x86.h"
#include "include/segNode.h"
#include "include/elf.h"




void init_segment_nodes();
segment_Node * get_free_node(uint32_t);
uint32_t new_segment(uint32_t,uint32_t,uint32_t ,uint32_t);


//init the memory manage unit
void
init_mmu(){
	init_segment_nodes();
}



uint32_t
get_string_from_user(TrapFrame *tf, uint32_t ptr){
	printk("In get_string_from_user \n");
	printk("ptr= 0x%x\n",ptr);
	uint32_t pa=KERNEL_SIZE + SELECTOR_INDEX(tf->ds) * USER_SIZE + ptr;
	printk("pa=0x%x\n",pa);
	return pa;
}
//return the pa of the ptr



uint32_t
segment_malloc(ProgramHeader *ph, TrapFrame *tf){
	/*
	 * alloc memory and fill in the idt
	 * */
	printk("Here work in segment_malloc in %s\n",__FILE__);
	segment_Node *Node = get_free_node(USER_SIZE);
	if(Node == NULL)
		assert(0);
	uint32_t pa = Node -> start;
	Node ->start += USER_SIZE;
	Node ->size -= USER_SIZE;
	//malloc memory of n * align size
	if(Node -> size ==0)
		Node -> active =false;
	printk("pa = %x\n",pa);
	tf->ds=SELECTOR_USER(new_segment(SEG_RW_DATA,pa,USER_SIZE,DPL_USER));
	tf->cs=SELECTOR_USER(new_segment(SEG_EXE_CODE,pa,USER_SIZE,DPL_USER));
	//tf->ss=SELECTOR_USER(new_segment(SEG_RW_DATA,pa,USER_STACK_SIZE,DPL_USER));//stack segment
	//tf->cs=tf->ds;
	tf->ss=tf->ds;
	/*
	 * Here, ds,cs,ss share the same memory
	 * it equals to the case where ds=cs=ss
	 */
	//tf->cs= tf->ds;
	//tf->ss=tf->ds;
	printk("ds= %d\n",tf->ds);
	printk("cs= %d\n",tf->cs);
	printk("ss= %d\n",tf->ss);
	printk("pa = 0x%x\n",pa);
	//printk("segdesc = %08x\n",*((uint64_t *)ph));
	return pa;
}



/*uint32_t
segment_malloc(ProgramHeader *ph, TrapFrame *tf, int32_t *count){
	printk("Here work in segment_malloc in %s\n",__FILE__);
	uint32_t memsz = ph->memsz;
	uint32_t align = ph->align;
	uint32_t n = (memsz / align) + 1;
	printk("n= %d in %s\n",n,__FILE__);
	segment_Node *Node = get_free_node(n * align);
	if(Node == NULL)
		assert(0);
	uint32_t pa = Node -> start;
	Node ->start += n * align;
	Node ->size -= n * align;
	//malloc memory of n * align size
	if(Node -> size ==0)
		Node -> active =false;


	if(ph->flags==6){
		tf->ds=SELECTOR_USER(new_segment(SEG_RW_DATA,pa,n * align,DPL_USER));
	}//data segmemt
	else if(ph->flags==5){
		tf->cs=SELECTOR_USER(new_segment(SEG_EXE_CODE,pa,n * align,DPL_USER));
	}//code segment
	else
		assert(0);

	printk("ds= %d\n",tf->ds);
	printk("cs= %d\n",tf->cs);
	printk("pa = 0x%x\n",pa);
	//printk("segdesc = %08x\n",*((uint64_t *)ph));
	(*count)++;
	return pa;
}
*/

void stack_malloc(TrapFrame *tf, int32_t *count){
	printk("Here work in stack_malloc in %s\n",__FILE__);
	segment_Node *Node = get_free_node(USER_STACK_SIZE);
	uint32_t pa = Node -> start;
	Node ->start += USER_STACK_SIZE;
	Node ->size -= USER_STACK_SIZE;
	tf->ss=SELECTOR_USER(new_segment(SEG_RW_DATA,pa,USER_STACK_SIZE,DPL_USER));//stack segment
	printk("ss= %d\n",tf->ss);
	printk("stack starts at 0x%x\n",pa);
	(*count)++;
}//need to fill PCB

	
