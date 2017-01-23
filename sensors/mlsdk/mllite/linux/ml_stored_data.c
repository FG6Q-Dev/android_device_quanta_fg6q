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

/**
 * @defgroup ML_STORED_DATA
 *
 * @{
 *      @file     ml_stored_data.c
 *      @brief    functions for reading and writing stored data sets.
 *                Typically, these functions process stored calibration data.
 */

#include <stdio.h>

#undef MPL_LOG_TAG
#define MPL_LOG_TAG "MPL-storeload"


#include "ml_stored_data.h"
#include "storage_manager.h"
#include "log.h"
#include "mlos.h"

#define LOADCAL_DEBUG    0
#define STORECAL_DEBUG   0

#define DEFAULT_KEY 29681

#define STORECAL_LOG MPL_LOGI
#define LOADCAL_LOG  MPL_LOGI

static char mpl_cal_protected_path[PATH_MAX];
static char mpl_cal_default_path[PATH_MAX];

inv_error_t inv_pop_cal_protected_path()
{
    const char *mpl_cal_protected_dir = getenv(MLCAL_PROTECTED_DIR);
    char *ptr;

    if (!mpl_cal_protected_dir)
        return INV_ERROR;

    if (strlen(mpl_cal_protected_dir) + strlen(MLCAL_FILE) >= PATH_MAX)
        return INV_ERROR;

    strcpy(mpl_cal_protected_path, mpl_cal_protected_dir);
    ptr = mpl_cal_protected_path + strlen(mpl_cal_protected_path);
    strcpy(ptr, MLCAL_FILE);
    MPL_LOGV("Protected calibration path is %s\n", mpl_cal_protected_path);
    return INV_SUCCESS;
}

inv_error_t inv_pop_cal_default_path()
{
    const char *mpl_cal_default_dir = getenv(MLCAL_DEFAULT_DIR);
    char *ptr;

    if (!mpl_cal_default_dir)
        return INV_ERROR;

    if (strlen(mpl_cal_default_dir) + strlen(MLCAL_FILE) >= PATH_MAX)
        return INV_ERROR;

    strcpy(mpl_cal_default_path, mpl_cal_default_dir);
    ptr = mpl_cal_default_path + strlen(mpl_cal_default_path);
    strcpy(ptr, MLCAL_FILE);
    MPL_LOGV("Default calibration path is %s\n", mpl_cal_default_path);
    return INV_SUCCESS;
}

bool inv_exist_cal_protected_path()
{
   if (strlen(mpl_cal_protected_path))
        return true;
   else
        return false;
}

bool inv_exist_cal_default_path()
{
    if (strlen(mpl_cal_default_path))
        return true;
    else
        return false;
}

const char *inv_get_cal_default_path()
{
    if (inv_exist_cal_default_path())
        return mpl_cal_default_path;
    else
        return NULL;
}

const char *inv_get_cal_protected_path()
{
    if (inv_exist_cal_protected_path())
        return mpl_cal_protected_path;
    else
        return NULL;
}

inv_error_t inv_copy_cal(size_t len)
{
    FILE *default_fp, *protected_fp;
    int bytesRead;
    unsigned char cal[len];

    if (!inv_exist_cal_default_path() || !inv_exist_cal_protected_path())
        return INV_ERROR_FILE_OPEN;

    default_fp = fopen(mpl_cal_default_path, "rb");
    if (!default_fp) {
        protected_fp = fopen(mpl_cal_protected_path, "rb");
        if (!protected_fp)
            return INV_ERROR_FILE_OPEN;
        else {
            default_fp = fopen(mpl_cal_default_path , "wb");
            if (!default_fp) {
                fclose(protected_fp);
                return INV_ERROR_FILE_OPEN;
            }
            bytesRead = fread(cal, 1, len, protected_fp);
            fwrite(cal, 1, bytesRead, default_fp);
            fclose(default_fp);
            fclose(protected_fp);
        }
    } else
        fclose(default_fp);

    return INV_SUCCESS;
}

inv_error_t inv_read_cal(unsigned char *cal, size_t len)
{
    FILE *fp;
    int bytesRead;
    inv_error_t result = INV_SUCCESS;

    if (inv_copy_cal(len) != INV_SUCCESS)
        return INV_ERROR_FILE_OPEN;

    if (!inv_exist_cal_default_path())
        return INV_ERROR_FILE_OPEN;

    fp = fopen(mpl_cal_default_path, "rb");

    if (fp == NULL) {
        MPL_LOGE("Cannot open file \"%s\" for read\n", MLCAL_FILE);
        return INV_ERROR_FILE_OPEN;
    }
    bytesRead = fread(cal, 1, len, fp);
    if (bytesRead != len) {
        MPL_LOGE("bytes read (%d) don't match requested length (%d)\n",
                 bytesRead, len);
        result = INV_ERROR_FILE_READ;
        goto read_cal_end;
    }
    else {
        MPL_LOGI("Bytes read = %d", bytesRead);
    }

read_cal_end:
    fclose(fp);
    return result;
}

