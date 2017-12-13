/*
 * drv.h
 *
 *  Created on: 2017Äê11ÔÂ26ÈÕ
 *      Author: wyk
 */

#ifndef DRV_H_
#define DRV_H_

#include <stdio.h>
#include <stdint.h>
//xdc
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/IHeap.h>
//#include <ti/csl/tistdtypes.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h>
#include <ti/csl/csl_chipAux.h>
#include <ti/csl/csl_pscAux.h>
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_pa_ss.h>
#include <ti/csl/csl_srio.h>
#include <ti/csl/csl_srioAux.h>
#include <ti/csl/csl_srioAuxPhyLayer.h>
#include <ti/csl/csl_msmc.h>
#include <ti/csl/csl_msmcAux.h>
#include <ti/csl/csl_tsc.h>
#include <ti/csl/csl_semAux.h>
//drv
/* Firmware images */
/* QMSS LLD include */
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/qmss/qmss_types.h>
#include <ti/drv/pa/pa.h>
#include <ti/drv/srio/srio_drv.h>
//sysbios
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#include <ti/sysbios/family/c66/tci66xx/CpIntc.h>
//#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c66/Cache.h>


#include "resourcemgr.h"
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/tools/console.h>
#include <ti/ndk/inc/tools/servers.h>
#include <ti/transport/ndk/nimu/nimu_eth.h>
#include <ti/transport/ndk/nimu/include/nimu_internal.h>
#include "osal.h"


#define PA_BYPASS

/* w should be power of 2 */
#define PLATFORM_CACHE_LINE_SIZE (128)
#define platform_roundup(n,w) (((n) + (w) - 1) & ~((w) - 1))

typedef struct{
    uint8_t chipnum;
    uint8_t Ga;
    uint8_t version;
} evmc6678Info;

extern evmc6678Info                gEvmc6678Info;
typedef struct edma_config{
    int16_t instNum;
    CSL_Edma3Handle hModule;
    CSL_Edma3ChannelHandle channelHandle;
    CSL_Edma3ParamHandle   hParam;
    CSL_Edma3ParamSetup    myParamSetup;
    uint8_t channelNum;
    uint8_t paramBlockNum;
    uint32_t srcaddr;
    uint32_t dstaddr;
    int16_t aCnt;
    int16_t bCnt;
    int16_t cCnt;
    int16_t srcBidx;
    int16_t dstBidx;
    int16_t srcCidx;
    int16_t dstCidx;

}edma_config_t;
extern edma_config_t edma_config[8];
typedef struct msg{
    uint16_t         id;             /* id */
    uint16_t        val;            /* message value */
} MsgObj, *Msg;

extern void board_delay_cycles(uint64_t cycles);
extern uint32_t l2_global_address (uint32_t addr);

#define MAXNUM 8
#define MAST_CORE 0
#define RIO_RUNING_CORE 0

#undef RIO
#define ETH
#undef IPC

#endif /* 320C66X_LE_BSP_EVMC6678_DRV_H_ */
