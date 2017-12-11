/**
 *   @file  srioEnumerate.c
 *
 *   @brief
 *      Srio enumerate.
 *
 *   wyk 
 */
#include "srio.h"

uint32_t DeviceID = 1;
uint32_t SwitchID = 0;

/* route Infomation */
typedef struct{
    uint8_t deviceID;
    uint8_t portNum;
}routeInfo;
/* route Table */
typedef struct{
    uint8_t LFT[MAX_DEVICEID];
}rioRouteTable;


typedef struct {
    uint16_t SwitchIdentity;
    uint16_t hopCount;
    uint16_t DeviceID;
    rioRouteTable RouteTable; 
}rioSwitch;

#pragma DATA_SECTION(Switches, ".initFlag");
rioSwitch Switches[MAX_SWITCHES];

extern Srio_DrvHandle      hSrioDriver;

void rioDelay(void)
{
    Task_sleep(10);
}

static STATUS rioConfigurationWrite(uint32_t localport, uint32_t destid, uint32_t hopcount,
    uint32_t offset, uint32_t *writedata)
{
    if(destid == HOST_REGS && hopcount == 0)
    {
        //Access the local device CAR/CSR register
        *(uint32_t *)(C6678_SRIO_CSR_CAR_BASE + offset) = *writedata;
		return 1;
    }else
    {
        //Access the remote device CAR/CSR register
        if(RioMaintWrite(hSrioDriver, offset, hopcount, destid, *writedata) == 0)
        {
			return 1;
		}
		else
		{
			return 0;
		} 
			
    }
}


static STATUS rioConfigurationRead(uint32_t localport, uint32_t destid, uint32_t hopcount,
    uint32_t offset, uint32_t *readdata)
{
    if(destid == HOST_REGS && hopcount == 0)
    {
        //Access the local device CAR/CSR register
        *readdata = *(uint32_t*)(C6678_SRIO_CSR_CAR_BASE + offset);
		return 1;
    }else
    {
        //Access the remote device CAR/CSR register
        if(RioMaintRead(hSrioDriver, offset, hopcount, destid, readdata) == 0)
        {
			return 1;
		}else
		{
			return 0;
		}	
    }
}

/*
 *func: rioAcquireDeviceLock
 *
 *bref: Srio Acquire device lock
 *
 *var:  localport        Local port number 
 *      destid           Destination ID of the processing element 
 *      hopcount         Hop count
 *      hostdeviceid     Host base device ID for the local processing element
 *      *hostlockid      Device ID of the host holding the lock if ERR_LOCK is
 *                       returned 
 *
 */

static STATUS rioAcquireDeviceLock (uint32_t localport,
                                    uint32_t destid, uint32_t hopcount,
                                    uint32_t hostdeviceid, uint32_t *hostlockid
)
{
    int32_t status;
	
    if(localport == 0)
    {
    	status = rioConfigurationWrite(localport, destid, hopcount, HOST_BASE_ID_LOCK, &hostdeviceid);
    	if(status == 1)
    	{
    		rioConfigurationRead(localport, destid, hopcount, HOST_BASE_ID_LOCK, hostlockid);
    	}

    }
    return 1;
}

/*
*func: rioReleaseDeviceLock
*bref: SRIO release device lock
*var:   
*       localport          Local port number [IN]
*       destid             Destination ID of the processing element [IN]
*       hopcount           Hop count [IN]
*       hostdeviceid       Host base device ID for the local processing element[IN]
*       *hostlockid        Device ID of the host holding the lock if ERR_LOCK
*       is returned 
*return value:
*       RIO_SUCCESS        The device lock was released successfully.
*       RIO_ERR_LOCK       Another host already acquired the specified processor
*                          element.
*       RIO_ERR_INVALID_PARAMETER One or more input parameters had an invalid value
*       RIO_ERR_RIO The RapidIO fabric returned a Response Packet with
*       ERROR              Error status returned by this function may 
*                                contain additional information from the Response Packet.
*       RIO_ERR_ACCESS A device-specific hardware interface was unable to generate a maintenance transaction and reported an
*/
static STATUS rioReleaseDeviceLock (uint32_t localport, uint32_t destid, uint32_t hopcount,
uint32_t hostdeviceid, uint32_t *hostlockid)
{
    if(localport == 0)
    {
        rioConfigurationWrite(localport, destid, hopcount, HOST_BASE_ID_LOCK, &hostdeviceid);
		rioConfigurationRead(localport, destid, hopcount, HOST_BASE_ID_LOCK, hostlockid);
        if(*hostlockid == 0xffff)
        {
            return RIO_SUCCESS;
        }else
        {
            return ERR_SLAVE;
        }
    }
    return RIO_SUCCESS;
}
/*
 *func: rioGetSourceOps
 *
 *bref: Srio get source operations
 *
 *var:  localport        Local port number 
 *      destid           Destination ID of the processing element 
 *      hopcount         Hop count
 *      *SourceOperationCAR      SourceOperationCAR
 *
 */

