#include "include/x86.h"
#include "include/list.h"
#include "include/assert.h"
#include "include/process.h"
#include "include/string.h"
#include "include/elf.h"
/*
 * 没有交换上下文时设置tss的tss0
 *
 */ 


#define SECTORS_SIZE 512
#define USTACKTOP 4095
#define ELF_MAGIC 0x464C457FU
#define ELF_PROG_LOAD 1
#define PT_LOAD 1

#define USER_OFFSET 102400
//此函数可用于测试

void set_tss_esp0(uint32_t esp);
void set_tss_ss0(uint32_t ss);
static ListHead ready,block,free;
void readseg(unsigned char *pa, int count, int offset);
//uint32_t segment_malloc(ProgramHeader *, TrapFrame *, int32_t *);
uint32_t segment_malloc(ProgramHeader *, TrapFrame *);
void stack_malloc(TrapFrame *, int32_t *);

void print_stack(TrapFrame *tf)
{
	printk("tf=0x%x\n",(uint32_t)tf);
	printk("edi = 0x%x\n",tf->edi);
	printk("esi = 0x%x\n",tf->esi);
	printk("ebp = 0x%x\n",tf->ebp);
	printk("esp_ = 0x%x\n",tf->esp_);
	printk("ebx = 0x%x\n",tf->ebx);
	printk("edx = 0x%x\n",tf->edx);
	printk("ecx = 0x%x\n",tf->ecx);
	printk("eax = 0x%x\n",tf->eax);

//	printk("gs = %d\n",tf->gs);
//	printk("fs = %d\n",tf->fs);
	printk("es = %x\n",tf->es);
	printk("ds = %x\n",tf->ds);
	printk("irq = %d\n",tf->irq);
	printk("err = %x\n",tf->err);
	printk("eip = 0x%x\n",tf->eip);


	printk("cs = 0x%x\n",tf->cs);
	printk("eflags = 0x%x\n",tf->eflags);
	printk("esp = 0x%x\n",tf->esp);
	printk("ss = %d\n",tf->ss);
}


void UTrapFrame_init(TrapFrame *tf)
{
	
	tf -> es = tf -> ds; 
	tf -> eflags = 0x00000202;
	tf->esp= USTACKTOP;

}

//PCBPool init
//进程池的初始化
void PCBPool_init()
{
	list_init(&ready);
	list_init(&block);
	list_init(&free);
	int i;
	for(i=0;i<PCB_Pool_NUM;i++)
		list_add_after(&free,&(PCBPool[i].list));
	if(free.next==&free)
		printk("empty\n");
}

//创建线程/进程的基本框架 
PCB *new_PCB()
{
	printk("new_PCB is now working!\n");
	PCB *new_pcb=list_entry(free.next,PCB,list);
	if(new_pcb!=NULL){
	list_del(&(new_pcb->list));
	list_add_after(&ready,&(new_pcb->list));}
	return new_pcb;
}


/*static int process_setup_vm(PCB *pcb)
{
	int res;
	PDE *upgdir;
	Page *page = NULL;

	res = page_alloc(&page,GFP_USER);
	   GFP_KERNEL表示分配的物理空间在16M以内，GFP_USER表示分配的物理内存
	   在16M以外
	   在设置进程地址空间时，需要将内核的3G以上的地址空间赋给用户地址线性空
	   间的3G以上

	if(res < 0)
	    return res;
    
	pcb->pgdir=(void *)(page2kva(page));//分配
	upgdir=(void *)(page2kva(page));
//	upgdir=(void *)(page2pa(page));//分配
	memcpy(upgdir,kpgdir,PG_SIZE);//复制内核目录

	printk("process_setup_vm is here working! %s %d\n",__FILE__,__LINE__);
	return 0;
}
*/
//复制内核页目录表


//创建一个进程
int process_fork(PCB **new_pcb)
{
	printk("process_fork %d is now working!\n",__LINE__);
	void *tf;

    /*
	   分配一个进程块，并对其进行初始化
	 */
	*new_pcb=new_PCB();
	if(*new_pcb==NULL)
		return -1;
	tf=(void *)(&((*new_pcb)->kstack)[KSTACK_SIZE-sizeof(TrapFrame)-1]);
	//tf=(void *)((*new_pcb)->kstack);//tf放在栈底
	(*new_pcb)->tf=tf;
	//idt=(void *)((*new_pcb)->kstack);
	//(*new_pcb)->idt=idt;//把idt放在了内核栈里

	return 1;
}

