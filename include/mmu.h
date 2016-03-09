#ifndef JOS_INC_MMU_H
#define JOS_INC_MMU_H

/*
 * This file contains definitions for the x86 memory management unit (MMU),
 * including and segmentation-related data structures and constants,
 * the %cr0, %cr4, and %eflags registers, and traps.
 */

#define MAXMEM			0x8000000
#define KERNEL_SIZE		0x400000
#define USER_STACK_SIZE	4096





// Control Register flags
#define CR0_PE		0x00000001	// Protection Enable
#define CR0_MP		0x00000002	// Monitor coProcessor
#define CR0_EM		0x00000004	// Emulation
#define CR0_TS		0x00000008	// Task Switched
#define CR0_ET		0x00000010	// Extension Type
#define CR0_NE		0x00000020	// Numeric Errror
#define CR0_WP		0x00010000	// Write Protect
#define CR0_AM		0x00040000	// Alignment Mask
#define CR0_NW		0x20000000	// Not Writethrough
#define CR0_CD		0x40000000	// Cache Disable
#define CR0_PG		0x80000000	// Paging

#define CR4_PCE		0x00000100	// Performance counter enable
#define CR4_MCE		0x00000040	// Machine Check Enable
#define CR4_PSE		0x00000010	// Page Size Extensions
#define CR4_DE		0x00000008	// Debugging Extensions
#define CR4_TSD		0x00000004	// Time Stamp Disable
#define CR4_PVI		0x00000002	// Protected-Mode Virtual Interrupts
#define CR4_VME		0x00000001	// V86 Mode Extensions

// Eflags register
#define FL_CF		0x00000001	// Carry Flag
#define FL_PF		0x00000004	// Parity Flag
#define FL_AF		0x00000010	// Auxiliary carry Flag
#define FL_ZF		0x00000040	// Zero Flag
#define FL_SF		0x00000080	// Sign Flag
#define FL_TF		0x00000100	// Trap Flag
#define FL_IF		0x00000200	// Interrupt Flag
#define FL_DF		0x00000400	// Direction Flag
#define FL_OF		0x00000800	// Overflow Flag
#define FL_IOPL_MASK	0x00003000	// I/O Privilege Level bitmask
#define FL_IOPL_0	0x00000000	//   IOPL == 0
#define FL_IOPL_1	0x00001000	//   IOPL == 1
#define FL_IOPL_2	0x00002000	//   IOPL == 2
#define FL_IOPL_3	0x00003000	//   IOPL == 3
#define FL_NT		0x00004000	// Nested Task
#define FL_RF		0x00010000	// Resume Flag
#define FL_VM		0x00020000	// Virtual 8086 mode
#define FL_AC		0x00040000	// Alignment Check
#define FL_VIF		0x00080000	// Virtual Interrupt Flag
#define FL_VIP		0x00100000	// Virtual Interrupt Pending
#define FL_ID		0x00200000	// ID flag



/*
 *
 *	Part 2.  Segmentation data structures and constants.
 *
 */

#define DPL_KERNEL              0
#define DPL_USER                3

//type for desc
#define SEG_WRITABLE			0X2
#define SEG_READABLE			0X2
#define SEG_EXECUTABLE			0X8
#define SEG_RW_DATA				0x2 //WRITEBLE
#define SEG_EXE_CODE			0xa	//READABLE|EXECUTABLE


#define NR_SEGMENTS             512
#define SEG_KERNEL_CODE         1 
#define SEG_KERNEL_DATA         2
//#define SEG_USER_CODE			3
//#define SEG_USER_DATA			4
#define SEG_TSS					3


//construct the selector for kernel or user
#define SELECTOR_KERNEL(s)		( (s << 3) | DPL_KERNEL )
#define SELECTOR_USER(s)		( (s << 3) | DPL_USER )



#ifdef __ASSEMBLER__

/*
 * Macros to build GDT entries in assembly.
 */
#define SEG_NULL						\
	.word 0, 0;						\
	.byte 0, 0, 0, 0
