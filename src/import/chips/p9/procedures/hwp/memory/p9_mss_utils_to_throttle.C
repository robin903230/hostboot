/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/memory/p9_mss_utils_to_throttle.C $ */
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
/// @file p9_mss_utils_to_throttle.C
/// @brief Sets throttles
/// TMGT will call this procedure to set the N address operations (commands)
/// allowed within a window of M DRAM clocks given the minimum dram data bus utilization.
///

// *HWP HWP Owner: Jacob Harvey <jlharvey@us.ibm.com>
// *HWP HWP Backup: Brian Silver <bsilver@us.ibm.com>
// *HWP Team: Memory
// *HWP Level: 2
// *HWP Consumed by: FSP:HB

#include <p9_mss_utils_to_throttle.H>

// fapi2
#include <fapi2.H>

// mss lib
#include <lib/power_thermal/throttle.H>
#include <lib/utils/index.H>
#include <lib/utils/find.H>
#include <lib/utils/conversions.H>
#include <lib/power_thermal/throttle.H>
#include <lib/mss_attribute_accessors.H>

using fapi2::TARGET_TYPE_MCS;
using fapi2::TARGET_TYPE_MCA;
using fapi2::TARGET_TYPE_DIMM;

extern "C"
{

///
/// @brief Sets number commands allowed within a given data bus utilization.
/// @param[in] i_targets vector of MCS on the same VDDR domain
/// @return FAPI2_RC_SUCCESS iff ok
/// @note throttle_per_slot will be equalized so all throttles coming out will be equal to worst case
///
    fapi2::ReturnCode p9_mss_utils_to_throttle( const std::vector< fapi2::Target<TARGET_TYPE_MCS> >& i_targets )
    {
        for( const auto& l_mcs : i_targets )
        {
            uint32_t l_databus_util [mss::PORTS_PER_MCS];
            uint32_t l_dram_clocks = 0;

            uint16_t l_throttled_cmds_port[mss::PORTS_PER_MCS];
            uint16_t l_throttled_cmds_slot[mss::PORTS_PER_MCS];
            uint32_t l_max_power[mss::PORTS_PER_MCS];

            FAPI_TRY( mss::mrw_mem_m_dram_clocks(l_dram_clocks) );
            //Util attribute set by OCC
            FAPI_TRY( mss::databus_util(l_mcs, l_databus_util) );

            for( const auto& l_mca : mss::find_targets<TARGET_TYPE_MCA>(l_mcs) )
            {
                const auto l_port_num = mss::index( l_mca );
                //Make a throttle object in order to calculate the port power
                fapi2::ReturnCode l_rc;
                mss::power_thermal::throttle l_throttle (l_mca, l_rc);
                FAPI_TRY(l_rc, "Error calculating mss::power_thermal::throttle constructor in p9_mss_utils_to_throttles");

                FAPI_INF( "MRW dram clock window: %d, databus utilization: %d",
                          l_dram_clocks,
                          l_databus_util);

                //Calculate programmable N address operations within M dram clock window
                l_throttled_cmds_port[l_port_num] = mss::power_thermal::throttled_cmds(l_databus_util[l_port_num],
                                                    l_dram_clocks );
                l_throttled_cmds_slot[l_port_num] = l_throttled_cmds_port[l_port_num];
                //Calculate the port power needed to reach the calculated N value
                l_max_power[l_port_num] = l_throttle.calc_power_from_n(l_throttled_cmds_slot[l_port_num]);

                FAPI_INF( "Calculated N commands per port [%d] = %d commands, maxpower is %d",
                          l_port_num,
                          l_throttled_cmds_port[l_port_num],
                          l_max_power[l_port_num]);
            }// end for

            FAPI_TRY( FAPI_ATTR_SET(fapi2::ATTR_MSS_PORT_MAXPOWER,
                                    l_mcs,
                                    l_max_power) );
            FAPI_TRY( FAPI_ATTR_SET(fapi2::ATTR_MSS_MEM_THROTTLED_N_COMMANDS_PER_SLOT,
                                    l_mcs,
                                    l_throttled_cmds_slot) );
            FAPI_TRY( FAPI_ATTR_SET(fapi2::ATTR_MSS_MEM_THROTTLED_N_COMMANDS_PER_PORT,
                                    l_mcs,
                                    l_throttled_cmds_port) );
        }

    fapi_try_exit:
        return fapi2::current_err;
    }

}// extern C
