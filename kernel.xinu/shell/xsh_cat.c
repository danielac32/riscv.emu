/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <littlefs.h>


void cat(char *input,char *output) {
    FILE *file;
    char ch;
    //char buffer[256];
    int bytes;
    
    char *tmp=full_path(input);
    if (tmp==NULL){
        update_path();
        return;
    }
    if(!disk.exist(tmp)){
       //printf("%s not found!\n",tmp );
       //update_path();
       file=disk.open(tmp,LFS_O_CREAT);
       disk.close(file);
       //return;
    }


    if(output==NULL){
        file=disk.open(tmp,LFS_O_RDWR);
        while(disk.available(file)){
           disk.read(&ch,1,file);
           //putc(CONSOLE,ch);
           printf("%c",ch );
        }
        disk.close(file);
        printf("\n");
    }else{
        file=disk.open(tmp,LFS_O_CREAT);
        disk.write(output,strlen(output),file);
        //disk.write(" ",1,fs);
        disk.close(file);
    } 
    update_path();
}


shellcmd xsh_cat(int nargs, char *args[])
{

 
    if (nargs == 2) {
        // cat archivo.txt
        cat(args[1],NULL);
        //printf("cat archivo.txt\n");
    } else if (nargs == 4 && strcmp(args[2], ">") == 0) {
        // cat archivo.txt > nuevo_contenido
        cat(args[1], args[3]);
        //printf("%s\n",(char *)&args[3] );

        //printf("cat archivo.txt > nuevo_contenido\n");
    }else {
        printf("Usage: cat archivo.txt [> nuevo_contenido]\n");
    }


   


	 #if 0
     FILE* fd
;
	/* For argument '--help', emit help about the 'cat' command	*/
    

 
   /*	if (nargs == 2 && strcmp(args[1], "h") == 0) {
   		//cat 
		printf("Use: %s [file...]\n\n", args[0]);
		printf("Description:\n");
		printf("\twrites contents of files or stdin to stdout\n");
		printf("Options:\n");
		printf("\tfile...\tzero or more file names\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}*/
    
    
    if(nargs <2)return -1;

    if (!strcmp(args[nargs-2],">")){
    	char *tmp=full_path((char*)args[1]);
    	if (tmp==NULL)return -1;

    	fd = fopen(tmp, "w");
        if(!fd){
           printf("%s found!\n",tmp );
           goto done;//syscallp.updatepath();//update_path();//strcpy(path, curdir);
        }
        for (int i = 1; i < nargs-2; ++i)
        {
            fputs(args[i],fd);
            fputs(" ",fd);
        }
    }else{
 		char *tmp=full_path((char*)args[1]);
 		if (tmp==NULL)return -1;
	    if (!(fd = fopen(tmp,"r"))){
            printf("%s not found\n", tmp);
            goto done;
        }
        char buff[64];
        while(!feof(fd)){
              char c = fgetc(fd);
              putc(CONSOLE,c);
        }
    }
    done:
    printf("\n");
    fclose(fd);
	update_path();
  #endif
	return 0;
}
