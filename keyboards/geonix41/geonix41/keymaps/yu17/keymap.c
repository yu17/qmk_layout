/* yu17 - Chosfox Geonix Rev.2.5 1U (4x12 ortho, 48 keys).
 * Port of the yu17 Planck layout. Builds ONLY inside the Chosfox vial-qmk fork:
 * Bluetooth / 2.4 GHz / battery / logo LEDs live in the closed vendor library
 * lib/rdr_lib/librdrcommon.a, driven from keyboards/geonix41/geonix41/geonix41.c
 * (which already defines process_record_user, keyboard_post_init_user,
 * housekeeping_task_user and rgb_matrix_indicators_advanced_user - this keymap
 * must not define those).
 *
 * Constraints imposed by the vendor library (found by disassembling it):
 *  - Vendor "Mac mode" = keymap layer 1, switched by TO(1) on Y / TO(0) on U,
 *    and re-enabled at boot (layer_on(1)) if saved. This keymap is Windows-
 *    only: Mac mode is cleared at start-up (keyboard_post_init_kb below) and
 *    no TO(1)-on-Y key exists, so layer 1 is free for Lower.
 *  - The exact keycodes MO(2) and MO(3) are treated as "the Fn key" (lights
 *    the Win/Mac indicator, arms some Fn+Space handling). This keymap never
 *    uses MO(2)/MO(3): Lower/Raise use the tri-layer keycodes, Fn is LT().
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include QMK_KEYBOARD_H
#include "rdr_common.h"
#include "dynamic_keymap.h"

enum geonix_layers { // same numbering as the Planck keymap
    _BASE,   // 0
    _LOWER,  // 1  (tri-layer lower)
    _RAISE,  // 2  (tri-layer upper)
    _FN,     // 3  LT(), never MO(3) - see above
    _EXTRA,  // 4  Lower + Raise (tri-layer adjust)
    _MOUSE,  // 5
    _KBDCFG, // 6  hold Lower + Raise + Fn
};

enum yu17_keycodes {
    LLCK_LOWER = QK_USER, // Fn + Lower: lock/unlock the Lower layer
    LLCK_RAISE,           // Fn + Raise: lock/unlock the Raise layer
};

#define LOWER TL_LOWR
#define RAISE TL_UPPR
#define FN_APP LT(_FN, KC_APP)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty                                                                  LED: RGB effect
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT | Lower| Space| Space| Raise| Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_BASE] = LAYOUT_tkl_ansi(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT ,
    KC_LCTL, FN_APP,  KC_LGUI, KC_LALT, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

/* Lower                                                                   LED: green
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |CapsWd|      |      |      |      |      |      |      |   -  |   =  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|      |      |      |      |      |      |      |   [  |   ]  |   \  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT |      | Space| Space| Extra| Home | PgDn | PgUp | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_tkl_ansi(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_ESC,  CW_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS, KC_EQL,  XXXXXXX,
    KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS, KC_ENT,
    KC_LCTL, FN_APP,  KC_LGUI, KC_LALT, XXXXXXX, KC_SPC,  KC_SPC,  RAISE,   KC_HOME, KC_PGDN, KC_PGUP, KC_END
),

/* Raise  (numpad; left half = normal keys, e.g. WASD for games)          LED: purple
 * Lock it with Fn + Raise; Fn + Space (or Fn + Raise again) unlocks.
 * Keypad codes (KC_P*), so games/sims see real numpad keys; Num Lock is
 * switched on automatically when this layer turns on (wired USB only).
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |      |NumLck|  7   |  8   |  9   |  *   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |  Up  |      |  4   |  5   |  6   |  -   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  | Left | Down |Right |  1   |  2   |  3   |  +   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT | Extra| Space| Space|(held)|  0   |  .   |  /   |Enter |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_tkl_ansi(
    _______, _______, _______, _______, _______, _______, XXXXXXX, KC_NUM,  KC_P7,   KC_P8,   KC_P9,   KC_PAST,
    _______, _______, _______, _______, _______, _______, KC_UP,   XXXXXXX, KC_P4,   KC_P5,   KC_P6,   KC_PMNS,
    _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_P1,   KC_P2,   KC_P3,   KC_PPLS,
    _______, _______, _______, _______, LOWER,   _______, _______, _______, KC_P0,   KC_PDOT, KC_PSLS, KC_PENT
),

/* Fn                                                                      LED: lemon
 * ,-----------------------------------------------------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Mute | Vol- | Vol+ |      |      |      |      |      |      | ScrLk| Pause| PrScr|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift| Brit-| Brit+|      |      |      |      |      |      | Insrt| Del  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|      | LGUI | LALT |LckLow|TO_BAS|TO_BAS|LckRai|      | RGUI | MENU |RCTRL |
 * `-----------------------------------------------------------------------------------'
 */
