#include <string.h>
#include <stdio.h>
#include "ml_sysfs_helper.h"

#define MPU_SYSFS_ABS_PATH "/sys/class/invensense/mpu"

#define CHIP_NUM 4
enum PROC_SYSFS_CMD {
	CMD_GET_SYSFS_PATH,
	CMD_GET_DMP_PATH,
	CMD_GET_CHIP_NAME,
	CMD_GET_SYSFS_KEY
};
static char sysfs_path[100];
static char *chip_name[CHIP_NUM] = {"ITG3500", "MPU6050", "MPU9150", "MPU3050"};
static int chip_ind;
static int initialized =0;
static int status = 0;

/* mode 0: search for which chip in the system and fill sysfs path
   mode 1: return event number
 */
static int parsing_proc_input(int mode, char *name){
	const char input[] = "/proc/bus/input/devices";
	char line[100], d;
	char tmp[100];
	FILE *fp;
	int i, j, result, find_flag;
	int event_number = -1;
	int input_number = -1;

	if(NULL == (fp = fopen(input, "rt")) ){
		return -1;
	}
	result = 1;
	find_flag = 0;
	while(result != 0 && find_flag < 2){
		i = 0;
		d = 0;
		memset(line, 0, 100);
		while(d != '\n'){		
			result = fread(&d, 1, 1, fp);
			if(result == 0){
				line[0] = 0;
				break;
			}
			sprintf(&line[i], "%c", d);		
			i ++;
		}
		if(line[0] == 'N'){
			i = 1;
			while(line[i] != '"'){
				i++;
			}
			i++;
			j = 0;
			find_flag = 0;
			if (mode == 0){
				while(j < CHIP_NUM){
					if(!memcmp(&line[i], chip_name[j], strlen(chip_name[j]))){
						find_flag = 1;
						chip_ind = j;
					}
					j++;
				}
			} else if (mode  != 0){
				if(!memcmp(&line[i], name, strlen(name))){
					find_flag = 1;
				}
			}
		}		
		if(find_flag){
			if(mode == 0){
				if(line[0] == 'S'){
					memset(tmp, 0, 100);
					i =1;
					while(line[i] != '=') i++;
					i++;
					j = 0;
					while(line[i] != '\n'){
						tmp[j] = line[i];
						i ++; j++;
					}	
					sprintf(sysfs_path, "%s%s", "/sys", tmp);
					find_flag++;
				}
			} else if(mode == 1){
				if(line[0] == 'H') {
					i = 2;
					while(line[i] != '=') i++;
					while(line[i] != 't') i++;	
					i++;
					event_number = 0;
					while(line[i] != '\n'){
						if(line[i] >= '0' && line[i] <= '9')
							event_number = event_number*10 + line[i]-0x30;
						i ++;
					}
					find_flag ++;
				}
			} else if (mode == 2) {
				if(line[0] == 'S'){
					memset(tmp, 0, 100);
					i =1;
					while(line[i] != '=') i++;
					i++;
					j = 0;
					while(line[i] != '\n'){
						tmp[j] = line[i];
						i ++; j++;
					}
					input_number = 0;
					if(tmp[j-2] >= '0' && tmp[j-2] <= '9') 
						input_number += (tmp[j-2]-0x30)*10;
					if(tmp[j-1] >= '0' && tmp[j-1] <= '9') 
						input_number += (tmp[j-1]-0x30);
					find_flag++;
				}
			}
		}
	}
	fclose(fp);
	if(find_flag == 0){
		return -1;
	}
	if(0 == mode)
		status = 1;
	if (mode == 1)
		return event_number;
	if (mode == 2)
		return input_number;
	return 0;

}
static int process_sysfs_request(enum PROC_SYSFS_CMD cmd, char *data)
{
	char key_path[100];
	FILE *fp;
	int i, result;
	if(initialized == 0){
		parsing_proc_input(0, NULL);
		initialized = 1;
	}
	if(initialized && status == 0)
		return -1;

	memset(key_path, 0, 100);
	switch(cmd){
	case CMD_GET_SYSFS_PATH:
		sprintf(data, "%s%s", sysfs_path, "/device/invensense/mpu");
		break;
	case CMD_GET_DMP_PATH:
		sprintf(data, "%s%s", sysfs_path, "/device/invensense/mpu/dmp_firmware");
		break;
	case CMD_GET_CHIP_NAME:
		sprintf(data, "%s", chip_name[chip_ind]);
		break;
	case CMD_GET_SYSFS_KEY:
		memset(key_path, 0, 100);
		sprintf(key_path, "%s%s", sysfs_path, "/device/invensense/mpu/key");
		if((fp = fopen(key_path, "rt")) == NULL)
			return -1;
		for(i=0;i<16;i++){
			fscanf(fp, "%02x", &result);
			data[i] = (char)result;
		}
		
		fclose(fp);
		break;
	default:
		break;
	}
	return 0;
}
/** 
 *  @brief  return sysfs key. if the key is not available
 *          return false. So the return value must be checked
 *          to make sure the path is valid.
 *  @unsigned char *name: This should be array big enough to hold the key
 *           It should be zeroed before calling this function.
 *           Or it could have unpredicable result.
 */
