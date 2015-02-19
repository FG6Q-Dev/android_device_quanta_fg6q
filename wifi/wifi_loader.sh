#!/system/bin/sh

# Copyright (c) 2012-2013, NVIDIA CORPORATION.  All rights reserved.
#
# NVIDIA CORPORATION and its licensors retain all intellectual property
# and proprietary rights in and to this software, related documentation
# and any modifications thereto.  Any use, reproduction, disclosure or
# distribution of this software and related documentation without an express
# license agreement from NVIDIA CORPORATION is strictly prohibited.

# QIC modified version V0.3
# To force auto chip detect machanism
#
# History:
#
# V0.7 - 2013/11/27
#        Add regulatory revision information check for JP SKU
#
# V0.6 - 2013/11/12
#        Remove board id related part since we are no longer support A build PCB
#
# V0.5 - 2013/9/2
#        Revise regulatory check mechanism, check regulatory file in mfg partition first 
#        then check sku id file.
#
# V0.4 - 2013/8/26
#        Add regulatory domain check mechanism
#
# V0.3 - 2013/7/24
#        Add qic_macwriter.sh to create mfg needed MAC address information
#
# V0.2 - 2013/7/23
#        Add backward support to A2 board CR503
#
# V0.1 - Initial.
#        Remove the support of specific chip id, always use auto detect machanism 


#if [ $(getprop ro.boot.commchip_id) -gt 0 ]; then
#	echo "setting user configured value of WiFi chipset"
#	setprop wifi.commchip_id $(getprop ro.boot.commchip_id)
#	#increase the wmem default and wmem max size
#	echo 262144 > /proc/sys/net/core/wmem_default
#	echo 262144 > /proc/sys/net/core/wmem_max
#	exit
#fi

MAC_WRITER=/system/bin/macwriter.sh
#BOARD_ID_FILE=/proc/board_id
NVRAM_FILE=/data/misc/wifi/firmware/nvram.txt
COUNTRY_FILE=/mfg/skuid.cfg
REG_FILE=/mfg/conn/wifi/wlreg.cfg

# vendor id defines
BRCM=0x02d0
#TI=0x0097
#MRVL=0x02df
brcm_symlink=y

check_regulatory() {

	if [ -f $REG_FILE ]; then
		country=`cat $REG_FILE`
	elif  [ -f $COUNTRY_FILE ]; then
		country=`cat $COUNTRY_FILE`
	else
		/system/bin/log -t "wifiloader" -p i "$REG_FILE and $COUNTRY_FILE not exist!!"
		country="us"
	fi

	REG_REVISION="0"
	case $country in
		us)
			COUNTRY_CODE="US"
			;;
		eu)
			COUNTRY_CODE="EU"
			;;
		jp)
			COUNTRY_CODE="JP"
			;;
		jp5)
			COUNTRY_CODE="JP"
			REG_REVISION="5"
			;;
		tw)
			COUNTRY_CODE="TW"
			;;
		*)
			COUNTRY_CODE="US"
			;;
	esac

	/system/bin/log -t "wifiloader" -p i "Set country code to $COUNTRY_CODE in $NVRAM_FILE"
	# echo "Set countr code to $COUNTRY_CODE in $NVRAM_FILE"
	echo "ccode=$COUNTRY_CODE" >> $NVRAM_FILE
	echo "regrev=$REG_REVISION" >> $NVRAM_FILE

}

#find hardware used and assigned corresponding mmc interface used for wifi chip
mmc=mmc2
if [ $(getprop ro.product.name) == "cardhu" ]; then
	mmc=mmc1
fi

vendor=$(cat /sys/bus/sdio/devices/$mmc:0001:1/vendor)
device=$(cat /sys/bus/sdio/devices/$mmc:0001:1/device)

#if [ -f $BOARD_ID_FILE ]; then
#        board_id=`cat $BOARD_ID_FILE`
#else
#	board_id=""
#fi

#if [ "$board_id""x" = "x" ]; then
#	device="0x4324_SP"
#else
#	if [ "$board_id" = "00" ]; then
#		device="0x4324_SP"
#	fi
#fi

vendor_device="$vendor"_"$device"

#Read vendor and product idea enumerated during kernel boot
if [ -z "$(getprop persist.commchip_vendor)" ]; then
	/system/bin/log -t "wifiloader" -p i "persist.commchip_vendor not defined. Reading enumerated data"
	setprop persist.commchip_vendor $vendor
	setprop persist.commchip_device $device
elif [ $vendor_device = $(getprop persist.commchip_vendor)"_"$(getprop persist.commchip_device) ]; then
	/system/bin/log -t "wifiloader" -p i "persist.commchip_vendor defined by user. Using user-defined config"
	#check if symlinks are available; if available, do not create symlinks
	#this check is needed when data partition is remounted after encryption
	if [ -L /data/misc/wifi/firmware/fw_bcmdhd.bin ]; then
		brcm_symlink=n
	fi
else
	/system/bin/log -t "wifiloader" -p i "Comm chip replaced by user. reset symlinks if needed"
	if [ $vendor = $BRCM ]; then
		/system/bin/rm /data/misc/wifi/firmware/fw_bcmdhd.bin
		/system/bin/rm /data/misc/wifi/firmware/fw_bcmdhd_apsta.bin
		/system/bin/rm /data/misc/wifi/firmware/nvram.txt
		/system/bin/rm /data/misc/wifi/firmware/firmware_path
		if [ $device = "43341" ]; then
			/system/bin/rm /data/misc/wifi/firmware/fw_bcmdhd_a0.bin
			/system/bin/rm /data/misc/wifi/firmware/fw_bcmdhd_apsta_a0.bin
			/system/bin/rm /data/misc/wifi/firmware/nvram_43341_rev3.txt
			/system/bin/rm /data/misc/wifi/firmware/nvram_43341_rev4.txt
		fi

	fi
	setprop persist.commchip_vendor $vendor
	setprop persist.commchip_device $device
