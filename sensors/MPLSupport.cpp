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

int read_sysfs_int(char *filename, int *var)
{
    int res=0;
    FILE  *sysfsfp;

    sysfsfp = fopen(filename, "r");
    if (sysfsfp != NULL) {
        if (fscanf(sysfsfp, "%d\n", var) < 1) {
           ALOGE("HAL:ERR failed to read an int from %s.", filename);
           res = -EINVAL;
        }
        fclose(sysfsfp);
    } else {
        res = -errno;
        ALOGE("HAL:ERR open file %s to read with error %d", filename, res);
    }
    return res;
}

int write_sysfs_int(char *filename, int var)
{
    int res = 0;
    FILE  *sysfsfp;

    ALOGV_IF(SYSFS_VERBOSE, "HAL:sysfs:echo %d > %s (%lld)",
          var, filename, getTimestamp());
    sysfsfp = fopen(filename, "w");
    if (sysfsfp == NULL) {
        res = -errno;
        ALOGE("HAL:ERR open file %s to write with error %d", filename, res);
        return res;
    }
    int fpres, fcres = 0;
    fpres = fprintf(sysfsfp, "%d\n", var);
    /* fprintf() can succeed because it never actually writes to the
     * underlying sysfs node.
     */
    if (fpres < 0) {
       res = -errno;
       fclose(sysfsfp);
    } else {
        fcres = fclose(sysfsfp);
        /* Check for errors from: fflush(), write(), and close() */
        if (fcres < 0) {
            res = -errno;
        }
    }
    if (fpres < 0 || fcres < 0) {
        ALOGE("HAL:ERR failed to write %d to %s (err=%d) print/close=%d/%d",
            var, filename, res, fpres, fcres);
    }
    return res;
}