inv_error_t inv_get_sysfs_key(unsigned char *key)
{
	if (process_sysfs_request(CMD_GET_SYSFS_KEY, (char*)key) < 0)
		return INV_ERROR_NOT_OPENED;
	else
		return INV_SUCCESS;
}

/** 
 *  @brief  return the sysfs path. If the path is not 
 *          found yet. return false. So the return value must be checked
 *          to make sure the path is valid.
 *  @unsigned char *name: This should be array big enough to hold the sysfs
 *           path. It should be zeroed before calling this function.
 *           Or it could have unpredicable result.
 */
inv_error_t inv_get_sysfs_path(char *name)
{
	if (process_sysfs_request(CMD_GET_SYSFS_PATH, name) < 0)
		return INV_ERROR_NOT_OPENED;
	else
		return INV_SUCCESS;
}

inv_error_t inv_get_sysfs_abs_path(char *name)
{
    strcpy(name, MPU_SYSFS_ABS_PATH);
    return INV_SUCCESS;
}

/** 
 *  @brief  return the dmp file path. If the path is not 
 *          found yet. return false. So the return value must be checked
 *          to make sure the path is valid.
 *  @unsigned char *name: This should be array big enough to hold the dmp file
 *           path. It should be zeroed before calling this function.
 *           Or it could have unpredicable result.
 */
inv_error_t inv_get_dmpfile(char *name)
{
   	if (process_sysfs_request(CMD_GET_DMP_PATH, name) < 0)
		return INV_ERROR_NOT_OPENED;
	else
		return INV_SUCCESS;
}
/** 
 *  @brief  return the chip name. If the chip is not 
 *          found yet. return false. So the return value must be checked
 *          to make sure the path is valid.
 *  @unsigned char *name: This should be array big enough to hold the chip name
 *           path(8 bytes). It should be zeroed before calling this function.
 *           Or it could have unpredicable result.
 */
inv_error_t inv_get_chip_name(char *name)
{
   	if (process_sysfs_request(CMD_GET_CHIP_NAME, name) < 0)
		return INV_ERROR_NOT_OPENED;
	else
		return INV_SUCCESS;
}
/** 
 *  @brief  return event handler number. If the handler number is not found
 *          return false. the return value must be checked
 *          to make sure the path is valid.
 *  @unsigned char *name: This should be array big enough to hold the chip name
 *           path(8 bytes). It should be zeroed before calling this function.
 *           Or it could have unpredicable result.
 *  @int *num: event number store
 */
inv_error_t  inv_get_handler_number(const char *name, int *num)
{
	initialized = 0;
	if ((*num = parsing_proc_input(1, (char *)name)) < 0)
		return INV_ERROR_NOT_OPENED;
	else
		return INV_SUCCESS;	
}

/** 
 *  @brief  return input number. If the handler number is not found
 *          return false. the return value must be checked
 *          to make sure the path is valid.
 *  @unsigned char *name: This should be array big enough to hold the chip name
 *           path(8 bytes). It should be zeroed before calling this function.
 *           Or it could have unpredicable result.
 *  @int *num: input number store
 */
inv_error_t  inv_get_input_number(const char *name, int *num)
{
	initialized = 0;
	if ((*num = parsing_proc_input(2, (char *)name)) < 0)
		return INV_ERROR_NOT_OPENED;
	else {
		return INV_SUCCESS;
	}	
}


