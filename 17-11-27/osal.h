/**
 *   @file  srio_osal.h
 *
 *   @brief
 *      This is the OS adaptation layer for the SRIO Driver which has
 *      been ported for XDC Runtime and BIOS.
 *
 *      This is an example of <b> Approach 2 </b> in which the
 *      application rebuilds the SRIO driver with the new definitions.
 *
 *      Please refer to the 'srio_osal.h' in the API documentation.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2009 Texas Instruments, Inc.
 *  \par
 */
#ifndef __OSAL_H__
#define __OSAL_H__

#include <string.h>
#include <stdio.h>
#include <xdc/runtime/System.h>
// #include <ti/sysbios/ipc/Semaphore.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drv/srio/srio_drv.h>

/* Memory Allocation OSAL Extern Definitions. */
extern void* Osal_srioMalloc (UInt32 num_bytes);
extern void  Osal_srioFree (void* ptr, UInt32 numBytes);
extern void* Osal_srioDataBufferMalloc(uint32_t numBytes);
extern void  Osal_srioDataBufferFree(void* ptr, uint32_t numBytes);

/* Multicore Protection OSAL Extern Definitions. */
extern void* Osal_srioMultiCoreEnter(void);
extern void  Osal_srioMultiCoreExit(void* critSectHandle);

/* Single Core Protection OSAL Extern Definitions. */
extern void* Osal_srioSingleCoreEnter(Srio_DrvHandle drvHandle);
extern void  Osal_srioSingleCoreExit(Srio_DrvHandle drvHandle, void* critSectHandle);

/* Descriptor OSAL Extern Definitions. */
extern void  Osal_srioBeginDescriptorAccess (Srio_DrvHandle drvHandle,void* ptr, uint32_t descSize);
extern void  Osal_srioEndDescriptorAccess (Srio_DrvHandle drvHandle,void* ptr, uint32_t descSize);

/* Cache OSAL Extern Definitions. */
extern void  Osal_srioCacheInvalidate(void* ptr, uint32_t size);
extern void  Osal_srioCacheWriteback(void* ptr, uint32_t size);

/* SRIO OSAL Semaphore API are mapped directly to the BIOS API */
#define Srio_osalCreateSem()                        (void*)Semaphore_create(0, NULL, NULL)
#define Srio_osalDeleteSem(X)                       Semaphore_delete(X)
#define Srio_osalPendSem(X)                         Semaphore_pend(X, BIOS_WAIT_FOREVER);
#define Srio_osalPostSem(X)                         Semaphore_post(X);

/* SRIO OSAL Memory Allocation API are redefined to TEST application API */
#define Srio_osalMalloc                             Osal_srioMalloc
#define Srio_osalFree                               Osal_srioFree

/* SRIO OSAL Memory Allocation API for the Data Buffer */
#define Srio_osalDataBufferMalloc                   Osal_srioDataBufferMalloc
#define Srio_osalDataBufferFree                     Osal_srioDataBufferFree

/* SRIO OSAL Logging API is mapped directly to an XDC Runtime API */
#define Srio_osalLog                                System_printf

/* Multicore Protection: */
#define Srio_osalEnterMultipleCoreCriticalSection   Osal_srioMultiCoreEnter
#define Srio_osalExitMultipleCoreCriticalSection    Osal_srioMultiCoreExit

/* Singlecore Protection: */
#define Srio_osalEnterSingleCoreCriticalSection     Osal_srioSingleCoreEnter
#define Srio_osalExitSingleCoreCriticalSection      Osal_srioSingleCoreExit

/* CACHE API: */
#define Srio_osalBeginMemAccess                     Osal_srioCacheInvalidate
#define Srio_osalEndMemAccess                       Osal_srioCacheWriteback

/* CACHE Descriptor API: */
#define Srio_osalBeginDescriptorAccess              Osal_srioBeginDescriptorAccess
#define Srio_osalEndDescriptorAccess                Osal_srioEndDescriptorAccess

int32_t Osal_sriodataBufferInitMemory(uint32_t dataBufferSize);
#endif


