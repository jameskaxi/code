#include <c6x.h>

#include <xdc/std.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/hal/Hwi.h>
/* CSL Semaphore module includes */
#include <ti/csl/csl_semAux.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_xmcAux.h>
#include <ti/csl/csl_chip.h>

#include <ti/drv/srio/srio_drv.h>

/**********************************************************************
 ****************************** Defines *******************************
 **********************************************************************/
#define CPPI_HW_SEM     1
#define QMSS_HW_SEM     2
#define QMSS_MT_HW_SEM  3
#define PA_HW_SEM       5
#define SRIO_HW_SEM	    6

#define IPCFREEQHND_SEM 7
#define IPCPOLLQHND_SEM 8


typedef struct MEM_MGMT_ENTRY
{
    uint8_t*    ptrMemory;
    uint32_t    isFree;
}MEM_MGMT_ENTRY;
#define MAX_MEM_MGR_ENTRIES 128
/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
uint32_t              qmssMallocCounter = 0;
uint32_t              qmssFreeCounter   = 0;
uint32_t              cppiMallocCounter = 0;
uint32_t              cppiFreeCounter   = 0;

MEM_MGMT_ENTRY     gDataBufferMemMgr[MAX_MEM_MGR_ENTRIES];
int32_t            gDataBufferMemMgrMaxSize = 0;

UInt32 malloc_counter = 0;
UInt32 free_counter   = 0;
/**********************************************************************
 *********************** QMSS OSAL Functions **************************
 **********************************************************************/
void* Osal_qmssMalloc (uint32_t num_bytes)
{
	Error_Block	errorBlock;

    /* Increment the allocation counter. */
    qmssMallocCounter++;	
	/* Allocate memory. */
	return Memory_alloc(NULL, num_bytes, 0, &errorBlock);
}

void Osal_qmssFree (void *ptr, uint32_t size)
{
    /* Increment the free counter. */
    qmssFreeCounter++;	
	Memory_free(NULL, ptr, size);
}

void* Osal_qmssCsEnter (void)
{
    while ((CSL_semAcquireDirect (QMSS_HW_SEM)) == 0);
    return NULL;
}

void Osal_qmssCsExit (void *CsHandle)
{
    CSL_semReleaseSemaphore (QMSS_HW_SEM);
}

void* Osal_qmssMtCsEnter (void)
{
	while ((CSL_semAcquireDirect (QMSS_MT_HW_SEM)) == 0);
	return NULL;
}


void Osal_qmssMtCsExit (void *CsHandle)
{
    /* Release Semaphore using handle */
	CSL_semReleaseSemaphore (QMSS_MT_HW_SEM);
    return;
}


void Osal_qmssLog (char *fmt, ... )
{

}

void Osal_qmssBeginMemAccess (void *ptr, uint32_t size)
{
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();
    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();
    /* Invalidate L2 cache. This should invalidate L1D as well. 
     * Wait until operation is complete. */    
    CACHE_invL2 (ptr, size, CACHE_FENCE_WAIT);
    /* Invalidate L1D cache and wait until operation is complete. 
     * Use this approach if L2 cache is not enabled */    
    CACHE_invL1d (ptr, size, CACHE_FENCE_WAIT);
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);
    return;
}

void Osal_qmssEndMemAccess (void *ptr, uint32_t size)
{
        uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();


    /* Writeback L2 cache. This should Writeback L1D as well. 
     * Wait until operation is complete. */ 
    CACHE_wbL2 (ptr, size, CACHE_FENCE_WAIT);
    /* Writeback L1D cache and wait until operation is complete. 
     * Use this approach if L2 cache is not enabled */    
    CACHE_wbL1d (ptr, size, CACHE_FENCE_WAIT);
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    /* Reenable Interrupts. */
    Hwi_restore(key);

    return;

}

/**********************************************************************
 *********************** CPPI OSAL Functions **************************
 **********************************************************************/
//extern IHeap_Handle CPPIHEAP;
void* Osal_cppiMalloc (uint32_t num_bytes)
{
	Error_Block	errorBlock;
    /* Increment the allocation counter. */
    cppiMallocCounter++;	
	/* Allocate memory. */
    //return Memory_alloc (CPPIHEAP, num_bytes, 0, &errorBlock);
	return Memory_alloc (NULL, num_bytes, 0, &errorBlock);
}


void Osal_cppiFree (void *ptr, uint32_t size)
{
    /* Increment the free counter. */
    cppiFreeCounter++;
//    Memory_free (CPPIHEAP, ptr, size);
	Memory_free (NULL, ptr, size);
}


void* Osal_cppiCsEnter (void)
{
    /* Get the hardware semaphore */
    while ((CSL_semAcquireDirect (CPPI_HW_SEM)) == 0);
    return NULL;
}


