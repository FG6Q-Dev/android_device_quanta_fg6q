/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2011 Invensense, Inc.
 * Copyright (c) 2013 NVIDIA CORPORATION.  All rights reserved.
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
 * Common definitions for MPL sensor devices.
 */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define ID_MPL_BASE (0)
#define ID_GY (ID_MPL_BASE)
#define ID_A  (ID_GY + 1)
#define ID_M  (ID_A + 1)
#define ID_O  (ID_M + 1)
#define ID_RV (ID_O + 1)
#define ID_LA (ID_RV + 1)
#define ID_GR (ID_LA + 1)
#define ID_L  (ID_GR + 1)
#define ID_P  (ID_L + 1)
#define ID_T  (ID_P + 1)
#define ID_AP (ID_P +1) /* Atomospheric Pressure */

#ifndef ANDROID_MPL_SENSOR_DEFS_H
#define ANDROID_MPL_SENSOR_DEFS_H

#define MPLROTATIONVECTOR_DEF {                         \
    "MPL rotation vector",                              \
    "Invensense",                                       \
    1, ID_RV,                                           \
    SENSOR_TYPE_ROTATION_VECTOR, 10240.0f, 1.0f,        \
    0.5f, 20000, 0, 0, 0, 0, 0, 0, { } }

#define MPLLINEARACCEL_DEF {                            \
    "MPL linear accel",                                 \
    "Invensense",                                       \
    1, ID_LA,                                           \
    SENSOR_TYPE_LINEAR_ACCELERATION, 10240.0f, 1.0f,    \
    0.5f, 20000, 0, 0, 0, 0, 0, 0, { } }

#define MPLGRAVITY_DEF {                                \
    "MPL gravity",                                      \
    "Invensense",                                       \
    1, ID_GR,                                           \
    SENSOR_TYPE_GRAVITY, 10240.0f, 1.0f,                \
    0.5f, 20000, 0, 0, 0, 0, 0, 0, { } }

#define MPLGYRO_DEF {                                   \
    "MPL Gyro",                                         \
    "Invensense",                                       \
    1, ID_GY,                                           \
    SENSOR_TYPE_GYROSCOPE, 10240.0f, 1.0f,              \
    0.5f, 20000, 0, 0, 0, 0, 0, 0, { } }

#define MPLACCEL_DEF {                                  \
    "MPL accel",                                        \
    "Invensense",                                       \
    1, ID_A,                                            \
    SENSOR_TYPE_ACCELEROMETER, 10240.0f, 1.0f,          \
    0.5f, 20000, 0, 0, 0, 0, 0, 0, { } }

#define MPLMAGNETICFIELD_DEF {                          \
    "MPL magnetic field",                               \
    "Invensense",                                       \
    1, ID_M,                                            \
    SENSOR_TYPE_MAGNETIC_FIELD, 10240.0f, 1.0f,         \
    0.5f, 20000, 0, 0, 0, 0, 0, 0, { } }

#define MPLORIENTATION_DEF {                            \
    "MPL Orientation",                                  \
    "Invensense",                                       \
    1, ID_O,                                            \
    SENSOR_TYPE_ORIENTATION, 360.0f, 1.0f,              \
    9.7f, 20000, 0, 0, 0, 0, 0, 0, { } }

#define MPLPRESSURE_DEF {                               \
    "MPL Pressure   ",                                  \
    "Invensense",                                       \
    1, ID_AP,                                           \
    SENSOR_TYPE_PRESSURE, 110000.0f, 1.0f,              \
    0.032f, 25500, 0, 0, 0, 0, 0, 0, { } }

#endif

#ifndef ANDROID_LIGHT_SENSOR_H
#define ANDROID_LIGHT_SENSOR_H

#define CM3218LIGHT_DEF {                     \
    "CM3218 Light Sensor",                    \
    "Capella Microsystems",                   \
    1, ID_L,                                  \
    SENSOR_TYPE_LIGHT, 10240.0f, 1.0f,        \
    0.5f, 0, 0, 0, 0, 0, 0, 0,  { } }

#endif  // ANDROID_LIGHT_SENSOR_H