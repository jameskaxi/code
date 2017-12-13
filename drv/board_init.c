/*
 * EVM_init.c
 *
 *  Created on: 2012-8-13
 *      Author: user
 */
//usrdef.h
#include "drv.h"

#ifdef RIO
/* Global SRIO Driver Handle. */
#pragma DATA_SECTION(hSrioDriver, ".resmgr_handles");
Srio_DrvHandle      hSrioDriver;
#endif

#pragma DATA_SECTION(gEvmc6678Info, ".resmgr_handles");
evmc6678Info 				gEvmc6678Info;

#define SES_BADDR   0xA0000000>>12
#define SES_RADDR(num)   ((0x800000000+512*0x100000+(num)*64*0x100000)>>4)>>8
#define SES_SEGSIZE 0x19 //64M

typedef struct  {
    volatile Uint32 REG_A_EntryCount;
    volatile Uint32 REG_B_ByteCount;
    volatile Uint32 REG_C_HeadTail_PacketSize;
    volatile Uint32 REG_D_Descriptor;
} QueueManageRegs;
/*queue access registers through VBUSP configration bus*/
QueueManageRegs * gpQueueManageRegs = (QueueManageRegs *)CSL_QM_SS_CFG_QM_QUEUE_DEQUEUE_REGS;
/**
 *  @brief
 *      Hardware network subsystem support, ethernet switch
 */
#define DEVICE_CPSW
#define DEVICE_CPSW_NUM_PORTS       3                    /* 3 switch ports */
#define DEVICE_CPSW_BASE            (0x02090800)
#define targetGetSwitchCtl()        CPSW_CTL_P0_ENABLE   /* Enable port 0 */
#define targetGetSwitchMaxPktSize() 9000

#define DEVICE_QM
#define DEVICE_QM_MANAGER_BASE          0x02a68000
#define DEVICE_QM_DESC_SETUP_BASE       0x02a6a000
#define DEVICE_QM_MANAGER_QUEUES_BASE   0x02a20000
#define DEVICE_QM_MANAGER_Q_PROXY_BASE  0x02a40000
#define DEVICE_QM_QUEUE_STATUS_BASE     0x02a00000
#define DEVICE_QM_NUM_LINKRAMS          2
#define DEVICE_QM_NUM_MEMREGIONS        20

/* Memory map */
/* Relative to the queue manager region */
#define QM_REG_REVISION         0x00
#define QM_REG_DIVERSION        0x08
#define QM_REG_LINKRAM_BASE(x)  (0x0c + 8*(x))
#define QM_REG_LINKRAM_SIZE(x)  (0x10 + 8*(x))
/* The queue peek registers (includes thresholds) */
#define QM_REG_STAT_CFG_REGD(x)  (0xc + 16*(x))
/* Relative to the descriptor setup region */
#define QM_REG_MEMR_BASE_ADDR(x)  (0x00 + 16*(x))
#define QM_REG_MEMR_START_IDX(x)  (0x04 + 16*(x))
#define QM_REG_MEMR_DESC_SETUP(x) (0x08 + 16*(x))
/* Queues, register A */
#define QM_REG_QUEUE_REGA(x)  (0x00 + 16*(x))
#define QM_QA_ENTRY_COUNT_MSB  18
#define QM_QA_ENTRY_COUNT_LSB   0
/* Queues, register D */
#define QM_REG_QUEUE_REGD(x)  (0x0c + 16*(x))
#define chipLmbd(x,y) _lmbd(x,y)
#define QM_INTD_CONFIG_REGION   0x02aa0000

#define DEVICE_CPDMA
#define DEVICE_PA_CDMA_GLOBAL_CFG_BASE   0x02004000
#define DEVICE_PA_CDMA_TX_CHAN_CFG_BASE  0x02004400
#define DEVICE_PA_CDMA_RX_CHAN_CFG_BASE  0x02004800
#define DEVICE_PA_CDMA_RX_FLOW_CFG_BASE  0x02005000

#define DEVICE_PA_CDMA_RX_NUM_CHANNELS   24
#define DEVICE_PA_CDMA_RX_NUM_FLOWS      32
#define DEVICE_PA_CDMA_TX_NUM_CHANNELS   9

#define DEVICE_RX_CDMA_TIMEOUT_COUNT    1000
/* Emulation control register */
#define CPDMA_REG_EMU_CTL   0x08

/* CPPI Tx DMA channel control registers */
#define CPDMA_REG_TCHAN_CFG_REG_A(x)   (0x00 + (x)*0x20)
#define CPDMA_REG_TCHAN_CFG_REG_B(x)   (0x04 + (x)*0x20)
/* CPPI Rx DMA channel control register */
#define CPDMA_REG_RCHAN_CFG_REG_A(x)    (0x00 + (x)*0x20)
/* CPPI Tx DMA Scheduler Configuration register */
#define CPDMA_REG_TCHAN_SCHED_CFG(x)    ((x)*0x04)
/* CPPI Tx DMA channel control register A definitions */
#define CPDMA_REG_VAL_TCHAN_A_TX_ENABLE ((unsigned int)1 << 31)
#define CPDMA_REG_VAL_TCHAN_A_TX_TDOWN  (1 << 30)
/* CPPI Rx DMA channel control register A definitions */
#define CPDMA_REG_VAL_RCHAN_A_RX_ENABLE ((unsigned int)1 << 31)
#define CPDMA_REG_VAL_RCHAN_A_RX_TDOWN  (1 << 30)
/* CPPI Rx DMA flow configuration registers */
#define CPDMA_RX_FLOW_CFG(reg,idx)      ( ((reg)*4) + ((idx)*0x20) )
#define CPDMA_RX_FLOW_REG_A     0
#define CPDMA_RX_FLOW_REG_B     1
#define CPDMA_RX_FLOW_REG_C     2
#define CPDMA_RX_FLOW_REG_D     3
#define CPDMA_RX_FLOW_REG_E     4
#define CPDMA_RX_FLOW_REG_F     5
#define CPDMA_RX_FLOW_REG_G     6
#define CPDMA_RX_FLOW_REG_H     7

