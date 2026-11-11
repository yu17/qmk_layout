# yu17 - Chosfox Geonix Rev.2.5 1U

The yu17 Planck layout ported key-for-key (both are 4x12 grids, same layer
numbers). Builds only inside the Chosfox vial-qmk fork. See the top-level
README for building and flashing.

Differences from the Planck:

* No audio, music mode, DIP switches or Plover layer.
* **Windows layout only.** The vendor Mac mode is cleared at start-up.
* **Per-key layer colours:** on Lower/Raise/Fn/Extra/Mouse/KbdCfg, keys that
  do something light in the layer colour and unused keys go dark. Shift turns
  white while Caps Lock / Caps Word is on.
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

"KbdCfg" = hold **Lower + Raise + Fn**.

| Stock | Function | Here |
|---|---|---|
| Fn + Tab / Q / W / E / R | USB / BT1 / BT2 / BT3 / 2.4 GHz (hold = pair) | KbdCfg + same keys |
| Fn + T | Next key-light effect | KbdCfg + T (also Extra + Z); KbdCfg + B = previous |
| Fn + Backspace | All lights on/off | KbdCfg + Backspace (also Extra + A) |
| Fn + Up / Down | Brightness + / - | KbdCfg + Up / Down (same keys, new bottom-row order) |
| Fn + Left / Right | Effect speed - / + | KbdCfg + Left / Right |
| Fn + Enter | Next ambient (side/logo) light effect | KbdCfg + Enter (also KbdCfg + V, Extra + S) |
| Fn + Left Alt | Wireless sleep timer 1/3/10/30 min (T flashes red/green/blue/white) | KbdCfg + X |
| Fn + Ctrl | Battery level | KbdCfg + Z |
| Fn + Del (hold 3 s) | 2.4 GHz low-latency on/off (Del flashes red/green) | KbdCfg + C (hold 3 s) |
| Fn + left Space (hold 3 s) | Factory reset | KbdCfg + left Space (hold 3 s) |
| Fn + Y / U | Mac / Windows mode | dropped (Windows only) |
| - | LED colour test | KbdCfg + `,` |
| Fn + number row, Z..N | Digits/symbols, Ins/Del/Home/End/PgUp/PgDn | Lower / Raise, as on the Planck |

The status flashes (sleep timer on T, low latency on Del) are drawn by the
vendor library at the stock key positions, so they show on T and on the
bottom-row key Del sits on in the stock layout (the Fn key here).

Factory reset restores this keymap's defaults (and clears Vial changes), not
the stock layout.
