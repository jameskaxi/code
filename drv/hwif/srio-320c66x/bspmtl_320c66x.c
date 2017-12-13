/**
 *   @file  srio_mtl_debug.c
 *
 *   @brief
 *      maintenance packages debug switch.
 *
 */
#include "srio.h"


/**
 *  @b Description
 *  @n
 *      This is the producer maintenance debug control function.
 *		to read switch 0x00, 0x14
 *  @retval
 *      Not Applicable.
 */
int32_t RioMaintWrite(Srio_DrvHandle hdev, uint32_t offset, uint8_t hopCount, uint16_t destId, uint32_t writeData)
{
    Srio_SockHandle         controlSocket;
    Srio_SockAddrInfo       to;
    Srio_SockBindAddrInfo	bindInfo;
    void* hSrcBuffer;

    hSrcBuffer = (void*)Memory_alloc(NULL, 128, 128, 0);
    memcpy(hSrcBuffer, (void*)&writeData, 0x04);
    CACHE_wbL2((void*)hSrcBuffer, 128, CACHE_WAIT);
	CACHE_wbL1d((void*)hSrcBuffer, 128, CACHE_WAIT);

    /* Open DIO SRIO Socket */
    controlSocket = Srio_sockOpen (hdev, Srio_SocketType_DIO, 1);
    if (controlSocket == NULL)
    {
        printf ("Error: DIO Control Socket open failed\n");
        Memory_free(NULL, hSrcBuffer, 128);
        return -1;
    }
    /* DIO Binding Information: Use 16 bit identifiers and we are bound to the first source id.
     * and we are using 16 bit device identifiers. */
    bindInfo.dio.doorbellValid  = 0;
    bindInfo.dio.intrRequest    = 0;
    bindInfo.dio.supInt         = 0;
    bindInfo.dio.xambs          = 0;
    bindInfo.dio.priority       = 0;
    bindInfo.dio.outPortID      = 0;
    bindInfo.dio.idSize         = 0;
    bindInfo.dio.srcIDMap       = 0;
    bindInfo.dio.hopCount       = hopCount;
    bindInfo.dio.doorbellReg    = 0;
    bindInfo.dio.doorbellBit    = 0;
    /* Bind the DIO socket. */
    if (Srio_sockBind (controlSocket, &bindInfo) < 0){
        printf ("Error: Binding the DIO Control Socket failed.\n");
    	Memory_free(NULL, hSrcBuffer, 128);
        /* Close the socket. */
        Srio_sockClose (controlSocket);
        return -1;
    }
	//maintenance test
	to.dio.rapidIOMSB    = 0x0;
	to.dio.rapidIOLSB    = offset;
	to.dio.dstID         = destId;
	to.dio.ttype         = Srio_Ttype_Maintenance_WRITE;
	to.dio.ftype         = Srio_Ftype_MAINTENANCE;
	if ((Srio_sockSend (controlSocket, (Srio_DrvBuffer)hSrcBuffer, 4, &to)) < 0){
		printf ("Error: Unable to send payload over DIO socket\n");
		Memory_free(NULL, hSrcBuffer, 128);
	    /* Close the socket. */
	    Srio_sockClose (controlSocket);
		return -1;
	}
	Memory_free(NULL, hSrcBuffer, 128);
    /* Close the socket. */
    Srio_sockClose (controlSocket);

    return 0;
}


/**
 *  @b Description
 *  @n
 *      This is the producer maintenance debug control function.
 *		to read switch
 *  @retval
 *      Not Applicable.
 */
int32_t RioMaintRead(Srio_DrvHandle hdev, uint32_t offset, uint8_t hopCount, uint16_t destId, uint32_t *result)
{
    Srio_SockHandle         controlSocket;
    Srio_SockAddrInfo       to;
    Srio_SockBindAddrInfo	bindInfo;
    void* hSrcBuffer;

    hSrcBuffer = (void*)Memory_alloc(NULL, 128, 128, 0);
    /* Open DIO SRIO Socket */
    controlSocket = Srio_sockOpen (hdev, Srio_SocketType_DIO, 1);
    if (controlSocket == NULL){
        printf ("Error: DIO Control Socket open failed\n");
        return -1;
    }
    /* DIO Binding Information: Use 16 bit identifiers and we are bound to the first source id.
     * and we are using 16 bit device identifiers. */
    bindInfo.dio.doorbellValid  = 0;
    bindInfo.dio.intrRequest    = 0;
    bindInfo.dio.supInt         = 0;
    bindInfo.dio.xambs          = 0;
    bindInfo.dio.priority       = 0;
    bindInfo.dio.outPortID      = 0;
    bindInfo.dio.idSize         = 0;
    bindInfo.dio.srcIDMap       = 0;
    bindInfo.dio.hopCount       = hopCount;
    bindInfo.dio.doorbellReg    = 0;
    bindInfo.dio.doorbellBit    = 0;
    /* Bind the DIO socket. */
    if (Srio_sockBind (controlSocket, &bindInfo) < 0)
    {
        printf ("Error: Binding the DIO Control Socket failed.\n");

    	Memory_free(NULL, hSrcBuffer, 128);
        /* Close the socket. */
        Srio_sockClose (controlSocket);
        return -1;
    }
	to.dio.rapidIOMSB    = 0x0;
	to.dio.rapidIOLSB    = offset;
	to.dio.dstID         = destId;
	to.dio.ttype         = Srio_Ttype_Maintenance_READ;
	to.dio.ftype         = Srio_Ftype_MAINTENANCE;
	if (Srio_sockSend (controlSocket, (Srio_DrvBuffer)hSrcBuffer, 4, &to) < 0)
	{
		printf ("Error: Unable to send payload over DIO socket\n");
		Memory_free(NULL, hSrcBuffer, 128);
	    /* Close the socket. */
	    Srio_sockClose (controlSocket);
		return -1;
	}
	CACHE_invL1d((void*)hSrcBuffer, 128, CACHE_WAIT);
	CACHE_invL2((void*)hSrcBuffer, 128, CACHE_WAIT);
	memcpy((void*)result, (void*)hSrcBuffer, 0x04);
	Memory_free(NULL, hSrcBuffer, 128);
    /* Close the socket. */
    Srio_sockClose (controlSocket);
    return 0;
}