#define DEVICE_PA
#define DEVICE_PA_BASE                  0x02000000
#define DEVICE_PA_NUM_PDSPS             6
#define DEVICE_PA_RUN_CHECK_COUNT       100         /* Number of loops to verify PA firmware is running */
#define DEVICE_PA_PLL_BASE              0x02620338
#define chipLower8(x)                   ((x) & 0x00ff)
#define PA_REG_MAILBOX_SLOT(pdsp, slot)     (0x00 + ((pdsp)*0x10) + ((slot)*0x04))
#define PA_REG_PDSP_CTL(pdsp)               (0x1000 + ((pdsp)*0x100))
#define PA_MEM_PDSP_IRAM(pdsp)              (0x10000 + ((pdsp)*0x8000))
#define PA_PID_SOFT_RESET               0x404
#define PA_PID_RANGE_LIM                0x408
#define PA_IDVAL                        0x40C

/* The pdsp control register */
#define PA_REG_VAL_PDSP_CTL_ENABLE_PDSP(pcVal)   (((pcVal) << 16) | 0x3)
#define PA_REG_VAL_PDSP_CTL_DISABLE_PDSP         0

/* Number of mailbox slots for each PDPS */
#define PA_NUM_MAILBOX_SLOTS                4
/**
 *  @brief
 *      Register access macros
 */
#define DEVICE_REG32_W(x,y)   *(volatile unsigned int *)(x)=(y)
#define DEVICE_REG32_R(x)    (*(volatile unsigned int *)(x))

#define BOOTBITMASK(x,y)      (   (   (  ((unsigned int)1 << (((unsigned int)x)-((unsigned int)y)+(unsigned int)1) ) - (unsigned int)1 )   )   <<  ((unsigned int)y)   )
#define BOOT_READ_BITFIELD(z,x,y)   (((unsigned int)z) & BOOTBITMASK(x,y)) >> (y)
#define BOOT_SET_BITFIELD(z,f,x,y)  (((unsigned int)z) & ~BOOTBITMASK(x,y)) | ( (((unsigned int)f) << (y)) & BOOTBITMASK(x,y) )



/******************************************************************************
 * board_delay_cycles
 ******************************************************************************/
void board_delay_cycles(uint64_t cycles)
{
    uint64_t start_val  = CSL_tscRead();
    while ((CSL_tscRead() - start_val) < cycles);
}

uint32_t l2_global_address (uint32_t addr)
{
    /* Check if the address is a valid Local L2 address to convert */
    if ((addr >= (uint32_t)0x800000)&&(addr < (uint32_t)0x880000))
    {
         /* Compute the global address. */
         return ((1 << 28) | (DNUM << 24) | (addr & 0x00ffffff));
    }
    else
    {
        return (addr);
    }
}



static void hwQmTeardown (void)
{
    unsigned int i;

    /* Linking RAM info */
    for (i = 0; i < DEVICE_QM_NUM_LINKRAMS; i++)
    {
        DEVICE_REG32_W (DEVICE_QM_MANAGER_BASE + QM_REG_LINKRAM_BASE(i), 0);
        DEVICE_REG32_W (DEVICE_QM_MANAGER_BASE + QM_REG_LINKRAM_SIZE(i), 0);
    }

    /* Memory region info */
    for (i = 0; i < DEVICE_QM_NUM_MEMREGIONS; i++)
    {
        DEVICE_REG32_W (DEVICE_QM_DESC_SETUP_BASE + QM_REG_MEMR_BASE_ADDR(i),  0);
        DEVICE_REG32_W (DEVICE_QM_DESC_SETUP_BASE + QM_REG_MEMR_START_IDX(i),  0);
        DEVICE_REG32_W (DEVICE_QM_DESC_SETUP_BASE + QM_REG_MEMR_DESC_SETUP(i), 0);
    }
    //empty all queues
    for(i=0; i<8192 ; i++)
        gpQueueManageRegs[i].REG_D_Descriptor= 0;

} /* hwQmTeardown */

/***************************************************************************************************
 * FUNCTION PURPOSE: Disable the transmit channels
 ***************************************************************************************************
 * DESCRIPTION: The transmit channels are disabled
 ***************************************************************************************************/
static void hwCpdmaTxDisable (void)
{
    unsigned int  i;
    unsigned int  v;

    for (i = 0; i < DEVICE_PA_CDMA_TX_NUM_CHANNELS; i++)
    {
        v = DEVICE_REG32_R (DEVICE_PA_CDMA_TX_CHAN_CFG_BASE + CPDMA_REG_TCHAN_CFG_REG_A(i));

        if ( (v & CPDMA_REG_VAL_TCHAN_A_TX_ENABLE) == CPDMA_REG_VAL_TCHAN_A_TX_ENABLE)  {
            v = v | CPDMA_REG_VAL_TCHAN_A_TX_TDOWN;
            DEVICE_REG32_W (DEVICE_PA_CDMA_TX_CHAN_CFG_BASE + CPDMA_REG_TCHAN_CFG_REG_A(i), v);
        }
    }
} /* hwCpdmaTxDisable */

/*********************************************************************************************
 * FUNCTION PURPOSE: Disable all rx channels and clear all the flow registers
 *********************************************************************************************
 * DESCRIPTION: The teardown is initiated and polled for completion. The function will
 *              return an error if the teardown is never complete, but will not stay
 *              in the function forever.
 *********************************************************************************************/
