/**
 *   @file  resource_table_defs.h
 *
 *   @brief   
 *      This file defines the identifiers used to populate the resource table used
 *      by the RM LLD to divy resources.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2012 Texas Instruments, Inc.
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
 *  \par
*/
#ifndef __RESOURCE_TABLE_DEFS_H__
#define __RESOURCE_TABLE_DEFS_H__

/* c99 include */
#include <stdint.h>

/**
@addtogroup RM_LLD_RESOURCE_TABLE
@{
*/

/** RM LLD Resource Table Resource Identifiers */
/** This value should be first entry in the resource table.  Used to verify RM can read the resource table. */
#define RM_RESOURCE_MAGIC_NUMBER (0x76543210)
/** This value should be last entry in the resource table.  Used by RM to find last entry in resource table. */   
#define RM_RESOURCE_FINAL_ENTRY (0xFFFFFFFF)                                                                                     

/** Start of QMSS resource identifiers */
#define RM_RESOURCE_QMSS_BASE 0
/** QMSS Firmware PDSP write permissions */
#define RM_RESOURCE_QMSS_FIRMWARE_PDSP (RM_RESOURCE_QMSS_BASE+1)
/** QMSS queues.  This identifier will be expanded to identify all queue types in the near future */
#define RM_RESOURCE_QMSS_QUEUE (RM_RESOURCE_QMSS_BASE+2) 
/** QMSS Memory regions */
#define RM_RESOURCE_QMSS_MEMORY_REGION (RM_RESOURCE_QMSS_BASE+3) 
/** QMSS Linking RAM Control */
#define RM_RESOURCE_QMSS_LINKING_RAM_CONTROL (RM_RESOURCE_QMSS_BASE+4) 
/** QMSS Linking RAM indices */
#define RM_RESOURCE_QMSS_LINKING_RAM (RM_RESOURCE_QMSS_BASE+5) 
/** QMSS accumulator channels */
#define RM_RESOURCE_QMSS_ACCUMULATOR_CH (RM_RESOURCE_QMSS_BASE+6) 
/** QMSS QOS PDSP timer */
#define RM_RESOURCE_QMSS_QOS_PDSP_TIMER (RM_RESOURCE_QMSS_BASE+7) 
/** QMSS QOS clusters */
#define RM_RESOURCE_QMSS_QOS_CLUSTER (RM_RESOURCE_QMSS_BASE+8) 
/** QMSS QOS queues */
#define RM_RESOURCE_QMSS_QOS_QUEUE (RM_RESOURCE_QMSS_BASE+9) 

/** Start of CPPI resource identifiers */
#define RM_RESOURCE_CPPI_BASE 64 
/** CPPI SRIO transmit channel */
#define RM_RESOURCE_CPPI_SRIO_TX_CH (RM_RESOURCE_CPPI_BASE+1) 
/** CPPI SRIO receive channel */
#define RM_RESOURCE_CPPI_SRIO_RX_CH (RM_RESOURCE_CPPI_BASE+2) 
/** CPPI SRIO flow */
#define RM_RESOURCE_CPPI_SRIO_FLOW (RM_RESOURCE_CPPI_BASE+3) 
/** CPPI AIF transmit channel */
#define RM_RESOURCE_CPPI_AIF_TX_CH (RM_RESOURCE_CPPI_BASE+4) 
/** CPPI AIF receive channel */
#define RM_RESOURCE_CPPI_AIF_RX_CH (RM_RESOURCE_CPPI_BASE+5) 
/** CPPI AIF flow */
#define RM_RESOURCE_CPPI_AIF_FLOW (RM_RESOURCE_CPPI_BASE+6) 
/** CPPI FFTC_A transmit channel */
#define RM_RESOURCE_CPPI_FFTC_A_TX_CH (RM_RESOURCE_CPPI_BASE+7) 
/** CPPI FFTC_A receive channel */
#define RM_RESOURCE_CPPI_FFTC_A_RX_CH (RM_RESOURCE_CPPI_BASE+8) 
/** CPPI FFTC_A flow */
#define RM_RESOURCE_CPPI_FFTC_A_FLOW  (RM_RESOURCE_CPPI_BASE+9) 
/** CPPI FFTC_B transmit channel */
#define RM_RESOURCE_CPPI_FFTC_B_TX_CH (RM_RESOURCE_CPPI_BASE+10) 
/** CPPI FFTC_B receive channel */
#define RM_RESOURCE_CPPI_FFTC_B_RX_CH (RM_RESOURCE_CPPI_BASE+11) 
/** CPPI FFTC_B flow */
#define RM_RESOURCE_CPPI_FFTC_B_FLOW (RM_RESOURCE_CPPI_BASE+12) 
/** CPPI PASS transmit channel */
#define RM_RESOURCE_CPPI_PASS_TX_CH (RM_RESOURCE_CPPI_BASE+13) 
/** CPPI PASS receive channel */
#define RM_RESOURCE_CPPI_PASS_RX_CH (RM_RESOURCE_CPPI_BASE+14) 
/** CPPI PASS flow */
#define RM_RESOURCE_CPPI_PASS_FLOW (RM_RESOURCE_CPPI_BASE+15) 
/** CPPI QMSS transmit channel */
#define RM_RESOURCE_CPPI_QMSS_TX_CH (RM_RESOURCE_CPPI_BASE+16) 
/** CPPI QMSS receive channel */
#define RM_RESOURCE_CPPI_QMSS_RX_CH (RM_RESOURCE_CPPI_BASE+17) 
/** CPPI QMSS flow */
#define RM_RESOURCE_CPPI_QMSS_FLOW (RM_RESOURCE_CPPI_BASE+18) 
/** CPPI FFTC_C transmit channel */
#define RM_RESOURCE_CPPI_FFTC_C_TX_CH (RM_RESOURCE_CPPI_BASE+19) 
/** CPPI FFTC_C receive channel */
#define RM_RESOURCE_CPPI_FFTC_C_RX_CH (RM_RESOURCE_CPPI_BASE+20) 
/** CPPI FFTC_C flow */
#define RM_RESOURCE_CPPI_FFTC_C_FLOW (RM_RESOURCE_CPPI_BASE+21) 
/** CPPI BCP transmit channel */
#define RM_RESOURCE_CPPI_BCP_TX_CH (RM_RESOURCE_CPPI_BASE+22) 
/** CPPI BCP receive channel */
#define RM_RESOURCE_CPPI_BCP_RX_CH (RM_RESOURCE_CPPI_BASE+23) 
/** CPPI BCP flow */
#define RM_RESOURCE_CPPI_BCP_FLOW (RM_RESOURCE_CPPI_BASE+24) 

