$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

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

# Media
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/media/media_profiles.xml:system/etc/media_profiles.xml \
    device/quanta/fg6q/media/media_codecs.xml:system/etc/media_codecs.xml \
    device/quanta/fg6q/media/enctune.conf:system/etc/enctune.conf

# GPS
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/gps/gpsconfig.xml:system/etc/gpsconfig.xml \
    device/quanta/fg6q/gps/libgps.conf:system/etc/libgps.conf \
    device/quanta/fg6q/gps/gps.conf:system/etc/gps.conf

# Audio
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/audio/asound.conf:system/etc/asound.conf \
    device/quanta/fg6q/audio/audioConfig_qvoice_icera_pc400.xml:system/etc/audioConfig_qvoice_icera_pc400.xml \
    device/quanta/fg6q/audio/audio_policy.conf:system/etc/audio_policy.conf \
    device/quanta/fg6q/audio/nvaudio_conf.xml:system/etc/nvaudio_conf.xml \
    device/quanta/fg6q/audio/firmware/es305_fw.bin:system/vendor/firmware/es305_fw.bin

# Audio override
PRODUCT_COPY_FILES_OVERRIDES := \
    system/etc/audio_effects.conf

# Wifi
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/wifi/nvram_4329.txt:system/etc/nvram_4329.txt \
    device/quanta/fg6q/wifi/nvram_4330.txt:system/etc/nvram_4330.txt \
    device/quanta/fg6q/wifi/nvram_43241.txt:system/etc/nvram_43241.txt \
    device/quanta/fg6q/wifi/nvram_43241_SP.txt:system/etc/nvram_43241_SP.txt \
    device/quanta/fg6q/wifi/bcm4329.hcd:system/etc/firmware/bcm4329.hcd \
    device/quanta/fg6q/wifi/bcm4330.hcd:system/etc/firmware/bcm4330.hcd \
    device/quanta/fg6q/wifi/bcm43241.hcd:system/etc/firmware/bcm43241.hcd \
    device/quanta/fg6q/wifi/firmware/bcm4330/fw_bcmdhd_mfg.bin:system/vendor/firmware/bcm4330/fw_bcmdhd_mfg.bin \
    device/quanta/fg6q/wifi/firmware/bcm4330/fw_bcmdhd.bin:system/vendor/firmware/bcm4330/fw_bcmdhd.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241/fw_bcmdhd_mfg.bin:system/vendor/firmware/bcm43241/fw_bcmdhd_mfg.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241/fw_bcmdhd.bin:system/vendor/firmware/bcm43241/fw_bcmdhd.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241_SP/fw_bcmdhd_mfg.bin:system/vendor/firmware/bcm43241_SP/fw_bcmdhd_mfg.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241_SP/fw_bcmdhd.bin:system/vendor/firmware/bcm43241_SP/fw_bcmdhd.bin \
    device/quanta/fg6q/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf

# Power
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/power.macallan.rc:system/etc/power.macallan.rc

# Camera
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/camera/nvcamera.conf:system/etc/nvcamera.conf \
    device/quanta/fg6q/camera/model_frontal.xml:system/etc/model_frontal.xml

# Keylayout
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    device/quanta/fg6q/keylayout/tegra-kbc.kl:system/usr/keylayout/tegra-kbc.kl

# Keylayout overrides
PRODUCT_COPY_FILES_OVERRIDES += \
    system/usr/keylayout/AVRCP.kl \
    system/usr/keylayout/Generic.kl

# These are the hardware-specific features
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml

# Quanta
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/permissions/com.nvidia.graphics.xml:system/etc/permissions/com.nvidia.graphics.xml \
    device/quanta/fg6q/permissions/com.nvidia.miracast.xml:system/etc/permissions/com.nvidia.miracast.xml \
    device/quanta/fg6q/permissions/com.nvidia.nvsi.xml:system/etc/permissions/com.nvidia.nvsi.xml \
    device/quanta/fg6q/permissions/com.nvidia.nvstereoutils.xml:system/etc/permissions/com.nvidia.nvstereoutils.xml

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    persist.sys.usb.config=mtp

PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0 \
    ap.interface=wlan1 \
    ro.carrier=wifi-only \
    ro.sf.override_null_lcd_density = 1 \
    persist.tegra.compositor=glcomposer \
    debug.hwui.render_dirty_regions=false \
    persist.tegra.nvmmlite = 1 \
    drm.service.enabled=true \
    tf.enable=y

# Audio
PRODUCT_PACKAGES += \
        audio.a2dp.default \
        audio.usb.default \
        audio.r_submix.default \
        libaudioutils

# Misc
PRODUCT_PACKAGES += \
    librs_jni \
    com.android.future.usb.accessory \
    libnetcmdiface \
    WiFiDirectDemo

# Filesystem management tools
PRODUCT_PACKAGES += \
       make_ext4fs \
       setup_fs


# Only a kernel from open sources is a happy kernel!
#LOCAL_PATH := device/quanta/fg6q
#ifeq ($(TARGET_PREBUILT_KERNEL),)
#	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
#else
#	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
#endif
#
#PRODUCT_COPY_FILES += \
#    $(LOCAL_KERNEL):kernel


PRODUCT_CHARACTERISTICS := tablet

PRODUCT_AAPT_CONFIG := xlarge hdpi xhdpi
PRODUCT_AAPT_PREF_CONFIG := xhdpi

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_NAME := full_fg6q
PRODUCT_DEVICE := fg6q
