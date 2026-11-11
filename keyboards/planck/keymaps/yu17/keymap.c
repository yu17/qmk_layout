/* Copyright 2015-2021 Jack Humbert
 * Customised by yu17.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#ifdef AUDIO_ENABLE
#    include "muse.h"
#endif

enum planck_layers {
    _0_BASE,
    _1_LOWER,
    _2_RAISE,
    _3_FN,
    _4_EXTRA,
    _5_MOUSE,
    _6_KBDCFG,
    _PLOVER,
};

enum planck_keycodes {
    PLOVER = SAFE_RANGE,
    EXT_PLV,
    LED_TST,   // blink the rainbow LED test pattern
    LLCK_LOWER, // Fn + Lower: lock/unlock the Lower layer
    LLCK_RAISE, // Fn + Raise: lock/unlock the Raise layer
};

#define LOWER MO(_1_LOWER)
#define RAISE MO(_2_RAISE)
#define FN_APP LT(_3_FN, KC_APP)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty                                                                  LED: base effect
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|  Fn  | LGUI | LALT | Lower|    Space    | Raise| Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_0_BASE] = LAYOUT_planck_grid(
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
 * | LCtrl|  Fn  | LGUI | LALT |      |    Space    | Extra| Home | PgDn | PgUp | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_1_LOWER] = LAYOUT_planck_grid(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_ESC,  CW_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS, KC_EQL,  XXXXXXX,
    KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS, KC_ENT,
    KC_LCTL, FN_APP,  KC_LGUI, KC_LALT, XXXXXXX, KC_SPC,  KC_SPC,  MO(_4_EXTRA), KC_HOME, KC_PGDN, KC_PGUP, KC_END
),

/* Raise  (numpad; left half = normal keys, e.g. WASD for games)          LED: purple
 * Lock it with Fn + Raise; Fn + Space (or Fn + Raise again) unlocks.
 * Keypad codes (KC_P*), so games/sims see real numpad keys; Num Lock is
 * switched on automatically when this layer turns on.
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |      |NumLck|  7   |  8   |  9   |  *   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |  Up  |      |  4   |  5   |  6   |  -   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  | Left | Down |Right |  1   |  2   |  3   |  +   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|  Fn  | LGUI | LALT | Extra|    Space    |(held)|  0   |  .   |  /   |Enter |
 * `-----------------------------------------------------------------------------------'
 */
[_2_RAISE] = LAYOUT_planck_grid(
    _______, _______, _______, _______, _______, _______, XXXXXXX, KC_NUM,  KC_P7,   KC_P8,   KC_P9,   KC_PAST,
    _______, _______, _______, _______, _______, _______, KC_UP,   XXXXXXX, KC_P4,   KC_P5,   KC_P6,   KC_PMNS,
    _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_P1,   KC_P2,   KC_P3,   KC_PPLS,
    _______, _______, _______, _______, MO(_4_EXTRA), _______, _______, _______, KC_P0, KC_PDOT, KC_PSLS, KC_PENT
),

/* Fn                                                                      LED: lemon
 * ,-----------------------------------------------------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Mute | Vol- | Vol+ | Prev | Play | Next | Stop |      |      | PrScr| ScrLk| Pause|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift| Brit-| Brit+|      |      |      |      |      |      | Insrt| Del  |RShift|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|      | LGUI | LALT |LckLow|   TO_BASE   |LckRai| RALT | RGUI | MENU |RCTRL |
 * `-----------------------------------------------------------------------------------'
 */
[_3_FN] = LAYOUT_planck_grid(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
    KC_MUTE, KC_VOLD, KC_VOLU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MSTP, XXXXXXX, XXXXXXX, KC_PSCR, KC_SCRL, KC_PAUS,
    KC_LSFT, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_INS,  KC_DEL,  KC_RSFT,
    KC_LCTL, XXXXXXX, KC_LGUI, KC_LALT, LLCK_LOWER, TO(_0_BASE), TO(_0_BASE), LLCK_RAISE, KC_RALT, KC_RGUI, KC_MENU, KC_RCTL
),

