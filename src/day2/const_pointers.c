#include <stdint.h>

/* ── What this file teaches:
   All 4 const + pointer combinations
   Read declarations RIGHT TO LEFT to understand them
   ──────────────────────────────────────────────── */

/* 1. Pointer to const — value locked, pointer can move
   Used when function promises: I will NOT modify your data */
void read_packet(const uint8_t *buffer, uint32_t len) {
    uint8_t first = buffer[0]; /* ✅ reading is fine   */
    /* buffer[0] = 5; */       /* ❌ ERROR — value locked */
    (void)first;
    (void)len;
}

/* 2. Const pointer — address locked, value can change
   Used for hardware registers — address never changes  */
uint32_t * const UART_DR = (uint32_t *)0x4000C000;
/* UART_DR = other; */ /* ❌ ERROR — pointer locked     */
/* *UART_DR = 'A';  */ /* ✅ writing data is fine       */

/* 3. Const pointer to const — both locked
   Used for ROM — fixed address, read only value        */
const uint32_t * const ROM_VER = (const uint32_t *)0x00000100;
/* ROM_VER = other; */ /* ❌ ERROR — pointer locked     */
/* *ROM_VER = 5;    */ /* ❌ ERROR — value locked       */

/* 4. No const — full freedom
   Used when you need to move pointer AND change value  */
void copy_buffer(uint8_t *dst, uint8_t *src, uint32_t len) {
    while (len--) {
        *dst++ = *src++; /* move pointer AND change value */
    }
}

int main(void) {
    uint8_t packet[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t output[4] = {0, 0, 0, 0};

    read_packet(packet, 4);
    copy_buffer(output, packet, 4);

    (void)ROM_VER;
    while (1) {}
    return 0;
}