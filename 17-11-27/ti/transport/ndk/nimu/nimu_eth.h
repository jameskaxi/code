/*
 * nimu_eth.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
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

/**
 *   @file  nimu.h
 *
 *   @brief
 *      This file defines the interface applications can use when
 *      integrating the Network Interface Module Ethernet driver adaptation layer
 *
 */

#ifndef _NIMU_ETH_H
#define _NIMU_ETH_H  /* #defined if this .h file has been included */
#include <ti/ndk/inc/stkmain.h>

/* NIMU Library Version */
#define NIMU_LIB_VERSION "1.00.00.08"

Ptr Osal_nimuMalloc (uint32_t num_bytes, uint32_t alignment);
void Osal_nimuFree (Ptr dataPtr, uint32_t num_bytes);

/* The EMAC Initialization Function. */
static int EmacInit (STKEVENT_Handle hEvent);
static int EMACInit_Core (STKEVENT_Handle hEvent);

int32_t Init_PASS(void);
int32_t Init_Cpsw(uint32_t mtu, uint8_t * srcmacaddress);
int32_t Verify_Init (void);
void Init_MDIO(void);
void Init_Switch (uint32_t mtu);
int32_t Switch_update_addr (uint32_t portNum, uint8_t macAddress[6], uint16_t add);
void Init_MAC(uint32_t, uint8_t *, uint32_t);
int32_t Setup_Tx(void);
int32_t Setup_Rx(NETIF_DEVICE* ptr_net_device);
int32_t Add_MACAddress (paEthInfo_t ethInfo, uint8_t flowId);
void EmacRxPktISR (NETIF_DEVICE* ptr_net_device );

#endif