inv_error_t inv_write_cal(unsigned char *cal, size_t len)
{
    FILE *fp;
    int bytesWritten;
    inv_error_t result = INV_SUCCESS;

    if (len <= 0) {
        MPL_LOGE("Nothing to write");
        return INV_ERROR_FILE_WRITE;
    }
    else {
        MPL_LOGI("cal data size to write = %d", len);
    }

    if (!inv_exist_cal_default_path())
        return INV_ERROR_FILE_OPEN;

    fp = fopen(mpl_cal_default_path, "wb");
    if (fp == NULL) {
        MPL_LOGE("Cannot open file \"%s\" for write\n", MLCAL_FILE);
        return INV_ERROR_FILE_OPEN;
    }
    bytesWritten = fwrite(cal, 1, len, fp);
    if (bytesWritten != len) {
        MPL_LOGE("bytes written (%d) don't match requested length (%d)\n",
                 bytesWritten, len);
        result = INV_ERROR_FILE_WRITE;
    }
    else {
        MPL_LOGI("Bytes written = %d", bytesWritten);
    }
    fclose(fp);
    return result;
}

/**
 *  @brief  Loads a type 0 set of calibration data.
 *          It parses a binary data set containing calibration data.
 *          The binary data set is intended to be loaded from a file.
 *          This calibrations data format stores values for (in order of
 *          appearance) :
 *          - temperature compensation : temperature data points,
 *          - temperature compensation : gyro biases data points for X, Y,
 *              and Z axes.
 *          - accel biases for X, Y, Z axes.
 *          This calibration data is produced internally by the MPL and its
 *          size is 2777 bytes (header and checksum included).
 *          Calibration format type 1 is currently used for ITG3500
 *
 *  @pre    inv_init_storage_manager()
 *          must have been called.
 *
 *  @param  calData
 *              A pointer to an array of bytes to be parsed.
 *  @param  len
 *              the length of the calibration
 *
 *  @return INV_SUCCESS if successful, a non-zero error code otherwise.
 */
inv_error_t inv_load_cal_V0(unsigned char *calData, size_t len)
{
    inv_error_t result;

    LOADCAL_LOG("Entering inv_load_cal_V0\n");

    /*if (len != expLen) {
        MPL_LOGE("Calibration data type 0 must be %d bytes long (got %d)\n",
                 expLen, len);
        return INV_ERROR_FILE_READ;
    }*/

    result = inv_load_mpl_states(calData, len);
    return result;
}

/**
 *  @brief  Loads a type 1 set of calibration data.
 *          It parses a binary data set containing calibration data.
 *          The binary data set is intended to be loaded from a file.
 *          This calibrations data format stores values for (in order of
 *          appearance) :
 *          - temperature,
 *          - gyro biases for X, Y, Z axes,
 *          - accel biases for X, Y, Z axes.
 *          This calibration data would normally be produced by the MPU Self
 *          Test and its size is 36 bytes (header and checksum included).
 *          Calibration format type 1 is produced by the MPU Self Test and
 *          substitutes the type 0 : inv_load_cal_V0().
 *
 *  @pre    
 *
 *  @param  calData
 *              A pointer to an array of bytes to be parsed.
 *  @param  len
 *              the length of the calibration
 *
 *  @return INV_SUCCESS if successful, a non-zero error code otherwise.
 */
inv_error_t inv_load_cal_V1(unsigned char *calData, size_t len)
{
    return INV_SUCCESS;
}

/**
 * @brief   Loads a set of calibration data.
 *          It parses a binary data set containing calibration data.
 *          The binary data set is intended to be loaded from a file.
 *
 * @pre     
 *          
 *
 * @param   calData
 *              A pointer to an array of bytes to be parsed.
 *
 * @return  INV_SUCCESS if successful, a non-zero error code otherwise.
 */
