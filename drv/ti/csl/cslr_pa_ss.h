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
#ifndef _CSLR_PA_SS_H_
#define _CSLR_PA_SS_H_

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>

/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for MAILBOX
\**************************************************************************/
typedef struct  {
    volatile Uint32 MBOX_SLOT[4];
} CSL_Pa_ssMailboxRegs;

/**************************************************************************\
* Register Overlay Structure for PKT_ID
\**************************************************************************/
typedef struct  {
    volatile Uint32 PKT_ID_REVISION;
    volatile Uint32 PKT_ID_SOFT_RESET;
    volatile Uint32 PKT_ID_RANGE_LIMIT;
    volatile Uint32 PKT_ID_VALUE;
} CSL_Pa_ssPkt_idRegs;

/**************************************************************************\
* Register Overlay Structure for LUT2
\**************************************************************************/
typedef struct  {
    volatile Uint32 LUT2_REVISION;
    volatile Uint32 LUT2_SOFT_RESET;
    volatile Uint8 RSVD0[24];
    volatile Uint32 LUT2_ADD_DATA_N[4];
    volatile Uint32 LUT2_ADD_DELETE_KEY;
    volatile Uint32 LUT2_ADD_DELETE_CONTROL;
} CSL_Pa_ssLut2Regs;

/**************************************************************************\
* Register Overlay Structure for STREAMSWITCH
\**************************************************************************/
typedef struct  {
    volatile Uint32 STREAMSWITCH_REVISION;
    volatile Uint32 STREAMSWITCH_SOFT_RESET;
} CSL_Pa_ssStreamswitchRegs;

/**************************************************************************\
* Register Overlay Structure for PDSP_CTLSTAT
\**************************************************************************/
typedef struct  {
    volatile Uint32 PDSP_CONTROL;
    volatile Uint32 PDSP_STATUS;
    volatile Uint32 PDSP_WAKEUP_ENABLE;
    volatile Uint32 PDSP_CYCLE_COUNT;
    volatile Uint32 PDSP_STALL_COUNT;
    volatile Uint8 RSVD0[12];
    volatile Uint32 PDSP_CONSTANT_TABLE_BLOCK_INDEX_0;
    volatile Uint32 PDSP_CONSTANT_TABLE_BLOCK_INDEX_1;
    volatile Uint32 PDSP_CONSTANT_TABLE_PROG_PTR_0;
    volatile Uint32 PDSP_CONSTANT_TABLE_PROG_PTR_1;
    volatile Uint8 RSVD3[208];
} CSL_Pa_ssPdsp_ctlstatRegs;

/**************************************************************************\
* Register Overlay Structure for PDSP_DEBUG
\**************************************************************************/
typedef struct  {
    volatile Uint32 PDSP_IGP[32];
    volatile Uint32 PDSP_ICTE[32];
} CSL_Pa_ssPdsp_debugRegs;

/**************************************************************************\
* Register Overlay Structure for PDSP_TIMER
\**************************************************************************/
typedef struct  {
    volatile Uint32 TIMER_CNTRL_REG;
    volatile Uint32 TIMER_LOAD_REG;
    volatile Uint32 TIMER_VALUE_REG;
    volatile Uint32 TIMER_IRQ_REG;
    volatile Uint8 RSVD6[240];
} CSL_Pa_ssPdsp_timerRegs;

/**************************************************************************\
* Register Overlay Structure for STATS
\**************************************************************************/
typedef struct  {
    volatile Uint32 STATS_REVISION;
    volatile Uint32 STATS_SOFT_RESET;
    volatile Uint32 STATS_INC_FLAGS;
    volatile Uint32 STATS_CAPTURE;
    volatile Uint8 RSVD0[16];
    volatile Uint32 STATS_COUNT_N[32];
} CSL_Pa_ssStatsRegs;

