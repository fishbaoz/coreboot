/* $NoKeywords:$ */
/**
 * @file
 *
 * mmflowd3CZ.c
 *
 * Main Memory initialization sequence for DDR3 only for CZ
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main/CZ)
 * @e \$Revision: 316426 $ @e \$Date: 2015-04-08 14:51:16 +0800 (Wed, 08 Apr 2015) $
 *
 **/
/*****************************************************************************
*
* Copyright 2008 - 2015 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
* AMD is granting you permission to use this software (the Materials)
* pursuant to the terms and conditions of your Software License Agreement
* with AMD.  This header does *NOT* give you permission to use the Materials
* or any rights under AMD's intellectual property.  Your use of any portion
* of these Materials shall constitute your acceptance of those terms and
* conditions.  If you do not agree to the terms and conditions of the Software
* License Agreement, please do not use any portion of these Materials.
*
* CONFIDENTIALITY:  The Materials and all other information, identified as
* confidential and provided to you by AMD shall be kept confidential in
* accordance with the terms and conditions of the Software License Agreement.
*
* LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
* PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
* OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
* IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
* (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
* INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
* GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
* RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
* EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
* THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
*
* AMD does not assume any responsibility for any errors which may appear in
* the Materials or any other related information provided to you by AMD, or
* result from use of the Materials or any related information.
*
* You agree that you will not reverse engineer or decompile the Materials.
*
* NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
* further information, software, technical information, know-how, or show-how
* available to you.  Additionally, AMD retains the right to modify the
* Materials at any time, without notice, and is not obligated to provide such
* modified Materials to you.
*
* U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
* "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
* subject to the restrictions as set forth in FAR 52.227-14 and
* DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
* Government constitutes acknowledgement of AMD's proprietary rights in them.
*
* EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
* direct product thereof will be exported directly or indirectly, into any
* country prohibited by the United States Export Administration Act and the
* regulations thereunder, without the required authorization from the U.S.
* government nor will be used for any purpose prohibited by the same.
* ***************************************************************************
*
*/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */



#include "AGESA.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "mport.h"
#include "mm.h"
#include "mn.h"
#include "mncz.h"
#include "mnD3cz.h"
#include "mt.h"
#include "mt3.h"
#include "mu.h"
#include "mmlvddr3.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mnPmuSramMsgBlockCZ.h"
#include "cpuFamilyTranslation.h"
#include "IdsF15CzAllService.h"
#include "Filecode.h"
#include "GeneralServices.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_MAIN_CZ_MMFLOWD3CZ_FILECODE
/* features */
#include "mftds.h"

extern MEM_FEAT_BLOCK_MAIN MemFeatMain;
extern OPTION_MEM_FEATURE_MAIN MemMS3Save;

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
AGESA_STATUS
MemMFlowD3CZ (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  );
/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function defines the memory initialization flow for
 *      systems that only support DDR3 and CZ processor.
 *
 *     @param[in,out]   *MemMainPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          AGESA_STATUS
 *                          - AGESA_FATAL
 *                          - AGESA_CRITICAL
 *                          - AGESA_SUCCESS
 */
