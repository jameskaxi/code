/**
 *   @file  qmss_qm.h
 *
 *   @brief   
 *      This is the Queue Manager module include file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
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


/** @defgroup QMSS_LLD_API QMSS
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *
 *
 * @subsection References
 *   -# QMSS Functional Specification 
 *
 * @subsection Assumptions
 *    
 */
#ifndef QMSS_QM_H_
#define QMSS_QM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QM device specific include file */
//#include <ti/drv/qmss/device/qmss_device.h>
        
/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_qm_config.h>
#include <ti/csl/cslr_qm_descriptor_region_config.h>
#include <ti/csl/cslr_qm_queue_management.h>
#include <ti/csl/cslr_qm_queue_status_config.h>
#include <ti/csl/cslr_pdsp.h>
#include <ti/csl/cslr_qm_intd.h>
#include <ti/csl/cslr_mcdma.h>
#include <ti/csl/cslr_cp_timer16.h>
#include <ti/csl/csl_qm_queue.h>

/**
@defgroup QMSS_LLD_SYMBOL  QMSS Low Level Driver Symbols Defined
@ingroup QMSS_LLD_API
*/
/**
@defgroup QMSS_LLD_ENUM  QMSS Low Level Driver Enums
@ingroup QMSS_LLD_API
*/
/**
@defgroup QMSS_LLD_DATASTRUCT  QMSS Low Level Driver Data Structures
@ingroup QMSS_LLD_API
*/
/**
@defgroup QMSS_LLD_FUNCTION  QMSS Low Level Driver Functions
@ingroup QMSS_LLD_API
*/
/**
@defgroup QMSS_LLD_OSAL  QMSS Low Level Driver OSAL Functions
@ingroup QMSS_LLD_API
*/

/**
@addtogroup QMSS_LLD_SYMBOL
@{
*/

/** Internal Linking RAM offset */
#define QMSS_LINKING_RAM_OFFSET                     0x80000
/** Internal Linking RAM default size */
#define QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE      0x3FFF

/** Used as input parameter when queue number is 
 * not known and not specified */
#define QMSS_PARAM_NOT_SPECIFIED                    -1

/** Used to indicate that QMSS HW Initialization is complete */
#define QMSS_HW_INIT_COMPLETE                       0xABCD

/** QMSS Low level Driver return and Error Codes */
/** QMSS successful return code */
#define QMSS_SOK                                    0
/** QMSS Error Base */       
#define QMSS_LLD_EBASE                              (-128)
/** QMSS LLD invalid parameter */
#define QMSS_INVALID_PARAM                          QMSS_LLD_EBASE-1
/** QMSS LLD not initialized */
#define QMSS_NOT_INITIALIZED                        QMSS_LLD_EBASE-2
/** QMSS LLD queue open error */
#define QMSS_QUEUE_OPEN_ERROR                       QMSS_LLD_EBASE-3
/** QMSS memory region not initialized */
#define QMSS_MEMREGION_NOT_INITIALIZED              QMSS_LLD_EBASE-4
/** QMSS memory region already initialized */
#define QMSS_MEMREGION_ALREADY_INITIALIZED          QMSS_LLD_EBASE-5
/** QMSS memory region invalid parameter */
#define QMSS_MEMREGION_INVALID_PARAM                QMSS_LLD_EBASE-6
/** QMSS maximum number of allowed descriptor are already configured */
#define QMSS_MAX_DESCRIPTORS_CONFIGURED             QMSS_LLD_EBASE-7
/** QMSS Specified memory region index is invalid or no memory regions are available */
#define QMSS_MEMREGION_INVALID_INDEX                QMSS_LLD_EBASE-8
/** QMSS memory region overlap */
#define QMSS_MEMREGION_OVERLAP                      QMSS_LLD_EBASE-9
/** QMSS memory region not in acscending order */
#define QMSS_MEMREGION_ORDERING                     QMSS_LLD_EBASE-10
/** QMSS PDSP firmware download failure */
#define QMSS_FIRMWARE_DOWNLOAD_FAILED               QMSS_LLD_EBASE-11
/** QMSS resource initialization permission denied */
#define QMSS_RESOURCE_INIT_DENIED                   QMSS_LLD_EBASE-12
/** QMSS resource usage permission denied */
#define QMSS_RESOURCE_USE_DENIED                    QMSS_LLD_EBASE-13
/** QMSS memory region initialization permission denied */
#define QMSS_RESOURCE_MEM_REGION_INIT_DENIED         QMSS_LLD_EBASE-14
/** QMSS memory region usage permission denied */
#define QMSS_RESOURCE_MEM_REGION_USE_DENIED         QMSS_LLD_EBASE-15
/** QMSS general linking RAM initialization permission denied */
#define QMSS_RESOURCE_LINKING_RAM_INIT_DENIED        QMSS_LLD_EBASE-16
/** QMSS firmware revision difference */
#define QMSS_FIRMWARE_REVISION_DIFFERENCE           QMSS_LLD_EBASE-17

