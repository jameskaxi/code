
#ifndef _RESOURCE_MGR_H
#define _RESOURCE_MGR_H  /* #defined if this .h file has been included */

#include <stdint.h>
//#include <xdc/std.h>
/* CPPI LLD include */
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/cppi_desc.h>
#include <ti/drv/cppi/include/cppi_pvt.h>
/* QMSS LLD include */
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/qmss/qmss_types.h>
#include <ti/drv/qmss/include/qmss_pvt.h>
/* PA LLD include */
#include <ti/drv/pa/pa.h>


#define NUM_FREE_DESC           	8192

//External memory start address
#define EMAC_EXTMEM     	    0x80000000
#define EMAC_LL2SRAM            0x00800000
#define EMAC_MSMCSRAM           0x0c000000

//Global semaphore define
#define CPPI_HW_SEM     			1
#define QMSS_HW_SEM     			2
#define SRIO_HW_SEM     			3
#define SRIO_APP_HW_SEM     		4
#define SRIO_BUF_HW_SEM         	5

#define IPCFREEQHND_SEM 			7
#define IPCPOLLQHND_SEM 			8
#define IPC_INIT                    9

//Event id define
#define ETH_EVENTID			    	48
#define ETH_INTERRUPT		    	8//7
#define SRIO_EVENTID		    	49
#define SRIO_INTERRUPT		    	6
#define DOORBEL_EVENTID		    	20
#define DOORBEL_INTERRUPT	    	4
#define IPC_EVENTID			    	91
#define IPC_INTERRUPT		    	5

/* PA definitions */
#define MAX_NUM_L2_HANDLES      	10
#define MAX_NUM_L3_HANDLES      	20
#define MAX_NUM_L4_HANDLES      	40
#define BUFSIZE_PA_INST         	256
#define BUFSIZE_L2_TABLE        	1000
#define BUFSIZE_L3_TABLE        	4000

#define NUM_PA_TX_QUEUES        	9
#define NUM_PA_RX_CHANNELS      	24

#define MAX_NUM_L2_HANDLES      	10
#define MAX_NUM_L3_HANDLES      	20
#define MAX_NUM_L4_HANDLES      	40

#define IPC_NUM_DESC		    	256
#define IPC_SIZE_DESC	        	128

#define SRIO_NUM_DESC           	128
#define SRIO_MAX_DESC_SIZE      	256

#define NIMU_NUM_TX_DESC	        16
#define NIMU_NUM_RX_DESC		    128-16
//#define NIMU_MAX_NUM_TX_CMD_DESC	2
#define	NIMU_MAX_DESC_SIZE			128

#define NIMU_NUM_DESC               (NIMU_NUM_TX_DESC + NIMU_NUM_RX_DESC)
#define MAX_NUM_DESC                (NIMU_NUM_DESC + SRIO_NUM_DESC + IPC_NUM_DESC)

/* Accumulator channel to use */
#define     PA_ACC_CHANNEL_NUM          0+DNUM
/* High Priority QM Rx Interrupt Threshold */
#define     RX_INT_THRESHOLD            20

#define HIGH_PRIORITY_ACC_INT_QUEUE_BASE  QMSS_HIGH_PRIORITY_QUEUE_BASE

//SRIO queues
//#define GARBAGE_LEN_QUEUE		    905
//#define GARBAGE_TOUT_QUEUE		    906
//#define GARBAGE_RETRY_QUEUE		    907
//#define GARBAGE_TRANS_ERR_QUEUE	    908
//#define GARBAGE_PROG_QUEUE		    909
//#define GARBAGE_SSIZE_QUEUE		    910
//IPC queues
#define IPC_INTERRUPT_QUEUE_BASE    911
#define IPC_POLL_QUEUE_BASE  IPC_INTERRUPT_QUEUE_BASE + 8

/**********************************************************************
 *********** Resoure Manager QMSS configuration Definitions ***********
 **********************************************************************/
typedef enum {
    QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF,
    QHANDLER_QPOP_FDQ_ATTACHEDBUF
} QHANDLER_TYPE;

typedef struct QMSS_CFG_Tag
{
    uint32_t        master_core;
    uint32_t        max_num_desc;
    uint32_t        desc_size;
    Qmss_MemRegion  mem_region;
} QMSS_CFG_T;

Qmss_QueueHnd getNimuFreequeue(void);
int32_t QMSS_QPOP(Qmss_QueueHnd handler, QHANDLER_TYPE type, Cppi_HostDesc** pHostDescriptor);
void QMSS_QPUSH(Qmss_QueueHnd handler, void *descAddr, uint32_t packetSize, uint32_t descSize, Qmss_Location location);
void QMSS_QPUSHDESCSIZE(Qmss_QueueHnd handler, void *descAddr, uint32_t descSize);

int32_t res_mgr_init_qmss(QMSS_CFG_T *p_qmss_cfg);
int32_t res_mgr_stop_qmss(void);
//OSAL QMSS
Ptr Osal_qmssCsEnter (void);
void Osal_qmssCsExit (Ptr CsHandle);
Ptr Osal_qmssMtCsEnter (void);
void Osal_qmssMtCsExit (Ptr CsHandle);
Ptr Osal_qmssMalloc (uint32_t num_bytes);
void Osal_qmssFree (Ptr dataPtr, uint32_t num_bytes);
void Osal_qmssBeginMemAccess (void *blockPtr, uint32_t size);
void Osal_qmssEndMemAccess (void *blockPtr, uint32_t size);

