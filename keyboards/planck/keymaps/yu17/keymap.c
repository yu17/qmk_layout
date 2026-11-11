/* Copyright 2015-2021 Jack Humbert
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
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  PLOVER,
  BACKLIT,
  EXT_PLV,
  LIGHT_OFF,
  LIGHT_ON
};

#define LOWER MO(_1_LOWER)
#define RAISE MO(_2_RAISE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT | Lower|    Space    | Raise| Left |   Up | Down |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_0_BASE] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT ,
    KC_LCTL, LT(_3_FN, KC_APP), KC_LGUI, KC_LALT, MO(_1_LOWER), KC_SPC, KC_SPC, MO(_2_RAISE), KC_LEFT, KC_UP, KC_DOWN, KC_RGHT
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |      |      |      |      |      |      |      |      |   -  |   =  | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|      |      |      |      |      |      |      |   [  |   ]  |   \  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT |      |    Space    | Extra| Home | PgUp | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_1_LOWER] = LAYOUT_planck_grid(
    KC_GRV,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_BSPC,
    KC_ESC,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS,  KC_EQL,  KC_DEL,
    KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS,  KC_ENT,
    KC_LCTL, LT(_3_FN, KC_APP), KC_LGUI, KC_LALT, XXXXXXX, KC_SPC, KC_SPC, MO(_4_EXTRA), KC_HOME, KC_PGUP, KC_PGDN, KC_END
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |      |      |      |      |      |      |      |      |   _  |   +  | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|      |      |      |      |      |      |      |   {  |   }  |   |  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT | Extra|    Space    |      | Home | PgUp | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_2_RAISE] = LAYOUT_planck_grid(
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
    KC_ESC,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_UNDS, KC_PLUS, KC_DEL,
    KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LCBR, KC_RCBR, KC_PIPE, KC_ENT,
    KC_LCTL, LT(_3_FN, KC_APP), KC_LGUI, KC_LALT, MO(_4_EXTRA), KC_SPC, KC_SPC, XXXXXXX, KC_HOME, KC_PGUP, KC_PGDN, KC_END
),

/* Fn
 * ,-----------------------------------------------------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Mute | Vol- | Vol+ |      |      |      |      |      |      | ScrLk| Pause| PrScr|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift| Brit-| Brit+|      |      |      |      |      |      | Insrt| Del  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|      | LGUI | LALT |TG_LOW|   TO_BASE   |TG_RIS|      | RGUI | MENU |RCTRL |
 * `-----------------------------------------------------------------------------------'
 */
[_3_FN] = LAYOUT_planck_grid(
    KC_F1,     KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
    KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SCRL, KC_PAUS, KC_PSCR,
    KC_LSFT, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_INS,  KC_DEL, XXXXXXX,
    KC_LCTL, XXXXXXX, KC_LGUI, KC_LALT, TG(_1_LOWER), TO(_0_BASE), TO(_0_BASE), TG(_2_RAISE),  MU_TOGG, KC_RGUI, KC_MENU, KC_RCTL
),

/* Extra
 * ,-----------------------------------------------------------------------------------.
 * |  F13 |  F14 |  F15 |  F16 |  F17 |  F18 |  F19 |  F20 |  F21 |  F22 |  F23 |  F24 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |RGB_MP|RGB_MB|      |      |      |      |      |      |Audoff|Aud on|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|RGBTOG|RGBHUI|RGBSAI|RGBVAI|TermOf|TermOn|NKOFF | NKON |GUIOFF|GUION | Mouse|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl|      | LGUI | LALT |      |    Space    |      |      |      | Reset|      |
 * `-----------------------------------------------------------------------------------'
 */
//[_4_EXTRA] = LAYOUT_planck_grid(
//    KC_F13,   KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
//    KC_ESC,  LIGHT_OFF,  LIGHT_ON, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  AU_OFF,   AU_ON, XXXXXXX,
//    KC_LSFT, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI,TERM_OFF, TERM_ON,  NK_OFF,   NK_ON, GUI_OFF,  GUI_ON, TO(_5_MOUSE),
//    KC_LCTL, XXXXXXX, KC_LGUI, KC_LALT, XXXXXXX,  KC_SPC,  KC_SPC, XXXXXXX, XXXXXXX, XXXXXXX,   RESET, XXXXXXX
//),
[_4_EXTRA] = LAYOUT_planck_grid(
    KC_F13,   KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
    KC_ESC,LIGHT_OFF,LIGHT_ON, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  AU_OFF,   AU_ON, XXXXXXX,
    KC_LSFT, RGB_TOG, BL_DOWN,   BL_UP, BL_BRTG, XXXXXXX, XXXXXXX,  NK_OFF,   NK_ON,  GU_OFF,   GU_ON, TO(_5_MOUSE),
    KC_LCTL, XXXXXXX, KC_LGUI, KC_LALT, XXXXXXX,  KC_SPC,  KC_SPC, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT, XXXXXXX
),

/* Mouse
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |      | MsWhU|      |      |      |      |      |MsSpd0|MsSpd1|MsSpd2|ToBase|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  | MsWhL| MsWhD| MsWhR|      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|      |      |      |      |      |      |      | MS_1 | MS_3 | MS_2 |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LCtrl| FN/M | LGUI | LALT | MS_4 |    Space    | MS_5 | MS_L | MS_U | MS_D | MS_R |
 * `-----------------------------------------------------------------------------------'
 */
