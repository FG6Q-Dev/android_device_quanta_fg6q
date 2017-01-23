/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */
 
/**
 *   @defgroup  HAL_Outputs hal_outputs
 *   @brief     Motion Library - HAL Outputs
 *              Sets up common outputs for HAL
 *
 *   @{
 *       @file hal_outputs.c
 *       @brief HAL Outputs.
 */
#include "hal_outputs.h"
#include "log.h"
#include "ml_math_func.h"
#include "mlmath.h"
#include "start_manager.h"
#include "data_builder.h"
#include "results_holder.h"

struct hal_output_t {
    int accuracy_mag; /**< Compass accuracy */
    int accuracy_gyro; /**< Gyro Accuracy */
    int accuracy_accel;	/**< Accel Accuracy */
    inv_time_t nav_timestamp;
    inv_time_t gam_timestamp;
    inv_time_t accel_timestamp;
    long nav_quat[4];
};

static struct hal_output_t hal_out;

/** Acceleration (m/s^2) in body frame.
* @param[out] values Acceleration in m/s^2 includes gravity. So while not in motion, it
*             should return a vector of magnitude near 9.81 m/s^2
* @param[out] accuracy Accuracy of the measurment, 0 is least accurate, while 3 is most accurate.
* @param[out] timestamp The timestamp for this sensor. Derived from the timestamp sent to
*             inv_build_accel().
*/
void inv_get_sensor_type_accelerometer(float *values, int8_t *accuracy,
                                       inv_time_t * timestamp)
{
    /* Converts fixed point to m/s^2. Fixed point has 1g = 2^16.
     * So this 9.80665 / 2^16 */
#define ACCEL_CONVERSION 0.000149637603759766f
    long accel[3];
    inv_get_accel_set(accel, accuracy, timestamp);
    values[0] = accel[0] * ACCEL_CONVERSION;
    values[1] = accel[1] * ACCEL_CONVERSION;
    values[2] = accel[2] * ACCEL_CONVERSION;
}

/** Linear Acceleration (m/s^2) in Body Frame.
* @param[out] values Linear Acceleration in body frame, length 3, (m/s^2). May show
*             accel biases while at rest.
* @param[out] accuracy Accuracy of the measurment, 0 is least accurate, while 3 is most accurate.
* @param[out] timestamp The timestamp for this sensor. Derived from the timestamp sent to
*             inv_build_accel().
*/
void inv_get_sensor_type_linear_acceleration(float *values, int8_t *accuracy,
        inv_time_t * timestamp)
{
    long gravity[3], accel[3];

    inv_get_accel_set(accel, accuracy, timestamp);
    inv_get_gravity(gravity);
    accel[0] -= gravity[0] >> 14;
    accel[1] -= gravity[1] >> 14;
    accel[2] -= gravity[2] >> 14;
    values[0] = accel[0] * ACCEL_CONVERSION;
    values[1] = accel[1] * ACCEL_CONVERSION;
    values[2] = accel[2] * ACCEL_CONVERSION;
}

/** Gravity vector (m/s^2) in Body Frame.
* @param[out] values Gravity vector in body frame, length 3, (m/s^2)
* @param[out] accuracy Accuracy of the measurment, 0 is least accurate, while 3 is most accurate.
* @param[out] timestamp The timestamp for this sensor. Derived from the timestamp sent to
*             inv_build_accel().
*/
void inv_get_sensor_type_gravity(float *values, int8_t *accuracy,
                                 inv_time_t * timestamp)
{
    long gravity[3];

    *accuracy = hal_out.accuracy_mag;
    *timestamp = hal_out.nav_timestamp;
    inv_get_gravity(gravity);
    values[0] = (gravity[0] >> 14) * ACCEL_CONVERSION;
    values[1] = (gravity[1] >> 14) * ACCEL_CONVERSION;
    values[2] = (gravity[2] >> 14) * ACCEL_CONVERSION;
}

/** Gyroscope data (rad/s) in body frame.
* @param[out] values Rotation Rate in rad/sec.
* @param[out] accuracy Accuracy of the measurment, 0 is least accurate, while 3 is most accurate.
* @param[out] timestamp The timestamp for this sensor. Derived from the timestamp sent to
*             inv_build_gyro().
*/
void inv_get_sensor_type_gyroscope(float *values, int8_t *accuracy,
                                   inv_time_t * timestamp)
{
    /* Converts fixed point to rad/sec. Fixed point has 1 dps = 2^16.
     * So this is: pi / 2^16 / 180 */
#define GYRO_CONVERSION 2.66316109007924e-007f
    long gyro[3];
    inv_get_gyro_set(gyro, accuracy, timestamp);
    values[0] = gyro[0] * GYRO_CONVERSION;
    values[1] = gyro[1] * GYRO_CONVERSION;
    values[2] = gyro[2] * GYRO_CONVERSION;
}

