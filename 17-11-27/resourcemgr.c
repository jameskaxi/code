/*
 *   @file  resourcemgr.c
 *
 *   @brief
 *      This file implements the common configuration of the resource manager for
 *      the application to use.
 *
 */
#include "drv.h"
#include <ti/drv/qmss/qmss_firmware.h>
#include <ti/drv/pa/fw/classify1_bin.h>
#include <ti/drv/pa/fw/classify2_bin.h>
#include <ti/drv/pa/fw/pam_bin.h>
/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
/* Configuration Information */
CPPI_CFG_T 		gCppiCfg[MAX_CPPI_CFG];

#pragma DATA_SECTION(gNimuHostDesc, ".resmgr_memregion")
#pragma DATA_ALIGN (gNimuHostDesc, 128)
uint8_t gNimuHostDesc[NIMU_NUM_DESC][NIMU_MAX_DESC_SIZE];

/* CPPI/QMSS Handles used by the application */
#pragma DATA_SECTION(gEthFreeQHnd, ".resmgr_handles")
Qmss_QueueHnd  gEthFreeQHnd;

#pragma DATA_ALIGN   (gTxReturnQHnd, 128)
#pragma DATA_SECTION (gTxReturnQHnd, ".resmgr_handles")
Qmss_QueueHnd gTxReturnQHnd;

#pragma DATA_ALIGN   (gTxFreeQHnd, 128)
#pragma DATA_SECTION (gTxFreeQHnd, ".resmgr_handles")
Qmss_QueueHnd gTxFreeQHnd;

#pragma DATA_ALIGN   (gRxFreeQHnd, 128)
#pragma DATA_SECTION (gRxFreeQHnd, ".resmgr_handles")
Qmss_QueueHnd gRxFreeQHnd;

#pragma DATA_ALIGN   (gRxQHnd, 128)
#pragma DATA_SECTION (gRxQHnd, ".resmgr_handles")
Qmss_QueueHnd gRxQHnd[8];

//#pragma DATA_ALIGN   (gTxCmdReturnQHnd, 128)
//#pragma DATA_SECTION (gTxCmdReturnQHnd, ".resmgr_handles")
//Qmss_QueueHnd gTxCmdReturnQHnd;

//#pragma DATA_ALIGN   (gTxCmdFreeQHnd, 128)
//#pragma DATA_SECTION (gTxCmdFreeQHnd, ".resmgr_handles")
//Qmss_QueueHnd gTxCmdFreeQHnd;

#pragma DATA_ALIGN   (gPaTxQHnd, 128)
#pragma DATA_SECTION (gPaTxQHnd, ".resmgr_handles")
Qmss_QueueHnd gPaTxQHnd [NUM_PA_TX_QUEUES];

/* PA command response queue handle */
#pragma DATA_ALIGN   (gPaCfgCmdRespQHnd, 128)
#pragma DATA_SECTION (gPaCfgCmdRespQHnd, ".resmgr_handles")
Qmss_QueueHnd gPaCfgCmdRespQHnd;
/* CPPI Handles */
#pragma DATA_ALIGN   (gRxFlowHnd, 128)
#pragma DATA_SECTION(gRxFlowHnd, ".resmgr_handles");
Cppi_FlowHnd gRxFlowHnd[8];

#pragma DATA_ALIGN   (gPaL2Handles, 128)
#pragma DATA_SECTION (gPaL2Handles, ".resmgr_handles")
paHandleL2L3_t gPaL2Handles[MAX_NUM_L2_HANDLES];

#pragma DATA_ALIGN   (gPaL3Handles, 128)
#pragma DATA_SECTION (gPaL3Handles, ".resmgr_handles")
paHandleL2L3_t gPaL3Handles[MAX_NUM_L3_HANDLES];

#pragma DATA_ALIGN   (gPaL4Handles, 128)
#pragma DATA_SECTION (gPaL4Handles, ".resmgr_handles")
paHandleL4_t gPaL4Handles[MAX_NUM_L4_HANDLES];

