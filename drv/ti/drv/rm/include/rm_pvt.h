/*
 *  file  rm_pvt.h
 *
 *  Private data structures of Resource Manager Low Level Driver.
 *
 *  ============================================================================
 *      (C) Copyright 2012, Texas Instruments, Inc.
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

#ifndef RM_PVT_H_
#define RM_PVT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Device Include */
#include <c6x.h>

/* RM includes */
#include <ti/drv/rm/rm_public_lld.h>

/* CSL includes */
#include <ti/csl/csl_qm_queue.h>

/* QMSS Resource Information */
#define RM_QMSS_FIRMWARE_PDSPS 2
#define RM_QMSS_QUEUES 8192
#define RM_QMSS_MEM_REGIONS 20
#define RM_QMSS_LINKING_RAM_RANGES 40 /* Twice as many memory regions */
#define RM_QMSS_ACCUM_CH 48
#define RM_QMSS_QOS_CLUSTER 8
#define RM_QMSS_QOS_QUEUES 64

#define RM_QMSS_LINKING_RAM_RANGE_INIT  (0xFFFFFFFF)

/* CPPI Resource Information */

/* Set CPPI DMA increments based on which DMAs are present for the device */

/* Set AIF increment if present */
#ifdef QMSS_MAX_AIF_QUEUE
#define RM_CPPI_AIF_INC 1
#else
#define RM_CPPI_AIF_INC 0
#endif
/* Set FFTC A increment if present */
#ifdef QMSS_MAX_FFTC_A_QUEUE
#define RM_CPPI_FFTC_A_INC 1
#else
#define RM_CPPI_FFTC_A_INC 0
#endif
/* Set FFTC B increment if present */
#ifdef QMSS_MAX_FFTC_B_QUEUE
#define RM_CPPI_FFTC_B_INC 1
#else
#define RM_CPPI_FFTC_B_INC 0
#endif
/* Set PASS increment if present */
#ifdef QMSS_MAX_PASS_QUEUE
#define RM_CPPI_PASS_INC 1
#else
#define RM_CPPI_PASS_INC 0
#endif
/* Set FFTC C increment if present */
#ifdef QMSS_MAX_FFTC_C_QUEUE
#define RM_CPPI_FFTC_C_INC 1
#else
#define RM_CPPI_FFTC_C_INC 0
#endif
/* Set BCP increment if present */
#ifdef QMSS_MAX_BCP_QUEUE
#define RM_CPPI_BCP_INC 1
#else
#define RM_CPPI_BCP_INC 0
#endif

/* Base number of DMAs for all devices. */
#define RM_CPPI_BASE_DMAS 2
/* Set max DMAs, adding additional DMAs if present */
#define RM_CPPI_MAX_DMAS (RM_CPPI_BASE_DMAS + RM_CPPI_AIF_INC + RM_CPPI_FFTC_A_INC + \
                                                RM_CPPI_FFTC_B_INC + RM_CPPI_PASS_INC + RM_CPPI_FFTC_C_INC + \
                                                RM_CPPI_BCP_INC)

/* Set DMA indices based on which DMAs are present in device */
#define RM_CPPI_SRIO_DMA_ID 0
#define RM_CPPI_AIF_DMA_ID (RM_CPPI_SRIO_DMA_ID + RM_CPPI_AIF_INC)
#define RM_CPPI_FFTC_A_DMA_ID (RM_CPPI_AIF_DMA_ID + RM_CPPI_FFTC_A_INC)
#define RM_CPPI_FFTC_B_DMA_ID (RM_CPPI_FFTC_A_DMA_ID + RM_CPPI_FFTC_B_INC)
#define RM_CPPI_PASS_DMA_ID (RM_CPPI_FFTC_B_DMA_ID + RM_CPPI_PASS_INC)
#define RM_CPPI_QMSS_DMA_ID (RM_CPPI_PASS_DMA_ID + 1)
#define RM_CPPI_FFTC_C_DMA_ID (RM_CPPI_QMSS_DMA_ID + RM_CPPI_FFTC_C_INC)
#define RM_CPPI_BCP_DMA_ID (RM_CPPI_FFTC_C_DMA_ID + RM_CPPI_BCP_INC)

#define RM_CPPI_SRIO_TX_CH 16
#define RM_CPPI_AIF_TX_CH 129
#define RM_CPPI_FFTC_A_TX_CH 4
#define RM_CPPI_FFTC_B_TX_CH 4
#define RM_CPPI_PASS_TX_CH 9
#define RM_CPPI_QMSS_TX_CH 32
#define RM_CPPI_FFTC_C_TX_CH 4
#define RM_CPPI_BCP_TX_CH 8

