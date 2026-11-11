# archive

Leftovers kept for reference; nothing here is built.

| File | What it is |
|---|---|
| `planck_rev6_drop_yu17-2023-12-11.bin` | The Planck firmware that was on the board before the 2026 rework (built from the old fork). To restore: put the Planck in bootloader mode, then `dfu-util -a 0 -d 0483:df11 -s 0x08000000:leave -D planck_rev6_drop_yu17-2023-12-11.bin`. |
| `massdrop_ctrl/keymap.c` | The CTRL keymap as it was in the old fork (Drop's 2020 default). The live copy is `keyboards/massdrop/ctrl/keymaps/yu17`. |
| `massdrop_ctrl/arm_atsam-i2c_master.patch` | The fork's diff of `tmk_core/protocol/arm_atsam/i2c_master.c` against upstream. It is an outdated copy of the file kept by old merges, not a real fix; not needed. |
| `select_keyboard.sh` | Old helper that switched the global `qmk config user.keyboard`; superseded by the Makefile targets. |

The full history of the old fork is in `../../backups/yu17-qmk_firmware-fork-master-9dc5752bb4.bundle`.