#define SEG(type,base,lim)					\
	.word (((lim) >> 12) & 0xffff), ((base) & 0xffff);	\
	.byte (((base) >> 16) & 0xff), (0x90 | (type)),		\
		(0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

#define SEL_KEL(s) \
	(s << 3) & 0xffff

#else	// not __ASSEMBLER__

#include <include/types.h>

/*
 *
 * type:
 *|bit 3|Data/Code	|	0/1	|
 *|bit 2|Expend-down|	0(normal)/1(expand-down) |
 *|bit 1|Writeble	|	0(read-only/execute-only) / 1(RW/executable and readable|
 *|bit 0|Accessed	|	0(hasn't been accessed)/1(accessed)|
 *
 * userful type:
 * 0x2 for R/W data segment
 * 0xa for executable code segment
 *
 */



// Segment Descriptors
typedef struct SegmentDescriptor {
	uint32_t limit_15_0          : 16;
	uint32_t base_15_0           : 16;
	uint32_t base_23_16          : 8;
	uint32_t type                : 4;
	uint32_t segment_type        : 1;
	uint32_t privilege_level     : 2;
	uint32_t present             : 1;
	uint32_t limit_19_16         : 4;
	uint32_t soft_use            : 1;
	uint32_t operation_size      : 1;
	uint32_t pad0                : 1;
	uint32_t granularity         : 1;
	uint32_t base_31_24          : 8;
} SegDesc;

// Null segment
#define SEG_NULL	(struct SegDesc){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
// Segment that is loadable but faults when used
#define SEG_FAULT	(struct SegDesc){ 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 }
// Normal segment
/*#define SEG(type, base, lim, dpl) (struct SegDesc)\
{ ((lim) >> 12) & 0xffff, (base) & 0xffff, ((base) >> 16) & 0xff,	\
    type, 1, dpl, 1, (unsigned) (lim) >> 28, 0, 0, 1, 1,		\
    (unsigned) (base) >> 24 }
#define SEG16(type, base, lim, dpl) (struct SegDesc)			\
{ (lim) & 0xffff, (base) & 0xffff, ((base) >> 16) & 0xff,		\
    type, 1, dpl, 1, (unsigned) (lim) >> 16, 0, 0, 1, 0,		\
    (unsigned) (base) >> 24 }
*/
static inline void
SEG(SegDesc *ptr, uint32_t type,uint32_t base, uint32_t limit, uint32_t dpl){
	ptr->limit_15_0  = (limit >> 12 ) & 0xFFFF;
	ptr->base_15_0   = base & 0xffff;
	ptr->base_23_16  = (base >> 16) & 0xff;
	ptr->type = type;
	ptr->segment_type = 1;
	ptr->privilege_level = dpl;
	ptr->present = 1;
	ptr->limit_19_16 = limit >> 28;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 1;
	ptr->base_31_24  = base >> 24;
}


static inline void
SEG16(SegDesc *ptr, uint32_t type,uint32_t base, uint32_t limit, uint32_t dpl){
	ptr->limit_15_0  = limit & 0xFFFF;
	ptr->base_15_0   = base & 0xffff;
	ptr->base_23_16  = (base >> 16) & 0xff;
	ptr->type = type;
	ptr->segment_type = 1;
	ptr->privilege_level = dpl;
	ptr->present = 1;
	ptr->limit_19_16 = limit >> 16;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 0;
	ptr->base_31_24  = base >> 24;
}


#endif /* !__ASSEMBLER__ */

// Application segment type bits
#define STA_X		0x8	    // Executable segment
#define STA_E		0x4	    // Expand down (non-executable segments)
#define STA_C		0x4	    // Conforming code segment (executable only)
#define STA_W		0x2	    // Writeable (non-executable segments)
#define STA_R		0x2	    // Readable (executable segments)
#define STA_A		0x1	    // Accessed

// System segment type bits
#define STS_T16A	0x1	    // Available 16-bit TSS
#define STS_LDT		0x2	    // Local Descriptor Table
#define STS_T16B	0x3	    // Busy 16-bit TSS
#define STS_CG16	0x4	    // 16-bit Call Gate
#define STS_TG		0x5	    // Task Gate / Coum Transmitions
#define STS_IG16	0x6	    // 16-bit Interrupt Gate
#define STS_TG16	0x7	    // 16-bit Trap Gate
#define STS_T32A	0x9	    // Available 32-bit TSS
#define STS_T32B	0xB	    // Busy 32-bit TSS
#define STS_CG32	0xC	    // 32-bit Call Gate
#define STS_IG32	0xE	    // 32-bit Interrupt Gate
#define STS_TG32	0xF	    // 32-bit Trap Gate


/*
 *
 *	Part 3.  Traps.
 *
 */

#ifndef __ASSEMBLER__

// Task state segment format (as described by the Pentium architecture book)
struct Taskstate {
	uint32_t ts_link;	// Old ts selector
	uintptr_t ts_esp0;	// Stack pointers and segment selectors
	uint16_t ts_ss0;	//   after an increase in privilege level
	uint16_t ts_padding1;
	uintptr_t ts_esp1;
	uint16_t ts_ss1;
	uint16_t ts_padding2;
	uintptr_t ts_esp2;
	uint16_t ts_ss2;
	uint16_t ts_padding3;
	physaddr_t ts_cr3;	// Page directory base
	uintptr_t ts_eip;	// Saved state from last task switch
	uint32_t ts_eflags;
	uint32_t ts_eax;	// More saved state (registers)
	uint32_t ts_ecx;
	uint32_t ts_edx;
	uint32_t ts_ebx;
	uintptr_t ts_esp;
	uintptr_t ts_ebp;
	uint32_t ts_esi;
	uint32_t ts_edi;
	uint16_t ts_es;		// Even more saved state (segment selectors)
	uint16_t ts_padding4;
	uint16_t ts_cs;
	uint16_t ts_padding5;
	uint16_t ts_ss;
	uint16_t ts_padding6;
	uint16_t ts_ds;
	uint16_t ts_padding7;
	uint16_t ts_fs;
	uint16_t ts_padding8;
	uint16_t ts_gs;
	uint16_t ts_padding9;
	uint16_t ts_ldt;
	uint16_t ts_padding10;
	uint16_t ts_t;		// Trap on task switch
	uint16_t ts_iomb;	// I/O map base address
};


typedef struct {
	uint32_t link;         // old ts selector
	uint32_t esp0;         // Ring 0 Stack pointer and segment selector
	uint32_t ss0;          // after an increase in privilege level
	char dontcare[88];
}TSS;


// Gate descriptors for interrupts and traps


struct GateDescriptor {
	uint32_t offset_15_0      : 16;//low 16bit of offset in segment
	uint32_t segment          : 16;//segment selector
	uint32_t pad0             : 8;//low 5#args, 0 for interrupt/trap gates high 3bit:reserved(should be 0 I guess)
	uint32_t type             : 4;//type(STS_{TG,IG32,TG32})
	uint32_t system           : 1;//must be 0(system)
	uint32_t privilege_level  : 2;//descriptor(meaning new)privilege
	uint32_t present          : 1;//Present
	uint32_t offset_31_16     : 16;//high bits of offset in segment
};


/*
struct TrapFrame {
	uint32_t edi, esi, ebp, esp_, ebx, edx, ecx, eax;
	int32_t irq;
};
*/

struct TrapFrame {
	uint32_t edi, esi, ebp, esp_;
	uint32_t ebx, edx, ecx, eax;   // Register saved by pushal
	uint32_t gs,fs,es,ds;   // Segment register

	int irq;
	uint32_t err;
	uint32_t eip;
	uint16_t cs;
	uint16_t padding3;
	uint32_t eflags; // Execution state before trap 
	uint32_t esp; 
	uint16_t ss;
	uint16_t padding4;
}__attribute__((packed));


typedef struct TrapFrame TrapFrame;

// Set up a normal interrupt/trap gate descriptor.
// - istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate.
    //   see section 9.6.1.3 of the i386 reference: "The difference between
    //   an interrupt gate and a trap gate is in the effect on IF (the
    //   interrupt-enable flag). An interrupt that vectors through an
    //   interrupt gate resets IF, thereby preventing other interrupts from
    //   interfering with the current interrupt handler. A subsequent IRET
    //   instruction restores IF to the value in the EFLAGS image on the
    //   stack. An interrupt through a trap gate does not change IF."
// - sel: Code segment selector for interrupt/trap handler
// - off: Offset in code segment for interrupt/trap handler
// - dpl: Descriptor Privilege Level -
//	  the privilege level required for software to invoke
//	  this interrupt/trap gate explicitly using an int instruction.
#define SETGATE(gate, istrap, sel, off, dpl)			\
{								\
	(gate).gd_off_15_0 = (uint32_t) (off) & 0xffff;		\
	(gate).gd_sel = (sel);					\
	(gate).gd_args = 0;					\
	(gate).gd_rsv1 = 0;					\
	(gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;	\
	(gate).gd_s = 0;					\
	(gate).gd_dpl = (dpl);					\
	(gate).gd_p = 1;					\
	(gate).gd_off_31_16 = (uint32_t) (off) >> 16;		\
}

// Set up a call gate descriptor.
#define SETCALLGATE(gate, sel, off, dpl)           	        \
{								\
	(gate).gd_off_15_0 = (uint32_t) (off) & 0xffff;		\
	(gate).gd_sel = (sel);					\
	(gate).gd_args = 0;					\
	(gate).gd_rsv1 = 0;					\
	(gate).gd_type = STS_CG32;				\
	(gate).gd_s = 0;					\
	(gate).gd_dpl = (dpl);					\
	(gate).gd_p = 1;					\
	(gate).gd_off_31_16 = (uint32_t) (off) >> 16;		\
}

// Pseudo-descriptors used for LGDT, LLDT and LIDT instructions.
struct Pseudodesc {
	uint16_t pd_lim;		// Limit
	uint32_t pd_base;		// Base address
} __attribute__ ((packed));

#endif /* !__ASSEMBLER__ */

#endif /* !JOS_INC_MMU_H */
