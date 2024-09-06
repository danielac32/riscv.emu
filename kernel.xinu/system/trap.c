
#include <trap.h>
/* #include <stdint.h>
#include <kernel.h>
#include <interrupt.h>
#include <string.h>
#include <rv.h>
#include <stdio.h>

struct irq_context
{
    unsigned int pc;
    unsigned int status;
    unsigned int cause;
    unsigned int reg[NUM_GP_REG];
};*/

//void    exception_dump_ctx(struct irq_context *ctx);
//struct irq_context * exceptionHandler(struct irq_context *ctx);
//struct irq_context *do_syscall(struct irq_context *ctx);

/*
struct irq_context *do_syscall(struct irq_context *ctx)
{
    uint32_t a0 = ctx->reg[REG_ARG0 + 0];
    uint32_t a1 = ctx->reg[REG_ARG0 + 1];
    uint32_t a2 = ctx->reg[REG_ARG0 + 2];
    uint32_t a3 = ctx->reg[REG_ARG0 + 3];
    uint32_t a4 = ctx->reg[REG_ARG0 + 4];
    uint32_t a5 = ctx->reg[REG_ARG0 + 5];
    uint32_t n  = ctx->reg[REG_ARG0 + 7];
    int      fd = (int)a0;

    
    return ctx;
}
*/

//extern void _start();
void exception_dump_ctx(struct irq_context *ctx)
{
    kprintf("Exception: PC=%08x Cause=%x Status=%08x\n", ctx->pc, ctx->cause, ctx->status);
    for (int i=0;i<NUM_GP_REG;i++)
    {
        kprintf("REG %.2d: %08x\n", i, ctx->reg[i]);
    }
    uint32_t value = 0x5555;  //0x5555 El valor que quieres almacenar
    asm volatile (
        "lui t0, 0x11100       \n"  // Carga los 20 bits superiores (0x11100000) en t0
        //"addi t0, t0, 1        \n"  // Añade los 12 bits inferiores (0x00000001) a t0
        "sw %0, 0(t0)"         // Almacena el valor en la dirección contenida en t0
        :                      // No salida
        : "r" (value)          // Entrada: el valor a almacenar
        : "t0", "memory"       // Clobbers: `t0` y `memory` (ya que estamos modificando memoria)
    );
    for(;;);
}

struct irq_context * exceptionHandler(struct irq_context *ctx)
{
	  unsigned int cause_code = ctx->cause & 0xfff;
 	  clear_interrupts();


		if (ctx->cause & CAUSE_INTERRUPT){
        switch (cause_code)
		    {
		    case 3:
		      kprintf("software interruption!\n");
		      ctx->pc += 4;
		      break;
		    case 7:
		    	{
		    		//asm volatile(".option arch, +zicsr");
		    		//w_mie(~((~r_mie()) | (1 << 7)));
		    		 
		            //ctx->pc = (unsigned int )&clkhandler;
		    		
		    		clkhandler();
		    		//w_mie(r_mie() | MIE_MTIE);

		    	}
		      break;
		    case 11:
		         kprintf("external interruption!\n");
		      break;
		    default:
		      kprintf("unknown async exception!\n");
		      break;
		    }
    	}else{
    		switch (cause_code){
    		case 1:
    			{
    				uint32_t val;
    				asm volatile(".option arch, +zicsr");
                    asm volatile(".option norvc\ncsrr %0, 0x343":"=r" (val));
    				kprintf("CAUSE_FAULT_FETCH %08x\n",val);
    				exception_dump_ctx(ctx);
    			}
    			//ctx->pc += 1;
    			break;
		    case 2:
		      kprintf("Illegal instruction!\n");
		      exception_dump_ctx(ctx);
		      break;
		    case 5:
		      kprintf("Fault load!\n");
		      exception_dump_ctx(ctx);
		      break;
		    case 7:
		      kprintf("Fault store!\n");
		      exception_dump_ctx(ctx);
		      break;
		    case 8:
		    case 11:
		       ctx=do_syscall(ctx);
		    break;
		    default:
		        kprintf("Sync exceptions! cause code: %d\n", cause_code);
		        exception_dump_ctx(ctx);
		    break;
      }
    }
return ctx;
}
