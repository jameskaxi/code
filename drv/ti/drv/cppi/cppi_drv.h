/**
 *   @file  cppi_drv.h
 *
 *   @brief   
 *      This is the CPPI Low Level Driver include file.
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


/** @defgroup CPPI_LLD_API CPPI
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *
 *
 * @subsection References
 *   -# CPPI Functional Specification 
 *
 * @subsection Assumptions
 *    
 */
#ifndef CPPI_DRV_H_
#define CPPI_DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_cppidma_global_config.h>
#include <ti/csl/cslr_cppidma_rx_channel_config.h>
#include <ti/csl/cslr_cppidma_rx_flow_config.h>
#include <ti/csl/cslr_cppidma_tx_channel_config.h>
#include <ti/csl/cslr_cppidma_tx_scheduler_config.h>
#include <ti/csl/csl_cppi.h>
#include <ti/drv/cppi/cppiver.h>

/**
@defgroup CPPI_LLD_SYMBOL  CPPI Low Level Driver Symbols Defined
@ingroup CPPI_LLD_API
*/
/**
@defgroup CPPI_LLD_ENUM  CPPI Low Level Driver Enums
@ingroup CPPI_LLD_API
*/
/**
@defgroup CPPI_LLD_DATASTRUCT  CPPI Low Level Driver Data Structures
@ingroup CPPI_LLD_API
*/
/**
@defgroup CPPI_LLD_FUNCTION  CPPI Low Level Driver Functions
@ingroup CPPI_LLD_API
*/
/**
@defgroup CPPI_LLD_OSAL  CPPI Low Level Driver OSAL Functions
@ingroup CPPI_LLD_API
*/

/**
@addtogroup CPPI_LLD_SYMBOL
@{
*/

/** Used as input parameter when queue number is 
 * not known and not specified */
#define CPPI_PARAM_NOT_SPECIFIED            -1

/** CPPI Low level Driver return and Error Codes */
/** CPPI successful return code */
#define CPPI_SOK                            0
/** CPPI Error Base */       
#define CPPI_LLD_EBASE                      (-128)
/** CPPI CPDMA not yet initialized */
#define CPPI_CPDMA_NOT_INITIALIZED          CPPI_LLD_EBASE-1
/** CPPI invalid parameter */
#define CPPI_INVALID_PARAM                  CPPI_LLD_EBASE-2
/** CPPI Rx/Tx channel not yet enabled */
#define CPPI_CHANNEL_NOT_OPEN               CPPI_LLD_EBASE-3
/** CPPI Rx flow not yet enabled */
#define CPPI_FLOW_NOT_OPEN                  CPPI_LLD_EBASE-4
/** CPPI Tx channels are still open. 
 * All Tx channels should be closed 
 * before calling CPPI_close */
#define CPPI_TX_CHANNELS_NOT_CLOSED         CPPI_LLD_EBASE-5
/** CPPI Rx channels are still open. 
 * All Rx channels should be closed 
 * before calling CPPI_close */
#define CPPI_RX_CHANNELS_NOT_CLOSED         CPPI_LLD_EBASE-6
/** CPPI Rx flows are still open. 
 * All Rx flows should be closed 
 * before calling CPPI_close */
#define CPPI_RX_FLOWS_NOT_CLOSED            CPPI_LLD_EBASE-7

/** Queue Manager subsystem memory region not enabled */
#define CPPI_QMSS_MEMREGION_NOT_INITIALIZED CPPI_LLD_EBASE-8
/** Queue open error */
#define CPPI_QUEUE_OPEN_ERROR               CPPI_LLD_EBASE-9
/** CPPI extended packet information block not present in descriptor */
#define CPPI_EPIB_NOT_PRESENT               CPPI_LLD_EBASE-10
/** CPPI protocol specific data not present in descriptor */
#define CPPI_PSDATA_NOT_PRESENT             CPPI_LLD_EBASE-11
/** CPPI CPDMA instances are still open. 
 * All CPDMA instances should be closed 
 * before calling CPPI_exit */
#define CPPI_CPDMA_NOT_CLOSED               CPPI_LLD_EBASE-12
/** CPPI resource initialization permission denied */
#define CPPI_RESOURCE_INIT_DENIED           CPPI_LLD_EBASE-13
/** CPPI resource usage permission denied */
#define CPPI_RESOURCE_USE_DENIED            CPPI_LLD_EBASE-14

