/**
 *   @file  rm.h
 *
 *   @brief   
 *      This is the RM LLD include file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2012, Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/

#ifndef RM_H_
#define RM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* RM resource table defs include file */
#include <ti/drv/rm/rmver.h>
#include <ti/drv/rm/rm_public_lld.h>
#include <ti/drv/rm/resource_table_defs.h>

/**  @mainpage Resource Manager Low Level Driver
 *
 *   @section intro  Introduction
 *
 *   The Resource Manager low level driver (RM LLD) is designed to provide initialization and permissions checking
 *   for LLD resources.  A system integrator can specify DSP permissions for LLD resources within the system.
 *   The resource permissions are stored in the RM LLD and used to validate resource requests from supported LLDs.
 *   Currently, the RM LLD supports permissions for the following LLDs
 *      - QMSS
 *      - CPPI
 *      - PA
 *    For information on the specific resources supported with each LLD please see @ref RM_LLD_RESOURCE_TABLE.
 *
 *    The system integrator must specify the supported LLD resource permissions for all DSPs prior to compile time.
 *    An array of @ref Rm_Resource objects must be created.  Each @ref Rm_Resource entry in the array specifies
 *    a resource type, start and end range for the resource, and the initialization and usage permissions for the
 *    resource for each DSP.  For an example resource table definitions please see the @ref rmResourceTable array
 *    defined in the resource_table\ directory.  This resource table assigns full initialization and usage permissions
 *    to all DSPs for all supported resources.  Please note that the resouce table must start with the
 *    @ref RM_RESOURCE_MAGIC_NUMBER entry and end with the @ref RM_RESOURCE_FINAL_ENTRY.  These
 *    first and last entries are used the RM LLD to validate and parse the resource table.
 *
 *    The RM LLD must be initialized and started prior to all supported LLD initialization and start routines.  The
 *    @ref Rm_init function should be invoked, and passed a pointer to the integrator defined resource table, on the
 *    master DSP core in the system.  All other DSP cores should invoke the @ref Rm_start API.  The @ref Rm_init 
 *    function first initializes the internal permission tables to deny all DSPs access to all resources.  Next the 
 *    @ref Rm_init function parses the resource table provided and copies all specified permissions into the internal
 *    tables.  When the permission transfer completes the @ref Rm_init function writes a global synchronization
 *    object which the @ref Rm_start functions are spinning on.  The slave cores that have invoked @ref Rm_start
 *    will stop spinning once the global synchronization has been written.  At this time @ref Rm_start will invalidate 
 *    all internal permission tables so that no further cache invalidate operations need to be performed when 
 *    checking resource permissions in the data path. The upfront cache invalidate operation is possible because 
 *    the RM LLD does not allow dynamic resource permission modifications. The permissions defined by the system
 *    integrator and loaded during RM initialization are static throughout the system up-time.
 *
 *    The RM LLD must be registered with a supported LLD in order for the supported LLD to perform resource 
 *    permission checks.  If the RM LLD is not registered with a supported LLD the LLD will operate as if the RM LLD
 *    is not there.  This maintains full backwards compatability with existing applications not using the RM LLD.  In order
 *    to register the RM LLD with supported LLDs the following steps should be taken
 *      - Get a @ref Rm_Handle via the @ref Rm_getHandle API on each DSP that uses the RM LLD.
 *      - Register the RM LLD with the supported LLDs by passing the @ref Rm_Handle to the 
 *          LLD's <<LLD name>>_startCfg API.  Again, this should be performed on all DSP cores using the RM LLD.
 *          Note: The master core for the QMSS LLD should have the @ref Rm_Handle registered via the Qmss_init API.
 *    After registering the RM LLD with supported LLDs all supported LLD resources covered by the RM LLD will invoke
 *    permission check callouts to the RM.  A permission denied or approved response will be given back to the
 *    invoking LLD based on the permissions stored in the RM LLD for the resource.
 *
 *    All internal RM LLD permission tables are placed into a single memory section called ".rm".  This memory section
 *    MUST be placed in shared memory (MSMC or DDR).  The permission tables are shared between all DSPs utilizing
 *    the RM LLD.
 *
 *    In summary, the initialization flow if the RM LLD is to be utilized should look like the following:
 *    
 *    Master DSP Core:
 *      - Call @ref Rm_init passing in the system integrator defined resource table
 *      - Call @ref Rm_getHandle
 *      - Call supported LLD _init or _create functions (for QMSS pass the @ref Rm_Handle inside the 
 *          Qmss_GlobalConfigParams structure)
 *      - Call supported LLD _startCfg functions passing the @ref Rm_Handle as an argument
 *
 *    Slave DSP Cores:
 *      - Call @ref Rm_start
 *      - Call @ref Rm_getHandle
 *      - Call supported LLD _start functions (if supported)
 *      - Call supported LLD _startCfg functions passing the @ref Rm_Handle as an argument
 *  
 */
 
/* Define RM_LLD_API as a master group in Doxygen format and add all RM LLD API 
   definitions to this group. */
/** @defgroup RM_LLD_API RM LLD Module API
 *  @{
 */
/** @} */

/**
@defgroup RM_LLD_SYMBOL  RM Low Level Driver Symbols Defined
@ingroup RM_LLD_API
*/
/**
@defgroup RM_LLD_ENUM  RM Low Level Driver Enums
@ingroup RM_LLD_API
*/
/**
@defgroup RM_LLD_RESOURCE_TABLE  RM LLD Resource Table Definition Symbols and Structures
@ingroup RM_LLD_API
*/
/**
@defgroup RM_LLD_DATASTRUCT  RM Low Level Driver Data Structures
@ingroup RM_LLD_API
*/
/**
@defgroup RM_LLD_FUNCTION  RM Low Level Driver Functions
@ingroup RM_LLD_API
*/
/**
@defgroup RM_LLD_OSAL  RM Low Level Driver OSAL Functions
@ingroup RM_LLD_API
*/

/**
@addtogroup RM_LLD_SYMBOL
@{
*/

/** RM LLD Return And Error Codes */
/** RM Resource Okay Return Base */
#define RM_OK  0

/** RM LLD Error Base */
#define RM_ERROR (-64)
/** RM LLD failed to populate internal permission tables */
#define RM_ERROR_PERMISSION_TABLE_POPULATION_FAILED (RM_ERROR-1)
/** RM LLD Table entry population failure */
#define RM_ERROR_PERMISSION_TABLE_ENTRY_FAILURE (RM_ERROR-2)

/** Master/Slave synchronization defines */
/** Permissions table not valid */
#define RM_PERMISSION_TABLE_NOT_VALID 0
/** Permissions table valid */
#define RM_PERMISSION_TABLE_VALID 1

/**
@}
*/

/** @addtogroup RM_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief RM Handle for LLDs
 */
typedef void *Rm_Handle;

/** 
@} 
*/

/* Exported functions available to application */
extern Rm_Result Rm_init (const Rm_Resource *rmResourceTable);
extern Rm_Result Rm_start (void);
extern Rm_Handle Rm_getHandle(void);
extern uint32_t Rm_getVersion (void);
extern const char* Rm_getVersionStr (void);

#ifdef __cplusplus
}
#endif

#endif /* RM_H_ */

