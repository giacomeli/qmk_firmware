#include "quantum.h"
#include "pointing_device.h"

#define PIN_XP GP15
#define PIN_XN GP16
#define PIN_YP GP17
#define PIN_YN GP18

#define BASE_MOVE_PIXELS      7
#define EXPONENTIAL_BOUND     10
#define EXPONENTIAL_BASE      1.2
#define MOVE_DEBOUNCE_DELTA   3

static uint8_t last_state_xp = 1;
static uint8_t last_state_xn = 1;
static uint8_t last_state_yp = 1;
static uint8_t last_state_yn = 1;

static uint16_t last_time = 0;

void matrix_init_kb(void) {
    setPinInputHigh(PIN_XP);
    setPinInputHigh(PIN_XN);
    setPinInputHigh(PIN_YP);
    setPinInputHigh(PIN_YN);
    matrix_init_user();
}

void matrix_scan_kb(void) {
    report_mouse_t mouse_report = pointing_device_get_report();

    uint8_t state_xp = readPin(PIN_XP);
    uint8_t state_xn = readPin(PIN_XN);
    uint8_t state_yp = readPin(PIN_YP);
    uint8_t state_yn = readPin(PIN_YN);

    int8_t vec_x = 0;
    int8_t vec_y = 0;
    bool moved = false;

    if (state_xp != last_state_xp) {
        last_state_xp = state_xp;
        vec_x += 1;
        moved = true;
    }
    if (state_xn != last_state_xn) {
        last_state_xn = state_xn;
        vec_x -= 1;
        moved = true;
    }
    if (state_yp != last_state_yp) {
        last_state_yp = state_yp;
        vec_y -= 1;
        moved = true;
    }
    if (state_yn != last_state_yn) {
        last_state_yn = state_yn;
        vec_y += 1;
        moved = true;
    }

    if (moved && (vec_x != 0 || vec_y != 0)) {
        uint16_t now = timer_read();
        uint16_t delta = now - last_time;

        if (delta >= MOVE_DEBOUNCE_DELTA) {
            last_time = now;

            int exp = EXPONENTIAL_BOUND - delta;
            if (exp < 1) exp = 1;

            double factor = 1.0;
            for (int i = 0; i < exp; i++) {
                factor *= EXPONENTIAL_BASE;
            }

            int movement = (int)(BASE_MOVE_PIXELS * factor);

            mouse_report.x += vec_x * movement;
            mouse_report.y += vec_y * movement;

            pointing_device_set_report(mouse_report);
            pointing_device_send();
        }
    }

    matrix_scan_user();
}