/**
@}
*/

/**
@addtogroup CPPI_LLD_ENUM
@{
*/

/** 
 * @brief CPPI Channel type 
 */
typedef enum
{
    /** Receive Channel */
    Cppi_ChType_RX_CHANNEL = 0,
    /** Transmit Channel */
    Cppi_ChType_TX_CHANNEL
}Cppi_ChType;

/** 
 * @brief CPPI Channel Enable
 */
typedef enum
{
    /** Disable Channel */
    Cppi_ChState_CHANNEL_DISABLE = 0,
    /** Enable Channel */
    Cppi_ChState_CHANNEL_ENABLE 
}Cppi_ChState;

/** 
 * @brief CPPI Wait after Channel Teardown
 */
typedef enum
{
    /** No wait */
    Cppi_Wait_NO_WAIT = 0,
    /** Wait */
    Cppi_Wait_WAIT
}Cppi_Wait;


/**
@}
*/

/** @addtogroup CPPI_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief CPPI global configuration structure
 */
typedef struct
{
    /** CPDMA this configuration belongs to */
    Cppi_CpDma      dmaNum;
    /** Maximum supported Rx Channels */
    uint32_t          maxRxCh;
    /** Maximum supported Tx Channels */
    uint32_t          maxTxCh;
    /** Maximum supported Rx Flows */
    uint32_t          maxRxFlow;
    /** Priority for all Rx transactions of this CPDMA */
    uint8_t           rxPriority;
    /** Priority for all Tx transactions of this CPDMA */
    uint8_t           txPriority;

    /** Base address for the CPDMA overlay registers */

    /** Global Config registers */
    CSL_Cppidma_global_configRegs       *gblCfgRegs;
    /** Tx Channel Config registers */
    CSL_Cppidma_tx_channel_configRegs   *txChRegs;
    /** Rx Channel Config registers */
    CSL_Cppidma_rx_channel_configRegs   *rxChRegs;
    /** Tx Channel Scheduler registers */
    CSL_Cppidma_tx_scheduler_configRegs *txSchedRegs;
    /** Rx Flow Config registers */
    CSL_Cppidma_rx_flow_configRegs      *rxFlowRegs;
}Cppi_GlobalConfigParams;

/** 
 * @brief CPPI heap configuration structure (optional)
 */
typedef struct
{
    /** Optional static heap.  In order to prevent the use of Cppi_osalMalloc,
     * the size should be at least the size returned by @ref Cppi_getHeapReq.
     * Otherwise, Cppi_osalMalloc will be used when the static heap is used.
     * A value of NULL disables the static heap */
    void                        *staticHeapBase;

    /** Optional static heap size in bytes.  Only used when staticHeapBase != NULL */
    uint32_t                     staticHeapSize;

    /** Heap alignment: power of 2 to align front and back of heap.  This
     * applies to both static and dynamic heaps.  A value of 0 means use
     * platform default which is 128 bytes (2^7).  The minimum for correct
     * functionality without shared memory is 3 (2^3 = 8).  */
    uint32_t                     heapAlignPow2;

    /** Dynamic heap block size in bytes.  This is the amount requested from
     * Cppi_osalMalloc, whenever more memory is needed.  Up to @ref heapAlignPow2
     * can be lost at both beginning and end due to alignment, so it is recommended
     * to make this value at least 4*(2^heapAlignPow2) or a minimum of 256 bytes,
     * whichever is larger.  A value of 0 will cause a system default (1024) to
     * be used.  A value < 0 will disable dynamic allocation */
    int32_t                      dynamicHeapBlockSize;
} Cppi_HeapParams;

/** 
 * @brief CPPI CPDMA configuration structure
 */
