/*
 * Copyright (C) 2008 The Android Open Source Project
 *
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

#ifndef ANDROID_ISL29018_H
#define ANDROID_ISL29018_H

#include "sensors.h"
#include "SensorBase.h"

#define ISL29018LIGHT_DEF(handle) {           \
    "Intersil isl29018 Ambient Light Sensor", \
    "Intersil",                               \
    1, handle,                                \
    SENSOR_TYPE_LIGHT, 1000.0f, 1.0f,         \
    0.09f, 0, { } }

#define ISL29018PROX_DEF(handle) {            \
    "Intersil isl29018 Proximity sensor",     \
    "Intersil",                               \
    1, handle,                                \
    SENSOR_TYPE_PROXIMITY, 1.0f, 1.0f,        \
    0.40f, 0, { } }

#define MAX_POLLING_DELAY 2000000000

class Isl29018Light : public SensorBase {
    bool mEnabled;
    bool mHasPendingEvent;
    unsigned int mLast_value;
    bool mAlready_warned;
    const char *mSysPath;
    int sid;
    int64_t mPollingDelay;
    sensors_event_t mLastns;

public:
            Isl29018Light(const char *sysPath, int sensor_id);
    virtual ~Isl29018Light();
    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);
    virtual int setDelay(int32_t handle, int64_t ns);
};

class Isl29018Prox : public SensorBase {
    bool mEnabled;
    bool mHasPendingEvent;
    bool mAlready_warned;
    float mLast_value;
    const char *mSysPath;
    int sid;
    unsigned int mProximityThreshold;
    int64_t mPollingDelay;
    sensors_event_t mLastns;

public:
            Isl29018Prox(const char *sysPath, int sensor_id,
                         unsigned int ProxThreshold);
    virtual ~Isl29018Prox();
    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);
    virtual int setDelay(int32_t handle, int64_t ns);
};

#endif  // ANDROID_ISL29018_H

