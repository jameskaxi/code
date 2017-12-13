/*
 * nand_flash.c
 *
 *  Created on: 2017-11-15
 *      Author: wyk
 */
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "types.h"

#define IMAGE_ADDR 0x90000000
#define IMAGE_LEN 0x100000

/******************************************************************************
 * Function:    test_nand
 ******************************************************************************/
 void write_read_image(void)
{
    uint8_t 				* buf1, * buf2, * buf_orig;
    /*uint16_t 				i, sp_flag = 0;*/
    uint32_t				offset;
	PLATFORM_DEVICE_info 	*p_device;

	buf2 = (uint8_t*)IMAGE_ADDR;
	/* Open the device */
    p_device = platform_device_open(PLATFORM_DEVID_NAND512R3A2D, 0);
    if (p_device == NULL)
    {
       platform_write("test_nand: Could not open NAND device  errno = 0x%x \n", platform_errno);
    }

    platform_blocknpage_to_offset(p_device->handle, &offset, 0, 0);
    if (platform_device_read(p_device->handle, offset, buf2, IMAGE_LEN) != Platform_EOK)
    {
        platform_write("test_nand: Unable to read block %d page %d errno = 0x%x\n", 0, 0, platform_errno );

    }
    platform_device_close(p_device->handle);

    return;
}