typedef struct
{
    /** CPDMA configuring control registers */
    Cppi_CpDma                  dmaNum;

    /** This field sets the depth of the write arbitration FIFO which stores write transaction information
     * between the command arbiter and write data arbiters in the Bus Interface Unit. Setting this field to smaller 
     * values will cause prevent the CDMAHP from having an excess of write transactions outstanding whose data is 
     * still waiting to be transferred.
     * System performance can suffer if write commands are allowed to be issued long before the corresponding 
     * write data will be transferred.  This field allows the command count to be optimized based on system dynamics
     *
     * Valid range is 1 to 32. If writeFifoDepth field is set to 0, this field will not be configured. The reset/default value is 20.
     */
    uint8_t                     writeFifoDepth;
    /** This field sets the timeout duration in clock cycles.  This field controls the minimum 
     * amount of time that an Rx channel will be required to wait when it encounters a buffer starvation 
     * condition and the Rx error handling bit is set to 1 (packet is to be preserved - no discard).  
     * If the Rx error handling bit in the flow table is cleared, this field will have no effect on the Rx operation.  
     * When this field is set to 0, the Rx engine will not force an Rx channel to wait after encountering a starvation 
     * event (the feature is disabled).  When this field is set to a value other than 0, the Rx engine will force any 
     * channel whose associated flow had the Rx error handling bit asserted and which encounters starvation to wait for 
     * at least the specified # of clock cycles before coming into context again to retry the access to the QM
     */ 
    uint16_t                    timeoutCount;
    /** The QM N Queues Region Base Address Register is used to provide a programmable 
     * pointer to the base address of the queues region in Queue Manager N in the system 
     */

    /** Queue Manager 0 base address register */
    volatile uint32_t            qm0BaseAddress;
    /** Queue Manager 1 base address register */
    volatile uint32_t            qm1BaseAddress;
    /** Queue Manager 2 base address register */
    volatile uint32_t            qm2BaseAddress;
    /** Queue Manager 3 base address register */
    volatile uint32_t            qm3BaseAddress;
}Cppi_CpDmaInitCfg;

/** 
 * @brief CPPI transmit channel configuration structure
 */
typedef struct
{
    /** Channel number */
    /** If channelNum is set to CPPI_PARAM_NOT_SPECIFIED then the next 
     * available channel will be allocated */
    int32_t           channelNum;
    /** Enable Tx Channel on creation. If not set use CPPI_channelEnable() API to enable it later */
    Cppi_ChState    txEnable;
    /** Tx scheduling priority for channelNum */
    uint8_t           priority;
    /** Tx Filter Software Info.  This field controls whether or not the DMA controller will pass the 
     * extended packet information fields (if present) from the descriptor to the back end application.
     * 0 - DMA controller will pass extended packet info fields if they are present in the descriptor
     * 1 - DMA controller will filter extended packet info fields
     */
    uint16_t             filterEPIB;
    /** Filter Protocol Specific Words. This field controls whether or not the DMA controller will 
     * pass the protocol specific words (if present) from the descriptor to the back end application.
     * 0 - DMA controller will pass PS words if present in descriptor
     * 1 - DMA controller will filter PS words
     */
    uint16_t             filterPS;
    /**
     * AIF Specific Monolithic Packet Mode. This field when set indicates that all monolithic packets 
     * which will be transferred on this channel will be formatted in an optimal configuration as needed 
     * by the Antenna Interface Peripheral.  The AIF configuration uses a fixed descriptor format which 
     * includes the 3 mandatory descriptor info words, a single Protocol Specific Word and data 
     * immediately following (data offset = 16).
     */
    uint16_t             aifMonoMode;
}Cppi_TxChInitCfg;

/** 
 * @brief CPPI receive channel configuration structure
 */
typedef struct
{
    /** Channel number */
    /** If channelNum is set to CPPI_PARAM_NOT_SPECIFIED then the next 
     * available channel will be allocated */
    int32_t           channelNum;
    /** Enable Rx Channel on creation. If not set use CPPI_channelEnable() API to enable it later */
    Cppi_ChState    rxEnable;
}Cppi_RxChInitCfg;

/** 
 * @brief CPPI receive flow configuration structure
 */
