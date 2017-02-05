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
    SENSOR_TYPE_ROTATION_VECTOR, 1.0f, 0.00001f,        \
    15.5f, 5000, 0, 0, 0, 0, 0, 0, { } }

#define MPLLINEARACCEL_DEF {                            \
    "MPL linear accel",                                 \
    "Invensense",                                       \
    1, ID_LA,                                           \
    SENSOR_TYPE_LINEAR_ACCELERATION, 20.0f, 0.04f,      \
    15.5f, 5000, 0, 0, 0, 0, 0, 0, { } }

#define MPLGRAVITY_DEF {                                \
    "MPL gravity",                                      \
    "Invensense",                                       \
    1, ID_GR,                                           \
    SENSOR_TYPE_GRAVITY, 9.81f, 0.00001f,               \
    15.5f, 5000, 0, 0, 0, 0, 0, 0, { } }

#define MPLGYRO_DEF {                                   \
    "MPL Gyro",                                         \
    "Invensense",                                       \
    1, ID_GY,                                           \
    SENSOR_TYPE_GYROSCOPE, 35.0f, 0.001f,               \
    5.5f, 5000, 0, 0, 0, 0, 0, 0, { } }

#define MPLACCEL_DEF {                                  \
    "MPL accel",                                        \
    "Invensense",                                       \
    1, ID_A,                                            \
    SENSOR_TYPE_ACCELEROMETER, 20.0f, 0.04f,            \
    0.0f, 5000, 0, 0, 0, 0, 0, 0, { } }

#define MPLMAGNETICFIELD_DEF {                          \
    "MPL magnetic field",                               \
    "Invensense",                                       \
    1, ID_M,                                            \
    SENSOR_TYPE_MAGNETIC_FIELD, 9830.0f, 0.285f,        \
    10.0f, 10000, 0, 0, 0, 0, 0, 0, { } }

#define MPLORIENTATION_DEF {                            \
    "MPL Orientation",                                  \
    "Invensense",                                       \
    1, ID_O,                                            \
    SENSOR_TYPE_ORIENTATION, 360.0f, 0.00001f,          \
    15.5f, 5000, 0, 0, 0, 0, 0, 0, { } }

#endif

#ifndef ANDROID_LIGHT_SENSOR_H
#define ANDROID_LIGHT_SENSOR_H

#define CM3218LIGHT_DEF {                     \
    "CM3218 Light Sensor",                    \
    "Capella Microsystems",                   \
    1, ID_L,                                  \
    SENSOR_TYPE_LIGHT, 20480.0f, 1.0f,        \
    0.5f, 0, 0, 0, 0, 0, 0, 0,  { } }

#endif  // ANDROID_LIGHT_SENSOR_H