[_FN] = LAYOUT_tkl_ansi(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
    KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SCRL, KC_PAUS, KC_PSCR,
    KC_LSFT, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_INS,  KC_DEL,  XXXXXXX,
    KC_LCTL, XXXXXXX, KC_LGUI, KC_LALT, LLCK_LOWER, TO(_BASE), TO(_BASE), LLCK_RAISE, XXXXXXX, KC_RGUI, KC_MENU, KC_RCTL
),

/* Extra  (Lower + Raise)                                                  LED: orange
 * ,-----------------------------------------------------------------------------------.
 * |  F13 |  F14 |  F15 |  F16 |  F17 |  F18 |  F19 |  F20 |  F21 |  F22 |  F23 |  F24 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |LgtTog|Ambnt |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|RGBMOD|RGBHUI|RGBSAI|RGBVAI|      |      |NKOFF | NKON |GUIOFF|GUION | Mouse|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|KbdCfg| LGUI | LALT |      | Space| Space|      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 *   LgtTog = all key lights on/off, Ambnt = cycle ambient (logo) light effect.
 *   KbdCfg is a hold key: Lower + Raise + (FN/M position).
 */
[_EXTRA] = LAYOUT_tkl_ansi(
    KC_F13,  KC_F14,   KC_F15,   KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
    KC_ESC,  RGB_RTOG, LOGO_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LSFT, UG_NEXT,  UG_HUEU,  UG_SATU, UG_VALU, XXXXXXX, XXXXXXX, NK_OFF,  NK_ON,   GU_OFF,  GU_ON,   TO(_MOUSE),
    KC_LCTL, MO(_KBDCFG), KC_LGUI, KC_LALT, XXXXXXX, KC_SPC, KC_SPC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

/* Mouse  (Extra -> Mouse; leave with ToBase)                              LED: red
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |      | MsWhU|      |   R  |   T  |   Y  |   U  |MsSpd0|MsSpd1|MsSpd2|ToBase|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  | MsWhL| MsWhD| MsWhR|      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |      |      |      | MS_1 | MS_3 | MS_2 |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT | MS_4 | Space| Space| MS_5 | MS_L | MS_D | MS_U | MS_R |
 * `-----------------------------------------------------------------------------------'
 */
[_MOUSE] = LAYOUT_tkl_ansi(
    KC_TAB,  XXXXXXX, MS_WHLU, XXXXXXX, KC_R,    KC_T,    KC_Y,    KC_U,    MS_ACL0, MS_ACL1, MS_ACL2, TO(_BASE),
    KC_ESC,  MS_WHLL, MS_WHLD, MS_WHLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    XXXXXXX, XXXXXXX, XXXXXXX, MS_BTN1, MS_BTN3, MS_BTN2, XXXXXXX,
    KC_LCTL, FN_APP,  KC_LGUI, KC_LALT, MS_BTN4, KC_SPC,  KC_SPC,  MS_BTN5, MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT
),

/* KbdCfg  (hold Lower + Raise + Fn)                                     LED: white
 * Keys sit where the stock firmware has them where possible (Fn+Tab/Q/W/E/R,
 * T, Bksp, Enter, arrows, left Space), so the vendor's indicator LEDs line up.
 * ,-----------------------------------------------------------------------------------.
 * | USB  | BT1  | BT2  | BT3  | 2.4G |RGBMOD|      |      |      |      |      |LgtTog|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | HUE+ | HUE- | SAT+ | SAT- |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Batt |Sleep |LowLat|Ambnt |RGBMD-|      |      |LEDtst|      |      |Ambnt |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |Reset3s      |      |Spd-  |Brgt- |Brgt+ |Spd+  |
 * `-----------------------------------------------------------------------------------'
 *   BT1-3 / 2.4G: tap = connect, hold = pair (vendor behaviour).
 *   Batt: show battery level on the number row. Sleep: cycle sleep timer.
 *   LowLat: hold 3 s = 2.4 GHz low-latency toggle. Reset3s: hold 3 s = factory reset.
 */
[_KBDCFG] = LAYOUT_tkl_ansi(
    MD_USB,  MD_BLE1, MD_BLE2, MD_BLE3, MD_24G,  UG_NEXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_RTOG,
    XXXXXXX, UG_HUEU, UG_HUED, UG_SATU, UG_SATD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, QK_BAT,  TIME_ST, QK_DEB,  LOGO_MOD, UG_PREV, XXXXXXX, XXXXXXX, TEST_CL, XXXXXXX, XXXXXXX, LOGO_MOD,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, U_EE_CLR, XXXXXXX, XXXXXXX, UG_SPDD, UG_VALD, UG_VALU, UG_SPDU
),

};
// clang-format on