//PA functions
int32_t res_mgr_init_pass(void);
Cppi_Handle res_mgr_cppi_get_passhandle(void);
Pa_Handle res_mgr_get_painstance(void);
void Osal_paBeginMemAccess (Ptr addr, uint32_t size);
void Osal_paEndMemAccess (Ptr addr, uint32_t size);

typedef enum {
    CPPI_CFG_PASS   = 0,
    CPPI_CFG_SRIO
} CPPI_CFG_TYPE;

#define         MAX_CPPI_CFG                2

typedef struct CPPI_CFG_Tag
{
    uint32_t        master_core;
    Cppi_CpDma      dma_num;
    uint32_t        num_tx_queues;
    uint32_t        num_rx_channels;
} CPPI_CFG_T;

//CPPI functions
int32_t res_mgr_init_cppi(CPPI_CFG_T *p_cppi_cfg);
int32_t res_mgr_stop_cppi(CPPI_CFG_TYPE cfg_type);

//OSAL CPPI
Ptr Osal_cppiCsEnter (void);
void Osal_cppiCsExit (Ptr CsHandle);
Ptr Osal_cppiMalloc (uint32_t num_bytes);
void Osal_cppiFree (Ptr dataPtr, uint32_t num_bytes);
void Osal_cppiBeginMemAccess (void *blockPtr, uint32_t size);
void Osal_cppiEndMemAccess (void *blockPtr, uint32_t size);

///////////////////////////////////////////////////////////////////////////////////////
//extern var
///////////////////////////////////////////////////////////////////////////////////////
/* Memory Heap Queue which has descriptors and buffers allocated and linked together. */
#ifdef RIO
extern uint32_t         gSrioHiPriAccumList[64];
extern Qmss_QueueHnd    gSrioFreeQHnd;
extern uint8_t          gSrioHostDesc[SRIO_MAX_DESC_SIZE * SRIO_NUM_DESC];
#endif
//extern Qmss_QueueHnd    gEthFreeQHnd;
//extern uint8_t gNimuHostDesc[NIMU_NUM_DESC][NIMU_MAX_DESC_SIZE];

//extern uint8_t gIPCDesc[IPC_NUM_DESC * IPC_SIZE_DESC];

typedef struct accumList{
    uint32_t gEthHiPriAccumList[8][(RX_INT_THRESHOLD+1) * 2];
    uint32_t      gSrioHiPriAccumList[64];
}accumList_t;

typedef struct desc{
    uint8_t gNimuHostDesc[NIMU_NUM_DESC][NIMU_MAX_DESC_SIZE];
    uint8_t gIPCDesc[IPC_NUM_DESC * IPC_SIZE_DESC];

#ifdef RIO
    /* Memory allocated for the descriptors. This is 16 bit aligned. */
    uint8_t       gSrioHostDesc[SRIO_MAX_DESC_SIZE * SRIO_NUM_DESC];

#endif
}desc_t;

typedef struct shareVal{
    Qmss_QueueHnd gEthFreeQHnd;
    Qmss_QueueHnd gTxReturnQHnd;
    Qmss_QueueHnd gTxFreeQHnd;
    Qmss_QueueHnd gRxFreeQHnd;
    Qmss_QueueHnd gRxQHnd[8];
    Qmss_QueueHnd gPaTxQHnd [NUM_PA_TX_QUEUES];
    Qmss_QueueHnd gPaCfgCmdRespQHnd;
    Qmss_QueueHnd gIPCFreeQHnd;
    Qmss_QueueHnd gIPCCompletionQHnd;
    Qmss_QueueHnd gIPCQHnd[8];
    Qmss_QueueHnd gIPCQHndPoll[8];

    /* CPPI Handles */
    Cppi_FlowHnd gRxFlowHnd[8];
    /* Configuration Information */
    CPPI_CFG_T      gCppiCfg[MAX_CPPI_CFG];
    paHandleL2L3_t gPaL2Handles[MAX_NUM_L2_HANDLES];
    paHandleL2L3_t gPaL3Handles[MAX_NUM_L3_HANDLES];
    paHandleL4_t gPaL4Handles[MAX_NUM_L4_HANDLES];

    /* PA instance */
    uint8_t        gPAInst[BUFSIZE_PA_INST];
    /* Memory used for PA handles */
    uint8_t    gMemL2Ram[BUFSIZE_L2_TABLE];
    uint8_t    gMemL3Ram[BUFSIZE_L3_TABLE];
    Cppi_ChHnd     gPassCpdmaTxChanHnd [NUM_PA_TX_QUEUES];
    Cppi_ChHnd     gPassCpdmaRxChanHnd [NUM_PA_RX_CHANNELS];
    Cppi_Handle    gPassCpdmaHnd;
    Pa_Handle      gPAInstHnd;
#ifdef RIO
Qmss_QueueHnd      gSrioFreeQHnd;
#endif
    Qmss_GlobalObj              qmssGObj;
    uint8_t                     queueFree[8192];
    Cppi_Obj     cppiObject;
    uint32_t       gGolbalInit;
}shareVal_t;

#endif
