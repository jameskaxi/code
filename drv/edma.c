/*
 *
*/
#include "drv.h"

CSL_TpccRegs*  gpEDMA_CC0_regs  = (CSL_TpccRegs*)CSL_EDMA0CC_REGS;
CSL_TpccRegs*  gpEDMA_CC1_regs  = (CSL_TpccRegs*)CSL_EDMA1CC_REGS;
CSL_TpccRegs*  gpEDMA_CC2_regs  = (CSL_TpccRegs*)CSL_EDMA2CC_REGS;
CSL_TptcRegs * gpEDMA_TC_regs[10]= {
    (CSL_TptcRegs *) CSL_EDMA0TC0_REGS,
    (CSL_TptcRegs *) CSL_EDMA0TC1_REGS,
    (CSL_TptcRegs *) CSL_EDMA1TC0_REGS,
    (CSL_TptcRegs *) CSL_EDMA1TC1_REGS,
    (CSL_TptcRegs *) CSL_EDMA1TC2_REGS,
    (CSL_TptcRegs *) CSL_EDMA1TC3_REGS,
    (CSL_TptcRegs *) CSL_EDMA2TC0_REGS,
    (CSL_TptcRegs *) CSL_EDMA2TC1_REGS,
    (CSL_TptcRegs *) CSL_EDMA2TC2_REGS,
    (CSL_TptcRegs *) CSL_EDMA2TC3_REGS
};
void EDMA_init ()
{
    int i;
    unsigned int * uipPaRAM;

    /*clear PaRAM*/
    uipPaRAM= (unsigned int *)&(gpEDMA_CC0_regs->PARAMSET[0]);
    for(i=0; i<8*CSL_EDMA3_TPCC0_NUM_PARAMSETS; i++)
        *uipPaRAM++=0;

    uipPaRAM= (unsigned int *)&(gpEDMA_CC1_regs->PARAMSET[0]);
    for(i=0; i<8*CSL_EDMA3_TPCC1_NUM_PARAMSETS; i++)
        *uipPaRAM++=0;

    uipPaRAM= (unsigned int *)&(gpEDMA_CC2_regs->PARAMSET[0]);
    for(i=0; i<8*CSL_EDMA3_TPCC2_NUM_PARAMSETS; i++)
        *uipPaRAM++=0;

    /*Assign PaRAM for different channels*/
    for(i=0; i<CSL_EDMA3_TPCC0_NUM_DMACH; i++)
        gpEDMA_CC0_regs->TPCC_DCHMAP[i] = i<< CSL_TPCC_TPCC_DCHMAP0_PAENTRY_SHIFT;

    for(i=0; i<CSL_EDMA3_TPCC1_NUM_DMACH; i++)
        gpEDMA_CC1_regs->TPCC_DCHMAP[i] = i<< CSL_TPCC_TPCC_DCHMAP0_PAENTRY_SHIFT;

    for(i=0; i<CSL_EDMA3_TPCC2_NUM_DMACH; i++)
        gpEDMA_CC2_regs->TPCC_DCHMAP[i] = i<< CSL_TPCC_TPCC_DCHMAP0_PAENTRY_SHIFT;

    /*Assign TC/Queue for different channels*/
    gpEDMA_CC0_regs->TPCC_DMAQNUM[0]= 0x10101010;
    gpEDMA_CC0_regs->TPCC_DMAQNUM[1]= 0x10101010;

    gpEDMA_CC1_regs->TPCC_DMAQNUM[0]= 0x32103210;
    gpEDMA_CC1_regs->TPCC_DMAQNUM[1]= 0x32103210;
    gpEDMA_CC1_regs->TPCC_DMAQNUM[2]= 0x32103210;
    gpEDMA_CC1_regs->TPCC_DMAQNUM[3]= 0x32103210;
    gpEDMA_CC1_regs->TPCC_DMAQNUM[4]= 0x32103210;
    gpEDMA_CC1_regs->TPCC_DMAQNUM[5]= 0x32103210;
    gpEDMA_CC1_regs->TPCC_DMAQNUM[6]= 0x32103210;
    gpEDMA_CC1_regs->TPCC_DMAQNUM[7]= 0x32103210;

    gpEDMA_CC2_regs->TPCC_DMAQNUM[0]= 0x32103210;
    gpEDMA_CC2_regs->TPCC_DMAQNUM[1]= 0x32103210;
    gpEDMA_CC2_regs->TPCC_DMAQNUM[2]= 0x32103210;
    gpEDMA_CC2_regs->TPCC_DMAQNUM[3]= 0x32103210;
    gpEDMA_CC2_regs->TPCC_DMAQNUM[4]= 0x32103210;
    gpEDMA_CC2_regs->TPCC_DMAQNUM[5]= 0x32103210;
    gpEDMA_CC2_regs->TPCC_DMAQNUM[6]= 0x32103210;
    gpEDMA_CC2_regs->TPCC_DMAQNUM[7]= 0x32103210;

    /*clear any events and status*/
    gpEDMA_CC0_regs->TPCC_ECR=  0xFFFF;
    gpEDMA_CC0_regs->TPCC_EECR= 0xFFFF;
    gpEDMA_CC0_regs->TPCC_ICR=  0xFFFF;
    gpEDMA_CC0_regs->TPCC_IECR= 0xFFFF;
    gpEDMA_CC0_regs->TPCC_EMCR= 0xFFFF;
    gpEDMA_CC0_regs->TPCC_QEMCR= 0xFFFF;
    gpEDMA_CC0_regs->TPCC_CCERRCLR= 0xFFFF;
    gpEDMA_CC0_regs->TPCC_SECR= 0xFFFF;

    gpEDMA_CC1_regs->TPCC_ECR= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_ECRH= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_EECR= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_EECRH= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_ICR= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_ICRH= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_IECR= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_IECRH= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_EMCR= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_EMCRH= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_QEMCR= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_CCERRCLR= 0xFFFF;
    gpEDMA_CC1_regs->TPCC_SECR= 0xFFFFFFFF;
    gpEDMA_CC1_regs->TPCC_SECRH= 0xFFFFFFFF;

    gpEDMA_CC2_regs->TPCC_ECR= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_ECRH= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_EECR= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_EECRH= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_ICR= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_ICRH= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_IECR= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_IECRH= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_EMCR= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_EMCRH= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_QEMCR= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_CCERRCLR= 0xFFFF;
    gpEDMA_CC2_regs->TPCC_SECR= 0xFFFFFFFF;
    gpEDMA_CC2_regs->TPCC_SECRH= 0xFFFFFFFF;

    for(i=0; i<4; i++)
    {
        //Clear TC error
        gpEDMA_TC_regs[i]->TPTC_ERRCLR= 0xF;

        //enable error interrupt
        gpEDMA_TC_regs[i]->TPTC_ERREN=
            (1<<CSL_TPTC_TPTC_ERREN_MMRAERR_SHIFT)
            |(1<<CSL_TPTC_TPTC_ERREN_TRERR_SHIFT)
            |(1<<CSL_TPTC_TPTC_ERREN_BUSERR_SHIFT);
    }
}

