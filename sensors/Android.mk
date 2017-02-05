LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    SensorWrapper.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_SHARED_LIBRARIES := \
    libhardware liblog


LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE := sensors.macallan
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
