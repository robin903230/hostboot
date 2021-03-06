/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/fapi2/test/p9_hwtests.C $                             */
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
//------------------------------------------------------------------------------
/// @file  p9_hwtests.C
///
/// @brief These procedures test the fapi2 hw_access interfaces.
//-----------------------------------------------------------------------------

#include <cxxtest/TestSuite.H>
#include <fapi2.H>
#include <fapi2_hw_access.H>
#include <errl/errlentry.H>
#include <xscom/piberror.H>
#include <plat_hwp_invoker.H>

//This function does nothing, it is used to call FAPI_INVOKE on
fapi2::ReturnCode empty_function(void)
{
    return fapi2::current_err;
}

fapi2::ReturnCode p9_scomtest_getscom_fail(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint64_t> l_scomdata = 0;

    FAPI_INF("Entering p9_scomtest_getscom_fail...");

    FAPI_INF("Do getscom on proc target");
    FAPI_TRY(fapi2::getScom(i_target,
                            0x11223344,
                            l_scomdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_scomtest_getscom_fail...");

    return fapi2::current_err;

}

fapi2::ReturnCode p9_scomtest_putscom_fail(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint64_t> l_scomdata = 0;

    FAPI_INF("Entering p9_scomtest_putscom_fail...");

    FAPI_INF("Do getscom on proc target");
    FAPI_TRY(fapi2::putScom(i_target,
                            0x22334455,
                            l_scomdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_scomtest_putscom_fail...");

    return fapi2::current_err;

}

fapi2::ReturnCode p9_cfamtest_getcfam_fail(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint32_t> l_cfamdata = 0;

    FAPI_INF("Entering p9_cfamtest_getcfam_fail...");

    FAPI_INF("Do getcfam on proc target");
    FAPI_TRY(fapi2::getCfamRegister(i_target,
                                    0x11223344,
                                    l_cfamdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_cfamtest_getcfam_fail...");

    return fapi2::current_err;

}


fapi2::ReturnCode p9_cfamtest_putcfam_fail(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint32_t> l_cfamdata = 0;

    FAPI_INF("Entering p9_cfamtest_putcfam_fail...");

    FAPI_INF("Do getcfam on proc target");
    FAPI_TRY(fapi2::putCfamRegister(i_target,
                                    0x22334455,
                                    l_cfamdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_cfamtest_putcfam_fail...");

    return fapi2::current_err;

}


fapi2::ReturnCode p9_scomtest_getscom_pass(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint64_t> l_scomdata = 0;

    FAPI_INF("Entering p9_scomtest_getscom_pass...");

    FAPI_INF("Do getscom on proc target");
    FAPI_TRY(fapi2::getScom(i_target,
                            0x02010803, //CXA FIR Mask Register
                            l_scomdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_scomtest_getscom_pass...");

    return fapi2::current_err;

}

fapi2::ReturnCode p9_scomtest_putscom_pass(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint64_t> l_scomdata = 0;

    FAPI_INF("Entering p9_scomtest_putscom_pass...");

    FAPI_INF("Do getscom on proc target");
    FAPI_TRY(fapi2::putScom(i_target,
                            0x02010803, //CXA FIR Mask Register
                            l_scomdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_scomtest_putscom_pass...");

    return fapi2::current_err;

}

fapi2::ReturnCode p9_cfamtest_getcfam_pass(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint32_t> l_cfamdata = 0;

    FAPI_INF("Entering p9_cfamtest_getcfam_pass...");

    FAPI_INF("Do getcfam on proc target");
    FAPI_TRY(fapi2::getCfamRegister(i_target,
                                    0x1000, //DATA_0 from FSI2PIB
                                    l_cfamdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_cfamtest_getcfam_pass...");

    return fapi2::current_err;

}

fapi2::ReturnCode p9_cfamtest_putcfam_pass(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint32_t> l_cfamdata = 0;

    FAPI_INF("Entering p9_cfamtest_putcfam_pass...");

    FAPI_INF("Do getcfam on proc target");
    FAPI_TRY(fapi2::putCfamRegister(i_target,
                                    0x1000, //DATA_0 from FSI2PIB
                                    l_cfamdata));

 fapi_try_exit:

    FAPI_INF("Exiting p9_cfamtest_putcfam_pass...");

    return fapi2::current_err;

}


fapi2::ReturnCode p9_ringtest_getring_fail(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::variable_buffer l_ringdata(64);

    FAPI_INF("Entering p9_ringtest_getring_fail...");

    FAPI_INF("Do getring on proc target");
    FAPI_TRY(fapi2::getRing(i_target,
                            (scanRingId_t)(0x22334455),
                            l_ringdata,
                            fapi2::RING_MODE_HEADER_CHECK));

 fapi_try_exit:

    FAPI_INF("Exiting p9_ringtest_getring_fail...");

    return fapi2::current_err;

}


fapi2::ReturnCode p9_ringtest_modring_fail(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::variable_buffer l_ringdata;
    l_ringdata.resize(861);

    FAPI_INF("Entering p9_ringtest_modring_fail...");

    FAPI_INF("Do modifyRing on proc target");
    FAPI_TRY(fapi2::modifyRing(i_target,
                               (scanRingId_t)0x22334455,
                               l_ringdata,
                               fapi2::CHIP_OP_MODIFY_MODE_OR,
                               fapi2::RING_MODE_HEADER_CHECK));

 fapi_try_exit:

    FAPI_INF("Exiting p9_ringtest_modring_fail...");

    return fapi2::current_err;

}


fapi2::ReturnCode p9_ringtest_getring_pass(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::variable_buffer l_ringdata;
    l_ringdata.resize(861);

    FAPI_INF("Entering p9_ringtest_getring_pass...");

    FAPI_INF("Do getring on proc target");
    FAPI_TRY(fapi2::getRing(i_target,
                            (scanRingId_t)0x00030088,
                            l_ringdata,
                            fapi2::RING_MODE_HEADER_CHECK));

 fapi_try_exit:

    FAPI_INF("Exiting p9_ringtest_getring_pass...");

    return fapi2::current_err;

}


fapi2::ReturnCode p9_ringtest_modring_pass(
               fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    uint32_t bit32_array[861];
    uint32_t length;

    for (length = 0; length < 861; length++)
    {
        bit32_array[length] = length;
    }

    fapi2::variable_buffer l_ringdata(bit32_array, length, 32 * 861);

    FAPI_INF("Entering p9_ringtest_modring_pass...");

    FAPI_INF("Do putring on proc target");
    FAPI_TRY(fapi2::modifyRing(i_target,
                               (scanRingId_t)0x00030088,
                               l_ringdata,
                               fapi2::CHIP_OP_MODIFY_MODE_OR,
                               fapi2::RING_MODE_HEADER_CHECK));

 fapi_try_exit:

    FAPI_INF("Exiting p9_ringtest_modring_pass...");
    return fapi2::current_err;

}

fapi2::ReturnCode p9_platPutRingWRingID_pass()
{
    //every test is displayed this way via FAPI_INF
    FAPI_INF("Entering p9_platPutRingWRingID_pass ...");
    // get the master proc
    TARGETING::Target * l_procTest;
    TARGETING::targetService().masterProcChipTargetHandle( l_procTest);
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>
    l_fapi2CpuTarget((l_procTest));

    fapi2::ReturnCode l_status =
            fapi2::putRing(l_fapi2CpuTarget, ob0_fure,
                    fapi2::RING_MODE_SET_PULSE_NO_OPCG_COND);

    if(l_status!= fapi2::FAPI2_RC_SUCCESS)
    {
        TS_FAIL("p9_platPutRingWRingID_pass>> proc test - failed");
    }

    return l_status;
}


fapi2::ReturnCode p9_opmodetest_getsetopmode()
{
    FAPI_INF("Ensure that getOpMode return NORMAL initially");
    do
    {
        fapi2::OpModes mode = fapi2::getOpMode();
        if(mode != fapi2::NORMAL)
        {
            TS_FAIL("p9_opmodetest_getsetopmode>> Expected fapi2::getOpMode to return fapi2::NORMAL (0x0) but instead returned %x", mode);
            break;
        }

        FAPI_INF("Setting opMode to IGNORE_HW_ERROR (0x1) and checking that we get it back with getOpMode");

        fapi2::setOpMode(fapi2::IGNORE_HW_ERROR);
        mode = fapi2::getOpMode();
        if(mode != fapi2::IGNORE_HW_ERROR)
        {
            TS_FAIL("p9_opmodetest_getsetopmode>> Expected fapi2::getOpMode to return fapi2::IGNORE_HW_ERROR (0x1) but instead returned %x", mode);
            break;
        }

        //Call FAPI_INVOKE on an empty function to test if it resets the opMode
        errlHndl_t l_errl = NULL;
        FAPI_INVOKE_HWP(l_errl,empty_function);

        mode = fapi2::getOpMode();
        if(mode != fapi2::NORMAL)
        {
            TS_FAIL("p9_opmodetest_getsetopmode>> Expected fapi2::getOpMode to return fapi2::NORMAL (0x0) but instead returned %x , FAPI_INVOKE failed to reset opmode", mode);
            break;
        }

    }while(0);
    return fapi2::current_err;
}

fapi2::ReturnCode p9_piberrmask_getsettest()
{
    FAPI_INF("Entering p9_piberrmask_getsettest...");

    FAPI_INF("Ensure that getPIBErrorMask return 0 initially");

    uint8_t mask = fapi2::getPIBErrorMask();
    do
    {
        if(mask != 0)
        {
            TS_FAIL("p9_piberrmask_getsettest>> Expected fapi2::getPIBErrorMask to return (0x0) but instead returned 0x%x", mask);
            break;
        }

        FAPI_INF("Setting pib_err_mask to PIB_CHIPLET_OFFLINE (0x2) and checking that we get it back with getPIBErrorMask");

        fapi2::setPIBErrorMask((uint8_t)PIB::PIB_CHIPLET_OFFLINE);
        mask = fapi2::getPIBErrorMask();
        if(mask != PIB::PIB_CHIPLET_OFFLINE)
        {
            TS_FAIL("p9_piberrmask_getsettest>> Expected fapi2::getPIBErrorMask to return 0x2 but instead returned 0x%x", mask);
            break;
        }

        //Call FAPI_INVOKE on an empty function to test if
        //it resets the pib err mask
        errlHndl_t l_errl = NULL;
        FAPI_INVOKE_HWP(l_errl,empty_function);

        mask = fapi2::getPIBErrorMask();
        if(mask != 0)
        {
            TS_FAIL("p9_piberrmask_getsettest>> Expected fapi2::getPIBErrorMask to return PIB_NO_ERROR (0x0) but instead returned %x , FAPI_INVOKE failed to reset pib_err_mask", mask);
            break;
        }

    FAPI_INF("Exiting p9_piberrmask_getsettest...");
    }while(0);

    return fapi2::current_err;
}

fapi2::ReturnCode p9_opmodetest_ignorehwerr(
                fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
                uint8_t o_fail)
{
    FAPI_INF("Entering p9_opmodetest_ignorehwerr...");
    //Count the number of scoms we do so we can tell that we ran all of them.
    //Putting in this test so we know opMode isnt getting reset on FAPI_TRY
    uint8_t scomCount = 0;
    const uint8_t EXPECTED_NUMBER_OF_SCOMS = 4;

    do{
        FAPI_INF("Setting opMode to IGNORE_HW_ERROR (0x1)");

        fapi2::setOpMode(fapi2::IGNORE_HW_ERROR);
        fapi2::buffer<uint64_t> l_scomdata1 = 0xFF00FF00;
        fapi2::buffer<uint64_t> l_scomdata2 = 0xFF00FF00;

        fapi2::buffer<uint64_t> l_scomresult1 = 0x0;
        fapi2::buffer<uint64_t> l_scomresult2 = 0x0;


        FAPI_INF("Attempting 1st putScom, this should fail but because of opMode we skip the err");
        FAPI_TRY(fapi2::putScom(i_target,
                                0xDEADBEEF,
                                l_scomdata1));
        scomCount++;

        FAPI_INF("Attempting 2nd putScom this should fail but because of opMode we skip the err");
        FAPI_TRY(fapi2::getScom(i_target,
                                0xCABBABEF,
                                l_scomdata2));
        scomCount++;

        FAPI_INF("Attempting 1st getScom, this should fail but because of opMode we skip the err");
        FAPI_TRY(fapi2::getScom(i_target,
                                0xDEADBEEF,
                                l_scomresult1));
        scomCount++;

        FAPI_INF("Attempting 2nd getScom,  this should fail but because of opMode we skip the err");
        FAPI_TRY(fapi2::getScom(i_target,
                                0xCABBABEF,
                                l_scomresult2));
        scomCount++;

    }while(0);

fapi_try_exit:

    if(scomCount != EXPECTED_NUMBER_OF_SCOMS)
    {
        o_fail = 1;
    }
    FAPI_INF("Exiting p9_opmodetest_ignorehwerr...");

    return fapi2::current_err;
}

fapi2::ReturnCode p9_piberrmask_masktest(
                fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{

    FAPI_INF("Entering p9_piberrmask_masktest...");

    uint8_t completionCheck = 0;

    //Ideally we would like this test case to test errors 1-7 but
    // I am not sure if we can simulate some of the errors

    fapi2::buffer<uint64_t> l_scomdata = 0xFF00FF00;

    //Set the mask to ignore invalid address errors ()
    fapi2::setPIBErrorMask(static_cast<uint8_t>(PIB::PIB_INVALID_ADDRESS));

    //Attempt writing to a bad address
    FAPI_TRY(fapi2::putScom(i_target,
                            0xDEADBEEF,
                            l_scomdata));

    //try another scom, this time a get to make sure that
    // FAPI_TRY does not reset the mask
    FAPI_TRY(fapi2::getScom(i_target,
                            0xDEADBEEF,
                            l_scomdata));

    completionCheck = 1;


 fapi_try_exit:

    if(completionCheck != 1)
    {
        FAPI_ERR("Pib Err Mask is not removing errors and is causing FAPI_TRY to fail");
    }
    FAPI_INF("Exiting p9_piberrmask_masktest...");
    return fapi2::current_err;
}
