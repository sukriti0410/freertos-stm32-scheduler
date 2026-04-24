/* Linker script with proper vector table for Cortex-M3
   ARM Cortex-M3 boot sequence:
   1. CPU reads address 0x00000000 → initial stack pointer
   2. CPU reads address 0x00000004 → address of Reset_Handler
   3. CPU jumps to Reset_Handler → your code runs            */

ENTRY(Reset_Handler)

MEMORY {
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS {
    .text : {
        KEEP(*(.isr_vector))   /* Vector table MUST be at 0x00000000 */
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data : { *(.data*) } > SRAM AT > FLASH
    .bss  : { *(.bss*)  } > SRAM

    /* Stack at top of SRAM */
    _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
}