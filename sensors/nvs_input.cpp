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

/* This is the generic sensor class driver for sensor kernel drivers that
 * implement the NVS (NVIDIA SENSOR) API.  The NVS API has the following
 * properties:
 * - The input polling mechanism is used.
 * - sysfs attributes are used.
 *      The following attributes are expected:
 *      - enable
 *          write:
 *              0 = off
 *              1 = on
 *          read:
 *              current enable status
 *      - delay
 *          write:
 *              requested polling delay in microseconds (us).
 *          read:
 *              If device is enabled then returns delay used.  When not enabled
 *              then returns minDelay.  See struct sensor_t minDelay.
 *      - divisor:
 *          read:
 *              To be used as X / divisor where X is
 *              - reported input value
 *              - resolution
 *              - max_range
 *      - resolution
 *          write:
 *              Resolution allows control of when the sensor reports its input.
 *              if 0 then reporting is done whenever data is ready.
 *              if 1 then reporting is done whenever data changes.
 *              A resolution > 1 creates a window around the previously
 *              reported data and data is reported and a new window created
 *              only when data is outside the window.
 *              The formula for a window is:
 *              LIMIT_LO = PREVIOUS_REPORTED_DATA - (RESOLUTION / 2)
 *              LIMIT_HI = PREVIOUS_REPORTED_DATA + (RESOLUTION / 2)
 *              Note: attibute divisor is used.  For example: If resolution = 2
 *              and divisor = 10 then the window will be +/- 0.1
 *          read:
 *              If device is enabled then returns value written.  Else see
 *              struct sensor_t resolution.
 *              Note: attribute divisor must be used to get the final value
 *              when device is disabled.
 *      - max_range:
 *          write:
 *              Sets scale.  Valid values from 0 to number of scale levels.
 *              For example, 0, 1, 2, 3.  When device is disabled, max_range
 *              can be read back after each scale write to determine the scale
 *              to use.  See struct sensor_t maxRange.
 *              -EINVAL is returned if past the scale level or it is not
 *              supported.
 *          read:
 *              If device is enabled then returns value written.  Else see
 *              struct sensor_t maxRange
 *              Note: attribute divisor must be used to get the final value
 *              when device is disabled.
 *      - microamp:
 *          read:
 *              See struct sensor_t power
 *              Note: power expects mA and microamp is reported as uA.
 *
 * These attributes can be used to populate the sensor_t structure at runtime.
 */
#include <fcntl.h>
#include <cutils/log.h>
#include <stdlib.h>
#include "nvs_input.h"
#include "SensorUtil.h"

/*****************************************************************************/

NvsInput::NvsInput(const char *name,
                   int input_num,
                   int sensor,
                   int type,
                   /* Use of resolution:
                    * - if resolution == 0 then report every polling delay.
                    * - if resolution == 1 then report every polling delay
                    *      only when value changes.
                    * - if resolution > 1 then report every polling delay
                    *      only when value is outside the resolution window.
                    */
                   unsigned int resolution)
    : SensorBase(NULL, name),
      mDelay(0),
      mDivisor(1),
      mInputReader(32)
{
    int err;

    mEnabled = false;
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = sensor;
    mPendingEvent.type = type;
    err = sysfsInputPaths(input_num);
    if (sysfsMem) {
        err = readIntFromFile(sysFs.divisor, &mDivisor);
        if (err > 0) {
            ALOGV_IF(SYSFS_VERBOSE, "%s %s=%u\n",
                     __func__, sysFs.divisor, mDivisor);
        } else {
            ALOGE("%s ERR attemping to read %s",
                  __func__, sysFs.divisor);
            mDivisor = 1;
        }
    }
    if (sysfsMem) {
        err = writeIntToFile(sysFs.resolution, resolution);
        if (err > 0) {
            ALOGV_IF(SYSFS_VERBOSE, "%s %s=%u\n",
                     __func__, sysFs.resolution, resolution);
        } else {
            ALOGE("%s ERR attemping to set %s to %u",
                  __func__, sysFs.resolution, resolution);
        }
    }
    setDelay(type, 0);
    enable(type, 0);
}

