/**
 *   @file  nimu_eth.c
 *
 *   @brief
 *  Ethernet Packet Driver rewritten using the NIMU Packet
 *  Architecture guidelines.
 *
 *  Note: The NDK nimu driver interface is built based on the examples from the
 *  PDK. Please refer to PDK examples
 *  (<pdk_install_dir>\packages\ti\drv\pa\example\emacExample) directory to get
 *  the knowledge on the QMSS, CPPI, PA LLD configurations/programs.
 *
 */
#include "drv.h"



#define MAX_CORES	8

int32_t Add_IPAddress (void);
uint32_t    coreKey [MAX_CORES];

#pragma DATA_SECTION(gCmdBuff, ".resmgr_memregion");
#pragma DATA_ALIGN(gCmdBuff, 128);
uint8_t gCmdBuff[8][512];






/**
 * @brief
 *   NIMUDeviceTable
 *
 * @details
 *  This is the NIMU Device Table for the Platform.
 *  This should be defined for each platform. Since the current platform
 *  has a single network Interface; this has been defined here. If the
 *  platform supports more than one network interface this should be
 *  defined to have a list of "initialization" functions for each of the
 *  interfaces.
 */

NIMU_DEVICE_TABLE_ENTRY NIMUDeviceTable[] =
{
/**
  * @brief  EmacInit for the platform
  */
    EmacInit,
    NULL
};

/**
 *  @b EmacStart
 *  @n
 *  The function is used to initialize and start the EMAC
 *  controller and device.
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int EmacStart
(
    NETIF_DEVICE*     ptr_net_device
)
{
    EMAC_DATA*      ptr_pvt_data;
#ifndef PA_BYPASS
    paMacAddr_t     broadcast_mac_addr  = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    paEthInfo_t     ethInfo             = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },     /* Src mac = dont care */
                                            { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 },     /* Default Dest mac */
                                              0,                                        /* vlan = dont care */
                                              0,                                        /* ignore ether type */
                                              0                                         /* MPLS tag = don't care */
                                          };
#endif

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA *)ptr_net_device->pvt_data;

	/* Setup Tx */
	if (Setup_Tx () != 0)
	{
		printf ("Tx setup failed \n");
		return -1;
	}
    /* Setup Rx */
    if (Setup_Rx (ptr_net_device) != 0)
    {
    	printf ("Rx setup failed \n");
        return -1;
    }
#ifndef PA_BYPASS
	memcpy (&ethInfo.dst[0],  ptr_pvt_data->pdi.bMacAddr,  sizeof(paMacAddr_t));
	if (Add_MACAddress (ethInfo, (uint8_t)Cppi_getFlowId(gGlobalDataPtr->gRxFlowHnd[DNUM])) != 0)
	{
		printf ("Add_MACAddress failed \n");
	}
	memcpy (&ethInfo.dst[0],  broadcast_mac_addr,  sizeof(paMacAddr_t));
	/* Set up the MAC Address LUT for Broadcast */
	if (Add_MACAddress (ethInfo, (uint8_t)Cppi_getFlowId(gGlobalDataPtr->gRxFlowHnd[DNUM])) != 0)
		printf ("Add_MACAddress failed \n");
#endif

    /* Copy the MAC Address into the network interface object here. */
    mmCopy(&ptr_net_device->mac_address[0], &ptr_pvt_data->pdi.bMacAddr[0], 6);

    /* Set the 'initial' Receive Filter */
    ptr_pvt_data->pdi.Filter = ETH_PKTFLT_MULTICAST;

    ptr_pvt_data->pdi.TxFree = 1;

    return 0;
}


/**
 *  @b EmacStop
 *  @n
 *  The function is used to de-initialize and stop the EMAC
 *  controller and device.
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int EmacStop(NETIF_DEVICE* ptr_net_device)
{
    EMAC_DATA*  ptr_pvt_data;
    uint16_t         count, i;
    Cppi_Desc*     pCppiDesc;
    uint8_t*         bufaddr;
    uint32_t         bufLen;

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA *)ptr_net_device->pvt_data;

    count = Qmss_getQueueEntryCount(gGlobalDataPtr->gRxQHnd[DNUM]);

    /* Free the packet data buffer associated with the Rx Descriptor */
    for (i=0; i < count; i++) {
        /* Need to free up the PBM handle */
        /* free the PBM packet */

        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP (gGlobalDataPtr->gRxQHnd[DNUM], QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc)) != NULL)
        {
            return -1;
        }

        /* Get the Address and Length for Free */
        Cppi_getData(Cppi_DescType_HOST, pCppiDesc, &bufaddr, &bufLen);
        Memory_free(0, (Ptr)*bufaddr, bufLen);
    }

    count = Qmss_getQueueEntryCount(gGlobalDataPtr->gRxFreeQHnd);
    /* Free the packet data buffer associated with the Rx Descriptor */
    for (i=0; i < count; i++) {
        /* Need to free up the PBM handle */
        /* free the PBM packet */

        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP (gGlobalDataPtr->gRxFreeQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc)) != NULL)
        {
            return -1;
        }

        /* Get the Address and Length for Free */
        Cppi_getData(Cppi_DescType_HOST, pCppiDesc, &bufaddr, &bufLen);
        Memory_free(0, (Ptr)*bufaddr, bufLen);
    }


    /* tear down queues and dmas */
    res_mgr_stop_qmss();
    Qmss_queueClose (gGlobalDataPtr->gPaCfgCmdRespQHnd);
    Qmss_queueClose (gGlobalDataPtr->gTxFreeQHnd);
    Qmss_queueClose (gGlobalDataPtr->gRxFreeQHnd);
    Qmss_queueClose (gGlobalDataPtr->gRxQHnd[DNUM]);
    Qmss_queueClose (gGlobalDataPtr->gTxReturnQHnd);

    for (i = 0; i < NUM_PA_TX_QUEUES; i++)
      Qmss_queueClose (gGlobalDataPtr->gPaTxQHnd[i]);

	res_mgr_stop_cppi(CPPI_CFG_PASS);

    /* Flush out our pending raw rx queue */
    while( PBMQ_count(&ptr_pvt_data->pdi.PBMQ_rawrx) )
        PBM_free( PBMQ_deq(&ptr_pvt_data->pdi.PBMQ_rawrx) );

    /* Flush out our pending queue */
    while( PBMQ_count(&ptr_pvt_data->pdi.PBMQ_rx) )
        PBM_free( PBMQ_deq(&ptr_pvt_data->pdi.PBMQ_rx) );


    /* EMAC Controller has been stopped. */
    return 0;
}
#ifndef SIMULATOR_SUPPORT
/**
 *  @b emac_display_linkstatus
 *  @n
 *      This function is called whenever there is a change in link state on
 *      master core.
 *
 *  @param[in]  port_num
 *      EMAC port number.
 *  @param[in]  link_status
 *      Status of the link.
 *
 *  @retval
 *      void
 */
void emac_display_linkstatus(uint32_t port_num, uint32_t link_status)
{
    /* This string array corresponds to link state as defined in csl_mdio.h */
    char *LinkStr[] = { "No Link",
                        "10Mb/s Half Duplex",
                        "10Mb/s Full Duplex",
                        "100Mb/s Half Duplex",
                        "100Mb/s Full Duplex",
                        "1000Mb/s Full Duplex" };

    printf("Port %d Link Status: %s on PHY %d\n", port_num, LinkStr[link_status]);
}
#endif

/**
 *  @b EmacPoll
 *  @n
 *  The function is used to poll the EMAC controller to check
 *  if there has been any activity
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *  @param[in]  timer_tick
 *      Flag to poll the driver.
 *
 *  @retval
 *      void
 */
static void EmacPoll (NETIF_DEVICE* ptr_net_device, uint timer_tick)
{
    return;
}


/**
 *  @b EmacSend
 *  @n
 *  The function is the interface routine invoked by the NDK stack to
 *  pass packets to the driver.
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *  @param[in]  hPkt
 *      Packet to be sent out on wire.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
#define DEBUG_TX_DESC 0

#if DEBUG_TX_DESC
#pragma DATA_SECTION(TxDescArray, ".DbgSection");
Cppi_HostDesc   TxDescArray[NUM_TX_DESC];
int             TxDescArrayIndex = 0;
int             gtotal_pkts_freed = 0;
#pragma DATA_SECTION(DescIndexArray, ".DbgSection");
int32_t DescIndexArray[NIMU_NUM_TX_DESC];
int32_t gDescTest = 0;
printHostDesc (Cppi_HostDesc*      pHostDesc)
{
   printf (" pHostDesc->buffLen       =%ld \n"      ,    pHostDesc->buffLen       );
   printf (" pHostDesc->buffPtr       =%ld \n"      ,    pHostDesc->buffPtr       );
   printf (" pHostDesc->descInfo      =%ld \n"      ,    pHostDesc->descInfo    );
   printf (" pHostDesc->nextBDPtr     =%ld \n"      ,    pHostDesc->nextBDPtr   );
   printf (" pHostDesc->origBufferLen =%ld \n"      ,    pHostDesc->origBufferLen);
   printf (" pHostDesc->origBuffPtr   =%ld \n"      ,    pHostDesc->origBuffPtr  );
   printf (" pHostDesc->packetInfo    =%ld \n"      ,    pHostDesc->packetInfo   );
   printf (" pHostDesc->psData        =%ld \n"      ,    pHostDesc->psData       );
   printf (" pHostDesc->softwareInfo0 =%ld \n"      ,    pHostDesc->softwareInfo0);
   printf (" pHostDesc->softwareInfo1 =%ld \n"      ,    pHostDesc->softwareInfo1);
   printf (" pHostDesc->softwareInfo2 =%ld \n"      ,    pHostDesc->softwareInfo2);
   printf (" pHostDesc->tagInfo       =%ld \n"      ,    pHostDesc->tagInfo      );
   printf (" pHostDesc->timeStamp     =%ld \n"      ,    pHostDesc->timeStamp    );
}
#endif
uint64_t gSendTick[1000];
static int EmacSend (NETIF_DEVICE* ptr_net_device, PBM_Handle hPkt)
{
    EMAC_DATA*          ptr_pvt_data;
    Cppi_Desc*          pCppiDesc;
    uint32_t            dataBufferSize, i,  count=0;
    register uint8_t*   buffer;
    register uint32_t   length;
    Cppi_HostDesc*      pHostDesc;
#if DEBUG_TX_DESC
    uint32_t 			j,matchFound;
    Cppi_HostDesc*    	pCppiDesc_dbg;
#endif
    uint32_t			coreNum;
    Ptr					key;
    static uint64_t		times=0;
    static int			cnts=0;

    /* Begin Critical Section before accessing shared resources. */
    key = Osal_cppiCsEnter();

    gSendTick[cnts++%1000] = CSL_tscRead()-times;
    times = CSL_tscRead();
    /* Disable interrupt */
    coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)] = Hwi_disableInterrupt(ETH_INTERRUPT);
    /* Get the core number. */
