/********************************************************************
* Copyright (C) 2012 Texas Instruments Incorporated.
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
#ifndef _QMSS_QOS_SCHED_REGS_H_
#define _QMSS_QOS_SCHED_REGS_H_

#include <ti/drv/qmss/qmss_types.h>

/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for COMMAND_BUFFER
\**************************************************************************/
typedef struct  
{
    volatile uint32_t COMMAND_BUFFER_WORD0;
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND_MASK         (0x000000FFu)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND_SHIFT        (0x00000000u)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND_RESETVAL     (0x00000000u)

#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_OPTION_MASK          (0x0000FF00u)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_OPTION_SHIFT         (0x00000008u)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_OPTION_RESETVAL      (0x00000000u)

#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_INDEX_MASK           (0xFFFF0000u)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_INDEX_SHIFT          (0x00000010u)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_INDEX_RESETVAL       (0x00000000u)

#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_RESETVAL             (0x00000000u)

    volatile uint32_t COMMAND_BUFFER_WORD1;
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE_MASK     (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE_SHIFT    (0x00000000u)
#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE_RESETVAL (0x00000000u)

#define CSL_QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RESETVAL             (0x00000000u)

    volatile uint8_t  RSVD0[8];
} Qmss_QosSchedCommandRegs;

/** QMSS QoS commands */
#define QMSS_QOSSCHED_COMMAND_GET_QUEUE_BASE                        (0x80)
#define QMSS_QOSSCHED_COMMAND_SET_QUEUE_BASE                        (0x81)
#define QMSS_QOSSCHED_COMMAND_TIMER_CONFIG                          (0x82)
#define QMSS_QOSSCHED_COMMAND_ENABLE_PORT                           (0x90)
#define QMSS_QOSSCHED_COMMAND_SHADOW_PORT                           (0x91)
#define QMSS_QOSSCHED_COMMAND_STATS_REQUEST                         (0x92)

/* INDEX field encoding for STATS_REQUEST */
#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_PORT_MASK                   (0x000F0000u)
#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_PORT_SHIFT                  (0x00000010u)
#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_PORT_RESETVAL               (0x00000000u)

#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_GROUP_MASK                  (0x00F00000u)
#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_GROUP_SHIFT                 (0x00000014u)
#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_GROUP_RESETVAL              (0x00000000u)

#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_QUEUE_MASK                  (0xFF000000u)
#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_QUEUE_SHIFT                 (0x00000018u)
#define CSL_QMSS_QOSSCHED_COMMAND_INDEX_QUEUE_RESETVAL              (0x00000000u)

/**************************************************************************\
* Register Overlay Structure for per queue configuration
\**************************************************************************/
typedef struct 
{
    volatile uint32_t WRR_INITIAL_CREDIT;  /* Initial Queue WRR credit on a "new" schedule */
#define CSL_QMSS_QOSSCHED_QUEUE_REGS_WRR_INITIAL_CREDIT_MASK        (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_QUEUE_REGS_WRR_INITIAL_CREDIT_SHIFT       (0x00000000u)
#define CSL_QMSS_QOSSCHED_QUEUE_REGS_WRR_INITIAL_CREDIT_RESETVAL    (0x00000000u)
    volatile uint32_t CONGESTION_THRESH;  /* The max amount of congestion before drop */
#define CSL_QMSS_QOSSCHED_QUEUE_REGS_CONGESTION_THRESH_MASK         (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_QUEUE_REGS_CONGESTION_THRESH_SHIFT        (0x00000000u)
#define CSL_QMSS_QOSSCHED_QUEUE_REGS_CONGESTION_THRESH_RESETVAL     (0x00000000u)
} Qmss_QosSchedQueueRegs;

typedef struct
{
    volatile uint32_t CIR_ITERATION;  /* CIR credit per iteration */
#define CSL_QMSS_QOSSCHED_GROUP_REGS_CIR_ITERATION_MASK             (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_CIR_ITERATION_SHIFT            (0x00000000u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_CIR_ITERATION_RESETVAL         (0x00000000u)

    volatile uint32_t PIR_ITERATION;  /* PIR credit per iteration */
#define CSL_QMSS_QOSSCHED_GROUP_REGS_PIR_ITERATION_MASK             (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_PIR_ITERATION_SHIFT            (0x00000000u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_PIR_ITERATION_RESETVAL         (0x00000000u)

    volatile uint32_t CIR_MAX;        /* CIR max */
#define CSL_QMSS_QOSSCHED_GROUP_REGS_CIR_MAX_MASK                   (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_CIR_MAX_SHIFT                  (0x00000000u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_CIR_MAX_RESETVAL               (0x00000000u)

    volatile uint32_t PIR_MAX;        /* PIR max */
#define CSL_QMSS_QOSSCHED_GROUP_REGS_PIR_MAX_MASK                   (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_PIR_MAX_SHIFT                  (0x00000000u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_PIR_MAX_RESETVAL               (0x00000000u)

    volatile uint32_t WRR_INITIAL_CREDIT; /* Initial WRR credit */
#define CSL_QMSS_QOSSCHED_GROUP_REGS_WRR_INITIAL_CREDIT_MASK        (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_WRR_INITIAL_CREDIT_SHIFT       (0x00000000u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_WRR_INITIAL_CREDIT_RESETVAL    (0x00000000u)

    volatile uint32_t QUEUE_COUNTS;  /* Total, SP, and WRR counts */
#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_TOT_MASK          (0x000000FFu)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_TOT_SHIFT         (0x00000000u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_TOT_RESETVAL      (0x00000000u)

#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_SP_MASK           (0x0000FF00u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_SP_SHIFT          (0x00000008u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_SP_RESETVAL       (0x00000000u)

#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_WRR_MASK          (0x00FF0000u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_WRR_SHIFT         (0x00000010u)
#define CSL_QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_WRR_RESETVAL      (0x00000000u)

    Qmss_QosSchedQueueRegs QUEUE[8];
} Qmss_QosSchedGroupRegs;

typedef struct {
    volatile uint32_t PORT_FLAGS;   /* Control flags */
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_WRR_IS_BYTES_MASK             (0x00000001u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_WRR_IS_BYTES_SHIFT            (0x00000000u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_WRR_IS_BYTES_RESETVAL         (0x00000000u)

#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CIR_IS_BYTES_MASK             (0x00000002u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CIR_IS_BYTES_SHIFT            (0x00000001u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CIR_IS_BYTES_RESETVAL         (0x00000000u)

#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CONG_THRESH_IS_BYTES_MASK     (0x00000004u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CONG_THRESH_IS_BYTES_SHIFT    (0x00000002u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CONG_THRESH_IS_BYTES_RESETVAL (0x00000000u)

#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_NUM_GROUPS_MASK               (0x0000FF00u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_NUM_GROUPS_SHIFT              (0x00000008u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_NUM_GROUPS_RESETVAL           (0x00000000u)

#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_OUT_QNUM_QMGR_MASK            (0xFFFF0000u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_OUT_QNUM_QMGR_SHIFT           (0x00000010u)
#define CSL_QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_OUT_QNUM_QMGR_RESETVAL        (0x00000000u)
    volatile uint32_t CIR_ITERATION_CREDIT;  /* CIR iteration credit */
    volatile uint32_t CIR_MAX;               /* Max CIR */

    /** Configuration for up to 5 groups */
    Qmss_QosSchedGroupRegs GROUPS[5];
} Qmss_QosSchedPortRegs;

typedef struct {
    volatile uint32_t BYTES_FORWARDED_LSW;
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_FORWARDED_LSW_MASK       (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_FORWARDED_LSW_SHIFT      (0x00000000u)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_FORWARDED_LSW_RESETVAL   (0x00000000u)

    volatile uint32_t BYTES_FORWARDED_MSW;
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_FORWARDED_MSW_MASK       (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_FORWARDED_MSW_SHIFT      (0x00000000u)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_FORWARDED_MSW_RESETVAL   (0x00000000u)

    volatile uint32_t BYTES_DISCARDED_LSW;
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_DROPPED_LSW_MASK         (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_DROPPED_LSW_SHIFT        (0x00000000u)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_DROPPED_LSW_RESETVAL     (0x00000000u)

    volatile uint32_t BYTES_DISCARDED_MSW;
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_DROPPED_MSW_MASK         (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_DROPPED_MSW_SHIFT        (0x00000000u)
#define CSL_QMSS_QOSSCHED_STATS_REGS_BYTES_DROPPED_MSW_RESETVAL     (0x00000000u)

    volatile uint32_t PACKETS_FORWARDED;
#define CSL_QMSS_QOSSCHED_STATS_REGS_PACKETS_FORWARDED_MASK         (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_STATS_REGS_PACKETS_FORWARDED_SHIFT        (0x00000000u)
#define CSL_QMSS_QOSSCHED_STATS_REGS_PACKETS_FORWARDED_RESETVAL     (0x00000000u)

    volatile uint32_t PACKETS_DISCARDED;
#define CSL_QMSS_QOSSCHED_STATS_REGS_PACKETS_DROPPED_MASK           (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_STATS_REGS_PACKETS_DROPPED_SHIFT          (0x00000000u)
#define CSL_QMSS_QOSSCHED_STATS_REGS_PACKETS_DROPPED_RESETVAL       (0x00000000u)
} Qmss_QosSchedStatsRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    Qmss_QosSchedCommandRegs COMMAND_BUFFER;  /* offset 0x0000, size = 0x0010 */
    volatile uint8_t         RSVD1[0x0030];   /* offset 0x0010, size = 0x0030 */
    Qmss_QosSchedPortRegs    PORT_SHADOW;     /* offset 0x0040, size = 0x01c4 */
    volatile uint8_t         RSVD2[0x00FC];   /* offset 0x0204, size = 0x00FC */
    Qmss_QosSchedStatsRegs   STATS;           /* offset 0x0300, size = 0x0018 */
    volatile uint8_t         RSVD3[0x1ce0];   /* offset 0x0318, size = 0x1ce0 */
    volatile uint32_t        MAGIC;           /* offset 0x1FF8, size = 0x0004 */
    volatile uint32_t        VERSION;         /* offset 0x1ffC, size = 0x0004 */
#define CSL_QMSS_QOSSCHED_VERSION_MASK                              (0xFFFFFFFFu)
#define CSL_QMSS_QOSSCHED_VERSION_SHIFT                             (0x00000000u)
#define CSL_QMSS_QOSSCHED_VERSION_RESETVAL                          (0x00000000u)
} Qmss_QosSchedRegs;


#endif /* _QMSS_QOS_SCHED_REGS_H_ */
