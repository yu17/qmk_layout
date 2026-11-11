# yu17 - Drop Planck rev6

Modified from the default Planck layout. Each layer is drawn in the comments of
`keymap.c`.

Layers: Base, Lower (green), Raise (purple, numpad), Fn (lemon, hold Fn),
Extra (orange, Lower + Raise), Mouse (red, Extra -> Mouse, leave with the
top-right key), Settings/KbdCfg (white, Lower + Raise + Fn), Plover (magenta).
A visual map of every layer: `make docs` -> `docs/layout.html`.

* **Raise = numpad** on the right (keypad codes, Num Lock key; Num Lock is
  switched on automatically when Raise turns on), arrows in the middle
  (inverted T on H / B N M), left half = normal keys so WASD games work.
* **Lock a layer:** Fn + Lower / Fn + Raise. Fn + Space (or the same lock
  combo) releases it. No idle timeout.
* **Shift + Backspace = Delete** (key override); Del is no longer on Lower.
* Arrows are Left, Down, Up, Right (PgDn/PgUp on Lower and the mouse arrows
  follow the same order).

## Underglow (9 hand-soldered WS2812 LEDs)

* **Power-on self test:** the strip blinks a rainbow 3x when plugged in, with
  each LED a different colour. Also on Extra + `Z` (`LED_TST`; Extra = hold
  Lower + Raise).
  * Nothing lights: check 5 V/GND and the data line from pin A1.
  * The first N LEDs light and the rest stay dark: the LED after the last
    lit one (or its data in/out joint) is bad. WS2812s pass data down a chain.
* **Layer colours** work even when the base glow is switched off.
* All lighting is on **Extra**: `A` = base glow on/off, then `+` on the top
  row and `-` below it for effect (`S`/`X`), hue (`D`/`C`), saturation
  (`F`/`V`), brightness (`G`/`B`), speed (`H`/`N`). Saved to EEPROM.
* **Settings** (Lower + Raise + Fn) is configuration only: reboot, debug,
  audio/music, Alt/Win swap, NKRO, Win-key lock, Plover, bootloader and
  `EE_CLR` (bottom-right) to reset all saved settings.

Before this rework the 2023 QMK build drove the LEDs at 80 kHz instead of the
WS2812's 800 kHz (fixed upstream in Oct 2024). Also, `LIGHT_OFF` saved "off" to
EEPROM, which also hid the layer colours. Either can look like dead hardware.
