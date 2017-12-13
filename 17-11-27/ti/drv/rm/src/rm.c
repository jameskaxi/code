/**
 *   @file  rm.c
 *
 *   @brief   
 *      This is the Resource Manager Low Level Driver file.
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

/* c99 include */
#include <stdint.h>
#include <stdlib.h>

/* RM includes */
#include <ti/drv/rm/rm.h>
#include <ti/drv/rm/rm_public_lld.h>
#include <ti/drv/rm/resource_table_defs.h>
#include <ti/drv/rm/include/rm_pvt.h>

/* RM OSAL layer */
#include <ti/drv/rm/rm_osal.h>

/* CSL includes */
#include <ti/csl/csl_qm_queue.h>

/**********************************************************************
 ************************** Globals ***********************************
 **********************************************************************/

/* Place QMSS PDSP permissions array */
#pragma DATA_SECTION (rmQmssPdspFirmwarePerms, ".rm");
#pragma DATA_ALIGN (rmQmssPdspFirmwarePerms, 128)
Rm_Perms rmQmssPdspFirmwarePerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_QMSS_FIRMWARE_PDSPS, Rm_Perms)];

/* Place QMSS queue permissions array */
#pragma DATA_SECTION (rmQmssQueuePerms, ".rm");
#pragma DATA_ALIGN (rmQmssQueuePerms, 128)
Rm_Perms rmQmssQueuePerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_QMSS_QUEUES, Rm_Perms)];

/* Place QMSS memory region permissions array */
#pragma DATA_SECTION (rmQmssMemRegionPerms, ".rm");
#pragma DATA_ALIGN (rmQmssMemRegionPerms, 128)
Rm_Perms rmQmssMemRegionPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_QMSS_MEM_REGIONS, Rm_Perms)];

/* Place QMSS Linking RAM permissions array */
#pragma DATA_SECTION (rmQmssLinkingRamPerms, ".rm");
#pragma DATA_ALIGN (rmQmssLinkingRamPerms, 128)
Rm_qmssLinkingRamPerms rmQmssLinkingRamPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_QMSS_LINKING_RAM_RANGES, Rm_qmssLinkingRamPerms)];

/* Place QMSS accumulator channel permissions array */
#pragma DATA_SECTION (rmQmssAccumChPerms, ".rm");
#pragma DATA_ALIGN (rmQmssAccumChPerms, 128)
Rm_Perms rmQmssAccumChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_QMSS_ACCUM_CH, Rm_Perms)];

/* Place QMSS QOS cluster permissions array */
#pragma DATA_SECTION (rmQmssQosClusterPerms, ".rm");
#pragma DATA_ALIGN (rmQmssQosClusterPerms, 128)
Rm_Perms rmQmssQosClusterPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_QMSS_QOS_CLUSTER, Rm_Perms)];

/* Place QMSS QOS queue permissions array */
#pragma DATA_SECTION (rmQmssQosQueuePerms, ".rm");
#pragma DATA_ALIGN (rmQmssQosQueuePerms, 128)
Rm_Perms rmQmssQosQueuePerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_QMSS_QOS_QUEUES, Rm_Perms)];

/* Place CPPI SRIO TX channel permissions array */
#pragma DATA_SECTION (rmCppiSrioTxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiSrioTxChPerms, 128)
Rm_Perms rmCppiSrioTxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_SRIO_TX_CH, Rm_Perms)];

#ifdef QMSS_MAX_AIF_QUEUE
/* Place CPPI AIF TX channel permissions array */
#pragma DATA_SECTION (rmCppiAifTxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiAifTxChPerms, 128)
Rm_Perms rmCppiAifTxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_AIF_TX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_FFTC_A_QUEUE
/* Place CPPI FFTC A TX channel permissions array */
#pragma DATA_SECTION (rmCppiFftcATxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcATxChPerms, 128)
Rm_Perms rmCppiFftcATxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_A_TX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_FFTC_B_QUEUE
/* Place CPPI FFTC B TX channel permissions array */
#pragma DATA_SECTION (rmCppiFftcBTxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcBTxChPerms, 128)
Rm_Perms rmCppiFftcBTxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_B_TX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_PASS_QUEUE
/* Place CPPI PASS TX channel permissions array */
#pragma DATA_SECTION (rmCppiPassTxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiPassTxChPerms, 128)
Rm_Perms rmCppiPassTxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_PASS_TX_CH, Rm_Perms)];
#endif

/* Place CPPI QMSS TX channel permissions array */
#pragma DATA_SECTION (rmCppiQmssTxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiQmssTxChPerms, 128)
Rm_Perms rmCppiQmssTxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_QMSS_TX_CH, Rm_Perms)];

#ifdef QMSS_MAX_FFTC_C_QUEUE
/* Place CPPI FFTC C TX channel permissions array */
#pragma DATA_SECTION (rmCppiFftcCTxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcCTxChPerms, 128)
Rm_Perms rmCppiFftcCTxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_C_TX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_BCP_QUEUE
/* Place CPPI BCP TX channel permissions array */
#pragma DATA_SECTION (rmCppiBcpTxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiBcpTxChPerms, 128)
Rm_Perms rmCppiBcpTxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_BCP_TX_CH, Rm_Perms)];
#endif

/* Place CPPI SRIO RX channel permissions array */
#pragma DATA_SECTION (rmCppiSrioRxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiSrioRxChPerms, 128)
Rm_Perms rmCppiSrioRxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_SRIO_RX_CH, Rm_Perms)];

#ifdef QMSS_MAX_AIF_QUEUE
/* Place CPPI AIF RX channel permissions array */
#pragma DATA_SECTION (rmCppiAifRxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiAifRxChPerms, 128)
Rm_Perms rmCppiAifRxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_AIF_RX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_FFTC_A_QUEUE
/* Place CPPI FFTC A RX channel permissions array */
#pragma DATA_SECTION (rmCppiFftcARxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcARxChPerms, 128)
Rm_Perms rmCppiFftcARxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_A_RX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_FFTC_B_QUEUE
/* Place CPPI FFTC B RX channel permissions array */
#pragma DATA_SECTION (rmCppiFftcBRxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcBRxChPerms, 128)
Rm_Perms rmCppiFftcBRxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_B_RX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_PASS_QUEUE
/* Place CPPI PASS RX channel permissions array */
#pragma DATA_SECTION (rmCppiPassRxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiPassRxChPerms, 128)
Rm_Perms rmCppiPassRxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_PASS_RX_CH, Rm_Perms)];
#endif

/* Place CPPI QMSS RX channel permissions array */
#pragma DATA_SECTION (rmCppiQmssRxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiQmssRxChPerms, 128)
Rm_Perms rmCppiQmssRxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_QMSS_RX_CH, Rm_Perms)];

#ifdef QMSS_MAX_FFTC_C_QUEUE
/* Place CPPI FFTC C RX channel permissions array */
#pragma DATA_SECTION (rmCppiFftcCRxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcCRxChPerms, 128)
Rm_Perms rmCppiFftcCRxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_C_RX_CH, Rm_Perms)];
#endif

#ifdef QMSS_MAX_BCP_QUEUE
/* Place CPPI BCP RX channel permissions array */
#pragma DATA_SECTION (rmCppiBcpRxChPerms, ".rm");
#pragma DATA_ALIGN (rmCppiBcpRxChPerms, 128)
Rm_Perms rmCppiBcpRxChPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_BCP_RX_CH, Rm_Perms)];
#endif