inv_error_t inv_load_cal(unsigned char *calData)
{
    int calType = 0;
    int len = 0;
    //int ptr;
    //uint32_t chk = 0;
    //uint32_t cmp_chk = 0;

    /*load_func_t loaders[] = {
        inv_load_cal_V0,
        inv_load_cal_V1,
    };
    */

    inv_load_cal_V0(calData, len);

    /* read the header (type and len)
       len is the total record length including header and checksum */
    len = 0;
    len += 16777216L * ((int)calData[0]);
    len += 65536L * ((int)calData[1]);
    len += 256 * ((int)calData[2]);
    len += (int)calData[3];

    calType = ((int)calData[4]) * 256 + ((int)calData[5]);
    if (calType > 5) {
        MPL_LOGE("Unsupported calibration file format %d. "
                 "Valid types 0..5\n", calType);
        return INV_ERROR_INVALID_PARAMETER;
    }

    /* call the proper method to read in the data */
    //return loaders[calType] (calData, len);
    return 0;
}

/**
 *  @brief  Stores a set of calibration data.
 *          It generates a binary data set containing calibration data.
 *          The binary data set is intended to be stored into a file.
 *
 *  @pre    inv_dmp_open()
 *
 *  @param  calData
 *              A pointer to an array of bytes to be stored.
 *  @param  length
 *              The amount of bytes available in the array.
 *
 *  @return INV_SUCCESS if successful, a non-zero error code otherwise.
 */
inv_error_t inv_store_cal(unsigned char *calData, size_t length)
{
    inv_error_t res = 0;
    size_t size;

    STORECAL_LOG("Entering inv_store_cal\n");

    inv_get_mpl_state_size(&size);

    MPL_LOGI("inv_get_mpl_state_size() : size=%d", size);

    /* store data */
    res = inv_save_mpl_states(calData, size);
    if(res != 0)
    {
        MPL_LOGE("inv_save_mpl_states() failed");
    }

    STORECAL_LOG("Exiting inv_store_cal\n");
    return INV_SUCCESS;
}

/**
 *  @brief  Load a calibration file.
 *
 *  @pre    Must be in INV_STATE_DMP_OPENED state.
 *          inv_dmp_open() or inv_dmp_stop() must have been called.
 *          inv_dmp_start() and inv_dmp_close() must have <b>NOT</b>
 *          been called.
 *
 *  @return 0 or error code.
 */
inv_error_t inv_load_calibration(void)
{
    unsigned char *calData;
    inv_error_t result = 0;
    size_t length;

    inv_get_mpl_state_size(&length);
    if (length <= 0) {
        MPL_LOGE("Could not get file calibration length - "
                 "error %d - aborting\n", result);
        return result;
    }

    calData = (unsigned char *)inv_malloc(length);
    if (!calData) {
        MPL_LOGE("Could not allocate buffer of %d bytes - "
                 "aborting\n", length);
        return INV_ERROR_MEMORY_EXAUSTED;
    }

    result = inv_read_cal(calData, length);
    if(result != INV_SUCCESS) {
        MPL_LOGE("Could not load cal file - "
                 "aborting\n");
    }

    result = inv_load_mpl_states(calData, length);
    if (result != INV_SUCCESS) {
        MPL_LOGE("Could not load the calibration data - "
                 "error %d - aborting\n", result);
        goto free_mem_n_exit;
    }

free_mem_n_exit:
    inv_free(calData);
    return INV_SUCCESS;
}

/**
 *  @brief  Store runtime calibration data to a file
 *
 *  @pre    Must be in INV_STATE_DMP_OPENED state.
 *          inv_dmp_open() or inv_dmp_stop() must have been called.
 *          inv_dmp_start() and inv_dmp_close() must have <b>NOT</b>
 *          been called.
 *
 *  @return 0 or error code.
 */
inv_error_t inv_store_calibration(void)
{
    unsigned char *calData;
    inv_error_t result;
    size_t length;

    result = inv_get_mpl_state_size(&length);
    calData = (unsigned char *)inv_malloc(length);
    if (!calData) {
        MPL_LOGE("Could not allocate buffer of %d bytes - "
                 "aborting\n", length);
        return INV_ERROR_MEMORY_EXAUSTED;
    }
    else {
        MPL_LOGI("mpl state size = %d", length);
    }

    result = inv_save_mpl_states(calData, length);
    if (result != INV_SUCCESS) {
        MPL_LOGE("Could not save mpl states - "
                 "error %d - aborting\n", result);
        goto free_mem_n_exit;
    }
    else {
        MPL_LOGE("calData from inv_save_mpl_states, size=%d", 
                 strlen((char *)calData));
    }

    result = inv_write_cal(calData, length);
    if (result != INV_SUCCESS) {
        MPL_LOGE("Could not store calibrated data on file - "
                 "error %d - aborting\n", result);
        goto free_mem_n_exit;

    }

free_mem_n_exit:
    inv_free(calData);
    return INV_SUCCESS;
}

/**
 *  @}
 */
