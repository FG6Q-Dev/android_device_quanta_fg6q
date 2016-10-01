LOCAL_PATH := $(call my-dir)

# camera

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
     libcamera_shim.c

LOCAL_SHARED_LIBRARIES := libutils libgui libshim_sensors

LOCAL_MODULE := libshim_camera
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

# Nvidia audio icu

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
     icu53.c

LOCAL_SHARED_LIBRARIES := libicuuc libicui18n
LOCAL_MODULE := libshim_nvaudio
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

# VectorImpl for sensors

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
     SharedBuffer.cpp \
     VectorImpl.cpp

LOCAL_C_INCLUDES += external/safe-iop/include
LOCAL_SHARED_LIBRARIES := liblog

LOCAL_MODULE := libshim_sensors
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

# gps

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
     libgps.c

LOCAL_SHARED_LIBRARIES := liblog libcutils libgui libbinder libutils libshim_sensors

LOCAL_MODULE := libgps
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