void Osal_cppiCsExit (void *CsHandle)
{
    /* Release the hardware semaphore */ 
    CSL_semReleaseSemaphore (CPPI_HW_SEM);
    return;
}


void Osal_cppiLog (char *fmt, ... )
{
}


void Osal_cppiBeginMemAccess (void *ptr, uint32_t size)
{
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();
    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();
    CACHE_invL2 (ptr, size, CACHE_FENCE_WAIT);
    CACHE_invL1d (ptr, size, CACHE_FENCE_WAIT);
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    Hwi_restore(key);

    return;
}

void Osal_cppiEndMemAccess (void *ptr, uint32_t size)
{
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();
    CACHE_wbL1d (ptr, size, CACHE_WAIT);
    CACHE_wbL2 (ptr, size, CACHE_WAIT);
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    Hwi_restore(key);
    return;
}


/**********************************************************************
 *********************** PASS OSAL Functions **************************
 **********************************************************************/
void Osal_paBeginMemAccess (Ptr addr, uint32_t size)
{
    uint32_t    key;

    /* Disable Interrupts */
    key = Hwi_disable();
    CSL_XMC_invalidatePrefetchBuffer();
//	CACHE_invL1d (addr, size, CACHE_WAIT);
//	CACHE_invL2 (addr, size, CACHE_WAIT);
    CACHE_invL2 (addr, size, CACHE_FENCE_WAIT);
    CACHE_invL1d (addr, size, CACHE_FENCE_WAIT);

    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    Hwi_restore(key);
	return;
}

void Osal_paEndMemAccess (Ptr addr, uint32_t size)
{
	uint32_t    key;
	/* Cache coherence protection is only required for multi-core application */
    key = Hwi_disable();
//	CACHE_wbL2 (addr, size, CACHE_WAIT);
//	CACHE_wbL1d (addr, size, CACHE_WAIT);
    CACHE_wbL2 (addr, size, CACHE_FENCE_WAIT);
    CACHE_wbL1d (addr, size, CACHE_FENCE_WAIT);
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");
    asm   (" nop      4");

    Hwi_restore(key);
	return;
}

/* OSAL Functions for the PA LLDs */
void Osal_paMtCsEnter (uint32_t *key)
{
     while ((CSL_semAcquireDirect (PA_HW_SEM)) == 0);
     *key = 0;
}

void Osal_paMtCsExit (uint32_t key)
{
    /* Release the hardware semaphore */
    CSL_semReleaseSemaphore (PA_HW_SEM);
}

/**********************************************************************
 *********************** NIMU OSAL Functions **************************
 **********************************************************************/
extern IHeap_Handle heapMemNIMU;
Ptr Osal_nimuMalloc (uint32_t num_bytes, uint32_t alignment)
{
	Error_Block	    errorBlock;
	/* Allocate memory.  */
	Error_init(&errorBlock);
	//return Memory_alloc(heapMemNIMU, num_bytes, alignment, &errorBlock);
	return Memory_alloc(NULL, num_bytes, alignment, &errorBlock);
}

void Osal_nimuFree (Ptr dataPtr, uint32_t num_bytes)
{
    /* Free up the memory */
    if (dataPtr)
    {
    	//Memory_free(heapMemNIMU, dataPtr, num_bytes);
    	Memory_free(NULL, dataPtr, num_bytes);
    }
}

/**********************************************************************
 *********************** SRIO OSAL Functions **************************
 **********************************************************************/
extern IHeap_Handle heapMemSRIO;
void* Osal_srioMalloc (UInt32 num_bytes)
{
	Error_Block	errorBlock;
    UInt32*     ptr;

    /* Increment the allocation counter. */
    malloc_counter++;

    /* Allocate memory */
//    ptr = (UInt32*)Memory_alloc(heapMemSRIO, num_bytes, NULL, &errorBlock);
    ptr = (UInt32*)Memory_alloc(NULL, num_bytes, NULL, &errorBlock);
    if (ptr == NULL)
        return NULL;

    /* Return the allocated memory block. */
	return ptr;
}

void Osal_srioFree (void* ptr, UInt32 numBytes)
{
    /* Increment the free counter. */
    free_counter++;
//    Memory_free(heapMemSRIO, ptr, numBytes);
    Memory_free(NULL, ptr, numBytes);
}

