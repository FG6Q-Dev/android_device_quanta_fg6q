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
#include <stdbool.h>

#define LOG_TAG "Macallan PowerHAL"
#include <utils/Log.h>
 
#include <hardware/hardware.h>
#include <hardware/power.h>

#define CPU_MAX_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
#define IO_IS_BUSY_PATH "/sys/devices/system/cpu/cpufreq/interactive/io_is_busy"
#define LOW_POWER_MAX_FREQ "918000"
#define NORMAL_MAX_FREQ "1810500"

static bool low_power_mode = false;

static char *max_cpu_freq = NORMAL_MAX_FREQ;
static char *low_power_max_cpu_freq = LOW_POWER_MAX_FREQ;

struct macallan_power_module {
    struct power_module base;
    pthread_mutex_t lock;
    int boostpulse_fd;
    int boostpulse_warned;
};

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

static void toggle_input(int on)
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

static void macallan_power_init(struct power_module *module)
{
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/timer_rate","20000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/timer_slack","80000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/min_sample_time","30000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/hispeed_freq","696000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/go_hispeed_load","99");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/target_loads","75 228000:85 696000:90 1530000:95");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/above_hispeed_delay","20000 1530000:50000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/boostpulse_duration","30000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/io_is_busy", "0");
}

static void macallan_power_set_interactive(struct power_module *module, int on)
{
    /*
     * Lower maximum frequency when screen is off.
     */
    sysfs_write(CPU_MAX_FREQ_PATH,(!on || low_power_mode) ? low_power_max_cpu_freq : max_cpu_freq);
    sysfs_write(IO_IS_BUSY_PATH, on ? "1" : "0");
    toggle_input(on);
}

static void macallan_power_hint(struct power_module *module, power_hint_t hint, void *data)
{
    struct macallan_power_module *macallan = (struct macallan_power_module *) module;
    switch (hint) {
        case POWER_HINT_VSYNC:
            break;
        case POWER_HINT_INTERACTION:
            break;
        case POWER_HINT_LOW_POWER:
            pthread_mutex_lock(&macallan->lock);
            if (data) {
                sysfs_write(CPU_MAX_FREQ_PATH, low_power_max_cpu_freq);
            } else {
                sysfs_write(CPU_MAX_FREQ_PATH, max_cpu_freq);
            }
            low_power_mode = data;
            pthread_mutex_unlock(&macallan->lock);
            break;
        default:
            break;
    }
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