static void hwCpdmaRxDisable (void)
{
    unsigned int  i;
    unsigned int  v;

    for (i = 0; i < DEVICE_PA_CDMA_RX_NUM_CHANNELS; i++)
    {
        /* If enabled, set the teardown bit */
        v = DEVICE_REG32_R (DEVICE_PA_CDMA_RX_CHAN_CFG_BASE + CPDMA_REG_RCHAN_CFG_REG_A(i));
        if ( (v & CPDMA_REG_VAL_RCHAN_A_RX_ENABLE) == CPDMA_REG_VAL_RCHAN_A_RX_ENABLE )
        {
            v = v | CPDMA_REG_VAL_RCHAN_A_RX_TDOWN;
            DEVICE_REG32_W (DEVICE_PA_CDMA_RX_CHAN_CFG_BASE + CPDMA_REG_RCHAN_CFG_REG_A(i), v);
        }
    }
    board_delay_cycles(500000);
    /* Clear all of the flow registers */
    for (i = 0; i < DEVICE_PA_CDMA_RX_NUM_FLOWS; i++)
    {
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_A, i), 0);
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_B, i), 0);
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_C, i), 0);
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_D, i), 0);
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_E, i), 0);
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_F, i), 0);
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_G, i), 0);
        DEVICE_REG32_W (DEVICE_PA_CDMA_RX_FLOW_CFG_BASE + CPDMA_RX_FLOW_CFG(CPDMA_RX_FLOW_REG_H, i), 0);
    }
} /* hwCpdmaRxDisable */

/***********************************************************************************************
 * FUNCTION PURPOSE: Disable the PA firmware
 ***********************************************************************************************
 * DESCRIPTION: All PDSPs are disabled and all mailbox slots cleared
 ***********************************************************************************************/
static void hwPaDisable (void)
{
    unsigned int i, j;

    /* Disable all pdsps, clear all mailboxes */
    for (i = 0; i < DEVICE_PA_NUM_PDSPS; i++)
    {
        DEVICE_REG32_W (DEVICE_PA_BASE + PA_REG_PDSP_CTL(i), PA_REG_VAL_PDSP_CTL_DISABLE_PDSP);
        for (j = 0; j < PA_NUM_MAILBOX_SLOTS; j++)
            DEVICE_REG32_W (DEVICE_PA_BASE + PA_REG_MAILBOX_SLOT(i, j), 0);
    }
    /* Clear Packet ID */
    DEVICE_REG32_W (DEVICE_PA_BASE + PA_PID_RANGE_LIM, 0x3FF);
    DEVICE_REG32_W (DEVICE_PA_BASE + PA_PID_SOFT_RESET, 0x3FF);
} /* hwPaDisable */

static void interruptClear(void)
{
    //Hwi_disableInterrupt(ETH_INTERRUPT);
    //Hwi_disableInterrupt(SRIO_INTERRUPT);
    //Clear EVTFLAGn

    /* Clear INTD
     * PA accumulator channel 0 applies for this case
     * */

    *(unsigned int*)0x1800000 = 0xffffffff;
    *(unsigned int*)0x1800004 = 0xffffffff;
    *(unsigned int*)0x1800008 = 0xffffffff;
    *(unsigned int*)0x180000c = 0xffffffff;
    //Clear EVTSETn
    *(unsigned int*)0x1800020 = 0;
    *(unsigned int*)0x1800024 = 0;
    *(unsigned int*)0x1800028 = 0;
    *(unsigned int*)0x180002c = 0;
    //Clear EVTCLRn
    *(unsigned int*)0x1800040 = 0xffffffff;
    *(unsigned int*)0x1800044 = 0xffffffff;
    *(unsigned int*)0x1800048 = 0xffffffff;
    *(unsigned int*)0x180004C = 0xffffffff;
    /* Clear INTD
     * PA accumulator channel 0 applies for this case
     * */
    //Qmss_ackInterrupt(8+DNUM, 1);
    Qmss_ackInterrupt(DNUM, 0);
    //DEVICE_REG32_W(QM_INTD_CONFIG_REGION + 0x10, 0x00);
    Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, DNUM);
}


/**
 *  @brief
 *      Configure the sgmii Serdes on devices using the initialization sequence
 */
