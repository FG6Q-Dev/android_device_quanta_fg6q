#
# Copyright (C) 2011 The Android Open-Source Project
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
#

# WARNING: Everything listed here will be built on ALL platforms,
# including x86, the emulator, and the SDK.  Modules must be uniquely
# named (liblights.tuna), and must build everywhere, or limit themselves
# to only building on ARM if they include assembly. Individual makefiles
# are responsible for having their own logic, for fine-grained control.

ifneq ($(filter fg6q,$(TARGET_DEVICE)),)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# copy audio_effects override
$(shell mkdir -p $(TARGET_OUT_ETC); \
        cp -pf device/quanta/fg6q/audio/audio_effects.conf $(TARGET_OUT_ETC))

# copy keylayout overrides
$(shell mkdir -p $(TARGET_OUT_KEYLAYOUT); \
        cp -pf device/quanta/fg6q/keylayout/AVRCP.kl $(TARGET_OUT_KEYLAYOUT); \
        cp -pf device/quanta/fg6q/keylayout/Generic.kl $(TARGET_OUT_KEYLAYOUT))
# Gamepad Layouts etc.
#        cp -pf device/quanta/fg6q/keylayout/Vendor_05ac_Product_0239.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_22b8_Product_093d.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_044f_Product_d007.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_045e_Product_028e.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_045e_Product_0719.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_046d_Product_c21d.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_046d_Product_c21e.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_046d_Product_c21f.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_046d_Product_c216.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_046d_Product_c294.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_046d_Product_c299.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_046d_Product_c532.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_054c_Product_0268.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_057e_Product_0306.kl $(TARGET_OUT_KEYLAYOUT); \
#        cp -pf device/quanta/fg6q/keylayout/Vendor_2378_Product_100a.kl $(TARGET_OUT_KEYLAYOUT)

include $(call all-makefiles-under,$(LOCAL_PATH))

endif
