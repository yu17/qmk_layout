# yu17 - Chosfox Geonix Rev.2.5 1U

The yu17 Planck layout ported key-for-key (both are 4x12 grids with the same
layer numbers; see the top-level README). Builds only inside the Chosfox
vial-qmk fork, because Bluetooth / 2.4 GHz / battery / ambient lighting live
in Chosfox's closed library (`lib/rdr_lib/librdrcommon.a`).

## Differences from the Planck

* No audio, music mode, DIP switches, encoder or Plover layer.
* **Windows layout only.** The vendor's Mac mode is cleared at every start.
* **Per-key colour hints** instead of a one-colour underglow (below).
* **Wireless** controls, battery, sleep timer and low-latency mode on
  Settings; the ambient (side/logo) strip's controls on Extra.
* **Num Lock** is switched on automatically when Raise turns on **only over
  USB** (see below); when wireless, use the Num Lock key on Raise.
* **Shift + Backspace = Delete** is a Vial key override in slot 0, written
  after every flash or factory reset; it can be edited or deleted in the
  Vial app.
* **NKRO** uses the vendor's 6-key / all-key toggle (one key), not QMK's
  NKRO on/off keys.

## Stock Fn functions -> this keymap

"Extra" = hold **Lower + Raise** (all lighting; `+` on the A row, `-`
below it; key lights: on/off, brightness, speed, effect, hue, saturation from
A to H; ambient: on/off, brightness, effect, hue from J to `;`). "Settings" = hold
**Lower + Raise + Fn** (wireless and keyboard configuration).

| Stock | Function | Here |
|---|---|---|
| Fn + Tab / Q / W / E / R | USB / BT1 / BT2 / BT3 / 2.4 GHz (hold = pair) | Settings + same keys |
| Fn + T | Next key-light effect | Extra + F (previous: Extra + V) |
| Fn + Backspace | All lights on/off | Extra + A |
| Fn + Up / Down | Brightness + / - | Extra + S / X |
| Fn + Left / Right | Effect speed - / + | Extra + C / D |
| Fn + Enter | Next ambient (side/logo) light effect | Extra + L (previous: Extra + `.`) |
| - | Ambient on/off, brightness +/-, hue +/- | Extra + J, K / `,`, `;` / `/` |
| Fn + Left Alt | Wireless sleep timer 1/3/10/30 min (T flashes red/green/blue/white) | Settings + X |
| Fn + Ctrl | Battery level | Settings + Z |
| Fn + Del (hold 3 s) | 2.4 GHz low-latency on/off (Del flashes red/green) | Settings + C (hold 3 s) |
| Fn + left Space (hold 3 s) | Factory reset | Settings + left Space (hold 3 s) |
| - | 6-key / all-key rollover (NKRO) | Settings + M |
| - | Win key off / on | Settings + `.` / `/` |
| Fn + Y / U | Mac / Windows mode | dropped (Windows only) |
| - | LED colour test | Settings + L |
| - | Effect set: short / full (see Lighting) | Extra + Z |
| Fn + number row, Z..N | Digits/symbols, Ins/Del/Home/End/PgUp/PgDn | Lower / Raise / Fn, as on the Planck |

The vendor draws its status flashes at the stock key positions: the sleep
timer on T, low latency on the second bottom-row key (Del in the stock
layout, Fn here), pairing on Tab/Q/W/E/R, battery on the home row.

Factory reset restores this keymap's defaults (and clears Vial changes), not
the stock layout.

## Lighting

### What you see

* **Base layer:** the normal RGB effect (see *Effect sets* below). Shift
  turns white while Caps Lock or Caps Word is on (on every layer).