/*Test throughput of EDMA "TC_channel" for data transfer from "src" to "dst"*/
void edma_copy (uint32_t src, uint32_t dst, int16_t uiACount, int16_t uiBCount, int16_t uiIndex)
{
    uint32_t CC_channel;
    uint32_t TC_num = 0;
    uint32_t param_num;
    CSL_TpccRegs*  EDMACCRegs;

    CC_channel = DNUM;
    param_num  = DNUM;
    EDMACCRegs = (CSL_TpccRegs*)CSL_EDMA0CC_REGS;
    if(DNUM < 4)
    {
        TC_num = DNUM;
        CC_channel = DNUM;
        EDMACCRegs = (CSL_TpccRegs*)CSL_EDMA1CC_REGS;
    }else
    {
        TC_num = DNUM-4;
        CC_channel = DNUM-4;
        EDMACCRegs = (CSL_TpccRegs*)CSL_EDMA2CC_REGS;
    }

    EDMACCRegs->TPCC_DCHMAP[CC_channel] = (param_num)<<5;
    //clear completion flag
    EDMACCRegs->TPCC_ICR = (1<<TC_num);

    EDMACCRegs->PARAMSET[param_num].OPT=
        CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,
            CSL_EDMA3_TCCH_DIS,
            CSL_EDMA3_ITCINT_DIS,
            CSL_EDMA3_TCINT_EN,
            TC_num,
            CSL_EDMA3_TCC_NORMAL,
            CSL_EDMA3_FIFOWIDTH_NONE,
            CSL_EDMA3_STATIC_DIS,
            CSL_EDMA3_SYNC_AB,
            CSL_EDMA3_ADDRMODE_INCR,
            CSL_EDMA3_ADDRMODE_INCR);
    EDMACCRegs->PARAMSET[param_num].SRC= src;
    EDMACCRegs->PARAMSET[param_num].A_B_CNT= CSL_EDMA3_CNT_MAKE(uiACount, uiBCount);
    EDMACCRegs->PARAMSET[param_num].DST= dst;
    EDMACCRegs->PARAMSET[param_num].SRC_DST_BIDX= CSL_EDMA3_BIDX_MAKE(uiIndex, uiIndex);
    EDMACCRegs->PARAMSET[param_num].LINK_BCNTRLD= CSL_EDMA3_LINKBCNTRLD_MAKE(0, uiBCount);
    EDMACCRegs->PARAMSET[param_num].SRC_DST_CIDX= 0;
    EDMACCRegs->PARAMSET[param_num].CCNT= 1;

    /*Manually trigger the EDMA*/
    EDMACCRegs->TPCC_ESR= 1<<(CC_channel);

    /* Wait for completion */
    while ((EDMACCRegs->TPCC_IPR&(1<<(TC_num))) ==0);
    //clear completion flag
    EDMACCRegs->TPCC_ICR=(1<<(TC_num));
    return;
}
