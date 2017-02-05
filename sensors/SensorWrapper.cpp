/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
* @file SensorWrapper.cpp
*
* This file wraps a vendor sensor module.
*
*/

//#define LOG_NDEBUG 0
//#define LOG_PARAMETERS

#define LOG_TAG "SensorWrapper"
#include <cutils/log.h>

#include <utils/threads.h>
#include <hardware/hardware.h>
#include <hardware/sensors.h>

#include "SensorWrapper.h"

typedef struct {
    sensors_poll_device_t base;
    union {
        sensors_poll_device_t *device;
        hw_device_t *hw_device;
    } vendor;
} device_t;

static android::Mutex vendor_mutex;

static union {
    const sensors_module_t *module;
    const hw_module_t *hw_module;
} vendor;

static const struct sensor_t sSensorList[] = {
      MPLROTATIONVECTOR_DEF,
      MPLLINEARACCEL_DEF,
      MPLGRAVITY_DEF,
      MPLGYRO_DEF,
      MPLACCEL_DEF,
      MPLMAGNETICFIELD_DEF,
      MPLORIENTATION_DEF,
      CM3218LIGHT_DEF,
};

static bool ensure_vendor_module_is_loaded(void)
{
    android::Mutex::Autolock lock(vendor_mutex);

    if (!vendor.module) {
        int rv = hw_get_module_by_class("sensors", "vendor", &vendor.hw_module);
        if (rv) {
            ALOGE("failed to open vendor module, error %d", rv);
            vendor.module = NULL;
        } else {
            ALOGI("loaded vendor module: %s version %x", vendor.module->common.name,
                vendor.module->common.module_api_version);
        }
    }

    return vendor.module != NULL;
}

static int activate(struct sensors_poll_device_t *dev, int sensor_handle, int enabled)
{
    device_t *device = (device_t *) dev;

    return device->vendor.device->activate(device->vendor.device, sensor_handle, enabled);
}
static int setDelay(struct sensors_poll_device_t *dev, int sensor_handle, int64_t sampling_period_ns)
{
    device_t *device = (device_t *) dev;

    return device->vendor.device->setDelay(device->vendor.device, sensor_handle, sampling_period_ns);
}
static int poll(struct sensors_poll_device_t *dev, sensors_event_t* data, int count)
{
    device_t *device = (device_t *) dev;

    return device->vendor.device->poll(device->vendor.device, data, count);
}

static int device_close(hw_device_t *hw_device)
{
    device_t *device = (device_t *) hw_device;
    int rv = device->base.common.close(device->vendor.hw_device);
    free(device);
    return rv;
}

static int device_open(const hw_module_t *module, const char *name, hw_device_t **device_out)
{
    int rv;
    device_t *device;

    if (!ensure_vendor_module_is_loaded()) {
        return -EINVAL;
    }

    device = (device_t *) calloc(sizeof(*device), 1);
    if (!device) {
        ALOGE("%s: Failed to allocate memory", __func__);
        return -ENOMEM;
    }

    rv = vendor.module->common.methods->open(vendor.hw_module, name, &device->vendor.hw_device);
    if (rv) {
        ALOGE("%s: failed to open, error %d\n", __func__, rv);
        free(device);
        return rv;
    }

    device->base.common.tag = HARDWARE_DEVICE_TAG;
    device->base.common.version  = SENSORS_DEVICE_API_VERSION_1_0;
    device->base.common.module   = const_cast<hw_module_t*>(module);
    device->base.common.close    = device_close;
    device->base.activate        = activate;
    device->base.setDelay        = setDelay;
    device->base.poll            = poll;

    *device_out = (hw_device_t *) device;
    return 0;
}

static int sensors__get_sensors_list(struct sensors_module_t* module, struct sensor_t const** list){
    *list = sSensorList;
    return ARRAY_SIZE(sSensorList);
}

static struct hw_module_methods_t sensors_module_methods = {
        .open = device_open
};

struct sensors_module_t HAL_MODULE_INFO_SYM = {
        .common = {
                .tag = HARDWARE_MODULE_TAG,
                .version_major = 1,
                .version_minor = 0,
                .id = SENSORS_HARDWARE_MODULE_ID,
                .name = "Macallan Sensor Wrapper",
                .author = "Spartaner25",
                .methods = &sensors_module_methods,
                .dso = 0,
                .reserved = {},
        },
        .get_sensors_list = sensors__get_sensors_list,
};