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
#include <ti/csl/cslr_tpcc.h>
#include <ti/csl/cslr_tptc.h>
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


#undef PA_BYPASS

/* w should be power of 2 */
#define PLATFORM_CACHE_LINE_SIZE (128)
#define platform_roundup(n,w) (((n) + (w) - 1) & ~((w) - 1))

typedef struct{
    uint8_t chipnum;
    uint8_t Ga;
    uint8_t version;
} evmc6678Info;

typedef struct msg{
    uint8_t         id;             /* id */
    uint8_t        val[3];            /* message value */
} msgObj_t;

extern shareVal_t *gGlobalDataPtr;
extern accumList_t *gAccumListPtr;
extern desc_t      *gDescPtr;

extern void board_delay_cycles(uint64_t cycles);
extern uint32_t l2_global_address (uint32_t addr);

#undef SGMII_LOOPBACK
#define MAXNUM 8
#define MAST_CORE 0
#define RIO_RUNING_CORE 0

#undef RIO
#define ETH
#define IPC

#endif /* 320C66X_LE_BSP_EVMC6678_DRV_H_ */
