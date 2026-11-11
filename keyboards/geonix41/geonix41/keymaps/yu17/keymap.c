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
 * | LCtrl|  Fn  | LGUI | LALT | Lower| Space| Space| Raise| Left | Down |  Up  |Right |
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
 * | LCtrl|  Fn  | LGUI | LALT |      | Space| Space| Extra| Home | PgDn | PgUp | End  |
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
 * | LCtrl|  Fn  | LGUI | LALT | Extra| Space| Space|(held)|  0   |  .   |  /   |Enter |
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
 * | Mute | Vol- | Vol+ | Prev | Play | Next | Stop |      |      | PrScr| ScrLk| Pause|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift| Brit-| Brit+|      |      |      |      |      |      | Insrt| Del  |RShift|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|      | LGUI | LALT |LckLow|TO_BAS|TO_BAS|LckRai| RALT | RGUI | MENU |RCTRL |
 * `-----------------------------------------------------------------------------------'
 */
[_FN] = LAYOUT_tkl_ansi(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
    KC_MUTE, KC_VOLD, KC_VOLU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MSTP, XXXXXXX, XXXXXXX, KC_PSCR, KC_SCRL, KC_PAUS,
    KC_LSFT, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_INS,  KC_DEL,  KC_RSFT,
    KC_LCTL, XXXXXXX, KC_LGUI, KC_LALT, LLCK_LOWER, TO(_BASE), TO(_BASE), LLCK_RAISE, KC_RALT, KC_RGUI, KC_MENU, KC_RCTL
),

/* Extra  (Lower + Raise): F13-F24 and all lighting                      LED: orange
 * Key lights on the left, ambient (side/logo) strip on the right.
 * ,-----------------------------------------------------------------------------------.
 * |  F13 |  F14 |  F15 |  F16 |  F17 |  F18 |  F19 |  F20 |  F21 |  F22 |  F23 |  F24 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |LgtTog|Brgt+ | Spd+ | Eff+ | Hue+ | Sat+ |AmbTog|AmbBr+|AmbEf+|AmbHu+|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|LEDtst|Brgt- | Spd- | Eff- | Hue- | Sat- |      |AmbBr-|AmbEf-|AmbHu-| Mouse|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|Settng| LGUI | LALT |      | Space| Space|      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 *   LgtTog = all lights on/off (vendor RGB_RTOG). LEDtst = vendor colour test.
 *   Settng is a hold key: Lower + Raise + Fn -> Settings layer.
 */
[_EXTRA] = LAYOUT_tkl_ansi(
    KC_F13,  KC_F14,   KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,   KC_F21,    KC_F22,   KC_F23,   KC_F24,
    KC_ESC,  RGB_RTOG, UG_VALU, UG_SPDU, UG_NEXT, UG_HUEU, UG_SATU, LOGO_TOG, LOGO_VAI,  LOGO_MOD, LOGO_HUI, XXXXXXX,
    KC_LSFT, TEST_CL,  UG_VALD, UG_SPDD, UG_PREV, UG_HUED, UG_SATD, XXXXXXX,  LOGO_VAD,  LOGO_RMOD, LOGO_HUD, TO(_MOUSE),
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
 * | LCtrl|  Fn  | LGUI | LALT | MS_4 | Space| Space| MS_5 | MS_L | MS_D | MS_U | MS_R |
 * `-----------------------------------------------------------------------------------'
 */
[_MOUSE] = LAYOUT_tkl_ansi(
    KC_TAB,  XXXXXXX, MS_WHLU, XXXXXXX, KC_R,    KC_T,    KC_Y,    KC_U,    MS_ACL0, MS_ACL1, MS_ACL2, TO(_BASE),
    KC_ESC,  MS_WHLL, MS_WHLD, MS_WHLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    XXXXXXX, XXXXXXX, XXXXXXX, MS_BTN1, MS_BTN3, MS_BTN2, XXXXXXX,
    KC_LCTL, FN_APP,  KC_LGUI, KC_LALT, MS_BTN4, KC_SPC,  KC_SPC,  MS_BTN5, MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT
),

