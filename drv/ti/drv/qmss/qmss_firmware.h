/**
 *   @file  qmss_firmware.h
 *
 *   @brief   
 *      This is the PDSP firmware include file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2012, Texas Instruments, Inc.
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


#ifndef QMSS_FW_H_
#define QMSS_FW_H_

#ifdef __cplusplus
extern "C" {
#endif

/* PDSP firmware files */
#include <ti/drv/qmss/firmware/acc16_le_bin.h>
#include <ti/drv/qmss/firmware/acc16_be_bin.h>
#include <ti/drv/qmss/firmware/acc32_le_bin.h>
#include <ti/drv/qmss/firmware/acc32_be_bin.h>
#include <ti/drv/qmss/firmware/acc48_le_bin.h>
#include <ti/drv/qmss/firmware/acc48_be_bin.h>
#include <ti/drv/qmss/firmware/qos_le_bin.h>
#include <ti/drv/qmss/firmware/qos_be_bin.h>
#include <ti/drv/qmss/firmware/qos_sched_le_bin.h>
#include <ti/drv/qmss/firmware/qos_sched_be_bin.h>

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/        

/** 
 * @brief PDSP Firmware symbols.
 */
/** @brief 32 channel high priority accumulation little endian firmware */
extern const unsigned int acc32_le[];
/** @brief 32 channel high priority accumulation big endian firmware */
extern const unsigned int acc32_be[];
/** @brief 16 channel low priority accumulation little endian firmware */
extern const unsigned int acc16_le[];
/** @brief 16 channel low priority accumulation big endian firmware */
extern const unsigned int acc16_be[];
/** @brief 48 channel combined high and low priority accumulation little endian firmware */
extern const unsigned int acc48_le[];
/** @brief 48 channel combined high and low priority accumulation big endian firmware */
extern const unsigned int acc48_be[];
/** @brief QoS little endian firmware */
extern const unsigned int qos_le[];
/** @brief QoS big endian firmware */
extern const unsigned int qos_be[];
/** @brief QoS scheduler little endian firmware */
extern const unsigned int qos_sched_le[];
/** @brief QoS scheduler big endian firmware */
extern const unsigned int qos_sched_be[];

/** 
@} 
*/

#ifdef __cplusplus
}
#endif

#endif /* QMSS_FW_H_ */