/** QMSS maximum number of memory regions */
#define QMSS_MAX_MEM_REGIONS                        20
#define QMSS_MAX_PDSP                               2

/** Macro to get the descriptor pointer if the popped descriptor contains the descriptor size. 
 * If Qmss_queuePushDescSize() API is used to push a descriptor onto a queue, the descriptor when 
 * popped will have the descriptor size information in the lower 4 bits. This macro is provided to 
 * clear out the size information */
#define QMSS_DESC_PTR(desc)                         ((uint32_t)desc & 0xFFFFFFF0)

/** Macro to get the descriptor size if the popped descriptor contains the descriptor size. 
 * If Qmss_queuePushDescSize() API is used to push a descriptor onto a queue, the descriptor when 
 * popped will have the descriptor size information in the lower 4 bits. This macro is provided to 
 * obtain the size information. Minimum size is 16 bytes. Maximum size is 256 bytes */
#define QMSS_DESC_SIZE(desc)                        ((((uint32_t)desc & 0x0000000F) + 1) << 4) 

/**
@}
*/

/**
@addtogroup QMSS_LLD_ENUM
@{
*/

/** 
 * @brief location where the packet is queued
 */
typedef enum
{
    /** Queue packet to the tail of the queue. Default behavior. */
    Qmss_Location_TAIL = 0,
    /** Queue packet to the head of the queue. */
    Qmss_Location_HEAD 
}Qmss_Location;

/** 
 * @brief Descriptor resource management
 */
typedef enum
{
    /** LLD doesnot manage the descriptors. The caller should manage them. */
    Qmss_ManageDesc_UNMANAGED_DESCRIPTOR = 0,
    /** LLD manages the descriptors. The descriptors are reclaimed using 
     * the QMSS_initDescriptor() or CPPI_initDescriptor() APIs
     * */
    Qmss_ManageDesc_MANAGE_DESCRIPTOR 
}Qmss_ManageDesc;

/** 
 * @brief Queue Manager's memory regions
 */