/* Place CPPI SRIO flow permissions array */
#pragma DATA_SECTION (rmCppiSrioFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiSrioFlowPerms, 128)
Rm_Perms rmCppiSrioFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_SRIO_FLOW, Rm_Perms)];

#ifdef QMSS_MAX_AIF_QUEUE
/* Place CPPI AIF flow permissions array */
#pragma DATA_SECTION (rmCppiAifFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiAifFlowPerms, 128)
Rm_Perms rmCppiAifFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_AIF_FLOW, Rm_Perms)];
#endif

#ifdef QMSS_MAX_FFTC_A_QUEUE
/* Place CPPI FFTC A flow permissions array */
#pragma DATA_SECTION (rmCppiFftcAFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcAFlowPerms, 128)
Rm_Perms rmCppiFftcAFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_A_FLOW, Rm_Perms)];
#endif

#ifdef QMSS_MAX_FFTC_B_QUEUE
/* Place CPPI FFTC B flow permissions array */
#pragma DATA_SECTION (rmCppiFftcBFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcBFlowPerms, 128)
Rm_Perms rmCppiFftcBFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_B_FLOW, Rm_Perms)];
#endif

#ifdef QMSS_MAX_PASS_QUEUE
/* Place CPPI PASS flow permissions array */
#pragma DATA_SECTION (rmCppiPassFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiPassFlowPerms, 128)
Rm_Perms rmCppiPassFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_PASS_FLOW, Rm_Perms)];
#endif

/* Place CPPI QMSS flow permissions array */
#pragma DATA_SECTION (rmCppiQmssFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiQmssFlowPerms, 128)
Rm_Perms rmCppiQmssFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_QMSS_FLOW, Rm_Perms)];

#ifdef QMSS_MAX_FFTC_C_QUEUE
/* Place CPPI FFTC C flow permissions array */
#pragma DATA_SECTION (rmCppiFftcCFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiFftcCFlowPerms, 128)
Rm_Perms rmCppiFftcCFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_FFTC_C_FLOW, Rm_Perms)];
#endif

#ifdef QMSS_MAX_BCP_QUEUE
/* Place CPPI BCP flow permissions array */
#pragma DATA_SECTION (rmCppiBcpFlowPerms, ".rm");
#pragma DATA_ALIGN (rmCppiBcpFlowPerms, 128)
Rm_Perms rmCppiBcpFlowPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_CPPI_BCP_FLOW, Rm_Perms)];
#endif

/* Place PA lookup table permissions array */
#pragma DATA_SECTION (rmPaLutPerms, ".rm");
#pragma DATA_ALIGN (rmPaLutPerms, 128)
Rm_Perms rmPaLutPerms[RM_ALIGN_PERMISSIONS_ARRAY(RM_PA_LUT, Rm_Perms)];

/* Rm_init/Rm_start synchronization object.  Initialized to 0. */
#pragma DATA_SECTION (rmGSyncObj, ".rm");
#pragma DATA_ALIGN (rmGSyncObj, 128)
Rm_Sync_Obj rmGSyncObj = 
{
    {
        RM_PERMISSION_TABLE_NOT_VALID,
    }
};

/* Create, populate, and place RM global permissions object */
#pragma DATA_SECTION (rmGPermsObj, ".rm");
#pragma DATA_ALIGN (rmGPermsObj, 128)
Rm_GlobalPermissionsObj rmGPermsObj = 
{
    {
        /* qmssPdspFirmwarePerms */
        &rmQmssPdspFirmwarePerms[0],  
        /* Pointer: qmssQueuePerms */
        &rmQmssQueuePerms[0],
        /* Pointer: qmssMemRegionPerms */
        &rmQmssMemRegionPerms[0],
        /* qmssLinkRamControlPerms */
        {
            0u,
            0u,
        },  
        /* Pointer: qmssLinkRamPerms */
        &rmQmssLinkingRamPerms[0],
        /* Pointer: qmssAccumChPerms */
        &rmQmssAccumChPerms[0], 
        /* qmssQosPdspTimerPerms */
        {
            0u,
            0u,
        },  
        /* Pointer: qmssQosClusterPerms */
        &rmQmssQosClusterPerms[0],
        /* Pointer: qmssQosQueuePerms */
        &rmQmssQosQueuePerms[0],
        /* Pointer array: cppiTxChPerms - Must be in same order as DMA objects */
        {
            { &rmCppiSrioTxChPerms[0], 
#ifdef QMSS_MAX_AIF_QUEUE          
               &rmCppiAifTxChPerms[0],
#endif           
#ifdef QMSS_MAX_FFTC_A_QUEUE           
               &rmCppiFftcATxChPerms[0],
#endif           
#ifdef QMSS_MAX_FFTC_B_QUEUE           
               &rmCppiFftcBTxChPerms[0],   
#endif
#ifdef QMSS_MAX_PASS_QUEUE
               &rmCppiPassTxChPerms[0],
#endif               
               &rmCppiQmssTxChPerms[0],
#ifdef QMSS_MAX_FFTC_C_QUEUE
               &rmCppiFftcCTxChPerms[0],
#endif
#ifdef QMSS_MAX_BCP_QUEUE
               &rmCppiBcpTxChPerms[0]
#endif           
            }
        },
        /* Pointer array: cppiRxChPerms - Must be in same order as DMA objects  */
        {
            { &rmCppiSrioRxChPerms[0], 
#ifdef QMSS_MAX_AIF_QUEUE          
               &rmCppiAifRxChPerms[0], 
#endif           
#ifdef QMSS_MAX_FFTC_A_QUEUE           
               &rmCppiFftcARxChPerms[0],
#endif           
#ifdef QMSS_MAX_FFTC_B_QUEUE           
               &rmCppiFftcBRxChPerms[0], 
#endif           
#ifdef QMSS_MAX_PASS_QUEUE
               &rmCppiPassRxChPerms[0], 
#endif               
               &rmCppiQmssRxChPerms[0],
#ifdef QMSS_MAX_FFTC_C_QUEUE
               &rmCppiFftcCRxChPerms[0],
#endif              
#ifdef QMSS_MAX_BCP_QUEUE
               &rmCppiBcpRxChPerms[0]
#endif           
            }
        },    
        /* Pointer array: cppiFlowPerms - Must be in same order as DMA objects  */
        {
            { &rmCppiSrioFlowPerms[0], 
#ifdef QMSS_MAX_AIF_QUEUE          
               &rmCppiAifFlowPerms[0], 
#endif           
#ifdef QMSS_MAX_FFTC_A_QUEUE           
               &rmCppiFftcAFlowPerms[0],
#endif           
#ifdef QMSS_MAX_FFTC_B_QUEUE           
               &rmCppiFftcBFlowPerms[0],    
#endif           
#ifdef QMSS_MAX_PASS_QUEUE
               &rmCppiPassFlowPerms[0], 
#endif               
               &rmCppiQmssFlowPerms[0],
#ifdef QMSS_MAX_FFTC_C_QUEUE
               &rmCppiFftcCFlowPerms[0],
#endif                     
#ifdef QMSS_MAX_BCP_QUEUE
               &rmCppiBcpFlowPerms[0]
#endif           
            }
        },
        /* paFirmwarePerms */
        {
            0u,
            0u,
        },  
        /* Pointer: paLutPerms */
        &rmPaLutPerms[0],
    }
};

