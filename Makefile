# yu17 QMK userspace build helper.
#
# Builds use repo-local QMK CLI config files (generated under .qmk/), so the
# global ~/.config/qmk/qmk.ini is never consulted or modified.
#
#   make planck          build Planck (Drop rev6) firmware against upstream qmk_firmware
#   make flash-planck    build + flash (put the Planck in bootloader mode when asked)
#   make geonix          build Geonix Rev2.5-1U firmware against the Chosfox vial-qmk fork
#   make ctrl            build Massdrop CTRL firmware against pinned QMK 0.26.11 (last with arm_atsam)
#   make all             build everything
#   make clean           remove built firmware files and generated configs

QMK_FIRMWARE    ?= $(abspath $(CURDIR)/../qmk_firmware)
GEONIX_FIRMWARE ?= $(abspath $(CURDIR)/../qmk_firmware_geonix41)
LEGACY_FIRMWARE ?= $(abspath $(CURDIR)/../qmk_firmware_legacy)

PLANCK_KB  := planck/rev6_drop
GEONIX_KB  := geonix41/geonix41
CTRL_KB    := massdrop/ctrl
KEYMAP     := yu17

QMK = uv run --project $(CURDIR) qmk --config-file

.PHONY: all planck flash-planck geonix ctrl clean
all: planck geonix ctrl

.qmk/%.ini: Makefile
	@mkdir -p .qmk
	@printf '[user]\nqmk_home = %s\noverlay_dir = %s\n' \
		"$(if $(filter geonix,$*),$(GEONIX_FIRMWARE),$(if $(filter legacy,$*),$(LEGACY_FIRMWARE),$(QMK_FIRMWARE)))" "$(CURDIR)" > $@

planck: .qmk/upstream.ini
	$(QMK) $< compile -kb $(PLANCK_KB) -km $(KEYMAP)

flash-planck: .qmk/upstream.ini
	$(QMK) $< flash -kb $(PLANCK_KB) -km $(KEYMAP)

geonix: .qmk/geonix.ini
	$(QMK) $< compile -kb $(GEONIX_KB) -km $(KEYMAP)

ctrl: .qmk/legacy.ini
	$(QMK) $< compile -kb $(CTRL_KB) -km $(KEYMAP)

clean:
	rm -rf .qmk *.bin *.hex *.uf2
