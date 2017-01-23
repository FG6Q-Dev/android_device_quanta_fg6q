#include <MPLSupport.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#include "SensorBase.h"
#include <fcntl.h>

int inv_read_data(char *fname, long *data)
{
    VFUNC_LOG;

    char buf[sizeof(long) * 4];
    int count, fd;

    fd = open(fname, O_RDONLY);
    if(fd < 0) {
        ALOGE("HAL:Error opening %s", fname);
        return -1;
    }
    memset(buf, 0, sizeof(buf));
    count = read_attribute_sensor(fd, buf, sizeof(buf));
    if(count < 1) {
        close(fd);
        return -1;
    } else {
        count = sscanf(buf, "%ld", data);
        if(count)
            ALOGV_IF(EXTRA_VERBOSE, "HAL:Data= %ld", *data);
    }
    close(fd);

    return 0;
}

/* This one DOES NOT close FDs for you */
int read_attribute_sensor(int fd, char* data, unsigned int size)
{
    VFUNC_LOG;

    int count = 0;
    if (fd > 0) {
        count = pread(fd, data, size, 0);
        if(count < 1) {
            ALOGE("HAL:read fails with error code=%d", count);
        }
    }
    return count;
}

/**
 *  @brief  Enable a sensor through the sysfs file descriptor
 *          provided.
 *  @note   this function one closes FD after the write
 *  @param  fd
 *              the file descriptor to write into
 *  @param  en
 *              the value to write, typically 1 or 0
 *  @return the errno whenever applicable.
 */
int enable_sysfs_sensor(int fd, int en)
{
    VFUNC_LOG;

    int nb = -1;
    int err = 0;

    if (fd >= 0) {
        char buf[2];
        if (en) {
            buf[0] = '1';
            nb = write(fd, buf, 1);
        } else {
            buf[0] = '0';
            nb = write(fd, buf, 1);
        }
        buf[1] = '\0';

        if (nb <= 0) {
            err = errno;
            ALOGE("HAL:enable_sysfs_sensor - write %c returned %d (%s / %d)",
                 buf[0], nb, strerror(err), err);
        }
        close(fd);
    } else {
        ALOGV_IF(EXTRA_VERBOSE, "HAL:enable_sysfs_sensor - fd<0");
    }

    return err;
}

/* This one closes FDs for you */
int write_attribute_sensor(int fd, long data)
{
    VFUNC_LOG;

    int num_b = 0;

    if (fd >= 0) {
        char buf[80];
        sprintf(buf, "%ld", data);
        num_b = write(fd, buf, strlen(buf) + 1);
        if (num_b <= 0) {
            int err = errno;
            ALOGE("HAL:write fd %d returned '%s' (%d)", fd, strerror(err), err);
        } else {
            ALOGV_IF(EXTRA_VERBOSE, "HAL:fd=%d write attribute to %ld", fd, data);
        }
        close(fd);
    }

    return num_b;
}
