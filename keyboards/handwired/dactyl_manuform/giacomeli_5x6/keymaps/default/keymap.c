#include QMK_KEYBOARD_H

#include "print.h"

void matrix_scan_user(void) {
    static bool debug_once = false;

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (matrix_is_on(row, col)) {
                if (!debug_once) {
                    uprintf("Key pressed at row: %d, col: %d\n", row, col);
                    debug_once = true;
                }
            }
        }
    }
}


#include "analog.h"

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (is_keyboard_left()) {
        mouse_report.x = 0;
        mouse_report.y = 0;
        return mouse_report;
    }

    mouse_report.x = -mouse_report.x;
    return mouse_report;
}

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x6(
        // Row 0:  6 left + 6 right = 12
        KC_ESC, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,       KC_6,   KC_7,   KC_8,   KC_9,   KC_0,    KC_BSPC,
        // Row 1
        KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,       KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,    KC_MINS,
        // Row 2
        KC_LSFT,KC_A,   KC_S,   KC_D,   KC_F,   KC_G,       KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN, KC_QUOT,
        // Row 3
        KC_LCTL,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,       KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_BSLS,
        // Row 4 (a “última fileira” - 6 keys left, 6 keys right)
        KC_LBRC,KC_RBRC,RAISE,  KC_SPC, KC_TAB, KC_HOME,     KC_1,   KC_2,   KC_3,   KC_4,   KC_7,    KC_8
    ),

    [_LOWER] = LAYOUT_5x6(
        // Row 0:  6 left + 6 right = 12
        KC_ESC, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,       KC_6,   KC_7,   KC_8,   KC_9,   KC_0,    KC_BSPC,
        // Row 1
        KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,       KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,    KC_MINS,
        // Row 2
        KC_LSFT,KC_A,   KC_S,   KC_D,   KC_F,   KC_G,       KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN, KC_QUOT,
        // Row 3
        KC_LCTL,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,       KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_BSLS,
        // Row 4 (a “última fileira” - 6 keys left, 6 keys right)
        KC_LBRC,KC_RBRC,RAISE,  KC_SPC, KC_TAB, KC_HOME,    KC_1,   KC_2,   KC_3,   KC_4,   KC_7,    KC_8

    ),

    [_RAISE] = LAYOUT_5x6(
          // Row 0:  6 left + 6 right = 12
        KC_ESC, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,       KC_6,   KC_7,   KC_8,   KC_9,   KC_0,    KC_BSPC,
        // Row 1
        KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,       KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,    KC_MINS,
        // Row 2
        KC_LSFT,KC_A,   KC_S,   KC_D,   KC_F,   KC_G,       KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN, KC_QUOT,
        // Row 3
        KC_LCTL,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,       KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_BSLS,
        // Row 4 (a “última fileira” - 6 keys left, 6 keys right)
        KC_LBRC,KC_RBRC,RAISE,  KC_SPC, KC_TAB, KC_HOME,    KC_1,   KC_2,   KC_3,   KC_4,   KC_7,    KC_8
    )
};
