# yu17 - Massdrop/Drop CTRL (original ATSAM / SAMD51 board)

Same as Drop's stock default layout (it was in my old `yu17/qmk_firmware`
fork since 2020; this copy is the QMK 0.26.11 default with current keycode
names).

**Upstream QMK removed this board** (the whole `arm_atsam` platform) in
0.27.0, so it is built against QMK **0.26.11**, the last release with it:
`make ctrl` uses `../qmk_firmware_legacy`, a git worktree of `../qmk_firmware`
pinned to that tag.

Flash with [mdloader](https://github.com/Massdrop/mdloader):
`mdloader --first --download massdrop_ctrl_yu17.bin --restart`, then hold
Fn + B (`MD_BOOT`, 0.5 s) or press the reset button on the back.
