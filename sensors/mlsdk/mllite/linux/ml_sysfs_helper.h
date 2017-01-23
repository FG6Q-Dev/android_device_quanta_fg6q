/*
 $License:
    Copyright (C) 2011 InvenSense Corporation, All Rights Reserved.
 $
 */

/*******************************************************************************
 *
 * $Id$
 *
 ******************************************************************************/

#ifndef MLDMP_SYSFS_HELPER_H__
#define MLDMP_SYSFS_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "invensense.h"

inv_error_t inv_get_sysfs_path(char *name);
inv_error_t inv_get_sysfs_abs_path(char *name);
inv_error_t inv_get_dmpfile(char *name);
inv_error_t inv_get_chip_name(char *name);
inv_error_t inv_get_sysfs_key(unsigned char *key);
inv_error_t inv_get_handler_number(const char *name, int *num);
inv_error_t  inv_get_input_number(const char *name, int *num);

#ifdef __cplusplus
}
#endif
#endif	/* MLDMP_SYSFS_HELPER_H__ */
