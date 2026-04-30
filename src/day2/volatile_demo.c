#include <stdint.h>

/* ── What this file teaches:
   1. volatile — forces compiler to read from memory every time
   2. Bit macros — SET CLEAR TOGGLE READ one bit at a time
   ──────────────────────────────────────────────────────── */

/* This simulates a hardware status register
   volatile = hardware can change this anytime
   without volatile, compiler may cache the value
   and never see the hardware update              */
volatile uint32_t hw_status = 0;

/* ── WITHOUT volatile — BROKEN ──────────────────
   Compiler may optimise the while loop away
   It assumes: "I wrote 0, it must still be 0"   */
void bad_wait(void) {
    uint32_t *reg = (uint32_t *)&hw_status;
    *reg = 0;
    while (*reg == 0) {
        /* compiler may delete this loop entirely */
    }
}

/* ── WITH volatile — CORRECT ────────────────────
   Compiler MUST read from memory every iteration */
void good_wait(void) {
    volatile uint32_t *reg = (volatile uint32_t *)&hw_status;
    *reg = 0;
    while (*reg == 0) {
        /* correctly checks memory every iteration */
    }
}

/* ── Bit macros ──────────────────────────────────
   Touch ONE bit at a time
   Leave ALL other bits unchanged                 */
#define SET_BIT(reg, bit)    ((reg) |=  (1U << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1U << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^=  (1U << (bit)))
#define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1U)

int main(void) {
    uint32_t control = 0;        /* 0b00000000 */

    SET_BIT(control, 0);         /* 0b00000001 — bit 0 ON  */
    SET_BIT(control, 3);         /* 0b00001001 — bit 3 ON  */
    CLEAR_BIT(control, 0);       /* 0b00001000 — bit 0 OFF */
    TOGGLE_BIT(control, 3);      /* 0b00000000 — bit 3 OFF */

    uint8_t is_on = READ_BIT(control, 3); /* returns 0 */
    (void)is_on;

    while (1) {}
    return 0;
}