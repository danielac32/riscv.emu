/* printf.c - printf*/

#include <xinu.h>
//#include <stdio.h>
#include <syscall.h>

 
extern void _fdoprnt(char *, va_list, int (*)(did32, char), int);
//extern void _fdoprnt2(char *, va_list, int (*)(int,did32, char), int,int);
//extern int xinu_putc(did32, char);


 
/*------------------------------------------------------------------------
 *  printf  -  standard C printf function
 *------------------------------------------------------------------------
 */


/*
static int __putc(int c, int b){
    __syscall8(0,0,b);//
    //kputc(b);
    return OK;
}*/


/*
int printf2(const char *fmt, ...){

    va_list ap;

    va_start(ap, fmt);
    __syscall(XINU_TEST2,(char *)fmt, ap);
    va_end(ap);
    return OK;
}
*/ 


extern  void    _doprnt(char *, va_list, int (*)(int));

extern syscall kputc(
      byte  c           /* character to write       */
    );


int printf(
      const char        *fmt,
      ...
    )
{
    va_list ap;

    va_start(ap, fmt);
   // _fdoprnt((char *)fmt, ap, putc, CONSOLE);
    _doprnt(fmt, ap, (int (*)(int))kputc);
    va_end(ap);

    return 0;
}


void hexDump2(unsigned int offset, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

void hexDump(unsigned int offset,char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Output description if given.
    if (desc != NULL)
        printf("%x (%s):\n", addr, desc);
    else
        printf("%x:\n", addr);
    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}