NvsInput::~NvsInput()
{
    if (data_fd > 0)
        close(data_fd);
    if (sysfsMem)
        free(sysfsMem);
}

int NvsInput::getFd() const
{
    return data_fd;
}

int NvsInput::enable(int32_t handle, int en)
{
    int err = 0;

    if (sysfsMem) {
        err = writeIntToFile(sysFs.enable, en);
        if (err > 0) {
            mEnabled = (bool)en;
            err = 0;
            ALOGV_IF(SYSFS_VERBOSE, "%s %s=%d\n",
                     __func__, sysFs.enable, en);
        } else {
            ALOGE("%s ERR attemping to set %s to %d",
                  __func__, sysFs.enable, en);
        }
    } else {
        mEnabled = (bool)en;
    }
    return err;
}

int NvsInput::getEnable(int32_t handle)
{
    if (mEnabled)
        return 1;

    return 0;
}

int NvsInput::setDelay(int32_t handle, int64_t ns)
{
    int err = 0;

    if (sysfsMem) {
        err = writeIntToFile(sysFs.delay, (int)(ns / 1000));
        if (err > 0) {
            mDelay = ns;
            err = 0;
            ALOGV_IF(SYSFS_VERBOSE, "%s %s=%lld\n",
                     __func__, sysFs.delay, ns);
        } else {
            ALOGE("%s ERR attemping to set %s to %lld",
                  __func__, sysFs.delay, ns);
        }
    } else {
        mDelay = ns;
    }
    return err;
}

int64_t NvsInput::getDelay(int32_t handle)
{
    return mDelay;
}

int NvsInput::readEvents(sensors_event_t* data, int count)
{
    if ((count < 1) || (data == NULL))
        return -EINVAL;

    if (!mEnabled)
        return 0;

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        if ((event->type == EV_ABS) || (event->type == EV_REL)) {
            processEvent(event->code, (float)event->value);
        } else if (event->type == EV_SYN) {
            int64_t time = timevalToNano(event->time);
            mPendingEvent.timestamp = time;
            *data++ = mPendingEvent;
            count--;
            numEventReceived++;
        } else {
            ALOGE("%s %s unknown event->type %d\n",
                 __func__, data_name, event->type);
        }
        mInputReader.next();
    }
    return numEventReceived;
}

void NvsInput::processEvent(int code, float fVal)
{
    if (code > REL_Z)
        code = 0;
    if (mDivisor > 1)
        fVal /= mDivisor;
    mPendingEvent.data[code] = fVal;
}

int NvsInput::sysfsInputPaths(int inputNum)
{
    char *ptr;
    char **pptr;
    int size;
    int i = 0;

    size = sizeof(struct sysfsAttrs) / sizeof(char*);
    sysfsMem = (char*)malloc(sizeof(char[size][SYSFS_PATH_SIZE_MAX]));
    ptr = sysfsMem;
    if (ptr != NULL) {
        pptr = (char**)&sysFs;
        do {
            *pptr++ = ptr;
            ptr += sizeof(char[SYSFS_PATH_SIZE_MAX]);
        } while (++i < size);
    } else {
        ALOGE("%s ERR allocating memory for sysfs", __func__);
        return -1;
    }

    sprintf(sysFs.path, "/sys/class/input/input%d/%s", inputNum, data_name);
    sprintf(sysFs.enable, "/sys/class/input/input%d/%s/enable", inputNum, data_name);
    sprintf(sysFs.delay, "/sys/class/input/input%d/%s/delay", inputNum, data_name);
    sprintf(sysFs.divisor, "/sys/class/input/input%d/%s/divisor", inputNum, data_name);
    sprintf(sysFs.resolution, "/sys/class/input/input%d/%s/resolution", inputNum, data_name);
    sprintf(sysFs.max_range, "/sys/class/input/input%d/%s/max_range", inputNum, data_name);
    sprintf(sysFs.microamp, "/sys/class/input/input%d/%s/microamp", inputNum, data_name);
    return 0;
}
