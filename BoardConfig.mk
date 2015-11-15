DEVICE_PACKAGE_OVERLAYS += device/quanta/fg6q/overlay

# This variable is set first, so it can be overridden
# by BoardConfigVendor.mk
BOARD_USES_GENERIC_AUDIO := false

# defines to support legacy blobs
COMMON_GLOBAL_CFLAGS += \
    -DNEEDS_VECTORIMPL_SYMBOLS \
    -DADD_LEGACY_MEMORY_DEALER_CONSTRUCTOR_SYMBOL \
    -DADD_LEGACY_ACQUIRE_BUFFER_SYMBOL

# inherit from the proprietary version
-include vendor/quanta/fg6q/BoardConfigVendor.mk

TARGET_ARCH := arm
TARGET_NO_BOOTLOADER := true
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_VARIANT := cortex-a15
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true
TARGET_BOARD_PLATFORM := tegra
TARGET_BOOTLOADER_BOARD_NAME := macallan

# Kernel
BOARD_KERNEL_CMDLINE := androidboot.selinux=disabled
BOARD_KERNEL_BASE := 0x10000000
BOARD_KERNEL_PAGESIZE := 2048

#TARGET_PREBUILT_KERNEL := device/quanta/fg6q/kernel

#Try to build the kernel
TARGET_KERNEL_SOURCE := kernel/quanta/fg6q
TARGET_KERNEL_CONFIG := cyanogenmod_fg6q_defconfig

# Video
BOARD_EGL_CFG := device/quanta/fg6q/egl.cfg
USE_OPENGL_RENDERER := true
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
VSYNC_EVENT_PHASE_OFFSET_NS := 0
SF_VSYNC_EVENT_PHASE_OFFSET_NS := 1

# Recovery
BOARD_HAS_NO_SELECT_BUTTON := true
BOARD_USE_CUSTOM_RECOVERY_FONT := \"roboto_23x41.h\"
# Use this flag if the board has a ext4 partition larger than 2gb
BOARD_HAS_LARGE_FILESYSTEM := true
RECOVERY_FSTAB_VERSION := 2

# TWRP
ifeq ($(TARGET_BUILD_VARIANT), eng)
    TARGET_KERNEL_CONFIG := recovery_fg6q_defconfig
    RECOVERY_VARIANT := twrp
endif
DEVICE_RESOLUTION := 2560x1600
RECOVERY_SDCARD_ON_DATA := true
RECOVERY_GRAPHICS_USE_LINELENGTH := true
TW_INTERNAL_STORAGE_PATH := "/data/media"
TW_INTERNAL_STORAGE_MOUNT_POINT := "data"
TW_EXTERNAL_STORAGE_PATH := "/external_sd"
TW_EXTERNAL_STORAGE_MOUNT_POINT := "external_sd"
TW_EXCLUDE_SUPERSU := true
TW_BRIGHTNESS_PATH := "/sys/class/backlight/pwm-backlight/brightness"

# Partitions
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_FLASH_BLOCK_SIZE := 131072
BOARD_BOOTIMAGE_PARTITION_SIZE := 8388608
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 8388608
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 2147483648
BOARD_USERDATAIMAGE_PARTITION_SIZE := 11320426496
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_CACHEIMAGE_PARTITION_SIZE := 2147483648

# Wifi related defines
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_WLAN_DEVICE := bcmdhd

WIFI_DRIVER_FW_PATH_PARAM   := "/sys/module/bcmdhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_AP      := "/system/vendor/firmware/bcm43241/fw_bcmdhd_apsta.bin"
WIFI_DRIVER_FW_PATH_STA     := "/system/vendor/firmware/bcm43241/fw_bcmdhd.bin"
#WIFI_DRIVER_FW_PATH_P2P     := "/system/vendor/firmware/bcm43241/fw_bcmdhd_p2p.bin"

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR ?= device/quanta/fg6q/bluetooth
