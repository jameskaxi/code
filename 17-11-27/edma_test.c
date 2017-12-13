//#include <stdio.h>
//#include <ti/csl/csl_edma3.h>
//#include <ti/csl/csl_edma3Aux.h>
//#include <ti/csl/csl_cacheAux.h>
#include "drv.h"
/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

/* Global Variables which are used to dump the TPCC register overlay in the 
 * debugger... */
CSL_TpccRegs*  gEDMACC0Regs  = (CSL_TpccRegs*)CSL_EDMA0CC_REGS;
CSL_TpccRegs*  gEDMACC1Regs  = (CSL_TpccRegs*)CSL_EDMA1CC_REGS;
CSL_TpccRegs*  gEDMACC2Regs  = (CSL_TpccRegs*)CSL_EDMA2CC_REGS;


edma_config_t edma_config[8];

int32_t edma_init (edma_config_t *cfg)
{    
    CSL_Edma3Obj                    edmaObj;
    CSL_Edma3ChannelObj             chObj;
    CSL_Edma3Context                context;
    CSL_Edma3ChannelAttr            chAttr;
    CSL_Status                      status;

    /* Module initialization */
    if (CSL_edma3Init(&context) != CSL_SOK) 
    {
        printf ("Error: EDMA module initialization failed\n");
        return -1;
    }
    
    /* Open the EDMA Module using the provided instance number */
    cfg->hModule = CSL_edma3Open(&edmaObj, cfg->instNum, NULL, &status);
    if ( (cfg->hModule == NULL) || (status != CSL_SOK))
    {
        printf ("Error: EDMA module open failed\n");
        return -1;
    }

    /* Channel open */
    chAttr.regionNum = CSL_EDMA3_REGION_GLOBAL;
    chAttr.chaNum    = cfg->channelNum;
    cfg->channelHandle = CSL_edma3ChannelOpen(&chObj, cfg->instNum, &chAttr, &status);
    if ((cfg->channelHandle == NULL) || (status != CSL_SOK))
    {
        printf ("Error: Unable to open EDMA Channel:%d\n", cfg->channelNum);
        return -1;
    }
 
    if(!cfg->instNum)
    {
        /* For first EDMA instance there are only 2 TCs and 2 event queues 
         * Modify the channel default queue setup from 0 to 1 
         */
         if (CSL_edma3HwChannelSetupQue(cfg->channelHandle,CSL_EDMA3_QUE_1) != CSL_SOK)
        {
            printf ("Error: EDMA channel setup queue failed\n");    
            return -1;
        }
    }
    else
    {
        /* For EDMA instance 1 and 2 maximum of 4 TCs and 4 event queues are supported
         * Change Channel Default queue setup from 0 to 3  
         */
        if (CSL_edma3HwChannelSetupQue(cfg->channelHandle,CSL_EDMA3_QUE_3) != CSL_SOK)
        {
            printf ("Error: EDMA channel setup queue failed\n");    
            return -1;
        }
    }


    /* Map the DMA Channel to PARAM Block 2. */
    CSL_edma3MapDMAChannelToParamBlock (cfg->hModule, cfg->channelNum, cfg->paramBlockNum);

    /* Obtain a handle to parameter set 2 */
    cfg->hParam = CSL_edma3GetParamHandle(cfg->channelHandle, cfg->paramBlockNum, &status);
    if (cfg->hParam == NULL)
    {
        printf ("Error: EDMA Get Parameter Entry failed for 2.\n");
        return -1;
    }
#if 0
    /* Setup the parameter entry parameters (Ping buffer) */
    cfg->myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS, \
                                             CSL_EDMA3_TCCH_DIS, \
                                             CSL_EDMA3_ITCINT_DIS, \
                                             CSL_EDMA3_TCINT_EN, \
                                             0, CSL_EDMA3_TCC_NORMAL,\
                                             CSL_EDMA3_FIFOWIDTH_NONE, \
                                             CSL_EDMA3_STATIC_DIS, \
                                             CSL_EDMA3_SYNC_A, \
                                             CSL_EDMA3_ADDRMODE_INCR, \
                                             CSL_EDMA3_ADDRMODE_INCR );
    cfg->myParamSetup.srcAddr    = (Uint32)cfg->srcaddr;
    cfg->myParamSetup.aCntbCnt   = CSL_EDMA3_CNT_MAKE(cfg->aCnt,cfg->bCnt);
    cfg->myParamSetup.dstAddr    = (Uint32)cfg->dstaddr;
    cfg->myParamSetup.srcDstBidx = CSL_EDMA3_BIDX_MAKE(cfg->srcBidx,cfg->dstBidx);
    cfg->myParamSetup.linkBcntrld= 0;//CSL_EDMA3_LINKBCNTRLD_MAKE(hParamPong,0);
    cfg->myParamSetup.srcDstCidx = CSL_EDMA3_CIDX_MAKE(cfg->srcCidx,cfg->dstCidx);
    cfg->myParamSetup.cCnt = cfg->cCnt;

    /* Ping setup */
    if (CSL_edma3ParamSetup(cfg->hParam,&cfg->myParamSetup) != CSL_SOK)
    {
        printf ("Error: EDMA Parameter Entry Setup failed\n");
        return -1;
    }
#endif
    /* The test passed. */    
    return 0;
}
void edma_setParam(edma_config_t *cfg)
{
    cfg->myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS, \
                                             CSL_EDMA3_TCCH_DIS, \
                                             CSL_EDMA3_ITCINT_DIS, \
                                             CSL_EDMA3_TCINT_EN, \
                                             0, CSL_EDMA3_TCC_NORMAL,\
                                             CSL_EDMA3_FIFOWIDTH_NONE, \
                                             CSL_EDMA3_STATIC_DIS, \
                                             CSL_EDMA3_SYNC_A, \
                                             CSL_EDMA3_ADDRMODE_INCR, \
                                             CSL_EDMA3_ADDRMODE_INCR );
    if (CSL_edma3ParamSetup(cfg->hParam,&cfg->myParamSetup) != CSL_SOK)
    {
        printf ("Error: EDMA Parameter Entry Setup failed\n");
    }
}
void edma_trigger(edma_config_t *cfg)
{
    CSL_Edma3CmdIntr                regionIntr;
#if 1
    /* Interrupt enable (Bits 0-1)  for the global region interrupts */
    regionIntr.region = CSL_EDMA3_REGION_GLOBAL;
    regionIntr.intr   = 0x0;
    regionIntr.intrh  = 0x0000;
    CSL_edma3HwControl(cfg->hModule,CSL_EDMA3_CMD_INTR_ENABLE,&regionIntr);
#endif
    /* Trigger channel */
    CSL_edma3HwChannelControl(cfg->channelHandle,CSL_EDMA3_CMD_CHANNEL_SET,NULL);
#if 1
    /* Poll on IPR bit 0 */
    do {
        CSL_edma3GetHwStatus(cfg->hModule,CSL_EDMA3_QUERY_INTRPEND,&regionIntr);
    } while (!(regionIntr.intr & 0x1));

    /* Clear the pending bit */
    CSL_edma3HwControl(cfg->hModule,CSL_EDMA3_CMD_INTRPEND_CLEAR,&regionIntr);
#endif
}
void edma_close(edma_config_t *cfg)
{
    /* Close channel */
    if (CSL_edma3ChannelClose(cfg->channelHandle) != CSL_SOK)
    {
        printf("Error: EDMA Channel Close failed\n");
    }
 
    /* Close EDMA module */
    if (CSL_edma3Close(cfg->hModule) != CSL_SOK)
    {
        printf("Error: EDMA Module Close failed\n");
    }
}
