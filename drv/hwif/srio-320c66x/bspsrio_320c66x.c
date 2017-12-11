

/**
 *   @file  device_srio_tput.c
 *
 *   @brief
 *      The 667x SRIO Device specific code. The SRIO driver calls out
 *      this code to initialize the SRIO IP block. The file is provided as
 *      a sample configuration and should be modified by customers for
 *      their own platforms and configurations.
 *
 */
#include "srio.h"

/**
 *  @b Description
 *  @n
 *      The function is used to get the status of the SRIO lane configuration.
 *      Allows user to confirm setting of the lane configuration against the
 *      actual status shown for the lane configuration.
 *
 *  @param[in]  hSrio
 *      SRIO Handle for the CSL Functional layer.
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0 (Invalid status)
 */
static int32_t displaySrioLanesStatus (CSL_SrioHandle hSrio)
{
    SRIO_LANE_STATUS    ptrLaneStatus;
    Int8                i;
    uint32_t            laneCode=0;
    int32_t             returnStatus=0;
    char                laneSetupText[128];

    for (i = 3; i >= 0; i--)
    {
        CSL_SRIO_GetLaneStatus (hSrio,i,&ptrLaneStatus);
        laneCode = laneCode | (ptrLaneStatus.laneNum << (i * 4));
    }

    switch (laneCode)
    {
        case 0x0000:    /* four 1x ports */
            sprintf (laneSetupText,"four 1x ports");
            break;
        case 0x0010:    /* one 2x port and two 1x ports */
            sprintf (laneSetupText,"one 2x port and two 1x ports");
            break;
        case 0x1000:    /* two 1x ports and one 2x port */
            sprintf (laneSetupText,"two 1x ports and one 2x port");
            break;
        case 0x1010:    /* two 2x ports */
            sprintf (laneSetupText,"two 2x ports");
            break;
        case 0x3210:    /* one 4x port */
            sprintf (laneSetupText,"one 4x port");
            break;
        default: /* four 1x ports */
            sprintf (laneSetupText,"INVALID LANE COMBINATION FOUND");
            returnStatus = -1;
            break;
    }
    printf ("Lanes status shows lanes formed as %s%s", laneSetupText, "\n");

    return returnStatus;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set the SRIO lane configuration.
 *      Prerequisite: Run setEnableSrioPllRxTx() first or it will overwrite the MSYNC setting done
 *                    by this function.
 *
 *  @param[in]  hSrio
 *      SRIO Handle for the CSL Functional layer.
 *  @param[in]  laneMode
 *      Mode number to set the lanes to a specific configuration.
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0 (Invalid lane configuration mode specified)
 */
static int32_t setSrioLanes (CSL_SrioHandle hSrio, srioLanesMode_e laneMode)
{
    Uint8   port, pathMode;
    Uint8   bootCompleteFlag;
    Uint32  serdesTXConfig;
    Uint32  msyncSet = 0x00100000;
    CSL_SRIO_GetBootComplete (hSrio, &bootCompleteFlag);

    if (bootCompleteFlag == 1)
        /* Set boot complete to be 0; to enable writing to the SRIO registers. */
        CSL_SRIO_SetBootComplete (hSrio, 0);

    /* Set the path mode number to the lane configuration specified */
    switch (laneMode)
    {
        case srio_lanes_form_four_1x_ports:                 /* four 1x ports (forms ports: 0,1,2 and 3) */
            pathMode = 0;
            break;
        case srio_lanes_form_one_2x_port_and_two_1x_ports:  /* one 2x port and two 1x ports (forms ports: 0, 2 and 3) */
            pathMode = 1;
            break;
        case srio_lanes_form_two_1x_ports_and_one_2x_port:  /* two 1x ports and one 2x port (forms ports: 0, 1 and 2) */
            pathMode = 2;
            break;
        case srio_lanes_form_two_2x_ports:                  /* two 2x ports (forms ports: 0 and 2) */
            pathMode = 3;
            break;
        case srio_lanes_form_one_4x_port:                   /* one 4x port (forms port: 0) */
            pathMode = 4;
            break;
        default:    /* Invalid lane configuration mode specified */
            return -1;
    }

    /* Configure the path mode for all ports. */
    for (port = 0; port < 4; port++)
    {
        /* Configure the path mode for the port. */
        CSL_SRIO_SetPLMPortPathControlMode (hSrio, port, pathMode);

        /* Get the current SERDES TX config */
        CSL_BootCfgGetSRIOSERDESTxConfig (port,&serdesTXConfig);

        /* Determine the MSYNC bit's setting according to laneMode being used */
        switch (laneMode)
        {
            case srio_lanes_form_four_1x_ports:                     /* four 1x ports (forms ports: 0,1,2 and 3) */
                msyncSet = 0x00100000;
                break;
            case srio_lanes_form_one_2x_port_and_two_1x_ports:      /* one 2x port and two 1x ports (forms ports: 0, 2 and 3) */
                msyncSet = (port != 1 ? 0x00100000 : 0xFFEFFFFF);
                break;
            case srio_lanes_form_two_1x_ports_and_one_2x_port:      /* two 1x ports and one 2x port (forms ports: 0, 1 and 2) */
                msyncSet = (port != 3 ? 0x00100000 : 0xFFEFFFFF);
                break;
            case srio_lanes_form_two_2x_ports:                      /* two 2x ports (forms ports: 0 and 2) */
                msyncSet = (((port != 1) && (port != 3)) ? 0x00100000 : 0xFFEFFFFF);
                break;
            case srio_lanes_form_one_4x_port:                       /* one 4x port (forms port: 0) */
                msyncSet = (port == 0 ? 0x00100000 : 0xFFEFFFFF);
                break;
            default:    /* Invalid lane configuration mode specified */
                return -1;
        }

        /* Set msync for each port according to the lane mode (port width) specified */
        if (msyncSet == 0x00100000)
            serdesTXConfig |= msyncSet;             /* Set MSYNC bit */
        else
            serdesTXConfig &= msyncSet;             /* Clear MSYNC bit */

        /* Write SERDES TX MSYNC bit */
        CSL_BootCfgSetSRIOSERDESTxConfig (port, serdesTXConfig);
    }

    if (bootCompleteFlag == 1)
        /* Set boot complete back to 1; configuration is complete. */
        CSL_SRIO_SetBootComplete (hSrio, 1);

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get PLL value needed to set the proper multiplier
 *      for the SERDES PLL to give the proper SRIO lane link rate.
 *
 *  @param[in]  refClockMhz
 *      Frequency in Megahertz of the input clock to the SERDES PLL (enum)
 *  @param[in]  linkRateGbps
 *      Desired link rate in gigabits per second (enum)
 *
 *  @retval
 *      SRIO SERDES Configuration PLL value
 */
static int32_t getPllValue (srioRefClockMhz_e refClockMhz, srioLaneRateGbps_e linkRateGbps)
{
    int32_t pllValue = 0;

    /* Get PLL value based on reference clock and lane rate */
    switch (refClockMhz)
    {
        case srio_ref_clock_125p00Mhz:
            switch (linkRateGbps)
            {
                case srio_lane_rate_3p125Gbps:
                    pllValue = 0x265; //10 0110 010 1 (12.5x and set enable bit)
                    break;
                default:
                    pllValue = 0x2A1; //10 1010 000 1 (20x and set enable bit)
                    break;
            }
            break;
        case srio_ref_clock_156p25Mhz:
            switch (linkRateGbps)
            {
                case srio_lane_rate_3p125Gbps:
                    pllValue = 0x251; //10 0101 000 1 (10x and set enable bit)
                    break;
                default:
                    pllValue = 0x281; //10 1000 000 1 (16x and set enable bit)
                    break;
            }
            break;
        case srio_ref_clock_250p00Mhz:
            switch(linkRateGbps)
            {
                case srio_lane_rate_3p125Gbps:
                    pllValue = 0x233; //10 0011 001 1 (6.25x and set enable bit)
                    break;
                default:
                    pllValue = 0x251; //10 0101 000 1 (10x and set enable bit)
                    break;
            }
            break;
        case srio_ref_clock_312p50Mhz:
            switch (linkRateGbps)
            {
                case srio_lane_rate_3p125Gbps:
                    pllValue = 0x229; //10 0010 100 1 (5x and set enable bit)
                    break;
                default:
                    pllValue = 0x241; //10 0100 000 1 (8x and set enable bit)
                    break;
            }
            break;
    }

    return pllValue;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set the SRIO SERDES PLL value and the RX and TX
 *      SERDES configuration values for all lanes. Plus the ability to set loopback
 *      mode if needed.
 *
 *  @param[in]  hSrio
 *      SRIO Handle for the CSL Functional layer.
 *  @param[in]  refClockMhz
 *      Frequency in Megahertz of the input clock to the SERDES PLL (enum)
 *  @param[in]  linkRateGbps
 *      Desired link rate in gigabits per second (enum)
 *  @param[in]  loopbackMode
 *      Loopback setting for SRIO lane (enum)
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0 (Invalid pllValue or invalid linkRateGbps)
 */
static int32_t setEnableSrioPllRxTx (CSL_SrioHandle hSrio, srioRefClockMhz_e refClockMhz, srioLaneRateGbps_e linkRateGbps, int isLoopbackMode)
{
    uint32_t pllValue = getPllValue (refClockMhz, linkRateGbps);
    uint32_t rxConfig = 0;
    uint32_t txConfig = 0;
    Uint8   bootCompleteFlag;

    /* Return immediately if pllValue has not been identified and is still zero */
    if (pllValue == 0)
        return -1;

    /* Get Pll setting for single speed of 3.125Gbps or setting for all other speeds of 5.0Gbps, 2.5Gbps, and 1.25Gpbs */
    CSL_BootCfgSetSRIOSERDESConfigPLL (pllValue);

    /* Loop around until the SERDES PLL is locked. */
    while (1)
    {
        uint32_t    status;

        /* Get the SRIO SERDES Status */
        CSL_BootCfgGetSRIOSERDESStatus (&status);
        if (status & 0x1)
            break;
    }

    /* Get boot complete flag setting */
    CSL_SRIO_GetBootComplete (hSrio, &bootCompleteFlag);

    if (bootCompleteFlag == 1)
        /* Set boot complete to be 0; to enable writing to the SRIO registers. */
        CSL_SRIO_SetBootComplete (hSrio, 0);

    /* Set rx/tx config values based on the lane rate specified */
    switch (linkRateGbps)
    {
        case srio_lane_rate_5p000Gbps:  /* Pll setting determines 5.0 Gpbs or 3.125 Gbps */
        case srio_lane_rate_3p125Gbps:  /* Same Tx and Rx settings for 5.0 Gbps or 3.125 Gbps */
            rxConfig = 0x00440495;
                        // (0)     Enable Receiver
                        // (1-3)   Bus Width 010b (20 bit)
                        // (4-5)   Half rate. Two data samples per PLL output clock cycle
                        // (6)     Normal polarity
                        // (7-9)   Termination programmed to be 001
                        // (10-11) Comma Alignment enabled
                        // (12-14) Loss of signal detection disabled
                        // (15-17) First order. Phase offset tracking up to +-488 ppm
                        // (18-20) Fully adaptive equalization
                        // (22)    Offset compensation enabled
                        // (23-24) Loopback disabled
                        // (25-27) Test pattern mode disabled
                        // (28-31) Reserved

            txConfig = 0x00180795;
            //txConfig = 0x00181F15;
                        // (0)     Enable Transmitter
                        // (1-3)   Bus Width 010b (20 bit)
                        // (4-5)   Half rate. Two data samples per PLL output clock cycle
                        // (6)     Normal polarity
                        // (7-10)  Swing max.
                        // (11-13) Precursor Tap weight 0%
                        // (14-18) Adjacent post cursor Tap weight 0%
                        // (19)    Transmitter pre and post cursor FIR filter update
                        // (20)    Synchronization master
                        // (21-22) Loopback disabled
                        // (23-25) Test pattern mode disabled
                        // (26-31) Reserved
            break;
        case srio_lane_rate_2p500Gbps:  /* Tx and Rx settings for 2.50 Gbps */
            rxConfig = 0x004404A5;
                        // (4-5)   Quarter rate. One data sample per PLL output clock cycle

            txConfig = 0x001807A5;
                        // (4-5)   Quarter rate. One data sample per PLL output clock cycle
            break;
        case srio_lane_rate_1p250Gbps:  /* Tx and Rx settings for 1.25 Gbps */
            rxConfig = 0x004404B5;
                        // (4-5)   Eighth rate. One data sample every two PLL output clock cycles

            txConfig = 0x001807B5;
                        // (4-5)   Eighth rate. One data sample every two PLL output clock cycles

            break;
        default:    /* Invalid SRIO lane rate specified */
            return -1;
    }

    /* Return with error if rx/tx configuration values have not been determined and are still zero */
    if ((rxConfig == 0) || (txConfig == 0))
        return -1;

    /* If loop-back mode then set the appropriate bits */
    if (isLoopbackMode)
    {
        /* set RX and TX loop-back bits */
        rxConfig |= 0x01800000;
        txConfig |= 0x00600000;
    }

    /* Configure the SRIO SERDES Receive Configuration. */
    CSL_BootCfgSetSRIOSERDESRxConfig (0, rxConfig);
    CSL_BootCfgSetSRIOSERDESRxConfig (1, rxConfig);
    CSL_BootCfgSetSRIOSERDESRxConfig (2, rxConfig);
    CSL_BootCfgSetSRIOSERDESRxConfig (3, rxConfig);

    /* Configure the SRIO SERDES Transmit Configuration. */
    CSL_BootCfgSetSRIOSERDESTxConfig (0, txConfig);
    CSL_BootCfgSetSRIOSERDESTxConfig (1, txConfig);
    CSL_BootCfgSetSRIOSERDESTxConfig (2, txConfig);
    CSL_BootCfgSetSRIOSERDESTxConfig (3, txConfig);

    if (bootCompleteFlag == 1)
        /* Set boot complete back to 1; configuration is complete. */
        CSL_SRIO_SetBootComplete (hSrio, 0);

    return 0;
}

/**
 *  @b Description
 *  @n
 *      Wait for configured SRIO ports to show operational status.
 *
  *  @param[in]  hSrio
 *      SRIO Handle for the CSL Functional layer.
 *  @param[in]  laneMode
 *      Mode number to determine which ports to check for operational status.
 *
 *  @retval
 *      Success - 0
 */
static int32_t waitAllSrioPortsOperational (CSL_SrioHandle hSrio, srioLanesMode_e laneMode)
{
    Uint8       port;
    Uint8       portsMask=0xF;      // 0b1111
    char        statusText[4]="";

    /* Set port mask to use based on the lane mode specified */
    switch (laneMode)
    {
        case srio_lanes_form_four_1x_ports:                 /* check ports 0 to 3 */
            portsMask = 0xF;    // 0b1111
            break;
        case srio_lanes_form_one_2x_port_and_two_1x_ports:  /* check ports 0, 2, and 3 */
            portsMask = 0xD;    // 0b1101
            break;
        case srio_lanes_form_two_1x_ports_and_one_2x_port:  /* check ports 0, 1, and 2 */
            portsMask = 0x7;    // 0b0111
            break;
        case srio_lanes_form_two_2x_ports:                  /* check ports 0 and 2 */
            portsMask = 0x5;    // 0b0101
            break;
        case srio_lanes_form_one_4x_port:                   /* check port 0 */
            portsMask = 0x1;    // 0b0001
            break;
        default: /* check ports 0 to 3 */
            portsMask = 0xF;    // 0b1111
            break;
    }

    /* Wait for all SRIO ports for specified lane mode to be operational */
    //IFPRINT(printf ("Waiting for SRIO ports to be operational...  \n"));
    //tscTemp = CSL_tscRead () + 5000000000;
    unsigned int tt = TSCL + 4000000000;
    for (port = 0; port < 4; port++)
    {
        if ((portsMask >> port) & 0x1 == 1)
        {
            sprintf (statusText, "NOT ");
            /* Check for good port status on each valid port, timeout if not received after 5 seconds */
            while (TSCL < tt)
            {
                if (CSL_SRIO_IsPortOk (hSrio, port) == TRUE)
                {
                    sprintf (statusText,"");
                    break;
                }
            }
            printf ("SRIO port %d is %s operational.\n", port, statusText);
            //tscTemp = CSL_tscRead() + 1000000000;
            tt = TSCL + 1000000000;
        }
    }

    return 0;
}
/**
 *  @b Description
 *  @n
 *      The function provides the initialization sequence for the SRIO IP
 *      block. This can be modified by customers for their application and
 *      configuration.
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
int32_t SrioDevice_init (void)
{
    CSL_SrioHandle      hSrio;
	int32_t             i;
    SRIO_PE_FEATURES    peFeatures;
    SRIO_OP_CAR         opCar;
//    Qmss_QueueHnd       queueHnd;
    uint8_t             isAllocated;
    Qmss_QueueHnd gargbageQueue[6];

    /* Disable SRIO reset isolation */
    if (CSL_PSC_isModuleResetIsolationEnabled(CSL_PSC_LPSC_SRIO))
    	CSL_PSC_disableModuleResetIsolation(CSL_PSC_LPSC_SRIO);

    /* Reset SRIO module and wait for the reset to complete */
    CSL_PSC_setModuleLocalReset (CSL_PSC_LPSC_SRIO,PSC_MDLRST_ASSERTED);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_SRIO,PSC_MODSTATE_ENABLE);
    //printf ("Debug: Waiting for module reset...\n");
    while (!CSL_PSC_isModuleResetDone(CSL_PSC_LPSC_SRIO));
    //printf ("Debug: Waiting for module local reset...\n");
    while (!CSL_PSC_isModuleLocalResetDone (CSL_PSC_LPSC_SRIO));

    /* Get the CSL SRIO Handle. */
    hSrio = CSL_SRIO_Open (0);
    if (hSrio == NULL)
        return -1;
	//CSL_SRIO_SetLSUSwappingMode(hSrio, 2);
	//CSL_SRIO_SetMAUSwappingMode(hSrio, 2);
	//CSL_SRIO_SetTXURXUSwappingMode(hSrio, 2);
    /* Enable reception of packets */
	hSrio->RIO_PCR = (hSrio->RIO_PCR & 0x7) | CSL_SRIO_RIO_PCR_RESTORE_MASK;

    /* Disable the SRIO Global block */
	CSL_SRIO_GlobalDisable (hSrio);

	/* Disable each of the individual SRIO blocks. */
	for(i = 0; i <= 9; i++)
		CSL_SRIO_DisableBlock (hSrio, i);

	/* Set boot complete to be 0; we are not done with the initialization. */
	CSL_SRIO_SetBootComplete (hSrio, 0);

    /* Set the sRIO shadow registers for 9/3/2/2 */
    CSL_SRIO_SetLSUShadowRegs (hSrio,0x19,0x19);

	/* Now enable the SRIO block and all the individual blocks also. */
	CSL_SRIO_GlobalEnable (hSrio);
	for (i = 0; i <= 9; i++)
		CSL_SRIO_EnableBlock (hSrio,i);

	if (0)
	{
		/* Configure SRIO to operate in Loopback mode. */
		CSL_SRIO_SetLoopbackMode (hSrio,0);
		CSL_SRIO_SetLoopbackMode (hSrio,1);
		CSL_SRIO_SetLoopbackMode (hSrio,2);
		CSL_SRIO_SetLoopbackMode (hSrio,3);
	}
	else
	{
		/* Configure SRIO to operate in Normal mode. */
		CSL_SRIO_SetNormalMode (hSrio,0);
		CSL_SRIO_SetNormalMode (hSrio,1);
		CSL_SRIO_SetNormalMode (hSrio,2);
		CSL_SRIO_SetNormalMode (hSrio,3);
	}

	/* Enable Automatic Priority Promotion of response packets. */
	CSL_SRIO_EnableAutomaticPriorityPromotion (hSrio);

	/* Set the SRIO Prescalar select to operate in the range of 44.7 to 89.5 */
	CSL_SRIO_SetPrescalarSelect (hSrio, 0);

    /* Unlock the Boot Configuration Kicker */
    CSL_BootCfgUnlockKicker ();

	if (setEnableSrioPllRxTx(hSrio, (srioRefClockMhz_e)SRIO_REF_CLOCK, (srioLaneRateGbps_e)SRIO_LANE_SPEED, 0) < 0)
		return -1;

    /* Loop around until the SERDES PLL is locked. */
    while (1)
    {
        uint32_t    status;

        /* Get the SRIO SERDES Status */
        CSL_BootCfgGetSRIOSERDESStatus (&status);
        if (status & 0x1)
            break;
    }

    /* Clear the LSU pending interrupts. */
    CSL_SRIO_ClearLSUPendingInterrupt (hSrio, 0xFFFFFFFF, 0xFFFFFFFF);

    /* Set the Device Information */
    //CSL_SRIO_SetDeviceInfo (hSrio, 0xFF, DEVICE_VENDOR_ID, DEVICE_REVISION);

    /* Set the Assembly Information */
    CSL_SRIO_SetAssemblyInfo (hSrio, DEVICE_ASSEMBLY_ID, DEVICE_ASSEMBLY_VENDOR_ID,
                             DEVICE_ASSEMBLY_REVISION, DEVICE_ASSEMBLY_INFO);

	/* Configure the processing element features*/
	peFeatures.isBridge                          = 0;
	peFeatures.isEndpoint                        = 0;
	peFeatures.isProcessor                       = 1;
	peFeatures.isSwitch                          = 0;
	peFeatures.isMultiport                       = 0;
	peFeatures.isFlowArbiterationSupported       = 0;
	peFeatures.isMulticastSupported              = 0;
	peFeatures.isExtendedRouteConfigSupported    = 0;
	peFeatures.isStandardRouteConfigSupported    = 1;
	peFeatures.isFlowControlSupported            = 1;
	peFeatures.isCRFSupported                    = 0;
	peFeatures.isCTLSSupported                   = 1;
	peFeatures.isExtendedFeaturePtrValid         = 1;
	peFeatures.numAddressBitSupported            = 1;
	CSL_SRIO_SetProcessingElementFeatures (hSrio, &peFeatures);

	/* Configure the source operation CAR */
	memset ((void *) &opCar, 0, sizeof (opCar));
	opCar.portWriteOperationSupport = 1;
	opCar.atomicClearSupport        = 1;
	opCar.atomicSetSupport          = 1;
	opCar.atomicDecSupport          = 1;
	opCar.atomicIncSupport          = 1;
	opCar.atomicTestSwapSupport     = 1;
	opCar.doorbellSupport           = 1;
	opCar.dataMessageSupport        = 1;
	opCar.writeResponseSupport      = 1;
	opCar.streamWriteSupport        = 1;
	opCar.writeSupport              = 1;
	opCar.readSupport               = 1;
	opCar.dataStreamingSupport      = 1;
	CSL_SRIO_SetSourceOperationCAR (hSrio, &opCar);

	/* Configure the destination operation CAR */
	memset ((void *) &opCar, 0, sizeof (opCar));
	opCar.portWriteOperationSupport  = 1;
	opCar.doorbellSupport            = 1;
	opCar.dataMessageSupport         = 1;
	opCar.writeResponseSupport       = 1;
	opCar.streamWriteSupport         = 1;
	opCar.writeSupport               = 1;
	opCar.readSupport                = 1;
	CSL_SRIO_SetDestOperationCAR (hSrio, &opCar);

	//CSL_SRIO_SetDeviceIDCSR (hSrio, srio_primary_ID, srio_primary_ID);
    /* Enable TLM Base Routing Information for Maintainance Requests & ensure that
     * the BRR's can be used by all the ports. */
    CSL_SRIO_SetTLMPortBaseRoutingInfo (hSrio, 0, 1, 1, 1, 0);
    /* Only add additional route if doing core to core on the same board */
    //if (0)
	//CSL_SRIO_SetTLMPortBaseRoutingInfo (hSrio, 0, 2, 1, 1, 0);

    /* Configure the Base Routing Register to ensure that all packets matching the 
     * Device Identifier & the Secondary Device Id are admitted. */
//    CSL_SRIO_SetTLMPortBaseRoutingPatternMatch (hSrio, 0, 1, device_id_8b[CNUM], srioIDMask);
//	CSL_SRIO_SetTLMPortBaseRoutingPatternMatch (hSrio, 0, 1, srio_primary_ID, srioIDMask);
    /* Only add additional route if doing core to core on the same board */
//    if (0)
//	   CSL_SRIO_SetTLMPortBaseRoutingPatternMatch (hSrio, 0, 2, srio_secondary_ID, srioIDMask);

    /* We need to open the Garbage collection queues in the QMSS. This is done to ensure that 
     * these queues are not opened by another system entity. */
    for (i = 0; i < 6; i++)
    {
        /* Open the Garabage queues */
        //queueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, gargbageQueue[i], &isAllocated);
    	gargbageQueue[i] = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated);
        if (gargbageQueue[i] < 0)
            return -1;

        /* Make sure the queue has not been opened already; we dont the queues to be shared by some other
         * entity in the system. */
        if (isAllocated > 1)
            return -1;
    }

    /* Set the Transmit Garbage Collection Information. */
    CSL_SRIO_SetTxGarbageCollectionInfo (hSrio, gargbageQueue[0], gargbageQueue[1], gargbageQueue[2], gargbageQueue[3], gargbageQueue[4], gargbageQueue[5]);

    /* Set the Host Device Identifier. */
//    CSL_SRIO_SetHostDeviceID (hSrio, srio_primary_ID);

    /* Configure the component tag CSR */
    CSL_SRIO_SetCompTagCSR (hSrio, 0x00000000);

	/* Configure the PLM for all the ports. */
	for (i = 0; i < 4; i++)
	{
		/* Set the PLM Port Silence Timer. */
		CSL_SRIO_SetPLMPortSilenceTimer (hSrio, i, 0x2);

		/* TODO: We need to ensure that the Port 0 is configured to support both
		 * the 2x and 4x modes. The Port Width field is read only. So here we simply
		 * ensure that the Input and Output ports are enabled. */
		CSL_SRIO_EnableInputPort (hSrio, i);
		CSL_SRIO_EnableOutputPort (hSrio, i);

		/* Set the PLM Port Discovery Timer. */
		CSL_SRIO_SetPLMPortDiscoveryTimer (hSrio, i, 0x2);

		/* Reset the Port Write Reception capture. */
		CSL_SRIO_SetPortWriteReceptionCapture (hSrio, i, 0x0);
	}

	/* Set the Port link timeout CSR */
	// CSL_SRIO_SetPortLinkTimeoutCSR (hSrio, 0x000FFF);
	CSL_SRIO_SetPortLinkTimeoutCSR (hSrio, 0xFF0FFF);
	CSL_SRIO_SetPortResponseTimeoutCSR (hSrio, 0xFF0FFF);

	/* Set the Port General CSR: Only executing as Master Enable */
	CSL_SRIO_SetPortGeneralCSR (hSrio, 0, 1, 0);

	/* Clear the sticky register bits. */
	CSL_SRIO_SetLLMResetControl (hSrio, 1);

	/* Set the device id to be 0 for the Maintenance Port-Write operation
	 * to report errors to a system host. */
	CSL_SRIO_SetPortWriteDeviceId (hSrio, 0x0, 0x0, 0x0);

    /* Set the Data Streaming MTU */
    CSL_SRIO_SetDataStreamingMTU (hSrio, 64);

	/* Configure the path mode for the ports. */
	if (setSrioLanes(hSrio, SRIO_PORT_WIDTH) < 0)
		return -1;

	/* Set the LLM Port IP Prescalar. */
	//CSL_SRIO_SetLLMPortIPPrescalar (hSrio, 0x21);
	CSL_SRIO_SetLLMPortIPPrescalar (hSrio, 0x1F);

	/* Configure the ingress watermarks */
	for (i = 0; i < 4; i++)
	{
		CSL_SRIO_SetPBMPortIngressPriorityWatermark (hSrio, i, 0, 0x24, 0x24);
		CSL_SRIO_SetPBMPortIngressPriorityWatermark (hSrio, i, 1, 0x1B, 0x1B);
		CSL_SRIO_SetPBMPortIngressPriorityWatermark (hSrio, i, 2, 0x12, 0x12);
		CSL_SRIO_SetPBMPortIngressPriorityWatermark (hSrio, i, 3, 0x9, 0x9);
	}

    /* Disable interrupt pacing for all interrupt destinations. */
    for (i = 0; i < 24; i++)
        CSL_SRIO_DisableInterruptPacing (hSrio, i);

	/* Set all the queues 0 to operate at the same priority level and to send packets onto Port 0 */
    for (i =0 ; i < 16; i++)
    	CSL_SRIO_SetTxQueueSchedInfo (hSrio, i, 0, 0);

#if 1
    /* Set the Doorbell route to determine which routing table is to be used 
     * This configuration implies that the Interrupt Routing Table is configured as 
     * follows:-
     *  Interrupt Destination 0 - INTDST 16 
     *  Interrupt Destination 1 - INTDST 17 
     *  Interrupt Destination 2 - INTDST 18
     *  Interrupt Destination 3 - INTDST 19 */
    CSL_SRIO_SetDoorbellRoute (hSrio, 0);
#else
    /* Set the Doorbell route to determine which routing table is to be used 
     * This configuration implies that the Interrupt Routing Table is configured as 
     * follows:-
     *  Interrupt Destination 0 - INTDST 0
     *  Interrupt Destination 1 - INTDST 1
     *  Interrupt Destination 2 - INTDST 2
     *  Interrupt Destination 3 - INTDST 3 */
   	CSL_SRIO_SetDoorbellRoute (hSrio, 1);
#endif    

    /* Route the Doorbell interrupts. 
     *  Doorbell Register 0 - All 16 Doorbits are routed to Interrupt Destination 0. 
     *  Doorbell Register 1 - All 16 Doorbits are routed to Interrupt Destination 1. 
     *  Doorbell Register 2 - All 16 Doorbits are routed to Interrupt Destination 2. 
     *  Doorbell Register 3 - All 16 Doorbits are routed to Interrupt Destination 3. */
    for (i = 0; i < 16; i++)
    {
        CSL_SRIO_RouteDoorbellInterrupts (hSrio, 0, i, 0);
        CSL_SRIO_RouteDoorbellInterrupts (hSrio, 1, i, 0);
        CSL_SRIO_RouteDoorbellInterrupts (hSrio, 2, i, 0);
        CSL_SRIO_RouteDoorbellInterrupts (hSrio, 3, i, 0);
    }
	/* Enable the peripheral. */
	CSL_SRIO_EnablePeripheral (hSrio);
    /* Configuration has been completed. */
    CSL_SRIO_SetBootComplete (hSrio, 1);
    /* Check Ports and make sure they are operational. */
	if (waitAllSrioPortsOperational(hSrio, SRIO_PORT_WIDTH) < 0)
	{
		printf("Wait for srio port operational failed!\n");
		return -1;
	}
    if (displaySrioLanesStatus(hSrio) < 0)
    {
    	printf("Display Srio Lanes Status failed!\n");
    }
    /* Initialization has been completed. */
    return 0;
}

/**
@}
*/

