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

#ifndef ANDROID_ADXL34X_H
#define ANDROID_ADXL34X_H

#include "SensorBase.h"
#include "sensors.h"
#include "InputEventReader.h"
#include <linux/input.h>
#include <hardware/sensors.h>

#define SENSOR_DELAY_FASTEST   1000000LL
#define SENSOR_DELAY_GAME      20000000LL
#define SENSOR_DELAY_UI        60000000LL
#define SENSOR_DELAY_NORMAL    200000000LL

// conversion of acceleration data to SI units (m/s^2)
#define RANGE_A                     (2*GRAVITY_EARTH)
#define RESOLUTION_A                (RANGE_A/(256*NUMOFACCDATA))

#define ADXL34XACCEL_DEF(handle) {                    \
    "ADXL34x 3-axis Accelerometer",                   \
    "Analog Devices",                                 \
    1, handle,                                        \
    SENSOR_TYPE_ACCELEROMETER, RANGE_A, RESOLUTION_A, \
    0.23f, 20000, { } }

class Adxl34xAccel : public SensorBase {
    bool mEnabled;
    sensors_event_t mPendingEvent;
    bool mHasPendingEvent;
    InputEventCircularReader mInputReader;

public:
            Adxl34xAccel(int sensor_id);
    virtual ~Adxl34xAccel();

    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);
    virtual int getFd() const;
    virtual int setDelay(int32_t handle, int64_t ns);
    void processEvent(int code, int value);
};

#endif  // ANDROID_ADXL34X_H
