#include <stdint.h>

/* ── What this file teaches:
   Function pointers + CAN dispatch table
   This is the exact pattern used in automotive ECUs
   ──────────────────────────────────────────────── */

/* Step 1 — Define what a handler function looks like
   Every handler takes a data pointer and returns nothing */
typedef void (*can_handler_t)(uint8_t *data);

/* Step 2 — Write the actual handler functions */
void handle_sensor(uint8_t *data) {
    /* process sensor reading */
    (void)data;
}

void handle_motor(uint8_t *data) {
    /* control motor speed */
    (void)data;
}

void handle_safety(uint8_t *data) {
    /* trigger safety response */
    (void)data;
}

void handle_brake(uint8_t *data) {
    /* apply brakes */
    (void)data;
}

/* Step 3 — Build the dispatch table
   Index = CAN message ID
   Value = function to call for that ID            */
can_handler_t dispatch_table[4] = {
    handle_sensor,   /* ID 0 → sensor data    */
    handle_motor,    /* ID 1 → motor command  */
    handle_safety,   /* ID 2 → safety signal  */
    handle_brake,    /* ID 3 → brake command  */
};

/* Step 4 — Use it
   One line handles ANY message ID
   No if-else needed                               */
void process_can_message(uint8_t msg_id, uint8_t *data) {
    if (msg_id < 4) {
        dispatch_table[msg_id](data); /* call handler */
    }
}

int main(void) {
    uint8_t dummy_data[8] = {0};

    /* Simulate 4 CAN messages arriving */
    process_can_message(0, dummy_data); /* calls handle_sensor  */
    process_can_message(1, dummy_data); /* calls handle_motor   */
    process_can_message(2, dummy_data); /* calls handle_safety  */
    process_can_message(3, dummy_data); /* calls handle_brake   */

    while (1) {}
    return 0;
}