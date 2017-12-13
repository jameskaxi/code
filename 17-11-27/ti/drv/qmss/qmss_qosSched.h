/**
 *   @file  qmss_qosSched.h
 *
 *   @brief   
 *      This is the QMSS QOS packet scheduler header file.
 *      This corresponds to the firmware qos_sched_[be|le]_bin.h
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


#ifndef QMSS_QOSSCHED_H_
#define QMSS_QOSSCHED_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QMSS LLD includes */
#include <ti/drv/qmss/qmss_drv.h>

/**
@addtogroup QMSS_LLD_SYMBOL
@{
*/

/** QMSS QoS PDSP number. QoS PDSP is downloaded to PDSP 1 */
#define QMSS_QOS_SCHED_PDSP_NUM                     1

/** Number of full featured physical ports */
#define QMSS_QOS_SCHED_FULL_MAX_PHYS_PORTS          2

/** Number of reduced feature physical ports */
#define QMSS_QOS_SCHED_LITE_MAX_PHYS_PORTS          10

/** Total number of physical ports */
#define QMSS_QOS_SCHED_MAX_PHYS_PORTS (QMSS_QOS_SCHED_FULL_MAX_PHYS_PORTS +\
	                               QMSS_QOS_SCHED_LITE_MAX_PHYS_PORTS)

/** Number of logical groups in full featured port */
#define QMSS_QOS_SCHED_FULL_MAX_LOG_GROUPS          5

/** Number of logical groups in reduced feature port */
#define QMSS_QOS_SCHED_LITE_MAX_LOG_GROUPS          1

/** Number of queues per group in full featured port */
#define QMSS_QOS_SCHED_FULL_MAX_QUEUES_PER_GROUP    8

/** Number of queues per group in reduced feature port */
#define QMSS_QOS_SCHED_LITE_MAX_QUEUES_PER_GROUP    4

/** Bits representing statistics types */
#define QMSS_QOS_SCHED_STATS_FORWARDED_BYTES        1
#define QMSS_QOS_SCHED_STATS_FORWARDED_PACKETS      2
#define QMSS_QOS_SCHED_STATS_DISCARDED_BYTES        4
#define QMSS_QOS_SCHED_STATS_DISCARDED_PACKETS      8

/** QMSS QoS scheduler return and Error Codes */
/** QMSS QoS scheduler successful return code */
#define QMSS_QOS_SCHED_RETCODE_SUCCESS              1
/** QMSS QoS scheduler invalid command return code */
#define QMSS_QOS_SCHED_INVALID_COMMAND              2
/** QMSS QoS scheduler invalid index return code */
#define QMSS_QOS_SCHED_INVALID_INDEX                3
/** QMSS QoS scheduler invalid option return code */
#define QMSS_QOS_SCHED_INVALID_OPTION               4
/** QMSS QoS scheduler invalid physical port */
#define QMSS_QOS_SCHED_INVALID_PORT                 100
/** QMSS QoS scheduler invalid group in port */
#define QMSS_QOS_SCHED_INVALID_GROUP                101
/** QMSS QoS scheduler invalid queue in group */
#define QMSS_QOS_SCHED_INVALID_QUEUE                102
/** QMSS QoS scheduler invalid port config wrrType */
#define QMSS_QOS_SCHED_INVALID_WRRTYPE              103
/** QMSS QoS scheduler invalid port config cirType */
#define QMSS_QOS_SCHED_INVALID_CIRTYPE              104
/** QMSS QoS scheduler invalid port config congestionType */
#define QMSS_QOS_SCHED_INVALID_CONGTYPE             105
/** QMSS QoS scheduler attempted to configure a running port */
#define QMSS_QOS_SCHED_INVALID_PORT_STATE           106
/** QMSS QoS scheduler attempted to configure a running group */
#define QMSS_QOS_SCHED_INVALID_GROUP_STATE          107
/** QMSS QoS scheduler cirMax too big (adding credit would overflow) */
#define QMSS_QOS_SCHED_INVALID_CIR_MAX              108
/** QMSS QoS scheduler pirMax too big (adding credit would overflow) */
#define QMSS_QOS_SCHED_INVALID_PIR_MAX              109

/** Shift to convert from packets to the format used for credits.
 *  This is used when the type is Qmss_QosSchedAcctType_PACKETS.
 *  20 means that each unit represents 1/1048576 of a packet */
#define QMSS_QOS_SCHED_PACKETS_SCALE_SHIFT          20
/** Shift to convert from bytes to the format used for credits.
 *  This is used when the type is Qmss_QosSchedAcctType_BYTES 
 *  8 means that each unit represents 1/2048 of a byte */
#define QMSS_QOS_SCHED_BYTES_SCALE_SHIFT            11
/*
@}
*/

/**
@addtogroup QMSS_LLD_ENUM
@{
*/

/** 
 * @brief QoS scheduler accounting types
 */
