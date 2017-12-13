/*
 * bsp-psc320c66x.c
 *
 *  Created on: 2014-4-19
 *      Author: Administrator
 */
#include <ti/csl/tistdtypes.h>
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>
#include <ti/csl/cslr_device.h>
#include <stdint.h>

/*===============================PSC===================================*/
/*****************************************************************************
 Prototype    : KeyStone_enable_PSC_module
 Description  : Enable the PSC module in KeyStone device
 Input        : Uint32 pwrDmnNum
                Uint32 moduleNum
 Output       : None
 Return Value :

  History        :
  1.Date         : 2010/12/12
    Author       : Brighton Feng
    Modification : Created function

*****************************************************************************/
int KeyStone_enable_PSC_module (unsigned int pwrDmnNum, unsigned int moduleNum)
{
    /* Set Power domain to ON */
    CSL_PSC_enablePowerDomain (pwrDmnNum);

    /* Enable the clocks too*/
    CSL_PSC_setModuleNextState (moduleNum, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));

    /* Return PSC status */
    if ((CSL_PSC_getPowerDomainState(pwrDmnNum) == PSC_PDSTATE_ON) &&
        (CSL_PSC_getModuleState (moduleNum) == PSC_MODSTATE_ENABLE))
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }
}
/*****************************************************************************
 Prototype    : KeyStone_disable_PSC_module
 Description  : Disable the PSC module in KeyStone device
 Input        : Uint32 pwrDmnNum
                Uint32 moduleNum
 Output       : None
 Return Value :

  History        :
  1.Date         : 2010/2/12
    Author       : Brighton Feng
    Modification : Created function

*****************************************************************************/
int KeyStone_disable_PSC_module (unsigned int pwrDmnNum, unsigned int moduleNum)
{
    /* disable the clocks*/
    CSL_PSC_setModuleNextState (moduleNum, PSC_MODSTATE_SWRSTDISABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));

    /* Return PSC status */
    if (CSL_PSC_getModuleState (moduleNum) == PSC_MODSTATE_SWRSTDISABLE)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }

}
/*****************************************************************************
 Prototype    : KeyStone_disable_PSC_Power_Domain
 Description  : Disable the PSC power domain
 Input        : Uint32 pwrDmnNum
 Output       : None
 Return Value :

  History        :
  1.Date         : 2010/12/12
    Author       : Brighton Feng
    Modification : Created function

*****************************************************************************/
int KeyStone_disable_PSC_Power_Domain (unsigned int pwrDmnNum)
{
    /* Set Power domain to OFF */
    CSL_PSC_disablePowerDomain (pwrDmnNum);

    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));

    /* Return PSC status */
    if (CSL_PSC_getPowerDomainState(pwrDmnNum) == PSC_PDSTATE_OFF)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }

}
#if 0
/**
 *  @b Description
 *  @n
 *      This function enables the power/clock domains for SRIO.
 *      The SRIO power domain is turned OFF by default.
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
int32_t enable_srio (void)
{
    /* Set SRIO Power domain to ON */
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_SRIO);

    /* Enable the clocks too for SRIO */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_SRIO, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_SRIO);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_SRIO));

    /* Return SRIO PSC status */
    if ((CSL_PSC_getPowerDomainState(CSL_PSC_PD_SRIO) == PSC_PDSTATE_ON) &&
        (CSL_PSC_getModuleState (CSL_PSC_LPSC_SRIO) == PSC_MODSTATE_ENABLE))
    {
        /* SRIO ON. Ready for use */
        return 0;
    }
    else
    {
        /* SRIO Power on failed. Return error */
        return -1;
    }
}


static int32_t Enable_PSC_Power_Domain (uint32_t pwrDmnNum)
{
	while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* Set Power domain to OFF */
    CSL_PSC_enablePowerDomain (pwrDmnNum);
    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);
    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* Return PSC status */
    if (CSL_PSC_getPowerDomainState(pwrDmnNum) == PSC_PDSTATE_ON)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }

}
static int32_t Disable_PSC_Power_Domain (uint32_t pwrDmnNum)
{
	while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* Set Power domain to OFF */
    CSL_PSC_disablePowerDomain (pwrDmnNum);
    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);
    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* Return PSC status */
    if (CSL_PSC_getPowerDomainState(pwrDmnNum) == PSC_PDSTATE_OFF)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }

}

static int32_t Disable_PSC_module (uint32_t pwrDmnNum, uint32_t moduleNum)
{
	while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* disable the clocks*/
    CSL_PSC_setModuleNextState (moduleNum, PSC_MODSTATE_SWRSTDISABLE);
    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);
    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* Return PSC status */
    if (CSL_PSC_getModuleState (moduleNum) == PSC_MODSTATE_SWRSTDISABLE)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }

}
static int32_t Enable_PSC_module (uint32_t pwrDmnNum, uint32_t moduleNum)
{
	while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* Enable the clocks too*/
    CSL_PSC_setModuleNextState (moduleNum, PSC_MODSTATE_ENABLE);
    /* Start the state transition */
    CSL_PSC_startStateTransition (pwrDmnNum);
    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (pwrDmnNum));
    /* Return PSC status */
    if (CSL_PSC_getModuleState (moduleNum) == PSC_MODSTATE_ENABLE)
    {
        /*Ready for use */
        return 0;
    }
    else
    {
        /*Return error */
        return -1;
    }
}

