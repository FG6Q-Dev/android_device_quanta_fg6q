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

#define LOG_NDEBUG 0

#include <cutils/log.h>

#include "CompassSensor.h"
#include "MPLSupport.h"

/*****************************************************************************/

CompassSensor::CompassSensor(const char *name,
                             int input_num,
                             unsigned int resolution)
    : NvsInput(name,
               input_num,
               ID_M,
               SENSOR_TYPE_MAGNETIC_FIELD,
               resolution),
      mCompassTimestamp(0)
{
    memset(mCachedCompassData, 0, sizeof(mCachedCompassData));

    FILE *fptr;
    char path[80];
    sprintf(path, "%s/orientation", sysFs.path);
    ALOGV_IF(SYSFS_VERBOSE, "HAL:sysfs:cat %s (%lld)",
             path, getTimestamp());
    fptr = fopen(path, "r");
    if (fptr != NULL) {
        int om[9];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d",
               &om[0], &om[1], &om[2], &om[3], &om[4],
               &om[5], &om[6], &om[7], &om[8]);
        fclose(fptr);
        mCompassOrientation[0] = om[0];
        mCompassOrientation[1] = om[1];
        mCompassOrientation[2] = om[2];
        mCompassOrientation[3] = om[3];
        mCompassOrientation[4] = om[4];
        mCompassOrientation[5] = om[5];
        mCompassOrientation[6] = om[6];
        mCompassOrientation[7] = om[7];
        mCompassOrientation[8] = om[8];
    } else {
        ALOGE("HAL:Couldn't read compass mounting matrix");
    }
}

CompassSensor::~CompassSensor()
{
}

/* use for Invensense compass calibration */
#define COMPASS_EVENT_DEBUG (0)
void CompassSensor::processCompassEvent(const input_event *event)
{
    VHANDLER_LOG;

    switch (event->code) {
    case EVENT_TYPE_ICOMPASS_X:
        ALOGV_IF(COMPASS_EVENT_DEBUG, "EVENT_TYPE_ICOMPASS_X\n");
        mCachedCompassData[0] = event->value;
        break;

    case EVENT_TYPE_ICOMPASS_Y:
        ALOGV_IF(COMPASS_EVENT_DEBUG, "EVENT_TYPE_ICOMPASS_Y\n");
        mCachedCompassData[1] = event->value;
        break;

    case EVENT_TYPE_ICOMPASS_Z:
        ALOGV_IF(COMPASS_EVENT_DEBUG, "EVENT_TYPE_ICOMPASS_Z\n");
        mCachedCompassData[2] = event->value;
        break;
    }

    mCompassTimestamp =
        (int64_t)event->time.tv_sec * 1000000000L + event->time.tv_usec * 1000L;
}

void CompassSensor::getOrientationMatrix(signed char *orient)
{
    VFUNC_LOG;

    memcpy(orient, mCompassOrientation, sizeof(mCompassOrientation));
}

long CompassSensor::getSensitivity()
{
    VFUNC_LOG;

    long sensitivity;
    ALOGV_IF(SYSFS_VERBOSE, "HAL:sysfs:cat %s (%lld)",
             sysFs.max_range, getTimestamp());
    inv_read_data(sysFs.max_range, &sensitivity);
    sensitivity *= (1L << 15);
    return sensitivity;
}

/**
    @brief         This function is called by sensors_mpl.cpp
                   to read sensor data from the driver.
    @param[out]    data      sensor data is stored in this variable. Scaled such that
                             1 uT = 2^16
    @para[in]      timestamp data's timestamp
    @return        1, if 1   sample read, 0, if not, negative if error
 */
int CompassSensor::readSample(long *data, int64_t *timestamp)
{
    VHANDLER_LOG;

    int numEventReceived = 0, done = 0;

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0) {
        //ALOGE("HAL:no compass events read");
        return n;
    }

    input_event const* event;

    while (done == 0 && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_REL) {
            processCompassEvent(event);
        } else if (type == EV_SYN) {
            *timestamp = mCompassTimestamp;
            memcpy(data, mCachedCompassData, sizeof(mCachedCompassData));
            done = 1;
        } else {
            ALOGE("HAL:Compass Sensor: unknown event (type=%d, code=%d)",
                  type, event->code);
        }
        mInputReader.next();
    }

    return done;
}