/** @brief Global Variable (should be local per DSP) containing LLD RM permission checkers */
Rm_LldPermCallouts rmPermissionCheckers = 
{
    Rm_initPermissionChecker,
    Rm_usePermissionChecker,
};

/** @brief Global Variable which describes the RM LLD Version Information */
const char   rmLldVersionStr[] = RM_LLD_VERSION_STR ":" __DATE__  ":" __TIME__;

/**********************************************************************
 ********************** Internal Functions *********************************
 **********************************************************************/

/**
 *  @b Description
 *  @n  
 *      Initialize the permission tables.  All resources are intialized to deny all initialization
 *      and usage permissions.
 *
 */
void Rm_permissionTableInit(void)
{
    uint16_t resourceIndex;
    Rm_Perms *resArrayPtr;
    uint16_t dmaNum;
    uint16_t dmaTxCh[RM_CPPI_MAX_DMAS] = {RM_CPPI_SRIO_TX_CH, 
#ifdef QMSS_MAX_AIF_QUEUE      
                                          RM_CPPI_AIF_TX_CH,
#endif                                                                       
#ifdef QMSS_MAX_FFTC_A_QUEUE                                                                       
                                          RM_CPPI_FFTC_A_TX_CH, 
#endif                                                                       
#ifdef QMSS_MAX_FFTC_B_QUEUE                                                                       
                                          RM_CPPI_FFTC_B_TX_CH, 
#endif    
#ifdef QMSS_MAX_PASS_QUEUE
                                          RM_CPPI_PASS_TX_CH, 
#endif
                                          RM_CPPI_QMSS_TX_CH,
#ifdef QMSS_MAX_FFTC_C_QUEUE
                                          RM_CPPI_FFTC_C_TX_CH,
#endif
#ifdef QMSS_MAX_BCP_QUEUE
                                          RM_CPPI_BCP_TX_CH
#endif                                                                       
                                         };
    uint16_t dmaRxCh[RM_CPPI_MAX_DMAS] = {RM_CPPI_SRIO_RX_CH,
#ifdef QMSS_MAX_AIF_QUEUE      
                                          RM_CPPI_AIF_RX_CH,
#endif                                                                       
#ifdef QMSS_MAX_FFTC_A_QUEUE                                                                       
                                          RM_CPPI_FFTC_A_RX_CH, 
#endif                                                                       
#ifdef QMSS_MAX_FFTC_B_QUEUE                                                                       
                                          RM_CPPI_FFTC_B_RX_CH, 
#endif                                                               
#ifdef QMSS_MAX_PASS_QUEUE
                                          RM_CPPI_PASS_RX_CH, 
#endif                                          
                                          RM_CPPI_QMSS_RX_CH,
#ifdef QMSS_MAX_FFTC_C_QUEUE
                                          RM_CPPI_FFTC_C_RX_CH,
#endif
#ifdef QMSS_MAX_BCP_QUEUE
                                          RM_CPPI_BCP_RX_CH
#endif       
                                         };
    uint16_t dmaFlow[RM_CPPI_MAX_DMAS] = {RM_CPPI_SRIO_FLOW, 
#ifdef QMSS_MAX_AIF_QUEUE      
                                          RM_CPPI_AIF_FLOW,
#endif                                                                       
#ifdef QMSS_MAX_FFTC_A_QUEUE                                                                       
                                          RM_CPPI_FFTC_A_FLOW, 
#endif                                                                       
#ifdef QMSS_MAX_FFTC_B_QUEUE                                                                       
                                          RM_CPPI_FFTC_B_FLOW, 
#endif                                                         
#ifdef QMSS_MAX_PASS_QUEUE
                                          RM_CPPI_PASS_FLOW, 
#endif                                          
                                          RM_CPPI_QMSS_FLOW,
#ifdef QMSS_MAX_FFTC_C_QUEUE
                                          RM_CPPI_FFTC_C_FLOW,
#endif
#ifdef QMSS_MAX_BCP_QUEUE
                                          RM_CPPI_BCP_FLOW
#endif       
                                         };

    /* QMSS Linking RAM Control */
    rmGPermsObj.obj.qmssLinkRamControlPerms.initPerms = 0;
    rmGPermsObj.obj.qmssLinkRamControlPerms.usePerms = 0;

    /* QMSS QOS PDSP Timer */
    rmGPermsObj.obj.qmssQosPdspTimerPerms.initPerms = 0;
    rmGPermsObj.obj.qmssQosPdspTimerPerms.usePerms = 0;

    /* PA Firmware */
    rmGPermsObj.obj.paFirmwarePerms.initPerms = 0;
    rmGPermsObj.obj.paFirmwarePerms.usePerms = 0;

    /* Writeback the values that were initialized in the global object itself */
    Rm_osalEndMemAccess ((void *) &rmGPermsObj, sizeof (Rm_GlobalPermissionsObj));

    /* QMSS PDSP Firmware */
    for (resourceIndex = 0; resourceIndex < RM_QMSS_FIRMWARE_PDSPS; resourceIndex++)
    {
        rmGPermsObj.obj.qmssPdspFirmwarePerms[resourceIndex].initPerms = 0;
        rmGPermsObj.obj.qmssPdspFirmwarePerms[resourceIndex].usePerms = 0;
    }
    /* Writeback the qmssPdspFirmwarePerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.qmssPdspFirmwarePerms, sizeof (Rm_Perms)*RM_QMSS_FIRMWARE_PDSPS);

    /* QMSS queues */
    for (resourceIndex = 0; resourceIndex < RM_QMSS_QUEUES; resourceIndex++)
    {
        rmGPermsObj.obj.qmssQueuePerms[resourceIndex].initPerms = 0;
        rmGPermsObj.obj.qmssQueuePerms[resourceIndex].usePerms = 0;
    }
    /* Writeback the qmssQueuePerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.qmssQueuePerms, sizeof (Rm_Perms)*RM_QMSS_QUEUES);

    /* QMSS memory regions */
    for (resourceIndex = 0; resourceIndex < RM_QMSS_MEM_REGIONS; resourceIndex++)
    {
        rmGPermsObj.obj.qmssMemRegionPerms[resourceIndex].initPerms = 0;
        rmGPermsObj.obj.qmssMemRegionPerms[resourceIndex].usePerms = 0;
    }
    /* Writeback the qmssMemRegionPerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.qmssMemRegionPerms, sizeof (Rm_Perms)*RM_QMSS_MEM_REGIONS);

    /* QMSS Linking RAM */
    for (resourceIndex = 0; resourceIndex < RM_QMSS_LINKING_RAM_RANGES; resourceIndex++)
    {
        rmGPermsObj.obj.qmssLinkRamPerms[resourceIndex].startIndex = RM_QMSS_LINKING_RAM_RANGE_INIT;
        rmGPermsObj.obj.qmssLinkRamPerms[resourceIndex].endIndex = RM_QMSS_LINKING_RAM_RANGE_INIT;
        rmGPermsObj.obj.qmssLinkRamPerms[resourceIndex].rangePerms.initPerms = 0;
        rmGPermsObj.obj.qmssLinkRamPerms[resourceIndex].rangePerms.usePerms = 0;
    }
    /* Writeback the qmssLinkRamPerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.qmssLinkRamPerms, sizeof (Rm_Perms)*RM_QMSS_LINKING_RAM_RANGES);

    /* QMSS accumulator channels */
    for (resourceIndex = 0; resourceIndex < RM_QMSS_ACCUM_CH; resourceIndex++)
    {
        rmGPermsObj.obj.qmssAccumChPerms[resourceIndex].initPerms = 0;
        rmGPermsObj.obj.qmssAccumChPerms[resourceIndex].usePerms = 0;
    }
    /* Writeback the qmssAccumChPerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.qmssAccumChPerms, sizeof (Rm_Perms)*RM_QMSS_ACCUM_CH);

    /* QMSS QOS Clusters */
    for (resourceIndex = 0; resourceIndex < RM_QMSS_QOS_CLUSTER; resourceIndex++)
    {
        rmGPermsObj.obj.qmssQosClusterPerms[resourceIndex].initPerms = 0;
        rmGPermsObj.obj.qmssQosClusterPerms[resourceIndex].usePerms = 0;
    }
    /* Writeback the qmssQosClusterPerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.qmssQosClusterPerms, sizeof (Rm_Perms)*RM_QMSS_QOS_CLUSTER);    

    /* QMSS QOS Queues */
    for (resourceIndex = 0; resourceIndex < RM_QMSS_QOS_QUEUES; resourceIndex++)
    {
        rmGPermsObj.obj.qmssQosQueuePerms[resourceIndex].initPerms = 0;
        rmGPermsObj.obj.qmssQosQueuePerms[resourceIndex].usePerms = 0;
    }
    /* Writeback the qmssQosQueuePerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.qmssQosQueuePerms, sizeof (Rm_Perms)*RM_QMSS_QOS_QUEUES);      

    /* CPPI DMA transmit channels */
    for (dmaNum = 0; dmaNum < RM_CPPI_MAX_DMAS; dmaNum++)
    {
        resArrayPtr = rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[dmaNum];
        
        for (resourceIndex = 0; resourceIndex < dmaTxCh[dmaNum]; resourceIndex++)
        {
            resArrayPtr[resourceIndex].initPerms = 0;
            resArrayPtr[resourceIndex].usePerms = 0;
        }

        /* Writeback each of the transmit channel arrays  */
        Rm_osalEndMemAccess ((void *) resArrayPtr, sizeof (Rm_Perms)*dmaTxCh[dmaNum]);
    }

    /* CPPI DMA receive channels */
    for (dmaNum = 0; dmaNum < RM_CPPI_MAX_DMAS; dmaNum++)
    {
        resArrayPtr = rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[dmaNum];
        
        for (resourceIndex = 0; resourceIndex < dmaRxCh[dmaNum]; resourceIndex++)
        {
            resArrayPtr[resourceIndex].initPerms = 0;
            resArrayPtr[resourceIndex].usePerms = 0;
        }

        /* Writeback each of the receive channel arrays */
        Rm_osalEndMemAccess ((void *) resArrayPtr, sizeof (Rm_Perms)*dmaRxCh[dmaNum]);
    }    

    /* CPPI DMA flows */
    for (dmaNum = 0; dmaNum < RM_CPPI_MAX_DMAS; dmaNum++)
    {
        resArrayPtr = rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[dmaNum];
        
        for (resourceIndex = 0; resourceIndex < dmaFlow[dmaNum]; resourceIndex++)
        {
            resArrayPtr[resourceIndex].initPerms = 0;
            resArrayPtr[resourceIndex].usePerms = 0;
        }

        /* Writeback each of the flow arrays */
        Rm_osalEndMemAccess ((void *) resArrayPtr, sizeof (Rm_Perms)*dmaFlow[dmaNum]);
    }       

    /* PA Lookup tables */
    for (resourceIndex = 0; resourceIndex < RM_PA_LUT; resourceIndex++)
    {
        rmGPermsObj.obj.paLutPerms[resourceIndex].initPerms = 0;
        rmGPermsObj.obj.paLutPerms[resourceIndex].usePerms = 0;
    }
    /* Writeback the paLutPerms array */
    Rm_osalEndMemAccess ((void *) rmGPermsObj.obj.paLutPerms, sizeof (Rm_Perms)*RM_PA_LUT);
    
}