#define  RIO_PORT_N_ERR_STS_PW_OUT_ES	0x00010000 /* Output Error-stopped */
#define  RIO_PORT_N_ERR_STS_PW_INP_ES	0x00000100 /* Input Error-stopped */
#define  RIO_PORT_N_ERR 0x00000004
#define  RIO_PORT_N_MNT_RSP_RVAL	0x80000000 /* Response Valid */
#define  RIO_PORT_N_MNT_RSP_ASTAT	0x000007e0 /* ackID Status */
#define  RIO_PORT_N_MNT_RSP_LSTAT	0x0000001f /* Link Status */
#define  RIO_PORT_N_ACK_INBOUND		0x3f000000
#define  RIO_PORT_N_ACK_OUTSTAND	0x00003f00
#define  RIO_PORT_N_ACK_OUTBOUND	0x0000003f
void RioPortCheck(uint32_t uiLocalPort, uint32_t uiRemotePort)
{
	CSL_SrioHandle hSrio;
	uint32_t ackid, result, portstate, checkcount, regval;
	uint32_t data;
	uint32_t far_ackid, far_linkstat, near_ackid;
    /* Get the CSL SRIO Handle. */
	TSCL = 0;

	hSrio = CSL_SRIO_Open (0);
#if 0
	//reset partner
	hSrio->RIO_SP[uiLocalPort].RIO_SP_LM_REQ = 0x03;
	while (hSrio->RIO_SP[uiLocalPort].RIO_SP_LM_RESP & RIO_PORT_N_MNT_RSP_RVAL)
	{
		asm(" nop 5");
		board_delay_cycles(5000000);
	}
	//reset self
	hSrio->RIO_SP[uiLocalPort].RIO_SP_CTL = 0xD0E00001;
	hSrio->RIO_SP[uiLocalPort].RIO_SP_CTL = 0xD0600001;
	hSrio->RIO_PLM[uiLocalPort].RIO_PLM_SP_LONG_CS_TX1 = 0x2003F044;
	board_delay_cycles(5000000);
#else

	hSrio->RIO_SP[uiLocalPort].RIO_SP_LM_REQ = 0x04;
	while (hSrio->RIO_SP[uiLocalPort].RIO_SP_LM_RESP & RIO_PORT_N_MNT_RSP_RVAL)
	{
		asm(" nop 5");
		board_delay_cycles(5000000);
	}
	board_delay_cycles(5000000);
	regval = hSrio->RIO_SP[uiLocalPort].RIO_SP_LM_RESP;
	portstate = hSrio->RIO_SP[uiLocalPort].RIO_SP_ERR_STAT;
	if(portstate & RIO_PORT_N_ERR)
		hSrio->RIO_SP[uiLocalPort].RIO_SP_ERR_STAT = 0x4;
	if(portstate & RIO_PORT_N_ERR_STS_PW_OUT_ES)
	{
		far_ackid = (regval & RIO_PORT_N_MNT_RSP_ASTAT) >> 5;
		far_linkstat = regval & RIO_PORT_N_MNT_RSP_LSTAT;

		regval = hSrio->RIO_SP[uiLocalPort].RIO_SP_ACKID_STAT;
		near_ackid = (regval & RIO_PORT_N_ACK_INBOUND) >> 24;


		hSrio->RIO_SP[uiLocalPort].RIO_SP_ACKID_STAT = ((near_ackid << 24) | (far_ackid << 8) | far_ackid);
		far_ackid++;
		srioMaintenance_Write(0x148+0x20*uiRemotePort, 0, 0xff, (far_ackid << 24) | (near_ackid << 8) | near_ackid);
	}

	if(portstate & RIO_PORT_N_ERR_STS_PW_INP_ES)
	{
		srioMaintenance_Write(0x140+0x20*uiRemotePort, 0, 0xff, 4);
		srioMaintenance_Read(0x158+0x20*uiRemotePort, 0, 0xff, &regval);
		IFPRINT(printf("regVal is %x\n", regval));
	}
	srioMaintenance_Write(0x158+0x20*uiRemotePort, 0, 0xff, 0xFFFFFFFF);
	srioMaintenance_Read(0x158+0x20*uiRemotePort, 0, 0xff, &regval);
	IFPRINT(printf("regVal is %x\n", regval));
	hSrio->RIO_SP[uiLocalPort].RIO_SP_ERR_STAT = 0xFFFFFFFF;
	board_delay_cycles(5000000);
	hSrio->RIO_SP[uiLocalPort].RIO_SP_LM_REQ = 0x04;
	while (hSrio->RIO_SP[uiLocalPort].RIO_SP_LM_RESP & RIO_PORT_N_MNT_RSP_RVAL)
	{
		asm(" nop 5");
		board_delay_cycles(5000000);
	}
	srioMaintenance_Read(0x158+0x20*uiRemotePort, 0, 0xff, &regval);
	IFPRINT(printf("regVal is %x\n", regval));
#endif
}
#endif
