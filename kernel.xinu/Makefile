

#PREFIX:=riscv64-unknown-elf-
#CFLAGS:=-I/usr/include
#export PATH=/opt/riscv/bin:$PATH
TOOLSET     ?= riscv32-unknown-elf-
CC           = $(TOOLSET)gcc
LD           = $(TOOLSET)ld
AR           = $(TOOLSET)gcc-ar
OBJCOPY      = $(TOOLSET)objcopy
 
 

RM = rm -f
fixpath = $(strip $1)

 
CFLAGS      ?= -O2 -march=rv32ima -mabi=ilp32 -static -ffreestanding -nostartfiles -lgcc -fno-builtin 
INCLUDES     =   -I include -I monkey/inc
LDSCRIPT     =  linker.ld

OBJDIR        = obj
START         = $(wildcard system/boot.S) $(wildcard system/intr.S) $(wildcard system/ctxsw.S)
STARTOBJ      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(START)))))
SOURCES       = $(wildcard system/*.c) 
OBJECTS       = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SOURCES)))))
SRCLIB        = $(wildcard lib/*.c)
LIBOBJ        = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCLIB)))))
#FLOATLIB      = $(wildcard floatmath/*.S)
#FLOATOBJ      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(FLOATLIB)))))
FSLIB         = $(wildcard littlefs/*.c)
FSOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(FSLIB)))))
SRCDEVICE     = $(wildcard device/nam/*.c) $(wildcard device/tty/*.c) 
DEVICEOBJ     = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCDEVICE)))))
SRCSHELL      = $(wildcard shell/*.c) 
SHELLOBJ      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCSHELL)))))

SRCMONKEY      =  $(wildcard monkey/src/*.c) 
MONKEYOBJ      =  $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCMONKEY)))))


DOUT         = kernel


SRCPATH = $(sort $(dir $(START) $(SOURCES) $(SRCLIB) $(FLOATLIB) $(FSLIB) $(SRCMONKEY) $(SRCSHELL) $(SRCDEVICE)))
vpath %.c $(SRCPATH)
vpath %.S $(SRCPATH)


 
$(OBJDIR):
	@mkdir $@



CFLAGS2 = -I include  -ffreestanding -nostdlib -nostartfiles -lgcc -fno-builtin  
CFLAGS2 +=-O3   -march=rv32ima -mabi=ilp32

objkern = \
system/boot.S \
system/ctxsw.S \
system/intr.S \
floatmath/*.S \
system/*.c   \
lib/*.c       \
device/nam/*.c       \
device/tty/*.c       \
shell/*.c       \
littlefs/*.c



mon:
	gcc -o mon monkey/src/*.c -I monkey/inc
	./mon mkfs/kernel/usr/test.monkey

kern: $(objkern)
	clear
	$(CC)  $(CFLAGS2) -T linker.ld -o kernel.elf $^
	riscv32-unknown-elf-objcopy kernel.elf -O binary kernel.bin

kernel: $(DOUT).bin
		riscv32-unknown-elf-objdump -d kernel.elf > kernel.list


$(DOUT).bin : $(DOUT).elf
	@echo building $@
	@$(OBJCOPY) -O binary $< $@



$(DOUT).elf : $(OBJDIR) $(STARTOBJ) $(LIBOBJ) $(FLOATOBJ) $(FSOBJ) $(DEVICEOBJ) $(MONKEYOBJ) $(SHELLOBJ) $(OBJECTS) 
	@echo building $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT) $(STARTOBJ) $(LIBOBJ) $(FLOATOBJ) $(MONKEYOBJ) $(FSOBJ) $(SHELLOBJ) $(DEVICEOBJ) $(OBJECTS) -o $@

	
clean: $(OBJDIR)
	$(MAKE) --version
	@$(RM) $(DOUT).*
	@$(RM) $(call fixpath, $(OBJDIR)/*.*)


$(OBJDIR)/%.o: %.S
	@echo assembling $<
	@$(CC) $(CFLAGS)  $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo compiling $<
	@$(CC) $(CFLAGS)  $(INCLUDES) -c $< -o $@

run:
	clear
	gcc -o mini-rv32ima ../mini-rv32ima/mini-rv32ima.c 
	./mini-rv32ima -f kernel.bin

riscv:
	gcc -o mini-rv32ima ../mini-rv32ima/mini-rv32ima.c 


fs:
	gcc -o mfs mkfs/*.c
	#./mfs -binary mkfs/kernel/shell
	./mfs mkfs/kernel
