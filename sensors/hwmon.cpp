/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <fcntl.h>
#include <cutils/log.h>
#include <stdlib.h>
#include <linux/input.h>
#include <hardware/sensors.h>

#include "hwmon.h"
#include "SensorUtil.h"

/*****************************************************************************/

HwmonTemp::HwmonTemp(const char *sysPath, int sensor_id)
    : SensorBase(NULL, NULL),
      mEnabled(false),
      mHasPendingEvent(false),
      mLast_value(-1),
      mAlready_warned(false),
      mSysPath(sysPath),
      sid(sensor_id)
{
}

HwmonTemp::~HwmonTemp() {
}

int HwmonTemp::enable(int32_t handle, int en) {
    if (en != 0)
        mEnabled = true;
    else
        mEnabled = false;
    return 0;
}

bool HwmonTemp::hasPendingEvents() const {
    if(mEnabled)
        return true;
    else
        return false;
}

int HwmonTemp::readEvents(sensors_event_t* data, int count) {
    float value = -1.0f;
    unsigned int iValue = 0;
    sensors_event_t evt;

    if (count < 1 || data == NULL || !mEnabled) {
        ALOGE("Will not work on zero count(%i) or null pointer\n", count);
        return 0;
    }

    int numEventReceived = 0;
    input_event const* event;

    int amt = readIntFromFile(mSysPath, &iValue);
    if (amt <= 0 && mAlready_warned == false) {
        ALOGE("TemperatureMonitor: read from %s failed", mSysPath);
        mAlready_warned = false;
        return 0;
    }
    value = iValue / 1000.0f;
    if (value == mLast_value)
       return 0;
    evt.version = sizeof(sensors_event_t);
    evt.sensor = sid;
    evt.type = SENSOR_TYPE_TEMPERATURE;
    evt.temperature = value;
    evt.timestamp = now_ns();
    *data = evt;
    mLast_value = value;
    ALOGD("TemperatureMonitor: value is %i", (int)value );
    return 1;
}

int HwmonTemp::getFd() const {
    return -1;
}