typedef enum
{
    /** Memory region not specified. LLD allocates the next available memory region */
    Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED = -1,
    /** Configure memory region0. */
    Qmss_MemRegion_MEMORY_REGION0 = 0,
    /** Configure memory region 1. */
    Qmss_MemRegion_MEMORY_REGION1,
    /** Configure memory region 2. */
    Qmss_MemRegion_MEMORY_REGION2,
    /** Configure memory region 3. */
    Qmss_MemRegion_MEMORY_REGION3,
    /** Configure memory region 4. */
    Qmss_MemRegion_MEMORY_REGION4,
    /** Configure memory region 5. */
    Qmss_MemRegion_MEMORY_REGION5,
    /** Configure memory region 6. */
    Qmss_MemRegion_MEMORY_REGION6,
    /** Configure memory region 7. */
    Qmss_MemRegion_MEMORY_REGION7,
    /** Configure memory region 8. */
    Qmss_MemRegion_MEMORY_REGION8,
    /** Configure memory region 9. */
    Qmss_MemRegion_MEMORY_REGION9,
    /** Configure memory region 10. */
    Qmss_MemRegion_MEMORY_REGION10,
    /** Configure memory region 11. */
    Qmss_MemRegion_MEMORY_REGION11,
    /** Configure memory region 12. */
    Qmss_MemRegion_MEMORY_REGION12,
    /** Configure memory region 13. */
    Qmss_MemRegion_MEMORY_REGION13,
    /** Configure memory region 14. */
    Qmss_MemRegion_MEMORY_REGION14,
    /** Configure memory region 15. */
    Qmss_MemRegion_MEMORY_REGION15,
    /** Configure memory region 16. */
    Qmss_MemRegion_MEMORY_REGION16,
    /** Configure memory region 17. */
    Qmss_MemRegion_MEMORY_REGION17,
    /** Configure memory region 18. */
    Qmss_MemRegion_MEMORY_REGION18,
    /** Configure memory region 19. */
    Qmss_MemRegion_MEMORY_REGION19
}Qmss_MemRegion;

/** 
 * @brief PDSP ID
 */
typedef enum
{
    /** PDSP 1 */
    Qmss_PdspId_PDSP1 = 0,
    /** PDSP 2 */
    Qmss_PdspId_PDSP2
}Qmss_PdspId;

/** 
 * @brief INTD interrupt types
 */
typedef enum
{
    /** Interrupt generated for the high priority accumulator.
     * 32 interrupts are generated in response to events in the 32 high-priority queues.
     */
    Qmss_IntdInterruptType_HIGH = 0,
    /** Interrupt generated for the low priority accumulator.
     * 16 interrupts are generated in response to events in the 512 low-priority queues.
     */
    Qmss_IntdInterruptType_LOW,
    /** Interrupt generated for QMSS CDMA.
     * 2 interrupts are generated for buffer descriptor starvation event on 
     * receive SOP (start of packet) and MOP (middle of packet) for any of the receive DMA units in the CDMA.
     */
    Qmss_IntdInterruptType_CDMA
}Qmss_IntdInterruptType;

/**
@}
*/

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief QMSS RM Handle
 */
typedef void *  Qmss_RmHnd;

/** 
 * @brief QMSS start configuration structure
 */
typedef struct
{
    /** Provide a handle to the Resource Manager instance */
    Qmss_RmHnd rmHandle;
} Qmss_StartCfg;

/** 
 * @brief Queue definition
 */
typedef struct
{
    /** Queue manager number */
    int32_t qMgr;        
    /** Queue number within Queue Manager */
    int32_t qNum;
}Qmss_Queue;

/** 
 * @brief Queue definition
 */
typedef struct
{
    /** Queue manager number */
    int32_t     startIndex;        
    /** Queue number within Queue Manager */
    int32_t     maxNum;
}Qmss_QueueNumRange;

/** 
 * @brief descriptor configuration structure
 */
typedef struct 
{
    uint32_t          memRegion;
    /** Number of descriptors that should be allocated */
    uint32_t          descNum;
    /** Queue where the descriptor is stored. If QueueNum is set to QMSS_PARAM_NOT_SPECIFIED then the next 
     * available queue of type Qmss_QueueType will be allocated */
    int32_t           destQueueNum;
    /** If QueueNum is set to QMSS_PARAM_NOT_SPECIFIED then the next available queue of type 
     * Qmss_QueueType will be allocated */
    Qmss_QueueType  queueType;
}Qmss_DescCfg;

/** 
 * @brief Memory region configuration information structure
 */
