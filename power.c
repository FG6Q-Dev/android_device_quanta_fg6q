/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (c) 2012-2014, NVIDIA CORPORATION.  All rights reserved.
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

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cutils/uevent.h>
#include <sys/poll.h>
#include <pthread.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <stdbool.h>

#define LOG_TAG "Macallan PowerHAL"
#include <utils/Log.h>
 
#include <hardware/hardware.h>
#include <hardware/power.h>

static int sysfs_write(char *path, char *s)
{
    char buf[80];
    int len;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);
        return -1;
    }

    close(fd);
    return 0;
}

static void macallan_power_init(struct power_module *module)
{
}

static void macallan_power_set_interactive(struct power_module *module, int on)
{
	int i = 0;
    int ret;
    char path[80];
	const char* state = (0 == on)?"0":"1";
    
	while(1)
    {
        sprintf(path, "/sys/class/input/input%d/enabled", i);
		ret = access(path, F_OK);
        if (ret < 0) {
			ALOGI("Breaked at device id:%d", i);
			break;
		}
        else {
			if (0 == on) {
				ALOGI("Disabling input device:%d", i);
			}
			else {
				ALOGI("Enabling input device:%d", i);
			}
			sysfs_write(path, state);
		}
		i++;
    }
}

static void macallan_power_hint(struct power_module *module, power_hint_t hint, void *data)
{
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_2,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "Macallan Power HAL",
        .author = "Spartaner25",
        .methods = &power_module_methods,
    },

    .init = macallan_power_init,
    .setInteractive = macallan_power_set_interactive,
    .powerHint = macallan_power_hint,
};