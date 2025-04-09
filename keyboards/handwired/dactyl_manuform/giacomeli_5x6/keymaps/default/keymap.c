#include QMK_KEYBOARD_H
#include "analog.h"
#include "print.h"
#include "quantum.h"

#define PIN_TRACKBALL_BLUE_LED GP22

#define _QWERTY 0
#define _RAISE 1


#define PIN_XP GP15
#define PIN_XN GP16
#define PIN_YP GP17
#define PIN_YN GP18


#define RAISE MO(_RAISE)



static bool last_xp = false;
static bool last_xn = false;
static bool last_yp = false;
static bool last_yn = false;

void matrix_init_kb(void) {

    // Initialize the matrix
    matrix_init_user();
    // Set the pin modes for the trackball
    setPinInput(PIN_XP);
    setPinInput(PIN_XN);
    setPinInput(PIN_YP);
    setPinInput(PIN_YN);

    // Set the pin modes for the blue LED
    setPinOutput(PIN_TRACKBALL_BLUE_LED);
    writePinHigh(PIN_TRACKBALL_BLUE_LED);
}

void matrix_scan_user(void) {
    static bool debug_once = false;
    bool curr_xp = !readPin(PIN_XP);
    bool curr_xn = !readPin(PIN_XN);
    bool curr_yp = !readPin(PIN_YP);
    bool curr_yn = !readPin(PIN_YN);


    if (curr_xp && !last_xp) {
        tap_code(QK_MOUSE_CURSOR_RIGHT);
    }
    if (curr_xn && !last_xn) {
        tap_code(QK_MOUSE_CURSOR_LEFT);
    }
    if (curr_yp && !last_yp) {
        tap_code(QK_MOUSE_CURSOR_UP);
    }
    if (curr_yn && !last_yn) {
        tap_code(QK_MOUSE_CURSOR_DOWN);
    }

    last_xp = curr_xp;
    last_xn = curr_xn;
    last_yp = curr_yp;
    last_yn = curr_yn;


    //log pin state for debugging GP15, GP16, GP17 and GP18
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
        KC_LCTL, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,       KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,  KC_ACL2,

                            KC_TAB, KC_BSPC, KC_LGUI,         KC_DEL, KC_SPACE,
                                    KC_LALT,   RAISE,         KC_ENT, KC_BTN1, KC_BTN2
    ),
    [_RAISE] = LAYOUT_5x6(
        KC_GRV,  _______, _______, _______, _______, _______,         KC_BRMD,  KC_BRMU,  _______, KC_KB_MUTE,  KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP,
        KC_ESC,  _______, _______, _______, _______, _______,         KC_Y,     KC_U,     KC_I,    KC_O,    KC_P,     KC_RBRC,
        KC_LSFT, _______, _______, _______, _______, _______,         KC_LEFT,  KC_DOWN,  KC_UP,   KC_RIGHT, _______,  KC_PIPE,
        KC_LCTL, _______, _______, _______, _______, _______,         _______,  KC_HOME,  KC_END,  _______,   KC_BSLS,  RAISE,

                                    KC_TAB, KC_DEL,  KC_LGUI,         KC_DEL, KC_SPACE,
                                            KC_LALT,   RAISE,         KC_ENT, MS_WHLU, MS_WHLD
    )
};
