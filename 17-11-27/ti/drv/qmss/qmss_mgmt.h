/**
 *   @file  qmss_mgmt.h
 *
 *   @brief   
 *      This is the Queue Manager queue management APIs.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2010, Texas Instruments, Inc.
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


#ifndef QMSS_MGMT_H_
#define QMSS_MGMT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QMSS LLD includes */
#include <ti/drv/qmss/qmss_qm.h>
#include <ti/drv/qmss/include/qmss_pvt.h>
        
/* QMSS OSAL layer */
#include <ti/drv/qmss/qmss_osal.h>


/**********************************************************************
 ************************** Externs *********************************
 **********************************************************************/
        
/* QMSS Local object */
extern Qmss_GlobalConfigParams  qmssLObj;

/** @addtogroup QMSS_LLD_FUNCTION
@{ 
*/

/**
 *  @b Description
 *  @n  
 *      This function pushes a descriptor onto a queue specified by the queue handle. 
 *      The "descSize" is used to specify the size of the descriptor being pushed.
 *      The optional parameter "packetSize" is used specify the size of packet during pop 
 *      operation. 
 *      The optional parameter "location" is used to override the default(tail) and push the packet 
 *      to the head of the queue.
 *
 *      **No validation is done on the input parameters**.
 *
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @param[in]  descAddr
 *      Memory address of the descriptor. Should be a global address.
 * 
 *  @param[in]  packetSize
 *      Size of packet pointed to by the descriptor.
 * 
 *  @param[in]  descSize
 *      Size of the descriptor. Minimum size is 16 bytes. Maximum size is 256 bytes
 * 
 *  @param[in]  location
 *      0 - Tail.
 *      1 - Head
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      None
 */
static inline void Qmss_queuePush (Qmss_QueueHnd hnd, void *descAddr, uint32_t packetSize, uint32_t descSize, Qmss_Location location)
{
    uint32_t            regc = 0, regd = 0;
    uint64_t            dWord = 0;
    volatile uint64_t   *regCregDPtr;
    void                *key;
 

    descAddr = Qmss_osalConvertDescVirtToPhy(descAddr);

    CSL_FINS (regc, QM_QUEUE_MANAGEMENT_QUEUE_REG_C_HEAD_TAIL, location);
    
    CSL_FINS (regc, QM_QUEUE_MANAGEMENT_QUEUE_REG_C_PACKET_SIZE, packetSize);
   
    regd = ((uint32_t) descAddr | ((descSize >> 4) - 1));

#ifdef _BIG_ENDIAN
    dWord = regc;
    dWord = ((dWord<<32)|regd);
#else
#ifdef _LITTLE_ENDIAN
    dWord = regd;
    dWord = ((dWord<<32)|regc);
#else
#error "Define _BIG_ENDIAN for Big Endian or _LITTLE_ENDIAN for Little Endian"
#endif
#endif
    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalMtCsEnter ();

    regCregDPtr = (volatile uint64_t *) (& qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_C);
    *regCregDPtr = dWord;
    
    /* End Critical Section */   
    Qmss_osalMtCsExit (key);
    return;
}

/**
 *  @b Description
 *  @n  
 *      It pushes a descriptor onto a queue specified by the queue handle. Does not allow
 *      specifying optional parameters. The descriptor size is not written to the queue. This 
 *      function should be used to push descriptors that will not be prefetched by the CPDMA.
 *
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @param[in]  descAddr
 *      Memory address of the descriptor. Should be a global address.
 * 
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      None
 */
static inline void Qmss_queuePushDesc (Qmss_QueueHnd hnd, void *descAddr)
{

    descAddr = Qmss_osalConvertDescVirtToPhy(descAddr);

    qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D = (uint32_t) descAddr;
    return;
}

/**
 *  @b Description
 *  @n  
 *      It pushes a descriptor onto a queue specified by the queue handle. Does not allow
 *      specifying optional parameters.
 *
 *      The "descSize" is used to specify the size of the descriptor being pushed. This 
 *      function should be used to push descriptors that will be prefetched by the CPDMA.
 *
 *      **No validation is done on the input parameters**.
 *
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @param[in]  descAddr
 *      Memory address of the descriptor. Should be a global address.
 * 
 *  @param[in]  descSize
 *      Size of the descriptor. Minimum size is 16 bytes. Maximum size is 256 bytes
 * 
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      None
 */
static inline void Qmss_queuePushDescSize (Qmss_QueueHnd hnd, void *descAddr, uint32_t descSize)
{

    descAddr = Qmss_osalConvertDescVirtToPhy(descAddr);

    qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D = ((uint32_t) descAddr | ((descSize >> 4) - 1));
    return;
}

/**
 *  @b Description
 *  @n  
 *      This function pop's a descriptor off the queue specified by the queue handle.
 *
 *      The lower 4 bits of the descriptor address contain the size of the descriptor 
 *      that was specified during the queue push operation.
 *      Caller should mask the lower order 4 bits before using the descriptor.
 *
 *      **No validation is done on the input parameters**
 *      
 *  @param[in]  hnd
 *      Queue handle.
 *
 * @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      Success - Descriptor address and size
 *  @retval
 *      0 -   If queue is empty
 */
