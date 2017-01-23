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

#ifndef ANDROID_SENSOR_UTIL__H
#define ANDROID_SENSOR_UTIL__H

/**
 * Open a file, read a single unsigned integer value from it,
 * and close it.
 *
 * @return 1 in case of success, 0 or < 0 in case of error.
 */
int readIntFromFile(const char *path, unsigned int *val);

/**
 * Open a file, write a single unsigned integer value to it,
 * and close it.
 *
 * @return 1 in case of success, 0 or < 0 in case of error.
 */
int writeIntToFile(const char *path, unsigned int val);

#endif