[_5_MOUSE] = LAYOUT_planck_grid(
    KC_TAB,  XXXXXXX, KC_WH_U, XXXXXXX,    KC_R,    KC_T,    KC_Y,    KC_U, KC_ACL0, KC_ACL1, KC_ACL2, TO(_0_BASE),
    KC_ESC,  KC_WH_L, KC_WH_D, KC_WH_R, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V, XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN1, KC_BTN3, KC_BTN2, XXXXXXX,
    KC_LCTL, LT(_3_FN, KC_APP), KC_LGUI, KC_LALT, KC_BTN4,  KC_SPC,  KC_SPC, KC_BTN5, KC_MS_L, KC_MS_U, KC_MS_D, KC_MS_R
),

/* KbdCfg
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|  Del |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|TermOn|TermOf|      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_6_KBDCFG] = LAYOUT_planck_grid(
    XXXXXXX,  QK_RBT, DB_TOGG, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD,  RGB_VAI, RGB_VAD, KC_DEL ,
    XXXXXXX, XXXXXXX, MU_NEXT,   AU_ON,  AU_OFF, AG_NORM, AG_SWAP,  QWERTY, COLEMAK,   DVORAK,  PLOVER, XXXXXXX,
    XXXXXXX, AU_PREV, AU_NEXT,   MU_ON,  MU_OFF,   MI_ON,  MI_OFF, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX
),

/* Plover layer (http://opensteno.org)
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

// How long (in milliseconds) to wait between animation steps for each of the "Solid color breathing" animations
const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM = {30, 20, 10, 5};
// How long (in milliseconds) to wait between animation steps for each of the "Cycling rainbow" animations
//const uint8_t RGBLED_RAINBOW_MOOD_INTERVALS[] PROGMEM = {120, 60, 30};
// How long (in milliseconds) to wait between animation steps for each of the "Swirling rainbow" animations
//const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {100, 50, 20};
// How long (in milliseconds) to wait between animation steps for each of the "Snake" animations
//const uint8_t RGBLED_SNAKE_INTERVALS[] PROGMEM = {100, 50, 20};
// How long (in milliseconds) to wait between animation steps for each of the "Knight" animations
//const uint8_t RGBLED_KNIGHT_INTERVALS[] PROGMEM = {127, 63, 31};
// How long (in milliseconds) to wait between animation steps for each of the "Twinkle" animations
const uint8_t RGBLED_TWINKLE_INTERVALS[] PROGMEM = {50, 25, 10};
// These control which hues are selected for each of the "Static gradient" modes
const uint8_t RGBLED_GRADIENT_RANGES[] PROGMEM = {255, 170, 127, 85, 64};

const rgblight_segment_t PROGMEM lit_0_solid_cyan[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 9, 116, 64, 255}//lighter than hsv_cyan
);

const rgblight_segment_t PROGMEM lit_1_solid_green[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 9, 85, 170, 255}//slightly ligher than hsv_green
);

const rgblight_segment_t PROGMEM lit_2_solid_purple[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 9, 170, 170, 255}//pueple-blue
);

const rgblight_segment_t PROGMEM lit_3_solid_lemon[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 9, 53, 153, 255}//lemon-yellow-green
);

const rgblight_segment_t PROGMEM lit_4_solid_orange[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 9, 20, 153, 255}//light than hsv_orange
);

const rgblight_segment_t* const PROGMEM backlit_layers[] = RGBLIGHT_LAYERS_LIST(
    lit_0_solid_cyan,
    lit_1_solid_green,
    lit_2_solid_purple,
    lit_3_solid_lemon,
    lit_4_solid_orange
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = backlit_layers;
}

// Additional operations each time led layer status being updated.

//bool led_update_user(led_t led_state) {
//    rgblight_set_layer_state(0, 1);
//    return true;
//}

#ifdef AUDIO_ENABLE
  float plover_song[][2]     = SONG(PLOVER_SOUND);
  float plover_gb_song[][2]  = SONG(PLOVER_GOODBYE_SOUND);
#endif

// Operations each time layer state being updated.

layer_state_t layer_state_set_user(layer_state_t state) {
  rgblight_set_layer_state(0, layer_state_cmp(state, 0));
  rgblight_set_layer_state(1, layer_state_cmp(state, 1));
  rgblight_set_layer_state(2, layer_state_cmp(state, 2));
  rgblight_set_layer_state(3, layer_state_cmp(state, 3));
  rgblight_set_layer_state(4, layer_state_cmp(state, 4));
  return update_tri_layer_state(state, _1_LOWER, _2_RAISE, _6_KBDCFG);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(_0_BASE);
      }
      return false;
      break;
    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
        #ifdef KEYBOARD_planck_rev5
          writePinLow(E6);
        #endif
      } else {
        unregister_code(KC_RSFT);
        #ifdef KEYBOARD_planck_rev5
          writePinHigh(E6);
        #endif
      }
      return false;
      break;
    case PLOVER:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          stop_all_notes();
          PLAY_SONG(plover_song);
        #endif
        layer_off(_2_RAISE);
        layer_off(_1_LOWER);
        layer_off(_6_KBDCFG);
        layer_on(_PLOVER);
        if (!eeconfig_is_enabled()) {
            eeconfig_init();
        }
        keymap_config.raw = eeconfig_read_keymap();
        keymap_config.nkro = 1;
        eeconfig_update_keymap(keymap_config.raw);
      }
      return false;
      break;
    case EXT_PLV:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(plover_gb_song);
        #endif
        layer_off(_PLOVER);
      }
      return false;
      break;
    case LIGHT_OFF:
      rgblight_disable();
      break;
    case LIGHT_ON:
      rgblight_enable();
      break;
  }
  return true;
}

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

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
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_PGDN);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_PGUP);
      #endif
    }
  }
    return true;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_song); }
#endif
                layer_on(_6_KBDCFG);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_gb_song); }
#endif
                layer_off(_6_KBDCFG);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
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
    case LT(_3_FN, KC_APP):
    case MO(_3_FN):
      return false;
    default:
      return true;
  }
}