/**
 *  @b Description
 *  @n  
 *      Sets a list of entries in a permissions array to the specified permissions
 *
 *  @param[in]  resourceEntry
 *      The resource entry from the application defined resource table containing
 *      a range of resources and the permissions to assign to them.
 *
 *  @param[in]  rmPermsArray
 *      The permissions array for the resource specified in the resourceEntry.
 *     
 *  @param[in]  len
 *      Full length of permissions array for writeback after the permissions have been
 *      transferred.
 */
void Rm_setTablePermissions (const Rm_Resource *resourceEntry, Rm_Perms *rmPermsArray, uint32_t len)
{
    uint32_t index;

    /* Scan through the resource range filling in the specified permission */
    for (index = resourceEntry->resourceStart; index < resourceEntry->resourceEnd + 1; index++)
    {
        rmPermsArray[index].initPerms = resourceEntry->resourceInitFlags;
        rmPermsArray[index].usePerms  = resourceEntry->resourceUseFlags;
    }

    Rm_osalEndMemAccess ((void *)rmPermsArray, sizeof (Rm_Perms)* len);
}

/**
 *  @b Description
 *  @n  
 *      Takes an application specified resource table as input and transfers all
 *      resource permissions specified within into the internal resource
 *      permission tables.  Upon completion of permission transfer a global
 *      synchronization object is written to sync with slave cores.
 *
 *  @param[in]  rmResourceTable
 *      Application defined resource table containing all resources that should
 *      have permissions set for the DSPs
 *
 *  @retval
 *      Success -   RM_OK
 *  @retval
 *      Failure -   RM_ERROR_PERMISSION_TABLE_POPULATION_FAILED
 */
