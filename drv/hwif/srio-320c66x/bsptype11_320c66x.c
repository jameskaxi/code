

/**
 *   @file  srio_type11_tput.c
 *
 *   @brief   
 *      Type-11 Consumer (Receive) and Producer (Transmit) benchmarking throughput and latency
 *      functions. Please see benchmarking.c brief for more details.
 *
 */
#include "srio.h"

//extern


/**
 *  @b Description
 *  @n
 *      This is the consumer Type11 latency test.
 *
 *  @retval
 *      Success - message received from the control socket.
 *  @retval
 *      Error   - 0
 */
int32_t type11MessageRecv(void)
{
    Srio_SockHandle         dataSocket;
    Srio_SockBindAddrInfo   bindInfo;
    Cppi_HostDesc*          ptrHostDesc;
    Srio_SockAddrInfo       from;
    int						num_bytes;
    UInt8*                  ptr_rxDataPayload;
    uint8_t     baseID8Bit;
    uint16_t    baseID16Bit;
    CSL_SrioHandle      hSrio;

    /* Get the CSL SRIO Handle. */
    hSrio = CSL_SRIO_Open (0);
    if (hSrio == NULL)
        return -1;
    /* Get the Device Identifier CSR */
    CSL_SRIO_GetDeviceIDCSR (hSrio, &baseID8Bit, &baseID16Bit);
	/* Create a Control Type11 NON-BLOCKING Socket if polled mode, and a BLOCKING Socket if not polled mode. */
	dataSocket = Srio_sockOpen (hSrioDriver, Srio_SocketType_TYPE11, TRUE);
	if (dataSocket == NULL)
	{
		printf ("Error: RX Data Socket Open Failed\n");
		return -1;
	}
	/* Populate the binding information. */
	bindInfo.type11.tt       = 0;
	bindInfo.type11.id       = baseID8Bit;
	bindInfo.type11.letter   = 2;
	bindInfo.type11.mbox     = 3;
	bindInfo.type11.segMap   = 0;

	/* Bind the socket. */
	if (Srio_sockBind (dataSocket, &bindInfo) < 0)
	{
		printf ("Error: Data Socket Binding Failed.\n");
		return -1;
	}
	while(1)
	{
	    /* Receive the data. */
	    num_bytes = Srio_sockRecv (dataSocket, (Srio_DrvBuffer*)&ptr_rxDataPayload, &from);
	    if (num_bytes < 0)
	    {
	    	printf("Srio_sockRecv failed!\n");
	    	break;
	    }else if(num_bytes > 0)
	    {
	    	printf("type11 memssage received %d\n", num_bytes);
	    }
	    Srio_freeRxDrvBuffer(dataSocket, (Srio_DrvBuffer)ptr_rxDataPayload);
	}
  	Srio_sockClose (dataSocket);
	return 0;
}

int32_t type11MessageSend(void)
{
    Srio_SockHandle         dataSocket;
    Srio_SockBindAddrInfo   bindInfo;
    Srio_SockAddrInfo       to;
    UInt32                  allocatedLen;
    UInt8*                  txData;
    Srio_DrvBuffer          hDrvBuffer;
    int idx;
    uint8_t     baseID8Bit;
    uint16_t    baseID16Bit;
    CSL_SrioHandle      hSrio;


    /* Get the CSL SRIO Handle. */
    hSrio = CSL_SRIO_Open (0);
    if (hSrio == NULL)
        return -1;

    /* Get the Device Identifier CSR */
    CSL_SRIO_GetDeviceIDCSR (hSrio, &baseID8Bit, &baseID16Bit);

    //buffer = (void*)Memory_alloc(NULL, 128, 128, 0);
	/* Create a Control Type11 NON-BLOCKING Socket if polled mode, and a BLOCKING Socket if not polled mode. */
	dataSocket = Srio_sockOpen (hSrioDriver, Srio_SocketType_TYPE11, TRUE);
	if (dataSocket == NULL)
	{
		printf ("Error: RX Data Socket Open Failed\n");
		return -1;
	}
    /* Initialize the core bindings; we use the same mailbox & letter identifiers. */
    bindInfo.type11.tt       = 0;//0 8bit id
    bindInfo.type11.id       = baseID8Bit;//device id
    bindInfo.type11.letter   = 2;
    bindInfo.type11.mbox     = 3;
    bindInfo.type11.segMap   = 0x0;
	/* Bind the socket. */
	if (Srio_sockBind (dataSocket, &bindInfo) < 0)
	{
		printf ("Error: Data Socket Binding Failed.\n");
		return -1;
	}

    /* Get a transmit buffer from the SRIO Driver. */
    hDrvBuffer = Srio_allocTransmitBuffer(hSrioDriver, &txData, &allocatedLen);
    if ((hDrvBuffer == NULL) || (txData == NULL))
    {
    	printf ("Error: Producer Memory Allocation failed.\n");
        return -1;
    }
    /* Create the transmit data payload. */
    for (idx = 0; idx < 100; idx++)
        txData[idx] = 0xA0 | DNUM;

    /* Send the data out. */
    to.type11.tt       = 0;//8bit id
    to.type11.id       = 2;//to id
    to.type11.letter   = 2;
    to.type11.mbox     = 3;
    while(1)
    {
    	 /* Send the data out from the producer core to the consumer core. */
		if (Srio_sockSend (dataSocket, hDrvBuffer, 512, &to) < 0)
		{
		   printf ("Error: SRIO Socket send failed\n");
		}
    }

  	Srio_freeTransmitBuffer(dataSocket,  (Srio_DrvBuffer)hDrvBuffer);
  	Srio_sockClose (dataSocket);
	return 0;
}