typedef struct 
{
    /** The base address of descriptor region. Note the 
     * descriptor Base address must be specified in ascending memory order
     * */
    uint32_t          *descBase;
    /** Size of each descriptor in the memory region. Must be a multiple of 16 */
    uint32_t          descSize;
    /** Number of descriptors in the memory region. 
     * Must be a minimum of 32. 
     * Must be 2^(5 or greater) 
     * Maximum supported value 2^20
     * */
    uint32_t          descNum;

    /** Memory Region corresponding to the descriptor. 
     * At init time this field must have a valid memory region 
     * index (0 to Maximum number of memory regions supported).
     *
     * At runtime this field is used to either 
     *      * set to Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED, in this case the LLD 
     *      * will decide which memory region to use.
     *      * OR 
     *      * specify the descriptor memory region, must be a valid memory 
     *      * region index (0 to Maximum number of memory regions supported).
     */
    Qmss_MemRegion  memRegion;
    /** Flag control whether the descriptors are managed 
     * by LLD or by the caller allocating descriptor memory */
    Qmss_ManageDesc manageDescFlag;
    /** Used to leave holes by configuring dummy regions which can be later 
     * configured with actual values. Must be calculated and a correct startIndex must be 
     * specified if memRegion value is valid (0 to Maximum number of memory regions supported). */
    uint32_t          startIndex;
} Qmss_MemRegInfo;

/** 
 * @brief Memory region configuration information structure of all memory regions
 */
typedef struct 
{
    /** Descriptor information for each CPDMA passed during cppi_Init */
    Qmss_MemRegInfo memRegInfo[QMSS_MAX_MEM_REGIONS];
    /** Current descriptor count. Sum of descriptors in all memory regions */
    uint32_t          currDescCnt;
} Qmss_MemRegCfg;

/** 
 * @brief QMSS PDSP firmware download information structure
 */
typedef struct
{
    /** ID of the PDSP to download this firmware to */
    Qmss_PdspId     pdspId;
    /** Pointer to the firmware image, If the firmware pointer is NULL, LLD will not 
     * download the firmware */
    void            *firmware;
    /** Size of firmware in bytes */
    uint32_t          size; 
}Qmss_PdspCfg;

/** 
 * @brief QMSS configuration structure
 */
typedef struct
{
    /** Base address of Linking RAM 0. LLD will configure linking RAM0 address to internal linking RAM 
     * address if a value of zero is specified. */
    uint32_t          linkingRAM0Base;
    /** Linking RAM 0 Size. LLD will configure linking RAM0 size to maximum internal linking RAM 
     * size if a value of zero is specified*/
    uint32_t          linkingRAM0Size; 
    /** Base address of Linking RAM 1. Depends on RAM 0 Size and total number of 
     * descriptors. If linkingRAM1Base is zero then linkingRAM0Size must be large 
     * enough to store all descriptors in the system */
    uint32_t          linkingRAM1Base;
    /** Maximum number of descriptors in the system. Should be equal to less than 
     * the RAM0+RAM1 size */
    uint32_t          maxDescNum;
    /** PDSP firmware to download. If the firmware pointer is NULL, LLD will not download the firmware */
    Qmss_PdspCfg    pdspFirmware[QMSS_MAX_PDSP];
    /** Status of QMSS HW. Set this to QMSS_HW_INIT_COMPLETE in case Initialization is already complete.
      * Setting this flag will bypass any QMSS Hardware initialization
      */
    uint32_t          qmssHwStatus;
}Qmss_InitCfg;

/** 
 * @brief QMSS Global configuration structure definition
 */