AGESA_STATUS
MemMFlowD3CZ (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  )
{
  MEM_NB_BLOCK  *NBPtr;
  MEM_DATA_STRUCT *MemPtr;

  NBPtr = MemMainPtr->NBPtr;
  MemPtr = MemMainPtr->MemPtr;

  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[BSP_DIE].SocketId, &(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedCZ (&(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid))) {
    MemPtr->IsFlowControlSupported = FALSE;
    return AGESA_FATAL;
  } else {
    MemPtr->IsFlowControlSupported = TRUE;
  }

  MemFInitTableDrive (&NBPtr[BSP_DIE], MTBeforeInitializeMCT);

  // Clear DisDllShutdownSR prior any P-State changes.
  MemNConfigureDisDllShutdownSrCZ (NBPtr);

  //----------------------------------------------------------------
  // Force NB-Pstate to NBP0
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "Force NBPState to NBP0\n");
  MemNChangeNbFrequencyWrapUnb (NBPtr, 0);

  if (NBPtr->MemPtr->ParameterListPtr->DimmTypeDddr3Capable == TRUE) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\nAnalyze DDR3 DIMM SPD Data\n");
    if (MemTDIMMPresence3 (NBPtr[BSP_DIE].TechPtr) && (NBPtr[BSP_DIE].MCTPtr->DimmPresent != 0)) {
      //
      // Setup D3 Platform Specific Pointers here.
      //
      MemNInitNBDataD3CZ (NBPtr);
      return MemMD3FlowCZ (MemMainPtr);
    } else {
      IDS_HDT_CONSOLE (MEM_FLOW, "\nNo DDR3 DIMMs found.\n");
      PutEventLog (AGESA_FATAL, MEM_ERROR_NO_DIMM_FOUND_ON_SYSTEM, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
      return AGESA_FATAL; // DDR3 requested but No DDR3 DIMMs found
    }
  } else {
    return AGESA_FATAL; // UNSUPPORTED DIMMs requested
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function defines the DDR3 initialization flow
 *      when only DDR3 DIMMs are present in the system
 *
 *     @param[in,out]   *MemMainPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          AGESA_STATUS
 *                          - AGESA_FATAL
 *                          - AGESA_CRITICAL
 *                          - AGESA_SUCCESS
 */
AGESA_STATUS
MemMD3FlowCZ (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  )
{
  UINT8            Dct;
  MEM_NB_BLOCK     *NBPtr;
  ID_INFO          CallOutIdInfo;
  INT8             MemPstate;
  UINT8            LowestMemPstate;
  UINT8            PmuImage;
  UINT8            CSTestFail;
  BOOLEAN          ErrorRecovery;
  BOOLEAN          IgnoreErr;
  UINT8            i;
  UINT8            TimesMrlRetrain;
  BOOLEAN          PmuResult;

  NBPtr = &MemMainPtr->NBPtr[BSP_DIE];
  ErrorRecovery = TRUE;
  IgnoreErr = FALSE;
  CSTestFail = 0;

  IDS_HDT_CONSOLE (MEM_FLOW, "DDR3 Mode\n");

  IDS_HDT_CONSOLE (MEM_FLOW, "Determine VDDIO\n");

  //----------------------------------------------------------------
  // Clock and power gate unused channels
  //----------------------------------------------------------------
  MemNClockAndPowerGateUnusedDctCZ (NBPtr);

  //----------------------------------------------------------------
  // Set DDR3 mode
  //----------------------------------------------------------------
  MemNSetDdrModeD3CZ (NBPtr);

  //----------------------------------------------------------------
  // Low voltage DDR3
  //----------------------------------------------------------------
  // Levelize DDR3 voltage based on socket, as each socket has its own voltage for dimms.
  AGESA_TESTPOINT (TpProcMemLvDdr3, &(MemMainPtr->MemPtr->StdHeader));
  if (!MemFeatMain.LvDDR3 (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // Find the maximum speed that all DCTs are capable running at
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "Determine Target Speed\n");
  if (!MemTSPDGetTargetSpeed3 (NBPtr->TechPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // Adjust memClkFreq based on MaxDdrRate
  //----------------------------------------------------------------
  MemNAdjustDdrSpeedUnb (NBPtr);

  //------------------------------------------------
  // Finalize target frequency
  //------------------------------------------------
  if (!MemMLvDdr3PerformanceEnhFinalize (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  //  Program DCT address map
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nConfigure DCTs and Set Addr Map\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctDimmValid == 0) {
      MemNDisableDctCZ (NBPtr);
    } else {
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMap Chip selects\n");
      if (MemTSPDSetBanks3 (NBPtr->TechPtr)) {
        if (MemNStitchMemoryNb (NBPtr)) {
          if (NBPtr->DCTPtr->Timings.CsEnabled == 0) {
            MemNDisableDctCZ (NBPtr);
          } else {
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tAuto Config\n");
            MemNAutoConfigCZ (NBPtr);
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tSet Training Config\n");
            MemNConfigureDctForTrainingD3CZ (NBPtr);
          }
        }
      }
    }
  }

  IDS_OPTION_HOOK (IDS_BEFORE_DRAM_INIT, NBPtr, &(MemMainPtr->MemPtr->StdHeader));
  //----------------------------------------------------------------
  //  Init Phy mode
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nInit Phy Mode\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);

      // 1. Program D18F2x9C_x0002_0099_dct[1:0][PmuReset,PmuStall] = 1,1.
      // 2. Program D18F2x9C_x0002_000E_dct[1:0][PhyDisable]=0. Tester_mode=0.
      MemNPmuResetNb (NBPtr);

      // 3. According to the type of DRAM attached, program the following phy mode
      MemNSetPhyDdrModeCZ (NBPtr, DRAM_TYPE_DDR3_CZ);
    }
  }

  //----------------------------------------------------------------
  //  Program Mem Pstate dependent registers
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nProgram Mem PState Dependent Registers\n");
  // Only configure M0 Context when in emulation mode.
  LowestMemPstate = 0;
  IEM_SKIP_CODE (IEM_EARLY_DCT_CONFIG) {
    // PMU Requires that both Mem PState Contexts are be configured.
    LowestMemPstate = 1;
  }

  for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
    // When memory pstate is enabled, this loop will goes through M1 first then M0
    // Otherwise, this loop only goes through M0.
    IDS_HDT_CONSOLE (MEM_FLOW, "\n");
    MemNSwitchMemPstateCZ (NBPtr, MemPstate);

    // By default, start up speed is DDR667 for M1
    // For M0, we need to set speed to highest possible frequency
    if (MemPstate == 0) {
      for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
        MemNSwitchDCTNb (NBPtr, Dct);
        NBPtr->DCTPtr->Timings.Speed = NBPtr->DCTPtr->Timings.TargetSpeed;
      }
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "\tMemClkFreq = %d MHz\n", NBPtr->DCTPtr->Timings.Speed);

    // Program SPD timings and frequency dependent settings
    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb (NBPtr, Dct);

      if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
        IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);

        IDS_HDT_CONSOLE (MEM_FLOW, "\t\tProgram SPD Timings\n");
        if (MemTAutoCycTiming3 (NBPtr->TechPtr)) {
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\tSet MemClk Frequency for MPState %d\n", MemPstate);
          MemNProgramMemPstateRegD3CZ (NBPtr, MemPstate);
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\tProgram Platform Specific Values\n");
          if (MemNPlatformSpecCZ (NBPtr)) {
            MemNSetBitFieldNb (NBPtr, BFMemClkDis, 0);
            // 7. Program default CAD bus values.
            // 8. Program default data bus values.
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tCAD Data Bus Config\n");
            MemNProgramCadDataBusD3CZ (NBPtr);

            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPredriver Init\n");
            MemNPredriverInitD3CZ (NBPtr);

            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMode Register Initialization\n");
            MemNModeRegisterInitializationD3CZ (NBPtr);

            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tDRAM PHY Power Savings\n");
            MemNDramPhyPowerSavingsCZ (NBPtr, DRAM_TYPE_DDR3_CZ);
          }
        }
      }
    }

    MemFInitTableDrive (NBPtr, MTBeforeDInit);
  }


  //----------------------------------------------------------------
  //  Program Phy
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE( MEM_FLOW, "\nProgram DDR Phy\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);

      // 4. Program general phy static configuration
      MemNPhyGenCfgCZ (NBPtr);

      // 5. Phy Voltage Level Programming
      MemNPhyVoltageLevelCZ (NBPtr, DRAM_TYPE_DDR3_CZ);

      // 7. Program DRAM channel frequency
      MemNProgramChannelFreqCZ (NBPtr, DRAM_TYPE_DDR3_CZ);

      // Step 8 and 9 are done in MemPs dependent section

      // 10. Program FIFO pointer init values
      MemNPhyFifoConfigD3CZ (NBPtr);
    }
  }

  MemFInitTableDrive (NBPtr, MTBeforeTrn);

  IEM_INSERT_CODE (IEM_EARLY_DEVICE_INIT, IemEarlyDeviceInitD3CZ, (NBPtr));

  //------------------------------------------------
  // Callout before Dram Init
  //------------------------------------------------
  AGESA_TESTPOINT (TpProcMemBeforeAgesaHookBeforeDramInit, &(MemMainPtr->MemPtr->StdHeader));
  CallOutIdInfo.IdField.SocketId = NBPtr->MCTPtr->SocketId;
  CallOutIdInfo.IdField.ModuleId = NBPtr->MCTPtr->DieId;
  //------------------------------------------------------------------------
  // Callout to Platform BIOS to set the VDDP/VDDR voltage
  //------------------------------------------------------------------------
  MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.IsValid = TRUE;
  MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.Voltage = VOLT1_05;
  IDS_HDT_CONSOLE (MEM_FLOW, "\nVDDP = %s\n", (MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.Voltage == VOLT1_05) ? "1.05V":
                                              ((MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.Voltage == VOLT0_95) ? "0.95V" : "Unsupported"));

  IDS_HDT_CONSOLE (MEM_FLOW, "\nCalling out to Platform BIOS on Socket %d, Module %d...\n", CallOutIdInfo.IdField.SocketId, CallOutIdInfo.IdField.ModuleId);
  AgesaHookBeforeDramInit ((UINTN) CallOutIdInfo.IdInformation, MemMainPtr->MemPtr);
  IDS_HDT_CONSOLE (MEM_FLOW, "\nVDDIO = %s\n", GET_VDDIO_STRING(NBPtr->RefPtr->DDRVoltage, DDR3_TECHNOLOGY));
  AGESA_TESTPOINT (TpProcMemAfterAgesaHookBeforeDramInit, &(NBPtr->MemPtr->StdHeader));
  //
  // Check for valid VDDR/VDDP Value after callout
  //
  if (MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.Voltage != VOLT1_05) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\nFATAL ERROR: Unsupported VDDP/VDDR Value indicated by platform BIOS.\n");
    PutEventLog (AGESA_FATAL, MEM_ERROR_VDDPVDDR_UNSUPPORTED, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // Enable PHY Calibration
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nEnable Phy Calibration\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctDimmValid != 0) {
      MemNEnablePhyCalibrationCZ (NBPtr);
    }
  }

  //----------------------------------------------------------------------------
  // Deassert MemResetL
  //----------------------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nDeassert MemResetL\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      // Deassert Procedure:
      //   MemResetL = 0
      //   Go to LP2
      //   Go to PS0
      MemNSetBitFieldNb (NBPtr, BFMemResetL, 0);
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 0);
    }
  }

  IEM_SKIP_CODE (IEM_EARLY_DEVICE_INIT) {
    MemUWait10ns (20000, NBPtr->MemPtr);
  }
  IDS_OPTION_HOOK (IDS_BEFORE_DQS_TRAINING, MemMainPtr, &(MemMainPtr->MemPtr->StdHeader));

  //----------------------------------------------------------------------------
  //  Program PMU SRAM Message Block, Initiate PMU based Dram init and training
  //----------------------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nBegin PMU Based DRAM Init and Training\n");
  //
  // Allocate Message Block on the heap
  //
  IDS_HDT_CONSOLE (MEM_FLOW, "\tAllocate the PMU SRAM Message Block buffer\n");
  if (MemNInitPmuSramMsgBlockCZ (NBPtr) == FALSE) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\tNot able to allocate the PMU SRAM Message Block buffer\n");
    // Not able to initialize the PMU SRAM Message Block buffer.  Log an event.
    PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_PMU_SRAM_MSG_BLOCK, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
    return AGESA_FATAL;
  }

  for (PmuImage = 0; PmuImage < NBPtr->PmuFirmwareImageTableSize; PmuImage++) {
    //
    // Load PMU Firmware Image
    //
    NBPtr->PmuFirmwareImage = PmuImage;
    //
    // Check PMU Firmware Image
    //
    if (MemNCheckPmuFirmwareImageCZ (NBPtr)) {
      NBPtr->FeatPtr->LoadPmuFirmware (NBPtr);
      for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
        MemNSwitchDCTNb  (NBPtr, Dct);
        if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
          IDS_HDT_CONSOLE (MEM_STATUS, "\n\tDct %d\n", Dct);
          for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
            // When memory pstate is enabled, this loop will goes through M1 first then M0
            // Otherwise, this loop only goes through M0.
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\t");
            MemNSwitchMemPstateCZ (NBPtr, MemPstate);

            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tStore MPState Dependent Memory Timings in PMU Msg Block\n");
            MemNPopulatePmuSramTimingsD3CZ (NBPtr);
          }

          MemNPopulatePmuSramConfigD3CZ (NBPtr);
          MemNSetPmuSequenceControlD3CZ (NBPtr);
          IEM_INSERT_CODE (IEM_BEFORE_PMU_MSG_BLOCK_WRITE, IemBeforePmuSramMsgBlockWriteCZ, (NBPtr));
          if (MemNWritePmuSramMsgBlockCZ (NBPtr) == FALSE) {
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tNot able to load the PMU SRAM Message Block in to DMEM\n");
            // Not able to load the PMU SRAM Message Block in to DMEM.  Log an event.
            PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_LOCATE_FOR_PMU_SRAM_MSG_BLOCK, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
            return AGESA_FATAL;
          }

          // Query for the calibrate completion.
          MemNPendOnPhyCalibrateCompletionCZ (NBPtr);

          MemNStartPmuNb (NBPtr);
        }
      }

      IDS_HDT_CONSOLE (MEM_STATUS, "\n\tWait for PMU Complete on all channels\n");
      for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
        MemNSwitchDCTNb  (NBPtr, Dct);
        if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
          IDS_HDT_CONSOLE (MEM_STATUS, "\t\tDct %d\n", Dct);
          //
          // Wait for PMU Completion
          //
          PmuResult = MemNPendOnPmuCompletionNb (NBPtr);
          //
          // Update PMU Message block from SRAM
          //
          MemNReadPmuSramMsgBlockCZ (NBPtr);
          if (PmuResult == FALSE) {
            //
            // PMU Training Failure
            //
            AGESA_TESTPOINT (TpProcMemPmuFailed, &(MemMainPtr->MemPtr->StdHeader));
            CSTestFail = ((PMU_SRAM_MSG_BLOCK_CZ*)NBPtr->PsPtr->PmuSramMsgBlockPtr)->CsTestFail;
            NBPtr->MCTPtr->ChannelTrainFail |= (UINT32)1 << Dct;
            IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\tPMU Training Failure. CSTestFail[CS7:CS0] = %x\n", CSTestFail);
            PutEventLog (AGESA_FATAL, MEM_ERROR_PMU_TRAINING, 0, 0, Dct, CSTestFail, &(MemMainPtr->MemPtr->StdHeader));
            if (!NBPtr->MemPtr->ErrorHandling (NBPtr->MCTPtr, Dct, CSTestFail, &(MemMainPtr->MemPtr->StdHeader))) {
              ASSERT (FALSE);
            }
          } // MemNPendOnPmuCompletionNb
          IDS_HDT_CONSOLE (MEM_STATUS, "\t\tPMU Finished\n");
          // Set calibration rate.
          // 2.10.10.2.8.3 Auto Calibration
          MemNRateOfPhyCalibrateCZ (NBPtr);
        } // Dct Memsize
      } // Wait all channels
    } // Check PMU Firmware Image
  }
  IDS_HDT_CONSOLE (MEM_FLOW, "\nPMU Based Training complete\n");

  //----------------------------------------------------------------
  // Disable chipselects that failed training
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nDIMM Excludes\n");
  MemNDimmExcludesCZ (NBPtr);

  IEM_INSERT_CODE (IEM_LATE_DCT_CONFIG, IemLateDctConfigD3CZ, (NBPtr));

  //----------------------------------------------------------------
  //  Synchronize Channels
  //----------------------------------------------------------------
  MemNSyncChannelInitCZ (NBPtr);

  //----------------------------------------------------------------
  //  Train MaxRdLatency
  //----------------------------------------------------------------
  IEM_SKIP_CODE (IEM_LATE_MAX_RD_LAT) {
    NBPtr->TechPtr->FindMaxDlyForMaxRdLat = MemTFindMaxRcvrEnDlyTrainedByPmuByte;
    NBPtr->TechPtr->ResetDCTWrPtr = MemNResetRcvFifoCZ;
    IDS_HDT_CONSOLE (MEM_FLOW, "\nTrain MaxRdLatency at each NBPState\n");
    TimesMrlRetrain = DEFAULT_MRL_RETRAIN_TIMES;
    IDS_OPTION_HOOK (IDS_MEM_MRL_RETRAIN_TIMES, &TimesMrlRetrain, &(MemMainPtr->MemPtr->StdHeader));
    for (i = 0; i < TimesMrlRetrain; i ++) {
      if (i > 0) {
        // Force NB P-state 0 for multiple rounds of MRL training
        NBPtr->NbFreqChgState = 0;
        MemNChangeNbFrequencyWrapUnb (NBPtr, 0);
      }
      MemTTrainMaxLatencyCZ (NBPtr->TechPtr);
      // The fourth loop will restore the Northbridge P-State control register
      // to the original value.
      for (NBPtr->NbFreqChgState = 1; NBPtr->NbFreqChgState <= 4; NBPtr->NbFreqChgState++) {
        if (!MemNChangeNbFrequencyWrapUnb (NBPtr, NBPtr->NbFreqChgState) || (NBPtr->NbFreqChgState == 4)) {
          break;
        }
        MemTTrainMaxLatencyCZ (NBPtr->TechPtr);
      }
    }
  }

  //----------------------------------------------------------------
  //  Configure DCT for normal operation
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nMission Mode Config\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);

      MemNConfigureDctNormalD3CZ (NBPtr);

      //----------------------------------------------------------------
      //  Program turnaround timings
      //----------------------------------------------------------------
      for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
        IDS_HDT_CONSOLE (MEM_FLOW, "\t");
        MemNSwitchMemPstateCZ (NBPtr, MemPstate);
        MemNProgramTurnaroundTimingsD3CZ (NBPtr);

        //----------------------------------------------------------------
        // After Mem Pstate1 Partial Training Table values
        //----------------------------------------------------------------
        MemFInitTableDrive (NBPtr, MTAfterMemPstate1PartialTrn);
      }
    }
  }

  IEM_INSERT_CODE (IEM_LATE_MAX_RD_LAT, IemLateMaxRdLatD3CZ, (NBPtr));

  IDS_HDT_CONSOLE (MEM_FLOW, "\nAdditional DRAM PHY Power Savings\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      MemNAddlDramPhyPowerSavingsCZ (NBPtr);
    }
  }

  //----------------------------------------------------------------
  //  Initialize Channels interleave address bit.
  //----------------------------------------------------------------
  MemNInitChannelIntlvAddressBitCZ (NBPtr);

  //----------------------------------------------------------------
  // Assign physical address ranges for DCTs and node. Also, enable channel interleaving.
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "HT Mem Map\n");
  if (!NBPtr->FeatPtr->InterleaveChannels (NBPtr)) {
    MemNHtMemMapCZ (NBPtr);
  }

  //----------------------------------------------------
  // If there is no dimm on the system, do fatal exit
  //----------------------------------------------------
  if (NBPtr->RefPtr->SysLimit == 0) {
    PutEventLog (AGESA_FATAL, MEM_ERROR_NO_DIMM_FOUND_ON_SYSTEM, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // CpuMemTyping
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "Mem Typing\n");
  MemNCPUMemTypingNb (NBPtr);

  IDS_OPTION_HOOK (IDS_AFTER_DQS_TRAINING, MemMainPtr, &(MemMainPtr->MemPtr->StdHeader));
  //----------------------------------------------------------------
  // After Training Table values
  //----------------------------------------------------------------
  MemFInitTableDrive (NBPtr, MTAfterTrn);

  //----------------------------------------------------------------
  // Interleave banks
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "Bank Interleave\n");
  if (NBPtr->FeatPtr->InterleaveBanks (NBPtr)) {
    if (NBPtr->MCTPtr->ErrCode == AGESA_FATAL) {
      return AGESA_FATAL;
    }
  }

  //----------------------------------------------------------------
  // After Programming Interleave registers
  //----------------------------------------------------------------
  MemFInitTableDrive (NBPtr, MTAfterInterleave);

  //----------------------------------------------------------------
  // Memory Clear
  //----------------------------------------------------------------
  AGESA_TESTPOINT (TpProcMemMemClr, &(MemMainPtr->MemPtr->StdHeader));
  if (!MemFeatMain.MemClr (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // ECC
  //----------------------------------------------------------------
  if (!MemFeatMain.InitEcc (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // C6 and ACP Engine Storage Allocation
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "Allocate C6 and ACP Engine Storage\n");
  MemNAllocateC6AndAcpEngineStorageCZ (NBPtr);

  //----------------------------------------------------------------
  // UMA Allocation & UMAMemTyping
  //----------------------------------------------------------------
  AGESA_TESTPOINT (TpProcMemUMAMemTyping, &(MemMainPtr->MemPtr->StdHeader));
  IDS_HDT_CONSOLE (MEM_FLOW, "UMA Allocation\n");
  if (!MemFeatMain.UmaAllocation (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // OnDimm Thermal
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "OnDIMM Thermal\n");
  if (NBPtr->FeatPtr->OnDimmThermal (NBPtr)) {
    if (NBPtr->MCTPtr->ErrCode == AGESA_FATAL) {
      return AGESA_FATAL;
    }
  }

  //----------------------------------------------------------------
  // Finalize MCT
  //----------------------------------------------------------------
  MemNFinalizeMctCZ (NBPtr);
  MemFInitTableDrive (NBPtr, MTAfterFinalizeMCT);

  //----------------------------------------------------------------
  // Memory Context Save
  //----------------------------------------------------------------
  MemFeatMain.MemSave (MemMainPtr);

  //----------------------------------------------------------------
  // Memory DMI support
  //----------------------------------------------------------------
  if (!MemFeatMain.MemDmi (MemMainPtr)) {
    return AGESA_CRITICAL;
  }

  //----------------------------------------------------------------
  // Memory CRAT support
  //----------------------------------------------------------------
  if (!MemFeatMain.MemCrat (MemMainPtr)) {
    return AGESA_CRITICAL;
  }

  //----------------------------------------------------------------
  // Save memory S3 data
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "S3 Save\n");
  if (!MemMS3Save (MemMainPtr)) {
    return AGESA_CRITICAL;
  }

  //----------------------------------------------------------------
  //  De-allocate the PMU SRAM Message Block buffer.
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\nDe-allocate PMU SRAM Message Block buffer\n");
  if (MemNPostPmuSramMsgBlockCZ (NBPtr) == FALSE) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\tNot able to free the PMU SRAM Message Block buffer\n");
    // Not able to free the PMU SRAM Message Block buffer.  Log an event.
    PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_DEALLOCATE_FOR_PMU_SRAM_MSG_BLOCK, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // Switch back to DCT 0 before sending control back
  //----------------------------------------------------------------
  MemNSwitchDCTNb (NBPtr, 0);

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
