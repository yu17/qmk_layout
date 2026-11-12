/* yu17 - Chosfox Geonix Rev.2.5 1U keymap configuration.
 * Builds ONLY inside the Chosfox vial-qmk fork (ES32FS026 + vendor wireless blob).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */
#pragma once

/* ---- Vial (copied from the vendor's keymaps/vial/config.h) ---------------
 * The EEPROM emulation settings below are board-specific and live in the
 * keymap in the vendor tree - keep them in sync if Chosfox changes them.
 */
#define VIAL_KEYBOARD_UID {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x11, 0x22}

// Hold Tab + Q while plugging in / when Vial asks, to unlock Vial.
#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 1151
#define EEPROM_SIZE 1152

#define FEE_PAGE_SIZE (0x200)
#define FEE_PAGE_COUNT (8)

#define FEE_PAGE_BASE_ADDRESS (0x1F000)
#define FEE_MCU_FLASH_SIZE (0x1000)

#define EECONFIG_USER_DATA_SIZE 4

#define EECONFIG_KB_DATA_SIZE 1

#define TRANSIENT_EEPROM_SIZE 4096

/* ---- yu17 ---------------------------------------------------------------- */
// Base, Lower, Raise, Fn, Extra, Mouse, Settings (KbdCfg)
#define DYNAMIC_KEYMAP_LAYER_COUNT 7

// Layer Lock (Fn + Lower / Fn + Raise) has no idle timeout: a locked numpad
// must not drop out mid-game. Unlock with Fn + Space or the same lock key.

// RGB effects: the board enables 45; also enable the 4 it leaves off and
// starlight_smooth (not in the board's list), so the full set (switchable at
// runtime, see keymap.c) has 50. Boot/reset
// default is a plain colour instead of QMK's rainbow cycle.
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#define ENABLE_RGB_MATRIX_PIXEL_RAIN
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR

// Tri-layer: Lower + Raise -> Extra
#define TRI_LAYER_LOWER_LAYER 1
#define TRI_LAYER_UPPER_LAYER 2
#define TRI_LAYER_ADJUST_LAYER 4