/** Start of CPPI resource identifiers */
#define RM_RESOURCE_PA_BASE 128 
/** PA Firmware write permissions */
#define RM_RESOURCE_PA_FIRMWARE (RM_RESOURCE_PA_BASE+1) 
/** PA look-up table entry */
#define RM_RESOURCE_PA_LUT_ENTRY (RM_RESOURCE_PA_BASE+2) 

/** RM LLD Resource Table Permission Codes */
/** Init or use permission allowed */
#define RM_RESOURCE_PERM_DENIED 0x0
/** Init or use permission denied */
#define RM_RESOURCE_PERM_ALLOWED 0x1

/** Resource entry flags bitfield DSP shift macro */
#define RM_RESOURCE_FLAG_DSP_SHIFT(dspNum, perms)  \
                                          (((uint32_t) perms) << dspNum)

/** Full Permissions - All DSPs can use and initialize resource */
#define RM_RESOURCE_ALL_DSPS_FULL_PERMS  \
                       ((RM_RESOURCE_FLAG_DSP_SHIFT(0, RM_RESOURCE_PERM_ALLOWED)) | \
                        (RM_RESOURCE_FLAG_DSP_SHIFT(1, RM_RESOURCE_PERM_ALLOWED)) | \
                        (RM_RESOURCE_FLAG_DSP_SHIFT(2, RM_RESOURCE_PERM_ALLOWED)) | \
                        (RM_RESOURCE_FLAG_DSP_SHIFT(3, RM_RESOURCE_PERM_ALLOWED)))

/** 
 * @brief Resource Table resource definition structure
 */
typedef struct 
{
    /** Resouce identifier. */
    uint32_t  resourceId;
    /** Start range for identified resource */
    uint32_t  resourceStart;
    /** End range for identified resource */
    uint32_t  resourceEnd;
    /** Resource initialization permission flags
      * Bits 0 : DSP 0 Permission Bit
      * Bits 1 : DSP 1 Permission Bit 
      * Bits 2 : DSP 2 Permission Bit
      * Bits 3 : DSP 3 Permission Bit 
      * Bits 31-8 : UNUSED
      */
    uint32_t  resourceInitFlags;
    /** Resource usage permission flags
      * Bits 0 : DSP 0 Permission Bit
      * Bits 1 : DSP 1 Permission Bit 
      * Bits 2 : DSP 2 Permission Bit
      * Bits 3 : DSP 3 Permission Bit 
      * Bits 31-8 : UNUSED
      */    
    uint32_t  resourceUseFlags;
} Rm_Resource;

/**
@}
*/

#endif	/* __RESOURCE_TABLE_DEFS_H__ */