/**
* This corresponds to Sensor.TYPE_ROTATION_VECTOR.
* The rotation vector represents the orientation of the device as a combination
* of an angle and an axis, in which the device has rotated through an angle @f$\theta@f$
* around an axis {x, y, z}. <br>
* The three elements of the rotation vector are
* {x*sin(@f$\theta@f$/2), y*sin(@f$\theta@f$/2), z*sin(@f$\theta@f$/2)}, such that the magnitude of the rotation
* vector is equal to sin(@f$\theta@f$/2), and the direction of the rotation vector is
* equal to the direction of the axis of rotation.
*
* The three elements of the rotation vector are equal to the last three components of a unit quaternion
* {x*sin(@f$\theta@f$/2), y*sin(@f$\theta@f$/2), z*sin(@f$\theta@f$/2)>. The 4th element is cos(@f$\theta@f$/2).
*
* Elements of the rotation vector are unitless. The x,y and z axis are defined in the same way as the acceleration sensor.
* The reference coordinate system is defined as a direct orthonormal basis, where:

    -X is defined as the vector product Y.Z (It is tangential to the ground at the device's current location and roughly points East).
    -Y is tangential to the ground at the device's current location and points towards the magnetic North Pole.
    -Z points towards the sky and is perpendicular to the ground.
* @param[out] values Length 4.
* @param[out] accuracy Accuracy 0 to 3, 3 = most accurate
* @param[out] timestamp Timestamp. In (ns) for Android.
*/
void inv_get_sensor_type_rotation_vector(float *values, int8_t *accuracy,
        inv_time_t * timestamp)
{
    *accuracy = hal_out.accuracy_mag;
    *timestamp = hal_out.nav_timestamp;

    if (hal_out.nav_quat[0] >= 0) {
        values[0] = hal_out.nav_quat[1] * INV_TWO_POWER_NEG_30;
        values[1] = hal_out.nav_quat[2] * INV_TWO_POWER_NEG_30;
        values[2] = hal_out.nav_quat[3] * INV_TWO_POWER_NEG_30;
        values[3] = hal_out.nav_quat[0] * INV_TWO_POWER_NEG_30;
    } else {
        values[0] = -hal_out.nav_quat[1] * INV_TWO_POWER_NEG_30;
        values[1] = -hal_out.nav_quat[2] * INV_TWO_POWER_NEG_30;
        values[2] = -hal_out.nav_quat[3] * INV_TWO_POWER_NEG_30;
        values[3] = -hal_out.nav_quat[0] * INV_TWO_POWER_NEG_30;
    }
}


/** Compass data (uT) in body frame.
* @param[out] values Compass data in (uT), length 3. May be calibrated by having
*             biases removed and sensitivity adjusted
* @param[out] accuracy Accuracy 0 to 3, 3 = most accurate
* @param[out] timestamp Timestamp. In (ns) for Android.
*/
void inv_get_sensor_type_magnetic_field(float *values, int8_t *accuracy,
                                        inv_time_t * timestamp)
{
    /* Converts fixed point to uT. Fixed point has 1 uT = 2^16.
     * So this is: 1 / 2^16*/
#define COMPASS_CONVERSION 1.52587890625e-005f
    long compass[3];
    inv_get_compass_set(compass, accuracy, timestamp);
    values[0] = compass[0] * COMPASS_CONVERSION;
    values[1] = compass[1] * COMPASS_CONVERSION;
    values[2] = compass[2] * COMPASS_CONVERSION;
}