Rm_Result Rm_populatePermissionTable(const Rm_Resource *rmResourceTable)
{
    const Rm_Resource *resourceEntry;
    uint16_t linkRamIndex;

    /* Verify resource table can be read by verifying magic number is contained
     * in first entry of the resource table */
    resourceEntry = rmResourceTable;
    
    /* Invalidate the resource */
    Rm_osalBeginMemAccess ((void *) resourceEntry, sizeof (Rm_Resource));
    if (resourceEntry->resourceId != RM_RESOURCE_MAGIC_NUMBER)
    {
        return RM_ERROR_PERMISSION_TABLE_POPULATION_FAILED;
    }

    /* Magic number is visible start parsing the resource table and transferring
      * permissions to the internal permissions tables */

    /* Parse resource table until last entry field is encountered */
    while (resourceEntry->resourceId != RM_RESOURCE_FINAL_ENTRY)
    {
        /* Invalidate the resource */
        Rm_osalBeginMemAccess ((void *) resourceEntry, sizeof (Rm_Resource));

        /* Populate a permission table based on the resourceId */
        switch (resourceEntry->resourceId)
        {
            case RM_RESOURCE_MAGIC_NUMBER:
                break;     
                
            case RM_RESOURCE_QMSS_FIRMWARE_PDSP:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_QMSS_FIRMWARE_PDSPS, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.qmssPdspFirmwarePerms, RM_QMSS_FIRMWARE_PDSPS);
            	  break;

            case RM_RESOURCE_QMSS_QUEUE:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_QMSS_QUEUES, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.qmssQueuePerms, RM_QMSS_QUEUES);
            	  break;

            case RM_RESOURCE_QMSS_MEMORY_REGION:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_QMSS_MEM_REGIONS, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.qmssMemRegionPerms, RM_QMSS_MEM_REGIONS);
            	  break;
                
            case RM_RESOURCE_QMSS_LINKING_RAM_CONTROL:
                rmGPermsObj.obj.qmssLinkRamControlPerms.initPerms = resourceEntry->resourceInitFlags;
                rmGPermsObj.obj.qmssLinkRamControlPerms.usePerms  = resourceEntry->resourceUseFlags;
            	  break;
                
            case RM_RESOURCE_QMSS_LINKING_RAM:
                /* Expect Linking RAM ranges to be presented in order */

                /* Find next available unused Linking RAM permissions entry */
                for (linkRamIndex = 0; linkRamIndex < RM_QMSS_LINKING_RAM_RANGES; linkRamIndex++)
                {
                    if ((rmGPermsObj.obj.qmssLinkRamPerms[linkRamIndex].startIndex == RM_QMSS_LINKING_RAM_RANGE_INIT) &&
                         (rmGPermsObj.obj.qmssLinkRamPerms[linkRamIndex].endIndex == RM_QMSS_LINKING_RAM_RANGE_INIT))
                    {
                        RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_QMSS_LINKING_RAM_RANGE_INIT, 
                                        RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                        /* Populate the entry with the Linking RAM resource data */
                        rmGPermsObj.obj.qmssLinkRamPerms[linkRamIndex].startIndex = resourceEntry->resourceStart;
                        rmGPermsObj.obj.qmssLinkRamPerms[linkRamIndex].endIndex = resourceEntry->resourceEnd;
                        rmGPermsObj.obj.qmssLinkRamPerms[linkRamIndex].rangePerms.initPerms = resourceEntry->resourceInitFlags;
                        rmGPermsObj.obj.qmssLinkRamPerms[linkRamIndex].rangePerms.usePerms  = resourceEntry->resourceUseFlags;

                        Rm_osalEndMemAccess ((void *)&rmGPermsObj.obj.qmssLinkRamPerms[0], sizeof (Rm_Perms)* RM_QMSS_LINKING_RAM_RANGES);
                        
                        /* Leave search loop */
                        break;
                    }
                }
            	  break;

            case RM_RESOURCE_QMSS_ACCUMULATOR_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_QMSS_ACCUM_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.qmssAccumChPerms, RM_QMSS_ACCUM_CH);
            	  break;

            case RM_RESOURCE_QMSS_QOS_PDSP_TIMER:
                rmGPermsObj.obj.qmssQosPdspTimerPerms.initPerms = resourceEntry->resourceInitFlags;
                rmGPermsObj.obj.qmssQosPdspTimerPerms.usePerms  = resourceEntry->resourceUseFlags;
            	  break;

            case RM_RESOURCE_QMSS_QOS_CLUSTER:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_QMSS_QOS_CLUSTER, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.qmssQosClusterPerms, RM_QMSS_QOS_CLUSTER);
            	  break;             
                
            case RM_RESOURCE_QMSS_QOS_QUEUE:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_QMSS_QOS_QUEUES, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.qmssQosQueuePerms, RM_QMSS_QOS_QUEUES);
            	  break;                    

            case RM_RESOURCE_CPPI_SRIO_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_SRIO_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_SRIO_DMA_ID], RM_CPPI_SRIO_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_SRIO_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_SRIO_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_SRIO_DMA_ID], RM_CPPI_SRIO_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_SRIO_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_SRIO_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_SRIO_DMA_ID], RM_CPPI_SRIO_FLOW);
            	  break;

#ifdef QMSS_MAX_AIF_QUEUE
            case RM_RESOURCE_CPPI_AIF_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_AIF_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_AIF_DMA_ID], RM_CPPI_AIF_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_AIF_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_AIF_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_AIF_DMA_ID], RM_CPPI_AIF_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_AIF_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_AIF_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_AIF_DMA_ID], RM_CPPI_AIF_FLOW);
            	  break;            
#endif

#ifdef QMSS_MAX_FFTC_A_QUEUE
            case RM_RESOURCE_CPPI_FFTC_A_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_A_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_FFTC_A_DMA_ID], RM_CPPI_FFTC_A_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_FFTC_A_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_A_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_FFTC_A_DMA_ID], RM_CPPI_FFTC_A_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_FFTC_A_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_A_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_FFTC_A_DMA_ID], RM_CPPI_FFTC_A_FLOW);
            	  break;     
#endif    

#ifdef QMSS_MAX_FFTC_B_QUEUE
            case RM_RESOURCE_CPPI_FFTC_B_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_B_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_FFTC_B_DMA_ID], RM_CPPI_FFTC_B_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_FFTC_B_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_B_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_FFTC_B_DMA_ID], RM_CPPI_FFTC_B_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_FFTC_B_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_B_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_FFTC_B_DMA_ID], RM_CPPI_FFTC_B_FLOW);
            	  break;       
#endif                

#ifdef QMSS_MAX_PASS_QUEUE
            case RM_RESOURCE_CPPI_PASS_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_PASS_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_PASS_DMA_ID], RM_CPPI_PASS_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_PASS_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_PASS_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_PASS_DMA_ID], RM_CPPI_PASS_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_PASS_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_PASS_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_PASS_DMA_ID], RM_CPPI_PASS_FLOW);
            	  break;
#endif                

            case RM_RESOURCE_CPPI_QMSS_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_QMSS_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_QMSS_DMA_ID], RM_CPPI_QMSS_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_QMSS_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_QMSS_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_QMSS_DMA_ID], RM_CPPI_QMSS_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_QMSS_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_QMSS_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_QMSS_DMA_ID], RM_CPPI_QMSS_FLOW);
            	  break;

#ifdef QMSS_MAX_FFTC_C_QUEUE
            case RM_RESOURCE_CPPI_FFTC_C_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_C_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_FFTC_C_DMA_ID], RM_CPPI_FFTC_C_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_FFTC_C_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_C_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_FFTC_C_DMA_ID], RM_CPPI_FFTC_C_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_FFTC_C_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_FFTC_C_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_FFTC_C_DMA_ID], RM_CPPI_FFTC_C_FLOW);
            	  break;          
#endif

#ifdef QMSS_MAX_BCP_QUEUE
            case RM_RESOURCE_CPPI_BCP_TX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_BCP_TX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[RM_CPPI_BCP_DMA_ID], RM_CPPI_BCP_TX_CH);
            	  break;

            case RM_RESOURCE_CPPI_BCP_RX_CH:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_BCP_RX_CH, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[RM_CPPI_BCP_DMA_ID], RM_CPPI_BCP_RX_CH);
            	  break;

            case RM_RESOURCE_CPPI_BCP_FLOW:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_CPPI_BCP_FLOW, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[RM_CPPI_BCP_DMA_ID], RM_CPPI_BCP_FLOW);
            	  break;      
