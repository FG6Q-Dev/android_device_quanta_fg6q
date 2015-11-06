$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

$(call inherit-product, frameworks/native/build/tablet-10in-xhdpi-2048-dalvik-heap.mk)

$(call inherit-product-if-exists, vendor/quanta/fg6q/fg6q-vendor.mk)

$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

DEVICE_PACKAGE_OVERLAYS += device/quanta/fg6q/overlay

# Root
PRODUCT_COPY_FILES += \
	device/quanta/fg6q/fstab.macallan:root/fstab.macallan \
	device/quanta/fg6q/ueventd.macallan.rc:root/ueventd.macallan.rc

# Init
PRODUCT_COPY_FILES += \
	device/quanta/fg6q/init/init.hdcp.rc:root/init.hdcp.rc \
	device/quanta/fg6q/init/init.macallan.rc:root/init.macallan.rc \
	device/quanta/fg6q/init/init.macallan.usb.rc:root/init.macallan.usb.rc \
	device/quanta/fg6q/init/init.tf.rc:root/init.tf.rc 

# Charger
PRODUCT_COPY_FILES += \
	device/quanta/fg6q/charger/res/images/charger/battery_fail.png:root/res/images/charger/battery_fail.png \
	device/quanta/fg6q/charger/res/images/charger/battery_scale.png:root/res/images/charger/battery_scale.png

# Media
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/media/media_profiles.xml:system/etc/media_profiles.xml \
    device/quanta/fg6q/media/media_codecs.xml:system/etc/media_codecs.xml \
    device/quanta/fg6q/media/enctune.conf:system/etc/enctune.conf \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml

# GPS
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/gps/gpsconfig.xml:system/etc/gps/gpsconfig.xml \
    device/quanta/fg6q/gps/gps.conf:system/etc/gps.conf

# Audio
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/audio/asound.conf:system/etc/asound.conf \
    device/quanta/fg6q/audio/audioConfig_qvoice_icera_pc400.xml:system/etc/audioConfig_qvoice_icera_pc400.xml \
    device/quanta/fg6q/audio/audio_policy.conf:system/etc/audio_policy.conf \
    device/quanta/fg6q/audio/nvaudio_conf.xml:system/etc/nvaudio_conf.xml \
    device/quanta/fg6q/audio/firmware/es305_fw.bin:system/vendor/firmware/es305_fw.bin

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
    device/quanta/fg6q/wifi/firmware/bcm43241/fw_bcmdhd_apsta.bin:system/vendor/firmware/bcm43241/fw_bcmdhd_apsta.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241/fw_bcmdhd.bin:system/vendor/firmware/bcm43241/fw_bcmdhd.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241/fw_bcmdhd_p2p.bin:system/vendor/firmware/bcm43241/fw_bcmdhd_p2p.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241_SP/fw_bcmdhd_mfg.bin:system/vendor/firmware/bcm43241_SP/fw_bcmdhd_mfg.bin \
    device/quanta/fg6q/wifi/firmware/bcm43241_SP/fw_bcmdhd.bin:system/vendor/firmware/bcm43241_SP/fw_bcmdhd.bin \
    device/quanta/fg6q/wifi/wifi_loader.sh:system/bin/wifi_loader.sh \
    device/quanta/fg6q/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
    device/quanta/fg6q/wifi/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf \
    device/quanta/fg6q/wifi/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf


PRODUCT_PACKAGES += \
    libwpa_client \
    hostapd \
    dhcpcd.conf \
    wpa_supplicant \
    wpa_supplicant.conf

# Power
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/power.macallan.rc:system/etc/power.macallan.rc

# Camera
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/camera/nvcamera.conf:system/etc/nvcamera.conf \
    device/quanta/fg6q/camera/model_frontal.xml:system/etc/model_frontal.xml

# Bluetooth
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf


# IDC
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/idc/raydium_ts.idc:system/usr/idc/raydium_ts.idc \
    device/quanta/fg6q/idc/sensor00fn11.idc:system/usr/idc/sensor00fn11.idc


# Keylayout
PRODUCT_COPY_FILES += \
    device/quanta/fg6q/keylayout/Generic.kl:system/usr/keylayout/Generic.kl \
    device/quanta/fg6q/keylayout/AVRCP.kl:system/usr/keylayout/AVRCP.kl \
    device/quanta/fg6q/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    device/quanta/fg6q/keylayout/tegra-kbc.kl:system/usr/keylayout/tegra-kbc.kl

# These are the hardware-specific features
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
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
    device/quanta/fg6q/permissions/com.nvidia.nvsi.xml:system/etc/permissions/com.nvidia.nvsi.xml

PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0 \
    ap.interface=wlan1 \
    ro.carrier=wifi-only \
    ro.sf.override_null_lcd_density = 1 \
    persist.tegra.compositor=glcomposer \
    debug.hwui.render_dirty_regions=false \
    persist.tegra.nvmmlite = 1 \
    drm.service.enabled=true \
    tf.enable=y \
    ro.opengles.version=131072

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    persist.sys.isUsbOtgEnabled=1 \
    persist.sys.usb.config=mtp,adb \
    persist.service.adb.enable=1

# Audio
PRODUCT_PACKAGES += \
    audio.a2dp.default \
    audio.usb.default \
    audio.r_submix.default \
    libaudioutils

# Filesystem management tools
PRODUCT_PACKAGES += \
    make_ext4fs \
    setup_fs

# Power management
PRODUCT_PACKAGES += \
    NvCPLSvc

$(call inherit-product, build/target/product/full.mk)

PRODUCT_CHARACTERISTICS := tablet

PRODUCT_AAPT_CONFIG := xlarge hdpi xhdpi
PRODUCT_AAPT_PREF_CONFIG := xhdpi

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_NAME := full_fg6q
PRODUCT_DEVICE := fg6q
