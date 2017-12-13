/*
 * main.c
 *
 *  Created on: 2015-12-8
 *      Author: Administrator
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <c6x.h>
#include <ti/csl/csl_bootcfgAux.h>
#include <ti/csl/csl_ipcAux.h>
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>
#include "platform.h"
#include "resource_mgr.h"
#include "platform_internal.h"

extern _boot();
//extern _bootcore0();
#define CORENUM 8
extern unsigned int _c_int00;
#define IPC_INTERRUPT_CORE0 0x02620240
#define BOOT_MAGIC_ADDRESS (0x880000 - 4)
/*convert local address to global address for EDMA on multi-core DSP*/
#define GLOBAL_ADDR(addr,corenumber) (unsigned int)addr<0x1000000?\
						(unsigned int)addr+(0x10000000+corenumber*0x1000000):\
						(unsigned int)addr

void bootOtherCore(void)
{
	unsigned int coreNumber;
	unsigned int *bootMagicAddr;

	CSL_BootCfgUnlockKicker();
	for(coreNumber=1;coreNumber<CORENUM;coreNumber++)
	{
		bootMagicAddr = (unsigned int *)(GLOBAL_ADDR(BOOT_MAGIC_ADDRESS,coreNumber));
		*bootMagicAddr = (0x10800000 | coreNumber<<24);
		CSL_IPC_genGEMInterrupt(coreNumber,0);//start cores..
	}
	CSL_BootCfgLockKicker();
}
/* OSAL functions for Platform Library */
uint8_t *Osal_platformMalloc (uint32_t num_bytes, uint32_t alignment)
{
	return malloc(num_bytes);
}

void Osal_platformFree (uint8_t *dataPtr, uint32_t num_bytes)
{
    /* Free up the memory */
    if (dataPtr)
    {
        free(dataPtr);
    }
}

void Osal_platformSpiCsEnter(void)
{
    /* Get the hardware semaphore.
     *
     * Acquire Multi core CPPI synchronization lock
     */
    while ((CSL_semAcquireDirect (PLATFORM_SPI_HW_SEM)) == 0);

    return;
}

void Osal_platformSpiCsExit (void)
{
    /* Release the hardware semaphore
     *
     * Release multi-core lock.
     */
    CSL_semReleaseSemaphore (PLATFORM_SPI_HW_SEM);

    return;
}

#define IMAGE_ADDR 0x90000000

void boot_bltb(void)
{
	void (*entry)(void);

	uint32_t bootEntry, secLen, secAddr, i;
	uint32_t offset = 0;

	bootEntry = *(uint32_t *)(IMAGE_ADDR);
	offset +=4;
	while(1)
	{
		secLen = *(uint32_t*)(IMAGE_ADDR + offset);
		if(secLen > 0 && secLen != 0xffffffff)
		{
			offset += 4;
			secAddr = *(uint32_t*)(IMAGE_ADDR + offset);
			offset += 4;
			for(i=0; i<secLen; i++)
				*(uint8_t*)(secAddr+i) = *(uint8_t*)(IMAGE_ADDR + offset + i);
			if(secLen%4)
				offset += 2;

			offset += secLen;
		}else
			break;
	}
	entry = (void (*)())bootEntry;
	(*entry)();
}

void main(void)
{
    platform_init_flags  init_flags;
    platform_init_config init_config;
    int i;
	if(DNUM == 0)
	{
	    memset(&init_flags, 0x01, sizeof(platform_init_flags));
	    memset(&init_config, 0x00, sizeof(platform_init_config));
	    init_flags.phy = 0;
	    if(platform_init(&init_flags, &init_config) != Platform_EOK)
	    	printf("Platform_init failed!\n");
	    platform_write_configure(PLATFORM_WRITE_ALL);
	    platform_uart_init();
	    gpioInit();
	    for(i=0; i<4; i++)
	    	gpioSetOutput(i);
		write_read_image();
		bootOtherCore();
	    for(i=0; i<4; i++)
	    	gpioClearOutput(i);
		//NandFlash read image
	}
	SetupXMC();

	boot_bltb();
}


