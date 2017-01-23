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

#include <fcntl.h>
#include <cutils/log.h>
#include <stdlib.h>

#include "isl29028.h"
#include "SensorUtil.h"

/*****************************************************************************/

Isl29028Light::Isl29028Light(const char *sysPath, const char *sysModePath, int sensor_id)
    : Isl29018Light(sysPath, sensor_id),
      mSysModePath(sysModePath)
{
}

int Isl29028Light::enable(int32_t handle, int en) {
    int err = writeIntToFile(mSysModePath, en);
    if (err <= 0)
        return err;

    return Isl29018Light::enable(handle, en);
}

Isl29028Prox::Isl29028Prox(const char *sysPath, const char *sysEnablePath,
      int sensor_id, unsigned int ProxThreshold)
    : Isl29018Prox(sysPath, sensor_id, ProxThreshold),
      mSysEnablePath(sysEnablePath)
{
}

int Isl29028Prox::enable(int32_t handle, int en) {
    int err = writeIntToFile(mSysEnablePath, en);
    if (err <= 0)
        return err;

    return Isl29018Prox::enable(handle, en);
}
