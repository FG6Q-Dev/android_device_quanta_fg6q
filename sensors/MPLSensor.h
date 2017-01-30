/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2012 Invensense, Inc.
 * Copyright (c) 2012-2013, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef ANDROID_MPL_SENSOR_H
#define ANDROID_MPL_SENSOR_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <poll.h>
#include <utils/Vector.h>
#include <utils/KeyedVector.h>
#include "sensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"
#include "CompassSensor.h"

#define ACCEL_THRESHOLD	                0.2
#define ACCEL_WINDOW_SIZE               10
#define IDLE_POLL_PERIOD                256000000 /* nanosecond */
#define DEFAULT_POLL_PERIOD             40000000 /* nanosecond */
/* The LPA_POLL_PERIOD_THRESHOLD is the threshold of when the
 * Low Power Accelerometer is enabled.  When the accelerometer is the only
 * enabled device and its polling delay is >= this value, LPA is enabled.
 * The HW supports 25ms, 50ms, 200ms, and 800ms and will select the next
 * fastest delay for the polling delay requested.  In other words, if the
 * accelerometer polling delay is 100ms, the LPA delay of 50ms is used.
 * Because of this we'll want the power savings of this feature and happily
 * go down to the full 25ms by setting the threshold to 25ms.
 * A value of 0 disables the feature.
 */
#define LPA_POLL_PERIOD_THRESHOLD_US    (0) /* microsecond */
#define HW_MOTION_DETECT_THRESHOLD      (0)
/* If you want the MPU to go to standby instead of completely powering off,
 * then set MPL_PM_STDBY to 1.
 */
#define MPL_PM_STDBY                    (0)

/*****************************************************************************/
/* Sensors Enable/Disable Mask
 *****************************************************************************/
#define MAX_CHIP_ID_LEN             (20)

#define INV_THREE_AXIS_GYRO         (0x000F)
#define INV_THREE_AXIS_ACCEL        (0x0070)
#define INV_THREE_AXIS_COMPASS      (0x0380)
#define INV_ALL_SENSORS             (0x7FFF)

#ifdef INVENSENSE_COMPASS_CAL
#define ALL_MPL_SENSORS_NP          (INV_THREE_AXIS_ACCEL \
                                      | INV_THREE_AXIS_COMPASS \
                                      | INV_THREE_AXIS_GYRO)
#else
// TODO: ID_M = 2 even for 3rd-party solution
#define ALL_MPL_SENSORS_NP          (INV_THREE_AXIS_ACCEL \
                                      | INV_THREE_AXIS_COMPASS \
                                      | INV_THREE_AXIS_GYRO)
#endif

/*****************************************************************************/
/** MPLSensor implementation which fits into the HAL example for crespo provided
 *  by Google.
 *  WARNING: there may only be one instance of MPLSensor, ever.
 */

class MPLSensor: public SensorBase
{
    typedef void (MPLSensor::*hfunc_t)(sensors_event_t*);

public:
    MPLSensor(CompassSensor *);
    virtual ~MPLSensor();

    enum
    {
        Gyro = 0,
        Accelerometer,
        MagneticField,
        Orientation,
        RotationVector,
        LinearAccel,
        Gravity,
        numSensors
    };

    virtual int setDelay(int32_t handle, int64_t ns);
    virtual int enable(int32_t handle, int enabled);
    int32_t getEnableMask() { return mEnabled; }

    virtual int readEvents(sensors_event_t *data, int count);
    virtual int getFd() const;
    virtual int getAccelFd() const;
    virtual int getCompassFd() const;
    virtual int getPollTime();
    virtual bool hasPendingEvents() const;
    virtual void sleepEvent();
    virtual void wakeEvent();
    int populateSensorList(struct sensor_t *list, int len);
    void cbProcData();

    //static pointer to the object that will handle callbacks
    static MPLSensor* gMPLSensor;

    //AKM HAL Integration
    //void set_compass(long ready, long x, long y, long z, long accuracy);
    int executeOnData(sensors_event_t* data, int count);
    int readAccelEvents(sensors_event_t* data, int count);
    int readCompassEvents(sensors_event_t* data, int count);

    int masterEnable(int en);

protected:
    CompassSensor *mCompassSensor;

    void gyroHandler(sensors_event_t *data);
    void accelHandler(sensors_event_t *data);
    void compassHandler(sensors_event_t *data);
    void aHandler(sensors_event_t *data);
    void rvHandler(sensors_event_t *data);
    void laHandler(sensors_event_t *data);
    void gravHandler(sensors_event_t *data);
    void orienHandler(sensors_event_t *data);
    void calcOrientationSensor(float *Rx, float *Val);
    virtual int update_delay();