/** This corresponds to Sensor.TYPE_ORIENTATION. All values are angles in degrees.
* @param[out] values Length 3, Degrees.<br>
*        - values[0]: Azimuth, angle between the magnetic north direction
*         and the y-axis, around the z-axis (0 to 359). 0=North, 90=East, 180=South, 270=West<br>
*        - values[1]: Pitch, rotation around x-axis (-180 to 180), with positive values
*         when the z-axis moves toward the y-axis.<br>
*        - values[2]: Roll, rotation around y-axis (-90 to 90), with positive
*          values when the x-axis moves toward the z-axis.<br>
*
* @note  This definition is different from yaw, pitch and roll used in aviation
*        where the X axis is along the long side of the plane (tail to nose).
*        Note: This sensor type exists for legacy reasons, please use getRotationMatrix()
*        in conjunction with remapCoordinateSystem() and getOrientation() to compute
*        these values instead.
*        Important note: For historical reasons the roll angle is positive in the
*        clockwise direction (mathematically speaking, it should be positive in
*        the counter-clockwise direction).
* @param[out] accuracy Accuracy of the measurment, 0 is least accurate, while 3 is most accurate.
* @param[out] timestamp The timestamp for this sensor.
*/
void inv_get_sensor_type_orientation(float *values, int8_t *accuracy,
                                     inv_time_t * timestamp)
{
    long t1, t2, t3;
    long q00, q01, q02, q03, q11, q12, q13, q22, q23, q33;

    *accuracy = hal_out.accuracy_mag;
    *timestamp = hal_out.nav_timestamp;

    q00 = inv_q29_mult(hal_out.nav_quat[0], hal_out.nav_quat[0]);
    q01 = inv_q29_mult(hal_out.nav_quat[0], hal_out.nav_quat[1]);
    q02 = inv_q29_mult(hal_out.nav_quat[0], hal_out.nav_quat[2]);
    q03 = inv_q29_mult(hal_out.nav_quat[0], hal_out.nav_quat[3]);
    q11 = inv_q29_mult(hal_out.nav_quat[1], hal_out.nav_quat[1]);
    q12 = inv_q29_mult(hal_out.nav_quat[1], hal_out.nav_quat[2]);
    q13 = inv_q29_mult(hal_out.nav_quat[1], hal_out.nav_quat[3]);
    q22 = inv_q29_mult(hal_out.nav_quat[2], hal_out.nav_quat[2]);
    q23 = inv_q29_mult(hal_out.nav_quat[2], hal_out.nav_quat[3]);
    q33 = inv_q29_mult(hal_out.nav_quat[3], hal_out.nav_quat[3]);

    /* X component of the Ybody axis in World frame */
    t1 = q12 - q03;

    /* Y component of the Ybody axis in World frame */
    t2 = q22 + q00 - (1L << 30);
    values[0] = atan2f((float) t1, (float) t2) * 180.f / (float) M_PI;
    if (values[0] < 0)
        values[0] += 360;

    /* Z component of the Ybody axis in World frame */
    t3 = q23 + q01;
    values[1] =
        -atan2f((float) t3,
                sqrtf((float) t1 * t1 +
                      (float) t2 * t2)) * 180.f / (float) M_PI;
    /* Z component of the Zbody axis in World frame */
    t2 = q33 + q00 - (1L << 30);
    if (t2 < 0) {
        if (values[1] >= 0)
            values[1] = 180.f - values[1];
        else
            values[1] = -180.f - values[1];
    }

    /* X component of the Xbody axis in World frame */
    t1 = q11 + q00 - (1L << 30);
    /* Y component of the Xbody axis in World frame */
    t2 = q12 + q03;
    /* Z component of the Xbody axis in World frame */
    t3 = q13 - q02;
    //values[2] = atan2f((float)t3,sqrtf((float)t1*t1+(float)t2*t2))*180.f/(float)M_PI;

    values[2] =
        -(atan2f((float)(q33 + q00 - (1L << 30)), (float)(q13 - q02)) *
          180.f / (float) M_PI - 90);
    if (values[2] >= 90)
        values[2] = 180 - values[2];

    if (values[2] < -90)
        values[2] = -180 - values[2];

}

/** Main callback to generate HAL outputs. Typically not called by library users.
* @param[in] sensor_cal Input variable to take sensor data whenever there is new 
* sensor data.
* @return Returns INV_SUCCESS if successful or an error code if not.
*/
inv_error_t inv_generate_hal_outputs(struct inv_sensor_cal_t *sensor_cal)
{
    (void) sensor_cal;
    inv_get_quaternion_set(hal_out.nav_quat, &hal_out.accuracy_mag,
                           &hal_out.nav_timestamp);
    return INV_SUCCESS;
}

/** Turns off generation of HAL outputs.
* @return Returns INV_SUCCESS if successful or an error code if not.
 */
inv_error_t inv_stop_hal_outputs(void)
{
    inv_error_t result;
    result = inv_unregister_data_cb(inv_generate_hal_outputs);
    return result;
}

/** Turns on generation of HAL outputs. This should be called after inv_stop_hal_outputs()
* to turn generation of HAL outputs back on. It is automatically called by inv_enable_hal_outputs().
* @return Returns INV_SUCCESS if successful or an error code if not.
*/
inv_error_t inv_start_hal_outputs(void)
{
    inv_error_t result;
    result =
        inv_register_data_cb(inv_generate_hal_outputs,
                             INV_PRIORITY_HAL_OUTPUTS,
                             INV_GYRO_NEW | INV_ACCEL_NEW | INV_MAG_NEW);
    return result;
}

/** Initializes hal outputs class. This is called automatically by the
* enable function. It may be called any time the feature is enabled, but
* is typically not needed to be called by outside callers.
* @return Returns INV_SUCCESS if successful or an error code if not.
*/
inv_error_t inv_init_hal_outputs(void)
{
    memset(&hal_out, 0, sizeof(hal_out));
    return INV_SUCCESS;
}

/** Turns on creation and storage of HAL type results.
* @return Returns INV_SUCCESS if successful or an error code if not.
*/
inv_error_t inv_enable_hal_outputs(void)
{
    inv_error_t result;
    result = inv_init_hal_outputs();
    if (result) {
        return result;
    }

    result = inv_register_mpl_start_notification(inv_start_hal_outputs);
    return result;
}

/** Turns off creation and storage of HAL type results.
*/
inv_error_t inv_disable_hal_outputs(void)
{
    inv_error_t result;

    result = inv_stop_hal_outputs();
    // Ignore error if we have already stopped this
    if (result == INV_ERROR_INVALID_PARAMETER)
        result = INV_SUCCESS;
    result = inv_unregister_mpl_start_notification(inv_start_hal_outputs);
    return result;
}

/**
 * @}
 */
