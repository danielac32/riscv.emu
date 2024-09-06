
CC = riscv32-unknown-elf-gcc
CFLAGS2 = -I include  -ffreestanding -static -nostartfiles -lgcc -fno-builtin  
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


kern: $(objkern)
	clear
	$(CC)  $(CFLAGS2) -T linker.ld -o kernel.elf $^
	riscv32-unknown-elf-objcopy kernel.elf -O binary kernel.bin