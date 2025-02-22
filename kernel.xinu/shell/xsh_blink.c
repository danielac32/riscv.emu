/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include "compiler.h"
#include "object.h"
#include "vm.h"
#include <littlefs.h>

/*char *input;
struct lexer lexer;
struct parser parser;
struct program *program;*/
/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */

char *readFile(char *path){
	char *tmp=full_path(path);
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

//char *input;


int example0(int nargs, char *args[])
{
	//input=readFile("/usr/test.monkey");
	/*input=readFile("/usr/test.monkey");
    struct lexer lexer = new_lexer(input);
	struct parser parser = new_parser(&lexer);
	struct program *program = parse_program(&parser);*/

	 
	printf("example\n");
   
	return OK;
}


shellcmd xsh_blink(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/
    //hw_cfg_pin(GPIOx(GPIO_C),     13,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
   
    FILE *file;
    //resume(create(example0, 2048, 1, "ex_0",0));
    //receive();

    char *input=readFile("/usr/test.monkey");
    struct lexer lexer = new_lexer(input);
	struct parser parser = new_parser(&lexer);
	struct program *program = parse_program(&parser);


	if (parser.errors > 0) {
		for (int8_t i = 0; i < parser.errors; i++) {
			printf("%s\n",parser.error_messages[i]);
		}

		goto exit_monkey;
	}


	struct compiler *compiler = compiler_new();
	int err = compile_program(compiler, program);
	if (err) {
		printf("%s\n",compiler_error_str(err));
		goto exit_monkey;
	}

	struct bytecode *code = get_bytecode(compiler);
    //printf("%d\n", code->instructions->size);
    //hexDump2(0,code,code->instructions->size);
    char *p=malloc(code->instructions->size);
    memcpy(p,code,code->instructions->size);
    //hexDump2(0,code,code->instructions->size);


    char *tmp=full_path("monkey");
    if(disk.exist(tmp)){
       printf("%s file found!\n",tmp );
       update_path();
       return -1;
    }
    
    file=disk.open(tmp,LFS_O_CREAT);
    disk.write(p,code->instructions->size,file);
    disk.close(file);
    free(p);
    



    char *bc=readFile("monkey");
    hexDump2(0,bc,code->instructions->size);
    struct vm *machine = vm_new((struct bytecode *)bc);
    //struct vm *machine = vm_new((struct bytecode *)p);
	//struct vm *machine = vm_new(code);
	err = vm_run(machine);
	if (err) {
		printf("Error executing bytecode: %d\n", err);
		goto exit_monkey;
	}
    free(bc);
    exit_monkey:
	free_program(program);
	compiler_free(compiler);
	free(code);
	vm_free(machine);
    free(input);
    update_path();
    return 0;
    while(1){
    	//printf("on\n");
		sleepms(100);
		//printf("off\n");
		sleepms(100);
    }
	return 0;
}
