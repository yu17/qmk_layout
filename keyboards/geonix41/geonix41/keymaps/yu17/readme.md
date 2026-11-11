# yu17 - Chosfox Geonix Rev.2.5 1U

The yu17 Planck layout ported key-for-key (both are 4x12 grids). Builds only
inside the Chosfox vial-qmk fork. See the top-level README.

Differences from the Planck:

* No audio, music mode, DIP switches or Plover layer.
* **Mac/Win:** KbdCfg + `Y` = Mac (Cmd/Opt swapped, vendor-style), KbdCfg + `U`
  = Windows. The mode is remembered by the vendor firmware.
* **Wireless:** KbdCfg + Tab / Q / W / E / R = USB / BT1 / BT2 / BT3 / 2.4 GHz.
  Tap to connect, hold to pair. These are the stock key positions, so the
  stock indicator LEDs line up.
* KbdCfg also has battery level (`Z`), sleep timer (`X`), 2.4 GHz
  low-latency (`C`, hold 3 s), ambient-light effect (`V`), LED colour test
  (`,`), brightness/speed on the arrows, and factory reset (left Space, hold
  3 s).
* **Per-key layer colours:** on Lower/Raise/Fn/Extra/Mouse/KbdCfg, keys that
  do something light in the layer colour and unused keys go dark. Shift turns
  white while Caps Lock / Caps Word is on.
* Lower/Raise use QMK tri-layer keys (`TL_LOWR`/`TL_UPPR`). The vendor library
  treats `MO(2)`/`MO(3)` as its own Fn key, so this keymap must not use them.
* Vial's tap-dance, combo, key-override and alt-repeat pools are 4 entries
  each, so 8 layers fit in the 1152-byte EEPROM.
