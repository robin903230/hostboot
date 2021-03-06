/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/diag/prdf/plat/prdfPlatServices.C $                   */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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

/**
 * @file  prdfPlatServices.C
 * @brief Wrapper code for external interfaces used by PRD.
 *
 * This file contains code that is strictly specific to Hostboot. All code that
 * is common between FSP and Hostboot should be in the respective common file.
 */

#include <prdfPlatServices.H>

#include <prdfGlobal.H>
#include <prdfErrlUtil.H>
#include <prdfTrace.H>
#include <prdfAssert.h>

#include <iipServiceDataCollector.h>
#include <UtilHash.H>

#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <initservice/initserviceif.H>
#include <devicefw/userif.H>
#include <iipMopRegisterAccess.h>
#include <ibscomreasoncodes.H>

using namespace TARGETING;

namespace PRDF
{

namespace PlatServices
{

//##############################################################################
//##                      System Level Utility functions
//##############################################################################

void getCurrentTime( Timer & o_timer )
{
    timespec_t curTime;
    PRDF_ASSERT(0 == clock_gettime(CLOCK_MONOTONIC, &curTime))

    // Hostboot uptime in seconds
    o_timer = curTime.tv_sec;

    // Since Hostboot doesn't have any system checkstop, we don't have to worry
    // about the detailed time struct for system checkstop timestamp.
}

//------------------------------------------------------------------------------

void milliSleep( uint32_t i_seconds, uint32_t i_milliseconds )
{
    nanosleep( i_seconds, i_milliseconds * 1000000 );
}

//------------------------------------------------------------------------------

/* TODO RTC 144705
void initiateUnitDump( TargetHandle_t i_target,
                       errlHndl_t i_errl,
                       uint32_t i_errlActions )
{
    // no-op in Hostboot but just go ahead and commit
    // the errorlog in case it's not null.
    if ( NULL != i_errl )
    {
        PRDF_COMMIT_ERRL(i_errl, i_errlActions);
    }
}
*/

//------------------------------------------------------------------------------

bool isSpConfigFsp()
{
    #ifdef __HOSTBOOT_RUNTIME

    return false; // Should never have an FSP when using HBRT.

    #else

    return INITSERVICE::spBaseServicesEnabled();

    #endif
}

//------------------------------------------------------------------------------

uint32_t getScom(TARGETING::TargetHandle_t i_target, BIT_STRING_CLASS& io_bs,
                   uint64_t i_address)
{
    errlHndl_t errl = NULL;
    uint32_t rc = SUCCESS;
    uint32_t tempBitOffset;
    size_t bsize = (io_bs.GetLength()+7)/8;
    CPU_WORD* buffer = io_bs.GetRelativePositionAlloc(tempBitOffset, 0);

    errl = deviceRead(i_target, buffer, bsize, DEVICE_SCOM_ADDRESS(i_address));

    if(( NULL != errl ) && ( IBSCOM::IBSCOM_BUS_FAILURE == errl->reasonCode() ))
    {
        PRDF_SET_ERRL_SEV(errl, ERRL_SEV_INFORMATIONAL);
        PRDF_COMMIT_ERRL(errl, ERRL_ACTION_HIDDEN);
        PRDF_INF( "Register access failed with reason code IBSCOM_BUS_FAILURE."
                  " Trying again, Target HUID:0x%08X Register 0x%016X Op:%u",
                  PlatServices::getHuid( i_target), i_address,
                  MopRegisterAccess::READ );

        errl = deviceRead(i_target, buffer, bsize,
                          DEVICE_SCOM_ADDRESS(i_address));
    }

    if( NULL != errl )
    {
        PRDF_ERR("[ScomAccessor::Access()] Error in "
                 "PRDF::PlatServices::getScom");
        rc = PRD_SCANCOM_FAILURE;
        PRDF_ADD_SW_ERR(errl, rc, PRDF_HOM_SCOM, __LINE__);

        bool l_isAbort = false;
        PRDF_ABORTING(l_isAbort);
        if (!l_isAbort)
        {
            PRDF_SET_ERRL_SEV(errl, ERRL_SEV_INFORMATIONAL);
            PRDF_COMMIT_ERRL(errl, ERRL_ACTION_HIDDEN);
        }
        else
        {
            delete errl;
            errl = NULL;
        }
    }

    return rc;
}

//------------------------------------------------------------------------------

uint32_t putScom(TARGETING::TargetHandle_t i_target, BIT_STRING_CLASS& io_bs,
                   uint64_t i_address)
{
    errlHndl_t errl = NULL;
    uint32_t rc = SUCCESS;
    uint32_t tempBitOffset;
    size_t bsize = (io_bs.GetLength()+7)/8;
    CPU_WORD* buffer = io_bs.GetRelativePositionAlloc(tempBitOffset, 0);

    errl = deviceWrite(i_target, buffer, bsize, DEVICE_SCOM_ADDRESS(i_address));

    if( NULL != errl )
    {
        PRDF_ERR("[ScomAccessor::Access()] Error in "
                 "PRDF::PlatServices::putScom");
        rc = PRD_SCANCOM_FAILURE;
        PRDF_ADD_SW_ERR(errl, rc, PRDF_HOM_SCOM, __LINE__);

        bool l_isAbort = false;
        PRDF_ABORTING(l_isAbort);
        if (!l_isAbort)
        {
            PRDF_SET_ERRL_SEV(errl, ERRL_SEV_INFORMATIONAL);
            PRDF_COMMIT_ERRL(errl, ERRL_ACTION_HIDDEN);
        }
        else
        {
            delete errl;
            errl = NULL;
        }
    }

    return rc;
}


//##############################################################################
//##                       Processor specific functions
//##############################################################################

/* TODO RTC 136050
void collectSBE_FFDC(TARGETING::TargetHandle_t i_procTarget)
{
    // Do nothing for Hostboot
}
*/

//------------------------------------------------------------------------------

TARGETING::TargetHandle_t getMasterCore( TARGETING::TargetHandle_t i_procTgt )
{
    #define PRDF_FUNC "[PlatServices::getMasterCore] "

    PRDF_ERR( PRDF_FUNC "MasterCore info not available in hostboot: PROC = "
              "0x%08x ",getHuid( i_procTgt ) );
    return NULL;

    #undef PRDF_FUNC
}

//##############################################################################
//##                        util functions
//##############################################################################

int32_t getCfam( ExtensibleChip * i_chip,
                 const uint32_t i_addr,
                 uint32_t & o_data)
{
    #define PRDF_FUNC "[PlatServices::getCfam] "

    int32_t rc = SUCCESS;

    do
    {
        // HB doesn't allow cfam access on master proc
        TargetHandle_t l_procTgt = i_chip->GetChipHandle();

        if( TYPE_PROC == getTargetType(l_procTgt) )
        {
            TargetHandle_t l_pMasterProcChip = NULL;
            targetService().
                masterProcChipTargetHandle( l_pMasterProcChip );

            if( l_pMasterProcChip == l_procTgt )
            {
                PRDF_DTRAC( PRDF_FUNC "can't access CFAM from master "
                            "proc: 0x%.8X", i_chip->GetId() );
                break;
            }
        }

        errlHndl_t errH = NULL;
        size_t l_size = sizeof(uint32_t);
        errH = deviceRead(l_procTgt, &o_data, l_size,
                          DEVICE_FSI_ADDRESS((uint64_t) i_addr));
        if (errH)
        {
            rc = FAIL;
            PRDF_ERR( PRDF_FUNC "chip: 0x%.8X, failed to get cfam address: "
                      "0x%X", i_chip->GetId(), i_addr );
            PRDF_COMMIT_ERRL(errH, ERRL_ACTION_SA|ERRL_ACTION_REPORT);
            break;
        }


    } while(0);


    return rc;

    #undef PRDF_FUNC
}

//------------------------------------------------------------------------------

TARGETING::TargetHandle_t getActiveRefClk(TARGETING::TargetHandle_t
                            i_procTarget,
                            TARGETING::TYPE i_connType,
                            uint32_t i_oscPos)
{
    return PlatServices::getClockId( i_procTarget,
                                     i_connType,
                                     i_oscPos );
}

//##############################################################################
//##                    Nimbus Maintenance Command wrappers
//##############################################################################

template<>
uint32_t startBgScrub<TYPE_MCA>( TargetHandle_t i_trgt, const MemRank & i_rank )
{
    #define PRDF_FUNC "[PlatServices::startBgScrub<TYPE_MCA>] "

    PRDF_ASSERT( nullptr != i_trgt );
    PRDF_ASSERT( TYPE_MCA == getTargetType(i_trgt) );

    uint32_t rc = SUCCESS;

    TargetHandle_t mcbTrgt = getConnectedParent( i_trgt, TYPE_MCBIST );
    PRDF_ASSERT( nullptr != mcbTrgt );

    uint32_t port = getTargetPosition(i_trgt) % MAX_MCA_PER_MCBIST;

    fapi2::Target<fapi2::TARGET_TYPE_MCBIST> fapiTrgt ( mcbTrgt );

    mss::mcbist::stop_conditions stopCond;
    stopCond.set_thresh_nce_int(1)
            .set_thresh_nce_soft(1)
            .set_thresh_nce_hard(1)
            .set_pause_on_mpe(mss::ON)
            .set_pause_on_ue(mss::ON)
            .set_pause_on_aue(mss::ON)
            .set_nce_hard_symbol_count_enable(mss::ON);

    mss::mcbist::speed scrubSpeed = enableFastBgScrub() ? mss::mcbist::LUDICROUS
                                                        : mss::mcbist::BG_SCRUB;

    mss::mcbist::address saddr, eaddr;
    mss::mcbist::address::get_srank_range( port,
                                           i_rank.getDimmSlct(),
                                           i_rank.getRankSlct(),
                                           i_rank.getSlave(),
                                           saddr,
                                           eaddr );

    fapi2::ReturnCode fapi_rc = memdiags::background_scrub( fapiTrgt, stopCond,
                                                            scrubSpeed, saddr );

    errlHndl_t errl = fapi2::rcToErrl( fapi_rc );
    if ( nullptr != errl )
    {
        PRDF_ERR( PRDF_FUNC "memdiags::stop(0x%08x) failed", getHuid(i_trgt) );
        PRDF_COMMIT_ERRL( errl, ERRL_ACTION_REPORT );
        rc = FAIL;
    }

    return rc;

    #undef PRDF_FUNC
}

//##############################################################################
//##                   Centaur Maintenance Command wrappers
//##############################################################################

template<>
uint32_t startBgScrub<TYPE_MBA>( TargetHandle_t i_trgt,
                                 const MemRank & i_rank )
{
    #define PRDF_FUNC "[PlatServices::startBgScrub<TYPE_MBA>] "

    PRDF_ASSERT( nullptr != i_trgt );
    PRDF_ASSERT( TYPE_MBA == getTargetType(i_trgt) );

    uint32_t rc = SUCCESS;

    PRDF_ERR( PRDF_FUNC "function not implemented yet" );
// TODO RTC 136126

    return rc;

    #undef PRDF_FUNC
}

//------------------------------------------------------------------------------

} // end namespace PlatServices

} // end namespace PRDF

