/* Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2011 Invensense, Inc.
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef INV_MPL_STORED_DATA_H
#define INV_MPL_STORED_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
    Includes.
*/
#include "mltypes.h"

/*
    Defines
*/
#define MLCAL_FILE "/inv_cal_data.bin"
#define MLCAL_PROTECTED_DIR "MPU_CONF_DIR"
#define MLCAL_DEFAULT_DIR "MPU_DATA_DIR"

/*
    APIs
*/
inv_error_t inv_load_calibration(void);
inv_error_t inv_store_calibration(void);

/*
    Internal APIs
*/
inv_error_t inv_read_cal(unsigned char *cal, size_t len);
inv_error_t inv_write_cal(unsigned char *cal, size_t len);
inv_error_t inv_load_cal_V0(unsigned char *calData, size_t len);
inv_error_t inv_load_cal_V1(unsigned char *calData, size_t len);

/*
    Other prototypes
*/
inv_error_t inv_load_cal(unsigned char *calData);
inv_error_t inv_store_cal(unsigned char *calData, size_t length);
inv_error_t inv_pop_cal_protected_path();
inv_error_t inv_pop_cal_default_path();
const char *inv_get_cal_protected_path();
const char *inv_get_cal_default_path();

#ifdef __cplusplus
}
#endif
#endif  /* INV_MPL_STORED_DATA_H */