typedef enum
{
    /** credits are accounted in packets with QMSS_QOS_SCHED_PACKETS_SCALE_SHIFT */
    Qmss_QosSchedAcctType_PACKETS,
    /** credits are accounted in bytes with QMSS_QOS_SCHED_BYTES_SCALE_SHIFT */
    Qmss_QosSchedAcctType_BYTES
} Qmss_QosSchedAcctType;

/**
@}
*/

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief QoS scheduler queue configuration structure
 */
typedef struct
{
    /** Initial WRR credit for the queue in its group in
     *  @ref Qmss_QosSchedPortCfg.wrrType units */
    uint32_t    wrrInitialCredit;
    /** Maximum congestion before drop in
     *  @ref Qmss_QosSchedPortCfg.congestionType units.  0
     *  means no congestion checking on this queue */
    uint32_t    congestionThresh;
} Qmss_QosSchedQueueCfg;


/** 
 * @brief QoS scheduler group configuration
 */
typedef struct
{
    /** Committed information rate (CIR) credit per iteration in
     *  @ref Qmss_QosSchedPortCfg.cirType units */
    uint32_t    cirIteration;

    /** Peak information rate credit per iteration in 
     *  @ref Qmss_QosSchedPortCfg.cirType units */
    uint32_t    pirIteration;

    /** Configured limit to accumulated CIR credit in
     *  @ref Qmss_QosSchedPortCfg.cirType units */
    uint32_t    cirMax;

    /** Configured limit to accumulated PIR credit in 
     *  @ref Qmss_QosSchedPortCfg.cirType units */
    uint32_t    pirMax;

    /** Initial WRR credit for the group on its port in
     *  @ref Qmss_QosSchedPortCfg.wrrType units */
    uint32_t    wrrInitialCredit;

    /** Total number of active QoS queues (up to 8).  Any queues beyond
     * @ref spQueueCount + @ref wrrQueueCount are best effort queues */
    uint8_t     totQueueCount;

    /** Number of strict priority (SP) queues - usually 2 - 3  */
    uint8_t     spQueueCount;

    /** Number of weighted round robin (WRR) queues - usually 
     *  @ref totQueueCount - @ref spQueueCount */
    uint8_t     wrrQueueCount;

    /** Configuration for each queue in the group */
    Qmss_QosSchedQueueCfg Queue[QMSS_QOS_SCHED_FULL_MAX_QUEUES_PER_GROUP];
} Qmss_QosSchedGroupCfg;

/** 
 * @brief QoS scheduler port configuration
 */
typedef struct
{
    /** Determines if weighted round robin parameters are specified in bytes 
     *  or packets */
    Qmss_QosSchedAcctType wrrType;

    /** Determines if committed information rate and peak information rate are
     * specified in bytes or packets */
    Qmss_QosSchedAcctType cirType;

    /** Determines if congestion thresholds are specified in bytes or packets */
    Qmss_QosSchedAcctType congestionType;

    /** Committed information rate per iteration in @ref cirType units */
    int32_t cirIteration;

    /** Max committed information rate credit in @ref cirType units */
    int32_t cirMax;

    /** Number of logical groups in @ref group */
    int8_t  groupCount;

    /** Configuration for up to 5 groups */
    Qmss_QosSchedGroupCfg group[QMSS_QOS_SCHED_FULL_MAX_LOG_GROUPS];

    /** Output/egress queue associated with this port */
    Qmss_Queue  outputQueue;
} Qmss_QosSchedPortCfg;

/** 
@} 
*/

/** 
 * @brief QoS scheduler statistics
 */
typedef struct
{
   /** # of bytes forwarded */
   uint64_t bytesForwarded;

   /** # of bytes discarded */
   uint64_t bytesDiscarded;

   /** # of packets forwarded */
   uint32_t packetsForwarded;

   /** # of packets discarded */
   uint32_t packetsDiscarded;
} Qmss_QosSchedStats;

/** 
@} 
*/

/* Exported APIs */
extern Qmss_Result Qmss_setQosSchedQueueBase (uint32_t queueNum);
extern Qmss_Result Qmss_getQosSchedQueueBase (uint32_t *queueNum);
extern Qmss_Result Qmss_configureQosSchedTimer (uint32_t timerConstant);
extern Qmss_Result Qmss_enableQosSchedPort (uint32_t port);
extern Qmss_Result Qmss_disableQosSchedPort (uint32_t port);
extern Qmss_Result Qmss_putCfgQosSchedPort (uint32_t port, Qmss_QosSchedPortCfg *cfg);
extern Qmss_Result Qmss_getCfgQosSchedPort (uint32_t port, Qmss_QosSchedPortCfg *cfg);
extern Qmss_Result Qmss_getQosSchedStats (Qmss_QosSchedStats *stats, uint32_t port, uint32_t group, uint32_t queue, uint32_t which_reset);
extern uint32_t    Qmss_getQosSchedFwVersion (void);

#ifdef __cplusplus
}
#endif

#endif /* QMSS_QOSSCHED_H_ */

