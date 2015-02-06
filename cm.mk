## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := fg6q

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/quanta/fg6q/device_fg6q.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := fg6q
PRODUCT_NAME := cm_fg6q
PRODUCT_BRAND := quanta
PRODUCT_MODEL := fg6q
PRODUCT_MANUFACTURER := quanta
