/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Copyright (c) 2012, NVIDIA Corporation. All Rights Reserved.
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
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>

#include <linux/lightsensor.h>

#include <cutils/log.h>

#include "cm3218.h"

/*****************************************************************************/

#define LS_DEVICE_NAME              "/dev/lightsensor"
#define EVENT_TYPE_LIGHT            ABS_MISC

Cm3218Light::Cm3218Light()
    : SensorBase(LS_DEVICE_NAME, "cm3218-ls"),
      mEnabled(false),
      mInputReader(16)
{
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_L;
    mPendingEvent.type = SENSOR_TYPE_LIGHT;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));
}

Cm3218Light::~Cm3218Light() {
}

int Cm3218Light::enable(int32_t handle, int enabled) {
    int err = 0;
    if (enabled != mEnabled) {
        if (!mEnabled) {
            open_device();
        }
        err = ioctl(dev_fd, LIGHTSENSOR_IOCTL_ENABLE, &enabled);
        ALOGE_IF(err, "LIGHTSENSOR_IOCTL_ENABLE failed (%s)", strerror(err));
        if (!err) {
            mEnabled = enabled;
        }
        if (!mEnabled) {
            close_device();
        }
    }
    return err;
}

int Cm3218Light::readEvents(sensors_event_t* data, int count)
{
    int numEventReceived = 0;
    input_event const* event = NULL;

    if (count < 1)
        return -EINVAL;

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    while (count && mInputReader.readEvent(&event)) {
        if (event->type == EV_ABS) {
            if (event->code == EVENT_TYPE_LIGHT) {
                mPendingEvent.light = event->value;
            }
        } else if (event->type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("Cm3218Light: unknown event (type=%d, code=%d)",
                    event->type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}
