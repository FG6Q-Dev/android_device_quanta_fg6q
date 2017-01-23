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

#include "SensorUtil.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int readIntFromFile(const char *path, unsigned int *val)
{
    char buffer[20] = {0};
    int data_fd;
    int err;

    /* The opening of the file handle is placed here
     * as work around that if it is opened in the constructor
     * read on the file handle fails.
     */
    data_fd = open(path, O_RDONLY);
    if (data_fd < 0)
        return 0;

    err = read(data_fd, buffer, sizeof(buffer));
    close(data_fd);

    if (err <= 0) return err;

    *val = atoi(buffer);
    return 1;
}

int writeIntToFile(const char *path, unsigned int val)
{
    int en_fd;
    char buffer[16] = {0};

    en_fd = open(path, O_RDWR);
    if (en_fd < 0) {
        return en_fd;
    }

    sprintf(buffer, "%d", val);
    int err = write(en_fd, buffer, sizeof(buffer));
    close(en_fd);
    if (err <= 0) {
        return err;
    }
    return 1;
}