STATUS rioGetSourceOps(uint32_t localport, uint32_t destid,
uint32_t hopcount, uint32_t *SourceOperationCAR)
{
	int32_t status;
    status = rioConfigurationRead(localport, destid, hopcount, SRC_OP, SourceOperationCAR);
	if(status == 0)
		return 0;
	else
		return 1;
}
/*
 *func: rioGetDestOps
 *
 *bref: Srio get source operations
 *
 *var:  localport        Local port number 
 *      destid           Destination ID of the processing element 
 *      hopcount         Hop count
 *      *DstOperationCAR      Destination OperationCAR
 *
 */

STATUS rioGetDestOps(uint32_t localport, uint32_t destid, uint32_t hopcount, uint32_t *DstOperationCAR)
{
	int32_t status;
    status = rioConfigurationRead(localport, destid, hopcount, DEST_OP, DstOperationCAR);
	if(status == 0)
		return 0;
	else
		return 1;
}

/*
 *func: rioSetBaseDeviceId
 *
 *bref: Srio set base device id
 *
 *var:  
 *
 */

STATUS rioSetBaseDeviceId (uint32_t localport, uint32_t destid, uint32_t hopcount, uint32_t newdeviceid)
{
	int32_t status;
	newdeviceid = (newdeviceid << 16);
	status = rioConfigurationWrite(localport, destid, hopcount, BASE_ID, &newdeviceid);
	if(status == 1){
		return 1;
	}else{
		return 0;
	}
    
}

/*
 *func: rioEnumerateHost
 *
 *bref: Srio enumerate Host
 *
 *var:  hostDeviceID
 *
 */
STATUS rioEnumerateHost(uint32_t hostDeviceID)
{
    //Try to acquire the lock
    uint32_t hostlockid;
	int32_t status;
	uint32_t readdata;

#if 1
   	rioConfigurationRead(0, HOST_REGS, 0, HOST_BASE_ID_LOCK, &readdata);
	if(readdata != 0x0000ffff)
	{
		(printf("Device has been enumerated by other device!\n"));
		return 0;		
	}
    rioAcquireDeviceLock(0, HOST_REGS, 0, hostDeviceID, &hostlockid);
    
    while(hostlockid < hostDeviceID)
    {
        rioDelay();
        //Retry lock acquisition
        rioAcquireDeviceLock(0, HOST_REGS, 0, hostDeviceID, &hostlockid);
    }
    if(hostlockid > hostDeviceID)
    {
        //Release the current lock
        rioReleaseDeviceLock(0, HOST_REGS, 0, hostDeviceID, &hostlockid);
        return ERR_SLAVE;
    }
#endif
    //Lock has been acquired so enumeration can begion
    //Assign the default host ID to the host
    if(hostlockid == hostDeviceID)
    {
    	status = rioSetBaseDeviceId(0, HOST_REGS, 0, hostDeviceID);
		if(status == 0)
		{
			(printf("Rio set base device id failed\n"));
			return ERR_SLAVE;
		}
	}

    //Increment the available device ID
    if(DeviceID == hostDeviceID)
    {
        DeviceID ++;
    }
    return RIO_SUCCESS;
}
/*
 *func: rioGetFeature
 *
 *bref: Srio get device feature
 *
 *var:  localport, destid, hopcount, *features
 *
 */
STATUS rioGetFeature(uint32_t localport, uint32_t destid, uint32_t hopcount, uint32_t *features)
{
	int32_t status;
    status = rioConfigurationRead(localport, destid, hopcount, PE_FEAT, features);
	if(status == 0)
		return 0;
	else
		return 1;
}

/*
 *func: rioEnumerateNeighbor
 *
 *bref: Neighbor enumerate
 *
 *var:  localport, destid, hopcount, *switchportinfo
 *
 */
STATUS rioGetSwitchPortInfo(uint32_t localport, uint32_t destid,
        uint32_t hopcount, uint32_t *switchportinfo)
{
	int32_t status;
    status = rioConfigurationRead(localport, destid, hopcount, SWITCH_PORT_INF, switchportinfo);
	if(status == 0)
		return 0;
	else
		return 1;	
}
/*
 *func: rioClearUp
 *
 *bref: System clear up. Reset the previously acquired lock because a master exits
 *      Use hostDeviceID to reset the lock
 *
 *var:  hostDeviceID
 *
 */
