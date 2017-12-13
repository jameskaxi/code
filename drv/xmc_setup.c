/*
 * xmc_setup.c
 *
 *  Created on: 2017-11-15
 *      Author: wyk
 */
#include <c6x.h>
#define XMC_BASE_ADDR (0x08000000)
#define XMPAX3_L     (*(int*)(XMC_BASE_ADDR + 0x00000018))
#define XMPAX3_H     (*(int*)(XMC_BASE_ADDR + 0x0000001C))

#define RADDR_BASE(a) 0x820000000 + (a)*64*0x100000
#define ADDR_BASE 	  0xa0000000

void SetupXMC(void)
{
	XMPAX3_L = (RADDR_BASE(DNUM)>>4) | 0xFF;
	XMPAX3_H = ADDR_BASE | 0x19;
}


