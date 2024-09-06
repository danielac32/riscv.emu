/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include "compiler.h"
#include "object.h"
#include "vm.h"
#include <littlefs.h>



/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */

char *readfile(char *path){
	char *tmp=full_path("monkey");
    if (tmp==NULL){
        update_path();
        return NULL;
    }

    if(!disk.exist(tmp)){
       printf("%s file not found!\n",tmp );
       update_path();
       return NULL;
    }

    uint8 *buff;
  
    FILE *fs=disk.open(tmp,LFS_O_RDWR);
    int size = disk.size(fs);
    printf("size file %d\n",size);
    buff = malloc(size+100);
    memset(buff,0,size);
    disk.read(buff,size,fs);
    disk.close(fs);
    buff[size] = '\0';
    return buff;
}



shellcmd xsh_tt(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/
    char *input=readfile(args[1]);
    if(input==NULL)return 0;
    struct vm *machine = vm_new((struct bytecode *)input);
    //struct vm *machine = vm_new(code);

    hexDump2(0,input,300);
    int err = vm_run(machine);
    if (err) {
        printf("Error executing bytecode: %d\n", err);
        goto exit_monkey;
    }
    
    exit_monkey:
 
 
    free(input);
    update_path();
 
 
	return 0;
}
