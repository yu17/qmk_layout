ifeq ($(strip $(AUDIO_ENABLE)), yes)
    SRC += muse.c
endif

MOUSEKEY_ENABLE = yes
RGBLIGHT_ENABLE = yes
COMMAND_ENABLE = yes

CAPS_WORD_ENABLE = yes     # CW_TOGG on Lower
LAYER_LOCK_ENABLE = yes    # Fn + Lower / Fn + Raise lock the layer
KEY_OVERRIDE_ENABLE = yes  # Shift + Backspace = Delete

#POINTING_DEVICE_ENABLE = yes
#RAW_ENABLE = yes
