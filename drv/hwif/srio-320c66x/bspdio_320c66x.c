#include "srio.h"


/****************************************************************************************
 ****************************************************************************************
 * Create global dio operation Socket
 *
 *
 ****************************************************************************************
 ****************************************************************************************
 * */
Srio_SockHandle RioSockCreate(Srio_DrvHandle hdev)
{
	/* Open DIO SRIO Socket */
	Srio_SockHandle dioSocket;
	Srio_SockBindAddrInfo   dioBindInfo;

	dioSocket = Srio_sockOpen (hdev, Srio_SocketType_DIO, TRUE);
	if (dioSocket == NULL)
	{
	     printf ("Error: DIO Control Socket open failed\n");
	     //return -1;
	}
    /* DIO Binding Information: Use 16 bit identifiers and we are bound to the first source id.
     * and we are using 16 bit device identifiers. */
	dioBindInfo.dio.doorbellValid  = 0;
    dioBindInfo.dio.intrRequest    = 0;
    dioBindInfo.dio.supInt         = 0;
    dioBindInfo.dio.xambs          = 0;
    dioBindInfo.dio.priority       = 0;
    dioBindInfo.dio.outPortID      = 0;
    dioBindInfo.dio.idSize         = 0;
    dioBindInfo.dio.srcIDMap       = 0;
    dioBindInfo.dio.hopCount       = 0;
    dioBindInfo.dio.doorbellReg    = 0;
    dioBindInfo.dio.doorbellBit    = 0;
	/* Bind the DIO socket. */
    if (Srio_sockBind (dioSocket, &dioBindInfo) < 0)
    {
        printf ("Error: Binding the DIO Control Socket failed.\n");
	    //return -1;
    }
    return dioSocket;
}

int RioSockDel(Srio_SockHandle srioSock)
{
	Srio_sockClose(srioSock);
	return 1;
}
/****************************************************************************************
 ****************************************************************************************
 * Do a dio Nwrite operation
 *
 *
 ****************************************************************************************
 ****************************************************************************************
 * */
int RioNwrite(Srio_DrvHandle hdev, Srio_SockHandle dioSocket, uint32_t *srcBuff, uint32_t dstBuff, uint32_t length, uint32_t dstID)
{
	Srio_SockAddrInfo       to;
    uint32_t *Ptr1;
#if 0
	Srio_SockHandle         dioSocket;
	Srio_SockBindAddrInfo   dioBindInfo;
	/* Open DIO SRIO Socket */
	dioSocket = Srio_sockOpen (hdev, Srio_SocketType_DIO, TRUE);
	if (dioSocket == NULL)
	{
	     printf ("Error: DIO Control Socket open failed\n");
	     return -1;
	}
    /* DIO Binding Information: Use 16 bit identifiers and we are bound to the first source id.
     * and we are using 16 bit device identifiers. */
	dioBindInfo.dio.doorbellValid  = 0;
    dioBindInfo.dio.intrRequest    = 0;
    dioBindInfo.dio.supInt         = 0;
    dioBindInfo.dio.xambs          = 0;
    dioBindInfo.dio.priority       = 0;
    dioBindInfo.dio.outPortID      = 0;
    dioBindInfo.dio.idSize         = 0;
    dioBindInfo.dio.srcIDMap       = 0;
    dioBindInfo.dio.hopCount       = 0;
    dioBindInfo.dio.doorbellReg    = 0;
    dioBindInfo.dio.doorbellBit    = 0;
	/* Bind the DIO socket. */
    if (Srio_sockBind (dioSocket, &dioBindInfo) < 0)
    {
        printf ("Error: Binding the DIO Control Socket failed.\n");
	    return -1;
    }
#endif
    /* Program the DIO Destination Information.
     *  - The DIO consumer memory block is located at the end of the Local memory. */
    to.dio.rapidIOMSB    = 0x0;
    to.dio.ttype         = Srio_Ttype_Write_NWRITE;
    to.dio.ftype         = Srio_Ftype_WRITE;
    to.dio.dstID         = dstID;

	to.dio.rapidIOLSB    = (uint32_t)dstBuff;
	Ptr1 = (uint32_t *)((uint32_t)srcBuff);
	/* Initiate the transfer. */
	if (Srio_sockSend (dioSocket, Ptr1, length, &to) < 0)
	{
		printf ("Error: Unable to send payload over DIO socket\n");
			return -1;
	}
	//Srio_sockClose(dioSocket);
	return 0;
}

/****************************************************************************************
 ****************************************************************************************
 * Do a dio Nread operation
 *
 *
 ****************************************************************************************
 ****************************************************************************************
 * */
int RioNread(Srio_DrvHandle hdev, Srio_SockHandle dioSocket, uint32_t *srcBuff, uint32_t dstBuff, uint32_t length, uint32_t dstID)
{

	Srio_SockAddrInfo       to;
    uint32_t *Ptr1;
#if 0
	Srio_SockHandle         dioSocket;
	Srio_SockBindAddrInfo   dioBindInfo;
	/* Open DIO SRIO Socket */
	dioSocket = Srio_sockOpen (hdev, Srio_SocketType_DIO, TRUE);
	if (dioSocket == NULL)
	{
	     printf ("Error: DIO Control Socket open failed\n");
	     return -1;
	}
    /* DIO Binding Information: Use 16 bit identifiers and we are bound to the first source id.
     * and we are using 16 bit device identifiers. */
	dioBindInfo.dio.doorbellValid  = 0;
    dioBindInfo.dio.intrRequest    = 0;
    dioBindInfo.dio.supInt         = 0;
    dioBindInfo.dio.xambs          = 0;
    dioBindInfo.dio.priority       = 0;
    dioBindInfo.dio.outPortID      = 0;
    dioBindInfo.dio.idSize         = 0;
    dioBindInfo.dio.srcIDMap       = 0;
    dioBindInfo.dio.hopCount       = 0;
    dioBindInfo.dio.doorbellReg    = 0;
    dioBindInfo.dio.doorbellBit    = 0;
	/* Bind the DIO socket. */
    if (Srio_sockBind (dioSocket, &dioBindInfo) < 0)
    {
        printf ("Error: Binding the DIO Control Socket failed.\n");
	    return -1;
    }
#endif
    /* Program the DIO Destination Information.
     *  - The DIO consumer memory block is located at the end of the Local memory. */
    to.dio.rapidIOMSB    = 0x0;
    to.dio.ttype         = Srio_Ttype_Request_NREAD;
    to.dio.ftype         = Srio_Ftype_REQUEST;
    to.dio.dstID         = dstID;

	to.dio.rapidIOLSB    = (uint32_t)dstBuff;
	Ptr1 = (uint32_t *)((uint32_t)srcBuff);
	/* Initiate the transfer. */
	if (Srio_sockSend (dioSocket, Ptr1, length, &to) < 0)
	{
		printf ("Error: Unable to send payload over DIO socket\n");
		return -1;
	}
	//Srio_sockClose(dioSocket);
	return 0;
}

