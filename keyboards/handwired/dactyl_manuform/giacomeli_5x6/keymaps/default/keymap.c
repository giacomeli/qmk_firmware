#include QMK_KEYBOARD_H
#include "analog.h"
#include "print.h"
#include "quantum.h"

#define PIN_TRACKBALL_RED_LED GP22

#define _QWERTY 0
#define _RAISE 1


#define PIN_XP GP15
#define PIN_XN GP16
#define PIN_YP GP17
#define PIN_YN GP18


#define RAISE TG(_RAISE)


#define OLED_DISPLAY_128X64

#define OLED_DC_PIN GP23
#define OLED_CS_PIN GP24
#define OLED_RESET_PIN GP25
#define OLED_DISPLAY_PIN GP26



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
    setPinOutput(PIN_TRACKBALL_RED_LED);
    writePinLow(PIN_TRACKBALL_RED_LED);
}


#ifdef OLED_ENABLE
static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}

bool oled_task_user(void) {
    render_logo();
    return false;
}
#endif
void matrix_scan_user(void) {
    static bool debug_once = false;
    bool curr_xp = !readPin(PIN_XP);
    bool curr_xn = !readPin(PIN_XN);
    bool curr_yp = !readPin(PIN_YP);
    bool curr_yn = !readPin(PIN_YN);

    uint8_t layer = get_highest_layer(layer_state | default_layer_state);

    if (layer == _RAISE) {
        writePinHigh(PIN_TRACKBALL_RED_LED);
    } else {
        writePinLow(PIN_TRACKBALL_RED_LED);
    }

    if (curr_xp && !last_xp) {
        tap_code(QK_MOUSE_CURSOR_RIGHT);
    }
    if (curr_xn && !last_xn) {
        tap_code(QK_MOUSE_CURSOR_LEFT);
    }
    if (curr_yp && !last_yp) {
        if (layer == _RAISE) {
            tap_code(KC_MS_WH_UP);  // scroll up
        } else {
            tap_code(QK_MOUSE_CURSOR_UP);
        }
    }
    if (curr_yn && !last_yn) {
        if (layer == _RAISE) {
            tap_code(KC_MS_WH_DOWN);  // scroll down
        } else {
            tap_code(QK_MOUSE_CURSOR_DOWN);
        }
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
        RAISE, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,       KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,  KC_ACL2,

                            KC_TAB, KC_BSPC, KC_LGUI,         KC_DEL, KC_SPACE,
                                    KC_LCTL, KC_LALT,         KC_ENT, KC_BTN1, KC_BTN2
    ),
    [_RAISE] = LAYOUT_5x6(
        KC_GRV,  _______, _______, _______, _______, _______,         KC_BRMD,  KC_BRMU,  _______, KC_KB_MUTE,  KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP,
        KC_ESC,  _______, _______, _______, _______, _______,         KC_Y,     KC_U,     KC_I,    KC_O,    KC_P,     KC_RBRC,
        KC_LSFT, _______, _______, _______, _______, _______,         KC_LEFT,  KC_DOWN,  KC_UP,   KC_RIGHT, _______,  KC_PIPE,
        RAISE, _______, _______, _______, _______, _______,         _______,  KC_HOME,  KC_END,  _______,   KC_BSLS,  KC_ACL0,

                                    KC_TAB, KC_DEL,  KC_LGUI,         KC_DEL, KC_SPACE,
                                            KC_LCTL, KC_LALT,         KC_ENT, MS_WHLU, MS_WHLD
    )
};
