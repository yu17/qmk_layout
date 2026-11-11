# yu17 - Drop Planck rev6

Target `planck/rev6_drop` (the Drop-made rev6, USB `03a8:a4f9`). Started from
the default Planck keymap. The layout is shared with the Geonix and described
in the top-level README; every layer is also drawn in the comments of
`keymap.c`, and `make docs` renders all of them into `docs/layout.html`.

## Planck-only features

* **Audio:** start-up song; audio on/off, voice -/+ and music mode settings on
  the Settings layer (Lower + Raise + Fn).
* **Music mode** toggle: Extra (Lower + Raise) + Left arrow. While it is on,
  keys play notes, except Fn, Lower, Raise and the Extra/Settings keys, so the
  toggle can always be reached to switch it off again.
* **Plover** steno layer: Settings + `;`; Exit is the bottom-left key. Turning
  Plover on also enables NKRO (saved), which Plover needs.
* **DIP switches** (if fitted): switch 0 holds the Settings layer on,
  switch 1 turns on "muse" mode (generative music; the encoder changes tempo,
  or pitch while Raise is held).
* **Encoder** (if fitted): scroll wheel down/up.
* **Num Lock** is switched on automatically whenever Raise turns on
  (Raise is a keypad).
* **Shift + Backspace = Delete** is a QMK key override (`key_overrides[]`).
* **Layer lock** keys (Fn + Lower / Fn + Raise) are custom keycodes that call
  `layer_lock_invert()`; QMK's Layer Lock idle timeout is not used (it would
  apply to every locked layer, including a locked keypad in a game).

## Underglow (9 hand-soldered WS2812 LEDs)

### What you see

* **Power-on self test:** the strip blinks a rainbow 3x when plugged in, each
  LED a different colour. Also on Extra + `Z` (`LED_TST`).
* **Base layer:** the normal RGBLight effect (default: static light cyan).
* **Other layers:** the whole strip in the layer's colour: green Lower,
  purple Raise, lemon Fn, orange Extra, red Mouse, white Settings, magenta
  Plover. These show even when the base glow is switched off.
* **Extra** has all lighting controls: `A` = base glow on/off, then `+` on the
  top row and `-` below it for brightness (`S`/`X`), speed (`D`/`C`), effect
  (`F`/`V`), hue (`G`/`B`), saturation (`H`/`N`). Saved to EEPROM.
* Lights turn off while the computer is asleep.

### How it works

* Built on QMK **RGBLight layers** (`RGBLIGHT_LAYERS`). `keymap.c` defines one
  RGBLight layer per keymap layer above Base (`underglow_layers[]`), each a
  single segment covering all 9 LEDs, plus a rainbow test layer.
  `layer_state_set_user()` switches each RGBLight layer on when its keymap
  layer is active. Later entries are drawn on top, so the highest active
  layer wins. Base has no entry, so the normal effect shows through.
* `RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF` draws the layer colours even while the
  base glow is disabled. Before the rework, "lights off" (saved in EEPROM)
  also hid every layer colour, which looked like dead LEDs.
* The test pattern is a blinking RGBLight layer (`RGBLIGHT_LAYER_BLINK`),
  blinked 3x from `keyboard_post_init_user()` and by `LED_TST`.
* Layer colours are fixed HSV values in `keymap.c`; the base effect's
  defaults and steps are in `config.h`.

### Status and troubleshooting

* Before this rework the 2023 QMK build drove the LEDs at 80 kHz instead of
  the WS2812's 800 kHz (fixed upstream in Oct 2024). That, and the saved
  "off" state, are fixed in firmware.
* **As of 2026-11-11 the LEDs are still dark with this firmware**, including
  the power-on test, so the remaining cause is most likely hardware:
  * Nothing lights: check 5 V and GND at the LED pads, and the data line from
    the MCU pin A1 to the first LED's data-in.
  * The first N LEDs light and the rest stay dark: the LED after the last lit
    one (or its data-in/out joint) is bad. WS2812s pass data down a chain.
* If the lights act strangely after flashing, reset all saved settings with
  Settings + bottom-right (`EE_CLR`).