static void sgmii_initSerdes(void)
{
    CSL_SGMII_STATUS sgmii_status;

    /* Unlock the chip configuration registers to allow SGMII SERDES registers to
    * be written */
    CSL_BootCfgUnlockKicker();

    /* Configure the SERDES */
    /* Multiply to be 8 with Quarter Rate in the Rx registers */
    CSL_BootCfgSetSGMIIConfigPLL (0x00000041);

    board_delay_cycles(100);

    //31:25    Reserved    0000000
    //23:24    LOOPBACK    00
    //   22    ENOC        1
    //21:18    EQ          0001
    //17:15    CDR         001 -- first order threshold of 17
    //14:12    LOS         000 -- tie off
    //11:10    ALIGN       01  -- Comma Aligned
    //09:07    TERM        100 -- tie off (100)
    //   06    INVPAIR     0
    //05:04    RATE        01  -- tie off (10)  //00 = Full Rate, 01 = Half Rate (*0.5), 10 = Quarter Rate (*0.25)
    //03:01    BUSWIDTH    000 -- tie off
    //   00    ENRX        1
    // 0000 0000 0100 0100 0000 0010 0001 0001 = 0x0044_0211 -- My estimated value
    // 0000 0000 0100 0100 0000 0100 0001 0001 = 0x0044_0411 -- New DV value
    // 0000 0000 0000 1000 0000 1000 0100 0001 = 0x0008_0841 -- Original DV value

    CSL_BootCfgSetSGMIIRxConfig (0, 0x00700621);
    CSL_BootCfgSetSGMIIRxConfig (1, 0x00700621);

    //31:22    Reserved    0
    //21:20    LOOPBACK    00
    //19:18    RDTCT       00  -- tie off
    //   17    ENIDL       0   -- tie off
    //   16    MYSNC       1   -- tie off
    //15:12    DEMPHASIS   ???? - 0001 Lets give some de-emphasis
    //11:08    SWING       ????
    //   07    CM          1   -- tie off
    //   06    INVPAIR     0
    //05:04    RATE        01  -- tie off
    //03:01    BUSWIDTH    000 -- tie off
    //   00    ENTX        1
    // 0000 0000 0011 0001 ???? ???? 1001 0001 = 0x0031_1E91 -- My estimated value
    // 0000 0000 0000 0001 0000 1111 0001 0001 = 0x0001_0F11 -- New DV value
    // 0000 0000 0100 0000 0001 1110 0100 0001 = 0x0040_1e41 -- Original DV value
    CSL_BootCfgSetSGMIITxConfig (0, 0x000108A1);
    CSL_BootCfgSetSGMIITxConfig (1, 0x000108A1);

    /* Poll the SGMII0 lock bit to confirm that the sgmii module has recognized
    that the SERDES PLL has locked */
    do
    {
        CSL_SGMII_getStatus(0, &sgmii_status);
    } while (sgmii_status.bIsLocked != 1);

    /* Poll the SGMII1 lock bit to confirm that the sgmii module has recognized
    that the SERDES PLL has locked */
    do
    {
        CSL_SGMII_getStatus(1, &sgmii_status);
    } while (sgmii_status.bIsLocked != 1);
    /* All done with configuration. Return Now. */
    return;
}


/** ============================================================================
 *   @n@b Init_SGMII
 *
 *   @b Description
 *   @n SGMII peripheral initialization code.
 *
 *   @param[in]
 *   @n macPortNum      MAC port number for which the SGMII port setup must
 *                      be performed.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
static void Init_SGMII (uint32_t macPortNum)
{
    CSL_SGMII_ADVABILITY    sgmiiCfg;
    CSL_SGMII_STATUS        sgmiiStatus;

    /* Reset the port before configuring it */
    CSL_SGMII_doSoftReset (macPortNum);
#ifndef SGMII_LOOPBACK
    while (CSL_SGMII_getSoftResetStatus (macPortNum) != 0);

     if (macPortNum == 1) {
        /* Hold the port in soft reset and set up
         * the SGMII control register:
         *      (1) Disable Master Mode
         *      (2) Enable Auto-negotiation
         */
        CSL_SGMII_startRxTxSoftReset (macPortNum);
        CSL_SGMII_enableMasterMode (macPortNum);
        CSL_SGMII_disableAutoNegotiation (macPortNum);
        CSL_SGMII_endRxTxSoftReset (macPortNum);

        /* Setup the Advertised Ability register for this port:
         *      (1) Enable Full duplex mode
         *      (2) Enable Auto Negotiation
         *      (3) Enable the Link
         */
        sgmiiCfg.linkSpeed      =   CSL_SGMII_1000_MBPS;
        sgmiiCfg.duplexMode     =   CSL_SGMII_FULL_DUPLEX;
        CSL_SGMII_setAdvAbility (macPortNum, &sgmiiCfg);

        do
        {
            CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
        } while (sgmiiStatus.bIsLinkUp != 1);

        /* Wait for SGMII Autonegotiation to complete without error */
//        do
//        {
//            CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
//            if (sgmiiStatus.bIsAutoNegError != 0)
//                return; /* This is an error condition */
//        } while (sgmiiStatus.bIsAutoNegComplete != 1);
    }

    if (macPortNum == 0) {
        /* Hold the port in soft reset and set up
         * the SGMII control register:
         *      (1) Disable Master Mode
         *      (2) Enable Auto-negotiation
         */
        CSL_SGMII_startRxTxSoftReset (macPortNum);
        CSL_SGMII_enableMasterMode (macPortNum);
        CSL_SGMII_disableAutoNegotiation (macPortNum);
        CSL_SGMII_endRxTxSoftReset (macPortNum);

        /* Setup the Advertised Ability register for this port:
         *      (1) Enable Full duplex mode
         *      (2) Enable Auto Negotiation
         *      (3) Enable the Link
         */
        sgmiiCfg.linkSpeed      =   CSL_SGMII_1000_MBPS;
        sgmiiCfg.duplexMode     =   CSL_SGMII_FULL_DUPLEX;
        sgmiiCfg.bLinkUp        =   1;
        CSL_SGMII_setAdvAbility (macPortNum, &sgmiiCfg);

        do
        {
            CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
        } while (sgmiiStatus.bIsLinkUp != 1);
        /* Wait for SGMII Autonegotiation to complete without error */
//        do
//        {
//            CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
//            if (sgmiiStatus.bIsAutoNegError != 0)
//                return; /* This is an error condition */
//        } while (sgmiiStatus.bIsAutoNegComplete != 1);
     }
#else
    CSL_SGMII_startRxTxSoftReset (macPortNum);
    CSL_SGMII_enableMasterMode (macPortNum);
    CSL_SGMII_disableAutoNegotiation (macPortNum);
    CSL_SGMII_endRxTxSoftReset (macPortNum);

    /* Setup the Advertised Ability register for this port:
     *      (1) Enable Full duplex mode
     *      (2) Enable Auto Negotiation
     *      (3) Enable the Link
     */
    sgmiiCfg.linkSpeed      =   CSL_SGMII_1000_MBPS;
    sgmiiCfg.duplexMode     =   CSL_SGMII_FULL_DUPLEX;
    sgmiiCfg.bLinkUp        =   1;
    CSL_SGMII_setAdvAbility (macPortNum, &sgmiiCfg);
    CSL_SGMII_enableLoopback(macPortNum);
    /* All done with configuration. Return Now. */