/* Settings / KbdCfg  (hold Lower + Raise + Fn)                           LED: white
 * Keyboard configuration only - lighting lives on Extra. Wireless keys sit
 * where the stock firmware has them (Fn + Tab/Q/W/E/R), so the vendor's
 * pairing / connection indicators line up.
 * ,-----------------------------------------------------------------------------------.
 * | USB  | BT1  | BT2  | BT3  | 2.4G |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Batt |Sleep |LowLat|      |      |      | NKRO |      |WinOff| WinOn|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |Reset3s      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 *   BT1-3 / 2.4G: tap = connect, hold = pair (vendor behaviour).
 *   Batt: battery level. Sleep: cycle wireless sleep timer (T flashes).
 *   LowLat: hold 3 s = 2.4 GHz low-latency toggle. Reset3s: hold 3 s = factory reset.
 *   NKRO: vendor 6-key / all-key toggle (QMK's NK_ON/NK_OFF would be undone by
 *   the vendor library at the next power-up).
 */
[_KBDCFG] = LAYOUT_tkl_ansi(
    MD_USB,  MD_BLE1, MD_BLE2, MD_BLE3, MD_24G,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, QK_BAT,  TIME_ST, QK_DEB,  XXXXXXX, XXXXXXX, XXXXXXX, SIX_N,   XXXXXXX, GU_OFF,  GU_ON,   XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, U_EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
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

/* ------------------------------------------------------ Per-key colours --
 * Base layer: the normal RGB effect (plus the Caps indicator).
 * Any other layer: every key shows what it does (palette below), keys that
 * fall through to a lower layer show that key's colour dimmed, unused keys
 * are dark, and both Space keys show the layer's colour. A few keys show live
 * state: Num Lock, the active connection, NKRO, Win key on/off.
 * The vendor's own indicators (pairing, battery, ...) are drawn on top.
 */
enum key_class {
    C_OFF, C_TYPE, C_MOD, C_LAYER, C_NAV, C_FKEY, C_VOLUME, C_BRIGHT, C_LIGHT, C_AMBIENT,
    C_WIRELESS, C_SYSTEM, C_FEATURE, C_MOUSE, C_NUMPAD,
    // live state
    C_NUM_OFF, C_CONN_ACTIVE, C_NKRO_ALL, C_NKRO_6, C_WIN_ON, C_WIN_OFF,
    C_COUNT
};

// {hue, sat, brightness %} - QMK 0-255 hue. Low saturation reads as pastel;
// tune here. Hues follow the layer map page (docs/layout.html).
static const uint8_t palette[C_COUNT][3] = {
    [C_OFF]         = {0, 0, 0},
    [C_TYPE]        = {150, 25, 70},  // typing keys: soft cool white
    [C_MOD]         = {150, 70, 75},  // modifiers: slate
    [C_LAYER]       = {160, 130, 100}, // layer keys: periwinkle
    [C_NAV]         = {120, 120, 100}, // navigation: mint/teal
    [C_FKEY]        = {29, 110, 100},  // F-keys: sand
    [C_VOLUME]      = {85, 120, 100},  // volume/media: green
    [C_BRIGHT]      = {55, 140, 100},  // screen brightness: yellow-green
    [C_LIGHT]       = {225, 110, 100}, // key lighting: pink
    [C_AMBIENT]     = {200, 120, 100}, // ambient strip: orchid
    [C_WIRELESS]    = {21, 130, 100},  // wireless/battery: apricot
    [C_SYSTEM]      = {0, 130, 100},   // system/reset: rose
    [C_FEATURE]     = {185, 110, 100}, // Caps Word: lavender
    [C_MOUSE]       = {140, 120, 100}, // mouse: sky
    [C_NUMPAD]      = {38, 120, 100},  // keypad (Num Lock on): butter
    [C_NUM_OFF]     = {18, 230, 55},   // Num Lock off: brown
    [C_CONN_ACTIVE] = {85, 255, 100},  // active connection: green
    [C_NKRO_ALL]    = {120, 200, 100}, // all-key rollover: teal
    [C_NKRO_6]      = {21, 220, 100},  // 6-key rollover: orange
    [C_WIN_ON]      = {85, 220, 100},  // Win key enabled (lights GU_ON)
    [C_WIN_OFF]     = {0, 220, 100},   // Win key disabled (lights GU_OFF)
};

static const uint8_t layer_hs[][2] = {
    // {hue, sat} of the Space-key layer indicator
    [_LOWER]  = {85, 170},  // green
    [_RAISE]  = {170, 170}, // purple
    [_FN]     = {53, 153},  // lemon
    [_EXTRA]  = {20, 153},  // orange
    [_MOUSE]  = {0, 200},   // red
    [_KBDCFG] = {0, 0},     // white
};

#define INDICATOR_MIN_VAL 64 // keep indicators visible at low brightness
#define FALLTHROUGH_PCT 45   // keys falling through to a lower layer

static uint8_t classify(uint16_t kc) {
    switch (kc) {
        case KC_NO: return C_OFF;
        case KC_NUM_LOCK: return host_keyboard_led_state().num_lock ? C_NUMPAD : C_NUM_OFF;
        case MD_USB: return Keyboard_Info.Key_Mode == QMK_USB_MODE ? C_CONN_ACTIVE : C_WIRELESS;
        case MD_24G: return Keyboard_Info.Key_Mode == QMK_2P4G_MODE ? C_CONN_ACTIVE : C_WIRELESS;
        case MD_BLE1: case MD_BLE2: case MD_BLE3:
            return (Keyboard_Info.Key_Mode == QMK_BLE_MODE && Keyboard_Info.Ble_Channel == kc - MD_BLE1 + QMK_BLE_CHANNEL_1) ? C_CONN_ACTIVE : C_WIRELESS;
        case SIX_N: return keymap_config.nkro ? C_NKRO_ALL : C_NKRO_6;
        case GU_ON: return keymap_config.no_gui ? C_OFF : C_WIN_ON;
        case GU_OFF: return keymap_config.no_gui ? C_WIN_OFF : C_OFF;
        case QK_BAT: case QK_DEB: case TIME_ST: case TIME_DT:
        case QMK_KB_2P4G_PAIR: case QMK_KB_BLE1_PAIR: case QMK_KB_BLE2_PAIR:
            return C_WIRELESS;
        case U_EE_CLR: case QK_WLO: case KC_UC: return C_SYSTEM;
        case RGB_RTOG: case TEST_CL: return C_LIGHT;
        case CW_TOGG: case QK_LLCK: return C_FEATURE;
        case TL_LOWR: case TL_UPPR: case LLCK_LOWER: case LLCK_RAISE: return C_LAYER;
        case KC_BRIGHTNESS_UP: case KC_BRIGHTNESS_DOWN: return C_BRIGHT;
        case KC_RIGHT: case KC_LEFT: case KC_DOWN: case KC_UP:
        case KC_HOME: case KC_END: case KC_PAGE_UP: case KC_PAGE_DOWN:
            return C_NAV;
        case QK_BOOT: case QK_REBOOT: case EE_CLR: case DB_TOGG: return C_SYSTEM;
    }
    if (kc >= LOGO_TOG && kc <= LOGO_SPD) return C_AMBIENT;
    if (IS_UNDERGLOW_KEYCODE(kc) || IS_RGB_MATRIX_KEYCODE(kc)) return C_LIGHT;
    if (IS_QK_MAGIC(kc)) return C_SYSTEM;
    if (IS_MOUSE_KEYCODE(kc)) return C_MOUSE;
    if (IS_MODIFIER_KEYCODE(kc)) return C_MOD;
    if (IS_QK_MOMENTARY(kc) || IS_QK_TO(kc) || IS_QK_TOGGLE_LAYER(kc) || IS_QK_LAYER_TAP(kc) || IS_QK_ONE_SHOT_LAYER(kc) ||
        IS_QK_LAYER_TAP_TOGGLE(kc) || IS_QK_DEF_LAYER(kc) || IS_QK_LAYER_MOD(kc))
        return C_LAYER;
    if ((kc >= KC_KP_SLASH && kc <= KC_KP_DOT) || kc == KC_KP_EQUAL) return host_keyboard_led_state().num_lock ? C_NUMPAD : C_NUM_OFF;
    if ((kc >= KC_F1 && kc <= KC_F12) || (kc >= KC_F13 && kc <= KC_F24)) return C_FKEY;
    if (kc >= KC_AUDIO_MUTE && kc <= KC_MEDIA_REWIND) return C_VOLUME;
    if (IS_QK_KB(kc)) return C_SYSTEM; // other vendor keys
    return C_TYPE;
}

// The key a position resolves to on the active layers (following _______).
static uint16_t resolve(uint8_t layer, keypos_t pos, bool *fell_through) {
    layer_state_t layers = layer_state | default_layer_state;
    *fell_through = false;
    for (int8_t l = layer; l >= 0; l--) {
        if (l != 0 && !(layers & ((layer_state_t)1 << l))) continue;
        uint16_t kc = keymap_key_to_keycode(l, pos);
        if (kc != KC_TRNS) return kc;
        *fell_through = true;
    }
    return KC_NO;
}

static rgb_t   key_rgb[MATRIX_ROWS][MATRIX_COLS];
static uint8_t key_layer = 0;

// Light-switch keys (key lights on/off, ambient on/off) on the current layer,
// so they can be shown even while the lights are switched off.
#define MAX_SWITCH_KEYS 4
static keypos_t switch_keys[MAX_SWITCH_KEYS];
static uint8_t  n_switch_keys = 0;

typedef struct {
    layer_state_t layers;
    uint8_t       leds, mode, channel, val;
    uint16_t      kcfg;
} indicator_sig_t;

static void refresh_colours(uint8_t layer, uint8_t val) {
    n_switch_keys = 0;
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            bool     dim;
            uint16_t kc  = resolve(layer, (keypos_t){.row = row, .col = col}, &dim);
            uint8_t  cls = classify(kc);
            if ((kc == RGB_RTOG || kc == LOGO_TOG) && n_switch_keys < MAX_SWITCH_KEYS) {
                switch_keys[n_switch_keys++] = (keypos_t){.row = row, .col = col};
            }
            uint16_t v   = (uint16_t)val * palette[cls][2] / 100;
            if (dim) v = v * FALLTHROUGH_PCT / 100;
            // Space keys show the layer (dangerous keys keep their own colour).
            if (row == 3 && (col == 5 || col == 6) && cls != C_SYSTEM && layer < ARRAY_SIZE(layer_hs)) {
                key_rgb[row][col] = hsv_to_rgb((hsv_t){layer_hs[layer][0], layer_hs[layer][1], val});
            } else {
                key_rgb[row][col] = hsv_to_rgb((hsv_t){palette[cls][0], palette[cls][1], (uint8_t)v});
            }
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer >= _LOWER) {
        uint8_t val = rgb_matrix_get_val();
        if (val < INDICATOR_MIN_VAL) val = INDICATOR_MIN_VAL;
        if (val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) val = RGB_MATRIX_MAXIMUM_BRIGHTNESS;

        // Recompute only when something shown changes (or every 250 ms, to
        // pick up keymap edits made in Vial).
        static indicator_sig_t last;
        static uint32_t        last_time;
        indicator_sig_t        sig;
        memset(&sig, 0, sizeof(sig)); // zero padding too: compared with memcmp
        sig.layers  = layer_state | default_layer_state;
        sig.leds    = host_keyboard_led_state().raw;
        sig.mode    = Keyboard_Info.Key_Mode;
        sig.channel = Keyboard_Info.Ble_Channel;
        sig.val     = val;
        sig.kcfg    = keymap_config.raw;
        if (layer != key_layer || memcmp(&sig, &last, sizeof(sig)) != 0 || timer_elapsed32(last_time) > 250) {
            refresh_colours(layer, val);
            key_layer = layer;
            last      = sig;
            last_time = timer_read32();
        }
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uint8_t led = g_led_config.matrix_co[row][col];
                if (led != NO_LED) rgb_matrix_set_color(led, key_rgb[row][col].r, key_rgb[row][col].g, key_rgb[row][col].b);
            }
        }
    }

    // Caps Lock / Caps Word: light the Shift key white (all layers).
    if (host_keyboard_led_state().caps_lock || is_caps_word_on()) {
        uint8_t led = g_led_config.matrix_co[2][0];
        if (led != NO_LED) rgb_matrix_set_color(led, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
    return false;
}

/* --------------------------------------------- Light switches while off --
 * With the lights switched off (RGB_RTOG), the vendor code paints every LED
 * black after our indicators and sets Led_Point_Sleep, which makes the LED
 * driver cut LED power for the frame. While Extra is held, draw just the two
 * light-switch keys after the vendor code and keep LED power on for this
 * frame. The vendor code recomputes Led_Point_Sleep every frame, so this is
 * temporary by construction and nothing is saved.
 */
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    rgb_matrix_indicators_advanced_user(led_min, led_max); // vendor: status LEDs, blanking when off
    if (Keyboard_Info.Led_On_Off == INIT_LED_OFF && get_highest_layer(layer_state) == _EXTRA && key_layer == _EXTRA) {
        for (uint8_t i = 0; i < n_switch_keys; i++) {
            uint8_t led = g_led_config.matrix_co[switch_keys[i].row][switch_keys[i].col];
            rgb_t   c   = key_rgb[switch_keys[i].row][switch_keys[i].col];
            if (led != NO_LED) rgb_matrix_set_color(led, c.r, c.g, c.b);
        }
        Led_Point_Sleep = false;
    }
    return false;
}