STATUS rioClearUp(uint32_t hostDeviceID)
{
    uint32_t hostlockid;
    //Clear the host lock
    if(hostDeviceID > DeviceID -1)
    {
        rioReleaseDeviceLock(0, HOST_REGS, 0, hostDeviceID, &hostlockid);
    }

    //Clear the discovered end point device lock
    while(DeviceID >= 1)
    {
        rioReleaseDeviceLock(0, DeviceID-1, 0, DeviceID-1, &hostlockid);
        DeviceID--;
    }

    //Clear the discovered switch device lock
    while(SwitchID >= 1)
    {
        rioReleaseDeviceLock(0, Switches[SwitchID - 1].DeviceID,
                        Switches[SwitchID - 1].hopCount, \
                        Switches[SwitchID - 1].DeviceID, &hostlockid);
        SwitchID --;
    }

    return RIO_SUCCESS;
}


/*
 *func: rioGetDeviceIdentify
 *
 *bref: Get device identify
 *
 *var:  localport, destid, hopcount, *identify
 *
 */
void rioGetDeviceIdentify(uint32_t localport, uint32_t destid, uint32_t hopcount, uint32_t *identify)
{
    rioConfigurationRead(localport, destid, hopcount, DEV_ID, identify);
}
/*
 *func: rioRouteAddEntry
 *
 *bref: Srio add route table entry
 *
 *var:  localport, destid, hopcount, tableidx, routedestid, routeportno
 *
 */
void rioRouteAddEntry(uint32_t localport, uint32_t destid,
        uint32_t hopcount, uint32_t tableidx, uint32_t portNum,
        uint32_t routedestid, uint32_t routeportno)
{
    if(localport == 0)
    {
        RioMaintWrite(hSrioDriver, RTE_DESTID, hopcount, destid, routedestid);
        RioMaintWrite(hSrioDriver, RTE_PORT, hopcount, destid, routeportno);
        //Enable the port
        RioMaintWrite(hSrioDriver, PORT_CTL + 0x20*portNum, hopcount, destid, 0xD0600001);
    }
}

/*
 *func: rioGetPortErrStatus
 *
 *bref: Srio get port error status
 *
 *var:  hostDeviceID hopcount
 *
 */
void rioGetPortErrStatus(uint32_t localport, uint32_t destid, uint32_t hopcount,uint32_t portNum, uint32_t *portErrorStatus)
{
    rioConfigurationRead(localport, destid, hopcount, PORT_ERR_STAT + 0x20*portNum, portErrorStatus);
}

/*
 *func: rioGetSwitchPortInfo
 *
 *bref: Get switch port information
 *
 *var:  hostDeviceID hopcount
 *
 */
