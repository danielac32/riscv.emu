OUTPUT_ARCH( "riscv" )
ENTRY( _start )



__stack_size = 8192;
MEMORY {
  RAM (rwx)  : ORIGIN = 0x80000000,  LENGTH = 512K
  FS  (rwx)  : ORIGIN = 0x8007d000,  LENGTH = 512K
}

SECTIONS
{

  . = 0x80000000;
  .text :  ALIGN( 4 ) {
        _text = .;
         *(.text .text.*)
        *(.rodata .rodata.*)
        _etext = .;
    }>RAM

    . = ALIGN(4);
    _sidata = .;
    .data : {
    . = ALIGN(4);
    _sdata = .;
        *(.data .data.*)
      . = ALIGN(4);
    _edata = .; 
    }>RAM

    . = ALIGN(4);

    _sbss = .;
    .bss : {
        *(.bss .bss.*)
    . = ALIGN(4);
    _ebss = .; 
    }>RAM

   
  .stack : ALIGN( 4 ) {
    _estack = .;
    . = . + __stack_size;
    _sstack = .;
    . = ALIGN(4);
    __global_pointer = . + 0x800;
    . = ALIGN(4);
    _end = ABSOLUTE(.);
  } >RAM
 
  
    . = 0x8007d000;
  fs = ABSOLUTE(.); 

  /* Define PROVIDE symbols at the end */
  PROVIDE( SYSCON = 0x11100001 );
  PROVIDE( TIMERL = 0x1100bff8 );
  PROVIDE( TIMERH = 0x1100bffc );
}
