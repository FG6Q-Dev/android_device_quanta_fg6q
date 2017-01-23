/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Copyright (c) 2012-2013, NVIDIA CORPORATION. All Rights Reserved.
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

#ifndef ANDROID_LIGHT_SENSOR_H
#define ANDROID_LIGHT_SENSOR_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "sensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"

/*****************************************************************************/

#define CM3217_LUX_CONV_FACTOR    2.1505

#define CM3217LIGHT_DEF {                     \
    "CM3217 Light Sensor",                    \
    "Capella Microsystems",                   \
    1, ID_L,                                  \
    SENSOR_TYPE_LIGHT, 10240.0f, 1.0f,        \
    0.5f, 0, { } }

struct input_event;

class Cm3217Light : public SensorBase {
    int mEnabled;
    InputEventCircularReader mInputReader;
    sensors_event_t mPendingEvent;
    bool mHasPendingEvent;

    int setInitialState();

public:
            Cm3217Light();
    virtual ~Cm3217Light();
    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);
};

/*****************************************************************************/

#endif  // ANDROID_LIGHT_SENSOR_H