/**************************************************************************\
* Register Overlay Structure for INTD
\**************************************************************************/
typedef struct  {
    volatile Uint32 INTD_REVISION;
    volatile Uint8 RSVD0[12];
    volatile Uint32 INTD_EOI;
    volatile Uint32 INTD_INTR_VECTOR;
    volatile Uint8 RSVD1[488];
    volatile Uint32 INTD_STATUS0;
    volatile Uint32 INTD_STATUS1;
    volatile Uint32 INTD_STATUS2;
    volatile Uint8 RSVD2[116];
    volatile Uint32 INTD_STATUS_CLR0;
    volatile Uint8 RSVD3[4];
    volatile Uint32 INTD_STATUS_CLR2;
    volatile Uint8 RSVD4[116];
    volatile Uint32 INTD_INTCNT[33];
    volatile Uint8 RSVD5[252];
    volatile Uint32 INTD_INTR_VECTOR_HOST;
} CSL_Pa_ssIntdRegs;

/**************************************************************************\
* Register Overlay Structure for PDSP_IRAM
\**************************************************************************/
typedef struct  {
    volatile Uint32 PDSP_RAM[2048];
    volatile Uint8 RSVD10[24576];
} CSL_Pa_ssPdsp_iramRegs;

/**************************************************************************\
* Register Overlay Structure for PDSP_SRAM
\**************************************************************************/
typedef struct  {
    volatile Uint32 PDSP_RAM[2048];
} CSL_Pa_ssPdsp_sramRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_Pa_ssMailboxRegs MAILBOX[6];
    volatile Uint8 RSVD0[928];
    CSL_Pa_ssPkt_idRegs PKT_ID;
    volatile Uint8 RSVD1[240];
    CSL_Pa_ssLut2Regs LUT2;
    volatile Uint8 RSVD2[200];
    CSL_Pa_ssStreamswitchRegs STREAMSWITCH;
    volatile Uint8 RSVD4[2552];
    CSL_Pa_ssPdsp_ctlstatRegs PDSP_CTLSTAT[6];
    volatile Uint8 RSVD5[2560];
    CSL_Pa_ssPdsp_debugRegs PDSP_DEBUG[6];
    volatile Uint8 RSVD7[2560];
    CSL_Pa_ssPdsp_timerRegs PDSP_TIMER[6];
    volatile Uint8 RSVD8[10752];
    CSL_Pa_ssStatsRegs STATS;
    volatile Uint8 RSVD9[3936];
    CSL_Pa_ssIntdRegs INTD;
    volatile Uint8 RSVD11[35708];
    CSL_Pa_ssPdsp_iramRegs PDSP_IRAM[6];
    CSL_Pa_ssPdsp_sramRegs PDSP_SRAM[4];
} CSL_Pa_ssRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* MBOX_SLOT */

#define CSL_PA_SS_MBOX_SLOT_STATUS_BITS_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_MBOX_SLOT_STATUS_BITS_SHIFT (0x00000000u)
#define CSL_PA_SS_MBOX_SLOT_STATUS_BITS_RESETVAL (0x00000000u)

#define CSL_PA_SS_MBOX_SLOT_RESETVAL     (0x00000000u)

/* PKT_ID_REVISION */

#define CSL_PA_SS_PKT_ID_REVISION_MODULE_ID_MASK (0x3FFF0000u)
#define CSL_PA_SS_PKT_ID_REVISION_MODULE_ID_SHIFT (0x00000010u)
#define CSL_PA_SS_PKT_ID_REVISION_MODULE_ID_RESETVAL (0x00000000u)

#define CSL_PA_SS_PKT_ID_REVISION_REVRTL_MASK (0x0000F800u)
#define CSL_PA_SS_PKT_ID_REVISION_REVRTL_SHIFT (0x0000000Bu)
#define CSL_PA_SS_PKT_ID_REVISION_REVRTL_RESETVAL (0x00000000u)

#define CSL_PA_SS_PKT_ID_REVISION_REVMAJ_MASK (0x00000700u)
#define CSL_PA_SS_PKT_ID_REVISION_REVMAJ_SHIFT (0x00000008u)
#define CSL_PA_SS_PKT_ID_REVISION_REVMAJ_RESETVAL (0x00000001u)

#define CSL_PA_SS_PKT_ID_REVISION_REVMIN_MASK (0x000000FFu)
#define CSL_PA_SS_PKT_ID_REVISION_REVMIN_SHIFT (0x00000000u)
#define CSL_PA_SS_PKT_ID_REVISION_REVMIN_RESETVAL (0x00000000u)

#define CSL_PA_SS_PKT_ID_REVISION_RESETVAL (0x00000100u)

/* PKT_ID_SOFT_RESET */

