//usrdef.h
#include "drv.h"
#undef SEM


extern Mailbox_Handle mailbox_ipc;

#pragma DATA_SECTION(gIPC_init, ".resmgr_handles")
#pragma DATA_ALIGN (gIPC_init, 128)
uint8_t gIPC_init = 0;

#pragma DATA_SECTION(gIPC_init, ".resmgr_handles")
#pragma DATA_ALIGN (gIPC_const, 128)
uint8_t gIPC_const = 0;

#define KICK0       *(unsigned int*)(0x02620038)
#define KICK1       *(unsigned int*)(0x0262003C)
//IPC interrupt
#define IPCGRBASE   0x02620240

#define IPCGR0      *(unsigned int*)(0x02620240)
#define IPCGR1      *(unsigned int*)(0x02620244)
#define IPCGR2      *(unsigned int*)(0x02620248)
#define IPCGR3      *(unsigned int*)(0x0262024C)
#define IPCGR4      *(unsigned int*)(0x02620250)
#define IPCGR5      *(unsigned int*)(0x02620254)
#define IPCGR6      *(unsigned int*)(0x02620258)
#define IPCGR7      *(unsigned int*)(0x0262025C)
#define IPCGRH      *(unsigned int*)(0x0262027C)
#define IPCAR0      *(unsigned int*)(0x02620280)
#define IPCAR1      *(unsigned int*)(0x02620284)
#define IPCAR2      *(unsigned int*)(0x02620288)
#define IPCAR3      *(unsigned int*)(0x0262028C)
#define IPCAR4      *(unsigned int*)(0x02620290)
#define IPCAR5      *(unsigned int*)(0x02620294)
#define IPCAR6      *(unsigned int*)(0x02620298)
#define IPCAR7      *(unsigned int*)(0x0262029C)

#define CORE0   0x01
#define CORE1   0x02
#define CORE2   0x04
#define CORE3   0x08
#define CORE4   0x10
#define CORE5   0x20
#define CORE6   0x40
#define CORE7   0x80


#define SYNC_CORE8  0xfe
#define SYNC_CORE4  0x0e

typedef int bool;

static inline uint32_t enterCritical(void)
{
	return Hwi_disableInterrupt(IPC_INTERRUPT);
}

static inline void exitCritical(uint32_t key)
{
	Hwi_restoreInterrupt(IPC_INTERRUPT, key);
}
#if 0
//one bit one core
Bool waitSync(uint8_t numCore, uint16_t *message)
{
	Cppi_Desc          *desc;
	uint32_t			hSrcBuffer;
	uint32_t flag = 0, tempcnt=0;
	msgObj_t      		msg;
	int i, j, num=0;
	uint32_t key;

	for(i=0; i<8; i++)
	{
		tempcnt += (numCore>>i) & 0x01;
	}

	while((num = Qmss_getQueueEntryCount(gIPCQHndPoll[DNUM])) != tempcnt)
	{
		//printf("gIPCQHndPoll %d\n", Qmss_getQueueEntryCount(gIPCQHndPoll[DNUM]));
		Task_yield();
	}

	for(j=0; j<tempcnt; j++)
	{
    	desc = Qmss_queuePop (gIPCQHndPoll[DNUM]);
    	Cache_inv ((void *) desc, 64, Cache_Type_L1D, CACHE_WAIT);
    	hSrcBuffer =  (uint32_t)Cppi_getSoftwareInfo0  ( Cppi_DescType_MONOLITHIC,(Cppi_Desc *) desc);
    	memcpy((void*)&msg, (void*)&hSrcBuffer, 0x04);
    	flag |= msg.id & 0xFFFF;
    	for(i=0; i<8; i++)
    	{
    		if((msg.id>>i)&0x01)
    		{
    			message[i] = msg.val;
    			break;
    		}
    	}
    	key = enterCritical();
    	while ((CSL_semAcquireDirect (IPCFREEQHND_SEM)) == 0);
    	Qmss_queuePushDesc (gIPCFreeQHnd, (UInt32 *) desc);
    	CSL_semReleaseSemaphore (IPCFREEQHND_SEM);
    	exitCritical(key);
	}

	if(flag == numCore)
		return TRUE;
	else
		return FALSE;
}
#endif

