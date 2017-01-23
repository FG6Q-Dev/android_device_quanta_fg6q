/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */
#ifndef INV_MESSAGE_LAYER_H__
#define INV_MESSAGE_LAYER_H__

#include "mltypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* Level 0 Type Messages */
    /** A motion event has occured */
#define INV_MSG_MOTION_EVENT    (0x01)
    /** A no motion event has occured */
#define INV_MSG_NO_MOTION_EVENT (0x02)
    /** A setting of the gyro bias has occured */
#define INV_MSG_NEW_GB_EVENT    (0x04)

    void inv_set_message(long set, long clear, int level);
    long inv_get_message_level_0(int clear);

#ifdef __cplusplus
}
#endif

#endif  // INV_MESSAGE_LAYER_H__
