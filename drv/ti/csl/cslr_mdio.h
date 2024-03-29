/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
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
*/
#ifndef _CSLR_MDIO_H_
#define _CSLR_MDIO_H_

/* CSL Modification:
 *  The file has been modified from the AUTOGEN file for the following
 *  reasons:-
 *      a) Modified the header file includes to be RTSC compliant
 */
#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>

/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for mdio_user_group
\**************************************************************************/
typedef struct  {
    volatile Uint32 USER_ACCESS_REG;
    volatile Uint32 USER_PHY_SEL_REG;
} CSL_Mdio_user_groupRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 VERSION_REG;
    volatile Uint32 CONTROL_REG;
    volatile Uint32 ALIVE_REG;
    volatile Uint32 LINK_REG;
    volatile Uint32 LINK_INT_RAW_REG;
    volatile Uint32 LINK_INT_MASKED_REG;
    volatile Uint8  RSVD0[8];
    volatile Uint32 USER_INT_RAW_REG;
    volatile Uint32 USER_INT_MASKED_REG;
    volatile Uint32 USER_INT_MASK_SET_REG;
    volatile Uint32 USER_INT_MASK_CLEAR_REG;
    volatile Uint8  RSVD1[80];
    CSL_Mdio_user_groupRegs USER_GROUP[2];
} CSL_Mdio_Regs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* mdio_user_access0_reg */

#define CSL_MDIO_USER_ACCESS_REG_DATA_MASK (0x0000FFFFu)
#define CSL_MDIO_USER_ACCESS_REG_DATA_SHIFT (0x00000000u)
#define CSL_MDIO_USER_ACCESS_REG_DATA_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_ACCESS_REG_PHYADR_MASK (0x001F0000u)
#define CSL_MDIO_USER_ACCESS_REG_PHYADR_SHIFT (0x00000010u)
#define CSL_MDIO_USER_ACCESS_REG_PHYADR_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_ACCESS_REG_REGADR_MASK (0x03E00000u)
#define CSL_MDIO_USER_ACCESS_REG_REGADR_SHIFT (0x00000015u)
#define CSL_MDIO_USER_ACCESS_REG_REGADR_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_ACCESS_REG_ACK_MASK (0x20000000u)
#define CSL_MDIO_USER_ACCESS_REG_ACK_SHIFT (0x0000001Du)
#define CSL_MDIO_USER_ACCESS_REG_ACK_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_ACCESS_REG_WRITE_MASK (0x40000000u)
#define CSL_MDIO_USER_ACCESS_REG_WRITE_SHIFT (0x0000001Eu)
#define CSL_MDIO_USER_ACCESS_REG_WRITE_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_ACCESS_REG_GO_MASK (0x80000000u)
#define CSL_MDIO_USER_ACCESS_REG_GO_SHIFT (0x0000001Fu)
#define CSL_MDIO_USER_ACCESS_REG_GO_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_ACCESS_REG_RESETVAL (0x00000000u)

/* mdio_user_phy_sel0_reg */

#define CSL_MDIO_USER_PHY_SEL_REG_PHYADR_MON_MASK (0x0000001Fu)
#define CSL_MDIO_USER_PHY_SEL_REG_PHYADR_MON_SHIFT (0x00000000u)
#define CSL_MDIO_USER_PHY_SEL_REG_PHYADR_MON_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE_MASK (0x00000040u)
#define CSL_MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE_SHIFT (0x00000006u)
#define CSL_MDIO_USER_PHY_SEL_REG_LINKINT_ENABLE_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_PHY_SEL_REG_LINKSEL_MASK (0x00000080u)
#define CSL_MDIO_USER_PHY_SEL_REG_LINKSEL_SHIFT (0x00000007u)
#define CSL_MDIO_USER_PHY_SEL_REG_LINKSEL_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_PHY_SEL_REG_RESETVAL (0x00000000u)

/* version_reg */

#define CSL_MDIO_VERSION_REG_REVMINOR_MASK (0x000000FFu)
#define CSL_MDIO_VERSION_REG_REVMINOR_SHIFT (0x00000000u)
#define CSL_MDIO_VERSION_REG_REVMINOR_RESETVAL (0x00000004u)

#define CSL_MDIO_VERSION_REG_REVMAJ_MASK (0x0000FF00u)
#define CSL_MDIO_VERSION_REG_REVMAJ_SHIFT (0x00000008u)
#define CSL_MDIO_VERSION_REG_REVMAJ_RESETVAL (0x00000001u)

#define CSL_MDIO_VERSION_REG_MODID_MASK (0xFFFF0000u)
#define CSL_MDIO_VERSION_REG_MODID_SHIFT (0x00000010u)
#define CSL_MDIO_VERSION_REG_MODID_RESETVAL (0x00000007u)

#define CSL_MDIO_VERSION_REG_RESETVAL (0x00070104u)

/* mdio_control_reg */

#define CSL_MDIO_CONTROL_REG_CLKDIV_MASK (0x0000FFFFu)
#define CSL_MDIO_CONTROL_REG_CLKDIV_SHIFT (0x00000000u)
#define CSL_MDIO_CONTROL_REG_CLKDIV_RESETVAL (0x000000FFu)

#define CSL_MDIO_CONTROL_REG_FAULT_DETECT_ENABLE_MASK (0x00040000u)
#define CSL_MDIO_CONTROL_REG_FAULT_DETECT_ENABLE_SHIFT (0x00000012u)
#define CSL_MDIO_CONTROL_REG_FAULT_DETECT_ENABLE_RESETVAL (0x00000000u)

