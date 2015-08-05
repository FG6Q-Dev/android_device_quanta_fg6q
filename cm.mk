# Release name
PRODUCT_RELEASE_NAME := fg6q

# Boot animation
TARGET_SCREEN_HEIGHT := 1600
TARGET_SCREEN_WIDTH := 2560

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_tablet_wifionly.mk)

# Inherit device configuration
$(call inherit-product, device/quanta/fg6q/device_fg6q.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := fg6q
PRODUCT_NAME := cm_fg6q
PRODUCT_BRAND := quanta
PRODUCT_MODEL := Gigaset QV1030
PRODUCT_MANUFACTURER := quanta