int32_t ipc_send(uint8_t dstcoreNum, msgObj_t  msg/*, Bool interuptFlag*/)
{
	Cppi_Desc          *desc;
	uint32_t			hSrcBuffer;
	int i;
	uint32_t key;
	uint32_t cnt;

	Bool interuptFlag = TRUE;
    key = enterCritical();
    while ((CSL_semAcquireDirect (IPCFREEQHND_SEM)) == 0);
    cnt = Qmss_getQueueEntryCount(gGlobalDataPtr->gIPCFreeQHnd);
    CSL_semReleaseSemaphore (IPCFREEQHND_SEM);
    exitCritical(key);
    if(cnt < 8)
    {
        System_printf("Not enough descriptors, send failed!\n");
        System_flush();
        return (-1);
    }

    for(i=0; i<8; i++)
    {
        if((dstcoreNum >> i)&0x01)
        {
            if(i==DNUM)
                continue;
            key = enterCritical();
            while ((CSL_semAcquireDirect (IPCFREEQHND_SEM)) == 0);
            desc = (Cppi_Desc *) Qmss_queuePop (gGlobalDataPtr->gIPCFreeQHnd);
            CSL_semReleaseSemaphore (IPCFREEQHND_SEM);
            exitCritical(key);
            if (desc != NULL)
            {
                memcpy(&hSrcBuffer, &msg, sizeof(int));
               // hSrcBuffer= (1<<DNUM) | message<<16;
                Cppi_setSoftwareInfo0  ( Cppi_DescType_MONOLITHIC, (Cppi_Desc *) desc, hSrcBuffer);
                Cache_wb((void *) desc, 128, Cache_Type_L1D, CACHE_WAIT);
                Cache_wb((void *) desc, 128, Cache_Type_L2, CACHE_WAIT);
                if(interuptFlag == TRUE)
                {
                    Qmss_queuePushDesc (gGlobalDataPtr->gIPCQHnd[i], (UInt32 *) desc);
                    KICK0 = 0x83e70b13;
                    KICK1 = 0x95a4f1e0;
                    *(uint32_t*)(IPCGRBASE + i*4) = 0x01/* | 0x01<<(DNUM+4)*/;
                }
                else
                {
                    while ((CSL_semAcquireDirect (IPCPOLLQHND_SEM)) == 0);
                    Qmss_queuePushDesc (gGlobalDataPtr->gIPCQHndPoll[i], (UInt32 *) desc);
                    CSL_semReleaseSemaphore (IPCPOLLQHND_SEM);
                }
            }
            else
            {
                System_printf ("Error pop desc\n");
                System_flush();
                return (-2);
            }
        }
    }
    return 0;
}
extern uint64_t timer[2];

void ipc_isr(void)
{
	Cppi_Desc          *desc;
	uint32_t			hSrcBuffer;
//	msgObj_t      		msg;
    uint32_t            key;
    uint32_t descNum;
    int i;

    key = Hwi_disableInterrupt(IPC_INTERRUPT);
    descNum = Qmss_getQueueEntryCount(gGlobalDataPtr->gIPCQHnd[DNUM]);
    for(i=0; i<descNum; i++)
    {
    	desc = Qmss_queuePop (gGlobalDataPtr->gIPCQHnd[DNUM]);
    	Cache_inv ((void *) desc, 128, Cache_Type_L1D, CACHE_WAIT);
    	Cache_inv ((void *) desc, 128, Cache_Type_L2, CACHE_WAIT);
    	hSrcBuffer =  (uint32_t)Cppi_getSoftwareInfo0  ( Cppi_DescType_MONOLITHIC,(Cppi_Desc *) desc);
    	//memcpy((void*)&msg, (void*)&hSrcBuffer, 0x04);
    	Mailbox_post(mailbox_ipc, &hSrcBuffer, BIOS_NO_WAIT);
    	while ((CSL_semAcquireDirect (IPCFREEQHND_SEM)) == 0);
    	Qmss_queuePushDesc (gGlobalDataPtr->gIPCFreeQHnd, (UInt32 *) desc);
    	CSL_semReleaseSemaphore (IPCFREEQHND_SEM);
    }
	Hwi_restoreInterrupt(IPC_INTERRUPT, key);

}

void ipc_init(void)
{
	Hwi_Handle ipcHwiHanle;
	Hwi_Params hwiParams;
	Error_Block eb;

	Hwi_Params_init(&hwiParams);
	Error_init(&eb);
	//Set the argument you want passed to you ISR function
	//hwiParams.arg = 1;
	//Set the event id of the peripheral assigend to this interrupt
	hwiParams.eventId = IPC_EVENTID;
	//Don't allow this interrupt to nest itself
	hwiParams.maskSetting = Hwi_MaskingOption_SELF;
	ipcHwiHanle = Hwi_create(IPC_INTERRUPT, (Hwi_FuncPtr)ipc_isr, &hwiParams, &eb);
	if(ipcHwiHanle != NULL)
	{
		if(Error_check(&eb))
		{
			System_printf("ipc hwi inits failed!\n");
			System_flush();
		}
	}
	while ((CSL_semAcquireDirect (IPC_INIT)) == 0);
	gIPC_init |= (1<<DNUM);
	CSL_semReleaseSemaphore (IPC_INIT);
}





