/* yu17 - Planck (Drop rev6, `planck/rev6_drop`) keymap configuration.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */
#pragma once

/* ---------------------------------------------------------------- Audio -- */
#ifdef AUDIO_ENABLE
#    define STARTUP_SONG SONG(PLANCK_SOUND)
// #define STARTUP_SONG SONG(NO_SOUND)
#endif

/* -------------------------------------------------------------- Encoder -- */
// Most tactile encoders have detents every 4 stages
#define ENCODER_RESOLUTION 4

/* ---------------------------------------------------------------- Mouse -- */
#define MOUSEKEY_DELAY 300
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_MAX_SPEED 4
#define MOUSEKEY_TIME_TO_MAX 20
#define MOUSEKEY_WHEEL_DELAY 300
#define MOUSEKEY_WHEEL_INTERVAL 100
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

/* ------------------------------------------------- RGB underglow (WS2812) --
 * 9 hand-soldered WS2812 LEDs on the PCB's underglow pads (data on A1, PWM
 * driver). LED count/pin/driver come from the keyboard's keyboard.json.
 *
 * Behaviour:
 *  - Base layer shows the normal RGBLight effect (default: static light cyan),
 *    adjustable from the Extra layer and saved to EEPROM.
 *  - Every other layer paints a solid colour over it (see keymap.c).
 *  - Layer colours are shown even while the base glow is switched off
 *    (UG_TOGG on Extra), so the indicator can never "silently die".
 *  - On power-up the strip blinks a rainbow test pattern 3x (also on the
 *    LED_TST key on Settings) - a quick hardware check for every LED.
 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYER_BLINK
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_MAX_LAYERS 16
#define RGBLIGHT_SLEEP // turn off while the host is suspended

#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT
#define RGBLIGHT_DEFAULT_HUE 116 // light cyan - the old base-layer colour
#define RGBLIGHT_DEFAULT_SAT 64
#define RGBLIGHT_DEFAULT_VAL 255

#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17
#define RGBLIGHT_LIMIT_VAL 255

// A few base-layer animations to cycle through with UG_NEXT (Extra).
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_BREATHE_CENTER 1.4
#define RGBLIGHT_EFFECT_BREATHE_MAX 255

/* ------------------------------------------------------------- Features -- */
// Layer Lock (Fn + Lower / Fn + Raise) has no idle timeout: a locked numpad
// must not drop out mid-game. Unlock with Fn + Space or the same lock key.

/* ------------------------------------------------------------------ USB -- */
// Claim little bus power so iPads/iPhones accept the keyboard.
// (Replaces the long-removed IOS_DEVICE_ENABLE option.)
#define USB_MAX_POWER_CONSUMPTION 90