#endif                

            case RM_RESOURCE_PA_FIRMWARE:
                rmGPermsObj.obj.paFirmwarePerms.initPerms = resourceEntry->resourceInitFlags;
                rmGPermsObj.obj.paFirmwarePerms.usePerms  = resourceEntry->resourceUseFlags;
            	  break;

            case RM_RESOURCE_PA_LUT_ENTRY:
                RM_RANGE_CHECK (resourceEntry->resourceStart, resourceEntry->resourceEnd, RM_PA_LUT, RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE);
                Rm_setTablePermissions (resourceEntry, rmGPermsObj.obj.paLutPerms, RM_PA_LUT);
            	  break;

            default:
                return RM_ERROR_PERMISSION_TABLE_POPULATION_FAILED;

        }

        resourceEntry++;
    };

    /* Write synchronization object so that slave cores know permissions table is
      * populated and valid */
    rmGSyncObj.obj.globalSyncObj = RM_PERMISSION_TABLE_VALID;
    
    /* Writeback Sync Object */
    Rm_osalEndMemAccess ((void *) &rmGSyncObj, sizeof (Rm_Sync_Obj));

    return RM_OK;
}

/**
 *  @b Description
 *  @n  
 *      This function is called on slave DSPs after the master DSP has populated
 *      the internal permission tables.  This function invalidates all internal
 *      global permission tables so that no further invalidates are required
 *      when LLDs perform resource permission checks
 *
 */
void Rm_updatePermissionTables(void)
{
    uint16_t dmaIndex;
    const uint16_t dmaTxCh[RM_CPPI_MAX_DMAS] = {RM_CPPI_SRIO_TX_CH, 
#ifdef QMSS_MAX_AIF_QUEUE      
                                                RM_CPPI_AIF_TX_CH,
#endif                                                                       
#ifdef QMSS_MAX_FFTC_A_QUEUE                                                                       
                                                RM_CPPI_FFTC_A_TX_CH, 
#endif                                                                       
#ifdef QMSS_MAX_FFTC_B_QUEUE                                                                       
                                                RM_CPPI_FFTC_B_TX_CH, 
#endif                                          
#ifdef QMSS_MAX_PASS_QUEUE
                                                RM_CPPI_PASS_TX_CH, 
#endif                                                
                                                RM_CPPI_QMSS_TX_CH,
#ifdef QMSS_MAX_FFTC_C_QUEUE
                                                RM_CPPI_FFTC_C_TX_CH,
#endif
#ifdef QMSS_MAX_BCP_QUEUE
                                                RM_CPPI_BCP_TX_CH
#endif                                                                       
                                               };
    const uint16_t dmaRxCh[RM_CPPI_MAX_DMAS] = {RM_CPPI_SRIO_RX_CH, 
#ifdef QMSS_MAX_AIF_QUEUE      
                                                RM_CPPI_AIF_RX_CH,
#endif                                                                       
#ifdef QMSS_MAX_FFTC_A_QUEUE                                                                       
                                                RM_CPPI_FFTC_A_RX_CH, 
#endif                                                                       
#ifdef QMSS_MAX_FFTC_B_QUEUE                                                                       
                                                RM_CPPI_FFTC_B_RX_CH, 
#endif                                                       
#ifdef QMSS_MAX_PASS_QUEUE
                                                RM_CPPI_PASS_RX_CH, 
#endif                                                
                                                RM_CPPI_QMSS_RX_CH,
#ifdef QMSS_MAX_FFTC_C_QUEUE
                                                RM_CPPI_FFTC_C_RX_CH,
#endif
#ifdef QMSS_MAX_BCP_QUEUE
                                                RM_CPPI_BCP_RX_CH
#endif                                                                       
                                               };
    const uint16_t dmaFlow[RM_CPPI_MAX_DMAS] = {RM_CPPI_SRIO_FLOW, 
#ifdef QMSS_MAX_AIF_QUEUE      
                                                RM_CPPI_AIF_FLOW,
#endif                                                                       
#ifdef QMSS_MAX_FFTC_A_QUEUE                                                                       
                                                RM_CPPI_FFTC_A_FLOW, 
#endif                                                                       
#ifdef QMSS_MAX_FFTC_B_QUEUE                                                                       
                                                RM_CPPI_FFTC_B_FLOW, 
#endif                                                                
#ifdef QMSS_MAX_PASS_QUEUE
                                                RM_CPPI_PASS_FLOW, 
#endif                                                
                                                RM_CPPI_QMSS_FLOW,
#ifdef QMSS_MAX_FFTC_C_QUEUE
                                                RM_CPPI_FFTC_C_FLOW,
#endif
#ifdef QMSS_MAX_BCP_QUEUE
                                                RM_CPPI_BCP_FLOW
#endif                                                                       
                                               }; 
    
    /* Invalidate all permission tables so no further invalidates are required
      * on slave cores */

    /* Global permissions object */
    Rm_osalBeginMemAccess ((void *) &rmGPermsObj, sizeof (Rm_GlobalPermissionsObj));
    
    /* QMSS Permission Tables */

    /* QMSS PDSPs */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.qmssPdspFirmwarePerms, sizeof (Rm_Perms) * RM_QMSS_FIRMWARE_PDSPS);
    /* QMSS Queues */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.qmssQueuePerms, sizeof (Rm_Perms) * RM_QMSS_QUEUES);
    /* QMSS Memory Regions */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.qmssMemRegionPerms, sizeof (Rm_Perms) * RM_QMSS_MEM_REGIONS);    
    /* QMSS Linking RAM */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.qmssLinkRamPerms, sizeof (Rm_qmssLinkingRamPerms) * RM_QMSS_LINKING_RAM_RANGES);
    /* QMSS Accumulator Channels */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.qmssAccumChPerms, sizeof (Rm_Perms) * RM_QMSS_ACCUM_CH);   
    /* QMSS QOS Clusters */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.qmssQosClusterPerms, sizeof (Rm_Perms) * RM_QMSS_QOS_CLUSTER);   
    /* QMSS QOS Queues */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.qmssQosQueuePerms, sizeof (Rm_Perms) * RM_QMSS_QOS_QUEUES);       

    /* CPPI Permission Tables */
    
    /* CPPI DMA Transmit Channels */
    for (dmaIndex = 0; dmaIndex < RM_CPPI_MAX_DMAS; dmaIndex++)
    {
        Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[dmaIndex], sizeof (Rm_Perms)*dmaTxCh[dmaIndex]); 
    }
    /* CPPI DMA Receive Channels */
    for (dmaIndex = 0; dmaIndex < RM_CPPI_MAX_DMAS; dmaIndex++)
    {
        Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[dmaIndex], sizeof (Rm_Perms)*dmaRxCh[dmaIndex]); 
    }
    /* CPPI DMA Receive Flows */
    for (dmaIndex = 0; dmaIndex < RM_CPPI_MAX_DMAS; dmaIndex++)
    {
        Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[dmaIndex], sizeof (Rm_Perms)*dmaFlow[dmaIndex]); 
    }

    /* PA permission tables */

    /* PA Firmware invalidated as part of global permissions object invalidate */
    /* PA LUTs */
    Rm_osalBeginMemAccess ((void *)rmGPermsObj.obj.paLutPerms, sizeof (Rm_Perms) * RM_PA_LUT);   
}