#pragma DATA_SECTION(gEthHiPriAccumList, ".resmgr_memregion")
#pragma DATA_ALIGN (gEthHiPriAccumList, 128)
uint32_t gEthHiPriAccumList[8][(RX_INT_THRESHOLD+1) * 2];
/* Semaphore Handle associated to receive data */
Semaphore_Handle    ethSemHandle;
/* Various stats  */
uint32_t gTxCounter=0, gRxCounter=0, gTxDropCounter = 0, gRxDropCounter=0;
/* TX port number */
//#pragma DATA_SECTION(gTxPort, ".resmgr_memregion")
uint32_t gTxPort;

/* PA instance */
#pragma DATA_SECTION(gPAInst, ".resmgr_memregion")
#pragma DATA_ALIGN(gPAInst, 128)
uint8_t        gPAInst[BUFSIZE_PA_INST];

/* Memory used for PA handles */
#pragma DATA_SECTION(gMemL2Ram, ".resmgr_memregion")
#pragma DATA_ALIGN(gMemL2Ram, 128)
uint8_t    gMemL2Ram[BUFSIZE_L2_TABLE];

#pragma DATA_SECTION(gMemL3Ram, ".resmgr_memregion")
#pragma DATA_ALIGN(gMemL3Ram, 128)
uint8_t    gMemL3Ram[BUFSIZE_L3_TABLE];

/* Channels handles for CPPI DMA support of the Packet Accelerator (PA) */
#pragma DATA_SECTION(gPassCpdmaTxChanHnd, ".resmgr_handles")
#pragma DATA_SECTION(gPassCpdmaRxChanHnd, ".resmgr_handles")
#pragma DATA_SECTION(gPassCpdmaHnd, 	  ".resmgr_handles")
#pragma DATA_ALIGN(gPassCpdmaTxChanHnd, 128)
#pragma DATA_ALIGN(gPassCpdmaRxChanHnd, 128)
#pragma DATA_ALIGN(gPassCpdmaHnd, 128)
Cppi_ChHnd     gPassCpdmaTxChanHnd [NUM_PA_TX_QUEUES];
Cppi_ChHnd     gPassCpdmaRxChanHnd [NUM_PA_RX_CHANNELS];
Cppi_Handle    gPassCpdmaHnd;

/* PA Driver Handle */
#pragma DATA_ALIGN   (gPAInstHnd, 128)
#pragma DATA_SECTION(gPAInstHnd, ".resmgr_handles")
Pa_Handle      gPAInstHnd;


/* Global debug counters */
//#pragma DATA_SECTION(gQPopErrorCounter, ".resmgr_memregion")
uint32_t gQPopErrorCounter;
//#pragma DATA_SECTION(gQPushErrorCounter, ".resmgr_memregion")
uint32_t gQPushErrorCounter;

#ifdef RIO
/* Memory allocated for the descriptors. This is 16 bit aligned. */
#pragma DATA_SECTION (gSrioHostDesc, ".resmgr_memregion")
#pragma DATA_ALIGN (gSrioHostDesc, 128)
uint8_t       gSrioHostDesc[SRIO_MAX_DESC_SIZE * SRIO_NUM_DESC];

//#pragma DATA_SECTION (gSrioHiPriAccumList, ".AccumList")
#pragma DATA_ALIGN (gSrioHiPriAccumList, 128)
uint32_t      gSrioHiPriAccumList[64];

#pragma DATA_SECTION(gSrioFreeQHnd, ".resmgr_handles")
Qmss_QueueHnd      gSrioFreeQHnd;
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//				2015,4,1    sync ipc
#pragma DATA_SECTION(gIPCDesc, ".resmgr_memregion")
#pragma DATA_ALIGN (gIPCDesc, 128)
uint8_t gIPCDesc[IPC_NUM_DESC * IPC_SIZE_DESC];

#pragma DATA_ALIGN   (gIPCFreeQHnd, 128)
#pragma DATA_SECTION (gIPCFreeQHnd, ".resmgr_handles")
Qmss_QueueHnd gIPCFreeQHnd;

#pragma DATA_ALIGN   (gIPCCompletionQHnd, 128)
#pragma DATA_SECTION (gIPCCompletionQHnd, ".resmgr_handles")
Qmss_QueueHnd gIPCCompletionQHnd;

