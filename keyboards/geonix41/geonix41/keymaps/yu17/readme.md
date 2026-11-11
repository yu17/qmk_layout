# yu17 - Chosfox Geonix Rev.2.5 1U

The yu17 Planck layout ported key-for-key (both are 4x12 grids, same layer
numbers). Builds only inside the Chosfox vial-qmk fork. See the top-level
README for building and flashing.

Differences from the Planck:

* No audio, music mode, DIP switches or Plover layer.
* **Windows layout only.** The vendor Mac mode is cleared at start-up.
* **Per-key colour hints:** only Base runs the RGB effect. On every other
  layer each key shows what it does (typing, modifier, layer, navigation,
  F-key, volume, screen brightness, lighting, ambient, wireless, system,
  mouse, keypad), keys falling through to a lower layer are dimmed, unused
  keys are dark, and both Space keys show the layer colour (green Lower,
  purple Raise, lemon Fn, orange Extra, red Mouse, white Settings).
  Live state: Num Lock (brown = off), active connection (green), NKRO
  (teal = all-key, orange = 6-key), Win key (lit GU_ON green / GU_OFF red).
  Shift turns white while Caps Lock / Caps Word is on. The palette is one
  table in `keymap.c` (`palette[]`).
  * Num Lock and connection state come from the host / vendor library; over
    Bluetooth / 2.4 GHz Num Lock may always read "off" if the wireless chip
    does not pass the host's LED state back.
* **Raise = numpad**, same as the Planck. Num Lock is switched on
  automatically only over USB; when wireless, use the Num Lock key on Raise.
* **Lock a layer:** Fn + Lower / Fn + Raise; Fn + Space releases it.
* **Shift + Backspace = Delete** is a Vial key override (slot 0), written
  after every flash / factory reset; edit or delete it in the Vial app.
* Arrows are Left, Down, Up, Right, as on the Planck.
* Lower/Raise use QMK tri-layer keys (`TL_LOWR`/`TL_UPPR`), and Fn is
  `LT(3, App)`. The vendor library treats the keycodes `MO(2)`/`MO(3)` as its
  own Fn key, so this keymap must not use them.

## Stock Fn functions -> this keymap

"Extra" = hold **Lower + Raise** (all lighting). "Settings" = hold
**Lower + Raise + Fn** (wireless and keyboard configuration).

| Stock | Function | Here |
|---|---|---|
| Fn + Tab / Q / W / E / R | USB / BT1 / BT2 / BT3 / 2.4 GHz (hold = pair) | Settings + same keys |
| Fn + T | Next key-light effect | Extra + S (previous: Extra + X) |
| Fn + Backspace | All lights on/off | Extra + A |
| Fn + Up / Down | Brightness + / - | Extra + G / B |
| Fn + Left / Right | Effect speed - / + | Extra + N / H |
| Fn + Enter | Next ambient (side/logo) light effect | Extra + K (previous: Extra + `,`) |
| - | Ambient on/off, hue +/-, brightness +/- | Extra + J, L / `.`, `;` / `/` |
| Fn + Left Alt | Wireless sleep timer 1/3/10/30 min (T flashes red/green/blue/white) | Settings + X |
| Fn + Ctrl | Battery level | Settings + Z |
| Fn + Del (hold 3 s) | 2.4 GHz low-latency on/off (Del flashes red/green) | Settings + C (hold 3 s) |
| Fn + left Space (hold 3 s) | Factory reset | Settings + left Space (hold 3 s) |
| - | 6-key / all-key rollover (NKRO) | Settings + M |
| - | Win key off / on | Settings + `.` / `/` |
| Fn + Y / U | Mac / Windows mode | dropped (Windows only) |
| - | LED colour test | Extra + Z |
| Fn + number row, Z..N | Digits/symbols, Ins/Del/Home/End/PgUp/PgDn | Lower / Raise, as on the Planck |

The status flashes (sleep timer on T, low latency on Del) are drawn by the
vendor library at the stock key positions, so they show on T and on the
bottom-row key Del sits on in the stock layout (the Fn key here).

Factory reset restores this keymap's defaults (and clears Vial changes), not
the stock layout.