#ifdef SIMULATOR_SUPPORT
    coreNum = CSL_chipReadDNUM();
#else
    coreNum = CSL_chipReadDNUM();
#endif

    count = Qmss_getQueueEntryCount (gGlobalDataPtr->gTxReturnQHnd);

    /* Check if we got to free up any PBM handles? */
    for (i = 0; i < count; i++)  {
        PBM_Handle                  hPkt_to_free;
#if DEBUG_TX_DESC
        gtotal_pkts_freed ++;
#endif
        /* Need to free up the PBM handle */
        /* free the PBM packet */

        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP (gGlobalDataPtr->gTxReturnQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc)) != NULL)
        {
            goto error_cond;
        }
        pHostDesc = (Cppi_HostDesc *)pCppiDesc;

        /* Software info is inteded to hold the PBM Pkt Handle for buffer management */
        hPkt_to_free = (PBM_Handle ) pHostDesc->softwareInfo0;

#if DEBUG_TX_DESC
              matchFound = 0; TxDescArrayIndex = NIMU_NUM_TX_DESC;
              for (j = 0; j < NIMU_NUM_TX_DESC; j++ ) {
                if ( (int32_t) pHostDesc == DescIndexArray[j]) {
                    matchFound = 1;
                    break;
                }
              }
              if (matchFound)
                 TxDescArrayIndex              = j;

              if (TxDescArrayIndex >= 0 || TxDescArrayIndex < NIMU_NUM_TX_DESC) {


                    if ((memcmp ((void *) &TxDescArray[TxDescArrayIndex], (void *) pHostDesc, sizeof (pHostDesc))) != 0) {
                    	printf ("EmacSend: TxDescriptor for the index = %d did not match - Possible Corruption \n", TxDescArrayIndex);
                    	printf ("Completed descriptor received is \n");
                        printHostDesc(pHostDesc);
                        printf ("Tx descriptor Sent is \n");
                        printHostDesc(&TxDescArray[TxDescArrayIndex]);

                    } else {
                        pHostDesc->softwareInfo2 = TxDescArray[TxDescArrayIndex].softwareInfo2 = 0;
                    }
              } else {
            	  printf ("EmacSend: TxDescArrayIndex is out of range during PBM Free \n");
              }
#endif
        /* Clear the PBM Handle set in the descriptor  */
        pHostDesc->softwareInfo0 = NULL;

        /* Push descriptor to Tx free queue */
        QMSS_QPUSH     (gGlobalDataPtr->gTxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);

        PBM_free( (PBM_Handle) hPkt_to_free);
    }

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA *)ptr_net_device->pvt_data;

    /* Make sure the driver does not transmit packet less than min. as per the
     * Ethernet standards. */
    if( PBM_getValidLen(hPkt) < 60 )
        PBM_setValidLen (hPkt, 64 );

    /* We do not do any packet size checks here. If the packet
     * is too big to fit in the MTU configured on the peripheral,
     * then the driver/CSL layer should catch the error.
     */
    if(1)
    {
        /* Peek into the packet to check out if any
         * prioritization is needed.
         *
         * All raw Ethernet packets are tagged with the EMAC
         * channel number onto which they need to be sent out
         * in the PktPriority field.
         */
        if (((PBM_Pkt *)hPkt)->PktPriority != PRIORITY_UNDEFINED)
        {
            /* PktPriority contains the EMAC channel number on which
             * the packet needs to be txed.
             */
            ptr_pvt_data->pdi.PhysIdx = (((PBM_Pkt *)hPkt)->PktPriority);
        }
        else
        {
            /* This is just a normal IP packet. Enqueue the packet in the
             * Tx queue and send it for transmission.
             * We are assuming here that the IP packets are transmitted
             * on Channel 0 -> Core 0; Channel 1 --> Core 1; Channel 2 --> Core 2.
             * and that the Raw packets can be transmitted on any channel.
             */
                ptr_pvt_data->pdi.PhysIdx = coreNum;
        }

        /* Pass the packet to the controller if the transmitter is free. */
        if(ptr_pvt_data->pdi.TxFree )
        {
            buffer = PBM_getDataBuffer(hPkt) + PBM_getDataOffset(hPkt);
            length = PBM_getValidLen(hPkt);
              /* Clean the cache for external/L2 addesses */
              if( ((uint32_t)buffer & EMAC_EXTMEM  ) ||
                  ((uint32_t)buffer & EMAC_LL2SRAM ) ) {
                  OEMCacheClean( (void *)buffer, length );
              }

              /* Get a free descriptor from the global free queue we setup
               * during initialization.
               */
              if ((QMSS_QPOP (gGlobalDataPtr->gTxFreeQHnd, QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc)) != NULL)
              {
                    gTxDropCounter++;
                    goto error_cond;
              }

              pHostDesc = (Cppi_HostDesc *)pCppiDesc;

              pHostDesc->softwareInfo0 = (uint32_t) hPkt;

              dataBufferSize  =   length;
              Cppi_setData (  Cppi_DescType_HOST,
                              (Cppi_Desc *) pCppiDesc,
                              (uint8_t *) l2_global_address((uint32_t)buffer),
                              dataBufferSize
                           );
              Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pCppiDesc, dataBufferSize);

              count = Qmss_getQueueEntryCount (gGlobalDataPtr->gTxReturnQHnd);

#if DEBUG_TX_DESC
              matchFound = 0; TxDescArrayIndex = NIMU_NUM_TX_DESC;
              for (j = 0; j < NIMU_NUM_TX_DESC; j++ ) {
                if ( (int32_t) pHostDesc == DescIndexArray[j]) {
                    matchFound = 1;
                    break;
                }
              }
              if (matchFound)
                 TxDescArrayIndex              = j;

              if (TxDescArrayIndex >= 0 || TxDescArrayIndex < NIMU_NUM_TX_DESC) {
                    pHostDesc->softwareInfo2      = 1;
                    TxDescArray[TxDescArrayIndex] = *pHostDesc;
              } else {
            	  printf ("EmacSend: TxDescArrayIndex is out of range \n");
              }
#endif

#ifdef PA_LOOPBACK
              QMSS_QPUSH (gGlobalDataPtr->gPaTxQHnd[0], pCppiDesc, dataBufferSize, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);
#else
              Cppi_setPSFlags (Cppi_DescType_HOST, (Cppi_Desc *)pCppiDesc, 0x1);
              /* Send the packet out the mac. It will loop back to PA if the mac/switch
               * have been configured properly
               */
              QMSS_QPUSH (gGlobalDataPtr->gPaTxQHnd[8], pCppiDesc, dataBufferSize, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);
#endif

              /* Increment the application transmit counter */
              gTxCounter ++;

#if DEBUG_TX_DESC
              if (gDescTest == 1) {
                  if ((QMSS_QPOP (gGlobalDataPtr->gPaTxQHnd[8], QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc_dbg)) != NULL)
                   {
                	  printf ("Warning: descriptor did not picked up \n");
                   }
              }
#endif
        }

        /* Packet has been successfully transmitted. */
        /* End Critical Section */
        Hwi_restoreInterrupt(ETH_INTERRUPT, coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)]);
        Osal_cppiCsExit (key);
        return 0;
    }

error_cond:
    {
        /* End Critical Section */
        Hwi_restoreInterrupt(ETH_INTERRUPT, coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)]);
        Osal_cppiCsExit (key);
        return -1;
    }
}


/**
 *  @b EmacPktService
 *  @n
 *  The function is called by the NDK core stack to receive any packets
 *  from the driver.
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *
 *  @retval
 *      void
 */
int gPBMQcnt;
static void EmacPktService (NETIF_DEVICE* ptr_net_device)
{
    EMAC_DATA*  ptr_pvt_data;
    PBM_Handle  hPacket;
    static uint64_t a;

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA *)ptr_net_device->pvt_data;

    /* Give all queued Raw packets first to the Ether module */
    while (PBMQ_count(&ptr_pvt_data->pdi.PBMQ_rawrx))
    {
        /* Dequeue a packet from the driver's Raw receive queue. */
        hPacket = PBMQ_deq(&ptr_pvt_data->pdi.PBMQ_rawrx);

        /* Prepare the packet so that it can be passed up the networking stack.
         * If this 'step' is not done the fields in the packet are not correct
         * and the packet will eventually be dropped.  */
        PBM_setIFRx (hPacket, ptr_net_device);

        /* Pass the packet to the NDK Core stack. */
        NIMUReceivePacket(hPacket);
    }

    /* Give all queued IP packets to the Ether module */
    while (gPBMQcnt = PBMQ_count(&ptr_pvt_data->pdi.PBMQ_rx))
    {
//        System_printf("%d, %d\n", gPBMQcnt, (uint32_t)(CSL_tscRead()-a));
//        a = CSL_tscRead();
        /* Dequeue a packet from the driver receive queue. */
        hPacket = PBMQ_deq(&ptr_pvt_data->pdi.PBMQ_rx);

        /* Prepare the packet so that it can be passed up the networking stack.
         * If this 'step' is not done the fields in the packet are not correct
         * and the packet will eventually be dropped.  */
        PBM_setIFRx (hPacket, ptr_net_device);

        /* Pass the packet to the NDK Core stack. */
        NIMUReceivePacket(hPacket);
    }

    /* Work has been completed; the receive queue is empty... */
    return;
}


/**
 *  @b Emacioctl
 *  @n
 *  The function is called by the NDK core stack to configure the
 *  driver
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *  @param[in]  cmd
 *      Ioctl command.
 *  @param[in]  pBuf
 *      Mac address to be added or deleted.
 *  @param[in]  size
 *      Size of Mac Address.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int Emacioctl (NETIF_DEVICE* ptr_net_device, uint cmd, void* pBuf, uint size)
{
    return 0;
}


/**
 *  @b EmacInit
 *  @n
 *  The function is used to initialize and register the EMAC
 *  with the Network Interface Management Unit (NIMU)
 *
 *  @param[in]  hEvent
 *      Stack Event Handle.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int EmacInit (STKEVENT_Handle hEvent)
{
    EMACInit_Core(hEvent);

    return 0;
}

static int EMACInit_Core (STKEVENT_Handle hEvent)
{
    NETIF_DEVICE*       ptr_device;
    EMAC_DATA*  ptr_pvt_data;
    char names[8][5]={"eth0","eth1","eth2","eth3","eth4","eth5","eth6","eth7"};
    uint32_t coreNum;
    uint32_t mac_addr2, mac_addr1;

    /* Get the core number. */
