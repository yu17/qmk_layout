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
  LED a different colour. Also on Settings + `L` (`LED_TST`).
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
  the power-on test, so the remaining cause is most likely hardware.
* **2026-11-14 hardware check (`yu17_diag`):** LED driver running; A1
  `pullup=0 pulldown=0 drivehigh=1 drivelow=0`; DIP/encoder pads fine;
  RGB enabled, val 255.
* **Bare-board measurements (2026-11-14):** 5 V on every LED, also while
  dark; DIN to GND 11.2 kOhm on *all* LEDs (so the low A1 reading above is
  just the LEDs' normal input load, not a leak); A1 held high gives 3.25 V at
  LED 1's DIN (trace intact, no level shifter). Lights sometimes work on a
  bare board after a rest; failures show LED 1 alone in green or yellow (it
  decodes only the first one or two of its three colour bytes) or all dark.
* **Most likely cause:** the data signal is out of spec. The WS2812B needs
  a high level of at least 0.7 x VDD (3.5 V at 5 V) and the STM32 drives
  3.3 V directly, so only LED 1, the one LED that sees the 3.3 V signal, sits
  at its threshold (LEDs 2-9 get a regenerated 5 V signal from LED 1).
  Temperature and supply voltage move it either side. Test: remove LED 1 and
  bridge its DIN to DOUT; if LED 2 now fails the same way, it is the signal,
  not the LED. Fix: power LED 1 through a diode (about 4.3 V, threshold
  about 3 V) or add a 5 V buffer such as a 74AHCT1G125 between A1 and DIN.
* General hints:
  * Nothing lights: check 5 V and GND at the LED pads, and the data line from
    the MCU pin A1 to the first LED's data-in.
  * The first N LEDs light and the rest stay dark: the LED after the last lit
    one (or its data-in/out joint) is bad. WS2812s pass data down a chain.
* If the lights act strangely after flashing, reset all saved settings with
  Settings + bottom-right (`EE_CLR`).

### Hardware check build (`yu17_diag`)

`make flash-planck-diag` flashes the same layout with one change: the LED
test key (Settings + `L`) **types** a report into whatever has focus (open a
text editor first). Each press moves to the next step: (1) report, then the
data pin A1 is held at 3.3 V; (2) A1 held at 0 V; (3) back to normal. Go back
to the normal firmware with `make flash-planck`.

| Report line | Healthy | Otherwise |
|---|---|---|
| `DRIVER pinmode=2 af=1 timer=running dma=running` | the MCU is sending LED data on A1 | a firmware/driver problem, not hardware |
| `A1 pullup=1 pulldown=0 drivehigh=1 drivelow=0` | no short on the data line, MCU pin can drive it | `pullup=0`: line pulled to GND (short, damaged LED input, or LEDs without 5 V); `pulldown=1`: pulled to a supply; `drivehigh=0`/`drivelow=1`: hard short or damaged MCU pin |
| `PADS dip ... enc ...` | all `1` when no DIP switch/encoder is fitted | a `0` on an empty pad = short/leak on that pin |
| `RGB enabled=1 ... val=...` | val > 0 | val 0 = brightness at zero |

A healthy `A1` line cannot tell "LED 1 is dead" from "the trace is open",
so use a multimeter:

1. **5 V at every LED**: VDD to GND on each LED (or its pads) ~5 V. Missing
   at all of them: the supply trace or USB 5 V; missing at some: a joint or a
   break between those.
2. **Data at LED 1** (per QMK's LED map, seen from above: the first in the
   chain is near the centre of the board, slightly below the middle; the
   chain then runs to the lower right, up the right edge, along the top to
   the left, down the left edge and back towards the bottom centre): step 1
   of the check holds A1 high, so its DIN pad should read ~3.3 V to GND, and
   ~0 V in step 2. If A1 reads right at the MCU side but not at DIN: broken
   trace/joint. If DIN follows correctly but nothing lights: LED 1 is dead
   (most likely victim of an electrostatic discharge; it is the first to
   see any spike on the data line). Replace it, or bridge its DIN to DOUT
   pad to test the rest of the chain.
3. **No 5 V or only partial lighting**: check the LED's orientation and
   reflow its joints.