#endif
    return;
}


///////////////////////////////////////////////////////////////////////////////////////
//extern function
///////////////////////////////////////////////////////////////////////////////////////
#ifdef RIO
extern void SRIO_Doorbell_ISR(UArg argument);

/**
 *  @b Description
 *  @n
 *      This is the Multicore Test task
 *
 *  @retval
 *      Not Applicable.
 */
static void srioDrvInit(void)
{
    Srio_DrvConfig  cfg;
    //Srio_DrvHandle  hSrioDriver;
    UInt8           isAllocated;

    /* Initialize the OSAL Data Buffer */
    if (Osal_sriodataBufferInitMemory(256) < 0)
    	return;
    /* Initialize the SRIO Driver Configuration. */
    memset ((Void *)&cfg, 0, sizeof(Srio_DrvConfig));

    /* Setup the SRIO Driver Managed Configuration. */
    cfg.bAppManagedConfig                   = FALSE;

    /* Driver Managed: Receive Configuration */
    cfg.u.drvManagedCfg.bIsRxCfgValid             = 1;
    cfg.u.drvManagedCfg.rxCfg.rxMemRegion         = Qmss_MemRegion_MEMORY_REGION1;
    cfg.u.drvManagedCfg.rxCfg.numRxBuffers        = 64;
    cfg.u.drvManagedCfg.rxCfg.rxMTU               = 256;

    /* Accumulator Configuration. */
    {
	    int32_t coreToQueueSelector[8];
        /* This is the table which maps the core to a specific receive queue. */
	    coreToQueueSelector[0] = 712;
	    coreToQueueSelector[1] = 713;
	    coreToQueueSelector[2] = 714;
	    coreToQueueSelector[3] = 715;
	    coreToQueueSelector[4] = 716;
	    coreToQueueSelector[5] = 717;
	    coreToQueueSelector[6] = 718;
	    coreToQueueSelector[7] = 719;

	    /* Since we are programming the accumulator we want this queue to be a HIGH PRIORITY Queue */
	    cfg.u.drvManagedCfg.rxCfg.rxCompletionQueue = Qmss_queueOpen (Qmss_QueueType_HIGH_PRIORITY_QUEUE,
                                                                         coreToQueueSelector[DNUM],
                                                                         &isAllocated);
		if (cfg.u.drvManagedCfg.rxCfg.rxCompletionQueue < 0)
		{
			printf ("Error: Unable to open the SRIO Receive Completion Queue\n");
			return;
		}

		/* Accumulator Configuration is VALID. */
		cfg.u.drvManagedCfg.rxCfg.bIsAccumlatorCfgValid = 1;

		/* Accumulator Configuration. */
	    cfg.u.drvManagedCfg.rxCfg.accCfg.channel             = 8+DNUM;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.command             = Qmss_AccCmd_ENABLE_CHANNEL;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.queueEnMask         = 0;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.queMgrIndex         = coreToQueueSelector[DNUM];
	    cfg.u.drvManagedCfg.rxCfg.accCfg.maxPageEntries      = 2;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.timerLoadCount      = 0;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.interruptPacingMode = Qmss_AccPacingMode_LAST_INTERRUPT;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.listEntrySize       = Qmss_AccEntrySize_REG_D;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.listCountMode       = Qmss_AccCountMode_ENTRY_COUNT;
	    cfg.u.drvManagedCfg.rxCfg.accCfg.multiQueueMode      = Qmss_AccQueueMode_SINGLE_QUEUE;

        /* Initialize the accumulator list memory */
        memset ((Void *)&gSrioHiPriAccumList[0], 0, sizeof(gSrioHiPriAccumList));
        cfg.u.drvManagedCfg.rxCfg.accCfg.listAddress = l2_global_address((UInt32)&gSrioHiPriAccumList[0]);
    }

    /* Driver Managed: Transmit Configuration */
    cfg.u.drvManagedCfg.bIsTxCfgValid             = 1;
    cfg.u.drvManagedCfg.txCfg.txMemRegion         = Qmss_MemRegion_MEMORY_REGION1;
    cfg.u.drvManagedCfg.txCfg.numTxBuffers        = 64;
    cfg.u.drvManagedCfg.txCfg.txMTU               = 256;

    /* Start the SRIO Driver */
    hSrioDriver = Srio_start(&cfg);
    if (hSrioDriver == NULL)
    {
    	printf ("Error: SRIO Driver Start Failed\n");
        return;
    }
    return;
}
#endif
#define SEGSIZE_256M   0x1B
static void initMPAX(Uint32 baddr, Uint32 raddr, Uint32 segsize, Uint32 index, Uint32 privid)
{
	CSL_MSMC_SESMPAXH mpaxh;
	CSL_MSMC_SESMPAXL mpaxl;

	if(CSL_MSMC_isSESLocked(privid))
		CSL_MSMC_unlockSES(privid);

    mpaxh.segSz = segsize;
    mpaxh.ai    = 0;
    mpaxh.be    = 0;
    mpaxh.baddr = baddr;
    CSL_MSMC_setSESMPAXH (privid, index, &mpaxh);

    mpaxl.ux = 1;
    mpaxl.uw = 1;
    mpaxl.ur = 1;
    mpaxl.sx = 1;
    mpaxl.sw = 1;
    mpaxl.sr = 1;
    mpaxl.raddr = raddr;

    CSL_MSMC_setSESMPAXL (privid, index, &mpaxl);
    CSL_MSMC_lockSES(privid);
}