STATUS rioEnumerateNeighbor(uint8_t hostDeviceID, uint8_t hopCount)
{
    uint32_t hostlockid;
    uint32_t PEFeatures;
    uint32_t SwitchPortInfor;
    uint32_t SwitchIdentify;
    uint32_t deviceid, portNum;
    uint32_t curDeviceID, curSwitchID;
    uint32_t PortErrorStatusCSR[20];
    int32_t  status;
	uint32_t readdata;

#if 1
   	rioConfigurationRead(0, RIO_GEN_DFLT_DID, hopCount, HOST_BASE_ID_LOCK, &readdata);
	if(readdata == hostDeviceID)
	{
		(printf("Device has been enumerated by own device!\n"));
		return 1;		
	}else if(readdata != 0x0000ffff)
	{
		(printf("Device has been enumerated by other device!\n"));
	}
	rioAcquireDeviceLock(0, RIO_GEN_DFLT_DID, hopCount, hostDeviceID, \
                    &hostlockid);
    //
    while(hostlockid < hostDeviceID)
    {
        //Delay for a while
        rioDelay();
        //Retry lock acquisition
        rioAcquireDeviceLock(0, RIO_GEN_DFLT_DID, hopCount, hostDeviceID, \
                        &hostlockid);
    }
    //Check to see if there is a master with a larger host device ID
    if(hostlockid > hostDeviceID)
    {
        return ERR_SLAVE;
    }
#endif
    //Lock has been acquired so enumeration can begion

    //Check Source Operation CAR and Destination Operation CAR to see if a Device
    //ID can be assigned
	//Check to see if the device is a switch
    rioGetFeature(0, RIO_GEN_DFLT_DID, hopCount, &PEFeatures);
    (printf("PEFeatures is %x \n", PEFeatures));
    if(PEFeatures & 0xe0000000)
    {
    	(printf("There is a device!\n"));
        //Set the device ID
        status = rioSetBaseDeviceId(0, RIO_GEN_DFLT_DID, hopCount, DeviceID);
		if(status == 0)
		{
			(printf("Set base device ID failed!\n"));
		}	
        //Increment the available device ID
        DeviceID ++;
        if(DeviceID == hostDeviceID)
        {
            DeviceID ++;
        }
    }
    //
    if(PEFeatures & 0x10000000)
    {
        //Read the switch information
        rioGetSwitchPortInfo(0, RIO_GEN_DFLT_DID, hopCount, &SwitchPortInfor);

        //Record the switch device identify
        rioGetDeviceIdentify(0, RIO_GEN_DFLT_DID, hopCount, &SwitchIdentify);
        Switches[SwitchID].SwitchIdentity = (uint16_t)(SwitchIdentify>>16);

        //Bookkeeping for the current switch ID
        curSwitchID = SwitchID;

        //Increment the available switch ID
        SwitchID ++;

        for(deviceid = 1; deviceid < DeviceID; deviceid ++)
        {
            rioRouteAddEntry(0, RIO_GEN_DFLT_DID, hopCount, 0xff, \
                        (SwitchPortInfor & 0x000000FF), deviceid, \
                        (SwitchPortInfor & 0x000000FF));
        }

        //Synchronize the current switch routing table with the global table
        for(deviceid = 1; deviceid < DeviceID; deviceid ++){
            Switches[curSwitchID].RouteTable.LFT[deviceid] = \
                                      ((SwitchPortInfor & 0xff000000)>>24);
        }

        //Update the hopcount to reach the current switch
        Switches[curSwitchID].hopCount = hopCount;

        for(portNum = 0; portNum < ((SwitchPortInfor & 0x0000FF00)>>8); portNum ++)
        {
            if((SwitchPortInfor & 0x000000FF) == portNum)
            {
					continue;
            }

            //Bookkeeping for the current available device ID
            curDeviceID = DeviceID;
            rioGetPortErrStatus(0, RIO_GEN_DFLT_DID, hopCount, portNum, &PortErrorStatusCSR[portNum]);
			printf("Port %d Error&Status CSR is %x\n", portNum, PortErrorStatusCSR[portNum]);
            //Check if it is possible to have a neighbor
            if((PortErrorStatusCSR[portNum] & 0x00000002) != 0){
                //Check if it is an enumeration boundary port
                //if()
                rioRouteAddEntry(0, RIO_GEN_DFLT_DID, hopCount, 0xff, portNum, \
                        0xff, portNum);
                //Discover the neighbor recursively
                if(rioEnumerateNeighbor(hostDeviceID, hopCount + 1) !=\
                        RIO_SUCCESS){
                    return status;
                }

                //If more than one end point device was found, update the current
                //switch rounting table entries beginning with the curDeviceID 
                //entry and end with the DeviceID -1 entry

                if(DeviceID > curDeviceID)
                {
                    for(deviceid = curDeviceID; deviceid < DeviceID; \
                        deviceid ++){
                        rioRouteAddEntry(0, RIO_GEN_DFLT_DID, hopCount, 0xff, portNum, \
                                    deviceid, portNum);
						//Synchronize the current switch routing table with the 
                    	//global table
                    	Switches[curSwitchID].RouteTable.LFT[portNum] = deviceid;
						//(printf("Curient switch ID is %d\n", curSwitchID));
						//(printf("Device ID is %d, portNum is %d\n", deviceid, portNum));
                    }
                    //Update the associated Device ID in the path
                    Switches[curSwitchID].DeviceID = curDeviceID;
                }
            }
        }
    }
    return RIO_SUCCESS;
}


STATUS rioSystemEnumerate(uint32_t hostDeviceID)
{
    int32_t status;
	//uint32_t switchNum;
	memset(Switches, 0x00, sizeof(Switches));
    //Discover the host first.
    status = rioEnumerateHost(hostDeviceID);

    if(status == ERR_SLAVE){
        rioClearUp(hostDeviceID);
        return ERR_SLAVE;
    }

    //Discover the host neighbor
    status = rioEnumerateNeighbor(hostDeviceID, 0);

    if(status == ERR_SLAVE){
        rioClearUp(hostDeviceID);
        return ERR_SLAVE;
    }
	printf("Find %d devices! Find %d Switchs!\n", (DeviceID-1), SwitchID);

    return RIO_SUCCESS;
}

void ShowRouteTable(void)
{
	uint32_t numSwitch, portNum;
	for(numSwitch = 0; numSwitch < SwitchID; numSwitch ++)
	{
		if(Switches[numSwitch].SwitchIdentity == 0x00007403)
		{
			for(portNum = 0; portNum < 0x12; portNum ++)
			{
				printf("Switch %d hopCount is %d device id is %d\n", \
						numSwitch, Switches[numSwitch].hopCount, \
						Switches[numSwitch].RouteTable.LFT[portNum]);
			}
		}else if(Switches[numSwitch].SwitchIdentity == 0x00007805)
		{
			for(portNum = 0; portNum < 0x08; portNum ++)
			{
				printf("Switch %d hopCount is %d device id is %d\n", \
						numSwitch, Switches[numSwitch].hopCount, \
						Switches[numSwitch].RouteTable.LFT[portNum]);
			}

		}
	}
}