int32_t Osal_sriodataBufferInitMemory(uint32_t dataBufferSize)
{
	Error_Block	errorBlock;
    uint8_t*    ptrMemory;
    uint32_t    index;

    /* Allocate memory for all the data buffers */
//    ptrMemory = (uint8_t*)Memory_alloc(heapMemSRIO, MAX_MEM_MGR_ENTRIES*dataBufferSize, 16, &errorBlock);
    ptrMemory = (uint8_t*)Memory_alloc(NULL, MAX_MEM_MGR_ENTRIES*dataBufferSize, 16, &errorBlock);
    if (ptrMemory == NULL)
        return -1;

    /* Convert to a global address */
    ptrMemory = (void*)l2_global_address((UInt32)ptrMemory);

    /* Now we chop up the memory and add it to the memory manager. */
    for (index = 0; index < MAX_MEM_MGR_ENTRIES; index++)
    {
        /* Populate the data memory management entry. */
        gDataBufferMemMgr[index].isFree    = 1;
        gDataBufferMemMgr[index].ptrMemory = ptrMemory;

        /* Increment the memory to the next address */
        ptrMemory = ptrMemory + dataBufferSize;
    }

    /* Remember the memory buffer size */
    gDataBufferMemMgrMaxSize = dataBufferSize;

    /* Memory Manager has been created. */
    return 0;
}

void* Osal_srioDataBufferMalloc (UInt32 num_bytes)
{
    uint32_t    index;
    void*       ptrMemory = NULL;

    /* Basic Validation: Ensure that the memory size requested is within range. */
    if (num_bytes > gDataBufferMemMgrMaxSize)
        return NULL;

    /* Increment the allocation counter. */
    malloc_counter++;

    /* Lock out interrupts */
    Hwi_disable();

    /* Cycle through for a free entry. */
    for (index = 0; index < MAX_MEM_MGR_ENTRIES; index++)
    {
        /* Check if the entry is free or not? */
        if (gDataBufferMemMgr[index].isFree == 1)
        {
            /* Entry was free. We can use it. */
            ptrMemory = gDataBufferMemMgr[index].ptrMemory;

            /* Mark the entry as used. */
            gDataBufferMemMgr[index].isFree = 0;

            /* We have found a match. */
            break;
        }
    }

    /* Unlock interrupts. */
    Hwi_enable();

    /* Return the allocated memory. */
    return ptrMemory;
}

void Osal_srioDataBufferFree (void* ptr, UInt32 numBytes)
{
    uint32_t    index;

    /* Increment the free counter. */
    free_counter++;

    /* Lock out interrupts */
    Hwi_disable();

    /* Cycle through and clean up */
    for (index = 0; index < MAX_MEM_MGR_ENTRIES; index++)
    {
        /* Check if the entry is free or not? */
        if (gDataBufferMemMgr[index].ptrMemory == (uint8_t*)ptr)
        {
            /* Mark the entry as free. */
            gDataBufferMemMgr[index].isFree = 1;

            /* We have found a match. */
            break;
        }
    }
    /* Unlock interrupts. */
    Hwi_enable();
    return;
}

void* Osal_srioMultiCoreEnter(void)
{
    /* Get the hardware semaphore */
    while ((CSL_semAcquireDirect (SRIO_HW_SEM)) == 0);
    return NULL;
}

void  Osal_srioMultiCoreExit(void* critSectHandle)
{
    /* Release the hardware semaphore */
    CSL_semReleaseSemaphore (SRIO_HW_SEM);
}

void* Osal_srioSingleCoreEnter(Srio_DrvHandle drvHandle)
{
	return (void*)Hwi_disable();
}

void Osal_srioSingleCoreExit(Srio_DrvHandle drvHandle, void* critSectHandle)
{
    /* Re-enable Interrupts.*/
     Hwi_enable();
}

void Osal_srioCacheInvalidate(void* ptr, uint32_t size)
{
    UInt  key;

    /* Disable Interrupts */
    key = Hwi_disable();
    /* Cleanup the prefetch buffer also. */
    CSL_XMC_invalidatePrefetchBuffer();
    /* Invalidate the cache. */
    CACHE_invL1d (ptr, size, CACHE_FENCE_WAIT);
    CACHE_invL2 (ptr, size, CACHE_FENCE_WAIT);
    /* Reenable Interrupts. */
    Hwi_restore(key);
}

void Osal_srioCacheWriteback(void* ptr, uint32_t size)
{
    UInt  key;

    /* Disable Interrupts */
    key = Hwi_disable();
    /* Writeback the cache. */
    CACHE_wbL2 (ptr, size, CACHE_FENCE_WAIT);
    CACHE_wbL1d (ptr, size, CACHE_FENCE_WAIT);
    /* Reenable Interrupts. */
    Hwi_restore(key);
}

void Osal_srioBeginDescriptorAccess (Srio_DrvHandle drvHandle, void* ptr, uint32_t descSize)
{
    /* In the Test Application all descriptor are located in LL2 memory we dont need to
     * add any special cache invalidation hook here. However if the descriptors were located
     * in SL2 (MSMC) this would be required. */
    return;
}

void Osal_srioEndDescriptorAccess (Srio_DrvHandle drvHandle, void* ptr, uint32_t descSize)
{
    /* In the Test Application the descriptors are located in Core 0 Global address space
     * If they are accessed from other cores we need to ensure that there is an MFENCE so
     * that all the access to the descriptors are complete before we start pushing them
     * out. */
    _mfence();
    return;
}


