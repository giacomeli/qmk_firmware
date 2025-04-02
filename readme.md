# QMK Firmware – Custom Dactyl Manuform 5x6 (giacomeli)

This repository contains a custom [QMK Firmware](https://qmk.fm) setup for a handwired **Dactyl Manuform 5x6 split keyboard** using **RP2040 microcontrollers**.

Features:
- 5x6 split layout
- RP2040 (e.g., Raspberry Pi Pico)
- **Serial split communication** via `USART` using **PIO0** (GP0 and GP1)
- **Joystick** connected to the **right half** (GP26 and GP27)
- **WS2812 RGB LED strip** connected to GP29 using **PIO1**
- Split logic using `EE_HANDS` (no hardcoded master)

---

## 🧰 Requirements

Before continuing, ensure you have the following installed:

- Python 3
- QMK CLI (`pip install qmk`)
- `arm-none-eabi-gcc` (via Homebrew, APT, or other)
- `make` and standard development tools
- A working `qmk_firmware` folder with submodules initialized

---

## 📦 Step 1: Clone this repository

```bash
git clone --recurse-submodules git@github.com:giacomeli/qmk_firmware.git
cd qmk_firmware

If you forgot --recurse-submodules, run:

git submodule update --init --recursive



⸻

📁 Step 2: Locate your custom keyboard folder

Your custom layout is located at:

keyboards/handwired/dactyl_manuform/giacomeli_5x6/



⸻

⚙️ Step 3: EEPROM-based split configuration

This keyboard uses EE_HANDS to determine whether a half is the left or right side.
You do not need to define MASTER_LEFT or MASTER_RIGHT.

Inside config.h:

#define EE_HANDS

You will flash a persistent setting into each half’s EEPROM to define its role.

⸻

💻 Step 4: Compile firmware for both halves

You must compile firmware for each side separately.

Left half:

qmk compile -kb handwired/dactyl_manuform/giacomeli_5x6 -km default -bl uf2-split-left

Right half:

qmk compile -kb handwired/dactyl_manuform/giacomeli_5x6 -km default -bl uf2-split-right

This produces .uf2 files which you can drag into your RP2040 device after entering bootloader mode.

⸻

🔁 Step 5: Flash and set EEPROM hand info

This step configures each half so that the firmware knows whether it is the left or right.

Flash left half and set eeconfig_hands=left

qmk flash -kb handwired/dactyl_manuform/giacomeli_5x6 -km default -bl uf2-split-left -e "eeconfig_hands=left"

Flash right half and set eeconfig_hands=right

qmk flash -kb handwired/dactyl_manuform/giacomeli_5x6 -km default -bl uf2-split-right -e "eeconfig_hands=right"



⸻

🧠 How the split communication works

This keyboard uses serial communication between halves via PIO0:
	•	TX (GP0) → connect to RX (GP1) on the other half
	•	RX (GP1) ← connect from TX (GP0) of the other half
	•	GND must be connected between both boards
	•	VCC can be shared if only one side is powered by USB

In config.h:

#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_DRIVER SIOD0
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_PIO_USE_PIO0



⸻

🎮 Joystick setup (only on right half)
	•	X-axis on GP26
	•	Y-axis on GP27

The firmware dynamically checks which side is running, and only enables joystick input on the right half using is_keyboard_right().

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (!is_keyboard_right()) {
        mouse_report.x = 0;
        mouse_report.y = 0;
        return mouse_report;
    }

    // invert X-axis
    mouse_report.x = -mouse_report.x;
    return mouse_report;
}



⸻

🌈 RGB LED configuration (WS2812)

WS2812 (NeoPixel) RGB LEDs are connected to GP29, using PIO1 to avoid conflicts with serial transport.

In config.h:

#define RGB_DI_PIN GP29
#define WS2812_PIO_USE_PIO1
#define RGBLIGHT_LED_COUNT 12  // Change to your number of LEDs
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_SPLIT

In rules.mk:

RGBLIGHT_ENABLE = yes
WS2812_DRIVER = vendor



⸻

🧪 Debugging

You can use qmk console to monitor output and ensure the split is working:

qmk console

Look for messages like:

handedness: left

Or joystick movement, keypress events, etc.

⸻

🧼 Optional: clean the build

qmk clean



⸻

📄 License

This project is based on QMK Firmware, licensed under the MIT License.