* **Any other layer:** no effect; every key is an indicator.
  * Each key's colour says what it does: soft white typing, slate modifiers,
    periwinkle layer keys, mint navigation, sand F-keys, green volume/media,
    yellow-green screen brightness, pink key lighting, orchid ambient
    lighting, apricot wireless/battery, rose system/reset, sky mouse, butter
    keypad.
  * Keys that fall through (`_______`) to a lower layer show that key's
    colour, dimmed. Unused keys (`XXXXXXX`) are dark.
  * Both **Space keys show the layer colour** (green Lower, purple Raise,
    lemon Fn, orange Extra, red Mouse, white Settings). Exception: a system
    key there keeps its own colour (the factory-reset key on Settings).
  * **Live state:** keypad and Num Lock turn brown when Num Lock is off; the
    active connection (USB / BT 1-3 / 2.4 GHz) is green; NKRO is teal for
    all-key, orange for 6-key; "Win key on" lights green while the Win key
    works, "Win key off" lights red while it is disabled.
* **Lights switched off** (Extra + A): everything is dark, as in the stock
  firmware, except that holding Extra lights the two light-switch keys
  (A = key lights, J = ambient) so you can find them.
* The vendor's own indicators (pairing, battery, sleep timer, charging, ...)
  are drawn on top of all of this.

### Effect sets

Effect +/- (Extra + F / V) cycle a **short set** of calm, non-rainbow
effects by default:

`solid_color` (default), `alphas_mods`, `gradient_left_right`, `breathing`,
`band_val`, `band_spiral_val`, `riverflow`, `hue_wave`, `starlight_smooth`,
`pixel_fractal`, `solid_reactive_simple`, `solid_reactive_wide`,
`solid_splash`, `typing_heatmap`.

Extra + Z (`FX_SET`) switches to the **full set** of all 50 effects (the 45
the board enables plus `solid_reactive_simple`, `pixel_fractal`,
`pixel_rain`, `typing_heatmap` and `starlight_smooth`, enabled in
`config.h`); the key is dim
pink for the short set and bright pink for the full set. The choice is not
saved: every boot starts with the short set, and if the saved effect is not
in it, the keyboard switches to `solid_color`. Switching back to the short
set does the same. The Vial app can still select any effect directly.

How: `fx_order[]` in `keymap.c` lists every effect, short set first.
Effect +/- find the current effect's position, add or subtract one, and wrap
at 14 (short) or 50 (full); an effect outside the short set while the short
set is active therefore wraps back into it. QMK's own effect numbering can't
be reordered, hence the table. Notes: `gradient_left_right` spreads more
colours as the effect speed rises (high speed looks like a rainbow), and
`typing_heatmap` runs from blue to red by how often keys are used.

### How it works

Each frame of the RGB matrix runs, in this order:

1. QMK renders the current effect (only visible on Base).
2. `rgb_matrix_indicators_user()` (this keymap): on any layer above Base,
   paints all 48 key LEDs from a cached colour table; on every layer, the
   Caps indicator.
3. `rgb_matrix_indicators_advanced_kb()` (this keymap) calls the board's
   `rgb_matrix_indicators_advanced_user()` -> the vendor's `User_Led_Show()`:
   status LEDs, the ambient strip, and the lights-off handling. It then
   applies the light-switch override (below).
4. The vendor LED driver sends the frame, or cuts LED power (pin D0) if the
   vendor flag `Led_Point_Sleep` is set.

**Colour table.** `classify()` maps a keycode to a class (typing, modifier,
layer, ... and the live-state classes); `palette[]` holds hue, saturation
and a brightness percentage per class, and `layer_hs[]` the Space-key layer
colours. Both tables are near the end of `keymap.c` and are the place to
tune colours. `resolve()` follows `_______` down through the active layers,
so fall-through keys show the key that actually fires. Brightness follows the
RGB brightness setting, clamped between 64 (`INDICATOR_MIN_VAL`, so
indicators stay visible when the effect is dimmed) and the board maximum
(144).

**Cache.** Looking keys up reads the (emulated) EEPROM, so the 48 colours are
recomputed only when something they depend on changes: the active layers,
the host's LED state (Num Lock / Caps Lock), the connection mode and
Bluetooth channel, the brightness, or QMK's keymap settings (NKRO, Win key).
They are also refreshed every 250 ms to pick up keymap edits made in Vial.

