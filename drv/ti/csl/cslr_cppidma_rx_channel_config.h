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
#ifndef _CSLR_CPPIDMA_RX_CHANNEL_CONFIG_H_
#define _CSLR_CPPIDMA_RX_CHANNEL_CONFIG_H_

/* CSL Modification:
 *  The file has been modified from the AUTOGEN file for the following
 *  reasons:-
 *      a) Modified the header file includes to be RTSC compliant
 */

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>

/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for RX_CHANNEL_GLOBAL_CONFIG
\**************************************************************************/
typedef struct  {
    volatile Uint32 RX_CHANNEL_GLOBAL_CONFIG_REG;
    volatile Uint8 RSVD0[28];
} CSL_Cppidma_rx_channel_configRx_channel_global_configRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_Cppidma_rx_channel_configRx_channel_global_configRegs RX_CHANNEL_GLOBAL_CONFIG[129];
} CSL_Cppidma_rx_channel_configRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* RX_CHANNEL_GLOBAL_CONFIG_REG */

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE_MASK (0x20000000u)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE_SHIFT (0x0000001Du)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE_RESETVAL (0x00000000u)

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN_MASK (0x40000000u)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN_SHIFT (0x0000001Eu)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN_RESETVAL (0x00000000u)

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE_MASK (0x80000000u)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE_SHIFT (0x0000001Fu)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE_RESETVAL (0x00000000u)

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RESETVAL (0x00000000u)

#endif
