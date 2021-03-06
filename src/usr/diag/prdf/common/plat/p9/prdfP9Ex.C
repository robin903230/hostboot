/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/diag/prdf/common/plat/p9/prdfP9Ex.C $                 */
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
// Framework includes
#include <prdfPluginDef.H>
#include <iipServiceDataCollector.h>
#include <prdfExtensibleChip.H>
#include <prdfPluginMap.H>

#include <prdfP9ExDataBundle.H>
#include <prdfP9ExExtraSig.H>
#include <prdfMfgThresholdMgr.H>
#include <prdfMfgThreshold.H>

namespace PRDF
{
namespace p9_ex
{

/**
 * @brief  Plugin that initializes the EX data bundle.
 * @param  i_exChip An ex chip.
 * @return SUCCESS
 */
int32_t Initialize( ExtensibleChip * i_exChip )
{
    i_exChip->getDataBundle() = new P9ExDataBundle( i_exChip );
    return SUCCESS;
}
PRDF_PLUGIN_DEFINE( p9_ex, Initialize );

/**
 * @brief Handle an L3 CE
 * @param i_chip Ex chip.
 * @param i_stepcode Step Code data struct
 * @return PRD return code
 */
int32_t L3CE( ExtensibleChip * i_chip,
                           STEP_CODE_DATA_STRUCT & i_stepcode )
{
    P9ExDataBundle * l_bundle = getExDataBundle(i_chip);
    uint16_t l_maxL3LineDelAllowed = 0;


    l_maxL3LineDelAllowed =
      MfgThresholdMgr::getInstance()->getThreshold(PlatServices::mfgMode() ?
                               TARGETING::ATTR_MNFG_TH_P8EX_L3_LINE_DELETES:
                               TARGETING::ATTR_FIELD_TH_P8EX_L3_LINE_DELETES);

    // MfgThresholdMgr treats 0 as a special value for infinite threshold
    // For this threshold, we want 0 to really represent 0 repairs allowed
    if (l_maxL3LineDelAllowed == MfgThreshold::INFINITE_LIMIT_THR)
        l_maxL3LineDelAllowed = 0;

    // Ensure we're still allowed to issue repairs
    if ((l_bundle->iv_L3LDCount < l_maxL3LineDelAllowed) &&
        (CHECK_STOP != i_stepcode.service_data->getPrimaryAttnType()))
    {
        // Add to CE table and Check if we need to issue a repair on this CE
        bool l_doDelete =
          l_bundle->iv_L3CETable->addAddress(l_bundle->iv_L3LDCount,
                                             i_stepcode);

        if (l_doDelete)
        {
            l_bundle->iv_L3LDCount++;

            // Do Delete
            PRDF_TRAC( "[L3CE] HUID: 0x%08x apply line delete",
                        i_chip->GetId());

            SCAN_COMM_REGISTER_CLASS * prgReg =
                               i_chip->getRegister("L3_PURGE_REG");

            prgReg->clearAllBits();
            prgReg->SetBit(5);

            if (SUCCESS != prgReg->Write() )
            {
                PRDF_ERR( "[L3CE] HUID: 0x%08x l3LineDelete failed",
                          i_chip->GetId());
                // Set signature to indicate L3 Line Delete failed
                i_stepcode.service_data->SetErrorSig(
                                PRDFSIG_P9EX_L3CE_LD_FAILURE);
            }
            else
            {
                // Set signature to indicate L3 Line Delete issued
                i_stepcode.service_data->SetErrorSig(
                                PRDFSIG_P9EX_L3CE_LD_ISSUED);
            }
        }
    }
    else
    {
        PRDF_TRAC( "[L3CE] HUID: 0x%08x No more repairs allowed",
                   i_chip->GetId());

        // MFG wants to be able to ignore these errors
        // If they have LD and array repairs set to 0, wait for
        // predictive threshold
        if (!PlatServices::mfgMode() ||
            l_maxL3LineDelAllowed != 0 )
        {
            i_stepcode.service_data->SetThresholdMaskId(0);
        }
    }

    return SUCCESS;
} PRDF_PLUGIN_DEFINE(p9_ex, L3CE);

}
}