/**
 *  @b Description
 *  @n  
 *      This function extracts the initialization permission for a DSP from a resource
 *      permission element.
 *
 *  @param[in]  resourcePermissions
 *      A permissions structure element to extract the per DSP initialization permission
 *
 *  @retval
 *      Success -   RM_INIT_PERMISSION_APPROVED
 *  @retval
 *      Failure -   RM_INIT_PERMISSION_DENIED
 */
Rm_Result Rm_getInitPermissions (Rm_Perms *resourcePermissions)
{
    /* Check the init permissions for the calling DSP */
    if (!(RM_GET_PERMISSIONS(resourcePermissions->initPerms)))
    {
        return RM_INIT_PERMISSION_DENIED;
    }
  
    return RM_INIT_PERMISSION_APPROVED;
}

/**
 *  @b Description
 *  @n  
 *      This function extracts the usage permission for a DSP from a resource
 *      permission element.
 *
 *  @param[in]  resourcePermissions
 *      A permissions structure element to extract the per DSP usage permission
 *
 *  @retval
 *      Success -   RM_INIT_PERMISSION_APPROVED
 *  @retval
 *      Failure -   RM_INIT_PERMISSION_DENIED
 */
Rm_Result Rm_getUsePermissions (Rm_Perms *resourcePermissions)
{
    /* Check the use permissions for the calling DSP */
    if (!(RM_GET_PERMISSIONS(resourcePermissions->usePerms)))
    {
        return RM_USE_PERMISSION_DENIED;
    }
  
    return RM_USE_PERMISSION_APPROVED;
}

/**
 *  @b Description
 *  @n  
 *      This function searches the list of linking RAM address ranges to find one that has
 *      the requested linking RAM address within it.  If found the function returns the permissions
 *      for this range.  Otherwise it returns denied.
 *
 *  @param[in]  isInitCheck
 *      True - Permissions check is for initialization
 *      False - Permissions check is for usage
 *
 *  @param[in]  linkRamPermArray
 *      Internal array of linking RAM ranges and their permissions
 *
 *  @param[in]  linkRamResData
 *      Linking RAM addresses to check for initialization or usage permissions
 *
 *  @retval
 *      Success -   RM_INIT_PERMISSION_APPROVED
 *  @retval
 *      Failure -   RM_INIT_PERMISSION_DENIED
 */
Rm_Result Rm_getLinkRamPermissions (Bool isInitCheck, Rm_qmssLinkingRamPerms *linkRamPermArray, Rm_ResourceInfo *linkRamResData)
{
    Rm_Result retVal;
    uint16_t linkRamIndex;

    /* Initialize the return value based on type of check boolean */
    if (isInitCheck)
    {
        retVal = RM_INIT_PERMISSION_DENIED;
    }
    else
    {
        retVal = RM_USE_PERMISSION_DENIED;
    }
    
    for (linkRamIndex = 0; linkRamIndex < RM_QMSS_LINKING_RAM_RANGES; linkRamIndex++)
    {
        if ((linkRamResData->res_info.linkRamData.linkRamStartIndex >= linkRamPermArray[linkRamIndex].startIndex) &&
             (linkRamResData->res_info.linkRamData.linkRamEndIndex <= linkRamPermArray[linkRamIndex].endIndex))
        {
            /* Check the use permissions for the calling DSP */
            if (isInitCheck)
            {
                if (RM_GET_PERMISSIONS(linkRamPermArray[linkRamIndex].rangePerms.initPerms))
                {
                    retVal = RM_USE_PERMISSION_APPROVED;
                }
            }
            else
            {
                if (RM_GET_PERMISSIONS(linkRamPermArray[linkRamIndex].rangePerms.usePerms))
                {
                    retVal = RM_USE_PERMISSION_APPROVED;
                }
            }
            break;
        }
    }

    return (retVal);
}

/**********************************************************************
 **********APIs visible to other LLDs internally via call table *******************
 **********************************************************************/

/**
 *  @b Description
 *  @n  
 *      This function is used by LLDs to check initialization permissions for a resource
 *
 *  @param[in]  resourceData
 *      Structure containing resource information such as resource type and the
 *      resource value to be checked
 *
 *  @retval
 *      Success -   RM_INIT_PERMISSION_APPROVED
 *  @retval
 *      Failure -   RM_INIT_PERMISSION_DENIED
 */
Rm_Result Rm_initPermissionChecker (Rm_ResourceInfo *resourceData)
{
    switch (resourceData->resourceType)
    {
        case Rm_resource_QMSS_FIRMWARE_PDSP:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssPdspFirmwarePerms[resourceData->res_info.pdspNum]));
            
        case Rm_resource_QMSS_QUEUE:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssQueuePerms[resourceData->res_info.queNum]));
            
        case Rm_resource_QMSS_MEMORY_REGION:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssMemRegionPerms[resourceData->res_info.memRegion]));

        case Rm_resource_QMSS_LINKING_RAM_CONTROL:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssLinkRamControlPerms));   
            
        case Rm_resource_QMSS_LINKING_RAM:
            return (Rm_getLinkRamPermissions (TRUE, &rmGPermsObj.obj.qmssLinkRamPerms[0], resourceData));
            
        case Rm_resource_QMSS_ACCUM_CH:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssAccumChPerms[resourceData->res_info.accumCh]));  

        case Rm_resource_QMSS_QOS_TIMER:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssQosPdspTimerPerms));   

        case Rm_resource_QMSS_QOS_CLUSTER:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssQosClusterPerms[resourceData->res_info.qosCluster]));           

        case Rm_resource_QMSS_QOS_QUEUE:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.qmssQosQueuePerms[resourceData->res_info.qosQueue]));                
            
        case Rm_resource_CPPI_TX_CH:
            {
                Rm_Perms *txChPermsArray = rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[resourceData->res_info.cpDmaData.dmaNum];
                return (Rm_getInitPermissions(&txChPermsArray[resourceData->res_info.cpDmaData.cppiChNumOrFlowId]));
            }
            
        case Rm_resource_CPPI_RX_CH:
            {
                Rm_Perms *rxChPermsArray = rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[resourceData->res_info.cpDmaData.dmaNum];
                return (Rm_getInitPermissions(&rxChPermsArray[resourceData->res_info.cpDmaData.cppiChNumOrFlowId]));
            }
            
        case Rm_resource_CPPI_RX_FLOW:
            {
                Rm_Perms *flowPermsArray = rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[resourceData->res_info.cpDmaData.dmaNum];
                return (Rm_getInitPermissions(&flowPermsArray[resourceData->res_info.cpDmaData.cppiChNumOrFlowId]));       
            }
            
        case Rm_resource_PA_FIRMWARE:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.paFirmwarePerms));    
            
        case Rm_resource_PA_LUT:
            return (Rm_getInitPermissions(&rmGPermsObj.obj.paLutPerms[resourceData->res_info.lutEntry]));   
     
        default:
            return (RM_INIT_PERMISSION_DENIED);
    }
}