**Light switches while lights are off.** The vendor's "all lights off"
(`RGB_RTOG`) does not disable the RGB matrix. Instead, every frame, the
vendor code paints all 77 LEDs black after our indicators and, when the
ambient strip is off too, sets `Led_Point_Sleep`, which makes the driver cut
LED power for that frame. While Extra is held with the lights off, step 3
paints the two light-switch keys after the vendor code and clears
`Led_Point_Sleep` for that frame. The vendor code recomputes the flag every
frame, so the override ends as soon as Extra is released and nothing is
saved. Cost: while Extra is held, the LED supply stays on (about the 77 LEDs'
idle current plus two dim keys); otherwise power use is unchanged.

**Caveats.** Num Lock state comes from the host's LED report; whether the
wireless link passes it back is unknown, so over Bluetooth / 2.4 GHz the
keypad may always show "Num Lock off". The connection colour shows the
*selected* connection (the vendor's saved mode), not whether it is actually
connected; the vendor's pairing blink shows on top while connecting.

## Vendor library notes (reverse-engineered)

Found by disassembling `librdrcommon.a` and reading the board file; keep these
in mind when changing the keymap:

* **Hooks.** The board file (`geonix41.c`) already defines
  `process_record_user`, `keyboard_post_init_user`, `housekeeping_task_user`
  and `rgb_matrix_indicators_advanced_user` to feed the library. This keymap
  therefore uses the `_kb` variants as wrappers that call the vendor's
  `_user` function first: `process_record_kb` (lock keys),
  `keyboard_post_init_kb` (Windows mode, key-override seeding) and
  `rgb_matrix_indicators_advanced_kb` (light-switch override).
* **Fn key.** The exact keycodes `MO(2)` and `MO(3)` are treated as "the Fn
  key" (Win/Mac indicator, some Fn + Space handling). This keymap never uses
  them: Lower/Raise are tri-layer keys (`TL_LOWR`/`TL_UPPR`, adjust layer =
  Extra) and Fn is `LT(3, KC_APP)`.
* **Mac mode.** The library treats keymap layer 1 as a Mac layer: `TO(1)` on
  the Y key / `TO(0)` on the U key switch modes, and at boot it turns layer 1
  on if Mac mode was saved. `keyboard_post_init_kb` forces Windows mode
  first, so layer 1 can be Lower.
* **NKRO.** The library saves its own NKRO setting and re-applies it at boot,
  so QMK's `NK_ON`/`NK_OFF` would not stick; the vendor `SIX_N` toggle is
  used instead.
* **RGB keys.** The library's "flash at brightness/speed limit" feedback is
  keyed on the `UG_*` keycodes (which drive the RGB matrix in this fork), so
  those are used rather than `RM_*`.
* **Status.** `Keyboard_Info` (exported) holds the connection mode, Bluetooth
  channel, NKRO, Mac/Win, light on/off and ambient settings; it is read for
  the live colours and auto Num Lock (USB only: `Key_Mode == QMK_USB_MODE`).
* **Vial key overrides** live in EEPROM and the keymap's `key_overrides[]` is
  ignored. `via_init_kb()` / `eeconfig_init_user()` note when Vial's EEPROM
  was just reset; `keyboard_post_init_kb()` then writes Shift + Backspace =
  Delete into slot 0 and calls `vial_init()` to reload.
* **EEPROM budget.** 1152 bytes of emulated EEPROM: 7 layers x 48 keys x 2 B
  = 672 B for the keymap plus Vial's default pools (8 combos, tap dances, key
  overrides, alt-repeat keys) and macros. An 8th layer needs the pools
  trimmed (`VIAL_*_ENTRIES`).
* **Build quirks.** The fork's own Vial keymap ships `vial.JSON` (wrong case
  on Linux); this keymap has its own `vial.json`. GCC 16 needs
  `-Wno-header-guard` for the vendor's CMSIS headers (`rules.mk`).