#define CSL_PA_SS_PKT_ID_SOFT_RESET_TRIGGER_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_PKT_ID_SOFT_RESET_TRIGGER_SHIFT (0x00000000u)
#define CSL_PA_SS_PKT_ID_SOFT_RESET_TRIGGER_RESETVAL (0x00000000u)

#define CSL_PA_SS_PKT_ID_SOFT_RESET_RESETVAL (0x00000000u)

/* PKT_ID_RANGE_LIMIT */

#define CSL_PA_SS_PKT_ID_RANGE_LIMIT_RANGE_LIMIT_MASK (0x000003FFu)
#define CSL_PA_SS_PKT_ID_RANGE_LIMIT_RANGE_LIMIT_SHIFT (0x00000000u)
#define CSL_PA_SS_PKT_ID_RANGE_LIMIT_RANGE_LIMIT_RESETVAL (0x000003FFu)

#define CSL_PA_SS_PKT_ID_RANGE_LIMIT_RESETVAL (0x000003FFu)

/* PKT_ID_VALUE */

#define CSL_PA_SS_PKT_ID_VALUE_ID_VALUE_MASK (0x000007FFu)
#define CSL_PA_SS_PKT_ID_VALUE_ID_VALUE_SHIFT (0x00000000u)
#define CSL_PA_SS_PKT_ID_VALUE_ID_VALUE_RESETVAL (0x00000000u)

#define CSL_PA_SS_PKT_ID_VALUE_RESETVAL  (0x00000000u)

/* LUT2_REVISION */

#define CSL_PA_SS_LUT2_REVISION_MODULE_ID_MASK (0x3FFF0000u)
#define CSL_PA_SS_LUT2_REVISION_MODULE_ID_SHIFT (0x00000010u)
#define CSL_PA_SS_LUT2_REVISION_MODULE_ID_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_REVISION_REVRTL_MASK (0x0000F800u)
#define CSL_PA_SS_LUT2_REVISION_REVRTL_SHIFT (0x0000000Bu)
#define CSL_PA_SS_LUT2_REVISION_REVRTL_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_REVISION_REVMAJ_MASK (0x00000700u)
#define CSL_PA_SS_LUT2_REVISION_REVMAJ_SHIFT (0x00000008u)
#define CSL_PA_SS_LUT2_REVISION_REVMAJ_RESETVAL (0x00000001u)

#define CSL_PA_SS_LUT2_REVISION_REVMIN_MASK (0x000000FFu)
#define CSL_PA_SS_LUT2_REVISION_REVMIN_SHIFT (0x00000000u)
#define CSL_PA_SS_LUT2_REVISION_REVMIN_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_REVISION_RESETVAL (0x00000100u)

/* LUT2_SOFT_RESET */

#define CSL_PA_SS_LUT2_SOFT_RESET_TRIGGER_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_LUT2_SOFT_RESET_TRIGGER_SHIFT (0x00000000u)
#define CSL_PA_SS_LUT2_SOFT_RESET_TRIGGER_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_SOFT_RESET_RESETVAL (0x00000000u)

/* LUT2_ADD_DATA_N */

#define CSL_PA_SS_LUT2_ADD_DATA_N_DATA_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_LUT2_ADD_DATA_N_DATA_SHIFT (0x00000000u)
#define CSL_PA_SS_LUT2_ADD_DATA_N_DATA_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_ADD_DATA_N_RESETVAL (0x00000000u)

/* LUT2_ADD_DELETE_KEY */

#define CSL_PA_SS_LUT2_ADD_DELETE_KEY_KEY_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_LUT2_ADD_DELETE_KEY_KEY_SHIFT (0x00000000u)
#define CSL_PA_SS_LUT2_ADD_DELETE_KEY_KEY_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_ADD_DELETE_KEY_RESETVAL (0x00000000u)

/* LUT2_ADD_DELETE_CONTROL */

#define CSL_PA_SS_LUT2_ADD_DELETE_CONTROL_GO_MASK (0x80000000u)
#define CSL_PA_SS_LUT2_ADD_DELETE_CONTROL_GO_SHIFT (0x0000001Fu)
#define CSL_PA_SS_LUT2_ADD_DELETE_CONTROL_GO_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_ADD_DELETE_CONTROL_ADD_DEL_MASK (0x40000000u)
#define CSL_PA_SS_LUT2_ADD_DELETE_CONTROL_ADD_DEL_SHIFT (0x0000001Eu)
#define CSL_PA_SS_LUT2_ADD_DELETE_CONTROL_ADD_DEL_RESETVAL (0x00000000u)

