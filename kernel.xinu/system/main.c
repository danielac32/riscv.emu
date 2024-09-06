#include <xinu.h>
#include <rv.h>
#include <syscall.h>
#include <littlefs.h>
#include "compiler.h"
#include "object.h"
#include "vm.h"

static char *readFile(char *path,uint32 *t){
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
    *t=size;
    //printf("size file %d\n",size);
    buff = malloc(size+100);
    memset(buff,0,size);
    disk.read(buff,size,fs);
    disk.close(fs);
    buff[size] = '\0';
    return buff;
}
 

static int compilesrc(){
	FILE *file;
	uint32 t;
    char *input=readFile("/usr/test.monkey",&t);
    printf("size file %d\n",t);
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

    char *tmp=full_path("monkey");
    if(disk.exist(tmp)){
       printf("%s file found!\n",tmp );
       update_path();
       return -1;
    }
    
    char *p=malloc(code->instructions->size);
    memcpy(p,code,code->instructions->size);


    hexDump2(0,p,code->instructions->size);

    file=disk.open(tmp,LFS_O_CREAT);
    disk.write((void *)p,code->instructions->size,file);
    disk.close(file);
    
    exit_monkey:
    free_program(program);
	compiler_free(compiler);
	free(code);
    free(input);
    free(p);
    update_path();
    return 0;
}




static int runbytecode(){
	uint32 t;
	char *input=readFile("monkey",&t);
	printf("size file %d\n",t);
	hexDump2(0,input,t);


    struct vm *machine = vm_new((struct bytecode *)input);
    int err = vm_run(machine);
    if (err) {
        printf("Error executing bytecode: %d\n", err);
        goto exit_monkey;
    }
    
    exit_monkey:
    vm_free(machine);
    free(input);
    update_path();
	return 0;
}


static int runcode(){


FILE *file;
	uint32 t;
    char *input=readFile("/usr/test.monkey",&t);
    printf("size file %d\n",t);
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

    char *tmp=full_path("monkey");
    if(disk.exist(tmp)){
       printf("%s file found!\n",tmp );
       update_path();
       return -1;
    }
    
    char *p=malloc(code->instructions->size);
    memcpy(p,code,code->instructions->size);


    hexDump2(0,p,code->instructions->size);

    file=disk.open(tmp,LFS_O_CREAT);
    disk.write((void *)p,code->instructions->size,file);
    disk.close(file);
    
    free_program(program);
	//compiler_free(compiler);
	free(code);
    free(input);
    free(p);



    input=readFile("monkey",&t);
	printf("size file %d\n",t);
	hexDump2(0,input,t);


    struct vm *machine = vm_new((struct bytecode *)input);
    err = vm_run(machine);
    if (err) {
        printf("Error executing bytecode: %d\n", err);
        goto exit_monkey;
    }

    exit_monkey:
    free(input);
    vm_free(machine);
    update_path();
return 0;
}



int main(int argc, char const *argv[])
{

	//compilesrc();
    //runbytecode();
    runcode();



	return 0;
}
 