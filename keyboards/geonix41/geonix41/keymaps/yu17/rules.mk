VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes
EEPROM_DRIVER = custom
RAW_ENABLE = yes

# Vendor header lib/rdr_lib/rdr_common.h (wireless/battery/logo-LED blob API).
VPATH += $(LIB_PATH)/rdr_lib

# GCC >= 16 warns about the vendor's ES32 CMSIS header guards; the fork
# builds with -Werror. Silence just that warning (vendor code, not ours).
EXTRAFLAGS += -Wno-header-guard

TRI_LAYER_ENABLE = yes  # TL_LOWR / TL_UPPR