/* ------------------------------------------------- Default key override --
 * Shift + Backspace = Delete. Vial keeps key overrides in EEPROM and ignores
 * a keymap's key_overrides[], so seed Vial slot 0 whenever Vial's EEPROM was
 * just reset (new firmware or factory reset). It then shows up - and can be
 * edited or deleted - in the Vial app.
 */
static bool vial_data_reset = false;

void via_init_kb(void) { // runs right before VIA validates (and maybe resets) its EEPROM
    if (!via_eeprom_is_valid()) vial_data_reset = true;
}

void eeconfig_init_user(void) { // full settings reset
    vial_data_reset = true;
}

static void seed_key_overrides(void) {
    vial_key_override_entry_t ko = {
        .trigger          = KC_BSPC,
        .replacement      = KC_DEL,
        .layers           = 0xFFFF,
        .trigger_mods     = MOD_MASK_SHIFT,
        .negative_mod_mask = 0,
        .suppressed_mods  = MOD_MASK_SHIFT,
        .options          = vial_ko_enabled | vial_ko_option_activation_trigger_down |
                   vial_ko_option_activation_required_mod_down | vial_ko_option_activation_negative_mod_up,
    };
    dynamic_keymap_set_key_override(0, &ko);
    vial_init(); // reload tap dance / combos / key overrides from EEPROM
}

/* ------------------------------------------------------ Windows-only --
 * The vendor post-init (inside keyboard_post_init_user, defined by the board)
 * turns on layer 1 if "Mac mode" was ever saved - layer 1 is Lower here.
 * Force Windows mode first; the vendor code persists it with its settings.
 */
void keyboard_post_init_kb(void) {
    Keyboard_Info.Mac_Win_Mode = INIT_WIN_MODE;
    if (vial_data_reset) seed_key_overrides();
    keyboard_post_init_user();
}

/* ------------------------------------------------------------ Lock keys --
 * The board file owns process_record_user (it feeds the wireless library),
 * so handle this keymap's own keycodes after it, in process_record_kb.
 */
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) return false;
    switch (keycode) {
        case LLCK_LOWER:
            if (record->event.pressed) layer_lock_invert(_LOWER);
            return false;
        case LLCK_RAISE:
            if (record->event.pressed) layer_lock_invert(_RAISE);
            return false;
    }
    return true;
}

/* -------------------------------------------------------- Auto Num Lock --
 * Raise is a keypad: switch Num Lock on when it turns on. Only over USB -
 * whether the wireless link reports the host's Num Lock state back is
 * unknown, and guessing wrong would flip Num Lock off instead.
 */
layer_state_t layer_state_set_user(layer_state_t state) {
    if (Keyboard_Info.Key_Mode == QMK_USB_MODE && layer_state_cmp(state, _RAISE) && !layer_state_cmp(layer_state, _RAISE) &&
        !host_keyboard_led_state().num_lock) {
        tap_code(KC_NUM);
    }
    return state;
}

/* ------------------------------------------------- Per-key layer colours --
 * On any layer above Base: keys that do something light up in the
 * layer's colour, unused (KC_NO) keys go dark, transparent keys keep the
 * normal effect. The vendor's own indicators (connection, battery, ...) are
 * drawn afterwards, on top of this.
 */
static const uint8_t layer_hs[][2] = {
    // {hue, sat}, same colours as the Planck underglow
    [_LOWER]  = {85, 170},  // green
    [_RAISE]  = {170, 170}, // purple
    [_FN]     = {53, 153},  // lemon
    [_EXTRA]  = {20, 153},  // orange
    [_MOUSE]  = {0, 200},   // red
    [_KBDCFG] = {0, 0},     // white
};

#define INDICATOR_MIN_VAL 64 // keep layer colours visible at low brightness

bool rgb_matrix_indicators_user(void) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer >= _LOWER && layer < ARRAY_SIZE(layer_hs)) {
        uint8_t val = rgb_matrix_get_val();
        if (val < INDICATOR_MIN_VAL) val = INDICATOR_MIN_VAL;
        if (val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) val = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
        hsv_t hsv = {layer_hs[layer][0], layer_hs[layer][1], val};
        rgb_t rgb = hsv_to_rgb(hsv);

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uint8_t led = g_led_config.matrix_co[row][col];
                if (led == NO_LED) continue;
                uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col});
                if (kc == KC_TRNS) continue;
                if (kc == KC_NO) {
                    rgb_matrix_set_color(led, 0, 0, 0);
                } else {
                    rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
                }
            }
        }
    }

    // Caps Lock / Caps Word: light the Shift key white.
    if (host_keyboard_led_state().caps_lock || is_caps_word_on()) {
        uint8_t led = g_led_config.matrix_co[2][0];
        if (led != NO_LED) rgb_matrix_set_color(led, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
    return false;
}
