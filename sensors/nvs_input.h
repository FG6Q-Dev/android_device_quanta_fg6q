/* Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef NVS_INPUT_H
#define NVS_INPUT_H

#define SYSFS_PATH_SIZE_MAX             (50)

#include "sensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"
#include <linux/input.h>
#include <hardware/sensors.h>

class NvsInput : public SensorBase {
public:
            NvsInput(const char *name,
                     int input_num,
                     int sensor,
                     int type,
                     unsigned int resolution);
    virtual ~NvsInput();

    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const { return false;};
    virtual int getFd() const;
    virtual int setDelay(int32_t handle, int64_t ns);
    virtual int64_t getDelay(int32_t handle);
    virtual int enable(int32_t handle, int enabled);
    virtual int getEnable(int32_t handle);
    virtual void processEvent(int code, float fVal);

protected:
    int64_t mDelay;
    unsigned int mDivisor;
    sensors_event_t mPendingEvent;
    InputEventCircularReader mInputReader;

    struct sysfsAttrs {
       char *path;
       char *enable;
       char *delay;
       char *divisor;
       char *resolution;
       char *max_range;
       char *microamp;
    } sysFs;
    char *sysfsMem;

    int sysfsInputPaths(int inputNum);
};

#endif  // NVS_INPUT_H