#define CSL_MDIO_CONTROL_REG_FAULT_MASK (0x00080000u)
#define CSL_MDIO_CONTROL_REG_FAULT_SHIFT (0x00000013u)
#define CSL_MDIO_CONTROL_REG_FAULT_RESETVAL (0x00000000u)

#define CSL_MDIO_CONTROL_REG_PREAMBLE_MASK (0x00100000u)
#define CSL_MDIO_CONTROL_REG_PREAMBLE_SHIFT (0x00000014u)
#define CSL_MDIO_CONTROL_REG_PREAMBLE_RESETVAL (0x00000000u)

#define CSL_MDIO_CONTROL_REG_HIGHEST_USER_CHANNEL_MASK (0x1F000000u)
#define CSL_MDIO_CONTROL_REG_HIGHEST_USER_CHANNEL_SHIFT (0x00000018u)
#define CSL_MDIO_CONTROL_REG_HIGHEST_USER_CHANNEL_RESETVAL (0x00000001u)

#define CSL_MDIO_CONTROL_REG_ENABLE_MASK (0x40000000u)
#define CSL_MDIO_CONTROL_REG_ENABLE_SHIFT (0x0000001Eu)
#define CSL_MDIO_CONTROL_REG_ENABLE_RESETVAL (0x00000000u)

#define CSL_MDIO_CONTROL_REG_IDLE_MASK (0x80000000u)
#define CSL_MDIO_CONTROL_REG_IDLE_SHIFT (0x0000001Fu)
#define CSL_MDIO_CONTROL_REG_IDLE_RESETVAL (0x00000001u)

#define CSL_MDIO_CONTROL_REG_RESETVAL (0x810000FFu)

/* mdio_alive_reg */

#define CSL_MDIO_ALIVE_REG_ALIVE_MASK (0xFFFFFFFFu)
#define CSL_MDIO_ALIVE_REG_ALIVE_SHIFT (0x00000000u)
#define CSL_MDIO_ALIVE_REG_ALIVE_RESETVAL (0x00000000u)

#define CSL_MDIO_ALIVE_REG_RESETVAL (0x00000000u)

/* mdio_link_reg */

#define CSL_MDIO_LINK_REG_LINK_MASK (0xFFFFFFFFu)
#define CSL_MDIO_LINK_REG_LINK_SHIFT (0x00000000u)
#define CSL_MDIO_LINK_REG_LINK_RESETVAL (0x00000000u)

#define CSL_MDIO_LINK_REG_RESETVAL (0x00000000u)

/* mdio_link_int_raw_reg */

#define CSL_MDIO_LINK_INT_RAW_REG_LINKINTRAW_MASK (0x00000003u)
#define CSL_MDIO_LINK_INT_RAW_REG_LINKINTRAW_SHIFT (0x00000000u)
#define CSL_MDIO_LINK_INT_RAW_REG_LINKINTRAW_RESETVAL (0x00000000u)

#define CSL_MDIO_LINK_INT_RAW_REG_RESETVAL (0x00000000u)

/* mdio_link_int_masked_reg */

#define CSL_MDIO_LINK_INT_MASKED_REG_LINKINTMASKED_MASK (0x00000003u)
#define CSL_MDIO_LINK_INT_MASKED_REG_LINKINTMASKED_SHIFT (0x00000000u)
#define CSL_MDIO_LINK_INT_MASKED_REG_LINKINTMASKED_RESETVAL (0x00000000u)

#define CSL_MDIO_LINK_INT_MASKED_REG_RESETVAL (0x00000000u)

/* mdio_user_int_raw_reg */

#define CSL_MDIO_USER_INT_RAW_REG_USERINTRAW_MASK (0x00000003u)
#define CSL_MDIO_USER_INT_RAW_REG_USERINTRAW_SHIFT (0x00000000u)
#define CSL_MDIO_USER_INT_RAW_REG_USERINTRAW_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_INT_RAW_REG_RESETVAL (0x00000000u)

/* mdio_user_int_masked_reg */

#define CSL_MDIO_USER_INT_MASKED_REG_USERINTMASKED_MASK (0x00000003u)
#define CSL_MDIO_USER_INT_MASKED_REG_USERINTMASKED_SHIFT (0x00000000u)
#define CSL_MDIO_USER_INT_MASKED_REG_USERINTMASKED_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_INT_MASKED_REG_RESETVAL (0x00000000u)

/* mdio_user_int_mask_set_reg */

#define CSL_MDIO_USER_INT_MASK_SET_REG_USERINTMASKSET_MASK (0x00000003u)
#define CSL_MDIO_USER_INT_MASK_SET_REG_USERINTMASKSET_SHIFT (0x00000000u)
#define CSL_MDIO_USER_INT_MASK_SET_REG_USERINTMASKSET_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_INT_MASK_SET_REG_RESETVAL (0x00000000u)

/* mdio_user_int_mask_clear_reg */

#define CSL_MDIO_USER_INT_MASK_CLEAR_REG_USERINTMASKCLR_MASK (0x00000003u)
#define CSL_MDIO_USER_INT_MASK_CLEAR_REG_USERINTMASKCLR_SHIFT (0x00000000u)
#define CSL_MDIO_USER_INT_MASK_CLEAR_REG_USERINTMASKCLR_RESETVAL (0x00000000u)

#define CSL_MDIO_USER_INT_MASK_CLEAR_REG_RESETVAL (0x00000000u)

#endif