#ifdef SIMULATOR_SUPPORT
    coreNum = CSL_chipReadDNUM();
#else
    coreNum = CSL_chipReadDNUM();
#endif

    /* Allocate memory for the private data */
    ptr_pvt_data    = Osal_nimuMalloc (platform_roundup(sizeof(EMAC_DATA), PLATFORM_CACHE_LINE_SIZE), PLATFORM_CACHE_LINE_SIZE);

    if (ptr_pvt_data == NULL)
    {
    	printf ("Error: Unable to allocate private memory data\n");
        return -1;
    }

    /* Initialize the allocated memory block. */
    mmZeroInit (ptr_pvt_data, sizeof(EMAC_DATA));

    /* Initialize the RX Queue */
    PBMQ_init(&ptr_pvt_data->pdi.PBMQ_rx);
    PBMQ_init(&ptr_pvt_data->pdi.PBMQ_rawrx);

    /* Initialize the private data */
    mmZeroInit(&ptr_pvt_data->pdi, sizeof(PDINFO));

    /* Set physical index */
    ptr_pvt_data->pdi.PhysIdx = coreNum;
    ptr_pvt_data->pdi.hEvent  = hEvent;

    /* MCast List is EMPTY */
    ptr_pvt_data->pdi.MCastCnt    = 0;

        /* Can not get MAC addr from efuse, set default MAC Address */
    CSL_BootCfgGetMacIdentifier(&mac_addr1, &mac_addr2);
    ptr_pvt_data->pdi.bMacAddr[0] = coreNum;//((mac_addr2 & 0x0000ff00) >> 8);
    ptr_pvt_data->pdi.bMacAddr[1] =  (mac_addr2 & 0x000000ff);
    ptr_pvt_data->pdi.bMacAddr[2] = ((mac_addr1 & 0xff000000) >> 24);
    ptr_pvt_data->pdi.bMacAddr[3] = ((mac_addr1 & 0x00ff0000) >> 16);
    ptr_pvt_data->pdi.bMacAddr[4] = ((mac_addr1 & 0x0000ff00) >> 8);
    ptr_pvt_data->pdi.bMacAddr[5] =  (mac_addr1 & 0x000000ff);

    /* Init Logical Device */
    /* ptr_pvt_data->pdi.hEther = hEther; */

    /* Allocate memory for the EMAC. */
    ptr_device    = Osal_nimuMalloc (platform_roundup(sizeof(NETIF_DEVICE), PLATFORM_CACHE_LINE_SIZE), PLATFORM_CACHE_LINE_SIZE);

    if (ptr_device == NULL)
    {
    	printf ("Error: Unable to allocate memory for the EMAC\n");
        return -1;
    }

    /* Initialize the allocated memory block. */
    mmZeroInit (ptr_device, sizeof(NETIF_DEVICE));

    /* Populate the Network Interface Object. */
    strcpy (ptr_device->name, names[coreNum]);
    ptr_device->mtu         = ETH_MAX_PAYLOAD - ETHHDR_SIZE;
    ptr_device->pvt_data    = (void *)ptr_pvt_data;

    /* Populate the Driver Interface Functions. */
    ptr_device->start       = EmacStart;
    ptr_device->stop        = EmacStop;
    ptr_device->poll        = EmacPoll;
    ptr_device->send        = EmacSend;
    ptr_device->pkt_service = EmacPktService;
    ptr_device->ioctl       = Emacioctl;
    ptr_device->add_header  = NIMUAddEthernetHeader;

	/* Init PA LLD */
	if (Init_PASS () != 0)
	{
		printf ("PASS init failed \n");
		return -1;
	}
	/* Initialize the CPSW switch */
	if (Init_Cpsw ((uint32_t) ptr_device->mtu,  ptr_pvt_data->pdi.bMacAddr) != 0)
	{
		printf ("Ethernet subsystem init failed \n");
		return -1;
	}
    /* Register the device with NIMU */
    if (NIMURegister (ptr_device) < 0)
    {
    	printf ("Error: Unable to register the EMAC\n");
        return -1;
    }

   // printf ("Registration of the EMAC Successful, waiting for link up ..\n");
    return 0;

}


