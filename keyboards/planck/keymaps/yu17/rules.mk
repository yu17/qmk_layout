ifeq ($(strip $(AUDIO_ENABLE)), yes)
    SRC += muse.c
endif

# Legacy Make Configuration
#SRC += muse.c
TERMINAL_ENABLE = yes
#POINTING_DEVICE_ENABLE = yes
MOUSEKEY_ENABLE = yes
RGBLIGHT_ENABLE = yes
#RAW_ENABLE = yes

COMMAND_ENABLE = yes