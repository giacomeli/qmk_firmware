#include QMK_KEYBOARD_H
#include "analog.h"
#include "print.h"

#define _QWERTY 0
#define _RAISE 1

#define RAISE MO(_RAISE)

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

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (is_keyboard_left()) {
        mouse_report.x = 0;
        mouse_report.y = 0;
        return mouse_report;
    }

    mouse_report.x = -mouse_report.x;
    return mouse_report;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x6(
        KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,       KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS,  KC_EQL,
        KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,       KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,     KC_LBRC,
        KC_LSFT, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,       KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
        KC_LCTL, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,       KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,  RAISE,

                            KC_TAB, KC_BSPC, KC_LGUI,         KC_DEL, KC_SPACE,
                                    KC_LALT,   RAISE,         KC_ENT, KC_BTN1, KC_BTN2
    ),
    [_RAISE] = LAYOUT_5x6(
        KC_GRV,  _______, _______, _______, _______, _______,         KC_BRMD,  KC_BRMU,  _______,    KC_KB_MUTE,  KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP,
        KC_ESC,  _______, _______, _______, _______, _______,         KC_Y,     KC_U,     KC_I,    KC_O,     KC_P,     KC_RBRC,
        KC_LSFT, _______, _______, _______, _______, _______,         KC_LEFT,  KC_DOWN,  KC_UP,   KC_RIGHT, _______,  KC_PIPE,
        KC_LCTL, _______, _______, _______, _______, _______,         _______,  KC_HOME,  KC_END,  _______,   KC_BSLS,  RAISE,

                                    KC_TAB, KC_DEL,  KC_LGUI,         KC_DEL, KC_SPACE,
                                            KC_LALT,   RAISE,         KC_ENT, MS_WHLU, MS_WHLD
    )
};