/*CPSW Configuration Register*/
#define CPSW_CFG_REG    0x2000604
#define CPSW_CFG_PA_BYPASS 0x0606

/* High Priority Accumulation Interrupt Service Handler for this application */
void Config_Cpsw_Pa_Bypass(void)
{
    Int32 *regAddr;

    regAddr = (Int32 *)CPSW_CFG_REG;

    System_printf ("CPSW Configuration Register 0x%x \n",*regAddr);
    *regAddr = CPSW_CFG_PA_BYPASS;
    System_printf ("CPSW Configuration Register updated to 0x%x \n",*regAddr);

}

void hook_reset(void)
{

}
/*************************************************************************
 *  @b evm_init()
 *  @retval
 *      None
 ************************************************************************/
void evm_init(void)
{
	int32_t result;
    QMSS_CFG_T qmss_cfg;

    Qmss_MemRegInfo  memRegInfo;
    Cppi_DescCfg cppiDescCfg;
    uint32_t numAllocated;
    uint8_t isAllocated;
    int i;

    Cache_Size size;

    size.l1pSize = Cache_L1Size_32K;
    size.l1dSize = Cache_L1Size_32K;
    size.l2Size  = Cache_L2Size_512K;
    Cache_setSize(&size);

    CSL_tscEnable();
    initMPAX(SES_BADDR, SES_RADDR(DNUM), SES_SEGSIZE, 1, DNUM);
    if(DNUM == MAST_CORE)
    {
        gGlobalDataPtr->gGolbalInit = 0xaa55aa55;
        interruptClear();
        hwPaDisable();
        hwCpdmaRxDisable();
        hwCpdmaTxDisable();
        hwQmTeardown();
    	initMPAX(SES_BADDR, SES_RADDR(DNUM), SES_SEGSIZE, 1, 8);//netcp
		initMPAX(SES_BADDR, SES_RADDR(DNUM), SES_SEGSIZE, 1, 9);//srio
    	initMPAX(SES_BADDR, SES_RADDR(DNUM), SES_SEGSIZE, 1, 0x0a);//qmss
//    	initMPAX(SES_BADDR, SES_RADDR(DNUM), SES_SEGSIZE, 1, 0x0a);//qmss
    	initMPAX(SES_BADDR, SES_RADDR(DNUM), SES_SEGSIZE, 1, 0x0b);//debug
#ifdef PA_BYPASS
    /*Marvin RX channel No. 22 and No. 23 use to Ethernet A,B TX in packet streaming switch*/
    Config_Cpsw_Pa_Bypass();
#endif
	    sgmii_initSerdes();
	    Init_SGMII(0);
	    Init_SGMII(1);
		qmss_cfg.max_num_desc = 8192;
		if (res_mgr_init_qmss (&qmss_cfg) != 0)
		{
			printf("Error: qmss init failed\n");
		}

#ifdef ETH
		CPPI_CFG_T      cppi_cfg;
		cppi_cfg.dma_num            = Cppi_CpDma_PASS_CPDMA;
		cppi_cfg.num_tx_queues      = NUM_PA_TX_QUEUES;
		cppi_cfg.num_rx_channels    = NUM_PA_RX_CHANNELS;
		if (res_mgr_init_cppi (&cppi_cfg) != 0)
		{
			printf ("Failed to initialize CPPI subsystem \n");
		}
#ifndef PA_BYPASS
		if (res_mgr_init_pass()!= 0)
		{
			printf ("Failed to initialize the Packet Accelerator \n");
		}
#endif

        /* Initialize the Host Region. */
        memset ((void *)&gDescPtr->gNimuHostDesc, 0, sizeof(gDescPtr->gNimuHostDesc));
        Cache_wb((void *)gDescPtr->gNimuHostDesc, sizeof(gDescPtr->gNimuHostDesc), Cache_Type_L1D, CACHE_WAIT);
        Cache_wb((void *)gDescPtr->gNimuHostDesc, sizeof(gDescPtr->gNimuHostDesc), Cache_Type_L2, CACHE_WAIT);
        /* Memory Region Configuration */
        memRegInfo.descBase         = (uint32_t *)l2_global_address((uint32_t)gDescPtr->gNimuHostDesc);
        memRegInfo.descSize         = NIMU_MAX_DESC_SIZE;
        memRegInfo.descNum          = NIMU_NUM_DESC;
        memRegInfo.manageDescFlag   = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
        memRegInfo.memRegion        = Qmss_MemRegion_MEMORY_REGION0;
        memRegInfo.startIndex       = 0;
        /* Insert Host Descriptor memory region */
        result = Qmss_insertMemoryRegion (&memRegInfo);
        if (result == QMSS_MEMREGION_ALREADY_INITIALIZED)
        {
            printf ("Memory Region %d already Initialized \n", memRegInfo.memRegion);
        }
        else if (result < QMSS_SOK)
        {
            (printf ("Memory Region insert failed!\n"));
        }
        /* Initialize all the descriptors we just allocated on the
         * memory region above. Setup the descriptors with some well
         * known values before we use them for data transfers.
         */
        memset (&cppiDescCfg, 0, sizeof (cppiDescCfg));
        cppiDescCfg.memRegion       =   Qmss_MemRegion_MEMORY_REGION0;
        cppiDescCfg.descNum         =   NIMU_NUM_DESC;
        cppiDescCfg.destQueueNum    =   QMSS_PARAM_NOT_SPECIFIED;
        cppiDescCfg.queueType       =   Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
        cppiDescCfg.initDesc        =   Cppi_InitDesc_INIT_DESCRIPTOR;
        cppiDescCfg.descType        =   Cppi_DescType_HOST;
        /* By default:
         *      (1) Return descriptors to tail of queue
         *      (2) Always return entire packet to this free queue
         *      (3) Set that PS Data is always present in start of SOP buffer
         *      (4) Configure free q num < 4K, hence qMgr = 0
         *      (5) Recycle back to the same Free queue by default.
         */
        cppiDescCfg.returnPushPolicy            =   Qmss_Location_TAIL;
        cppiDescCfg.cfg.host.returnPolicy       =   Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;
        cppiDescCfg.cfg.host.psLocation         =   Cppi_PSLoc_PS_IN_DESC;
        cppiDescCfg.returnQueue.qMgr            =   0;
        cppiDescCfg.returnQueue.qNum            =   QMSS_PARAM_NOT_SPECIFIED;
        cppiDescCfg.epibPresent                 =   Cppi_EPIB_EPIB_PRESENT;
        /* Initialize the descriptors, create a free queue and push descriptors to a global free queue */
        if ((gGlobalDataPtr->gEthFreeQHnd = Cppi_initDescriptor (&cppiDescCfg, &numAllocated)) <= 0)
        {
            /* Error Initializing Free Descriptors, Error: %d \n", gGlobalFreeQHnd */
        }

#ifdef IPC
        /* Memory Region Configuration for IPC */
        memset (gDescPtr->gIPCDesc, 0, sizeof(gDescPtr->gIPCDesc) );
        Cache_wb((void *) gDescPtr->gIPCDesc, sizeof(gDescPtr->gIPCDesc), Cache_Type_L1D, CACHE_WAIT);
        Cache_wb((void *) gDescPtr->gIPCDesc, sizeof(gDescPtr->gIPCDesc), Cache_Type_L2, CACHE_WAIT);
        memRegInfo.descBase         = (uint32_t *)l2_global_address((uint32_t)gDescPtr->gIPCDesc);
        memRegInfo.descSize         = IPC_SIZE_DESC;
        memRegInfo.descNum          = IPC_NUM_DESC;
        memRegInfo.manageDescFlag   = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
        memRegInfo.memRegion        = Qmss_MemRegion_MEMORY_REGION1;
        memRegInfo.startIndex       = 128;
        /* Initialize and inset the memory region. */
        result = Qmss_insertMemoryRegion (&memRegInfo);
        if (result == QMSS_MEMREGION_ALREADY_INITIALIZED)
        {
            (printf("Memory Region %d already Initialized \n", memRegInfo.memRegion));
        }
        if (result < QMSS_SOK)
        {
            (printf ("Error inserting memory region for IPC!\n"));
        }

        for(i=0; i<8; i++)
        {
            gGlobalDataPtr->gIPCQHnd[i] = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated);
            gGlobalDataPtr->gIPCQHndPoll[i] = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated);
        }
        /* Setup the descriptors for receive free queue */
        memset (&cppiDescCfg, 0, sizeof (cppiDescCfg));

        cppiDescCfg.memRegion = Qmss_MemRegion_MEMORY_REGION1;
        cppiDescCfg.descNum = IPC_NUM_DESC;
        cppiDescCfg.destQueueNum = QMSS_PARAM_NOT_SPECIFIED;
        cppiDescCfg.queueType = Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
        cppiDescCfg.initDesc = Cppi_InitDesc_INIT_DESCRIPTOR;
        cppiDescCfg.descType = Cppi_DescType_MONOLITHIC;
        cppiDescCfg.epibPresent = Cppi_EPIB_EPIB_PRESENT;
        cppiDescCfg.returnQueue.qMgr =   0;
        cppiDescCfg.returnQueue.qNum =   QMSS_PARAM_NOT_SPECIFIED;
        cppiDescCfg.cfg.mono.dataOffset = 32;//need read doc
        /* Initialize the descriptors and push to monolithic free Queue */
        if ((gGlobalDataPtr->gIPCFreeQHnd  = Cppi_initDescriptor (&cppiDescCfg, &numAllocated)) < 0)
        {
            (printf ("Error Core %d : Initializing monolithic descriptor error code: %d \n", DNUM, gGlobalDataPtr->gIPCFreeQHnd ));
        }
