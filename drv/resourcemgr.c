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
shareVal_t  *gGlobalDataPtr = (shareVal_t*)(0x9ea00000);
accumList_t *gAccumListPtr  = (accumList_t*)(0x9eb00000);
desc_t      *gDescPtr       = (desc_t*)(0x9ec00000);

/* Semaphore Handle associated to receive data */
uint32_t gTxCounter=0, gRxCounter=0, gTxDropCounter = 0, gRxDropCounter=0;
uint32_t gQPopErrorCounter;
uint32_t gQPushErrorCounter;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

extern Cppi_GlobalConfigParams cppiGblCfgParams;
extern Qmss_GlobalConfigParams  qmssGblCfgParams;

Qmss_QueueHnd getNimuFreequeue(void)
{
	return gGlobalDataPtr->gEthFreeQHnd;
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
    Qmss_queueClose (gGlobalDataPtr->gEthFreeQHnd);
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
	return gGlobalDataPtr->gPassCpdmaHnd;
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
        memcpy(&gGlobalDataPtr->gCppiCfg[CPPI_CFG_PASS], p_cppi_cfg, sizeof(CPPI_CFG_T));
    }

    if ((gGlobalDataPtr->gPassCpdmaHnd = Cppi_open (&cpdmaCfg)) == NULL)
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
        if ((gGlobalDataPtr->gPassCpdmaTxChanHnd[i] = Cppi_txChannelOpen (gGlobalDataPtr->gPassCpdmaHnd, &txChCfg, &isAllocated)) == NULL)
        {
            printf("Error opening Tx channel %d\n", txChCfg.channelNum);
            return -1;
        }
        Cppi_channelEnable (gGlobalDataPtr->gPassCpdmaTxChanHnd[i]);
    }

    /* Open all CPPI Rx channels. These will be used by PA to stream data out. */
    for (i = 0; i < p_cppi_cfg->num_rx_channels; i++)
    {
        /* Open a CPPI Rx channel that will be used by PA to stream data out. */
        rxChInitCfg.channelNum  =   i;
        rxChInitCfg.rxEnable    =   Cppi_ChState_CHANNEL_DISABLE;
        if ((gGlobalDataPtr->gPassCpdmaRxChanHnd[i] = Cppi_rxChannelOpen (gGlobalDataPtr->gPassCpdmaHnd, &rxChInitCfg, &isAllocated)) == NULL)
        {
        	printf("Error opening Rx channel: %d \n", rxChInitCfg.channelNum);
            return -1;
        }

        /* Also enable Rx Channel */
        Cppi_channelEnable (gGlobalDataPtr->gPassCpdmaRxChanHnd[i]);
    }

    /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
    Cppi_setCpdmaLoopback(gGlobalDataPtr->gPassCpdmaHnd, 0);
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
        for (i = 0; i < gGlobalDataPtr->gCppiCfg[cfg_type].num_rx_channels; i++)
            Cppi_channelClose (gGlobalDataPtr->gPassCpdmaRxChanHnd[i]);
        
        for (i = 0; i < gGlobalDataPtr->gCppiCfg[cfg_type].num_tx_queues; i++)
            Cppi_channelClose (gGlobalDataPtr->gPassCpdmaTxChanHnd[i]);
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
    Pa_resetControl (gGlobalDataPtr->gPAInstHnd, pa_STATE_RESET);

    /* PDPSs 0-2 use image c1 */
    for (i = 0; i < 3; i++)
    {
        Pa_downloadImage (gGlobalDataPtr->gPAInstHnd, i, (Ptr)c1, sizeof(c1));
    }

    /* PDSP 3 uses image c2 */
    Pa_downloadImage (gGlobalDataPtr->gPAInstHnd, 3, (Ptr)c2, sizeof(c2));

    /* PDSPs 4-5 use image m */
    for (i = 4; i < 6; i++)
    {
        Pa_downloadImage (gGlobalDataPtr->gPAInstHnd, i, (Ptr)m, sizeof(m));
    }

    /* Enable the PA back */
    Pa_resetControl (gGlobalDataPtr->gPAInstHnd, pa_STATE_ENABLE);

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
	return gGlobalDataPtr->gPAInstHnd;
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
    if ((uint32_t)gGlobalDataPtr->gPAInst & (aligns[0] - 1))
    {
        printf("Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[0], (uint32_t)gGlobalDataPtr->gPAInst);
        return -1;
    }
    
    if (sizeof(gGlobalDataPtr->gPAInst) < sizes[0])
    {
        printf ("Pa_getBufferReq requires %d bytes for instance buffer, have only %d\n", sizes[0], sizeof(gGlobalDataPtr->gPAInst));
        return -1;
    }
    
    bases[0]    =   (Void *)gGlobalDataPtr->gPAInst;
    
    /* The second buffer is the L2 table */
    if ((uint32_t)gGlobalDataPtr->gMemL2Ram & (aligns[1] - 1))
    {
        printf ("Pa_getBufferReq requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[1], (uint32_t)gGlobalDataPtr->gMemL2Ram);
        return (-1);
    }
    
    if (sizeof(gGlobalDataPtr->gMemL2Ram) < sizes[1])
    {
        printf ("Pa_getBufferReq requires %d bytes for buffer 1, have only %d\n", sizes[1], sizeof(gGlobalDataPtr->gMemL2Ram));
        return -1;
    }
    
    bases[1]    =   (Void *)gGlobalDataPtr->gMemL2Ram;
    
    /* The third buffer is the L3 table */
    if ((uint32_t)gGlobalDataPtr->gMemL3Ram & (aligns[2] - 1))
    {
        printf ("Pa_alloc requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[2], (uint32_t)gGlobalDataPtr->gMemL3Ram);
        return (-1);
    }
    
    if (sizeof(gGlobalDataPtr->gMemL3Ram) < sizes[2])
    {
        printf ("Pa_alloc requires %d bytes for buffer 1, have only %d\n", sizes[2], sizeof(gGlobalDataPtr->gMemL3Ram));
        return (-1);
    }
    
    bases[2]    =   (Void *)gGlobalDataPtr->gMemL3Ram;
    
    /* Finally initialize the PA LLD */
    paCfg.initTable =   TRUE;
    paCfg.initDefaultRoute = TRUE;
    paCfg.baseAddr = CSL_PA_SS_CFG_REGS;
    paCfg.sizeCfg   =   &paSize;
    if ((retVal = Pa_create (&paCfg, bases, &gGlobalDataPtr->gPAInstHnd)) != pa_OK)
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

