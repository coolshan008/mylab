#include "include/stdio.h"
#include "include/assert.h"
#include "include/x86.h"
#include "include/segNode.h"
#include "include/elf.h"


void init_segment_nodes();
segment_Node * get_free_node(uint32_t);


/*struct segment_list{
	segment_Node * ptr;
	segment_list *next;
};
typedef struct segment_list segment_list;

*/

void
init_mmu(){
	init_segment_nodes();
}



/*
 * also need to fill PCB
 *
 */
uint32_t
segment_malloc(ProgramHeader *ph, SegDesc *idt, int32_t count){
	/*
	 * alloc memory and fill in the idt
	 * */
	printk("Here work in segment_malloc in %s\n",__FILE__);
	uint32_t memsz = ph->memsz;
	uint32_t align = ph->align;
	uint32_t n = (memsz % align) + 1;
	segment_Node *Node = get_free_node(n * align);
	uint32_t pa = Node -> start;
	Node ->start += n * align;
	Node ->size -= n * align;
	//malloc memory of n * align size
	if(Node -> size ==0)
		Node -> active =false;

	if(ph->flags==6)
		SEG(idt + count,SEG_RW_DATA,pa,n * align,DPL_USER);
	else if(ph->flags==5)
		SEG(idt + count,SEG_EXE_CODE,pa,n * align,DPL_USER);
	else
		assert(0);

	count++;
	return pa;
}

void stack_malloc(SegDesc *idt, int32_t count){
	printk("Here work in stack_malloc in %s\n",__FILE__);
	segment_Node *Node = get_free_node(USER_STACK_SIZE);
	uint32_t pa = Node -> start;
	Node ->start += USER_STACK_SIZE;
	Node ->size -= USER_STACK_SIZE;
	SEG(idt + count,SEG_RW_DATA,pa,USER_STACK_SIZE,DPL_USER);
	count++;
}//need to fill PCB

	