/* Extra  (Lower + Raise): F13-F24 and all lighting                      LED: orange
 * ,-----------------------------------------------------------------------------------.
 * |  F13 |  F14 |  F15 |  F16 |  F17 |  F18 |  F19 |  F20 |  F21 |  F22 |  F23 |  F24 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |LgtTog|Brgt+ | Spd+ | Eff+ | Hue+ | Sat+ |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|LEDtst|Brgt- | Spd- | Eff- | Hue- | Sat- |      |      |      |      | Mouse|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|Settng| LGUI | LALT |      |    Space    |      |MusTog|      |      |      |
 * `-----------------------------------------------------------------------------------'
 *   LgtTog = base glow on/off (saved; layer colours keep working).
 *   MusTog = music mode (keys play notes); toggle it off here again.
 *   Settng is a hold key: Lower + Raise + Fn -> Settings layer.
 */
[_4_EXTRA] = LAYOUT_planck_grid(
    KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
    KC_ESC,  UG_TOGG, UG_VALU, UG_SPDU, UG_NEXT, UG_HUEU, UG_SATU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LSFT, LED_TST, UG_VALD, UG_SPDD, UG_PREV, UG_HUED, UG_SATD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(_5_MOUSE),
    KC_LCTL, MO(_6_KBDCFG), KC_LGUI, KC_LALT, XXXXXXX, KC_SPC, KC_SPC, XXXXXXX, MU_TOGG, XXXXXXX, XXXXXXX, XXXXXXX
),

/* Mouse  (Extra -> Mouse; leave with ToBase)                              LED: red
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |      | MsWhU|      |   R  |   T  |   Y  |   U  |MsSpd0|MsSpd1|MsSpd2|ToBase|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  | MsWhL| MsWhD| MsWhR|      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |      |      |      | MS_1 | MS_3 | MS_2 |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|  Fn  | LGUI | LALT | MS_4 |    Space    | MS_5 | MS_L | MS_D | MS_U | MS_R |
 * `-----------------------------------------------------------------------------------'
 */
[_5_MOUSE] = LAYOUT_planck_grid(
    KC_TAB,  XXXXXXX, MS_WHLU, XXXXXXX, KC_R,    KC_T,    KC_Y,    KC_U,    MS_ACL0, MS_ACL1, MS_ACL2, TO(_0_BASE),
    KC_ESC,  MS_WHLL, MS_WHLD, MS_WHLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    XXXXXXX, XXXXXXX, XXXXXXX, MS_BTN1, MS_BTN3, MS_BTN2, XXXXXXX,
    KC_LCTL, FN_APP,  KC_LGUI, KC_LALT, MS_BTN4, KC_SPC,  KC_SPC,  MS_BTN5, MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT
),

/* Settings / KbdCfg  (hold Lower + Raise + Fn, or DIP switch 0)         LED: white
 * Keyboard configuration only - lighting lives on Extra.
 * ,-----------------------------------------------------------------------------------.
 * |      |Reboot|Debug |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|      |      |      |Plover|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|      |      |NKOFF | NKON |WinOff| WinOn|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      | Boot |EEPclr|
 * `-----------------------------------------------------------------------------------'
 *   Boot = bootloader (for flashing). EEPclr = reset all saved settings.
 */
[_6_KBDCFG] = LAYOUT_planck_grid(
    XXXXXXX, QK_RBT,  DB_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, MU_NEXT, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, XXXXXXX, XXXXXXX, XXXXXXX, PLOVER,  XXXXXXX,
    XXXXXXX, AU_PREV, AU_NEXT, MU_ON,   MU_OFF,  XXXXXXX, XXXXXXX, NK_OFF,  NK_ON,   GU_OFF,  GU_ON,   XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT, EE_CLR
),