#endif

#endif
#ifdef RIO
        initMPAX((SES_BADDR>>12), (0x80000000+((8+RIO_RUNING_CORE)<<24))>>8, SES_SEGSIZE, 1, 9);//srio
        /* Memory Region Configuration */
        memset (gSrioHostDesc, 0, SRIO_MAX_DESC_SIZE * SRIO_NUM_DESC );
        memRegInfo.descBase         = (uint32_t *)l2_global_address((uint32_t)gSrioHostDesc);
        memRegInfo.descSize         = SRIO_MAX_DESC_SIZE;
        memRegInfo.descNum          = SRIO_NUM_DESC;
        memRegInfo.manageDescFlag   = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
        memRegInfo.memRegion        = Qmss_MemRegion_MEMORY_REGION1;
        memRegInfo.startIndex       = 128;
        /* Initialize and inset the memory region. */
        result = Qmss_insertMemoryRegion (&memRegInfo);
        if (result == QMSS_MEMREGION_ALREADY_INITIALIZED)
        {
            (printf("Memory Region %d already Initialized \n", memRegInfo.memRegion));
        }
        if (result < QMSS_SOK)
        {
            (printf ("Error inserting memory region for SRIO!\n"));
        }
#endif
	}
    else
    {
        Cache_inv((void *)gGlobalDataPtr, sizeof(shareVal_t), Cache_Type_L1D, CACHE_WAIT);
        Cache_inv((void *)gGlobalDataPtr, sizeof(shareVal_t), Cache_Type_L2, CACHE_WAIT);
    	while (gGlobalDataPtr->gGolbalInit != 0x55aa55aa)
    	{
            Cache_inv((void *)gGlobalDataPtr, sizeof(shareVal_t), Cache_Type_L1D, CACHE_WAIT);
            Cache_inv((void *)gGlobalDataPtr, sizeof(shareVal_t), Cache_Type_L2, CACHE_WAIT);
    	}
        result = Qmss_start ();
        if(result != QMSS_SOK)
        	printf ("Core %d : Error starting Queue Manager error code : %d\n", DNUM, result);

    }
    ipc_init();
    if(DNUM == MAST_CORE)
    {
        gGlobalDataPtr->gGolbalInit = 0x55aa55aa;
        Cache_wbInv ((void *)gGlobalDataPtr, sizeof(shareVal_t), Cache_Type_L1D, CACHE_WAIT);
        Cache_wbInv ((void *)gGlobalDataPtr, sizeof(shareVal_t), Cache_Type_L2, CACHE_WAIT);
    }
}