/** ============================================================================
 *   @n@b Setup_Tx
 *
 *   @b Description
 *   @n This API sets up all relevant data structures and configuration required
 *      for sending data to PASS/Ethernet. It sets up a Tx free descriptor queue,
 *      PASS Tx queues required for send.
 *
 *   @param[in]
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Setup_Tx (void)
{
    uint8_t			isAllocated, i;
    Qmss_Queue      qRetInfo;
    Ptr             pCppiDesc;
    uint32_t        mySWInfo[] = {0x00000000, 0x00000000, 0x00000000};

	/* Open all Transmit (Tx) queues.
	 *
	 * These queues are used to send data to PA PDSP/CPSW.
	 */
	for (i = 0; i < NUM_PA_TX_QUEUES; i ++)
	{

		if ((gGlobalDataPtr->gPaTxQHnd[i] = Qmss_queueOpen (Qmss_QueueType_PASS_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
		{
			printf ("Error opening PA Tx queue \n");
			return -1;
		}
	}

	/* Open a Tx Free Descriptor Queue (Tx FDQ).
	 *
	 * This queue will be used to hold Tx free decriptors that can be filled
	 * later with data buffers for transmission onto wire.
	 */
	if ((gGlobalDataPtr->gTxFreeQHnd = Qmss_queueOpen (Qmss_QueueType_STARVATION_COUNTER_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
	{
		printf ("Error opening Tx Free descriptor queue \n");
		return -1;
	}

	/* Open a Tx Return Descriptor Queue (Tx Transmit CompletionQ).
	 *
	 * This queue will be used to hold Tx completed decriptors that can be filled
	 * later with data buffers for transmission onto wire.
	 */
	if ((gGlobalDataPtr->gTxReturnQHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
	{
		printf ("Error opening Tx Return descriptor queue \n");
		return -1;
	}

	qRetInfo = Qmss_getQueueNumber (gGlobalDataPtr->gTxReturnQHnd);

	/* Attach some free descriptors to the Tx free queue we just opened. */
	for (i = 0; i < NIMU_NUM_TX_DESC; i++)
	{
		/* Get a free descriptor from the global free queue we setup
		 * during initialization.
		 */
		if ((QMSS_QPOP (getNimuFreequeue(), QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc )) != NULL)
		{
			break;
		}

		/* Setup the Completion queue:
		 *
		 * Setup the return policy for this desc to return to the completion q we just
		 * setup instead of the global free queue.
		 */
#if DEBUG_TX_DESC
		DescIndexArray[i] =  (int32_t) pCppiDesc;
#endif
		Cppi_setReturnQueue ((Cppi_DescType) Cppi_DescType_HOST, pCppiDesc, qRetInfo);

		/* Software info is inteded to hold the PBM Pkt Handle for buffer management */
		mySWInfo[1] = i;
		Cppi_setSoftwareInfo (Cppi_DescType_HOST, pCppiDesc, (uint8_t *) mySWInfo);

		/* Push descriptor to Tx free queue */
		QMSS_QPUSHDESCSIZE (gGlobalDataPtr->gTxFreeQHnd, pCppiDesc, NIMU_MAX_DESC_SIZE);
	}
	if (i != NIMU_NUM_TX_DESC)
	{
		printf ("Error allocating Tx free descriptors \n");
		return -1;
	}
    /* All done with Rx configuration. Return success. */
    return 0;
}

/** ============================================================================
 *   @n@b Setup_Rx
 *
 *   @b Description
 *   @n This API sets up all relevant data structures and configuration required
 *      for receiving data from PASS/Ethernet. It sets up a Rx free descriptor queue
 *      with some empty pre-allocated buffers to receive data, and an Rx queue
 *      to which the Rxed data is streamed for the example application. This API
 *      also sets up the QM high priority accumulation interrupts required to
 *      receive data from the Rx queue.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Setup_Rx (NETIF_DEVICE*     ptr_net_device)
{
    int32_t				result;
    uint8_t             isAllocated, accChannelNum, i;
    uint16_t			numAccEntries, intThreshold, pktLen;
    Qmss_Queue          rxFreeQInfo, rxQInfo;
    Ptr                 pCppiDesc;
    Qmss_AccCmdCfg      accCfg;
    Cppi_RxFlowCfg      rxFlowCfg;
    Ptr                 pDataBuffer;
    uint32_t            mySWInfo[] = {0x11112222, 0x33334444};
    uint32_t            ListAddress;

	pktLen = ptr_net_device->mtu + ETHHDR_SIZE + 4; /* ETh Header + 4 bytes of FCS */

	//i           = MAX_HI_PRI_ACCUM_LIST_SIZE - 1 - (RX_INT_THRESHOLD+1);
	ListAddress = l2_global_address((uint32_t) &gAccumListPtr->gEthHiPriAccumList[DNUM][0]);

	if ((gGlobalDataPtr->gRxQHnd[DNUM] = Qmss_queueOpen (Qmss_QueueType_HIGH_PRIORITY_QUEUE, QMSS_HIGH_PRIORITY_QUEUE_BASE+DNUM, &isAllocated)) < 0)
	{
		printf ("Error opening a High Priority Accumulation Rx queue \n");
		return -1;
	}


	/* Setup high priority accumulation interrupts on the Rx queue.
	 *
	 * Let's configure the accumulator with the following settings:
	 *      (1) Interrupt pacing disabled.
	 *      (2) Interrupt on every received packet
	 */
	intThreshold    =   RX_INT_THRESHOLD;
	numAccEntries   =   (intThreshold + 1) * 2;
	accChannelNum   =   PA_ACC_CHANNEL_NUM;

	/* Initialize the accumulator list memory */
	memset ((void *) ListAddress, 0, numAccEntries * 4);

	/* Ensure that the accumulator channel we are programming is not
	 * in use currently.
	 */
	result = Qmss_disableAccumulator (Qmss_PdspId_PDSP1, accChannelNum);
	if (result != QMSS_ACC_SOK && result != QMSS_ACC_CHANNEL_NOT_ACTIVE)
	{
		printf ("Error Disabling high priority accumulator for channel : %d error code: %d\n",
					  accChannelNum, result);
		return -1;
	}

	if(Qmss_configureAccTimer (Qmss_PdspId_PDSP1, 175) != QMSS_ACC_SOK)
		printf("Qmss_configureAccTimer failed!\n");

	/* Setup the accumulator settings */
	accCfg.channel             =   accChannelNum;
	accCfg.command             =   Qmss_AccCmd_ENABLE_CHANNEL;
	accCfg.queueEnMask         =   0;
	accCfg.listAddress         =   ListAddress;
	accCfg.queMgrIndex         =   gGlobalDataPtr->gRxQHnd[DNUM];
	accCfg.maxPageEntries      =   (intThreshold + 1); /* Add an extra entry for holding the entry count */
	accCfg.timerLoadCount      =   125;//80-----800us
	accCfg.interruptPacingMode =   Qmss_AccPacingMode_FIRST_NEW_PACKET;
	accCfg.listEntrySize       =   Qmss_AccEntrySize_REG_D;
	accCfg.listCountMode       =   Qmss_AccCountMode_ENTRY_COUNT;
	accCfg.multiQueueMode      =   Qmss_AccQueueMode_SINGLE_QUEUE;

	/* Program the accumulator */
	if ((result = Qmss_programAccumulator (Qmss_PdspId_PDSP1, &accCfg)) != QMSS_ACC_SOK)
	{
		printf ("Error Programming high priority accumulator for channel : %d queue : %d error code : %d\n",
						accCfg.channel, accCfg.queMgrIndex, result);
		return -1;
	}
    /* Register interrupts for the system event corresponding to the
     * accumulator channel we are using.
     */
	Hwi_Params hwiParams;
	Error_Block eb;
	Hwi_Params_init(&hwiParams);
	Error_init(&eb);
	hwiParams.arg = (UArg)ptr_net_device;
	hwiParams.eventId = ETH_EVENTID;
	hwiParams.maskSetting = Hwi_MaskingOption_SELF;
	Hwi_create(ETH_INTERRUPT, (Hwi_FuncPtr)EmacRxPktISR, &hwiParams, &eb);
	if(Error_check(&eb))
	{
		(printf("ipc hwi inits failed!\n"));
	}

	/* Open a Rx Free Descriptor Queue (Rx FDQ).
	 *
	 * This queue will hold all the Rx free descriptors. These descriptors will be
	 * used by the PASS CPDMA to hold data received via CPSW.
	 */
	if ((gGlobalDataPtr->gRxFreeQHnd = Qmss_queueOpen (Qmss_QueueType_STARVATION_COUNTER_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
	{
		printf ("Error opening Rx Free descriptor queue \n");
		return -1;
	}
	rxFreeQInfo = Qmss_getQueueNumber (gGlobalDataPtr->gRxFreeQHnd);

	/* Attach some free descriptors to the Rx free queue we just opened. */
	for (i = 0; i < NIMU_NUM_RX_DESC; i++)
	{
		/* Get a free descriptor from the global free queue we setup
		 * during initialization.
		 */
		if ((QMSS_QPOP (getNimuFreequeue(), QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc **)&pCppiDesc)) != NULL)
		{
			break;
		}

		pktLen = platform_roundup(pktLen, PLATFORM_CACHE_LINE_SIZE);
		if ((pDataBuffer = Osal_nimuMalloc (pktLen, PLATFORM_CACHE_LINE_SIZE)) == NULL)
		{
			printf ("Error allocating memory for Rx data buffer \n");
			break;
		}

		/* Populate the Rx free descriptor with the buffer we just allocated. */
		Cppi_setData (Cppi_DescType_HOST, pCppiDesc, (uint8_t *)l2_global_address((uint32_t)pDataBuffer), pktLen);

		/* Save original buffer information */
		Cppi_setOriginalBufInfo (Cppi_DescType_HOST, pCppiDesc, (uint8_t *)l2_global_address((uint32_t)pDataBuffer), pktLen);

		/* Setup the Completion queue:
		 *
		 * Setup the return policy for this desc to return to the free q we just
		 * setup instead of the global free queue.
		 */
		Cppi_setReturnQueue (Cppi_DescType_HOST, pCppiDesc, rxFreeQInfo);

		Cppi_setSoftwareInfo (Cppi_DescType_HOST, pCppiDesc, (uint8_t *) mySWInfo);

		Cppi_setPacketLen    (Cppi_DescType_HOST, pCppiDesc, pktLen);

		/* Push descriptor to Rx free queue */
		QMSS_QPUSHDESCSIZE (gGlobalDataPtr->gRxFreeQHnd, pCppiDesc, NIMU_MAX_DESC_SIZE);
	}
	if (i != NIMU_NUM_RX_DESC)
	{
		printf ("Error allocating Rx free descriptors \n");
		return -1;
	}

	/* Setup a Rx Flow.
	 *
	 * A Rx flow encapsulates all relevant data properties that CPDMA would
	 * have to know in order to successfully receive data.
	 */
	/* Initialize the flow configuration */
	memset (&rxFlowCfg, 0, sizeof(Cppi_RxFlowCfg));
	rxQInfo = Qmss_getQueueNumber (gGlobalDataPtr->gRxQHnd[DNUM]);
	/* Let CPPI pick the next available flow */
#ifdef PA_BYPASS
	rxFlowCfg.flowIdNum             = 22;
#else
	rxFlowCfg.flowIdNum             =   CPPI_PARAM_NOT_SPECIFIED;//
#endif
	rxFlowCfg.rx_dest_qmgr          =   rxQInfo.qMgr;
	rxFlowCfg.rx_dest_qnum          =   rxQInfo.qNum;
	rxFlowCfg.rx_desc_type          =   Cppi_DescType_HOST;

	rxFlowCfg.rx_ps_location        =   Cppi_PSLoc_PS_IN_DESC;
	rxFlowCfg.rx_psinfo_present     =   1;    /* Enable PS info */

	rxFlowCfg.rx_error_handling     =   0;    /* Drop the packet, do not retry on starvation by default */
	rxFlowCfg.rx_einfo_present      =   1;    /* EPIB info present */

	rxFlowCfg.rx_dest_tag_lo_sel    =   0;    /* Disable tagging */
	rxFlowCfg.rx_dest_tag_hi_sel    =   0;
	rxFlowCfg.rx_src_tag_lo_sel     =   0;
	rxFlowCfg.rx_src_tag_hi_sel     =   0;

	rxFlowCfg.rx_size_thresh0_en    =   0;    /* By default, we disable Rx Thresholds */
	rxFlowCfg.rx_size_thresh1_en    =   0;    /* By default, we disable Rx Thresholds */
	rxFlowCfg.rx_size_thresh2_en    =   0;    /* By default, we disable Rx Thresholds */
	rxFlowCfg.rx_size_thresh0       =   0x0;
	rxFlowCfg.rx_size_thresh1       =   0x0;
	rxFlowCfg.rx_size_thresh2       =   0x0;

	rxFlowCfg.rx_fdq0_sz0_qmgr      =   rxFreeQInfo.qMgr; /* Setup the Receive free queue for the flow */
	rxFlowCfg.rx_fdq0_sz0_qnum      =   rxFreeQInfo.qNum;
	rxFlowCfg.rx_fdq0_sz1_qnum      =   0x0;
	rxFlowCfg.rx_fdq0_sz1_qmgr      =   0x0;
	rxFlowCfg.rx_fdq0_sz2_qnum      =   0x0;
	rxFlowCfg.rx_fdq0_sz2_qmgr      =   0x0;
	rxFlowCfg.rx_fdq0_sz3_qnum      =   0x0;
	rxFlowCfg.rx_fdq0_sz3_qmgr      =   0x0;

	rxFlowCfg.rx_fdq1_qnum          =   rxFreeQInfo.qNum;  /* Use the Rx Queue to pick descriptors */
	rxFlowCfg.rx_fdq1_qmgr          =   rxFreeQInfo.qMgr;
	rxFlowCfg.rx_fdq2_qnum          =   rxFreeQInfo.qNum;  /* Use the Rx Queue to pick descriptors */
	rxFlowCfg.rx_fdq2_qmgr          =   rxFreeQInfo.qMgr;
	rxFlowCfg.rx_fdq3_qnum          =   rxFreeQInfo.qNum;  /* Use the Rx Queue to pick descriptors */
	rxFlowCfg.rx_fdq3_qmgr          =   rxFreeQInfo.qMgr;

	/* Configure the Rx flow */
	if ((gGlobalDataPtr->gRxFlowHnd[DNUM] = Cppi_configureRxFlow (res_mgr_cppi_get_passhandle(), &rxFlowCfg, &isAllocated)) == NULL)
	{
		printf ("Error configuring Rx flow \n");
		return -1;
	}
#ifdef PA_BYPASS
    rxFlowCfg.flowIdNum             = 23;
    /* Configure the Rx flow */
    if ((gGlobalDataPtr->gRxFlowHnd[DNUM] = Cppi_configureRxFlow (res_mgr_cppi_get_passhandle(), &rxFlowCfg, &isAllocated)) == NULL)
    {
        printf ("Error configuring Rx flow \n");
        return -1;
    }
#endif
	Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, PA_ACC_CHANNEL_NUM);

    /* All done with Rx configuration. Return success. */
    return 0;
}

/** ============================================================================
 *   @n@b Init_MAC
 *
 *   @b Description
 *   @n This API initializes the CPGMAC Sliver (MAC Port) port.
 *
 *   @param[in]
 *   @n macPortNum      MAC port number for which the initialization must be done.
 *
 *   @param[in]
 *   @n macAddress      MAC address to configure on this port.
 *
 *   @param[in]
 *   @n mtu             Maximum Frame length to configure on this port.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_MAC (uint32_t macPortNum, uint8_t macAddress[6], uint32_t mtu)
{
    /* Reset MAC Sliver 0 */
    CSL_CPGMAC_SL_resetMac (macPortNum);
    while (CSL_CPGMAC_SL_isMACResetDone (macPortNum) != TRUE);

    /* Setup the MAC Control Register for this port:
     *      (1) Enable Full duplex
     *      (2) Enable GMII
     *      (3) Enable Gigabit
     *      (4) Enable External Configuration. This enables
     *          the "Full duplex" and "Gigabit" settings to be
     *          controlled externally from SGMII
     *      (5) Don't Enable any control/error frames
     *      (6) Enable short frames
     */
    CSL_CPGMAC_SL_enableFullDuplex (macPortNum);
    CSL_CPGMAC_SL_enableGMII (macPortNum);
    CSL_CPGMAC_SL_enableGigabit (macPortNum);
    CSL_CPGMAC_SL_enableExtControl (macPortNum);

    /* Adding these configurations to allow the switch not to ignore any packets */
    CSL_CPGMAC_SL_enableRxCSF(macPortNum);

    /* Configure the MAC address for this port */
    CSL_CPSW_3GF_setPortMACAddress (macPortNum, macAddress);

    /* Configure VLAN ID/CFI/Priority.
     *
     * For now, we are not using VLANs so just configure them
     * to all zeros.
     */
    CSL_CPSW_3GF_setPortVlanReg (macPortNum, 0, 0, 0);

    /* Configure the Receive Maximum length on this port,
     * i.e., the maximum size the port can receive without
     * any errors.
     *
     * Set the Rx Max length to the MTU configured for the
     * interface.
     */
    CSL_CPGMAC_SL_setRxMaxLen (macPortNum, mtu);
    /* Done setting up the MAC port */
    return;
}

/** ============================================================================
 *   @n@b Init_MDIO
 *
 *   @b Description
 *   @n Not supported at moment. MDIO is not simulated yet.
 *
 *   @param[in]
 *   @n None
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_MDIO (void)
{
    /* There is nothing to be done for C6678 EVM */
     return;
}

/** ============================================================================
 *   @n@b Init_Switch
 *
 *   @b Description
 *   @n This API sets up the ethernet switch subsystem and its Address Lookup
 *      Engine (ALE) in "Switch" mode.
 *
 *   @param[in]
 *   @n mtu             Maximum Frame length to configure on the switch.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_Switch (uint32_t mtu)
{
    CSL_CPSW_3GF_PORTSTAT               portStatCfg;
    uint32_t  rx_max_len = mtu + ETHHDR_SIZE + 4; /* 4 bytes of FCS */

    /* Enable the CPPI port, i.e., port 0 that does all
     * the data streaming in/out of EMAC.
     */
    CSL_CPSW_3GF_enablePort0 ();
    CSL_CPSW_3GF_disableVlanAware ();
    CSL_CPSW_3GF_setPort0VlanReg (0, 0, 0);
    CSL_CPSW_3GF_setPort0RxMaxLen (rx_max_len);

    /* Enable statistics on both the port groups:
     *
     * MAC Sliver ports -   Port 1, Port 2
     * CPPI Port        -   Port 0
     */
    portStatCfg.p0AStatEnable   =   1;
    portStatCfg.p0BStatEnable   =   1;
    portStatCfg.p1StatEnable    =   1;
    portStatCfg.p2StatEnable    =   1;
    CSL_CPSW_3GF_setPortStatsEnableReg (&portStatCfg);

    /* Setup the Address Lookup Engine (ALE) Configuration:
     *      (1) Enable ALE.
     *      (2) Clear stale ALE entries.
     *      (3) Disable VLAN Aware lookups in ALE since
     *          we are not using VLANs by default.
     *      (4) No Flow control
     *      (5) Configure the Unknown VLAN processing
     *          properties for the switch, i.e., which
     *          ports to send the packets to.
     */
    CSL_CPSW_3GF_enableAle ();
    CSL_CPSW_3GF_clearAleTable ();
    CSL_CPSW_3GF_disableAleVlanAware ();
    CSL_CPSW_3GF_disableAleTxRateLimit ();

    /* Setting the Switch MTU Size to more than needed */
    CSL_CPGMAC_SL_setRxMaxLen(0, rx_max_len);
    CSL_CPGMAC_SL_setRxMaxLen(1, rx_max_len);

    //CSL_CPSW_3GF_enableAleBypass();
    CSL_CPSW_3GF_FLOWCNTL a;
    a.p0FlowEnable= 1;
    a.p1FlowEnable= 1;
    a.p2FlowEnable=1;

    CSL_CPSW_3GF_setFlowControlReg(&a);
#ifdef SIMULATOR_SUPPORT
    CSL_CPSW_3GF_enableAleBypass();
#endif
    /* Done with switch configuration */
    return;
}


/** ============================================================================
 *   @n@b Switch_update_addr
 *
 *   @b Description
 *   @n This API add/delete entries in the Address Lookup Engine (ALE) in "Switch" mode.
 *
 *   @param[in]
 *   @n portNum         Switch port number.

 *   @param[in]
 *   @n macAddress      MAC address to configure on the switch.
 *
 *   @param[in]
 *   @n add             0:add; 1:delete.
 *
 *   @return
 *   @n None
 *
 *   @Note  It supports "add" operation only now.
 * =============================================================================
 */
int Switch_update_addr (uint32_t portNum, uint8_t macAddress[6], Uint16 add)
{
    CSL_CPSW_3GF_ALE_PORTCONTROL        alePortControlCfg;
#ifdef SIMULATOR_SUPPORT
    uint32_t                              i;
    CSL_CPSW_3GF_ALE_UNICASTADDR_ENTRY  ucastAddrCfg;
#endif


    /* Configure the address in "Learning"/"Forward" state */
    alePortControlCfg.portState             =   ALE_PORTSTATE_FORWARD;

    alePortControlCfg.dropUntaggedEnable    =   0;
    alePortControlCfg.vidIngressCheckEnable =   0;
    alePortControlCfg.noLearnModeEnable     =   0;
    alePortControlCfg.mcastLimit            =   0;
    alePortControlCfg.bcastLimit            =   0;

    CSL_CPSW_3GF_setAlePortControlReg (portNum, &alePortControlCfg);

#ifdef SIMULATOR_SUPPORT
    /* Program the ALE with the MAC address.
     *
     * The ALE entries determine the switch port to which any
     * matching received packet must be forwarded to.
     */
    /* Get the next free ALE entry to program */
    for (i = 0; i < CSL_CPSW_3GF_NUMALE_ENTRIES; i++)
    {
        if (CSL_CPSW_3GF_getALEEntryType (i) == ALE_ENTRYTYPE_FREE)
        {
            /* Found a free entry */
            break;
        }
    }
    if (i == CSL_CPSW_3GF_NUMALE_ENTRIES)
    {
        /* No free ALE entry found. return error. */
        return -1;
    }
    else
    {
        /* Found a free ALE entry to program our MAC address */
        memcpy (ucastAddrCfg.macAddress, macAddress, 6);    // Set the MAC address
        ucastAddrCfg.ucastType      =      ALE_UCASTTYPE_UCAST_NOAGE;   // Add a permanent unicast address entryALE_UCASTTYPE_UCAST_NOAGE.
        ucastAddrCfg.secureEnable   =      FALSE;
        ucastAddrCfg.blockEnable    =      FALSE;
        ucastAddrCfg.portNumber     =      portNum;   // Add the ALE entry for this port

        /* Setup the ALE entry for this port's MAC address */
        CSL_CPSW_3GF_setAleUnicastAddrEntry (i, &ucastAddrCfg);
    }
#endif
    /* Done with upading address */
    return 0;
}

/** ============================================================================
 *   @n@b Verify_Init
 *
 *   @b Description
 *   @n This API initializes the CPPI LLD, opens the PASS CPDMA and opens up
 *      the Tx, Rx channels required for data transfers.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Verify_Init (void)
{
    int32_t count, returnVal = 0, i;
    int32_t num_tx_desc = NIMU_NUM_TX_DESC;
    int32_t num_rx_desc = NIMU_NUM_RX_DESC;
    int32_t max_queue_number = QMSS_MAX_LOW_PRIORITY_QUEUE          +
                             QMSS_MAX_PASS_QUEUE                    +
                             QMSS_MAX_LOW_PRIORITY_QUEUE         	+
                             QMSS_MAX_PASS_QUEUE                 	+
                             QMSS_MAX_INTC_QUEUE                 	+
                             QMSS_MAX_SRIO_QUEUE                 	+
                             QMSS_MAX_HIGH_PRIORITY_QUEUE        	+
                             QMSS_MAX_STARVATION_COUNTER_QUEUE   	+
                             QMSS_MAX_INFRASTRUCTURE_QUEUE       	+
                             QMSS_MAX_TRAFFIC_SHAPING_QUEUE      	+
                             QMSS_MAX_GENERAL_PURPOSE_QUEUE      	;

    /*Verify if we got NIMU_NUM_TX_DESC Tx Free Q*/
    if ((count = Qmss_getQueueEntryCount (gGlobalDataPtr->gTxFreeQHnd)) != num_tx_desc)  {
    	printf ("Verify_Init: Expected %d entry count for gTxFreeQHnd queue %d, found %d entries\n", num_tx_desc, gGlobalDataPtr->gTxFreeQHnd, count);
        returnVal =  -1;
    }

    /* Verify if we got NIMU_NUM_RX_DESC Rx FDQ */
    if ((count = Qmss_getQueueEntryCount (gGlobalDataPtr->gRxFreeQHnd)) != num_rx_desc)  {
    	printf ("Verify_Init: Expected %d entry count for gRxFreeQHnd queue %d, found %d entries\n", num_rx_desc, gGlobalDataPtr->gRxFreeQHnd, count);
         returnVal = -1;
    }

    /* Verify if we got empty Tx completion Q*/
    if ((count = Qmss_getQueueEntryCount (gGlobalDataPtr->gTxReturnQHnd)) != 0)  {
    	printf ("Verify_Init: Expected 0 entry count for gTxReturnQHnd queue %d, found %d entries\n", gGlobalDataPtr->gTxReturnQHnd, count);
        returnVal = -1;
    }

    /* Verify if we got NIMU_NUM_RX_DESC Rx FDQ */
    if ((count = Qmss_getQueueEntryCount (gGlobalDataPtr->gRxQHnd[DNUM])) != 0)  {
    	printf ("Verify_Init: Expected 0 entry count for gRxQHnd= %d, found %d entries\n", gGlobalDataPtr->gRxQHnd, count);
         returnVal = -1;
    }

    for (i = 0; i < max_queue_number; i++ ) {
        if ( (i == gGlobalDataPtr->gRxFreeQHnd) || (i == gGlobalDataPtr->gTxFreeQHnd) )
            continue;

        count = Qmss_getQueueEntryCount (i);
        if (count != 0) {
        	//printf ("Verify_Init: Expected 0 entry count for Queue number = %d, found %d entries\n", i, count);
        }
    }

    /* Verify_Init Done. Return success */
    return (returnVal);
}

/** ============================================================================
 *   @n@b Init_Cpsw
 *
 *   @b Description
 *   @n This API sets up the entire ethernet subsystem and all its associated
 *      components.
 *
 *   @param[in]
 *   @n None
 *
 *   @return
 *   @n None
 * =============================================================================
 */
int32_t Init_Cpsw (uint32_t mtu, uint8_t* myMACAddress)
{                      
#ifdef SIMULATOR_SUPPORT

    uint8_t bcMACAddress[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    extern  uint8_t clientMACAddress [6];

    /* Initialize the SGMII/Sliver submodules for the MAC port. */
    Init_SGMII (0);
    Init_MAC (0, myMACAddress, mtu);

    Init_SGMII (1);
    Init_MAC (1, myMACAddress, mtu);

    /* Setup the Ethernet switch finally. */
    Init_Switch (mtu);


   /* Update the ALE Entries and ensure that these are correctly configured.
    * There are 2 Entries created here:
    *  Entry1: My OWN MAC Address goes to Port 0
    *  Entry2: Destination MAC Address is forwarded to Port1
    * If there are more destination MAC Addresses to which packets need to be sent
    * than additional entries need to be configured. */

   /* This is needed only for testing in Simulator*/
   Switch_update_addr(0, myMACAddress, 0);
   Switch_update_addr(1, clientMACAddress, 0);
   Switch_update_addr(1, myMACAddress, 0);  // testing a hybrid between cooked up ping and the original app (cooked up raw message)

   // checking out if adding bc message still work for unicast
   Switch_update_addr(1, bcMACAddress, 0);     // verified needed for BCAST tx

   // add this to see if BC packet response can come into the PA
   Switch_update_addr(0, bcMACAddress, 0);     // verfied needed for BCast Rx

#else

    uint32_t  i;
    uint8_t   backplaneMac[6] = {0x1, 0x1, 0x1, 0x1, 0x1, 0x1}; /* Mask for creating mac address by flipping LSB */
    uint8_t   cppiMac [6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};  /* MAC address for (CPPI) Port 0 - we made it up*/


	/* we need to create a mac address for the Backplane Ethernet port
	 * using the Ethernet MAC Adress
	 */
    for (i=0; i < 6; i++) {
        backplaneMac[i] |= myMACAddress[i];
    }
#if 1
	Init_MAC (0, backplaneMac, mtu);

	/* set the silver port to the stacks ethernet address */
	Init_MAC (1, myMACAddress, mtu);

    /* Setup the Phys by initializing the MDIO - not needed for Simulator*/
    Init_MDIO ();

    /* Setup the Ethernet switch finally. */
    Init_Switch (mtu);

    /* This is a little confusing but different APIs use different numbering */
    Switch_update_addr(0, cppiMac, 0);
    Switch_update_addr(1, backplaneMac, 0);
    Switch_update_addr(2, myMACAddress, 0);
#endif
#endif

    /* CPSW subsystem setup done. Return success */
    return 0;
}

/* ============================================================================
 *   @n@b Init_PASS
 *
 *   @b Description
 *   @n This API initializes the PASS/PDSP and opens a queue that the application
 *      can use to receive command responses from the PASS.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Init_PASS (void)
{
    uint8_t			isAllocated;

    /* Open a PA Command Response Queue.
     *
     * This queue will be used to hold responses from the PA PDSP for all the
     * commands issued by NIMU.
     *
     * This queue is used only when configuring the PA PDSP. We will use it when adding our mac address.
     */
    if ((gGlobalDataPtr->gPaCfgCmdRespQHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
    {
    	printf ("Error opening a PA Command Response queue \n");
        return -1;
    }

    /* Init done. Return success. */
    return 0;
}


/** ============================================================================
 *   @n@b Add_MACAddress
 *
 *   @b Description
 *   @n This API adds the switch MAC address to the PA PDSP Lookup table. This
 *      ensures that all packets destined for this MAC address get processed
 *      for forwarding to the host.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Add_MACAddress (paEthInfo_t ethInfo, uint8_t flowId)
{
    int32_t              j;
    uint16_t            cmdSize;
    Qmss_Queue          cmdReplyQInfo;

    paReturn_t          retVal;
    paEntryHandle_t     retHandle;
    int32_t             handleType, cmdDest;
    uint32_t            psCmd       =   ((uint32_t)(4 << 5) << 24);
    uint32_t            myswinfo[]  =   {0x11112222, 0x33334444};
    uint32_t            myswinfo_orig[] =   {0x00000000, 0x00000000, 0x00000000};;
    uint8_t*            pCmdDataBuffer;
    Cppi_HostDesc*      pHostDesc = NULL;
    Qmss_Queue          rxQInfo;
    uint32_t            count, cmdbuf_len = 320;
    int32_t             ret_val = 0;

    paRouteInfo_t       routeInfo;
    paRouteInfo_t       nFailInfo;
    paCmdReply_t        cmdReplyInfo;

    routeInfo.dest = pa_DEST_HOST;
    routeInfo.flowId = flowId;//(uint8_t)Cppi_getFlowId(gRxFlowHnd);;
    routeInfo.queue = 0;
    routeInfo.mRouteIndex = -1;
    routeInfo.swInfo0 = 0xaaaaaaaa;
    routeInfo.swInfo1 = 0;
    routeInfo.customType = 0;
    routeInfo.customIndex = 0;
    routeInfo.pktType_emacCtrl = 0;
    routeInfo.pCmd = NULL;

    nFailInfo.dest = pa_DEST_DISCARD;
    nFailInfo.flowId = flowId;//(uint8_t)Cppi_getFlowId(gRxFlowHnd);;
    nFailInfo.queue = 0;
    nFailInfo.mRouteIndex = 0;
    nFailInfo.swInfo0 = 0;
    nFailInfo.swInfo1 = 0;
    nFailInfo.customType = 0;
    nFailInfo.customIndex = 0;
    nFailInfo.pktType_emacCtrl = 0;
    nFailInfo.pCmd = NULL;

    cmdReplyInfo.dest = pa_DEST_HOST;
    cmdReplyInfo.replyId = 0;
    cmdReplyInfo.queue = 0;
    cmdReplyInfo.flowId = flowId;//(uint8_t)Cppi_getFlowId(gRxFlowHnd);;

    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if ((QMSS_QPOP (gGlobalDataPtr->gTxFreeQHnd, QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc **)&pHostDesc )) != NULL)
    {
        printf ("Error obtaining a Tx free descriptor \n");
        return -1;
    }

    /* Allocate a Tx buffer and attach the command info to it. */
    cmdbuf_len = platform_roundup(cmdbuf_len, PLATFORM_CACHE_LINE_SIZE);
    if ((pCmdDataBuffer = (Ptr) Osal_nimuMalloc (cmdbuf_len, PLATFORM_CACHE_LINE_SIZE)) == NULL)
    {
        printf ("Error allocating memory for PA Command data buffer \n");
        return -1;
    }

    /* Populate the Tx free descriptor with the buffer we just allocated. */
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)l2_global_address((uint32_t)pCmdDataBuffer), cmdbuf_len);

    /* Save original buffer information */
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)l2_global_address((uint32_t)pCmdDataBuffer), cmdbuf_len);

    cmdSize                 =   pHostDesc->buffLen;
    cmdReplyInfo.replyId    =   0x11111111;  /* unique for each add mac command */

    /* Get the PA response queue number and populate the destination queue number
     * in the PA response configuration.
     */
    cmdReplyQInfo           =   Qmss_getQueueNumber (gGlobalDataPtr->gPaCfgCmdRespQHnd);
    cmdReplyInfo.queue      =   cmdReplyQInfo.qNum;

    /* Setup the Rx queue as destination for the packets */
    rxQInfo                 =   Qmss_getQueueNumber (gGlobalDataPtr->gRxQHnd[DNUM]);
    routeInfo.queue         =   rxQInfo.qNum;

    retVal  =   Pa_addMac  (res_mgr_get_painstance(),
                            pa_LUT1_INDEX_NOT_SPECIFIED,
                            &ethInfo,
                            &routeInfo,
                            &nFailInfo,
                            &gGlobalDataPtr->gPaL2Handles[0],
                            (paCmd_t) pHostDesc->buffPtr,
                            &cmdSize,
                            &cmdReplyInfo,
                            &cmdDest);
    if (retVal != pa_OK)
    {
        printf ("Pa_addMac returned error %d\n", retVal);
        ret_val = -1;
        goto return_fail;
    }


    /* This sets the extended info for descriptors, and this is required so PS info
     * goes to the right spot
     */
    Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)myswinfo);

    /* Set the buffer length to the size used. It will be restored when the descriptor
     * is returned
     */
    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, cmdSize);
    pHostDesc->buffLen  =   cmdSize;

    /* Mark the packet as a configuration packet */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)&psCmd, 4);

    /* Send the command to the PA and wait for the return */
    QMSS_QPUSH    (gGlobalDataPtr->gPaTxQHnd[cmdDest - pa_CMD_TX_DEST_0],
                    pHostDesc,
                    pHostDesc->buffLen,
                    NIMU_MAX_DESC_SIZE,
                    Qmss_Location_TAIL
                   );

    /* Poll on the PA response queue to see if response from PA has come */
    for (j = 0; j < 100; j++)
    {
        board_delay_cycles (1000);

        /* Verify if we got empty Tx completion Q*/
        count = Qmss_getQueueEntryCount (gGlobalDataPtr->gTxReturnQHnd);

        if (count != 0)  {

            if ((QMSS_QPOP (gGlobalDataPtr->gTxReturnQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc **)&pHostDesc)) != NULL)
            {
                ret_val = -1;
                goto return_fail;
            }

            /* Restore the states */
            Cppi_setSoftwareInfo(Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)myswinfo_orig);
            Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL, NULL);
            Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL, NULL);
            Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL, NULL);
            Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL);
            pHostDesc->buffLen = 0;

            QMSS_QPUSHDESCSIZE (gGlobalDataPtr->gTxFreeQHnd, pHostDesc, NIMU_MAX_DESC_SIZE);
        }

        if (Qmss_getQueueEntryCount (gGlobalDataPtr->gPaCfgCmdRespQHnd) > 0)
        {

            /* We have a response from PA PDSP for the command we submitted earlier for
             * MAC address addition.
             */
            if ((QMSS_QPOP (gGlobalDataPtr->gPaCfgCmdRespQHnd, QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc **)&pHostDesc)) != NULL)
            {
                ret_val = -1;
                goto return_fail;
            }

            if (pHostDesc->softwareInfo0 != cmdReplyInfo.replyId)
            {
                printf ("Found an entry in PA response queue with swinfo0 = 0x%08x, expected 0x%08x\n",
                                pHostDesc->softwareInfo0, cmdReplyInfo.replyId);
                pHostDesc->buffLen  =   pHostDesc->origBufferLen;
                QMSS_QPUSH (gGlobalDataPtr->gTxFreeQHnd, pHostDesc, pHostDesc->buffLen, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);
                ret_val = -1;
                goto return_fail;

            }

            retVal  =   Pa_forwardResult (res_mgr_get_painstance(), (Ptr)pHostDesc->buffPtr, &retHandle, &handleType, &cmdDest);
            if (retVal != pa_OK)
            {
                printf ("PA sub-system rejected Pa_addMac command\n");
                ret_val = -1;
                goto return_fail;
            }

            /* Reset the buffer length and put the descriptor back on the Rx free queue */
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            QMSS_QPUSH (gGlobalDataPtr->gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);
            break;
        }
    }

    if (j == 100)
    {
        printf ("Timeout waiting for reply from PA to Pa_addMac command\n");
        ret_val = -1;
    }

