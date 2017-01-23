/*
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef COMPASS_SENSOR_H
#define COMPASS_SENSOR_H

#include "nvs_input.h"

class CompassSensor : public NvsInput {
    signed char mCompassOrientation[9];
    long mCachedCompassData[3];
    int64_t mCompassTimestamp;

public:
    CompassSensor(const char *name,
                  int input_num,
                  unsigned int resolution);
    virtual ~CompassSensor();
    virtual int readEvents(sensors_event_t *data, int count) { return 0; }
    int readSample(long *data, int64_t *timestamp);
    int providesCalibration() { return 0; }
    void getOrientationMatrix(signed char *orient);
    long getSensitivity();
    int getAccuracy() { return 0; }
    int isIntegrated() { return 1; }

private:
    void processCompassEvent(const input_event *event);
};

/*****************************************************************************/

#endif  // COMPASS_SENSOR_H


