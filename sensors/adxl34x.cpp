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

#include <fcntl.h>
#include <cutils/log.h>
#include <stdlib.h>

#include "adxl34x.h"
#include "SensorUtil.h"

#define EVENT_RATE_CODE_25HZ    8
#define EVENT_RATE_CODE_50HZ    9
#define EVENT_RATE_CODE_100HZ   10
#define EVENT_RATE_CODE_200HZ   11

#define RATE_SYSFS_PATH  "/sys/class/input/input2/device/rate"
#define AUTOSLEEP_SYSFS_PATH  "/sys/class/input/input2/device/autosleep"

/*****************************************************************************/

Adxl34xAccel::Adxl34xAccel(int sensor_id)
    : SensorBase(NULL, "ADXL34x accelerometer"),
      mEnabled(0),
      mHasPendingEvent(false),
      mInputReader(32)
{
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = sensor_id;
    mPendingEvent.type = SENSOR_TYPE_ACCELEROMETER;
    mPendingEvent.acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;
}

Adxl34xAccel::~Adxl34xAccel() {
   if (data_fd >= 0)
       close(data_fd);
}

int Adxl34xAccel::setDelay(int32_t handle, int64_t ns) {
    unsigned int autosleep = 1;
    unsigned int code = EVENT_RATE_CODE_25HZ;

   /*
    * Mapping of data rate according to the delay
    *          FASTEST      GAME        UI         NORMAL
    * <--200HZ-->|<--100HZ-->|<--50HZ-->|<----25HZ---|----
    *
    * Autosleep is disabled when events rate required
    * is fast, for games. If autosleep is active accelerometer
    * takes time to wake and response delivered is not
    * good. For UI events, such as screen orientation
    * autosleep can be enabled which will reduce power consumption.
    */
    if (ns <= SENSOR_DELAY_FASTEST) {
        code = EVENT_RATE_CODE_200HZ;
        autosleep = 0;
    } else if (ns <= SENSOR_DELAY_GAME) {
        code = EVENT_RATE_CODE_100HZ;
        autosleep = 0;
    } else if (ns <= SENSOR_DELAY_UI) {
        code = EVENT_RATE_CODE_50HZ;
    }

    /* Change data rate through sysfs entry */
    if (writeIntToFile(RATE_SYSFS_PATH, code) <=0)
        return 0;

    /* Change autosleep option through sysfs entry */
    if (writeIntToFile(AUTOSLEEP_SYSFS_PATH, autosleep) <= 0)
        return 0;

    return 0;
}

int Adxl34xAccel::enable(int32_t handle, int en) {
    if ((en != 0) && (data_fd >= 0)) {
        mEnabled = true;
    }
    else
        mEnabled = false;
    return 0;
}

bool Adxl34xAccel::hasPendingEvents() const {
    return false;
}

int Adxl34xAccel::readEvents(sensors_event_t* data, int count) {
    if (count < 1)
        return -EINVAL;

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_ABS) {
            processEvent(event->code, event->value);
        } else if (type == EV_SYN) {
            int64_t time = timevalToNano(event->time);
            mPendingEvent.timestamp = time;
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("Accelerometer: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }
    return numEventReceived;
}

void Adxl34xAccel::processEvent(int code, int value) {
    switch (code) {
        case ABS_X:
            mPendingEvent.acceleration.x = value * (GRAVITY_EARTH/200.0f);
            break;
        case ABS_Y:
            mPendingEvent.acceleration.y = value * (GRAVITY_EARTH/200.0f);
            break;
        case ABS_Z:
            mPendingEvent.acceleration.z = value * (GRAVITY_EARTH/200.0f);
            break;
     }
}


int Adxl34xAccel::getFd() const {
    ALOGD("Accelerometer: getFd returning %d", data_fd);
    return data_fd;
}