/**
 *  @b Description
 *  @n  
 *      This function is used by LLDs to check usage permissions for a resource
 *
 *  @param[in]  resourceData
 *      Structure containing resource information such as resource type and the
 *      resource value to be checked
 *
 *  @retval
 *      Success -   RM_INIT_PERMISSION_APPROVED
 *  @retval
 *      Failure -   RM_INIT_PERMISSION_DENIED
 */
Rm_Result Rm_usePermissionChecker (Rm_ResourceInfo *resourceData)
{
    switch (resourceData->resourceType)
    {
        case Rm_resource_QMSS_FIRMWARE_PDSP:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssPdspFirmwarePerms[resourceData->res_info.pdspNum]));
            
        case Rm_resource_QMSS_QUEUE:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssQueuePerms[resourceData->res_info.queNum]));
            
        case Rm_resource_QMSS_MEMORY_REGION:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssMemRegionPerms[resourceData->res_info.memRegion]));

        case Rm_resource_QMSS_LINKING_RAM_CONTROL:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssLinkRamControlPerms));   
            
        case Rm_resource_QMSS_LINKING_RAM:
            return (Rm_getLinkRamPermissions(FALSE, &rmGPermsObj.obj.qmssLinkRamPerms[0], resourceData));
            
        case Rm_resource_QMSS_ACCUM_CH:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssAccumChPerms[resourceData->res_info.accumCh]));     

        case Rm_resource_QMSS_QOS_TIMER:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssQosPdspTimerPerms));   

        case Rm_resource_QMSS_QOS_CLUSTER:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssQosClusterPerms[resourceData->res_info.qosCluster]));           

        case Rm_resource_QMSS_QOS_QUEUE:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.qmssQosQueuePerms[resourceData->res_info.qosQueue]));                
            
        case Rm_resource_CPPI_TX_CH:
            {
                Rm_Perms *txChPermsArray = rmGPermsObj.obj.cppiTxChPerms.dmaPermPtrs[resourceData->res_info.cpDmaData.dmaNum];
                return (Rm_getUsePermissions(&txChPermsArray[resourceData->res_info.cpDmaData.cppiChNumOrFlowId]));
            }
            
        case Rm_resource_CPPI_RX_CH:
            {
                Rm_Perms *rxChPermsArray = rmGPermsObj.obj.cppiRxChPerms.dmaPermPtrs[resourceData->res_info.cpDmaData.dmaNum];
                return (Rm_getUsePermissions(&rxChPermsArray[resourceData->res_info.cpDmaData.cppiChNumOrFlowId]));
            }
            
        case Rm_resource_CPPI_RX_FLOW:
            {
                Rm_Perms *flowPermsArray = rmGPermsObj.obj.cppiFlowPerms.dmaPermPtrs[resourceData->res_info.cpDmaData.dmaNum];
                return (Rm_getUsePermissions(&flowPermsArray[resourceData->res_info.cpDmaData.cppiChNumOrFlowId]));       
            }
            
        case Rm_resource_PA_FIRMWARE:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.paFirmwarePerms));    
            
        case Rm_resource_PA_LUT:
            return (Rm_getUsePermissions(&rmGPermsObj.obj.paLutPerms[resourceData->res_info.lutEntry]));  
     
        default:
            return (RM_USE_PERMISSION_DENIED);
    }
}

/*  */

/**********************************************************************
 *********************** Application visible APIs ***************************
 **********************************************************************/

/** @addtogroup RM_LLD_FUNCTION
@{ 
*/

/**
 *  @b Description
 *  @n  
 *      This function initializes the Resource Manager low level driver
 *      This function is called once in the system to setup the Resource Manager 
 *      low level driver by mapping the application defined resource table to internal
 *      permission tables.  After mapping is complete a global synchronization object
 *      is written to sync with slave cores
 *
 *  @param[in]  rmResourceTable
 *      Resource table defined by application.  Used to populate internal permission
 *      tables.
 *
 *  @post  
 *      RM LLD global permissions are set.
 *
 *  @retval
 *      Success -   RM_OK
 *  @retval
 *      Failure -   RM_ERROR_PERMISSION_TABLE_POPULATION_FAILED
 */
Rm_Result Rm_init (const Rm_Resource *rmResourceTable)
{
    void *key;
    Rm_Result ret_val = RM_ERROR;

    /* Check permission structure sizes to make sure they're evenly
      * divisible into a cache line.  This generates no object code when
      * optimizer is on.  If failes, assert will occur at compile time */
    RM_COMPILE_TIME_SIZE_CHECK((RM_MAX_CACHE_ALIGN/sizeof(Rm_Perms)) * sizeof(Rm_Perms) == RM_MAX_CACHE_ALIGN);
    RM_COMPILE_TIME_SIZE_CHECK((RM_MAX_CACHE_ALIGN/sizeof(Rm_qmssLinkingRamPerms)) * \
                                                          sizeof(Rm_qmssLinkingRamPerms) == RM_MAX_CACHE_ALIGN);

    /* Begin Critical Section before accessing shared resources. */
    key = Rm_osalCsEnter ();
    
    /* Initialize the permissions table */
    Rm_permissionTableInit();

    if (!rmResourceTable)
    {
        /* End Critical Section */
        Rm_osalCsExit (key);
        return RM_ERROR_PERMISSION_TABLE_POPULATION_FAILED;
    }

    ret_val = Rm_populatePermissionTable(rmResourceTable);

    /* End Critical Section */
    Rm_osalCsExit (key);
    return ret_val;
}

/**
 *  @b Description
 *  @n  
 *      This function waits for the Resource Manager master to populate the 
 *      global permissions table based on a global sync object.  Once the
 *      global sync object has been written by the master core this function
 *      will invalidate all permissions tables.  Since the permissions table are
 *      static, and will not change through the system up-time, a single
 *      invalidation at the start will suffice.
 *      
 *      This function can be called on all core besides that which called
 *      Rm_init.  Calling this function on said cores will act as a blocking
 *      synchronization point.
 *
 *  @retval
 *      Success -   RM_OK
 *  @retval
 *      Failure -   RM_FAILURE
 */
Rm_Result Rm_start (void)
{
    /* Loop until the global sync object signals the permissions table has been
      * populated and valid */
    do
    {
        /* Invalidate the global sync object */
        Rm_osalBeginMemAccess ((void *) &rmGSyncObj, sizeof (Rm_Sync_Obj));
    } while (rmGSyncObj.obj.globalSyncObj != RM_PERMISSION_TABLE_VALID);

    /* Master core finished populating the permission tables.  Must invalidate
      * all tables to see latest permissions */
    Rm_updatePermissionTables();

    return RM_OK;
}

/**
 *  @b Description
 *  @n  
 *      This function returns an RM handle to the application to provide
 *      to LLDs that want to use the RM.
 *
 *  @retval
 *      Success -   RM Handle.  Used an an input parameter for LLD startCfg functions.
 *  @retval
 *      Failure -   NULL
 */
Rm_Handle Rm_getHandle(void)
{
   return ((void *) &rmPermissionCheckers);
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version information of the RM LLD.
 *
 *  @retval
 *      Version Information.
 */
uint32_t Rm_getVersion (void)
{
    return RM_LLD_VERSION_ID;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version string for the RM LLD.
 *
 *  @retval
 *      Version String.
 */
const char* Rm_getVersionStr (void)
{
    return rmLldVersionStr;
}

/**
@}
*/
