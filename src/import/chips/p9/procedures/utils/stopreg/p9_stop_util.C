/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/utils/stopreg/p9_stop_util.C $ */
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

///
/// @file   p9_stop_util.C
/// @brief  implements some utilty functions for STOP API.
///
// *HWP HW Owner    :  Greg Still <stillgs@us.ibm.com>
// *HWP FW Owner    :  Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team        :  PM
// *HWP Level       :  2
// *HWP Consumed by :  HB:HYP

#include "p9_stop_api.H"
#include "p9_stop_util.H"
#include "p9_stop_data_struct.H"

#ifdef __cplusplus
namespace stopImageSection
{
#endif

/**
 * @brief   Returns proc chip's fuse mode status.
 * @param   i_pImage    points to start of chip's HOMER image.
 * @param   o_fuseMode  points to fuse mode information.
 * @return  STOP_SAVE_SUCCESS if functions succeeds, error code otherwise.
 */
StopReturnCode_t  isFusedMode( void* const i_pImage, bool* o_fuseMode )
{
    *o_fuseMode = false;
    StopReturnCode_t l_rc = STOP_SAVE_SUCCESS;

    do
    {
        if( !i_pImage )
        {
            MY_ERR( "invalid pointer to HOMER image");
            l_rc = STOP_SAVE_ARG_INVALID_IMG;
            break;
        }

        HomerSection_t* pHomerDesc = ( HomerSection_t* ) i_pImage;
        HomerImgDesc_t* pHomer =  (HomerImgDesc_t*)( pHomerDesc->interrruptHandler );

        if( SWIZZLE_8_BYTE(HOMER_MAGIC_WORD) != pHomer->homerMagicNumber )
        {
            MY_ERR("corrupt or invalid HOMER image location 0x%016llx",
                   pHomer->homerMagicNumber );
            break;
        }


        if( (uint8_t) FUSE_MODE == pHomer->fuseModeStatus )
        {
            *o_fuseMode = true;
            break;
        }

        if( (uint8_t) REGULAR_MODE == pHomer->fuseModeStatus )
        {
            break;
        }

        MY_ERR("Unexpected value 0x%08x for fuse mode. Bad or corrupt "
               "HOMER location", pHomer->fuseModeStatus );
        l_rc = STOP_SAVE_FAIL;

    }
    while(0);

    return l_rc;
}

//----------------------------------------------------------------------

StopReturnCode_t getCoreAndThread( void* const i_pImage, const uint64_t i_pir,
                                   uint32_t* o_pCoreId, uint32_t* o_pThreadId )
{
    StopReturnCode_t l_rc = STOP_SAVE_SUCCESS;

    do
    {
        // for SPR restore using 'Virtual Thread' and 'Physical Core' number
        // In Fuse Mode:
        // bit b28 and b31 of PIR give physical core and b29 and b30 gives
        // virtual thread id.
        // In Non Fuse Mode
        // bit 28 and b29 of PIR give both logical and physical core number
        // whereas b30 and b31 gives logical and virtual thread id.
        bool fuseMode = false;
        uint8_t coreThreadInfo = (uint8_t)i_pir;
        *o_pCoreId = 0;
        *o_pThreadId = 0;
        l_rc = isFusedMode( i_pImage, &fuseMode );

        if( l_rc )
        {
            MY_ERR(" Checking Fuse mode. Read failed 0x%08x", l_rc );
            break;
        }

        if( fuseMode )
        {
            if( coreThreadInfo & FUSE_BIT1 )
            {
                *o_pThreadId = 2;
            }

            if( coreThreadInfo & FUSE_BIT2 )
            {
                *o_pThreadId += 1;
            }

            if( coreThreadInfo & FUSE_BIT0 )
            {
                *o_pCoreId = 2;
            }

            if( coreThreadInfo & FUSE_BIT3 )
            {
                *o_pCoreId += 1;
            }
        }
        else
        {
            if( coreThreadInfo & FUSE_BIT0 )
            {
                *o_pCoreId = 2;
            }

            if ( coreThreadInfo & FUSE_BIT1 )
            {
                *o_pCoreId += 1;
            }

            if( coreThreadInfo & FUSE_BIT2 )
            {
                *o_pThreadId = 2;
            }

            if( coreThreadInfo & FUSE_BIT3 )
            {
                *o_pThreadId += 1;
            }
        }

        //quad field is not affected by fuse mode
        *o_pCoreId += 4 * (( coreThreadInfo & 0x70 ) >> 4 );
    }
    while(0);

    return l_rc;
}

#ifdef __cplusplus
}//namespace stopImageSection ends
#endif

