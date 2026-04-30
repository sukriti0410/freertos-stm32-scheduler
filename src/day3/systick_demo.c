#include <stdint.h>

/* ── What this file teaches:
   SysTick interrupt — fires automatically every N cycles
   ISR updates a counter — main reads and prints it
   This is EXACTLY how FreeRTOS scheduler heartbeat works
   ──────────────────────────────────────────────────── */

/* ── SysTick registers — memory mapped addresses ────
   These addresses are fixed on every ARM Cortex-M chip
   Same addresses at Infineon, Marvell, everywhere      */
#define SYSTICK_CTRL  (*(volatile uint32_t *)0xE000E010)
#define SYSTICK_LOAD  (*(volatile uint32_t *)0xE000E014)
#define SYSTICK_VAL   (*(volatile uint32_t *)0xE000E018)

/* ── Semihosting — prints to your terminal via QEMU ─ */
#define SYS_WRITE0 0x04
static void print(const char *s) {
    __asm volatile (
        "mov r0, %[op]\n"
        "mov r1, %[arg]\n"
        "bkpt 0xAB\n"
        :
        : [op] "r" (SYS_WRITE0), [arg] "r" (s)
        : "r0", "r1", "memory"
    );
}

/* ── Simple number to string ─────────────────────── */
static void print_number(uint32_t n) {
    char buf[12];
    int i = 10;
    buf[11] = '\0';
    buf[10] = '\n';
    if (n == 0) {
        print("0\n");
        return;
    }
    while (n > 0 && i >= 0) {
        buf[i--] = '0' + (n % 10);
        n /= 10;
    }
    print(&buf[i + 1]);
}

/* ── tick_count — shared between ISR and main ───────
   MUST be volatile — ISR modifies it, main reads it  */
volatile uint32_t tick_count = 0;

/* ── SysTick_Handler — runs every time SysTick fires
   ISR rules:
   1. Keep it short — just increment counter
   2. No blocking — never wait inside ISR
   3. Shared vars must be volatile — tick_count is ✅  */
void SysTick_Handler(void) {
    tick_count++;
}

/* ── SysTick initialisation ─────────────────────────
   mps2-an385 clock = 25MHz
   Count 2,500,000 = fires ~10 times per second       */
void systick_init(void) {
    SYSTICK_LOAD = 2500000 - 1;  /* reload value      */
    SYSTICK_VAL  = 0;            /* reset counter      */
    SYSTICK_CTRL = 0x7;          /* enable + interrupt
                                    + use CPU clock    */
}

/* ── Linker script symbols ───────────────────────── */
extern uint32_t _stack_top;
int main(void);

/* ── Reset handler — CPU jumps here on boot ──────── */
void Reset_Handler(void) {
    systick_init();  /* start SysTick before main      */
    main();
    while (1) {}
}

/* ── Vector table ───────────────────────────────────
   Cortex-M3 needs entries 0-15 minimum
   Position 15 = SysTick_Handler                      */
__attribute__((section(".isr_vector")))
void (*const vector_table[])(void) = {
    (void (*)(void))&_stack_top,  /*  0: stack pointer */
    Reset_Handler,                 /*  1: reset         */
    0,                             /*  2: NMI           */
    0,                             /*  3: HardFault     */
    0,                             /*  4: MemManage     */
    0,                             /*  5: BusFault      */
    0,                             /*  6: UsageFault    */
    0,                             /*  7: reserved      */
    0,                             /*  8: reserved      */
    0,                             /*  9: reserved      */
    0,                             /* 10: reserved      */
    0,                             /* 11: SVCall        */
    0,                             /* 12: DebugMon      */
    0,                             /* 13: reserved      */
    0,                             /* 14: PendSV        */
    SysTick_Handler,               /* 15: SysTick ✅    */
};

/* ── Main ───────────────────────────────────────────
   Watches tick_count and prints every new tick        */
int main(void) {
    uint32_t last_tick = 0;

    print("SysTick Demo Started\n");
    print("Waiting for ticks...\n");

    while (1) {
        /* Has tick_count changed since last check?    */
        if (tick_count != last_tick) {
            last_tick = tick_count;
            print("Tick: ");
            print_number(last_tick);
        }
    }
    return 0;
}