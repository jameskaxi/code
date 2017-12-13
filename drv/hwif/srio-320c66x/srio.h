/*
 * srio.h
 *
 *  Created on: 2017Äê11ÔÂ26ÈÕ
 *      Author: wyk
 */

#ifndef SRIO_H
#define SRIO_H

#include <stdio.h>
//csl
#include <xdc/std.h>
#include <ti/csl/csl_tsc.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_srio.h>
#include <ti/csl/csl_srioAux.h>
#include <ti/csl/csl_srioAuxPhyLayer.h>
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>
#include <ti/csl/csl_bootcfg.h>
#include <ti/csl/csl_bootcfgAux.h>
/* SRIO Driver Include File. */
#include <ti/drv/srio/srio_drv.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/knl/semaphore.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/IHeap.h>

#include "resourcemgr.h"

/* SRIO Device Information
 * - 16 bit Device Identifier.
 * - 8 bit Device Identifier.
 * - Vendor Identifier.
 * - Device Revision. */
#define DEVICE_VENDOR_ID            0x30
#define DEVICE_REVISION             0x0

/* SRIO Assembly Information
 * - Assembly Identifier
 * - Assembly Vendor Identifier.
 * - Assembly Device Revision.
 * - Assembly Extension Features */
#define DEVICE_ASSEMBLY_ID          0x0
#define DEVICE_ASSEMBLY_VENDOR_ID   0x30
#define DEVICE_ASSEMBLY_REVISION    0x0
#define DEVICE_ASSEMBLY_INFO        0x0100
/**
 *  @ingroup sriolld_hwconfig_constants
 *
 *  @{
 */
/** These are the possible values for SRIO PLL input reference clock in MHZ */
typedef enum
{
  srio_ref_clock_125p00Mhz = 0,    /**< Reference clock of 125.00Mhz for SRIO PLL */
  srio_ref_clock_156p25Mhz,        /**< Reference clock of 156.25Mhz for SRIO PLL */
  srio_ref_clock_250p00Mhz,        /**< Reference clock of 250.00Mhz for SRIO PLL */
  srio_ref_clock_312p50Mhz         /**< Reference clock of 312.50Mhz for SRIO PLL */
} srioRefClockMhz_e;

/** These are the possible values for SRIO lane speed in Gbps */
typedef enum
{
  srio_lane_rate_1p250Gbps = 0,                 /**< Rate of 1.250Gbps for SRIO lanes */
  srio_lane_rate_2p500Gbps,                     /**< Rate of 2.500Gbps for SRIO lanes */
  srio_lane_rate_3p125Gbps,                     /**< Rate of 3.125Gbps for SRIO lanes */
  srio_lane_rate_5p000Gbps                      /**< Rate of 5.000Gbps for SRIO lanes */
} srioLaneRateGbps_e;

/** These are the possible values for SRIO lane mode */
typedef enum
{
  srio_lanes_form_four_1x_ports = 0,             /**< SRIO lanes form four 1x ports */
  srio_lanes_form_one_2x_port_and_two_1x_ports,  /**< SRIO lanes form one 2x port and two 1x ports */
  srio_lanes_form_two_1x_ports_and_one_2x_port,  /**< SRIO lanes form two 1x ports and one 2x port */
  srio_lanes_form_two_2x_ports,                  /**< SRIO lanes form two 2x ports */
  srio_lanes_form_one_4x_port                    /**< SRIO lanes form one 4x port */
} srioLanesMode_e;

#define SRIO_REF_CLOCK                      srio_ref_clock_312p50Mhz
//#define SRIO_LANE_SPEED                       srio_lane_rate_3p125Gbps
#define SRIO_LANE_SPEED                     srio_lane_rate_2p500Gbps
#define SRIO_PORT_WIDTH                     srio_lanes_form_one_4x_port

#define C6678_SRIO_BASE             0x02900000
#define C6678_SRIO_CSR_CAR_OFFSET   0xB000
#define C6678_SRIO_CSR_CAR_BASE     C6678_SRIO_BASE + C6678_SRIO_CSR_CAR_OFFSET
#define DEV_ID              0x00
#define DEV_INFO            0x01
#define ASBLY_ID            0x02
#define PE_FEAT             0x10
#define SWITCH_PORT_INF     0x14
#define SRC_OP              0x18
#define DEST_OP             0x1c
#define BASE_ID             0x60
#define HOST_BASE_ID_LOCK   0x68
#define RTE_DESTID          0x70
#define RTE_PORT            0x74
#define PORT_ERR_STAT       0x158
#define PORT_CTL            0x15c

#define HOST_REGS           0xFFFFFFFF
#define RIO_GEN_DFLT_DID    0x000000FF
#define RIO_BOOT_DFLT_DID   0x0000FFFE
#define RIO_HOST_DFLT_DID   0x00000000

#define DEVICE
#define MAX_DEVICEID        256
#define MAX_SWITCHES        16
#define MAX_SWITCH_PORT     18
#define RIO_SUCCESS         1
#define ERR_SLAVE           0

typedef int32_t            STATUS;

extern Srio_DrvHandle      hSrioDriver;
extern Qmss_QueueHnd       gSrioFreeQHnd;

#endif /* 320C66X_LE_BSP_EVMC6678_SRIO_320C66X_SRIO_H_ */
