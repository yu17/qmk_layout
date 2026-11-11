# yu17 QMK userspace

My keymaps, kept outside QMK as a QMK
[External Userspace](https://docs.qmk.fm/newbs_external_userspace).
The firmware trees stay untouched; this repo only holds my files.

| Keyboard | Keymap | Builds against |
|---|---|---|
| Drop Planck rev6 (`planck/rev6_drop`) | [`keyboards/planck/keymaps/yu17`](keyboards/planck/keymaps/yu17) | upstream `../qmk_firmware` |
| Chosfox Geonix Rev.2.5 1U (`geonix41/geonix41`) | [`keyboards/geonix41/geonix41/keymaps/yu17`](keyboards/geonix41/geonix41/keymaps/yu17) | Chosfox vial-qmk fork `../qmk_firmware_geonix41` |
| Massdrop CTRL, original ATSAM (`massdrop/ctrl`) | [`keyboards/massdrop/ctrl/keymaps/yu17`](keyboards/massdrop/ctrl/keymaps/yu17) | QMK 0.26.11 `../qmk_firmware_legacy` (upstream dropped the board) |

Expected layout on disk:

```
QMK/
├── qmk_firmware/            upstream qmk/qmk_firmware (git pull to update)
├── qmk_firmware_geonix41/   Chosfox fork, unzipped from qmk_firmware_Rev.2.5-1U.zip
├── qmk_firmware_legacy/     git worktree of qmk_firmware pinned at tag 0.26.11 (CTRL)
└── qmk_userspace/           this repo
```

## Building

Needs `uv`, `arm-none-eabi-gcc`, `dfu-util`. The QMK CLI is installed by uv
into `.venv/` on first use; nothing global is touched (the Makefile generates
repo-local CLI configs in `.qmk/`, so `~/.config/qmk/qmk.ini` is ignored).

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

Recreating the legacy tree (only lib/printf is needed for the CTRL):

```sh
cd ../qmk_firmware && git worktree add --detach ../qmk_firmware_legacy 0.26.11
cd ../qmk_firmware_legacy && git submodule update --init --depth 1 lib/printf
```

Updating upstream QMK:

```sh
cd ../qmk_firmware && git pull --ff-only && git submodule update --init --recursive
```

## Planck bootloader

Lower + Raise + Fn (Settings layer) + the key left of the bottom-right
corner (`QK_BOOT`), or the reset button on the PCB bottom. `make flash-planck`
waits for the DFU device.

## Geonix notes

* The Geonix MCU (Essemi ES32FS026) is **not supported by upstream QMK** and
  its Bluetooth / 2.4 GHz / battery / logo-LED code is a closed binary
  (`lib/rdr_lib/librdrcommon.a`) in the Chosfox fork. Its keymap can only be
  built there.
* The firmware is a **Vial** build: after flashing, layers, combos, tap dance,
  macros etc. can be changed live in the Vial app (unlock: hold Tab + Q).
* **Flashing** (verified 2026-11-11): unplug, hold **Tab** and plug in. The
  keyboard shows up as a 118 KB USB drive `RDMCTMZT DFU` (`03eb:2045`). Copy
  the `.bin` onto it and it reboots into the new firmware by itself:
  `make flash-geonix` (or `mcopy -i /dev/sdX geonix41_geonix41_yu17.bin ::GEONIX.BIN && sync`,
  or drag the file in a file manager). To roll back, copy the stock
  `qmk_firmware_geonix41/geonix41_geonix41_vial.bin` the same way.
* **Vial on Linux** needs access to the raw-HID device, e.g.
  `/etc/udev/rules.d/59-vial.rules`:
  `KERNEL=="hidraw*", SUBSYSTEM=="hidraw", ATTRS{serial}=="*vial:f64c2b3c*", MODE="0660", GROUP="users", TAG+="uaccess", TAG+="udev-acl"`
* Factory reset (Settings + left Space, hold 3 s) restores this keymap's
  defaults, not the stock layout.

## Rollback / history

My keymaps used to live in the `yu17/qmk_firmware` GitHub fork (2020-2023).
That fork was reset to upstream; its old `master` is kept as a git bundle in
`../backups/` (unbundle inside a qmk_firmware clone). `archive/` keeps the
original CTRL keymap, the fork's `select_keyboard.sh`, and an old
`arm_atsam/i2c_master.c` diff (stale, not needed).


`archive/planck_rev6_drop_yu17-2023-12-11.bin` is the Planck firmware that was
on the board before this rework (`dfu-util -a 0 -d 0483:df11 -s 0x08000000:leave -D <file>`
with the Planck in bootloader mode).