#define RM_CPPI_SRIO_RX_CH 16
#define RM_CPPI_AIF_RX_CH 129
#define RM_CPPI_FFTC_A_RX_CH 4
#define RM_CPPI_FFTC_B_RX_CH 4
#define RM_CPPI_PASS_RX_CH 24
#define RM_CPPI_QMSS_RX_CH 32
#define RM_CPPI_FFTC_C_RX_CH 4
#define RM_CPPI_BCP_RX_CH 8

#define RM_CPPI_SRIO_FLOW 20
#define RM_CPPI_AIF_FLOW 129
#define RM_CPPI_FFTC_A_FLOW 8
#define RM_CPPI_FFTC_B_FLOW 8
#define RM_CPPI_PASS_FLOW 32
#define RM_CPPI_QMSS_FLOW 64
#define RM_CPPI_FFTC_C_FLOW 8
#define RM_CPPI_BCP_FLOW 64

/* PA Resource Information */
#define RM_PA_LUT 5

/* Permissions Access Defines and Macros */
#define RM_RESOURCE_PERM_INIT_SHIFT 0
#define RM_RESOURCE_PERM_USE_SHIFT  1

#define RM_GET_RESOURCE_FLAG(flag) ((flag) >> ((DNUM) << 1))

#define RM_GET_RESOURCE_INIT_FLAG(flag) ((RM_GET_RESOURCE_FLAG (flag) >> RM_RESOURCE_PERM_INIT_SHIFT) & 1)
#define RM_GET_RESOURCE_USE_FLAG(flag)  ((RM_GET_RESOURCE_FLAG (flag) >> RM_RESOURCE_PERM_USE_SHIFT)  & 1)

#define RM_GET_PERMISSIONS(perms) (perms >> DNUM)

#define RM_RANGE_CHECK(start, end, max, ret_val) \
do { \
    if ((start > end) || (end > max)) \
    { \
        return ret_val; \
    } \
} while(0)

/* RM standard permission structure definition */
typedef struct
{
    /** Initialization permissions
      * Bit 0 - Core 0 init permission
      * Bit 1 - Core 1 init permission
      * Bit 2 - Core 2 init permission
      * Bit 3 - Core 3 init permission
      */    
    uint32_t initPerms;  
    /** Usage permissions
      * Bit 0 - Core 0 use permission
      * Bit 1 - Core 1 use permission
      * Bit 2 - Core 2 use permission
      * Bit 3 - Core 3 use permission
      */    
    uint32_t usePerms;
} Rm_Perms;

/* RM permissions structure for Linking RAM regions */
typedef struct
{
    /** Linking RAM start index for these permissions */
    uint32_t startIndex;
    /** Linking RAM end index for these permissions */
    uint32_t endIndex;
    /** Permissions for the range */
    Rm_Perms rangePerms;
} Rm_qmssLinkingRamPerms;

/* RM permissions structure for CPPI DMA channels and flows */
typedef struct
{
    /** Array of pointers to each DMAs channel or flow permissions
       * From CPPI LLD - DMA 0 = SRIO
       *                          DMA 1 = AIF
       *                          DMA 2 = FFTC A
       *                          DMA 3 = FFTC B
       *                          DMA 4 = PASS
       *                          DMA 5 = QMSS
       *                          DMA 6 = FFTC C
       *                          DMA 7 = BCP 
       * 
       * Note: Some DMAs may not be supported based on the device */
    Rm_Perms *dmaPermPtrs[RM_CPPI_MAX_DMAS];
} Rm_CppiChAndFlowPerms;

/* RM Cache Line Alignment Defines and Macros */

#define RM_MAX_CACHE_ALIGN 128  /* Maximum alignment for cache line size */

/* This macro generates compilier error if postulate is false, so 
 * allows 0 overhead compile time size check.  This "works" when
 * the expression contains sizeof() which otherwise doesn't work
 * with preprocessor */
#define RM_COMPILE_TIME_SIZE_CHECK(postulate)                         \
   do {                                                                 \
       typedef struct {                                                 \
         uint8_t NegativeSizeIfPostulateFalse[((int)(postulate))*2 - 1];\
       } PostulateCheck_t;                                              \
   }                                                                    \
   while (0)

/* Macro to pad out internal permission structures to multiple of RM_MAX_CACHE_ALIGN bytes
  * The macro prevent something else from being placed on same cache line as the permission.  
  * arrays.  Note that pad[0] is illegal, so must add full RM_MAX_CACHE_ALIGN if structure
  * is already padded by chance. */