#define CSL_PA_SS_LUT2_ADD_DELETE_CONTROL_RESETVAL (0x00000000u)

/* STREAMSWITCH_REVISION */

#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MINOR_MASK (0x0000003Fu)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MINOR_SHIFT (0x00000000u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MINOR_RESETVAL (0x00000000u)

#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_CUSTOM_MASK (0x000000C0u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_CUSTOM_SHIFT (0x00000006u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_CUSTOM_RESETVAL (0x00000000u)

#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MAJOR_MASK (0x00000700u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MAJOR_SHIFT (0x00000008u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MAJOR_RESETVAL (0x00000000u)

#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_RTL_MASK (0x0000F800u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_RTL_SHIFT (0x0000000Bu)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_RTL_RESETVAL (0x00000000u)

#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MODULE_MASK (0x0FFF0000u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MODULE_SHIFT (0x00000010u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_MODULE_RESETVAL (0x00000000u)

#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_SCHEME_MASK (0xC0000000u)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_SCHEME_SHIFT (0x0000001Eu)
#define CSL_PA_SS_STREAMSWITCH_REVISION_REV_SCHEME_RESETVAL (0x00000000u)

#define CSL_PA_SS_STREAMSWITCH_REVISION_RESETVAL (0x00000000u)

/* STREAMSWITCH_SOFT_RESET */

#define CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTA_MASK (0x00000007u)
#define CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTA_SHIFT (0x00000000u)
#define CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTA_RESETVAL (0x00000006u)

#define CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTB_MASK (0x00000700u)
#define CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTB_SHIFT (0x00000008u)
#define CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTB_RESETVAL (0x00000006u)

#define CSL_PA_SS_STREAMSWITCH_SOFT_RESET_RESETVAL (0x00000606u)

/* PDSP_CONTROL */

#define CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK (0x00000001u)
#define CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_RESETVAL (0x00000001u)

#define CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK (0x00000002u)
#define CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_SHIFT (0x00000001u)
#define CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONTROL_PDSP_SLEEPING_MASK (0x00000004u)
#define CSL_PA_SS_PDSP_CONTROL_PDSP_SLEEPING_SHIFT (0x00000002u)
#define CSL_PA_SS_PDSP_CONTROL_PDSP_SLEEPING_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONTROL_COUNTER_ENABLE_MASK (0x00000008u)
#define CSL_PA_SS_PDSP_CONTROL_COUNTER_ENABLE_SHIFT (0x00000003u)
#define CSL_PA_SS_PDSP_CONTROL_COUNTER_ENABLE_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONTROL_SINGLE_STEP_MASK (0x00000100u)
#define CSL_PA_SS_PDSP_CONTROL_SINGLE_STEP_SHIFT (0x00000008u)
#define CSL_PA_SS_PDSP_CONTROL_SINGLE_STEP_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONTROL_BIG_ENDIAN_MASK (0x00004000u)
#define CSL_PA_SS_PDSP_CONTROL_BIG_ENDIAN_SHIFT (0x0000000Eu)
#define CSL_PA_SS_PDSP_CONTROL_BIG_ENDIAN_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONTROL_PDSP_STATE_MASK (0x00008000u)
#define CSL_PA_SS_PDSP_CONTROL_PDSP_STATE_SHIFT (0x0000000Fu)
#define CSL_PA_SS_PDSP_CONTROL_PDSP_STATE_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONTROL_PCOUNTER_RST_VAL_MASK (0xFFFF0000u)
#define CSL_PA_SS_PDSP_CONTROL_PCOUNTER_RST_VAL_SHIFT (0x00000010u)
#define CSL_PA_SS_PDSP_CONTROL_PCOUNTER_RST_VAL_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONTROL_RESETVAL  (0x00000001u)

/* PDSP_STATUS */

#define CSL_PA_SS_PDSP_STATUS_PCOUNTER_MASK (0x0000FFFFu)
#define CSL_PA_SS_PDSP_STATUS_PCOUNTER_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_STATUS_PCOUNTER_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_STATUS_RESETVAL   (0x00000000u)