/* Plover layer (http://opensteno.org)                                     LED: magenta
 * ,-----------------------------------------------------------------------------------.
 * |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   S  |   T  |   P  |   H  |   *  |   *  |   F  |   P  |   L  |   T  |   D  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   S  |   K  |   W  |   R  |   *  |   *  |   R  |   B  |   G  |   S  |   Z  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Exit |      |      |   A  |   O  |             |   E  |   U  |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_PLOVER] = LAYOUT_planck_grid(
    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1   ,
    XXXXXXX, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,
    XXXXXXX, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    EXT_PLV, XXXXXXX, XXXXXXX, KC_C,    KC_V,    XXXXXXX, XXXXXXX, KC_N,    KC_M,    XXXXXXX, XXXXXXX, XXXXXXX
)

};
// clang-format on

/* ---------------------------------------------------------- Key overrides -- */
// Shift + Backspace = Delete (Del is no longer on Lower).
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);
const key_override_t *key_overrides[] = {&delete_key_override};

/* ------------------------------------------------------ Underglow layers --
 * The base layer has no entry: it shows the regular RGBLight effect.
 * Order matters - later entries are drawn on top, so the highest active
 * keymap layer wins. HSV values are QMK's 0-255 scale.
 */
const rgblight_segment_t PROGMEM lit_lower_green[]   = RGBLIGHT_LAYER_SEGMENTS({0, RGBLIGHT_LED_COUNT, 85, 170, 255});  // slightly lighter than HSV_GREEN
const rgblight_segment_t PROGMEM lit_raise_purple[]  = RGBLIGHT_LAYER_SEGMENTS({0, RGBLIGHT_LED_COUNT, 170, 170, 255}); // purple-blue
const rgblight_segment_t PROGMEM lit_fn_lemon[]      = RGBLIGHT_LAYER_SEGMENTS({0, RGBLIGHT_LED_COUNT, 53, 153, 255});  // lemon-yellow-green
const rgblight_segment_t PROGMEM lit_extra_orange[]  = RGBLIGHT_LAYER_SEGMENTS({0, RGBLIGHT_LED_COUNT, 20, 153, 255});  // lighter than HSV_ORANGE
const rgblight_segment_t PROGMEM lit_mouse_red[]     = RGBLIGHT_LAYER_SEGMENTS({0, RGBLIGHT_LED_COUNT, 0, 200, 255});   // "you're in mouse mode"
const rgblight_segment_t PROGMEM lit_kbdcfg_white[]  = RGBLIGHT_LAYER_SEGMENTS({0, RGBLIGHT_LED_COUNT, 0, 0, 160});     // white, dimmed
const rgblight_segment_t PROGMEM lit_plover_magenta[] = RGBLIGHT_LAYER_SEGMENTS({0, RGBLIGHT_LED_COUNT, 213, 200, 255});
// Test pattern: every LED a different hue, so a dead LED (or a broken data
// link - everything after it goes dark) is easy to spot.
const rgblight_segment_t PROGMEM lit_test_rainbow[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, 0, 255, 160}, {1, 1, 28, 255, 160}, {2, 1, 57, 255, 160},
    {3, 1, 85, 255, 160}, {4, 1, 113, 255, 160}, {5, 1, 142, 255, 160},
    {6, 1, 170, 255, 160}, {7, 1, 198, 255, 160}, {8, 1, 227, 255, 160}
);

enum underglow_layers { // index into underglow_layers[] below
    LIT_LOWER,
    LIT_RAISE,
    LIT_FN,
    LIT_EXTRA,
    LIT_MOUSE,
    LIT_KBDCFG,
    LIT_PLOVER,
    LIT_TEST,
};

const rgblight_segment_t *const PROGMEM underglow_layers[] = RGBLIGHT_LAYERS_LIST(
    [LIT_LOWER]  = lit_lower_green,
    [LIT_RAISE]  = lit_raise_purple,
    [LIT_FN]     = lit_fn_lemon,
    [LIT_EXTRA]  = lit_extra_orange,
    [LIT_MOUSE]  = lit_mouse_red,
    [LIT_KBDCFG] = lit_kbdcfg_white,
    [LIT_PLOVER] = lit_plover_magenta,
    [LIT_TEST]   = lit_test_rainbow
);

