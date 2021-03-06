/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/common/pmlib/include/pstate_pgpe_occ_api.h $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/// @file  p9_pstates_pgpe_occ_api.h
/// @brief Structures used between PGPE HCode and OCC Firmware
///
// *HWP HW Owner        : Rahul Batra <rbatra@us.ibm.com>
// *HWP HW Owner        : Michael Floyd <mfloyd@us.ibm.com>
// *HWP Team            : PM
// *HWP Level           : 1
// *HWP Consumed by     : PGPE:OCC


#ifndef __P9_PSTATES_PGPE_API_H__
#define __P9_PSTATES_PGPE_API_H__

#include <p9_pstates_common.h>

#ifdef __cplusplus
extern "C" {
#endif

//---------------
// IPC from 405
//---------------

enum MESSAGE_ID_IPI2HI
{
    MSGID_405_INVALID       = 0,
    MSGID_405_START_SUSPEND = 1,
    MSGID_405_CLIPS         = 2,
    MSGID_405_WOF_INDEX     = 3
};

//
// Return Codes
//
// Will be filled in with the PK_PANIC code

//
// PMCR Owner
//
enum PMCR_OWNER
{
    PMCR_OWNER_OCC          = 0,
    PMCR_OWNER_HOST         = 1
};

typedef struct ipcmsg_base
{
    uint8_t   msg_id;
    uint8_t   rc;
} ipcmsg_base_t;

typedef struct ipcmsg_start_suspend
{
    ipcmsg_base_t   msg_cb;
    uint8_t         action;
    PMCR_OWNER      pmcr_owner;

} ipcmsg_start_suspend_t;


typedef struct ipcmsg_clips
{
    ipcmsg_base_t   msg_cb;
    uint8_t         ps_clip_min[MAX_QUADS];
    uint8_t         ps_clip_max[MAX_QUADS];
    uint8_t         pad[2];
} ipcmsg_clips_t;


typedef struct ipcmsg_wof
{
    ipcmsg_base_t   msg_cb;
    uint8_t         enable;         // WOF enable
    uint8_t         ceff_vdd_index; // Effective Capacitance VDD
    uint8_t         ceff_vdn_index; // Effective Capacitance VDN
    uint8_t         fratio_index;   // Frequency Ratio
    uint8_t         vratio_index;   // Voltage Ratio
    uint8_t         pad;
} ipcmsg_wof_t;


typedef struct ipcmsg_reset
{
    ipcmsg_base_t   msg_cb;
} ipcmsg_reset_t;


// -----------------------------------------------------------------------------
// Start Pstate Table

#define MAX_PSTATE_TABLE_ENTRIES 256

/// Pstate Table produce by the PGPE for consumption by OCC Firmware
///
/// This structure defines the Pstate Table content
/// -- 16B structure

typedef struct
{
    /// Pstate number
    Pstate      pstate;

    /// Assocated Frequency (in MHz)
    uint16_t    frequency_mhz;

    /// Internal VDD voltage ID at the output of the PFET header
    uint8_t    internal_vdd_vid;

} OCCPstateTable_entry_t;

typedef struct
{
    /// Number of Pstate Table entries
    uint32_t                entries;

    /// Internal VDD voltage ID at the output of the PFET header
    OCCPstateTable_entry_t  table[MAX_PSTATE_TABLE_ENTRIES];

} OCCPstateTable_t;

// End Pstate Table
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Start FFDC

/// Scopes of the First Failure Data Capture (FFDC) registers
enum scope_type =
{
    FFDC_CHIP = 0,  // Address is chip scope (eg absolute)
    FFDC_QUAD = 1,  // Address + 0x01000000*quad for good quads from 0 to 5
    FFDC_CORE = 2,  // Address + 0x01000000*core for good cores from 0 to 23
    FFDC_CME = 3    // Address if EX is even; Address + 0x400*EX for EX odd for good Exs from 0 to 11
};

/// Address types of First Failure Data Capture (FFDC) register addresses
enum scope_type =
{
    FFDC_OCI  = 0,   // Address is an OCI address
    FFDC_SCOM = 1    // Address is a SCOM address
};

/// Register definition of the Hcode FFDC register list
typedef struct
{
    uint32_t            address;
    union address_attribute
    {
        uint32_t value;
        struct
        {
            uint32_t    address_type : 16;
            uint32_t    scope        : 16;
        } attr;
    }
} Hcode_FFDC_entry_t;

/// Hcode FFDC register list
typedef struct
{
    /// Number of FFDC address list entries
    uint32_t            list_entries;

    /// FFDC Address list
    Hcode_FFDC_entry_t  list[MAX_FFDC_REG_LIST];
} Hcode_FFDC_list_t;



/// Hcode FFDC register list
/// @todo RTC: 161183  Fill out the rest of this FFDC list
/// @note The reserved FFDC space for registers and traces set aside in the
/// OCC is 1KB.   On the register side, the following list will generate
/// 12B of content (4B address, 8B data) x the good entries per scope.
/// CHIP scope are not dependent on partial good or currently active and will
/// take 12B x 8 = 96B.  CME scope entries will, at maximum, generate 12B x
/// 12 CMEs x  4 SCOMs = 576B..  The overall  totla for registers is 96 + 576
///
#define MAX_FFDC_REG_LIST 12
typedef struct Hcode_FFDC_list =
{
    {PERV_TP_OCC_SCOM_OCCLFIR,  FFDC_SCOM, FFDC_CHIP }, // OCC LFIR
    {PU_PBAFIR,                 FFDC_SCOM, FFDC_CHIP }, // PBA LFIR
    {EX_CME_SCOM_LFIR,          FFDC_SCOM, FFDC_CME  }, // CME LFIR
    {PU_GPE3_GPEDBG_OCI,        FFDC_OCI,  FFDC_CHIP }, // SGPE XSR, SPRG0
    {PU_GPE3_GPEDDR_OCI,        FFDC_OCI,  FFDC_CHIP }, // SGPE IR, EDR
    {PU_GPE3_PPE_XIDBGPRO,      FFDC_OCI,  FFDC_CHIP }, // SGPE XSR, IAR
    {PU_GPE2_GPEDBG_OCI,        FFDC_OCI,  FFDC_CHIP }, // PGPE XSR, SPRG0
    {PU_GPE2_GPEDDR_OCI,        FFDC_OCI,  FFDC_CHIP }, // PGPE IR, EDR
    {PU_GPE2_PPE_XIDBGPRO,      FFDC_OCI,  FFDC_CHIP }, // PGPE XSR, IAR
    {EX_PPE_XIRAMDBG,           FFDC_SCOM, FFDC_CME  }, // CME XSR, SPRG0
    {EX_PPE_XIRAMEDR,           FFDC_SCOM, FFDC_CME  }, // CME IR, EDR
    {EX_PPE_XIDBGPRO,           FFDC_SCOM, FFDC_CME  }, // CME XSR, IAR
};

// End FFDC
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Start Quad State

typedef union quad_state0
{
    uint64_t value;
    struct
    {
        uint32_t high_order;
        uint32_t low_order;
    } words;
    struct
    {
        uint64_t quad0_pstate             : 8;  // Pstate of Quad 0; 0xFF indicates EQ is off
        uint64_t quad1_pstate             : 8;  // Pstate of Quad 1; 0xFF indicates EQ is off
        uint64_t quad2_pstate             : 8;  // Pstate of Quad 2; 0xFF indicates EQ is off
        uint64_t quad3_pstate             : 8;  // Pstate of Quad 3; 0xFF indicates EQ is off
        uint64_t ivrm_state               : 4;  // ivrm state: bit vector 0:quad0, 1:quad1, 2:quad2, 3;quad3
        uint64_t ivrm_state_rsvd          : 4;
        uint64_t core_poweron_state       : 16; // bit vector: 0:core0, 1:core1, ..., 15:core15
        uint64_t external_vrm_setpoint    : 8;  // set point in mV
    } fields;
} quad_state0_t;

typedef union quad_state1
{
    uint64_t value;
    struct
    {
        uint32_t high_order;
        uint32_t low_order;
    } words;
    struct
    {
        uint64_t quad4_pstate             : 8;  // Pstate of Quad 4; 0xFF indicates EQ is off
        uint64_t quad5_pstate             : 8;  // Pstate of Quad 5; 0xFF indicates EQ is off
        uint64_t quad_pstate_rsvd         : 16;
        uint64_t ivrm_state               : 2;  // ivrm state: bit vector 0:quad4, 1:quad5
        uint64_t ivrm_state_rsvd          : 6;
        uint64_t core_poweron_state       : 8;  // bit vector: 0:core16, 1:core17, ..., 7:core23
        uint64_t core_poweron_state_rsvd  : 8;
        uint64_t external_vrm_setpoint    : 8;  // set point in mV
    } fields;
} quad_state1_t;

// End Quad State
// -----------------------------------------------------------------------------


typedef struct
{
    /// Magic number + version.  "OPS" || version (nibble)
    uint32_t            magic;

    /// PGPE Beacon
    uint32_t            pgpe_beacon;

    /// Actual Pstate 0 - Quads 0, 1, 2, 3
    quad_state0_t       quad_pstate_0;

    /// Actual Pstate 1 - Quads 4, 5
    quad_state1_t       quad_pstate_1;

    /// FFDC Address list
    Hcode_FFDC_list_t   ffdc_list;

    /// Pstate Table
    OCCPstateTable_t    pstate_table;

} HcodeOCCSharedData_t;

#ifdef __cplusplus
} // end extern C
#endif

#endif    /* __P9_PSTATES_PGPE_API_H__ */
