/**
 *   @file  rm_public_lld.h
 *
 *   @brief   
 *      This is the RM LLD include file for other LLDs.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2012, Texas Instruments, Inc.
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

#ifndef RM_PUBLIC_LLD_H_
#define RM_PUBLIC_LLD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QMSS type include */
#include <ti/drv/qmss/qmss_qm.h>

/* CPPI type include */
#include <ti/csl/csl_cppi.h>

/**
@addtogroup RM_LLD_SYMBOL
@{
*/

/** RM Permission Return And Error Codes */
/** RM LLD Permission Approved Base */
#define RM_APPROVED_BASE 0
/** Resource initialization permission approved */
#define RM_INIT_PERMISSION_APPROVED  (RM_APPROVED_BASE+1)
/** Resource usage permission approved */
#define RM_USE_PERMISSION_APPROVED  (RM_APPROVED_BASE+2)

/** RM LLD Permission Denied Base */
#define RM_DENIED_BASE (-64)
/** Resource initialization permission denied */
#define RM_INIT_PERMISSION_DENIED  (RM_DENIED_BASE-1)
/** Resource usage permission denied */
#define RM_USE_PERMISSION_DENIED  (RM_DENIED_BASE-2)

/**
@}
*/

/**
@addtogroup RM_LLD_ENUM
@{
*/

/** 
 * @brief LLD resource type
 */
typedef enum
{
    /** QMSS Firmware Type */
    Rm_resource_QMSS_FIRMWARE_PDSP = 0,
    /** QMSS Queue Type */
    Rm_resource_QMSS_QUEUE,
    /** QMSS Memory Region Type */
    Rm_resource_QMSS_MEMORY_REGION,
    /** QMSS Linking RAM Control Type */
    Rm_resource_QMSS_LINKING_RAM_CONTROL,
    /** QMSS Linking RAM Type */
    Rm_resource_QMSS_LINKING_RAM,
    /** QMSS Accumulator Channel Type */
    Rm_resource_QMSS_ACCUM_CH,
    /** QMSS QOS PDSP Timer */
    Rm_resource_QMSS_QOS_TIMER,
    /** QMSS QOS Cluster Type */
    Rm_resource_QMSS_QOS_CLUSTER,
    /** QMSS QOS Queue Type */
    Rm_resource_QMSS_QOS_QUEUE,
    /** CPPI Transmit Channel Type */
    Rm_resource_CPPI_TX_CH,
    /** CPPI Receive Channel Type */
    Rm_resource_CPPI_RX_CH,
    /** CPPI Receive Flow Type */
    Rm_resource_CPPI_RX_FLOW,
    /** PA Firmware Type */
    Rm_resource_PA_FIRMWARE,
    /** PA LUT Type */
    Rm_resource_PA_LUT
}Rm_ResourceType;

/**
@}
*/

/** @addtogroup RM_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief Structure specifying the Linking RAM start and end indices
 */
typedef struct
{
    uint32_t linkRamStartIndex;
    uint32_t linkRamEndIndex;
} Rm_ResourceLinkRamInfo;

/** 
 * @brief Structure specifying the CPPI rx channel, tx channel, and flow ID information
 */
typedef struct
{
    Cppi_CpDma dmaNum;
    int32_t cppiChNumOrFlowId;
} Rm_ResourceCpDmaInfo;

/** 
 * @brief Structure specifying the LLD resource to have permissions checked by RM
 */
typedef struct
{
    /** LLD resource type to be checked */
    Rm_ResourceType resourceType;
    
    /** Resource parameters union. */
    union
    {
        /** QMSS Resource Data */
        uint16_t pdspNum;
        int queNum;
        Qmss_MemRegion memRegion;
        Rm_ResourceLinkRamInfo linkRamData;
        uint8_t accumCh;
        uint32_t qosCluster;
        uint32_t qosQueue;        

        /** CPPI Resource Data */
        Rm_ResourceCpDmaInfo cpDmaData;

        /** PA Resource Data */
        int32_t lutEntry;
    } res_info;
} Rm_ResourceInfo;

/** 
 * @brief RM return result
 */
typedef int32_t   Rm_Result;

/** 
 * @brief Structure of LLD function callouts for permissions checks
 */
typedef struct
{
    /** Init permissions check function */
    Rm_Result  (*rmInitPermissionsCheck)(Rm_ResourceInfo *resourceData);
    /** Use permissions check function */
    Rm_Result  (*rmUsePermissionsCheck)(Rm_ResourceInfo *resourceData); 
}Rm_LldPermCallouts;

/** 
@} 
*/

#ifdef __cplusplus
}
#endif

#endif /* RM_PUBLIC_LLD_H_ */

