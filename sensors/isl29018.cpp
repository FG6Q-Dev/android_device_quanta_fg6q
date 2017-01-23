/*
 * Copyright (c) 2011-2012, NVIDIA CORPORATION.  All rights reserved.
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

#include "isl29018.h"
#include "SensorUtil.h"

/*****************************************************************************/

Isl29018Light::Isl29018Light(const char *sysPath, int sensor_id)
    : SensorBase(NULL, NULL),
      mEnabled(false),
      mHasPendingEvent(false),
      mLast_value(-1),
      mAlready_warned(false),
      mSysPath(sysPath),
      sid(sensor_id)
{
}

Isl29018Light::~Isl29018Light() {
}

int Isl29018Light::enable(int32_t handle, int en) {
    if (en != 0) {
        mEnabled = true;
        mLast_value = -1;
        mPollingDelay = MAX_POLLING_DELAY;
        mLastns.timestamp = 0;
    }
    else
        mEnabled = false;
    return 0;
}

int Isl29018Light::setDelay(int32_t handle, int64_t ns) {
    mPollingDelay = ns;
    return 0;
}

bool Isl29018Light::hasPendingEvents() const {
    if(mEnabled)
        return true;
    else
        return false;
}

int Isl29018Light::readEvents(sensors_event_t* data, int count) {
    unsigned int value = 0;
    sensors_event_t evt;
    sensors_event_t Currentns;
    int64_t Delay;

    if (count < 1 || data == NULL || !mEnabled) {
        ALOGE("Will not work on zero count(%i) or null pointer\n", count);
        return 0;
    }

    Currentns.timestamp = getTimestamp();
    Delay = Currentns.timestamp - mLastns.timestamp;
    if (Delay < mPollingDelay)
        return 0;
    else
        mLastns.timestamp = Currentns.timestamp;

    input_event const* event;

    int amt = readIntFromFile(mSysPath, &value);
    if (amt <= 0 && mAlready_warned == false) {
        ALOGE("LightSensor: read from %s failed", mSysPath);
        mAlready_warned = false;
        return 0;
    }

    if (((float)value) == mLast_value)
        return 0;

    evt.version = sizeof(sensors_event_t);
    evt.sensor = sid;
    evt.type = SENSOR_TYPE_LIGHT;
    evt.light = (float)value;
    evt.timestamp = Currentns.timestamp;
    *data = evt;
    mLast_value = evt.light;
    ALOGV("LightSensor: value is %i", (int)value );
    return 1;
}

Isl29018Prox::Isl29018Prox(const char *sysPath, int sensor_id, unsigned int ProxThreshold)
    : SensorBase(NULL, NULL),
      mEnabled(0),
      mHasPendingEvent(false),
      mAlready_warned(false),
      mLast_value(-1),
      mSysPath(sysPath),
      sid(sensor_id),
      mProximityThreshold(ProxThreshold)
{
}

Isl29018Prox::~Isl29018Prox() {
}

int Isl29018Prox::enable(int32_t handle, int en) {
    if (en != 0) {
        mLast_value = -1;
        mEnabled = true;
        mPollingDelay = MAX_POLLING_DELAY;
        mLastns.timestamp = 0;
    }
    else
       mEnabled = false;
    return 0;
}

int Isl29018Prox::setDelay(int32_t handle, int64_t ns) {
    mPollingDelay = ns;
    return 0;
}

bool Isl29018Prox::hasPendingEvents() const {
    if(mEnabled)
        return true;
    else
        return false;
}

int Isl29018Prox::readEvents(sensors_event_t* data, int count) {
    unsigned int value = 0;
    unsigned int distanceValue;
    sensors_event_t Currentns;
    int64_t Delay;

    if (count < 1 || data == NULL || !mEnabled)
        return 0;

    Currentns.timestamp = getTimestamp();
    Delay = Currentns.timestamp - mLastns.timestamp;
    if (Delay < mPollingDelay)
        return 0;
    else
        mLastns.timestamp = Currentns.timestamp;

    int amt = readIntFromFile(mSysPath, &value);
    if (amt <= 0 && mAlready_warned == false) {
        ALOGE("ProximitySensor: read from %s failed", mSysPath);
        mAlready_warned = true;
        return 0;
    }

    distanceValue = value > mProximityThreshold ? 0 : 1; /* 0 = near; 1 = far. Based on how android expects */
    if (distanceValue == mLast_value)
        return 0;

    mLast_value = distanceValue;
    (*data).version = sizeof(sensors_event_t);
    (*data).sensor = sid;
    (*data).type = SENSOR_TYPE_PROXIMITY;
    (*data).distance =  (float)distanceValue;
    (*data).timestamp = Currentns.timestamp;
    ALOGV("ProximitySensor: value is %i", (int)value );
    return 1;
}
