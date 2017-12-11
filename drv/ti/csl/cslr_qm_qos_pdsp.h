/********************************************************************
* Copyright (C) 2003-2010 Texas Instruments Incorporated.
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
#ifndef _CSLR_QM_QOS_PDSP_H_
#define _CSLR_QM_QOS_PDSP_H_

/* CSL Modification:
 *  The file has been modified from the AUTOGEN file for the following
 *  reasons:-
 *      a) Modified the header file includes to be RTSC compliant
 */

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for COMMAND_BUFFER
\**************************************************************************/
typedef struct  {
    volatile Uint32 COMMAND_BUFFER_WORD0;
    volatile Uint32 COMMAND_BUFFER_WORD1;
    volatile Uint8 RSVD0[8];
} CSL_Qm_qos_pdspCommand_bufferRegs;

/**************************************************************************\
* Register Overlay Structure for QOS_CLUSTER
\**************************************************************************/
typedef struct  {
    volatile Uint32 QOS_CLUSTER_WORD0;
    volatile Uint32 QOS_CLUSTER_WORD1;
    volatile Uint32 QOS_CLUSTER_WORD2;
    volatile Uint32 QOS_CLUSTER_WORD3;
    volatile Uint32 QOS_CLUSTER_WORD4;
    volatile Uint32 QOS_CLUSTER_WORD5;
    volatile Uint32 QOS_CLUSTER_WORD6;
    volatile Uint32 QOS_CLUSTER_WORD7;
    volatile Uint32 QOS_CLUSTER_WORD8;
    volatile Uint32 QOS_CLUSTER_WORD9;
    volatile Uint32 QOS_CLUSTER_WORD10;
    volatile Uint32 QOS_CLUSTER_WORD11;
    volatile Uint32 QOS_CLUSTER_WORD12;
    volatile Uint32 QOS_CLUSTER_WORD13;
} CSL_Qm_qos_pdspQos_clusterRegs;

/**************************************************************************\
* Register Overlay Structure for QOS_QUEUE
\**************************************************************************/
typedef struct  {
    volatile Uint32 QOS_QUEUE_WORD0;
    volatile Uint32 QOS_QUEUE_WORD1;
    volatile Uint32 QOS_QUEUE_WORD2;
    volatile Uint32 QOS_QUEUE_WORD3;
    volatile Uint32 QOS_QUEUE_WORD4;
    volatile Uint32 QOS_QUEUE_WORD5;
} CSL_Qm_qos_pdspQos_queueRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_Qm_qos_pdspCommand_bufferRegs COMMAND_BUFFER;
    volatile Uint8 RSVD1[48];
    CSL_Qm_qos_pdspQos_clusterRegs QOS_CLUSTER[8];
    CSL_Qm_qos_pdspQos_queueRegs QOS_QUEUE[64];
} CSL_Qm_qos_pdspRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* COMMAND_BUFFER_WORD0 */

#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND_MASK (0x000000FFu)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_OPTION_MASK (0x0000FF00u)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_OPTION_SHIFT (0x00000008u)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_OPTION_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX_MASK (0xFFFF0000u)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD0_RESETVAL (0x00000000u)

/* COMMAND_BUFFER_WORD1 */

