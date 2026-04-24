/* ============================================================
   Day 1 Final: Cortex-M3 with proper vector table
   
   Cortex-M3 boot sequence (this is how ALL ARM chips start):
   1. Read stack pointer value from address 0x00000000
   2. Read Reset_Handler address from 0x00000004
   3. Jump to Reset_Handler
   4. Reset_Handler calls main()
   
   Without this vector table, the CPU has no idea where to
   start — it jumps to garbage and crashes. That's exactly
   what happened before this fix.
   ============================================================ */

/* Semihosting — QEMU captures this and prints to your terminal */
#define SYS_WRITE0 0x04

static void semi_puts(const char *str) {
    __asm volatile (
        "mov r0, %[op]\n"
        "mov r1, %[arg]\n"
        "bkpt 0xAB\n"
        :
        : [op] "r" (SYS_WRITE0), [arg] "r" (str)
        : "r0", "r1", "memory"
    );
}

/* ── Main program ───────────────────────────────────────────── */
int main(void) {
    semi_puts("==============================\n");
    semi_puts("  Day 1: Bare-metal ARM C\n");
    semi_puts("  Sukriti Mukherjee\n");
    semi_puts("  FreeRTOS Project - Day 1/10\n");
    semi_puts("==============================\n");
    semi_puts("Toolchain:  OK\n");
    semi_puts("QEMU:       OK\n");
    semi_puts("GitHub:     OK\n");
    semi_puts("Ready for Day 2.\n");

    while (1) { }
    return 0;
}

/* ── Vector table — MUST be at address 0x00000000 ───────────── */
/* The linker script places .isr_vector section first in FLASH   */

/* Forward declaration — stack top comes from linker script */
extern unsigned int _stack_top;

/* Reset handler — CPU jumps here on power-on or reset */
void Reset_Handler(void) {
    main();         /* Just call main */
    while (1) { }  /* Never return */
}

/* Minimal vector table — first 2 entries are all Cortex-M3
   needs to boot. Entry 0 = initial stack pointer.
   Entry 1 = address of Reset_Handler.                    */
__attribute__((section(".isr_vector")))
void (*const vector_table[])(void) = {
    (void (*)(void))&_stack_top,   /* 0x00: Initial stack pointer */
    Reset_Handler,                  /* 0x04: Reset handler address */
};