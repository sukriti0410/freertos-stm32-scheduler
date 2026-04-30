#include <stdint.h>

/* ── What this file teaches:
   Endianness detection using union trick
   Byte swapping for MACsec protocol frames
   ──────────────────────────────────────────────── */

/* Union stores both members at SAME memory address
   Write as uint32_t, read as individual bytes
   This reveals the byte order of your chip         */
typedef union {
    uint32_t value;
    uint8_t  bytes[4];
} EndianProbe_t;

uint8_t is_little_endian(void) {
    EndianProbe_t probe;
    probe.value = 0x01020304;
    /* ARM little-endian: bytes[0] = 0x04 (LSB first) */
    /* Big-endian:        bytes[0] = 0x01 (MSB first) */
    return (probe.bytes[0] == 0x04) ? 1 : 0;
}

/* Swap all 4 bytes — used before putting values
   into MACsec SecTAG fields (network byte order)   */
uint32_t swap32(uint32_t val) {
    return ((val >> 24) & 0x000000FFU) |
           ((val >>  8) & 0x0000FF00U) |
           ((val <<  8) & 0x00FF0000U) |
           ((val << 24) & 0xFF000000U);
}

/* Convert host byte order to network byte order
   Call this before writing PN into MACsec SecTAG   */
uint32_t host_to_network(uint32_t val) {
    if (is_little_endian()) {
        return swap32(val); /* ARM is LE, network is BE */
    }
    return val; /* already big-endian, no swap needed  */
}

int main(void) {
    /* Packet number 1 going into MACsec SecTAG      */
    uint32_t pn = 1;

    /* ARM stores as: 01 00 00 00 (little-endian)    */
    /* Network needs: 00 00 00 01 (big-endian)        */
    uint32_t pn_network = host_to_network(pn);

    /* Convert back after receiving from network     */
    uint32_t pn_host = host_to_network(pn_network);

    uint8_t le = is_little_endian(); /* = 1 on ARM   */

    (void)pn_network;
    (void)pn_host;
    (void)le;

    while (1) {}
    return 0;
}