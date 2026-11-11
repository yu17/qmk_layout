ifeq ($(strip $(AUDIO_ENABLE)), yes)
    SRC += muse.c
endif

MOUSEKEY_ENABLE = yes
RGBLIGHT_ENABLE = yes
COMMAND_ENABLE = yes

CAPS_WORD_ENABLE = yes   # CW_TOGG on Lower/Raise
LAYER_LOCK_ENABLE = yes  # QK_LLCK on Lower/Raise

#POINTING_DEVICE_ENABLE = yes
#RAW_ENABLE = yes
