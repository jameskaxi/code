/*
 * vpx4_dsp1_flash_drv.c
 *
 * based on emifflash_drv.c
 *
 *  Created on: 2012-10-26
 *      Author:
 */
#include <stdint.h>
#include <stdio.h>

#define FLASH_START     0x70000000
#define SECTOR_SIZE     0x20000
/* Erase a segment of Flash memory */
void FLASH_erase(uint32_t start)
{
    uint16_t *pdata;

   /* Start sector erase sequence */
    *( (uint16_t *)FLASH_START + 0x555 ) = 0x00AA; /* write unlock cycle 1 */
    *( (uint16_t *)FLASH_START + 0x2AA ) = 0x0055; /* write unlock cycle 2 */
    *( (uint16_t *)FLASH_START + 0x555 ) = 0x0080; /* write setup command */
    *( (uint16_t *)FLASH_START + 0x555 ) = 0x00AA; /* write additional unlock cycle 1 */
    *( (uint16_t *)FLASH_START + 0x2AA ) = 0x0055; /* write additional unlock cycle 2 */
    //*( (uint16_t *)sector_address ) = 0x0030; /* write sector erase command */
    /* Start erase at sector address */
    pdata = (uint16_t *)start;
    *pdata = 0x30;
    /* Wait for erase to complete */
    while (1)
    if (*pdata & 0x80)
        break;


      /* Put back in read mode */
      *((uint16_t *)FLASH_START) = 0xf0;
}

/* Write data to a data range in Flash */
void FLASH_write(uint32_t src, uint32_t dst, uint32_t length)
{
	uint16_t *psrc, *pdst;
    uint32_t i;

    /* Establish source and destination */
    psrc = (uint16_t *)src;
    pdst = (uint16_t *)dst;
    for (i = 0; i < length; i++)
    {
        // Program one 8-bit word
        *((uint16_t *)FLASH_START + 0x555) = 0x00AA;
        *((uint16_t *)FLASH_START + 0x2AA) = 0x0055;
        *((uint16_t *)FLASH_START + 0x555) = 0x00A0;
        *pdst = *psrc;

        // Wait for operation to complete
        while(1)
            if (*pdst == *psrc)
                break;

        pdst++;
        psrc++;
    }

    /* Put back in read mode */
   *((uint16_t *)FLASH_START) = 0xf0;
}