/* PDSP_WAKEUP_ENABLE */

#define CSL_PA_SS_PDSP_WAKEUP_ENABLE_BITWISE_ENABLES_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_PDSP_WAKEUP_ENABLE_BITWISE_ENABLES_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_WAKEUP_ENABLE_BITWISE_ENABLES_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_WAKEUP_ENABLE_RESETVAL (0x00000000u)

/* PDSP_CYCLE_COUNT */

#define CSL_PA_SS_PDSP_CYCLE_COUNT_CYCLECOUNT_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_PDSP_CYCLE_COUNT_CYCLECOUNT_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_CYCLE_COUNT_CYCLECOUNT_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CYCLE_COUNT_RESETVAL (0x00000000u)

/* PDSP_STALL_COUNT */

#define CSL_PA_SS_PDSP_STALL_COUNT_STALLCOUNT_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_PDSP_STALL_COUNT_STALLCOUNT_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_STALL_COUNT_STALLCOUNT_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_STALL_COUNT_RESETVAL (0x00000000u)

/* PDSP_CONSTANT_TABLE_BLOCK_INDEX_0 */

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_0_C24_BLK_INDEX_MASK (0x0000000Fu)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_0_C24_BLK_INDEX_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_0_C24_BLK_INDEX_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_0_C25_BLK_INDEX_MASK (0x000F0000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_0_C25_BLK_INDEX_SHIFT (0x00000010u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_0_C25_BLK_INDEX_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_0_RESETVAL (0x00000000u)

/* PDSP_CONSTANT_TABLE_BLOCK_INDEX_1 */

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_1_C26_BLK_INDEX_MASK (0x0000000Fu)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_1_C26_BLK_INDEX_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_1_C26_BLK_INDEX_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_1_C27_BLK_INDEX_MASK (0x000F0000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_1_C27_BLK_INDEX_SHIFT (0x00000010u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_1_C27_BLK_INDEX_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_BLOCK_INDEX_1_RESETVAL (0x00000000u)

/* PDSP_CONSTANT_TABLE_PROG_PTR_0 */

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_0_C28_POINTER_MASK (0x0000FFFFu)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_0_C28_POINTER_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_0_C28_POINTER_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_0_C29_POINTER_MASK (0xFFFF0000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_0_C29_POINTER_SHIFT (0x00000010u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_0_C29_POINTER_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_0_RESETVAL (0x00000000u)

/* PDSP_CONSTANT_TABLE_PROG_PTR_1 */

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_1_C30_POINTER_MASK (0x0000FFFFu)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_1_C30_POINTER_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_1_C30_POINTER_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_1_C31_POINTER_MASK (0xFFFF0000u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_1_C31_POINTER_SHIFT (0x00000010u)
#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_1_C31_POINTER_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_CONSTANT_TABLE_PROG_PTR_1_RESETVAL (0x00000000u)

/* PDSP_IGP */

#define CSL_PA_SS_PDSP_IGP_REGN_MASK     (0xFFFFFFFFu)
#define CSL_PA_SS_PDSP_IGP_REGN_SHIFT    (0x00000000u)
#define CSL_PA_SS_PDSP_IGP_REGN_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_IGP_RESETVAL      (0x00000000u)

/* PDSP_ICTE */

#define CSL_PA_SS_PDSP_ICTE_CT_ENTRYN_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_PDSP_ICTE_CT_ENTRYN_SHIFT (0x00000000u)
#define CSL_PA_SS_PDSP_ICTE_CT_ENTRYN_RESETVAL (0x00000000u)

#define CSL_PA_SS_PDSP_ICTE_RESETVAL     (0x00000000u)

/* TIMER_CNTRL_REG */

#define CSL_PA_SS_TIMER_CNTRL_REG_GO_MASK (0x00000001u)
#define CSL_PA_SS_TIMER_CNTRL_REG_GO_SHIFT (0x00000000u)
#define CSL_PA_SS_TIMER_CNTRL_REG_GO_RESETVAL (0x00000000u)

#define CSL_PA_SS_TIMER_CNTRL_REG_MODE_MASK (0x00000002u)
#define CSL_PA_SS_TIMER_CNTRL_REG_MODE_SHIFT (0x00000001u)
#define CSL_PA_SS_TIMER_CNTRL_REG_MODE_RESETVAL (0x00000000u)

#define CSL_PA_SS_TIMER_CNTRL_REG_PRESCALE_MASK (0x0000003Cu)
#define CSL_PA_SS_TIMER_CNTRL_REG_PRESCALE_SHIFT (0x00000002u)
#define CSL_PA_SS_TIMER_CNTRL_REG_PRESCALE_RESETVAL (0x00000000u)

#define CSL_PA_SS_TIMER_CNTRL_REG_PSE_MASK (0x00008000u)
#define CSL_PA_SS_TIMER_CNTRL_REG_PSE_SHIFT (0x0000000Fu)
#define CSL_PA_SS_TIMER_CNTRL_REG_PSE_RESETVAL (0x00000000u)

#define CSL_PA_SS_TIMER_CNTRL_REG_RESETVAL (0x00000000u)

/* TIMER_LOAD_REG */

#define CSL_PA_SS_TIMER_LOAD_REG_COUNT_MASK (0x0000FFFFu)
#define CSL_PA_SS_TIMER_LOAD_REG_COUNT_SHIFT (0x00000000u)
#define CSL_PA_SS_TIMER_LOAD_REG_COUNT_RESETVAL (0x0000FFFFu)

#define CSL_PA_SS_TIMER_LOAD_REG_RESETVAL (0x0000FFFFu)

/* TIMER_VALUE_REG */

#define CSL_PA_SS_TIMER_VALUE_REG_CURR_COUNT_MASK (0x0000FFFFu)
#define CSL_PA_SS_TIMER_VALUE_REG_CURR_COUNT_SHIFT (0x00000000u)
#define CSL_PA_SS_TIMER_VALUE_REG_CURR_COUNT_RESETVAL (0x0000FFFFu)

#define CSL_PA_SS_TIMER_VALUE_REG_RESETVAL (0x0000FFFFu)

/* TIMER_IRQ_REG */

#define CSL_PA_SS_TIMER_IRQ_REG_INTERRUPT_MASK (0x00000001u)
#define CSL_PA_SS_TIMER_IRQ_REG_INTERRUPT_SHIFT (0x00000000u)
#define CSL_PA_SS_TIMER_IRQ_REG_INTERRUPT_RESETVAL (0x00000000u)

#define CSL_PA_SS_TIMER_IRQ_REG_RESETVAL (0x00000000u)

/* STATS_REVISION */

#define CSL_PA_SS_STATS_REVISION_REV_SCHEME_MASK (0xC0000000u)
#define CSL_PA_SS_STATS_REVISION_REV_SCHEME_SHIFT (0x0000001Eu)
#define CSL_PA_SS_STATS_REVISION_REV_SCHEME_RESETVAL (0x00000001u)

#define CSL_PA_SS_STATS_REVISION_MODULE_ID_MASK (0x3FFF0000u)
#define CSL_PA_SS_STATS_REVISION_MODULE_ID_SHIFT (0x00000010u)
#define CSL_PA_SS_STATS_REVISION_MODULE_ID_RESETVAL (0x00000E59u)

#define CSL_PA_SS_STATS_REVISION_REVRTL_MASK (0x0000F800u)
#define CSL_PA_SS_STATS_REVISION_REVRTL_SHIFT (0x0000000Bu)
#define CSL_PA_SS_STATS_REVISION_REVRTL_RESETVAL (0x00000000u)

#define CSL_PA_SS_STATS_REVISION_REVMAJ_MASK (0x00000700u)
#define CSL_PA_SS_STATS_REVISION_REVMAJ_SHIFT (0x00000008u)
#define CSL_PA_SS_STATS_REVISION_REVMAJ_RESETVAL (0x00000001u)

#define CSL_PA_SS_STATS_REVISION_REVMIN_MASK (0x000000FFu)
#define CSL_PA_SS_STATS_REVISION_REVMIN_SHIFT (0x00000000u)
#define CSL_PA_SS_STATS_REVISION_REVMIN_RESETVAL (0x00000000u)

#define CSL_PA_SS_STATS_REVISION_RESETVAL (0x4E590100u)

/* STATS_SOFT_RESET */

#define CSL_PA_SS_STATS_SOFT_RESET_TRIGGER_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_STATS_SOFT_RESET_TRIGGER_SHIFT (0x00000000u)
#define CSL_PA_SS_STATS_SOFT_RESET_TRIGGER_RESETVAL (0x00000000u)

#define CSL_PA_SS_STATS_SOFT_RESET_RESETVAL (0x00000000u)

/* STATS_INC_FLAGS */

#define CSL_PA_SS_STATS_INC_FLAGS_INC_FLAGS_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_STATS_INC_FLAGS_INC_FLAGS_SHIFT (0x00000000u)
#define CSL_PA_SS_STATS_INC_FLAGS_INC_FLAGS_RESETVAL (0x00000000u)

#define CSL_PA_SS_STATS_INC_FLAGS_RESETVAL (0x00000000u)

/* STATS_CAPTURE */

#define CSL_PA_SS_STATS_CAPTURE_STAT_CAPTURE_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_STATS_CAPTURE_STAT_CAPTURE_SHIFT (0x00000000u)
#define CSL_PA_SS_STATS_CAPTURE_STAT_CAPTURE_RESETVAL (0x00000000u)

#define CSL_PA_SS_STATS_CAPTURE_RESETVAL (0x00000000u)

/* STATS_COUNT_N */

#define CSL_PA_SS_STATS_COUNT_N_STAT_COUNT_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_STATS_COUNT_N_STAT_COUNT_SHIFT (0x00000000u)
#define CSL_PA_SS_STATS_COUNT_N_STAT_COUNT_RESETVAL (0x00000000u)

#define CSL_PA_SS_STATS_COUNT_N_RESETVAL (0x00000000u)

/* INTD_REVISION */

#define CSL_PA_SS_INTD_REVISION_REV_MINOR_MASK (0x0000003Fu)
#define CSL_PA_SS_INTD_REVISION_REV_MINOR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_REVISION_REV_MINOR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_REVISION_REV_CUSTOM_MASK (0x000000C0u)
#define CSL_PA_SS_INTD_REVISION_REV_CUSTOM_SHIFT (0x00000006u)
#define CSL_PA_SS_INTD_REVISION_REV_CUSTOM_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_REVISION_REV_MAJOR_MASK (0x00000700u)
#define CSL_PA_SS_INTD_REVISION_REV_MAJOR_SHIFT (0x00000008u)
#define CSL_PA_SS_INTD_REVISION_REV_MAJOR_RESETVAL (0x00000001u)

#define CSL_PA_SS_INTD_REVISION_REV_RTL_MASK (0x0000F800u)
#define CSL_PA_SS_INTD_REVISION_REV_RTL_SHIFT (0x0000000Bu)
#define CSL_PA_SS_INTD_REVISION_REV_RTL_RESETVAL (0x00000016u)

#define CSL_PA_SS_INTD_REVISION_REV_MODULE_MASK (0x0FFF0000u)
#define CSL_PA_SS_INTD_REVISION_REV_MODULE_SHIFT (0x00000010u)
#define CSL_PA_SS_INTD_REVISION_REV_MODULE_RESETVAL (0x00000E83u)

#define CSL_PA_SS_INTD_REVISION_REV_SCHEME_MASK (0xC0000000u)
#define CSL_PA_SS_INTD_REVISION_REV_SCHEME_SHIFT (0x0000001Eu)
#define CSL_PA_SS_INTD_REVISION_REV_SCHEME_RESETVAL (0x00000001u)

#define CSL_PA_SS_INTD_REVISION_RESETVAL (0x4E83B100u)

/* INTD_EOI */

#define CSL_PA_SS_INTD_EOI_EOI_VECTOR_MASK (0x000000FFu)
#define CSL_PA_SS_INTD_EOI_EOI_VECTOR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_EOI_EOI_VECTOR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_EOI_RESETVAL      (0x00000000u)

/* INTD_INTR_VECTOR */

#define CSL_PA_SS_INTD_INTR_VECTOR_INTR_VECTOR_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_INTD_INTR_VECTOR_INTR_VECTOR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_INTR_VECTOR_INTR_VECTOR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_INTR_VECTOR_RESETVAL (0x00000000u)

/* INTD_STATUS0 */

#define CSL_PA_SS_INTD_STATUS0_STATUS_HOST_IN_INTR_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_INTD_STATUS0_STATUS_HOST_IN_INTR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_STATUS0_STATUS_HOST_IN_INTR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS0_RESETVAL  (0x00000000u)

/* INTD_STATUS1 */

#define CSL_PA_SS_INTD_STATUS1_STATUS_HOST_LINTR_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_INTD_STATUS1_STATUS_HOST_LINTR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_STATUS1_STATUS_HOST_LINTR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS1_RESETVAL  (0x00000000u)

/* INTD_STATUS2 */

#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CDMA_STARVE_MASK (0x00000001u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CDMA_STARVE_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CDMA_STARVE_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CPSW_STAT0_SYNCD_MASK (0x00000002u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CPSW_STAT0_SYNCD_SHIFT (0x00000001u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CPSW_STAT0_SYNCD_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CPSW_STAT1_SYNCD_MASK (0x00000004u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CPSW_STAT1_SYNCD_SHIFT (0x00000002u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_CPSW_STAT1_SYNCD_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_EVNT_PEND_SYNCD_MASK (0x00000008u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_EVNT_PEND_SYNCD_SHIFT (0x00000003u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_EVNT_PEND_SYNCD_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_STAT_OVER_MASK (0x00000010u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_STAT_OVER_SHIFT (0x00000004u)
#define CSL_PA_SS_INTD_STATUS2_STATUS_HOST_STAT_OVER_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS2_RESETVAL  (0x00000000u)

/* INTD_STATUS_CLR0 */

#define CSL_PA_SS_INTD_STATUS_CLR0_STATUS_HOST_IN_INTR_CLR_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_INTD_STATUS_CLR0_STATUS_HOST_IN_INTR_CLR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_STATUS_CLR0_STATUS_HOST_IN_INTR_CLR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS_CLR0_RESETVAL (0x00000000u)

/* INTD_STATUS_CLR2 */

#define CSL_PA_SS_INTD_STATUS_CLR2_STATUS_HOST_CDMA_STARVE_CLR_MASK (0x00000001u)
#define CSL_PA_SS_INTD_STATUS_CLR2_STATUS_HOST_CDMA_STARVE_CLR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_STATUS_CLR2_STATUS_HOST_CDMA_STARVE_CLR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_STATUS_CLR2_RESETVAL (0x00000000u)

/* INTD_INTCNT */

#define CSL_PA_SS_INTD_INTCNT_INTCNT_HOST_CNT_IN_INTR_MASK (0x000000FFu)
#define CSL_PA_SS_INTD_INTCNT_INTCNT_HOST_CNT_IN_INTR_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_INTCNT_INTCNT_HOST_CNT_IN_INTR_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_INTCNT_RESETVAL   (0x00000000u)

/* INTD_INTR_VECTOR_HOST */

#define CSL_PA_SS_INTD_INTR_VECTOR_HOST_INTR_VECTOR_HOST_MASK (0xFFFFFFFFu)
#define CSL_PA_SS_INTD_INTR_VECTOR_HOST_INTR_VECTOR_HOST_SHIFT (0x00000000u)
#define CSL_PA_SS_INTD_INTR_VECTOR_HOST_INTR_VECTOR_HOST_RESETVAL (0x00000000u)

#define CSL_PA_SS_INTD_INTR_VECTOR_HOST_RESETVAL (0x00000000u)

/* PDSP_RAM */

#define CSL_PA_SS_PDSP_RAM_RAM_MASK      (0xFFFFFFFFu)
#define CSL_PA_SS_PDSP_RAM_RAM_SHIFT     (0x00000000u)
#define CSL_PA_SS_PDSP_RAM_RAM_RESETVAL  (0x00000000u)

#define CSL_PA_SS_PDSP_RAM_RESETVAL      (0x00000000u)

/* PDSP_RAM */

#define CSL_PA_SS_PDSP_RAM_RAM_MASK      (0xFFFFFFFFu)
#define CSL_PA_SS_PDSP_RAM_RAM_SHIFT     (0x00000000u)
#define CSL_PA_SS_PDSP_RAM_RAM_RESETVAL  (0x00000000u)

#define CSL_PA_SS_PDSP_RAM_RESETVAL      (0x00000000u)

#endif