return_fail:

    Osal_nimuFree (pCmdDataBuffer, cmdbuf_len);

    return (ret_val);
}
#if 0
/** ============================================================================
 *   @n@b Add_IPAddress
 *
 *   @b Description
 *   @n This API adds the IP Address the application's using to the PA PDSP
 *      Lookup table. This ensures that all packets destined for this
 *      IP address get forwarded up to the host.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    Int32
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
uint8_t gIpaddr[2][4] = {
		{192, 168, 1, 230},
		{192, 168, 1, 231}
};
uint8_t gCmd[8][1024];
int32_t Add_IPAddress (void)
{
    Int32                       j;
    UInt16                      cmdSize;
    Qmss_Queue                  cmdReplyQInfo;
    paIpInfo_t                  ipInfo      =    {  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   /* IP source = dont care */
                                                    { 192, 168, 1, 231, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   /* IP dest */
                                                    0,         /* SPI = dont care */
                                                    0,         /* flow = dont care */
                                                    pa_IPV4,   /* IP type */
                                                    0,         /* GRE protocol */
                                                    0,         /* Ip protocol = dont care (TCP or UDP or anything else) */
                                                    0,         /* TOS */
                                                    FALSE,     /* TOS = dont care (seperate field since TOS=0 is valid */
                                                    0          /* SCTP destination port = dont care */
                                                };

    if(DNUM == MAST_CORE)
    {
    	memcpy(&ipInfo.dst.ipv4, &gIpaddr[0][0], 4);
    }else
    {
    	memcpy(&ipInfo.dst.ipv4, &gIpaddr[0][1], 4);
    }


    paRouteInfo_t               routeInfo   =   {   pa_DEST_HOST,                /* Continue parsing */
                                                    0,                                          /* Flow Id = dont care */
                                                    0,                                          /* queue = dont care */
                                                    0,                                          /* multi route = dont care */
                                                    0,                                          /* swinfo0 = dont care */
                                                    0,                                          /* SwInfo 1 is dont care */
                                                    0,                                          /* customType = pa_CUSTOM_TYPE_NONE */         \
                                                    0,                                          /* customIndex: not used */        \
                                                    0,                                          /* pkyType: for SRIO only */       \
                                                    NULL                                        /* No commands */
                                                };
    paRouteInfo_t               nFailInfo   =   {   pa_DEST_DISCARD,                            /* Toss the packet  */
	                                                0,                                          /* Flow Id = dont care */
                                                    0,                                          /* queue = dont care */
                                                    0,                                          /* mutli route = dont care */
                                                    0,                                          /* swinfo0 = dont care */
                                                    0,                                          /* SwInfo 1 is dont care */
                                                    0,                                          /* customType = pa_CUSTOM_TYPE_NONE */         \
                                                    0,                                          /* customIndex: not used */        \
                                                    0,                                          /* pkyType: for SRIO only */       \
                                                    NULL                                        /* No commands */
                                                };
    paCmdReply_t                cmdReplyInfo =  {   pa_DEST_HOST,                               /* Replies go to the host */
                                                    0,                                          /* User chosen ID to go to swinfo0 */
                                                    0,                                          /* Destination queue */
                                                    0                                           /* Flow ID */
                                                };
    paReturn_t                  retVal;
    paEntryHandle_t             retHandle;
    Int32                       handleType, cmdDest;
    UInt32                      psCmd       =   ((UInt32)(4 << 5) << 24);
    UInt32                      myswinfo[]  =   {0x11112222, 0x33334444};
    Cppi_HostDesc*              pHostDesc;
    uint8_t* pCmdDataBuffer;
    uint32_t cmdbuf_len;

    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if ((pHostDesc = Qmss_queuePop (gTxFreeQHnd)) == NULL)
    {
        printf ("Error obtaining a Tx free descriptor \n");
        return -1;
    }

    pCmdDataBuffer = &gCmdBuff[DNUM][0];
    cmdbuf_len = (((300+15)/16)*16);
    /* The descriptor address returned from the hardware has the
     * descriptor size appended to the address in the last 4 bits.
     *
     * To get the true descriptor pointer, always mask off the last
     * 4 bits of the address.
     */
    pHostDesc = (Ptr) ((UInt32) pHostDesc & 0xFFFFFFF0);
    /* Populate the Tx free descriptor with the buffer we just allocated. */
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)l2_global_address((uint32_t)pCmdDataBuffer), cmdbuf_len);

    /* Save original buffer information */
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)l2_global_address((uint32_t)pCmdDataBuffer), cmdbuf_len);

    CACHE_invL1d (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_invL2 (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_invL1d ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_invL2 ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    pHostDesc->buffLen      =   pHostDesc->origBufferLen;
    cmdSize                 =   pHostDesc->buffLen;
    cmdReplyInfo.replyId    =   0x55550001;  /* unique for each add mac command */

    /* Get the PA response queue number and populate the destination queue number
     * in the PA response configuration.
     */
    cmdReplyQInfo           =   Qmss_getQueueNumber (gPaCfgCmdRespQHnd);
    cmdReplyInfo.queue      =   cmdReplyQInfo.qNum;
    cmdReplyInfo.flowId     =   (uint8_t)Cppi_getFlowId(gRxFlowHnd[DNUM]);

    routeInfo.flowId        = (uint8_t)Cppi_getFlowId(gRxFlowHnd[DNUM]);
    nFailInfo.flowId        = (uint8_t)Cppi_getFlowId(gRxFlowHnd[DNUM]);

    retVal  =   Pa_addIp    (res_mgr_get_painstance(),
                             pa_LUT_INST_NOT_SPECIFIED,
                             pa_LUT1_INDEX_NOT_SPECIFIED,
                            &ipInfo,
                            gPaL2Handles[0],
                            &routeInfo,
                            &nFailInfo,
                            &gPaL3Handles[0],
                            (paCmd_t) pHostDesc->buffPtr,
                            &cmdSize,
                            &cmdReplyInfo,
                            &cmdDest);
    if (retVal != pa_OK)
    {
        printf ("Pa_addIp returned error %d\n", retVal);
        return -1;
    }

    /* This sets the extended info for descriptors, and this is required so PS info
     * goes to the right spot
     */
    Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (UInt8 *)myswinfo);

    /* Set the buffer length to the size used. It will be restored when the descriptor
     * is returned
     */
    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, cmdSize);
    pHostDesc->buffLen  =   cmdSize;

    /* Mark the packet as a configuration packet */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (UInt8 *)&psCmd, 4);

    /* Send the command to the PA and wait for the return */
    CACHE_wbL1d (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_wbL2 (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_wbL1d ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_wbL2 ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
//    SYS_CACHE_WB (pHostDesc, SIZE_HOST_DESC, CACHE_FENCE_WAIT);
//    SYS_CACHE_WB ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_FENCE_WAIT);
    Qmss_queuePush (gPaTxQHnd[cmdDest - pa_CMD_TX_DEST_0],
                    pHostDesc,
                    pHostDesc->buffLen,
                    NIMU_MAX_DESC_SIZE,
                    Qmss_Location_TAIL
                    );

    int count;
    /* Poll on the PA response queue to see if response from PA has come */
    for (j = 0; j < 100; j++)
    {
    	board_delay_cycles (1000);

        if (Qmss_getQueueEntryCount (gPaCfgCmdRespQHnd) > 0)
        {
            /* We have a response from PA PDSP for the command we submitted earlier for
             * MAC address addition.
             */
            pHostDesc = Qmss_queuePop (gPaCfgCmdRespQHnd);

            /* Clear the size bytes */
            pHostDesc = (Ptr) ((UInt32) pHostDesc & 0xFFFFFFF0);
            CACHE_invL1d (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
            CACHE_invL2 (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
            CACHE_invL1d ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
            CACHE_invL2 ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
//            SYS_CACHE_INV (pHostDesc, SIZE_HOST_DESC, CACHE_FENCE_WAIT);
//            SYS_CACHE_INV ((Ptr)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_FENCE_WAIT);

            if (pHostDesc->softwareInfo0 != cmdReplyInfo.replyId)
            {
                printf ("Add_IPAddress:Found an entry in PA response queue with swinfo0 = 0x%08x, expected 0x%08x\n",
                                pHostDesc->softwareInfo0, cmdReplyInfo.replyId);
                pHostDesc->buffLen  =   pHostDesc->origBufferLen;
//                SYS_CACHE_WB (pHostDesc, SIZE_HOST_DESC, CACHE_FENCE_WAIT);
                CACHE_wbL1d (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
                CACHE_wbL2 (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
                Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);

                return -1;
            }


            retVal  =   Pa_forwardResult (res_mgr_get_painstance(), (Ptr)pHostDesc->buffPtr, &retHandle, &handleType, &cmdDest);
            if (retVal != pa_OK)
            {
                printf ("PA sub-system rejected Pa_addIp command\n");
                return -1;
            }

            /* Reset the buffer lenght and put the descriptor back on the Tx free queue */
            pHostDesc->buffLen = pHostDesc->origBufferLen;
//            SYS_CACHE_WB (pHostDesc, SIZE_HOST_DESC, CACHE_FENCE_WAIT);
            CACHE_wbL1d (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
            CACHE_wbL2 (pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
            Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);

            /* Verify if we got empty Tx completion Q*/
            count = Qmss_getQueueEntryCount (gTxReturnQHnd);

            if (count != 0)  {

                if ((QMSS_QPOP (gTxReturnQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc **)&pHostDesc)) != NULL)
                {
                	return -1;
                }

                /* Restore the states */
                Cppi_setSoftwareInfo(Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)myswinfo);
                Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL, NULL);
                Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL, NULL);
                Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL, NULL);
                Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, NULL);
                pHostDesc->buffLen = 0;

                QMSS_QPUSHDESCSIZE (gTxFreeQHnd, pHostDesc, NIMU_MAX_DESC_SIZE);
            }

            break;
        }
    }

    if (j == 100)
    {
        printf ("Timeout waiting for reply from PA to Pa_addIP command\n");
        return -1;
    }

    return 0;
}
#endif
/**
 *  @b Description
 *  @n
 *      Call back function provided by application for EMAC driver
 *      to report a received packet descriptor.
 *
 *  @retval
 *      None.
 */
 Bool  gIsPingListUsed = 0;
 uint32_t gDescCount;
 uint64_t gTemp[1000];
 int gIsrCnt=0;
