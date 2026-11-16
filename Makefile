# yu17 QMK userspace build helper.
#
# Builds use repo-local QMK CLI config files (generated under .qmk/), so the
# global ~/.config/qmk/qmk.ini is never consulted or modified.
#
#   make planck          build Planck (Drop rev6) firmware against upstream qmk_firmware
#   make flash-planck    build + flash (put the Planck in bootloader mode when asked)
#   make planck-diag / flash-planck-diag   hardware-check build (yu17_diag, see its readme)
#   make geonix          build Geonix Rev2.5-1U firmware against the Chosfox vial-qmk fork
#   make flash-geonix    build + copy onto the Geonix bootloader drive (hold Tab while plugging in)
#   make ctrl            build Massdrop CTRL firmware against pinned QMK 0.26.11 (last with arm_atsam)
#   make all             build everything
#   make docs            regenerate docs/layout.html (visual map of all layers)
#   make clean           remove built firmware files and generated configs

QMK_FIRMWARE    ?= $(abspath $(CURDIR)/../qmk_firmware)
GEONIX_FIRMWARE ?= $(abspath $(CURDIR)/../qmk_firmware_geonix41)
LEGACY_FIRMWARE ?= $(abspath $(CURDIR)/../qmk_firmware_legacy)

PLANCK_KB  := planck/rev6_drop
GEONIX_KB  := geonix41/geonix41
CTRL_KB    := massdrop/ctrl
KEYMAP     := yu17

QMK = uv run --project $(CURDIR) qmk --config-file

.PHONY: FORCE all planck flash-planck planck-diag flash-planck-diag geonix flash-geonix ctrl docs clean
all: planck geonix ctrl

FORCE:

# Regenerated every run, so QMK_FIRMWARE=... etc. overrides always apply.
.qmk/%.ini: FORCE
	@mkdir -p .qmk
	@printf '[user]\nqmk_home = %s\noverlay_dir = %s\n' \
		"$(if $(filter geonix,$*),$(GEONIX_FIRMWARE),$(if $(filter legacy,$*),$(LEGACY_FIRMWARE),$(QMK_FIRMWARE)))" "$(CURDIR)" > $@

planck: .qmk/upstream.ini
	$(QMK) $< compile -kb $(PLANCK_KB) -km $(KEYMAP)

flash-planck: .qmk/upstream.ini
	$(QMK) $< flash -kb $(PLANCK_KB) -km $(KEYMAP)

planck-diag: .qmk/upstream.ini
	$(QMK) $< compile -kb $(PLANCK_KB) -km $(KEYMAP)_diag

flash-planck-diag: .qmk/upstream.ini
	$(QMK) $< flash -kb $(PLANCK_KB) -km $(KEYMAP)_diag

geonix: .qmk/geonix.ini
	$(QMK) $< compile -kb $(GEONIX_KB) -km $(KEYMAP)

# Geonix bootloader = FAT12 USB drive "RDMCTMZT DFU" (03eb:2045); copying the
# .bin onto it flashes and reboots. If the raw device is writable (udev rule
# udev/70-geonix-bootloader.rules, or the disk group) it is written raw with
# mtools; otherwise it is mounted through udisks (no extra rights needed) and
# the file copied in.
flash-geonix: geonix
	@dev=$$(for d in /dev/sd?; do udevadm info -q property $$d 2>/dev/null | grep -q '^ID_MODEL=RDMCTMZT_DFU$$' && echo $$d; done | head -1); \
	if [ -z "$$dev" ]; then echo "Geonix bootloader drive not found: unplug, hold Tab, plug in, retry."; exit 1; fi; \
	if [ -w "$$dev" ]; then \
		echo "Flashing via $$dev (raw)"; \
		MTOOLS_SKIP_CHECK=1 mcopy -o -i $$dev geonix41_geonix41_yu17.bin ::GEONIX.BIN && sync; \
	else \
		mnt=$$(findmnt -n -o TARGET --source $$dev); \
		if [ -z "$$mnt" ]; then udisksctl mount --no-user-interaction -b $$dev >/dev/null && mnt=$$(findmnt -n -o TARGET --source $$dev); fi; \
		if [ -z "$$mnt" ]; then echo "Could not mount $$dev"; exit 1; fi; \
		echo "Flashing via $$mnt (mounted $$dev)"; \
		cp geonix41_geonix41_yu17.bin "$$mnt/GEONIX.BIN" && sync; \
	fi && echo "Done - the keyboard reboots by itself within a few seconds."

ctrl: .qmk/legacy.ini
	$(QMK) $< compile -kb $(CTRL_KB) -km $(KEYMAP)

docs:
	python3 -I tools/layout_doc.py $(LEGACY_FIRMWARE)

clean:
	rm -rf .qmk *.bin *.hex *.uf2
