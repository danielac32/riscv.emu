#include <xinu.h>
#include <rv.h>
#include <syscall.h>
#include <littlefs.h>



extern shellcmd xsh_ps(int nargs, char *args[]);

int main()
{
	ready(create(shell, 4096*2, 3, "shell", 0));
    xsh_ps(0,NULL);
    printf("run main\n");
	while(1){
		while(!kbhit()){
			yield();
		}
		
		printf("%c",readbyte());
	}
	//ready(create(shell, 4096, 3, "shell", 0));
	return 0;
}