#pragma DATA_ALIGN   (gIPCQHnd, 128)
#pragma DATA_SECTION (gIPCQHnd, ".resmgr_handles")
Qmss_QueueHnd gIPCQHnd[8];

#pragma DATA_ALIGN   (gIPCQHndPoll, 128)
#pragma DATA_SECTION (gIPCQHndPoll, ".resmgr_handles")
Qmss_QueueHnd gIPCQHndPoll[8];

#pragma DATA_ALIGN   (cppiQMSSHnd, 128)
#pragma DATA_SECTION (cppiQMSSHnd, ".resmgr_handles")
Cppi_Handle cppiQMSSHnd;

#pragma DATA_ALIGN (isQMSSInitialized, 128)
#pragma DATA_SECTION (isQMSSInitialized, ".resmgr_handles")
uint8_t             isQMSSInitialized[128];

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

extern Cppi_GlobalConfigParams cppiGblCfgParams;
extern Qmss_GlobalConfigParams  qmssGblCfgParams;

Qmss_QueueHnd getNimuFreequeue(void)
{
	return gEthFreeQHnd;
}

 /** ============================================================================
 *   @n@b QMSS_QPOP
 *
 *   @b Description
 *   @n This API implements a the queue pop logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  type
 *        Queue handler type
 *   @param[in]  pHostDescriptor
 *       pointer to  descriptor 
 * 
 *   @return    
 *   @n None
 * =============================================================================
 */
