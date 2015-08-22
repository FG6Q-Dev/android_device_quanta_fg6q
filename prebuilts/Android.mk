# Copyright (C) 2014 The Android Open Source Project
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

include $(CLEAR_VARS)
LOCAL_MODULE := NvCPLSvc
LOCAL_MODULE_SUFFIX := .apk
LOCAL_SRC_FILES := NvCPLSvc.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_PATH := $(PRODUCT_OUT)/system/app
LOCAL_CERTIFICATE := platform
include $(BUILD_PREBUILT)