#define LED_TEST_BLINK() rgblight_blink_layer_repeat(LIT_TEST, 300, 3)

void keyboard_post_init_user(void) {
    rgblight_layers = underglow_layers;
    LED_TEST_BLINK(); // power-on self test
}

#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    // Raise is a keypad: make sure Num Lock is on when it turns on.
    if (layer_state_cmp(state, _2_RAISE) && !layer_state_cmp(layer_state, _2_RAISE) && !host_keyboard_led_state().num_lock) {
        tap_code(KC_NUM);
    }
    rgblight_set_layer_state(LIT_LOWER, layer_state_cmp(state, _1_LOWER));
    rgblight_set_layer_state(LIT_RAISE, layer_state_cmp(state, _2_RAISE));
    rgblight_set_layer_state(LIT_FN, layer_state_cmp(state, _3_FN));
    rgblight_set_layer_state(LIT_EXTRA, layer_state_cmp(state, _4_EXTRA));
    rgblight_set_layer_state(LIT_MOUSE, layer_state_cmp(state, _5_MOUSE));
    rgblight_set_layer_state(LIT_KBDCFG, layer_state_cmp(state, _6_KBDCFG));
    rgblight_set_layer_state(LIT_PLOVER, layer_state_cmp(state, _PLOVER));
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case PLOVER:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(plover_song);
#endif
                layer_off(_2_RAISE);
                layer_off(_1_LOWER);
                layer_off(_4_EXTRA);
                layer_off(_6_KBDCFG);
                layer_on(_PLOVER);
                if (!eeconfig_is_enabled()) {
                    eeconfig_init();
                }
                eeconfig_read_keymap(&keymap_config);
                keymap_config.nkro = 1;
                eeconfig_update_keymap(&keymap_config);
            }
            return false;
        case EXT_PLV:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                PLAY_SONG(plover_gb_song);
#endif
                layer_off(_PLOVER);
            }
            return false;
        case LED_TST:
            if (record->event.pressed) {
                LED_TEST_BLINK();
            }
            return false;
        case LLCK_LOWER:
            if (record->event.pressed) {
                layer_lock_invert(_1_LOWER);
            }
            return false;
        case LLCK_RAISE:
            if (record->event.pressed) {
                layer_lock_invert(_2_RAISE);
            }
            return false;
    }
    return true;
}

/* -------------------------------------------------- Encoder / DIP / Muse -- */
bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (muse_mode) {
        if (IS_LAYER_ON(_2_RAISE)) {
            if (clockwise) {
                muse_offset++;
            } else {
                muse_offset--;
            }
        } else {
            if (clockwise) {
                muse_tempo += 1;
            } else {
                muse_tempo -= 1;
            }
        }
    } else {
        if (clockwise) {
#ifdef MOUSEKEY_ENABLE
            tap_code(MS_WHLD);
#else
            tap_code(KC_PGDN);
#endif
        } else {
#ifdef MOUSEKEY_ENABLE
            tap_code(MS_WHLU);
#else
            tap_code(KC_PGUP);
#endif
        }
    }
    // Handled here - don't let the keyboard-level handler send PgUp/PgDn or
    // volume on top of it.
    return false;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_song);
                }
#endif
                layer_on(_6_KBDCFG);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_gb_song);
                }
#endif
                layer_off(_6_KBDCFG);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            muse_mode = active;
            break;
    }
    return true;
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        // Layer keys keep working in music mode, so Extra (Lower + Raise ->
        // MusTog) and Settings stay reachable to switch it off again.
        case FN_APP:
        case MO(_3_FN):
        case LOWER:
        case RAISE:
        case MO(_4_EXTRA):
        case MO(_6_KBDCFG):
            return false;
        default:
            return true;
    }
}