fi

#Find device and set configurations
#broadcomm comm chip
if [ $vendor = $BRCM ]; then
	if [ $device = "0x4329" ]; then
		/system/bin/log -t "wifiloader" -p i  "BCM4329 chip identified"
		chip="4329"
	elif [ $device = "0x4330" ]; then
		/system/bin/log -t "wifiloader" -p i  "BCM4330 chip identified"
		chip="4330"
	elif [ $device = "0x4334" ]; then
		/system/bin/log -t "wifiloader" -p i  "BCM3434 chip identified"
		chip="4334"
	elif [ $device = "0xa94d" ]; then
		/system/bin/log -t "wifiloader" -p i  "BCM43341 chip identified"
		chip="43341"
	elif [ $device = "0x4324" ]; then
		/system/bin/log -t "wifiloader" -p i  "BCM43241 chip identified"
		chip="43241"
	elif [ $device = "0x4324_SP" ]; then
                /system/bin/log -t "wifiloader" -p i  "BCM43241 chip identified(SP)"
                chip="43241_SP"
	elif [ $device = "0x4335" ]; then
                /system/bin/log -t "wifiloader" -p i  "BCM4335 chip identified"
                chip="4335"
	fi

	if [ $brcm_symlink = y ]; then
		if [ -f /system/vendor/firmware/bcm$chip/fw_bcmdhd_JPCH14.bin ]; then
			/system/bin/ln -s /system/vendor/firmware/bcm$chip/fw_bcmdhd_JPCH14.bin /data/misc/wifi/firmware/fw_bcmdhd_non_mfg.bin
			/system/bin/ln -s /system/vendor/firmware/bcm$chip/fw_bcmdhd_mfg_JPCH14.bin /data/misc/wifi/firmware/fw_bcmdhd_mfg.bin
		else
			/system/bin/ln -s /system/vendor/firmware/bcm$chip/fw_bcmdhd.bin /data/misc/wifi/firmware/fw_bcmdhd_non_mfg.bin
			/system/bin/ln -s /system/vendor/firmware/bcm$chip/fw_bcmdhd_mfg.bin /data/misc/wifi/firmware/fw_bcmdhd_mfg.bin
		fi
		/system/bin/ln -s /data/misc/wifi/firmware/fw_bcmdhd_non_mfg.bin /data/misc/wifi/firmware/fw_bcmdhd.bin
		/system/bin/ln -s /system/vendor/firmware/bcm$chip/fw_bcmdhd.bin /data/misc/wifi/firmware/fw_bcmdhd_apsta.bin
		if [ $chip = "43341" ]; then
			/system/bin/ln -s /system/vendor/firmware/bcm$chip/fw_bcmdhd_a0.bin /data/misc/wifi/firmware/fw_bcmdhd_a0.bin
			/system/bin/ln -s /system/vendor/firmware/bcm$chip/fw_bcmdhd_a0.bin /data/misc/wifi/firmware/fw_bcmdhd_apsta_a0.bin
			/system/bin/ln -s /system/etc/nvram_rev2.txt /data/misc/wifi/firmware/nvram.txt
			/system/bin/ln -s /system/etc/nvram_rev3.txt /data/misc/wifi/firmware/nvram_43341_rev3.txt
			/system/bin/ln -s /system/etc/nvram_rev4.txt /data/misc/wifi/firmware/nvram_43341_rev4.txt
		else
			/system/bin/ln -s /system/etc/nvram_$chip.txt /data/misc/wifi/firmware/nvram.txt
		fi

	fi

	# nvram.txt override for country check
	rm -f $NVRAM_FILE
	cp /system/etc/nvram_$chip.txt $NVRAM_FILE
	chmod 644 $NVRAM_FILE
	check_regulatory

	insmod /system/lib/modules/cfg80211.ko
	if [  $chip = "4335" ]; then
		insmod /system/lib/modules/bcm4335.ko
		/system/bin/ln -s /sys/module/bcm4335/parameters/firmware_path /data/misc/wifi/firmware/firmware_path
	elif [  $chip = "43341" ]; then
                insmod /system/lib/modules/bcm43341.ko
                /system/bin/ln -s /sys/module/bcm43341/parameters/firmware_path /data/misc/wifi/firmware/firmware_path
	elif [  $chip = "43241" ]; then
                insmod /system/lib/modules/bcm43241.ko
                /system/bin/ln -s /sys/module/bcm43241/parameters/firmware_path /data/misc/wifi/firmware/firmware_path
	elif [  $chip = "43241_SP" ]; then
                insmod /system/lib/modules/bcm43241.ko
                /system/bin/ln -s /sys/module/bcm43241/parameters/firmware_path /data/misc/wifi/firmware/firmware_path
	else
		insmod /system/lib/modules/bcmdhd.ko
		/system/bin/ln -s /sys/module/bcmdhd/parameters/firmware_path /data/misc/wifi/firmware/firmware_path
	fi
	setprop wifi.supplicant wpa_suppl_nl
fi

# Create MAC address file and determin module maker
$MAC_WRITER

# Copy BT mac address file
cp /mfg/conn/bt/bdaddr /data/misc/bluetooth
chmod 644 /data/misc/bluetooth/bdaddr

#increase the wmem default and wmem max size
echo 262144 > /proc/sys/net/core/wmem_default
echo 262144 > /proc/sys/net/core/wmem_max

#sleep for -1 timeout, so that wifiloader can be run as daemon
#sleep 4294967295
