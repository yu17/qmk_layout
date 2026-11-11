# yu17 QMK userspace

My keymaps, kept outside QMK as a QMK
[External Userspace](https://docs.qmk.fm/newbs_external_userspace).
The firmware trees stay untouched; this repo only holds my files.

| Keyboard | Keymap (and its readme) | Builds against |
|---|---|---|
| Drop Planck rev6 (`planck/rev6_drop`) | [`keyboards/planck/keymaps/yu17`](keyboards/planck/keymaps/yu17) | upstream `../qmk_firmware` |
| Chosfox Geonix Rev.2.5 1U (`geonix41/geonix41`) | [`keyboards/geonix41/geonix41/keymaps/yu17`](keyboards/geonix41/geonix41/keymaps/yu17) | Chosfox vial-qmk fork `../qmk_firmware_geonix41` |
| Massdrop CTRL, original ATSAM (`massdrop/ctrl`) | [`keyboards/massdrop/ctrl/keymaps/yu17`](keyboards/massdrop/ctrl/keymaps/yu17) | QMK 0.26.11 `../qmk_firmware_legacy` (upstream dropped the board) |

Documentation:

* This file: the shared Planck/Geonix layout, building, flashing, history.
* Each keymap's `readme.md`: board-specific features, and how the lighting
  code works (Planck underglow, Geonix per-key colours and the vendor
  library constraints).
* `docs/layout.html`: a visual map of every layer of all three keyboards,
  generated from the `keymap.c` files (`make docs`). Each layer is also drawn
  as a comment in `keymap.c`.

## Shared layout (Planck and Geonix)

Both are 4x12 grids and use the same layers, numbered the same way:

| # | Layer | How to get there | What's on it |
|---|---|---|---|
| 0 | Base | default | QWERTY; bottom row Ctrl, Fn, Win, Alt, Lower, Space, Space, Raise, Left, Down, Up, Right |
| 1 | Lower | hold Lower; lock with Fn + Lower | digits row, `- = [ ] \`, Caps Word (A), Home / PgDn / PgUp / End |
| 2 | Raise | hold Raise; lock with Fn + Raise | keypad (keypad codes + Num Lock) on the right, arrows in the middle, left half falls through (WASD etc. for games); Num Lock is switched on automatically |
| 3 | Fn | hold Fn (tap Fn = Menu/App) | F1-F12; Mute, Vol-, Vol+, Prev, Play/Pause, Next, Stop; PrtSc, ScrLk, Pause; screen brightness; Ins, Del, Right Shift; Right Alt, Right Win, Menu, Right Ctrl; lock Lower / lock Raise; Space = back to Base |
| 4 | Extra | hold Lower + Raise | F13-F24, all lighting controls, Mouse layer key |
| 5 | Mouse | Extra + Mouse key (stays on) | mouse movement, buttons, wheel; top-right key returns to Base |
| 6 | Settings | hold Lower + Raise + Fn | keyboard configuration (Planck: audio, Alt/Win swap, NKRO, Win key, Plover, bootloader, reset; Geonix: wireless, battery, sleep timer, NKRO, Win key, factory reset) |
| 7 | Plover | Planck only: Settings + Plover | steno layer for the Plover app |

Everywhere:

* **Shift + Backspace = Delete.**
* **Fn + Space** returns to Base and releases a locked layer.
* **Layer lock** (Fn + Lower / Fn + Raise) has no timeout; release it with
  Fn + Space or the same combo.
* Arrows are Left, Down, Up, Right; PgDn/PgUp and the mouse arrows follow.
* Each layer has its own colour: green Lower, purple Raise, lemon Fn, orange
  Extra, red Mouse, white Settings (Planck: whole underglow; Geonix: Space
  keys, plus per-key hints).

## Building

Needs `uv`, `arm-none-eabi-gcc`, `dfu-util`, `mtools`. The QMK CLI is
installed by uv into `.venv/` on first use; nothing global is touched (the
Makefile generates repo-local CLI configs in `.qmk/`, so
`~/.config/qmk/qmk.ini` is ignored).

```sh
make planck         # -> planck_rev6_drop_yu17.bin
make flash-planck   # build + flash; put the Planck in bootloader when asked
make geonix         # -> geonix41_geonix41_yu17.bin
make flash-geonix   # build + copy to the Geonix bootloader drive (hold Tab while plugging in)
make ctrl           # -> massdrop_ctrl_yu17.bin
make docs           # -> docs/layout.html, a visual map of every layer
make all
```

Firmware paths can be overridden: `make planck QMK_FIRMWARE=/path/to/qmk_firmware`.

Expected layout on disk:

```
QMK/
├── qmk_firmware/            upstream qmk/qmk_firmware (git pull to update)
├── qmk_firmware_geonix41/   Chosfox fork, unzipped from qmk_firmware_Rev.2.5-1U.zip
├── qmk_firmware_legacy/     git worktree of qmk_firmware pinned at tag 0.26.11 (CTRL)
├── backups/                 git bundle of the old yu17/qmk_firmware fork
└── qmk_userspace/           this repo
```

Recreating the legacy tree (only lib/printf is needed for the CTRL):

```sh
cd ../qmk_firmware && git worktree add --detach ../qmk_firmware_legacy 0.26.11
cd ../qmk_firmware_legacy && git submodule update --init --depth 1 lib/printf
```

Updating upstream QMK:

```sh
cd ../qmk_firmware && git pull --ff-only && git submodule update --init --recursive
```

## Flashing

* **Planck:** `make flash-planck`, then enter the bootloader with
  Lower + Raise + Fn (Settings) + the key left of the bottom-right corner
  (`QK_BOOT`), or the reset button on the PCB bottom.
* **Geonix** (verified 2026-11-11): unplug, hold **Tab** and plug in. The
  keyboard shows up as a 118 KB USB drive `RDMCTMZT DFU` (`03eb:2045`);
  `make flash-geonix` copies the firmware onto it and it reboots by itself
  (by hand: `mcopy -i /dev/sdX geonix41_geonix41_yu17.bin ::GEONIX.BIN && sync`,
  or drag the file in a file manager). To roll back, copy the stock
  `qmk_firmware_geonix41/geonix41_geonix41_vial.bin` the same way.
* **CTRL:** see its readme (needs `mdloader`).

## Geonix notes

* The Geonix MCU (Essemi ES32FS026) is **not supported by upstream QMK** and
  its Bluetooth / 2.4 GHz / battery / ambient-LED code is a closed binary
  (`lib/rdr_lib/librdrcommon.a`) in the Chosfox fork, so its keymap can only
  be built there. The keymap readme lists what that library expects.
* The firmware is a **Vial** build: layers, combos, tap dance, key overrides,
  macros etc. can be changed live in the Vial app (unlock: hold Tab + Q).
  Flashing new firmware (or a factory reset) resets Vial changes to this
  keymap.
* **Vial on Linux** needs access to the raw-HID device, e.g.
  `/etc/udev/rules.d/59-vial.rules`:
  `KERNEL=="hidraw*", SUBSYSTEM=="hidraw", ATTRS{serial}=="*vial:f64c2b3c*", MODE="0660", GROUP="users", TAG+="uaccess", TAG+="udev-acl"`

## Rollback / history

My keymaps used to live in the `yu17/qmk_firmware` GitHub fork (2020-2023).
That fork was reset to upstream; its old `master` is kept as a git bundle in
`../backups/` (unbundle inside a qmk_firmware clone). See
[`archive/`](archive) for the old CTRL keymap, the Planck firmware that was on
the board before this rework, and other leftovers.