    int writeAccelFifoRate(uint64_t ns);
    void resetAccelWindow();
    bool isDeviceOrientationStable();
    void updateAccelWindow(float accel0, float accel1, float accel2);

    void inv_set_device_properties();
    int inv_constructor_init();
    int inv_constructor_default_enable();
    int setGyroInitialState();
    int setAccelInitialState();
    int onPower(int en);
    int enableGyro(int en);
    int enableAccel(int en);
    int enableCompass(int en);
    void computeLocalSensorMask(int enabled_sensors);
    int enableSensors(unsigned long enableSensors, int en);
    int inv_read_gyro_buffer(int fd, short *data, long long *timestamp);
    int update_delay_sysfs_sensor(int fd, uint64_t ns);
    int inv_float_to_q16(float *fdata, long *ldata);
    int inv_long_to_q16(long *fdata, long *ldata);
    int inv_float_to_round(float *fdata, long *ldata);
    int inv_float_to_round2(float *fdata, short *sdata);
    int inv_read_temperature(long long *data);
    int inv_read_dmp_state(int fd);
    int inv_read_sensor_bias(int fd, long *data);
    void inv_get_sensors_orientation(void);
    int inv_init_sysfs_attributes(void);
    void setCompassDelay(int64_t ns);
    int lpa_delay_enable(unsigned long us);
    int motion_detect_enable(bool enable);

    int mNewData;   // flag indicating that the MPL calculated new output values
    int mDmpStarted;
    long mMasterSensorMask;
    long mLocalSensorMask;
    int mPollTime;
    bool mHaveGoodMpuCal;   // flag indicating that the cal file can be written
    int mGyroAccuracy;      // value indicating the quality of the gyro calibr.
    int mAccelAccuracy;     // value indicating the quality of the accel calibr.
    struct pollfd mPollFds[5];
    int mSampleCount;
    pthread_mutex_t mMplMutex;
    bool mIntegratedAccel;

    enum FILEHANDLES
    {
        MPUIRQ_FD,
        ACCELIRQ_FD,
        COMPASSIRQ_FD,
        TIMERIRQ_FD,
        FIFOIRQ_FD,
    };

    int mpu_int_fd;
    int accel_fd;
    int mpufifo_fd;
    int gyro_temperature_fd;

    uint32_t mEnabled;
    uint32_t mOldEnabledMask;
    sensors_event_t mPendingEvents[numSensors];
    uint64_t mDelays[numSensors];
    hfunc_t mHandlers[numSensors];
    short mCachedGyroData[3];
    long mCachedAccelData[3];
    long mCachedCompassData[3];
    bool mForceSleep;
    android::KeyedVector<int, int> mIrqFds;

    InputEventCircularReader mAccelInputReader;
    InputEventCircularReader mGyroInputReader;

    bool mFirstRead;
    short mTempScale;
    short mTempOffset;
    int64_t mTempCurrentTime;
    int mAccelScale;

    uint32_t mPendingMask;
    unsigned long mSensorMask;

    uint64_t mPollPeriod;
    int mAccelWindowIndex;
    bool mWindowFull;
    bool mAccelVariableRate;
    float mAccelWindowSum[3];
    float mAccelWindow[ACCEL_WINDOW_SIZE][3];

    char chip_ID[MAX_CHIP_ID_LEN];

    signed char mGyroOrientation[9];
    signed char mAccelOrientation[9];

    int64_t mSensorTimestamp;
    int64_t mCompassTimestamp;

    struct sysfs_attrbs {
       char *chip_enable;
       char *power_state;
       char *dmp_firmware;
       char *firmware_loaded;
       char *tap_on;
       char *key;
       char *self_test;
       char *temp_scale;
       char *temp_offset;
       char *temperature;

       char *gyro_fifo_enable;
       char *gyro_enable;
       char *gyro_delay;
       char *gyro_orient;

       char *accl_fifo_enable;
       char *accl_enable;
       char *accl_delay;
       char *accl_fsr;
       char *accl_bias;
       char *accl_orient;
       char *lpa_delay;
       char *mot_thr;
    } mpu;

    char *sysfs_names_ptr;

private:
    /* added for dynamic get sensor list */
    void fillAccel(const char* accel, struct sensor_t *list);
    void fillGyro(const char* gyro, struct sensor_t *list);
    void fillRV(struct sensor_t *list);
    void fillOrientation(struct sensor_t *list);
    void fillGravity(struct sensor_t *list);
    void fillLinearAccel(struct sensor_t *list);
    void storeCalibration();
    void loadDMPTap();
};

extern "C" {
    void setCallbackObject(MPLSensor*);
    MPLSensor *getCallbackObject();
}

#endif  // ANDROID_MPL_SENSOR_H