static inline void* Qmss_queuePop (Qmss_QueueHnd hnd)
{
    return Qmss_osalConvertDescPhyToVirt((void *) (qmssLObj.qmQueMgmtReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D));
}

/**
 *  @b Description
 *  @n  
 *      This function pop's a descriptor off the queue specified by the queue handle.
 *      It also returns the packet size of the popped decriptor. The packet size is 
 *      available only if it was specified during the push operation. 
 *
 *      **This function is not multicore/multithread safe.**
 *
 *      It is possible that the descriptor is popped by another core/task between the time taken 
 *      to read the packet size and the descriptor address by the first core/task. 
 *      
 *       The caller should provide appropriate locks.
 *
 *      The packet size field is part of the descriptor and should be used to ensure correctness.
 *
 *      The lower 4 bits of the descriptor address contain the size of the descriptor 
 *      that was specified during the queue push operation.
 *      Caller should mask the lower order 4 bits before using the descriptor.
 *
 *      **No validation is done on the input parameters**
 *      
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @param[out]  descAddr
 *      Descriptor address and size.
 *
 *  @param[out]  packetSize
 *      Packet size in bytes.
 *
 * @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      None
 */
static inline void Qmss_queuePopDescSize (Qmss_QueueHnd hnd, void **descAddr, uint32_t *packetSize)
{
    *packetSize = CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_C, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_C_PACKET_SIZE); 
    *descAddr = (void *)Qmss_osalConvertDescPhyToVirt((void *)qmssLObj.qmQueMgmtReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D);
    return;
}

/** 
 *  @b Description
 *  @n  
 *      Given the queue handle the function returns the address of queue management register D. 
 *      register D is used to write the descriptor address during a queue push operation. 
 *
 *  @param[in]  hnd      
 *      Queue handle
 *
 *  @retval
 *      4 byte memory address.
 */
static inline Qmss_QueuePushHnd Qmss_getQueuePushHandle (Qmss_QueueHnd hnd)
{
    return ((Qmss_QueuePushHnd) &(qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D));
}

/**
 *  @b Description
 *  @n  
 *      This function diverts the entire content of source queue to the destination queue. 
 *      "location" indicates whether the contents should be merged on to the head or tail 
 *      of the destination queue.
 * 
 *  @param[in]  srcQnum
 *      Source queue handle.
 *
 *  @param[in]  dstQnum
 *      Destination queue handle.
 * 
 *  @param[in]  location
 *      Head/Tail.
 * 
 *  @pre  
 *      Qmss_queueOpen function for source queue and destination queue should be called before 
 *      calling this function.
 *
 *  @post  
 *      Contents of source queue is moved to the destination queue. Source queue is empty.
 *
 *  @retval
 *      None
 */
static inline void Qmss_queueDivert (Qmss_QueueHnd srcQnum, Qmss_QueueHnd dstQnum, Qmss_Location location)
{
    uint32_t  temp = 0;
    
    CSL_FINS (temp, QM_CONFIG_QUEUE_DIVERSION_REG_SOURCE_QNUM, srcQnum);
    CSL_FINS (temp, QM_CONFIG_QUEUE_DIVERSION_REG_DEST_QNUM, dstQnum);
    CSL_FINS (temp, QM_CONFIG_QUEUE_DIVERSION_REG_HEAD_TAIL, location);

    qmssLObj.qmConfigReg->QUEUE_DIVERSION_REG = temp;
    return; 
}

/**
 *  @b Description
 *  @n  
 *      This function deletes all the contents of the queue.
 * 
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @post  
 *      Queue is empty
 *
 *  @retval
 *      None
 */
static inline void Qmss_queueEmpty (Qmss_QueueHnd hnd)
{
    qmssLObj.qmQueMgmtReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D = (uint32_t) 0x0;
    return;
}

/**
 *  @b Description
 *  @n  
 *      This function returns the number of packets that are currently queued on the queue.
 * 
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      Queue entry count 
 */
static inline uint32_t Qmss_getQueueEntryCount (Qmss_QueueHnd hnd)
{
    return (uint32_t) CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_A, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_A_QUEUE_ENTRY_COUNT); 
}

/**
 *  @b Description
 *  @n  
 *      This function returns the total number of bytes that are contained in all of the 
 *      packets that are currently queued on the queue.
 * 
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      Queue byte count 
 */
static inline uint32_t Qmss_getQueueByteCount (Qmss_QueueHnd hnd)
{
    return (uint32_t) CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_B, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_B_QUEUE_BYTE_COUNT); 
}

/**
 *  @b Description
 *  @n  
 *      This function returns the packet size of the packet queued at the head of the queue.
 * 
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      Queue packet size
 */
static inline uint32_t Qmss_getQueuePacketSize (Qmss_QueueHnd hnd)
{
    return (uint32_t) CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_C, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_C_PACKET_SIZE); 
}

/** 
@} 
*/


#ifdef __cplusplus
}
#endif

#endif /* QMSS_MGMT_H_ */

