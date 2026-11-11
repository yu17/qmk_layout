# yu17 QMK userspace

My keymaps, kept outside QMK as a QMK
[External Userspace](https://docs.qmk.fm/newbs_external_userspace).
The firmware trees stay untouched; this repo only holds my files.

| Keyboard | Keymap | Builds against |
|---|---|---|
| Drop Planck rev6 (`planck/rev6_drop`) | [`keyboards/planck/keymaps/yu17`](keyboards/planck/keymaps/yu17) | upstream `../qmk_firmware` |
| Chosfox Geonix Rev.2.5 1U (`geonix41/geonix41`) | [`keyboards/geonix41/geonix41/keymaps/yu17`](keyboards/geonix41/geonix41/keymaps/yu17) | Chosfox vial-qmk fork `../qmk_firmware_geonix41` |

Expected layout on disk:

```
QMK/
├── qmk_firmware/            upstream qmk/qmk_firmware (git pull to update)
├── qmk_firmware_geonix41/   Chosfox fork, unzipped from qmk_firmware_Rev.2.5-1U.zip
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
make all
```

Firmware paths can be overridden: `make planck QMK_FIRMWARE=/path/to/qmk_firmware`.

Updating upstream QMK:

```sh
cd ../qmk_firmware && git pull --ff-only && git submodule update --init --recursive
```

## Planck bootloader

Lower + Raise (Extra layer) + the key left of the bottom-right corner
(`QK_BOOT`), or the reset button on the PCB bottom. `make flash-planck`
waits for the DFU device.

## Geonix notes

* The Geonix MCU (Essemi ES32FS026) is **not supported by upstream QMK** and
  its Bluetooth / 2.4 GHz / battery / logo-LED code is a closed binary
  (`lib/rdr_lib/librdrcommon.a`) in the Chosfox fork. Its keymap can only be
  built there.
* The firmware is a **Vial** build: after flashing, layers, combos, tap dance,
  macros etc. can be changed live in the Vial app (unlock: hold Tab + Q).
* **Flashing:** the chip uses Essemi's own ROM bootloader, not DFU, so
  `qmk flash` / QMK Toolbox cannot flash it. Use Chosfox's firmware updater
  (ask Chosfox support if it isn't on their download page) with
  `geonix41_geonix41_yu17.bin`. Keep their stock
  `qmk_firmware_geonix41/geonix41_geonix41_vial.bin` to roll back.
* Factory reset (KbdCfg + left Space, hold 3 s) restores this keymap's
  defaults, not the stock layout.

## Rollback

`archive/planck_rev6_drop_yu17-2023-12-11.bin` is the Planck firmware that was
on the board before this rework (`dfu-util -a 0 -d 0483:df11 -s 0x08000000:leave -D <file>`
with the Planck in bootloader mode).