void EmacRxPktISR(NETIF_DEVICE* ptr_net_device)
{
    uint32_t              protocol, pktLen;
    UINT8*              pBuffer;
    Cppi_HostDesc*      pHostDesc;
    PBM_Handle          hPkt;
    Cppi_Desc*          pCppiDesc;
    uint32_t                i;
    uint32_t*             ListAddress;
    EMAC_DATA*          ptr_pvt_data;
    PBM_Pkt*            rx_pbm_pkt;
    void*               key;
    static uint64_t gTime;

    /* Disable the interrupt */
    coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)] =  Hwi_disableInterrupt(ETH_INTERRUPT); //Hwi_disable();
//    gTemp[gIsrCnt++%1000] = CSL_tscRead()-gTime;
//    gTime = CSL_tscRead();
    /* Begin Critical Section before accessing shared resources. */
    key = Osal_qmssCsEnter ();
    //
    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA *)ptr_net_device->pvt_data;

    /* Clean the cache for external addesses */
    if(
        ((uint32_t)(&gAccumListPtr->gEthHiPriAccumList[DNUM][0]) & EMAC_EXTMEM ) ||
        ((uint32_t)(&gAccumListPtr->gEthHiPriAccumList[DNUM][0]) & EMAC_MSMCSRAM )
      )
    {
        /* This needs to be enabled if gEthHiPriAccumList is in External Memory or MSMCMEM */
        /* Inv cache based on the qhandler type*/
        CACHE_invL1d((void *)&gAccumListPtr->gEthHiPriAccumList[DNUM][0], sizeof(gAccumListPtr->gEthHiPriAccumList)/8, CACHE_WAIT);
        CACHE_invL2((void *) &gAccumListPtr->gEthHiPriAccumList[DNUM][0], sizeof(gAccumListPtr->gEthHiPriAccumList)/8, CACHE_WAIT);
    }

    //i           = MAX_HI_PRI_ACCUM_LIST_SIZE - 1 - (RX_INT_THRESHOLD+1);
    ListAddress = (uint32_t* )l2_global_address((uint32_t) &gAccumListPtr->gEthHiPriAccumList[DNUM][0]);

    /* Process ISR.
     *
     * Get the number of entries in accumulator list.
     * The hardware enqueues data alternatively to Ping/Pong buffer lists in
     * the accumulator. Hence, we need to track which list (Ping/Pong)
     * we serviced the last time and accordingly process the other one
     * this time around.
     */
     if (!gIsPingListUsed)
     {
        /* Serviced Pong list last time. So read off the Ping list now */
        gDescCount   =   ListAddress[0];
     }
     else
     {
        /* Serviced Pong list last time. So read off the Ping list now */
        gDescCount   =   ListAddress[RX_INT_THRESHOLD + 1];
     }

    if (gDescCount == 0) {
        //gIsrCnt++;
        /* End Critical Section */
        Osal_qmssCsExit (key);
        Hwi_restoreInterrupt(ETH_INTERRUPT, coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)]);
        /* Clear INTD */
        Qmss_ackInterrupt(PA_ACC_CHANNEL_NUM, 1);
        Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, PA_ACC_CHANNEL_NUM);
        return ; /* Not enough packets are received */
    }

    /* Process all the Results received
     *
     * Skip the first entry in the list that contains the
     * entry gDescCount and proceed processing results.
     */
    for (i = 1; i <= gDescCount; i ++)
    {

        gRxCounter ++;

        /* Get the result descriptor.
         *
         * The hardware enqueues data alternatively to Ping/Pong buffer lists in
         * the accumulator. Hence, we need to track which list (Ping/Pong)
         * we serviced the last time and accordingly process the other one
         * this time around.
         */
        if (!gIsPingListUsed)
        {
            /* Serviced Pong list last time. So read off the Ping list now */
            pCppiDesc   =   (Cppi_Desc *) ListAddress [i];
        }
        else
        {
            /* Serviced Ping list last time. So read off the Pong list now
             *
             * Skip over Ping list length to arrive at Pong list start.
             */
            pCppiDesc   =   (Cppi_Desc *) ListAddress [i + RX_INT_THRESHOLD + 1];
        }

        /* Descriptor size appended to the address in the last 4 bits.
         *
         * To get the true descriptor size, always mask off the last
         * 4 bits of the address.
         */
        pCppiDesc = (Ptr) ((uint32_t) pCppiDesc & 0xFFFFFFF0);
        pHostDesc = (Cppi_HostDesc *)pCppiDesc;

        /* Inv cache based on the qhandler type*/
        CACHE_invL1d((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
        CACHE_invL2((void *) pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
        CACHE_invL1d((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
        CACHE_invL2((void *) pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);

        /* Allocate the PBM packet for the Max MTU size*/
        if (NULL == (hPkt = PBM_alloc(1514))) {
            /* could not get a free NDK packet, maybe the next time around we can... */
            gRxDropCounter++;
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            QMSS_QPUSH (gGlobalDataPtr->gRxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);
            continue;
        }

        rx_pbm_pkt = (PBM_Pkt *) hPkt;

        PBM_setDataOffset((PBM_Handle) hPkt, 0);

        /* removing the FCS len here */
        pktLen    = (pHostDesc->buffLen-4);

        if (pktLen > (ptr_net_device->mtu + ETHHDR_SIZE + 4)) {
            pktLen = (ptr_net_device->mtu + ETHHDR_SIZE + 4);
        }

        if (pktLen < 60) {
            pktLen = 64;
        }

        PBM_setValidLen((PBM_Handle) hPkt, pktLen);

        /* Handle raw frames separately, i.e. check the
        * Ethernet Protocol type in this packet and see
        * if its a well known ether type. If so, this is normal
        * IP stream, enqueue this is in usual Rx queue and let the
        * stack know that a packet has arrived for it. However, if
        * the ethernet type in the packet is not a well known one,
        * this could be a custom raw ethernet packet, enqueue it
        * separately in the Raw Rx queue and notiofy stack. The Raw
        * Ethernet packets when being handed over are given
        * preferential treatment and are serviced before the normal
        * IP traffic. Hence the 2 queues.
        */
        pBuffer =  (UINT8* )l2_global_address((uint32_t) pHostDesc->buffPtr);

        /* Extract the ethernet type from the packet. */
        protocol = ( pBuffer[12] << 8) | pBuffer[13] ;
        protocol = (protocol & 0xFFFFu);

        PBM_setIFRx((PBM_Handle) hPkt, (HANDLE) protocol );
        /* Copy the data buffer received to the allocated PBM packet */
        mmCopy((UINT8* )rx_pbm_pkt->pDataBuffer, (UINT8* )pBuffer, pktLen) ;


        /* Is it a standard ethernet type? */
        if (protocol != ETHERTYPE_IP && protocol != ETHERTYPE_IPv6 && protocol != ETHERTYPE_VLAN
            && protocol != ETHERTYPE_PPPOECTL && protocol != ETHERTYPE_PPPOEDATA )
        {
            /* This is a raw packet, enqueue in Raw Rx Queue */
            PBMQ_enq( &ptr_pvt_data->pdi.PBMQ_rawrx, (PBM_Handle) hPkt);
        }
        else
        {            /* This is a normal IP packet. Enqueue in Rx Queue */
            PBMQ_enq( &ptr_pvt_data->pdi.PBMQ_rx, (PBM_Handle) hPkt );
        }

       /* Free the packet back to the Rx FDQ */
        pHostDesc->buffLen = pHostDesc->origBufferLen;
        QMSS_QPUSH (gGlobalDataPtr->gRxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, NIMU_MAX_DESC_SIZE, Qmss_Location_TAIL);
    }

    ListAddress = (uint32_t *) l2_global_address((uint32_t) &gAccumListPtr->gEthHiPriAccumList[DNUM][0]);

    /* Clear the accumulator list and save whether we used Ping/Pong
     * list information for next time around.
     */
     if (!gIsPingListUsed)
    {
        /* Just processed Ping list */
        gIsPingListUsed  =   1;

        i = sizeof (gAccumListPtr->gEthHiPriAccumList)/16;

        /* Clear the accumulator list after processing */
        memset ((void *) &ListAddress[0], 0, i);

        if(
            ((uint32_t)(&ListAddress[0]) & EMAC_EXTMEM ) ||
            ((uint32_t)(&ListAddress[0]) & EMAC_MSMCSRAM )
          )
        {
            /* This needs to be enabled if gEthHiPriAccumList is in External Memory or MSMCMEM */
            CACHE_wbL1d((void *)&ListAddress[0], i, CACHE_WAIT);
            CACHE_wbL2 ((void *)&ListAddress[0], i, CACHE_WAIT);
        }
    }
    else
    {
        /* Just processed Pong list */
        gIsPingListUsed  =   0;
        i = sizeof (gAccumListPtr->gEthHiPriAccumList)/16;

        /* Clear the accumulator list after processing */
        memset ((void *) &ListAddress[RX_INT_THRESHOLD+1], 0, i);

        if(
            ((uint32_t)(&ListAddress[RX_INT_THRESHOLD+1]) & EMAC_EXTMEM ) ||
            ((uint32_t)(&ListAddress[RX_INT_THRESHOLD+1]) & EMAC_MSMCSRAM )
          )
        {
            /* This needs to be enabled if gEthHiPriAccumList is in External Memory or MSMCMEM */
            CACHE_wbL1d((void *)&ListAddress[RX_INT_THRESHOLD+1], i, CACHE_WAIT);
            CACHE_wbL2((void *) &ListAddress[RX_INT_THRESHOLD+1], i, CACHE_WAIT);
        }
    }

    /* Notify NDK stack of pending Rx Ethernet packet */
    STKEVENT_signal( ptr_pvt_data->pdi.hEvent, STKEVENT_ETHERNET, 1 );

    /* Clean the cache for external addesses */
    if( (uint32_t)(PBM_getDataBuffer(hPkt)) & EMAC_EXTMEM ) {
        OEMCacheClean( (void *) hPkt, PBM_getBufferLen(hPkt) );
    }

    /* End Critical Section */
    Osal_qmssCsExit (key);
    Hwi_restoreInterrupt(ETH_INTERRUPT, coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)]);

    /* Clear INTD */
    Qmss_ackInterrupt(PA_ACC_CHANNEL_NUM, 1);
    Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, PA_ACC_CHANNEL_NUM);

}


/* Nothing past this point */

