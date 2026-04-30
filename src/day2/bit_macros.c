#include <stdint.h>

/* ── What this file teaches:
   Professional bit manipulation library
   Used in every hardware driver you will ever write
   ──────────────────────────────────────────────── */

#define SET_BIT(REG, BIT)    ((REG) |=  (1UL << (BIT)))
#define CLEAR_BIT(REG, BIT)  ((REG) &= ~(1UL << (BIT)))
#define TOGGLE_BIT(REG, BIT) ((REG) ^=  (1UL << (BIT)))
#define READ_BIT(REG, BIT)   (((REG) >> (BIT)) & 1UL)

/* Multi-bit field — set a GROUP of bits together
   Example: word length field is bits [6:5] in UART  */
#define SET_FIELD(REG, MASK, POS, VAL) \
    ((REG) = ((REG) & ~(MASK)) | (((VAL) << (POS)) & (MASK)))

/* Simulated UART control register bit positions     */
#define UART_EN_BIT   0   /* bit 0 = UART enable     */
#define UART_TXE_BIT  8   /* bit 8 = TX enable       */
#define UART_RXE_BIT  9   /* bit 9 = RX enable       */

#define WLEN_MASK  0x00000060UL  /* bits [6:5]        */
#define WLEN_POS   5
#define WLEN_8BIT  3U            /* 0b11 = 8-bit word */

static uint32_t UART_CTL = 0;

void uart_setup(void) {
    /* Step 1: Disable UART before configuring       */
    CLEAR_BIT(UART_CTL, UART_EN_BIT);

    /* Step 2: Set 8-bit word length                 */
    SET_FIELD(UART_CTL, WLEN_MASK, WLEN_POS, WLEN_8BIT);

    /* Step 3: Enable TX and RX                      */
    SET_BIT(UART_CTL, UART_TXE_BIT);
    SET_BIT(UART_CTL, UART_RXE_BIT);

    /* Step 4: Enable UART last                      */
    SET_BIT(UART_CTL, UART_EN_BIT);

    /* Verify TX is on                               */
    uint8_t tx = READ_BIT(UART_CTL, UART_TXE_BIT);
    (void)tx; /* should be 1                         */
}

int main(void) {
    uart_setup();

    uint32_t reg = 0;
    SET_BIT(reg, 0);         /* 0b00000001 */
    SET_BIT(reg, 5);         /* 0b00100001 */
    CLEAR_BIT(reg, 0);       /* 0b00100000 */
    TOGGLE_BIT(reg, 5);      /* 0b00000000 */

    uint8_t b = READ_BIT(reg, 5); /* = 0   */
    (void)b;

    while (1) {}
    return 0;
}