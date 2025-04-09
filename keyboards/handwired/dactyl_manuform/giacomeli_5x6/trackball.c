#include "quantum.h"
#include "pointing_device.h"

#define PIN_XP GP15
#define PIN_XN GP16
#define PIN_YP GP17
#define PIN_YN GP18

#define BASE_MOVE_PIXELS      10
#define EXPONENTIAL_BOUND     10
#define EXPONENTIAL_BASE      1.1

static uint8_t last_state_xp = 1;
static uint8_t last_state_xn = 1;
static uint8_t last_state_yp = 1;
static uint8_t last_state_yn = 1;

static uint16_t last_time_xp = 0;
static uint16_t last_time_xn = 0;
static uint16_t last_time_yp = 0;
static uint16_t last_time_yn = 0;

static int calculate_movement(uint8_t curr_state, uint8_t *last_state, uint16_t *last_time, bool positive) {
    if (curr_state != *last_state) {
        uint16_t now = timer_read();
        uint16_t delta = now - *last_time;
        *last_time = now;
        *last_state = curr_state;

        int exp = EXPONENTIAL_BOUND - delta;
        if (exp < 1) exp = 1;

        double factor = 1.0;
        for (int i = 0; i < exp; i++) {
            factor *= EXPONENTIAL_BASE;
        }

        int movement = (int)(BASE_MOVE_PIXELS * factor);
        return positive ? movement : -movement;
    }
    return 0;
}

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

    int move_x = 0;
    int move_y = 0;

    move_x += calculate_movement(state_xp, &last_state_xp, &last_time_xp, true);
    move_x += calculate_movement(state_xn, &last_state_xn, &last_time_xn, false);
    move_y += calculate_movement(state_yp, &last_state_yp, &last_time_yp, false);
    move_y += calculate_movement(state_yn, &last_state_yn, &last_time_yn, true);

    if (move_x != 0 || move_y != 0) {
        mouse_report.x += move_x;
        mouse_report.y += move_y;
        pointing_device_set_report(mouse_report);
        pointing_device_send();
    }

    matrix_scan_user();
}
