#include <xinu.h>
#include <rv.h>
#include <syscall.h>
#include <littlefs.h>

syscall_t syscallp;
littlefs_t disk;

static	void sysinit(); 	/* Internal system initialization	*/
 
extern int main(int argc, char const *argv[]);

struct  procent proctab[NPROC]; /* Process table            */
struct  sentry  semtab[NSEM];

int prcount;        /* Total number of live processes   */
pid32   currpid;        /* ID of currently executing process    */


void ilde(){

    while(1){
       // sleep(1);
    }
}


int nullprocess(void) {
resume(create(shell, 4096, 1, "shell", 0));
//ready(create(ilde, 2048, 3, "ilde", 0));
//ready(create(ilde, 2048, 3, "ilde", 0));
//while(1){
//sleep(100);
//}
return 0;
}


extern shellcmd xsh_blink(int nargs, char *args[]);

void	nulluser(){
struct	memblk	*memptr;	/* Ptr to memory block		*/
uint32	free_mem;	
uint32 t = csr_read(0xc00);
/*
w_mstatus(r_mstatus() | MSTATUS_MIE);
w_mie(r_mie() | MIE_MSIE);
w_mie(r_mie() | MIE_MTIE);*/

//enable();
sysinit();
preempt = QUANTUM;
 
kprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);

heap_init((void *)&_end);
uint32_t f=heap_free();
kprintf("free memory: %d\n",f);

kprintf("%10d bytes of Xinu code.\n",
	(uint32)&_etext - (uint32)&_text);
kprintf("           [0x%08X to 0x%08X]\n",
	(uint32)&_text, (uint32)&_etext - 1);
kprintf("%10d bytes of data.\n",
	(uint32)&_ebss - (uint32)&_sdata);
kprintf("           [0x%08X to 0x%08X]\n\n",
	(uint32)&_sdata, (uint32)&_ebss - 1);



kprintf("upload fs at (%08x)\n",(uint32)&fs);
upload_fs((uint32)&fs);
int ret;
int disk_free;
littlefs_init(&disk, &config, 500000);
#if 0
kprintf("startup_test: format    ");
ret = disk.format(disk.lfs, &config);
if(ret) 
{
    kprintf("[FAIL]\n");
    kprintf("Error Code: %d\n", ret);
    return ;
}
kprintf("[ OK ]\n");
#endif
kprintf("startup_test: mount     ");
ret = disk.mount(disk.lfs, &config);
if(ret) 
{
    kprintf("[FAIL]\n");
    kprintf("Error Code: %d\n", ret);
    return ;
}

kprintf("[ OK ]\n");
disk_free = disk.free();
kprintf("startup_test: available size = %d Bytes\n", disk_free);

//for (int i = 0; i < 10000; ++i)
//{
   //printf("");
//}
disable();
struct lfs_info info;

DIR *dir = disk.openDir("/");
if(dir==0){
    kprintf("not a directory ");
    return;
}
while (disk.readDir(dir, &info)> 0){
    if (strcmp(info.name, ".") && strcmp(info.name, "..") && strcmp(info.name, "//")){
        if(disk.exist(info.name)){
			if (info.type == LFS_TYPE_DIR){
               	kprintf(" /%s\n",info.name);
            }else{
                kprintf(" %s %d \n",info.name,info.size);
            }
        }
    }
}
disk.closeDir(dir);


asm("nop");
asm("nop");
asm("nop");
uint32 q = csr_read(0xc00);
kprintf("time: %d\n",q-t);


/*
int pid = create((void *)nullprocess, 2048, 10, "Null process", 0, NULL);
struct procent * prptr = &proctab[pid];
prptr->prstate = PR_CURR;
//init_timer(10);
timel = 1;
nullprocess();
*/

f=heap_free();
kprintf("free memory: %d\n",f);

main(0,NULL);//xsh_blink(0,NULL);
f=heap_free();
kprintf("free memory: %d\n",f);

for (;;);

//timeh = 0;
int pid=create(ilde, 2048, 1, "nullp", 0);
struct procent * prptr = &proctab[pid];
prptr->prstate = PR_CURR;

//timel = 1;
init_timer(10);
//ready(create(ilde, 2048, 3, "ilde", 0));
ready(create(nullprocess, 2048, 1, "start", 0));

//ready(create(main, 4096, 3, "main", 0));
//ready(create(shell, 4096, 3, "shell", 0));

//timeh = 0;
//init_timer(10);

//timel = 1;
//
for(;;);//
}


static	void	sysinit()
{
	int32	i;
	struct procent *prptr;     /* Ptr to process table entry   */
    struct  sentry  *semptr;
    clkinit();
    syscall_init(&syscallp);
	/* Platform Specific Initialization */
    //meminit();
	  //platinit();
	clear_interrupts();
	enable_interrupt_status();
	enable_interrupt(MTIM);
	enable_interrupt(MSIT);
 
	//kprintf(CONSOLE_RESET);
	for (int i = 0; i < 10; ++i)
	{
		kprintf("\n");
	}
	
	prcount = 0;
//	prcount = 1;


	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->elf = FALSE;
        prptr->prprio = 0;
	}


    for (i = 0; i < NSEM; i++) {
        semptr = &semtab[i];
        semptr->sstate = S_FREE;
        semptr->scount = 0;
        semptr->squeue = newqueue();
    }

    
    readylist = newqueue();

	
	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}
