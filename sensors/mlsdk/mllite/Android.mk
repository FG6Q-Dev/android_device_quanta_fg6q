# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

ifneq ($(TARGET_SIMULATOR),true)

include $(NVIDIA_DEFAULTS)
LOCAL_MODULE := libmllite

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := -DLOG_TAG=\"Sensors\"
LOCAL_CFLAGS += -DLINUX

LOCAL_SRC_FILES := $(call all-c-files-under)

LOCAL_C_INCLUDES := device/nvidia/drivers/sensors/mlsdk/driver/include

LOCAL_SHARED_LIBRARIES := liblog

LOCAL_PRELINK_MODULE := false

#TODO: Remove following lines before include statement and fix the source giving warnings/errors
LOCAL_NVIDIA_NO_WARNINGS_AS_ERRORS := 1
include $(NVIDIA_SHARED_LIBRARY)

endif # !TARGET_SIMULATOR