#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD0 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD0_GLOBAL_CREDIT_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD0_GLOBAL_CREDIT_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD0_GLOBAL_CREDIT_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD0_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD1 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD1_MAXIMUM_GLOBAL_CREDIT_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD1_MAXIMUM_GLOBAL_CREDIT_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD1_MAXIMUM_GLOBAL_CREDIT_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD1_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD2 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_COUNT_MASK (0x000000FFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_COUNT_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_COUNT_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_0_MASK (0x0000FF00u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_0_SHIFT (0x00000008u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_0_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_1_MASK (0x00FF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_1_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_1_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_2_MASK (0xFF000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_2_SHIFT (0x00000018u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_2_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD2_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD3 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_3_MASK (0x000000FFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_3_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_3_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_4_MASK (0x0000FF00u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_4_SHIFT (0x00000008u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_4_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_5_MASK (0x00FF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_5_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_5_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_6_MASK (0xFF000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_6_SHIFT (0x00000018u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_6_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD3_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD4 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_7_MASK (0x000000FFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_7_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_7_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_8_MASK (0x0000FF00u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_8_SHIFT (0x00000008u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_8_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_REALTIME_FLAGS_MASK (0x01FF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_REALTIME_FLAGS_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_REALTIME_FLAGS_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD4_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD5 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_COUNT_MASK (0x000000FFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_COUNT_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_COUNT_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_0_MASK (0xFFFF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_0_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_0_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD5_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD6 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_1_MASK (0x0000FFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_1_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_1_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_2_MASK (0xFFFF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_2_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_2_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD6_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD7 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_3_MASK (0x0000FFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_3_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_3_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_4_MASK (0xFFFF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_4_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_4_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD7_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD8 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_5_MASK (0x0000FFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_5_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_5_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_6_MASK (0xFFFF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_6_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_6_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD8_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD9 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_7_MASK (0x0000FFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_7_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_7_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_8_MASK (0xFFFF0000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_8_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_8_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD9_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD10 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD10_EGRESS_CONGESTION_THRESHOLD_1_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD10_EGRESS_CONGESTION_THRESHOLD_1_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD10_EGRESS_CONGESTION_THRESHOLD_1_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD10_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD11 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD11_EGRESS_CONGESTION_THRESHOLD_2_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD11_EGRESS_CONGESTION_THRESHOLD_2_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD11_EGRESS_CONGESTION_THRESHOLD_2_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD11_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD12 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD12_EGRESS_CONGESTION_THRESHOLD_3_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD12_EGRESS_CONGESTION_THRESHOLD_3_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD12_EGRESS_CONGESTION_THRESHOLD_3_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD12_RESETVAL (0x00000000u)

/* QOS_CLUSTER_WORD13 */

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD13_EGRESS_CONGESTION_THRESHOLD_4_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD13_EGRESS_CONGESTION_THRESHOLD_4_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD13_EGRESS_CONGESTION_THRESHOLD_4_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_CLUSTER_WORD13_RESETVAL (0x00000000u)

/* QOS_QUEUE_WORD0 */

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD0_EGRESS_QUEUE_MASK (0x0000FFFFu)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD0_EGRESS_QUEUE_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD0_EGRESS_QUEUE_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD0_ITERATION_CREDIT_MASK (0xFFFF0000u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD0_ITERATION_CREDIT_SHIFT (0x00000010u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD0_ITERATION_CREDIT_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD0_RESETVAL (0x00000000u)

/* QOS_QUEUE_WORD1 */

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD1_TOTAL_CREDIT_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD1_TOTAL_CREDIT_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD1_TOTAL_CREDIT_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD1_RESETVAL (0x00000000u)

/* QOS_QUEUE_WORD2 */

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD2_MAXIMUM_CREDIT_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD2_MAXIMUM_CREDIT_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD2_MAXIMUM_CREDIT_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD2_RESETVAL (0x00000000u)

/* QOS_QUEUE_WORD3 */

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD3_CONGESTION_THRESHOLD_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD3_CONGESTION_THRESHOLD_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD3_CONGESTION_THRESHOLD_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD3_RESETVAL (0x00000000u)

/* QOS_QUEUE_WORD4 */

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD4_PACKETS_FORWARDED_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD4_PACKETS_FORWARDED_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD4_PACKETS_FORWARDED_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD4_RESETVAL (0x00000000u)

/* QOS_QUEUE_WORD5 */

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD5_PACKETS_DROPPED_MASK (0xFFFFFFFFu)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD5_PACKETS_DROPPED_SHIFT (0x00000000u)
#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD5_PACKETS_DROPPED_RESETVAL (0x00000000u)

#define CSL_QM_QOS_PDSP_QOS_QUEUE_WORD5_RESETVAL (0x00000000u)

#endif