typedef struct
{
    /** Maximum number of queue Managers */
    uint32_t                                maxQueMgr;
    /** Maximum number of queues */
    uint32_t                                maxQue;

    /** Queue start index and maximum number of queues of each queue type */
    Qmss_QueueNumRange                      maxQueueNum[25];

    /** Base address for the CPDMA overlay registers */

    /** QM Global Config registers */
    CSL_Qm_configRegs                       *qmConfigReg;
    /** QM Descriptor Config registers */
    CSL_Qm_descriptor_region_configRegs     *qmDescReg;
    /** QM queue Management registers, accessed via CFG port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtReg;
    /** QM queue Management Proxy registers, accessed via CFG port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtProxyReg;
    /** QM queue status registers */
    CSL_Qm_queue_status_configRegs          *qmQueStatReg;
    /** QM INTD registers */
    CSL_Qm_intdRegs                         *qmQueIntdReg;
    /** QM PDSP command register */
    volatile uint32_t                       *qmPdspCmdReg[QMSS_MAX_PDSP];
    /** QM PDSP control register */
    CSL_PdspRegs                            *qmPdspCtrlReg[QMSS_MAX_PDSP];
    /** QM PDSP IRAM register */
    volatile uint32_t                       *qmPdspIRamReg[QMSS_MAX_PDSP];
    /** QM Status RAM */
    CSL_Qm_Queue_Status                     *qmStatusRAM;
    /** QM Linking RAM register */
    volatile uint32_t                       *qmLinkingRAMReg;
    /** QM McDMA register */
    CSL_McdmaRegs                           *qmMcDMAReg;
    /** QM Timer16 register */
    CSL_Cp_timer16Regs                      *qmTimer16Reg[2];
    /** QM queue Management registers, accessed via DMA port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtDataReg;
    /** QM queue Management Proxy registers, accessed via DMA port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtProxyDataReg;
    /** QM stores the Resource Manager handle for internal use */
    Qmss_RmHnd                              qmRmHandle;

}Qmss_GlobalConfigParams;

/** 
 * @brief Queue handle
 */
typedef int32_t   Qmss_QueueHnd;

/** 
 * @brief QMSS return result
 */
typedef int32_t   Qmss_Result;

/** 
 * @brief Handle used in the "Fast Push" set of APIs
 */
typedef uint32_t* Qmss_QueuePushHnd;

/** 
@} 
*/

/* Exported functions */
extern Qmss_Result Qmss_init (Qmss_InitCfg *initCfg, Qmss_GlobalConfigParams *qmssGblCfgParams);
extern Qmss_Result Qmss_start (void);
extern Qmss_Result Qmss_startCfg(Qmss_StartCfg *startCfg);
extern Qmss_Result Qmss_getMemoryRegionCfg (Qmss_MemRegCfg *memRegInfo);
extern Qmss_Result Qmss_insertMemoryRegion (Qmss_MemRegInfo *memRegCfg);
extern Qmss_QueueHnd Qmss_initDescriptor (Qmss_DescCfg *descCfg, uint32_t *numAllocated);
extern Qmss_QueueHnd Qmss_queueOpen (Qmss_QueueType queType, int32_t queNum, uint8_t *isAllocated);
extern Qmss_QueueHnd Qmss_queueOpenInRange (uint32_t startQueNum, uint32_t endQueNum, uint8_t *isAllocated);
extern Qmss_Result Qmss_queueClose (Qmss_QueueHnd hnd);
extern uint32_t Qmss_getQueueThreshold (Qmss_QueueHnd hnd);
extern Qmss_Result Qmss_setQueueThreshold (Qmss_QueueHnd hnd, uint16_t hilo, uint8_t threshold);
extern uint32_t Qmss_getStarvationCount (Qmss_QueueHnd hnd);
extern uint16_t Qmss_getQueueThresholdStatus (Qmss_QueueHnd hnd);
extern Qmss_Queue Qmss_getQueueNumber (Qmss_QueueHnd hnd);
extern Qmss_QueueHnd Qmss_getQueueHandle (Qmss_Queue queue);
extern uint32_t Qmss_getMemRegDescSize (uint32_t memRegion);
extern Qmss_Result Qmss_downloadFirmware (Qmss_PdspId pdspId, void *image, uint32_t size);
extern Qmss_Result Qmss_setEoiVector (Qmss_IntdInterruptType type, uint8_t accumCh);
extern Qmss_Result Qmss_ackInterrupt (uint8_t interruptNum, uint8_t value);
extern uint32_t Qmss_getVersion (void);
extern const char* Qmss_getVersionStr (void);
 
#ifdef __cplusplus
}
#endif

#endif /* QMSS_QM_H_ */

