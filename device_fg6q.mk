$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

$(call inherit-product-if-exists, vendor/quanta/fg6q/fg6q-vendor.mk)

DEVICE_PACKAGE_OVERLAYS += device/quanta/fg6q/overlay

# Root
PRODUCT_COPY_FILES += \
	device/quanta/fg6q/fstab.macallan:root/fstab.macallan \
	device/quanta/fg6q/ueventd.macallan.rc:root/ueventd.macallan.rc

# Init
PRODUCT_COPY_FILES += \
	device/quanta/fg6q/init/init.macallan.rc:root/init.macallan.rc \
	device/quanta/fg6q/init/init.macallan.usb.rc:root/init.macallan.usb.rc \

LOCAL_PATH := device/quanta/fg6q
ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel

$(call inherit-product, build/target/product/full.mk)

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_NAME := full_fg6q
PRODUCT_DEVICE := fg6q
