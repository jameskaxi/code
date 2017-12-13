/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#ifndef _CSLR_PCIESS_CFGTYPE1_H_
#define _CSLR_PCIESS_CFGTYPE1_H_

/* CSL Modification:
 *  The file has been modified from the AUTOGEN file for the following
 *  reasons:-
 *      a) Modified the header file includes to be RTSC compliant
 */

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>

/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint8 RSVD0[12];
    volatile Uint32 BIST_HEADER;
    volatile Uint32 BAR[2];
    volatile Uint32 BUSNUM;
    volatile Uint32 SECSTAT;
    volatile Uint32 MEMSPACE;
    volatile Uint32 PREFETCH_MEM;
    volatile Uint32 PREFETCH_BASE;
    volatile Uint32 PREFETCH_LIMIT;
    volatile Uint32 IOSPACE;
    volatile Uint32 CAP_PTR;
    volatile Uint32 EXPNSN_ROM;
    volatile Uint32 BRIDGE_INT;
} CSL_Pciess_cfgtype1Regs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* BIST_HEADER */

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_BISTCAPABLE_MASK (0x80000000u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_BISTCAPABLE_SHIFT (0x0000001Fu)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_BISTCAPABLE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_STARTBIST_MASK (0x40000000u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_STARTBIST_SHIFT (0x0000001Eu)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_STARTBIST_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_COMPLETIONCODE_MASK (0x0F000000u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_COMPLETIONCODE_SHIFT (0x00000018u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_COMPLETIONCODE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_MULTIFUNCTION_DEVICE_MASK (0x00800000u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_MULTIFUNCTION_DEVICE_SHIFT (0x00000017u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_MULTIFUNCTION_DEVICE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_HEADERTYPE_MASK (0x007F0000u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_HEADERTYPE_SHIFT (0x00000010u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_HEADERTYPE_RESETVAL (0x00000001u)

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_LATENCYTIMER_MASK (0x0000FF00u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_LATENCYTIMER_SHIFT (0x00000008u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_LATENCYTIMER_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_CACHELINE_SIZE_MASK (0x000000FFu)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_CACHELINE_SIZE_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_CACHELINE_SIZE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BIST_HEADER_RESETVAL (0x00010000u)

/* BAR */

#define CSL_PCIESS_CFGTYPE1_BAR_BASEADDRESS_MASK (0xFFFFFFF0u)
#define CSL_PCIESS_CFGTYPE1_BAR_BASEADDRESS_SHIFT (0x00000004u)
#define CSL_PCIESS_CFGTYPE1_BAR_BASEADDRESS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BAR_PREFETCHABLE_MASK (0x00000008u)
#define CSL_PCIESS_CFGTYPE1_BAR_PREFETCHABLE_SHIFT (0x00000003u)
#define CSL_PCIESS_CFGTYPE1_BAR_PREFETCHABLE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BAR_TYPE_MASK (0x00000006u)
#define CSL_PCIESS_CFGTYPE1_BAR_TYPE_SHIFT (0x00000001u)
#define CSL_PCIESS_CFGTYPE1_BAR_TYPE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BAR_MEMORYSPACE_MASK (0x00000001u)
#define CSL_PCIESS_CFGTYPE1_BAR_MEMORYSPACE_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_BAR_MEMORYSPACE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BAR_RESETVAL (0x00000000u)

/* BUSNUM */

#define CSL_PCIESS_CFGTYPE1_BUSNUM_SECONDARYLATENCY_TIMER_MASK (0xFF000000u)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_SECONDARYLATENCY_TIMER_SHIFT (0x00000018u)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_SECONDARYLATENCY_TIMER_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BUSNUM_SUBORDINATEBUS_NUMBER_MASK (0x00FF0000u)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_SUBORDINATEBUS_NUMBER_SHIFT (0x00000010u)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_SUBORDINATEBUS_NUMBER_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BUSNUM_SECONDARYBUS_NUMBER_MASK (0x0000FF00u)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_SECONDARYBUS_NUMBER_SHIFT (0x00000008u)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_SECONDARYBUS_NUMBER_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BUSNUM_PRIMARYBUS_NUMBER_MASK (0x000000FFu)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_PRIMARYBUS_NUMBER_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_BUSNUM_PRIMARYBUS_NUMBER_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BUSNUM_RESETVAL (0x00000000u)

/* SECSTAT */

#define CSL_PCIESS_CFGTYPE1_SECSTAT_DTCT_PERROR_MASK (0x80000000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_DTCT_PERROR_SHIFT (0x0000001Fu)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_DTCT_PERROR_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_SYS_ERROR_MASK (0x40000000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_SYS_ERROR_SHIFT (0x0000001Eu)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_SYS_ERROR_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_MST_ABORT_MASK (0x20000000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_MST_ABORT_SHIFT (0x0000001Du)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_MST_ABORT_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_TGT_ABORT_MASK (0x10000000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_TGT_ABORT_SHIFT (0x0000001Cu)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_RX_TGT_ABORT_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_TX_TGT_ABORT_MASK (0x08000000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_TX_TGT_ABORT_SHIFT (0x0000001Bu)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_TX_TGT_ABORT_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_MST_DPERR_MASK (0x01000000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_MST_DPERR_SHIFT (0x00000018u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_MST_DPERR_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOLIMIT_MASK (0x0000F000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOLIMIT_SHIFT (0x0000000Cu)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOLIMIT_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOBASE_MASK (0x000000F0u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOBASE_SHIFT (0x00000004u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOBASE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOADDRESSING_MASK (0x00000001u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOADDRESSING_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_SECSTAT_IOADDRESSING_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_SECSTAT_RESETVAL (0x00000000u)

/* MEMSPACE */

#define CSL_PCIESS_CFGTYPE1_MEMSPACE_MEMORYLIMIT_MASK (0xFFF00000u)
#define CSL_PCIESS_CFGTYPE1_MEMSPACE_MEMORYLIMIT_SHIFT (0x00000014u)
#define CSL_PCIESS_CFGTYPE1_MEMSPACE_MEMORYLIMIT_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_MEMSPACE_MEMORYBASE_MASK (0x0000FFF0u)
#define CSL_PCIESS_CFGTYPE1_MEMSPACE_MEMORYBASE_SHIFT (0x00000004u)
#define CSL_PCIESS_CFGTYPE1_MEMSPACE_MEMORYBASE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_MEMSPACE_RESETVAL (0x00000000u)

/* PREFETCH_MEM */

#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_ENDADDRESS_MASK (0xFFF00000u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_ENDADDRESS_SHIFT (0x00000014u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_ENDADDRESS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_STARTADDRESS_MASK (0x0000FFF0u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_STARTADDRESS_SHIFT (0x00000004u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_STARTADDRESS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_MEMORYADDRESSING_MASK (0x00000001u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_MEMORYADDRESSING_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_MEMORYADDRESSING_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_PREFETCH_MEM_RESETVAL (0x00000000u)

/* PREFETCH_BASE */

#define CSL_PCIESS_CFGTYPE1_PREFETCH_BASE_BASEADDRESS_MASK (0xFFFFFFFFu)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_BASE_BASEADDRESS_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_BASE_BASEADDRESS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_PREFETCH_BASE_RESETVAL (0x00000000u)

/* PREFETCH_LIMIT */

#define CSL_PCIESS_CFGTYPE1_PREFETCH_LIMIT_LIMITADDRESS_MASK (0xFFFFFFFFu)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_LIMIT_LIMITADDRESS_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_PREFETCH_LIMIT_LIMITADDRESS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_PREFETCH_LIMIT_RESETVAL (0x00000000u)

/* IOSPACE */

#define CSL_PCIESS_CFGTYPE1_IOSPACE_IOBASE_MASK (0xFFFF0000u)
#define CSL_PCIESS_CFGTYPE1_IOSPACE_IOBASE_SHIFT (0x00000010u)
#define CSL_PCIESS_CFGTYPE1_IOSPACE_IOBASE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_IOSPACE_IOLIMIT_MASK (0x0000FFFFu)
#define CSL_PCIESS_CFGTYPE1_IOSPACE_IOLIMIT_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_IOSPACE_IOLIMIT_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_IOSPACE_RESETVAL (0x00000000u)

/* CAP_PTR */

#define CSL_PCIESS_CFGTYPE1_CAP_PTR_CAP_PTR_MASK (0x000000FFu)
#define CSL_PCIESS_CFGTYPE1_CAP_PTR_CAP_PTR_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_CAP_PTR_CAP_PTR_RESETVAL (0x00000040u)

#define CSL_PCIESS_CFGTYPE1_CAP_PTR_RESETVAL (0x00000040u)

/* EXPNSN_ROM */

#define CSL_PCIESS_CFGTYPE1_EXPNSN_ROM_EXPANSIONROM_BASE_ADDRESS_MASK (0xFFFFF800u)
#define CSL_PCIESS_CFGTYPE1_EXPNSN_ROM_EXPANSIONROM_BASE_ADDRESS_SHIFT (0x0000000Bu)
#define CSL_PCIESS_CFGTYPE1_EXPNSN_ROM_EXPANSIONROM_BASE_ADDRESS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_EXPNSN_ROM_EXPANSIONROM_ENABLE_MASK (0x00000001u)
#define CSL_PCIESS_CFGTYPE1_EXPNSN_ROM_EXPANSIONROM_ENABLE_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_EXPNSN_ROM_EXPANSIONROM_ENABLE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_EXPNSN_ROM_RESETVAL (0x00000000u)

/* BRIDGE_INT */

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SERREN_STATUS_MASK (0x08000000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SERREN_STATUS_SHIFT (0x0000001Bu)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SERREN_STATUS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_TIMER_STATUS_MASK (0x04000000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_TIMER_STATUS_SHIFT (0x0000001Au)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_TIMER_STATUS_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SEC_TIMER_MASK (0x02000000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SEC_TIMER_SHIFT (0x00000019u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SEC_TIMER_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_PRI_TIMER_MASK (0x01000000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_PRI_TIMER_SHIFT (0x00000018u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_PRI_TIMER_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_B2B_EN_MASK (0x00800000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_B2B_EN_SHIFT (0x00000017u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_B2B_EN_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SEC_BUS_RST_MASK (0x00400000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SEC_BUS_RST_SHIFT (0x00000016u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SEC_BUS_RST_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_MST_ABORT_MODE_MASK (0x00200000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_MST_ABORT_MODE_SHIFT (0x00000015u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_MST_ABORT_MODE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_VGA_DECODE_MASK (0x00100000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_VGA_DECODE_SHIFT (0x00000014u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_VGA_DECODE_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_VGA_EN_MASK (0x00080000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_VGA_EN_SHIFT (0x00000013u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_VGA_EN_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_ISA_EN_MASK (0x00040000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_ISA_EN_SHIFT (0x00000012u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_ISA_EN_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SERR_EN_MASK (0x00020000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SERR_EN_SHIFT (0x00000011u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_SERR_EN_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_PERR_RESP_EN_MASK (0x00010000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_PERR_RESP_EN_SHIFT (0x00000010u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_PERR_RESP_EN_RESETVAL (0x00000000u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_INT_PIN_MASK (0x0000FF00u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_INT_PIN_SHIFT (0x00000008u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_INT_PIN_RESETVAL (0x00000001u)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_INT_LINE_MASK (0x000000FFu)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_INT_LINE_SHIFT (0x00000000u)
#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_INT_LINE_RESETVAL (0x000000FFu)

#define CSL_PCIESS_CFGTYPE1_BRIDGE_INT_RESETVAL (0x000001FFu)

#endif
