# yu17 - Drop Planck rev6

Modified from the default Planck layout. Each layer is drawn in the comments of
`keymap.c`.

Layers: Base, Lower (green), Raise (purple), Fn (lemon, hold FN/M),
Extra (orange, Lower + Raise), Mouse (red, Extra -> Mouse, leave with the
top-right key), KbdCfg (white, Lower + Raise + FN/M), Plover (magenta).

## Underglow (9 hand-soldered WS2812 LEDs)

* **Power-on self test:** the strip blinks a rainbow 3x when plugged in, with
  each LED a different colour. Also available on KbdCfg + `LED_TST` (Lower +
  Raise + FN/M, then the `,` key).
  * Nothing lights: check 5 V/GND and the data line from pin A1.
  * The first N LEDs light and the rest stay dark: the LED after the last
    lit one (or its data in/out joint) is bad. WS2812s pass data down a chain.
* **Layer colours** work even when the base glow is switched off.
* Base glow: Extra + `A` = off, Extra + `S` = on (saved). Effect, colour and
  brightness are on KbdCfg and are saved to EEPROM.
* If the lights act strangely after flashing, reset the settings with
  KbdCfg + bottom-right (`EE_CLR`).

Before this rework the 2023 QMK build drove the LEDs at 80 kHz instead of the
WS2812's 800 kHz (fixed upstream in Oct 2024). Also, `LIGHT_OFF` saved "off" to
EEPROM, which also hid the layer colours. Either can look like dead hardware.
