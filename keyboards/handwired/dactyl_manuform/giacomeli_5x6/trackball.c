#include "quantum.h"
#include "pointing_device.h"

#define PIN_XP GP15
#define PIN_XN GP16
#define PIN_YP GP17
#define PIN_YN GP18

#define TRACKBALL_MOVE_DELTA    1
#define TRACKBALL_MAX_SPEED     2
#define TRACKBALL_TIME_TO_MAX   20
#define TRACKBALL_DEBOUNCE      5

static uint16_t last_time = 0;
static uint8_t repeat_counter = 0;

void matrix_init_kb(void) {
    setPinInputHigh(PIN_XP);
    setPinInputHigh(PIN_XN);
    setPinInputHigh(PIN_YP);
    setPinInputHigh(PIN_YN);
    matrix_init_user();
}

static int calculate_speed(void) {
    if (repeat_counter == 0) {
        return TRACKBALL_MOVE_DELTA;
    } else if (repeat_counter >= TRACKBALL_TIME_TO_MAX) {
        return TRACKBALL_MOVE_DELTA * TRACKBALL_MAX_SPEED;
    } else {
        return (TRACKBALL_MOVE_DELTA * TRACKBALL_MAX_SPEED * repeat_counter) / TRACKBALL_TIME_TO_MAX;
    }
}

void matrix_scan_kb(void) {
    uint16_t now = timer_read();
    uint16_t delta = now - last_time;

    if (delta < TRACKBALL_DEBOUNCE) {
        matrix_scan_user();
        return;
    }

    last_time = now;

    report_mouse_t mouse_report = pointing_device_get_report();

    int8_t vec_x = 0;
    int8_t vec_y = 0;

    if (!readPin(PIN_XP)) vec_x += 1;
    if (!readPin(PIN_XN)) vec_x -= 1;
    if (!readPin(PIN_YP)) vec_y -= 1;
    if (!readPin(PIN_YN)) vec_y += 1;

    if (vec_x != 0 || vec_y != 0) {
        if (repeat_counter < UINT8_MAX) {
            repeat_counter++;
        }

        int move = calculate_speed();

        // ajustar diagonais
        if (vec_x != 0 && vec_y != 0) {
            vec_x = (vec_x * 181 + 128) / 256;
            vec_y = (vec_y * 181 + 128) / 256;
        }

        mouse_report.x += vec_x * move;
        mouse_report.y += vec_y * move;

        pointing_device_set_report(mouse_report);
        pointing_device_send();
    } else {
        repeat_counter = 0;
    }

    matrix_scan_user();
}