#define RM_ALIGN_PERMISSIONS_ARRAY(numElements, permStructType) ( \
     (((sizeof(permStructType) * (numElements)) % RM_MAX_CACHE_ALIGN) == 0) ? (numElements) : \
     ((numElements) + \
       (RM_MAX_CACHE_ALIGN - \
         ((sizeof(permStructType) * (numElements)) % RM_MAX_CACHE_ALIGN))/sizeof(permStructType)))

/* RM Global Sync Object (unpadded) */
typedef struct
{
    /** Rm_init/Rm_start synchronization object. */
    uint8_t globalSyncObj;
} Rm_Sync_Obj_Unpadded;

/* RM Global Sync Object (padded) */
typedef struct
{
    /** Data structure without padding, so sizeof() can compute padding */
    Rm_Sync_Obj_Unpadded obj;
    /** Pad out to end of RM_MAX_CACHE_ALIGN bytes to prevent something else
     * from being placed on same cache line as Rm_Synci_Obj.  Note that 
     * pad[0] is illegal, so must add full RM_MAX_CACHE_ALIGN if structure
     * is already padded by chance. */
    uint8_t  pad[RM_MAX_CACHE_ALIGN - 
                       (sizeof(Rm_Sync_Obj_Unpadded) % RM_MAX_CACHE_ALIGN)];
} Rm_Sync_Obj;


/* RM Global permissions object definition. (unpadded) */
typedef struct 
{
    /** Store the QMSS PDSP firmware permissions */
    Rm_Perms *qmssPdspFirmwarePerms;  
    /** Store a pointer to the QMSS queue permissions array */
    Rm_Perms *qmssQueuePerms;
    /** Store a pointer to the QMSS memory region permissions array */
    Rm_Perms *qmssMemRegionPerms;
    /** Store the QMSS Linking RAM Control permissions */
    Rm_Perms qmssLinkRamControlPerms;
    /** Store a pointer to the QMSS linking RAM permissions list */
    Rm_qmssLinkingRamPerms *qmssLinkRamPerms;
    /** Store a pointer to the QMSS accumulator channel permissions array */
    Rm_Perms *qmssAccumChPerms;
    /** Store the QMSS QOS PDSP timer permissions */
    Rm_Perms qmssQosPdspTimerPerms;      
    /** Store a pointer to the QMSS QOS cluster permissions array */
    Rm_Perms *qmssQosClusterPerms;    
    /** Store a pointer to the QMSS QOS queue permissions array */
    Rm_Perms *qmssQosQueuePerms;       
    /** Store the structure of pointers to the CPPI transmit channel permissions array */
    Rm_CppiChAndFlowPerms cppiTxChPerms;
    /** Store the structure of pointers to the CPPI receive channel permissions array */
    Rm_CppiChAndFlowPerms cppiRxChPerms;
    /** Store the structure of pointers to the CPPI flow permissions array */
    Rm_CppiChAndFlowPerms cppiFlowPerms;
    /** Store the PA firmware permissions */
    Rm_Perms paFirmwarePerms;  
    /** Store a pointer to the PA lookup table permissions array */
    Rm_Perms *paLutPerms;    
}Rm_GlobalPermissionsObj_Unpadded;

/* RM Global Permissions Object (padded) */
typedef struct
{
    /** Data structure without padding, so sizeof() can compute padding */
    Rm_GlobalPermissionsObj_Unpadded obj;
    /** Pad out to end of RM_MAX_CACHE_ALIGN bytes to prevent something else
     * from being placed on same cache line as Rm_Synci_Obj.  Note that 
     * pad[0] is illegal, so must add full RM_MAX_CACHE_ALIGN if structure
     * is already padded by chance. */
    uint8_t  pad[RM_MAX_CACHE_ALIGN - 
                       (sizeof(Rm_GlobalPermissionsObj_Unpadded) % RM_MAX_CACHE_ALIGN)];
} Rm_GlobalPermissionsObj;

extern void Rm_permissionTableInit(void);
extern void Rm_setTablePermissions (const Rm_Resource *resourceEntry, Rm_Perms *rmPermsArray, uint32_t len);
extern Rm_Result Rm_populatePermissionTable(const Rm_Resource *rmResourceTable);
extern void Rm_updatePermissionTable(void);
extern Rm_Result Rm_getInitPermissions (Rm_Perms *resourcePermissions);
extern Rm_Result Rm_getUsePermissions (Rm_Perms *resourcePermissions);

/* Permission checker handlers */
extern Rm_Result Rm_initPermissionChecker (Rm_ResourceInfo *resourceData);
extern Rm_Result Rm_usePermissionChecker (Rm_ResourceInfo *resourceData);

#ifdef __cplusplus
}
#endif

#endif /* RM_PVT_H_ */

