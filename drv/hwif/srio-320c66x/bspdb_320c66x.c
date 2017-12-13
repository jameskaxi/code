/*
 * srio_doorbell.c
 *
 *  Created on: 2013-10-25
 *      Author: Administrator
 */
#include "srio.h"

/* Global SRIO Driver Handle. */
extern Srio_DrvHandle      hSrioDriver;

#if 0
/**
 *  @b Description
 *  @n
 *      The function creates the management socket.
 *
 *  @retval
 *      Not Applicable.
 */
Srio_SockHandle createMgmtSocket(void)
{
    Srio_SockHandle         mgmtSocket;
    Srio_SockBindAddrInfo   bindInfo;
    uint32_t                doorbellInfo;
    /* Create the Management Socket. */
    mgmtSocket = Srio_sockOpen (hSrioDriver, Srio_SocketType_DIO, 1);
    if (mgmtSocket == NULL)
    {
        printf ("Error: Unable to open the DIO Management socket\n");
        return NULL;
    }

    /* DIO Binding Information: Use 16/8 bit identifiers and we are bound to the first source id.
     * and we are using 16/8 bit device identifiers. */
    bindInfo.dio.doorbellValid  = 0;
    bindInfo.dio.intrRequest    = 0;
    bindInfo.dio.supInt         = 0;
    bindInfo.dio.xambs          = 0;
    bindInfo.dio.priority       = 0;
    bindInfo.dio.outPortID      = 0;
    bindInfo.dio.idSize         = 0;
    bindInfo.dio.srcIDMap       = 0;
    bindInfo.dio.hopCount       = 0;
    bindInfo.dio.doorbellReg    = 0;
    bindInfo.dio.doorbellBit    = 0;

    /* Bind the DIO socket. */
    if (Srio_sockBind (mgmtSocket, &bindInfo) < 0)
    {
        printf ("Error: Binding the DIO Management Socket failed.\n");
        return NULL;
    }

	/* Register the CONSUMER Doorbell Information */
	doorbellInfo = SRIO_SET_DBELL_INFO(0, 1);

    if (Srio_setSockOpt (mgmtSocket, Srio_Opt_REGISTER_DOORBELL, (void *)&doorbellInfo, sizeof(uint32_t)) < 0)
    {
        printf ("Error: Unable to register the Doorbell\n");
        return NULL;
    }
    /* Return the management socket. */
    return mgmtSocket;
}
#endif
/**
 *  @b Description
 *  @n
 *      Utility Function which sends a PING reply to the PRODUCER
 *      This is used to synchronize the PRODUCER and CONSUMER
 *      applications
 *
 *  @retval
 *      Not Applicable.
 */
int32_t RioDoorbell(Srio_DrvHandle hdev, uint16_t dstID, uint16_t doorbellReg, uint16_t doorbellBit)
{
    Srio_SockAddrInfo       to;
    uint32_t                doorbellInfo;
    Srio_SockHandle         mgmtSocket;
    Srio_SockBindAddrInfo   bindInfo;

    /* Create the Management Socket. */
    mgmtSocket = Srio_sockOpen (hdev, Srio_SocketType_DIO, FALSE);
    if (mgmtSocket == NULL)
    {
        printf ("Error: Unable to open the DIO Management socket\n");
        return NULL;
    }

    /* DIO Binding Information: Use 16/8 bit identifiers and we are bound to the first source id.
     * and we are using 16/8 bit device identifiers. */
    bindInfo.dio.doorbellValid  = 0;
    bindInfo.dio.intrRequest    = 0;
    bindInfo.dio.supInt         = 0;
    bindInfo.dio.xambs          = 0;
    bindInfo.dio.priority       = 0;
    bindInfo.dio.outPortID      = 0;
    bindInfo.dio.idSize         = 0;
    bindInfo.dio.srcIDMap       = 0;
    bindInfo.dio.hopCount       = 0;
    bindInfo.dio.doorbellReg    = 0;
    bindInfo.dio.doorbellBit    = 0;
    /* Bind the DIO socket. */
    if (Srio_sockBind (mgmtSocket, &bindInfo) < 0)
    {
        printf ("Error: Binding the DIO Management Socket failed.\n");
        return -1;
    }
#if 1
	/* Register the CONSUMER Doorbell Information */
	doorbellInfo = SRIO_SET_DBELL_INFO(doorbellReg, doorbellBit);

    if (Srio_setSockOpt (mgmtSocket, Srio_Opt_REGISTER_DOORBELL, (void *)&doorbellInfo, sizeof(uint32_t)) < 0)
    {
        printf ("Error: Unable to register the Doorbell\n");
        return -1;
    }
#endif
    /* Program the destination information: */
    to.dio.rapidIOMSB    = 0x0;
    to.dio.rapidIOLSB    = 0x0;
    to.dio.dstID         = dstID;
    to.dio.ttype         = 0;
    to.dio.ftype         = Srio_Ftype_DOORBELL;

    /* Use the SRIO Driver Macro to program the doorbell information */
    doorbellInfo = SRIO_SET_DBELL_INFO(doorbellReg, doorbellBit);

    /* Send the Doorbell. */
    if (Srio_sockSend (mgmtSocket, (Srio_DrvBuffer)doorbellInfo, 0, &to) < 0)
    {
        printf("Error: Unable to send doorbell.\n");
        return -1;
    }
    Srio_sockClose(mgmtSocket);
    return 0;
}

