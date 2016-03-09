#include "include/x86.h"
#include "include/segNode.h"


#define NR_SEG_NODES 1000
/*
 * We have totally 1000 segmentNode to 
 * record the segment information for
 * user process segment.
 * Currently, we won't dill with the case where we don't have 
 * enough node for a new segment.
 */



segment_Node segment_buffer[NR_SEG_NODES];


/*
 * initial the space available for user process
 * space area is [KERNEL_SIZE,MAXMEM - KERNEL_SIZE)
 */
void
init_segment_nodes(){
	for(int i = 0;i < NR_SEG_NODES;i++){
		segment_buffer[i].active = false;
	}
	segment_buffer[0].start = KERNEL_SIZE;//start from the end of kernel
	segment_buffer[0].size = MAXMEM - KERNEL_SIZE;//space size for user
	segment_buffer[0].active = true;//this node record a segment
}


/*
 * This func return a node which record segment with enough space
 * and will return NULL if not exist
 */
segment_Node *
get_free_node(uint32_t needSize){
	for(int i = 0;i < NR_SEG_NODES;i++){
		if(segment_buffer[i].active == true && segment_buffer[i].size>needSize)
			return &(segment_buffer[i]);
	}
	return NULL;
}


/*
 * need more to revoke memory
 */