//从内核空间到用户空间的返回
void pcb_pop_tf(TrapFrame *tf)
{	
	printk("pcb_pop_tf here is working! %s %d\n",__FILE__,__LINE__);
	//print_stack(tf);



	//printk("pcb_pop_tf here is working! %s %d\n",__FILE__,__LINE__);
	asm volatile("movl %0,%%esp": :"g"(tf):"memory");
	asm volatile("popal");
	asm volatile("popl %%gs"::);
	asm volatile("popl %%fs"::);
	asm volatile("popl %%es"::);
	asm volatile("popl %%ds"::);
	asm volatile("addl $0x8,%%esp"::);
	asm volatile("iret");

	
	printk("pcb_pop_tf failed! %s %d\n",__FILE__,__LINE__);
	panic("iret failed!\n");

}

//让用户进程执行
void process_exec(PCB *pcb)
{
	/*
	 * 加载新的idt
	   让用户程序运行起来，核心是加载相应的页目录和进入ring3环境
	 */
	set_tss_esp0((uint32_t)((pcb->kstack)+KSTACK_SIZE));//设置esp0为该进程内核栈顶
	printk("process_exec is here working %s %d\n",__FILE__,__LINE__);
	pcb_pop_tf((TrapFrame *)(pcb->tf));
	printk("process_exec is working with fault%s %d\n",__FILE__,__LINE__);
}


//将用户进程的二进制文件直接从磁盘上加载到物理内存上
static void process_load(PCB *pcb)
{
	struct ELFHeader *elf;
	struct ProgramHeader *ph;
	int i;
	uint8_t *j;
	//int32_t count;
	uint32_t pa;

	unsigned char buf[4096];
	readseg(buf,4096,USER_OFFSET);
	elf=(struct ELFHeader *)buf;
	//printk("magic = %x\n",elf->magic);
	assert(elf->magic == ELF_MAGIC);
	ph=(ProgramHeader *)((uint32_t)elf+elf->phoff);
	pa = segment_malloc(ph, (TrapFrame *)(pcb->tf));//获取段基址
	//process_setup_vm(pcb);
	for(i=0; i< elf->phnum; i++) {
		if(ph->type==PT_LOAD){
			readseg((unsigned char *)(pa+ ph->vaddr), ph->filesz, USER_OFFSET+ph->off); /* 读入数据 */
			for (j = (uint8_t *)(pa + ph->vaddr+ ph->filesz); (uint32_t)j < pa + ph->vaddr+ ph->memsz; *j ++ = 0);
		}
		ph++;
	}
	((TrapFrame *)(pcb->tf))->esp= USER_STACK_TOP;//stack top
	((TrapFrame *)(pcb->tf))->eip=elf->entry;//eip initial
	printk("eip = 0x%x  in %s\n",elf->entry,__FUNCTION__);
	//segment_alloc(pcb,(void *)USTACKTOP,PG_SIZE);
	//stack_malloc((TrapFrame *)(pcb->tf), &count);
	printk("process_load is here working! %s %d\n",	__FILE__,__LINE__);
}

//在内核空间创建一个用户进程，并从此跳转到用户空间执行此进程
int enter_process_userspace(void)
{
	PCB *pcb;
	TrapFrame *tf;
	int res;

	printk("enter_process_userspace is working %s %d\n",\
			__FILE__,__LINE__);
	res = process_fork(&pcb);
	printk("res=%d",res);
	if(res < 0)
		return -1;
	printk("enter_process_userspace process_fork is working %s %d \n",\
			__FILE__,__LINE__);
    
	process_load(pcb);
	printk("enter_process_userspace process_load is working %s %d\n",\
			__FILE__,__LINE__);
	tf= pcb->tf;
	UTrapFrame_init(tf);//此时初始化tf是因为只有这时cs,ds,ss才初始化完成
	//may wrong

	process_exec(pcb);
	printk("enter_process_userspace process_exec fail%s %d\n",\
			__FILE__,__LINE__);

	printk("enter_process_userspace is working %s %d \n",__FILE__,\
			__LINE__);
    
	return 0;
}