typedef struct 
{
    /** Rx flow configuration register A */

    /** flow ID number */
    /** If flowIdNum is set to CPPI_PARAM_NOT_SPECIFIED then the next available flow ID will be allocated */
    int16_t           flowIdNum;
    /** This field indicates the default receive queue that this channel should use */
    uint16_t          rx_dest_qnum;
    /** This field indicates the default receive queue manager that this channel should use */
    uint16_t          rx_dest_qmgr;
    /** This field specifies the number of bytes that are to be skipped in the SOP buffer before beginning 
     * to write the payload or protocol specific bytes(if they are in the sop buffer).  This value must
     * be less than the minimum size of a buffer in the system */
    uint16_t          rx_sop_offset;
    /** This field controls where the Protocol Specific words will be placed in the Host Mode CPPI data structure 
     * 0 - protocol specific information is located in descriptor 
     * 1 - protocol specific information is located in SOP buffer */
    uint16_t             rx_ps_location;
    /** This field indicates the descriptor type to use 1 = Host, 2 = Monolithic */
    uint8_t           rx_desc_type;
    /** This field controls the error handling mode for the flow and is only used when channel errors occurs 
     * 0 = Starvation errors result in dropping packet and reclaiming any used descriptor or buffer resources 
     * back to the original queues/pools they were allocated to
     * 1 = Starvation errors result in subsequent re-try of the descriptor allocation operation.  
     */
    uint16_t             rx_error_handling;
    /** This field controls whether or not the Protocol Specific words will be present in the Rx Packet Descriptor 
     * 0 - The port DMA will set the PS word count to 0 in the PD and will drop any PS words that are presented 
     * from the back end application.
     * 1 - The port DMA will set the PS word count to the value given by the back end application and will copy 
     * the PS words from the back end application to the location 
     */
    uint16_t             rx_psinfo_present;
    /** This field controls whether or not the Extended Packet Info Block will be present in the Rx Packet Descriptor.  
     * 0 - The port DMA will clear the Extended Packet Info Block Present bit in the PD and will drop any extended 
     * packet info words that are presented from the back end application. 
     * 1 - The port DMA will set the Extended Packet Info Block Present bit in the PD and will copy any extended packet
     * info words that are presented across the Rx streaming interface into the extended packet info words in the descriptor.
     * If no extended packet info words are presented from the back end application, the port DMA will overwrite the fields with zeroes.
     */
    uint16_t             rx_einfo_present;

    /** Rx flow configuration register B */

    /** This is the value to insert into bits 7:0 of the destination tag if the rx_dest_tag_lo_sel is set to 1 */
    uint8_t           rx_dest_tag_lo;
    /** This is the value to insert into bits 15:8 of the destination tag if the rx_dest_tag_hi_sel is set to 1 */
    uint8_t           rx_dest_tag_hi;
    /** This is the value to insert into bits 7:0 of the source tag if the rx_src_tag_lo_sel is set to 1 */
    uint8_t           rx_src_tag_lo;
    /** This is the value to insert into bits 15:8 of the source tag if the rx_src_tag_hi_sel is set to 1 */
    uint8_t           rx_src_tag_hi;    

    /** Rx flow configuration register C */
    /** This bits control whether or not the flow will compare the packet size received from the back end application 
     * against the rx_size_thresh0 fields to determine which FDQ to allocate the SOP buffer from.  
     * The bits in this field is encoded as follows:
     * 0 = Do not use the threshold.
     * 1 = Use the thresholds to select SOP FDQ rx_fdq0_sz0_qnum/rx_fdq0_sz0_qmgr.
     */
    uint8_t             rx_size_thresh0_en;
    /** This bits control whether or not the flow will compare the packet size received from the back end application 
     * against the rx_size_thresh1 fields to determine which FDQ to allocate the SOP buffer from.  
     * The bits in this field is encoded as follows:
     * 0 = Do not use the threshold.
     * 1 = Use the thresholds to select SOP FDQ rx_fdq0_sz1_qnum/rx_fdq0_sz1_qmgr.
     */
    uint8_t             rx_size_thresh1_en;
        /** This bits control whether or not the flow will compare the packet size received from the back end application 
     * against the rx_size_thresh2 fields to determine which FDQ to allocate the SOP buffer from.  
     * The bits in this field is encoded as follows:
     * 0 = Do not use the threshold.
     * 1 = Use the thresholds to select SOP FDQ rx_fdq0_sz2_qnum/rx_fdq0_sz2_qmgr.
     */
    uint8_t             rx_size_thresh2_en;

    /** This field specifies the source for bits 7:0 of the source tag field in word 1 of the output PD.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_dest_tag_lo
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with dest_tag[7:0] from back end application
     * 5 = overwrite with dest_tag[15:8] from back end application
     * 6-7 = RESERVED
     */    
    uint8_t           rx_dest_tag_lo_sel;
    /** This field specifies the source for bits 15:8 of the source tag field in the word 1 of the output PD.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_dest_tag_hi
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with dest_tag[7:0] from back end application
     * 5 = overwrite with dest_tag[15:8] from back end application
     * 6-7 = RESERVED
     */
    uint8_t           rx_dest_tag_hi_sel;
    /** This field specifies the source for bits 7:0 of the source tag field in the output packet descriptor.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_src_tag_lo
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with src_tag[7:0] from back end application
     * 5 = RESERVED
     * 6-7 = RESERVED
     */
    uint8_t           rx_src_tag_lo_sel;
    /** This field specifies the source for bits 15:8 of the source tag field in the output packet descriptor.
     * This field is encoded as follows:
     * 0 = do not overwrite
     * 1 = overwrite with value given in rx_src_tag_hi
     * 2 = overwrite with flow_id[7:0] from back end application
     * 3 = RESERVED
     * 4 = overwrite with src_tag[7:0] from back end application
     * 5 = RESERVED
     * 6-7 = RESERVED
     */
    uint8_t           rx_src_tag_hi_sel;    
 
    /** Rx flow configuration register D */

    /** This field specifies which Free Descriptor Queue should be used for the 2nd Rx buffer in a host type packet */
    uint16_t          rx_fdq1_qnum;
    /** This field specifies which Queue Manager should be used for the 2nd Rx buffer in a host type packet */
    uint16_t          rx_fdq1_qmgr;
    /** This field specifies which Free Descriptor Queue should be used for the 1st Rx buffer in a packet whose 
     * size is less than or equal to the rx_size0 value */
    uint16_t          rx_fdq0_sz0_qnum;
    /** This field specifies which Queue Manager should be used for the 1st Rx buffer in a packet whose size 
     * is less than or equal to the rx_size0 value */
    uint16_t          rx_fdq0_sz0_qmgr;

    /** Rx flow configuration register E */

    /** This field specifies which Free Descriptor Queue should be used for the 4th or later Rx
     *  buffers in a host type packet */
    uint16_t          rx_fdq3_qnum;
    /** This field specifies which Queue Manager should be used for the 4th or later Rx buffers 
     * in a host type packet */
    uint16_t          rx_fdq3_qmgr;
    /** This field specifies which Free Descriptor Queue should be used for the 3rd Rx buffer in a host type packet */
    uint16_t          rx_fdq2_qnum;
    /** This field specifies which Queue Manager should be used for the 3rd Rx buffer in a host type packet */
    uint16_t          rx_fdq2_qmgr;

    /** Rx flow configuration register F */

    /** This value is left shifted by 5 bits and compared against the packet size to determine which free descriptor 
     * queue should be used for the SOP buffer in the packet.  If the  packet size is greater than the rx_size_thresh0 
     * but is less than or equal to the value given in this threshold, the DMA controller in the port will allocate the 
     * SOP buffer from the queue given by the rx_fdq0_sz1_qmgr and rx_fdq0_sz1_qnum fields. 
     * If enabled, this value must be greater than the value given in the rx_size_thresh0 field. This field is optional.
     */
    uint16_t          rx_size_thresh1;
    /** This value is left shifted by 5 bits and compared against the packet size to determine which free descriptor 
     * queue should be used for the SOP buffer in the packet.  If the packet size is less than or equal to the value 
     * given in this threshold, the DMA controller in the port will allocate the SOP buffer from the queue given by 
     * the rx_fdq0_sz0_qmgr and rx_fdq0_sz0_qnum fields. This field is optional.
     */
    uint16_t          rx_size_thresh0;
    
    /** Rx flow configuration register G */

    /** This field specifies which Queue should be used for the 1st Rx buffer in a packet whose size is 
     * less than or equal to the rx_size0 value */
    uint16_t          rx_fdq0_sz1_qnum;
    /** This field specifies which Queue Manager should be used for the 1st Rx buffer in a packet whose size 
     * is less than or equal to the rx_size0 value */
    uint16_t          rx_fdq0_sz1_qmgr;
    /** This value is left shifted by 5 bits and compared against the packet size to determine which free descriptor 
     * queue should be used for the SOP buffer in the packet.  If the  packet size is less than or equal to the value
     * given in this threshold, the DMA controller in the port will allocate the SOP buffer from the queue given by the 
     * rx_fdq0_sz2_qmgr and rx_fdq0_sz2_qnum fields.
     * If enabled, this value must be greater than the value given in the rx_size_thresh1 field. This field is optional.
     */
    uint16_t  		rx_size_thresh2;

    /** Rx flow configuration register H */

    /** This field specifies which Free Descriptor Queue should be used for the 1st Rx buffer in a
     * packet whose size is less than or equal to the rx_size3 value */
    uint16_t          rx_fdq0_sz3_qnum;
    /** This field specifies which Free Descriptor Queue Manager should be used for the 1st Rx buffer in a 
     * packet whose size is less than or equal to the rx_size3 value */
    uint16_t          rx_fdq0_sz3_qmgr;
    /** This field specifies which Free Descriptor Queue should be used for the 1st Rx buffer in a packet 
     * whose size is less than or equal to the rx_size2 value */
    uint16_t          rx_fdq0_sz2_qnum;
    /** This field specifies which Free Descriptor Queue Manager should be used for the 1st Rx buffer in a packet 
     * whose size is less than or equal to the rx_size2 value */
    uint16_t          rx_fdq0_sz2_qmgr;
}Cppi_RxFlowCfg;

