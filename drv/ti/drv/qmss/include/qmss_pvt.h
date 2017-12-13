/*
 *  file  qmss_pvt.h
 *
 *  Private data structures of Queue Manager Low Level Driver.
 *
 *  ============================================================================
 *      (C) Copyright 2009-2012, Texas Instruments, Inc.
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

#ifndef QMSS_PVT_H_
#define QMSS_PVT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <ti/drv/qmss/qmss_qm.h>

/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_qm_config.h>
#include <ti/csl/cslr_qm_descriptor_region_config.h>
#include <ti/csl/cslr_qm_queue_management.h>
#include <ti/csl/cslr_qm_queue_status_config.h>
#include <ti/csl/cslr_qm_intd.h>
#include <ti/csl/cslr_pdsp.h>

/* Maximum cache line size for alignment */
#define QMSS_MAX_CACHE_ALIGN 128

/* QMSS Global object definition. */
typedef struct 
{
    /** Store the configuration structure passed during Qmss_init */
    Qmss_GlobalConfigParams             qmssGblCfgParams;
    /** Store the intialization structure passed during Qmss_init */
    Qmss_InitCfg                        initCfg;
    /** Current Memory regions configuration */
    Qmss_MemRegInfo                     memRegInfo[QMSS_MAX_MEM_REGIONS];
    /** General purpose source queue handles */
    int32_t                             descQueue[QMSS_MAX_MEM_REGIONS];
    /** Current descriptor count */
    uint32_t                            currDescCnt;
} Qmss_GlobalObj_Unpadded;

typedef struct
{
    /** Data structure without padding, so sizeof() can compute padding */
    Qmss_GlobalObj_Unpadded obj;
    /** Pad out to end of QMSS_MAX_CACHE_ALIGN bytes to prevent something else
     * from being placed on same cache line as Cppi_Obj.  Note that 
     * pad[0] is illegal, so must add full QMSS_MAX_CACHE_ALIGN if structure
     * is already padded by chance. */
    uint8_t                 pad[QMSS_MAX_CACHE_ALIGN - 
                            (sizeof(Qmss_GlobalObj_Unpadded) % QMSS_MAX_CACHE_ALIGN)];
} Qmss_GlobalObj;

extern int32_t Qmss_getMemRegQueueHandle(uint32_t memRegion);
extern uint32_t Qmss_getMemRegDescSize (uint32_t memRegion);

#ifdef __cplusplus
}
#endif

#endif /* QMSS_PVT_H_ */

