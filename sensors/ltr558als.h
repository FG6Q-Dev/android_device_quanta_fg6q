/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef ANDROID_LTR558ALS_H
#define ANDROID_LTR558ALS_H

#include "SensorBase.h"

#define LTR_558ALS_DEF(handle) {           \
    "Lite On Technology Corp LTR-558 Ambient Light Sensor", \
    "Lite-On Technology",                               \
    1, handle,                                \
    SENSOR_TYPE_LIGHT, 1000.0f, 1.0f,         \
    0.09f, 0, { } }

#define LTR_558PS_DEF(handle) {            \
    "Lite On Technology Corp LTR-558 Proximity sensor",     \
    "Lite-On Technology",                               \
    1, handle,                                \
    SENSOR_TYPE_PROXIMITY, 1.0f, 1.0f,        \
    0.40f, 0, { } }

#define MAX_POLLING_DELAY 2000000000

class ltr558Light : public SensorBase {
    bool mEnabled;
    bool mHasPendingEvent;
    float mLast_value;
    bool mAlready_warned;
    const char *mSysPath;
    const char *mSysEnablePath;
    int sid;
    int64_t mPollingDelay;
    sensors_event_t mLastns;

public:
            ltr558Light(const char *sysPath, const char *sysEnablePath, int sensor_id);
    virtual ~ltr558Light();
    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);
    virtual int setDelay(int32_t handle, int64_t ns);
};

class ltr558Prox : public SensorBase {
    bool mEnabled;
    bool mHasPendingEvent;
    bool mAlready_warned;
    float mLast_value;
    const char *mSysPath;
    const char *mSysEnablePath;
    int sid;
    unsigned int mProximityThreshold;
    int64_t mPollingDelay;
    sensors_event_t mLastns;

public:
            ltr558Prox(const char *sysPath, const char *sysEnablePath,
                       int sensor_id, unsigned int ProxThreshold);
    virtual ~ltr558Prox();
    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);
    virtual int setDelay(int32_t handle, int64_t ns);
};

#endif  // ANDROID_LTR558ALS_H