void evm_init1(void)
{
	if(DNUM == RIO_RUNING_CORE)
	{
#ifdef RIO
		/* Power on SRIO peripheral before using it */
		KeyStone_enable_PSC_module(CSL_PSC_PD_SRIO, CSL_PSC_LPSC_SRIO);

		/* Device Specific SRIO Initializations: This should always be called before
		 * initializing the SRIO Driver. */
		if (SrioDevice_init () < 0)
		{
			(printf("Error: Srio device initialization faield!\n"));
		}
		/* Initialize the SRIO Driver */
		if (Srio_init () < 0)
		{
			(printf ("Error: SRIO Driver Initialization Failed\n"));
		}
		srioDrvInit();

		Hwi_Params hwiParams;
		Error_Block eb;
		Hwi_Params_init(&hwiParams);
		Error_init(&eb);
		hwiParams.arg = (UArg)hSrioDriver;
		hwiParams.eventId = SRIO_EVENTID;
		hwiParams.maskSetting = Hwi_MaskingOption_SELF;
		//Hwi_create(SRIO_INTERRUPT, (Hwi_FuncPtr)Srio_rxCompletionIsr, &hwiParams, &eb);
//		if(Error_check(&eb))
//		{
//			(printf("Srio_rxCompletion hwi inits failed!\n"));
//		}

		Hwi_Params_init(&hwiParams);
		Error_init(&eb);
	    CSL_SrioHandle hSrioCSL;
	    hSrioCSL = CSL_SRIO_Open (0);

		hwiParams.arg = (UArg)hSrioCSL;
		hwiParams.eventId = DOORBEL_EVENTID;
		hwiParams.maskSetting = Hwi_MaskingOption_SELF;
		Hwi_create(DOORBEL_INTERRUPT, (Hwi_FuncPtr)SRIO_Doorbell_ISR, &hwiParams, &eb);
		if(Error_check(&eb))
		{
			(printf("SRIO_Doorbell hwi init failed!\n"));
		}

		Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, 8+DNUM);

//	    if(gEvmc6678Info.chipnum == 0)
//	    {
//	    	rioSystemEnumerate(1);
//	    }
#if 0

	    CSL_SRIO_SetDeviceIDCSR (hSrioCSL, gEvmc6678Info.chipnum+1, 0xffff);
    	RioMaintWrite(hSrioDriver, RTE_DESTID, 0, 0xff, gEvmc6678Info.chipnum+1);
    	RioMaintWrite(hSrioDriver, RTE_PORT, 0, 0xff, gEvmc6678Info.chipnum+4);
        //Enable the port
    	RioMaintWrite(hSrioDriver, PORT_CTL + 0x20*(gEvmc6678Info.chipnum+4), 0, 0xff, 0xD0600001);
#endif
#if 0
    	uint32_t result;
	    CSL_SRIO_SetDeviceIDCSR (hSrioCSL, gEvmc6678Info.chipnum+1, 0xffff);
	    if(gEvmc6678Info.chipnum == 3)
	    {
	    	RioMaintWrite(hSrioDriver, RTE_DESTID, 0, 0xff, 0x04);
	    	RioMaintWrite(hSrioDriver, RTE_PORT, 0, 0xff, 0x04);
	        //Enable the port
	    	RioMaintWrite(hSrioDriver, PORT_CTL + 0x20*4, 0, 0xff, 0xD0600001);
	    	RioMaintRead(hSrioDriver, PORT_CTL + 0x20*4, 0, 0xff, &result);
	        printf("result is %x\n", result);
	    }else
	    {
	    	RioMaintWrite(hSrioDriver, RTE_DESTID, 0, 0xff, gEvmc6678Info.chipnum+1);
	    	RioMaintWrite(hSrioDriver, RTE_PORT, 0, 0xff, gEvmc6678Info.chipnum+5);
	        //Enable the port
	    	RioMaintWrite(hSrioDriver, PORT_CTL + 0x20*(gEvmc6678Info.chipnum+5), 0, 0xff, 0xD0600001);
	    }

	    if(gEvmc6678Info.chipnum == 0)
	    {
	    	RioMaintWrite(hSrioDriver, RTE_DESTID, 0, 0xff, 0xff);
	    	RioMaintWrite(hSrioDriver, RTE_PORT, 0, 0xff, 0x09);
	        //Enable the port
	    	RioMaintWrite(hSrioDriver, PORT_CTL + 0x20*0x09, 0, 0xff, 0xD0600001);
	    }
#endif
//	    if(gEvmc6678Info.Ga == 1)
	    	//CSL_SRIO_SetDeviceIDCSR (hSrioCSL, gEvmc6678Info.chipnum + 4, 0xffff);
//	    if(gEvmc6678Info.Ga == 2)
//	    	CSL_SRIO_SetDeviceIDCSR (hSrio, gEvmc6678Info.chipnum + 4, 0xffff);
#endif
	}

}