int32_t QMSS_QPOP(Qmss_QueueHnd handler,  QHANDLER_TYPE type, Cppi_HostDesc** pHostDescriptor)
{

    Cppi_HostDesc*      pHostDesc = *pHostDescriptor;

    pHostDesc = Qmss_queuePop (handler);
    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if (pHostDesc == NULL)
    {
        gQPopErrorCounter++;
        return -1;
    }
    /* The descriptor address returned from the hardware has the 
     * descriptor size appended to the address in the last 4 bits.
     *
     * To get the true descriptor size, always mask off the last 
     * 4 bits of the address.
     */
    pHostDesc = (Cppi_HostDesc *) ((uint32_t) pHostDesc & 0xFFFFFFF0);
    *pHostDescriptor = pHostDesc;

    /* Inv cache based on the qhandler type*/
    CACHE_invL1d((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_FENCE_WAIT);
    CACHE_invL2((void *) pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);

    if (type != QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF)
    {
        /* Not needed for FDQ with no attached buffers pop */
        CACHE_invL1d((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_FENCE_WAIT);
        CACHE_invL2((void *) pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    }
    return 0;
}

/** ============================================================================
 *   @n@b QMSS_QPUSH
 *
 *   @b Description
 *   @n This API implements a the queue push logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  descAddr
 *        Descriptor address
 *   @param[in]  packetSize
 *        packet Size
 *   @param[in]  descSize
 *        descriptor Size
 *   @param[in]  location
 *        Qmss_Location location
 *   @return    
 *   @n None
 * =============================================================================
 */
void QMSS_QPUSH (Qmss_QueueHnd handler, void *descAddr, uint32_t packetSize, uint32_t descSize, Qmss_Location location )
{
    Cppi_HostDesc* pHostDesc = (Cppi_HostDesc *) descAddr;
    
    if (descAddr == NULL)
    {
        gQPushErrorCounter ++;
        return;
    }    
    /* Wb data cache */
    CACHE_wbL1d((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_wbL2((void *) pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_wbL1d((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_wbL2((void *) pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);

    Qmss_queuePushDescSize (handler, pHostDesc, descSize);
}

/** ============================================================================
 *   @n@b QMSS_QPUSHDESCSIZE
 *
 *   @b Description
 *   @n This API implements a the queue push logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  descAddr
 *        Descriptor address
 *   @param[in]  descSize
 *        descriptor Size
 *   @return    
 *   @n None
 * =============================================================================
 */
void QMSS_QPUSHDESCSIZE (Qmss_QueueHnd handler, void *descAddr, uint32_t descSize)
{
    Cppi_HostDesc* pHostDesc = (Cppi_HostDesc *) descAddr;

    if (descAddr == NULL)
    {
        gQPushErrorCounter++;
        return;
    }
    /* Wb data cache */
    CACHE_wbL1d((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_FENCE_WAIT);
    CACHE_wbL2((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_FENCE_WAIT);
    CACHE_wbL1d((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_FENCE_WAIT);
    CACHE_wbL2((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_FENCE_WAIT);
    Qmss_queuePushDescSize(handler, pHostDesc, descSize);
}


int queueReset(void);
/**********************************************************************
 ************ Resource Manager QMSS configuration Functions ************
 **********************************************************************/
int queueReset(void)
{
    Cppi_Desc*     pCppiDesc;
    uint16_t         count, i;

    int handleNum, handleEnd = 8192;
    for (handleNum = 0; handleNum<handleEnd; handleNum++)
    {
		count = Qmss_getQueueEntryCount(handleNum);
//		if(count != 0)
//			printf("queueNum %d, count is %d\n", handleNum, count);
		for (i=0; i < count; i++)
			QMSS_QPOP (handleNum, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc);
    }
    return 1;

}

/** ============================================================================
 *   @n@b res_mgr_init_qmss
 *
 *   @b Description
 *   @n This API initializes the QMSS LLD.
 *
 *   @param[in]  p_qmss_cfg
 *        Pointer to QMSS_CFG_T
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t res_mgr_init_qmss (QMSS_CFG_T      *p_qmss_cfg)
{
    int32_t            			result;
    Qmss_InitCfg                qmssInitConfig;

    /* Initialize QMSS */
    memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));
    /* Set up QMSS configuration */
    /* Use internal linking RAM */
    qmssInitConfig.linkingRAM0Base  =   0;
    qmssInitConfig.linkingRAM0Size  =   0;
    qmssInitConfig.linkingRAM1Base  =   0x0;
    qmssInitConfig.maxDescNum       =   p_qmss_cfg->max_num_desc;
    qmssInitConfig.pdspFirmware[0].pdspId = Qmss_PdspId_PDSP1;
    #if 1 /* LE Mode */
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_le;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_le);
    #else
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_be;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_be);
    #endif
    /* Initialize the Queue Manager */
    result = Qmss_init (&qmssInitConfig, &qmssGblCfgParams);
    if (result < QMSS_SOK)
    {
        /* Error initializing Queue Manager SubSystem */
    	printf("Error initializing Queue Manager SubSystem\n");
    }

    /* Start Queue manager on this core */
    result = Qmss_start ();
    if (result < QMSS_SOK)
    {
    	printf("Core %d : Error starting Queue Manager error code : %d\n", DNUM, result);
    }
    else
    {
    	printf("Qmss start ok!\n");
    }
    queueReset();

    result = Cppi_init (&cppiGblCfgParams);
    if (result != CPPI_SOK)
    {
    	printf ("Error initializing Queue Manager SubSystem error code : %d\n", result);
        return -1;
    }
    return 0;
}
#if 1
/** ============================================================================
 *   @n@b res_mgr_stop_qmss
 *
 *   @b Description
 *   @n This API de-initializes the QMSS LLD.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t res_mgr_stop_qmss (void)
{
    Qmss_queueClose (gEthFreeQHnd);
    return 0;
}

/**********************************************************************
 ************ Resoure Manager CPPI configuration Functions ************
 **********************************************************************/

/** ============================================================================
 *   @n@b res_mgr_cppi_get_passhandle
 *
 *   @b Description
 *   @n This API returns the handle to the the CPPI DMA for the Packet Accelerator (PA).
 *
 *   @param[in]  None
 *
 *   @return  Cppi_Handle
 * =============================================================================
 */
Cppi_Handle res_mgr_cppi_get_passhandle (void)
{
	return gPassCpdmaHnd;
}


/** ============================================================================
 *   @n@b res_mgr_init_cppi
 *
 *   @b Description
 *   @n This API initializes the CPPI LLD, opens the PDMA and opens up
 *      the Tx, Rx channels required for data transfers.
 *
 *   @param[in]  p_qmss_cfg
 *        Pointer to CPPI_CFG_T
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t res_mgr_init_cppi(CPPI_CFG_T *p_cppi_cfg)
{
    int32_t                     i;
    Cppi_CpDmaInitCfg           cpdmaCfg;
    uint8_t                     isAllocated;
    Cppi_TxChInitCfg            txChCfg;
    Cppi_RxChInitCfg            rxChInitCfg;

    /* Initialize CPDMA */
    memset (&cpdmaCfg, 0, sizeof (Cppi_CpDmaInitCfg));
    cpdmaCfg.dmaNum     = p_cppi_cfg->dma_num;
    if (cpdmaCfg.dmaNum == Cppi_CpDma_PASS_CPDMA)
    {
        memcpy(&gCppiCfg[CPPI_CFG_PASS], p_cppi_cfg, sizeof(CPPI_CFG_T));
    }

    if ((gPassCpdmaHnd = Cppi_open (&cpdmaCfg)) == NULL)
    {
		printf("Error initializing CPPI for CPDMA %d \n", cpdmaCfg.dmaNum);
		return -1;
	}
    /* Open all CPPI Tx Channels. These will be used to send data to PASS/CPSW */
    for (i = 0; i < p_cppi_cfg->num_tx_queues; i ++)
    {
        txChCfg.channelNum      =   i;       /* CPPI channels are mapped one-one to the PA Tx queues */
        txChCfg.txEnable        =   Cppi_ChState_CHANNEL_DISABLE;  /* Disable the channel for now. */
        txChCfg.filterEPIB      =   0;
        txChCfg.filterPS        =   0;
        txChCfg.aifMonoMode     =   0;
        txChCfg.priority        =   2;
        if ((gPassCpdmaTxChanHnd[i] = Cppi_txChannelOpen (gPassCpdmaHnd, &txChCfg, &isAllocated)) == NULL)
        {
            printf("Error opening Tx channel %d\n", txChCfg.channelNum);
            return -1;
        }
        Cppi_channelEnable (gPassCpdmaTxChanHnd[i]);
    }

    /* Open all CPPI Rx channels. These will be used by PA to stream data out. */
    for (i = 0; i < p_cppi_cfg->num_rx_channels; i++)
    {
        /* Open a CPPI Rx channel that will be used by PA to stream data out. */
        rxChInitCfg.channelNum  =   i;
        rxChInitCfg.rxEnable    =   Cppi_ChState_CHANNEL_DISABLE;
        if ((gPassCpdmaRxChanHnd[i] = Cppi_rxChannelOpen (gPassCpdmaHnd, &rxChInitCfg, &isAllocated)) == NULL)
        {
        	printf("Error opening Rx channel: %d \n", rxChInitCfg.channelNum);
            return -1;
        }

        /* Also enable Rx Channel */
        Cppi_channelEnable (gPassCpdmaRxChanHnd[i]);
    }

    /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
    Cppi_setCpdmaLoopback(gPassCpdmaHnd, 0);
    /* CPPI Init Done. Return success */
    return 0;
}

/** ============================================================================
 *   @n@b res_mgr_stop_cppi
 *
 *   @b Description
 *   @n This API de-initializes the CPPI LLD.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t res_mgr_stop_cppi (CPPI_CFG_TYPE cfg_type)
{
    uint32_t i;

    if (cfg_type == CPPI_CFG_PASS)
    {
        for (i = 0; i < gCppiCfg[cfg_type].num_rx_channels; i++)
            Cppi_channelClose (gPassCpdmaRxChanHnd[i]);
        
        for (i = 0; i < gCppiCfg[cfg_type].num_tx_queues; i++)
            Cppi_channelClose (gPassCpdmaTxChanHnd[i]);
    }

    return 0;
}


/**********************************************************************
 ************ Resource Manager PA configuration Functions ************
 **********************************************************************/
/** ============================================================================
 *   @n@b Download_PAFirmware
 *
 *   @b Description
 *   @n This API downloads the PA firmware required for PDSP operation.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Download_PAFirmware (Void)
{
    int32_t                   i;

    /* Hold the PA in reset state during download */
    Pa_resetControl (gPAInstHnd, pa_STATE_RESET);

    /* PDPSs 0-2 use image c1 */
    for (i = 0; i < 3; i++)
    {
        Pa_downloadImage (gPAInstHnd, i, (Ptr)c1, sizeof(c1));
    }

    /* PDSP 3 uses image c2 */
    Pa_downloadImage (gPAInstHnd, 3, (Ptr)c2, sizeof(c2));

    /* PDSPs 4-5 use image m */
    for (i = 4; i < 6; i++)
    {
        Pa_downloadImage (gPAInstHnd, i, (Ptr)m, sizeof(m));
    }

    /* Enable the PA back */
    Pa_resetControl (gPAInstHnd, pa_STATE_ENABLE);

    return 0;
}

/** ============================================================================
 *   @n@b res_mgr_get_painstance
 *
 *   @b Description
 *   @n This API returns the handle to the PA.
 *
 *   @param[in]  None
 *
 *   @return  Pa_Handle
 * =============================================================================
 */
Pa_Handle res_mgr_get_painstance (void)
{
	return gPAInstHnd;
}

/* ============================================================================
 *   @n@b res_mgr_init_pass
 *
 *   @b Description
 *   @n This API initializes the PASS/PDSP.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */

int32_t res_mgr_init_pass (void)
{
    paSizeInfo_t                paSize = {0};
    paConfig_t                  paCfg = {0};
    int32_t                     retVal;
    int32_t                     sizes[pa_N_BUFS];
    int32_t                     aligns[pa_N_BUFS];
    void*                       bases[pa_N_BUFS];
    
    /* Allocate space for the PA LLD buffers. The buffers we need to
    * allocate space are:
    *      (1) PA LLD Instance Info Handle
    *      (2) PA LLD L2 Handle database
    *      (3) PA LLD L3 Handle database
    */
    paSize.nMaxL2 = MAX_NUM_L2_HANDLES;
    paSize.nMaxL3 = MAX_NUM_L3_HANDLES;
    if ((retVal = Pa_getBufferReq(&paSize, sizes, aligns)) != pa_OK)
    {
        printf ("Pa_getBufferReq returned error %d\n", retVal);
        return -1;
    }
    
    /* Validate the buffer allocations */
    /* The first buffer is always the instance buffer */
    if ((uint32_t)gPAInst & (aligns[0] - 1))
    {
        printf("Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[0], (uint32_t)gPAInst);
        return -1;
    }
    
    if (sizeof(gPAInst) < sizes[0])
    {
        printf ("Pa_getBufferReq requires %d bytes for instance buffer, have only %d\n", sizes[0], sizeof(gPAInst));
        return -1;
    }
    
    bases[0]    =   (Void *)gPAInst;
    
    /* The second buffer is the L2 table */
    if ((uint32_t)gMemL2Ram & (aligns[1] - 1))
    {
        printf ("Pa_getBufferReq requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[1], (uint32_t)gMemL2Ram);
        return (-1);
    }
    
    if (sizeof(gMemL2Ram) < sizes[1])
    {
        printf ("Pa_getBufferReq requires %d bytes for buffer 1, have only %d\n", sizes[1], sizeof(gMemL2Ram));
        return -1;
    }
    
    bases[1]    =   (Void *)gMemL2Ram;
    
    /* The third buffer is the L3 table */
    if ((uint32_t)gMemL3Ram & (aligns[2] - 1))
    {
        printf ("Pa_alloc requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[2], (uint32_t)gMemL3Ram);
        return (-1);
    }
    
    if (sizeof(gMemL3Ram) < sizes[2])
    {
        printf ("Pa_alloc requires %d bytes for buffer 1, have only %d\n", sizes[2], sizeof(gMemL3Ram));
        return (-1);
    }
    
    bases[2]    =   (Void *)gMemL3Ram;
    
    /* Finally initialize the PA LLD */
    paCfg.initTable =   TRUE;
    paCfg.initDefaultRoute = TRUE;
    paCfg.baseAddr = CSL_PA_SS_CFG_REGS;
    paCfg.sizeCfg   =   &paSize;
    if ((retVal = Pa_create (&paCfg, bases, &gPAInstHnd)) != pa_OK)
    {
        printf ("Pa_create returned with error code %d\n", retVal);
        return -1;
    }
    
    /* Download the PASS PDSP firmware */
    if (Download_PAFirmware ())
    {
        return -1;
    }
    /* PA Init Done. Return success */
    return 0;
}
#endif

