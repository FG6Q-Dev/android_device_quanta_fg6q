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

#ifndef ANDROID_ISL29028_H
#define ANDROID_ISL29028_H

#include "isl29018.h"

#define ISL29028LIGHT_DEF(handle) {           \
    "Intersil isl29028 Ambient Light Sensor", \
    "Intersil",                               \
    1, handle,                                \
    SENSOR_TYPE_LIGHT, 1000.0f, 1.0f,         \
    0.09f, 0, { } }

#define ISL29028PROX_DEF(handle) {            \
    "Intersil isl29028 Proximity sensor",     \
    "Intersil",                               \
    1, handle,                                \
    SENSOR_TYPE_PROXIMITY, 1.0f, 1.0f,        \
    0.40f, 0, { } }

class Isl29028Light : public Isl29018Light {
    const char *mSysModePath;

public:
            Isl29028Light(const char *sysPath, const char *sysModePath, int sensor_id);
    virtual int enable(int32_t handle, int enabled);
};

class Isl29028Prox : public Isl29018Prox {
    const char *mSysEnablePath;

public:
            Isl29028Prox(const char *sysPath, const char *sysEnablePath,
            int sensor_id, unsigned int ProxThreshold);
    virtual int enable(int32_t handle, int enabled);
};

#endif  // ANDROID_ISL29028_H