/** 
 * @brief CPPI RM Handle
 */
typedef void *  Cppi_RmHnd;

/** 
 * @brief CPPI start configuration structure
 */
typedef struct
{
    /** Provide a handle to the Resource Manager instance */
    Cppi_RmHnd rmHandle;
} Cppi_StartCfg;

/** 
 * @brief CPPI init configuration structure
 */
typedef struct
{
    /** dynamic heap configuration parameters */
    Cppi_HeapParams heapParams;
} Cppi_InitCfg;

/** 
 * @brief CPPI return result
 */
typedef int32_t   Cppi_Result;

/** 
 * @brief CPPI handle
 */
typedef uint32_t  *Cppi_Handle;

/** 
 * @brief CPPI channel handle
 */
typedef uint32_t  *Cppi_ChHnd;

/** 
 * @brief CPPI receive flow handle
 */
typedef uint32_t  *Cppi_FlowHnd;

/** 
@} 
*/

/* Exported functions */
extern Cppi_Result Cppi_initCfg (Cppi_GlobalConfigParams *cppiGblCfgParams, Cppi_InitCfg *initCfg);
extern Cppi_Result Cppi_init (Cppi_GlobalConfigParams *cppiGblCfgParams);
extern Cppi_Result Cppi_getHeapReq (Cppi_GlobalConfigParams *cppiGblCfgParams, uint32_t *size);
extern void Cppi_startCfg (Cppi_StartCfg *startCfg);
extern Cppi_Result Cppi_exit (void);
extern Cppi_Handle Cppi_open (Cppi_CpDmaInitCfg *initCfg);
extern Cppi_Result Cppi_close (Cppi_Handle hnd);
extern Cppi_ChHnd Cppi_txChannelOpen (Cppi_Handle hnd, Cppi_TxChInitCfg *cfg, uint8_t *isAllocated);
extern Cppi_ChHnd Cppi_rxChannelOpen (Cppi_Handle hnd, Cppi_RxChInitCfg *cfg, uint8_t *isAllocated);
extern Cppi_Result Cppi_channelEnable (Cppi_ChHnd hnd);
extern Cppi_Result Cppi_channelDisable (Cppi_ChHnd hnd);
extern Cppi_Result Cppi_channelTeardown (Cppi_ChHnd hnd, Cppi_Wait wait);
extern Cppi_Result Cppi_channelClose (Cppi_ChHnd hnd);
extern Cppi_Result Cppi_channelPause (Cppi_ChHnd hnd);
extern Cppi_Result Cppi_channelStatus (Cppi_ChHnd hnd);
extern Cppi_FlowHnd Cppi_configureRxFlow (Cppi_Handle hnd, Cppi_RxFlowCfg *cfg, uint8_t *isAllocated);
extern Cppi_Result Cppi_closeRxFlow (Cppi_FlowHnd hnd);
extern uint32_t Cppi_getChannelNumber (Cppi_ChHnd hnd);
extern uint32_t Cppi_getFlowId (Cppi_FlowHnd hnd);
extern Cppi_Result Cppi_setCpdmaLoopback (Cppi_Handle hnd, uint8_t loopback);
extern Cppi_Result Cppi_getCpdmaLoopback (Cppi_Handle hnd);
extern uint32_t Cppi_getVersion (void);
extern const char* Cppi_getVersionStr (void);

#ifdef __cplusplus
}
#endif

#endif /* CPPI_DRV_H_ */

