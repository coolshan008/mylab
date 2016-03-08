#ifndef __SEGNODE_H__
#define __SEGNODE_H__

struct segment_Node{
	uint32_t start;//The starting point
	uint32_t size;//the size of this segment
	uint8_t active;//this node is active when something is record
};

typedef struct segment_Node segment_Node;



#endif
