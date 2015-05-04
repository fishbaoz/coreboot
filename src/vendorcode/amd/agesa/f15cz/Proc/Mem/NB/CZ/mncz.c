/* $NoKeywords:$ */
/**
 * @file
 *
 * mncz.c
 *
 * Common Northbridge  functions for CZ
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/CZ)
 * @e \$Revision: 309090 $ @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mp.h"
#include "mncz.h"
#include "mu.h"
#include "S3.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "heapManager.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_NB_CZ_MNCZ_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define MEM_PSC_FLOW_DEFTRUE (BOOLEAN (*) (MEM_NB_BLOCK*, MEM_PSC_TABLE_BLOCK *)) memDefTrue

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG UserOptions;
extern PSO_ENTRY DefaultPlatformMemoryConfiguration[];
extern MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[];
extern MEM_PSC_FLOW MemPGetMaxFreqSupported;

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the northbridge block
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *     @param[in]        *FeatPtr  - Pointer to the MEM_FEAT_BLOCK_NB
 *     @param[in]        *SharedPtr - Pointer to the MEM_SHARED_DATA
 *     @param[in]        NodeID  - UINT8 indicating node ID of the NB object.
 *
 *     @return     Boolean indicating that this is the correct memory
 *                 controller type for the node number that was passed in.
 */

BOOLEAN
MemConstructNBBlockCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       MEM_FEAT_BLOCK_NB *FeatPtr,
  IN       MEM_SHARED_DATA *SharedPtr,
  IN       UINT8 NodeID
  )
{
  UINT8 Dct;
  UINT8 Channel;
  UINT8 SpdSocketIndex;
  UINT8 SpdChannelIndex;
  DIE_STRUCT *MCTPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;

  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem->SocketId, &(MemPtr->DiesPerSystem->LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedCZ (&(MemPtr->DiesPerSystem->LogicalCpuid))) {
    return FALSE;
  }

  NBPtr->MemPtr = MemPtr;
  NBPtr->RefPtr = MemPtr->ParameterListPtr;
  NBPtr->SharedPtr = SharedPtr;

  MCTPtr = MemPtr->DiesPerSystem;
  NBPtr->MCTPtr = MCTPtr;
  NBPtr->MCTPtr->NodeId = NodeID;
  NBPtr->PciAddr.AddressValue = MCTPtr->PciAddr.AddressValue;
  NBPtr->VarMtrrHiMsk = GetVarMtrrHiMsk (&(MemPtr->DiesPerSystem->LogicalCpuid), &(MemPtr->StdHeader));

  //
  // Allocate buffer for DCT_STRUCTs and CH_DEF_STRUCTs
  //
  AllocHeapParams.RequestedBufferSize = MAX_DCTS_PER_NODE_CZ * (
                                          sizeof (DCT_STRUCT) + (
                                            MAX_CHANNELS_PER_DCT_CZ * (sizeof (CH_DEF_STRUCT) + sizeof (MEM_PS_BLOCK) + sizeof (CH_TIMING_STRUCT))
                                          )
                                        );
  AllocHeapParams.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_DCT_STRUCT_HANDLE, NodeID, 0, 0);
  AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
  if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) != AGESA_SUCCESS) {
    PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_DCT_STRUCT_AND_CH_DEF_STRUCTs, NBPtr->Node, 0, 0, 0, &MemPtr->StdHeader);
    SetMemError (AGESA_FATAL, MCTPtr);
    ASSERT(FALSE); // Could not allocate buffer for DCT_STRUCTs and CH_DEF_STRUCTs
    return FALSE;
  }

  MCTPtr->DctCount = MAX_DCTS_PER_NODE_CZ;
  MCTPtr->DctData = (DCT_STRUCT *) AllocHeapParams.BufferPtr;
  AllocHeapParams.BufferPtr += MAX_DCTS_PER_NODE_CZ * sizeof (DCT_STRUCT);
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_CZ; Dct++) {
    MCTPtr->DctData[Dct].Dct = Dct;
    MCTPtr->DctData[Dct].ChannelCount = MAX_CHANNELS_PER_DCT_CZ;
    MCTPtr->DctData[Dct].ChData = (CH_DEF_STRUCT *) AllocHeapParams.BufferPtr;
    MCTPtr->DctData[Dct].ChData[0].Dct = Dct;
    AllocHeapParams.BufferPtr += MAX_CHANNELS_PER_DCT_CZ * sizeof (CH_DEF_STRUCT);
    MCTPtr->DctData[Dct].TimingsMemPs1 = (CH_TIMING_STRUCT *) AllocHeapParams.BufferPtr;
    AllocHeapParams.BufferPtr += MAX_CHANNELS_PER_DCT_CZ * sizeof (CH_TIMING_STRUCT);
  }
  NBPtr->PSBlock = (MEM_PS_BLOCK *) AllocHeapParams.BufferPtr;

  //
  // Initialize Socket List
  //
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_CZ; Dct++) {
    MemPtr->SocketList[MCTPtr->SocketId].ChannelPtr[(MCTPtr->DieId * 2) + Dct] = &(MCTPtr->DctData[Dct].ChData[0]);
    MemPtr->SocketList[MCTPtr->SocketId].TimingsPtr[(MCTPtr->DieId * 2) + Dct] = &(MCTPtr->DctData[Dct].Timings);
    MCTPtr->DctData[Dct].ChData[0].ChannelID = (MCTPtr->DieId * 2) + Dct;
  }

  MemNInitNBDataCZ (NBPtr);

  FeatPtr->InitEarlySampleSupport (NBPtr);

  NBPtr->FeatPtr = FeatPtr;

  //
  // Calculate SPD Offsets per channel and assign pointers to the data.  At this point, we calculate the Node-Dct-Channel
  // centric offsets and store the pointers to the first DIMM of each channel in the Channel Definition struct for that
  // channel.  This pointer is then used later to calculate the offsets to be used for each logical dimm once the
  // dimm types(QR or not) are known. This is done in the Technology block constructor.
  //
  // Calculate the SpdSocketIndex separately from the SpdChannelIndex.
  // This will facilitate modifications due to some processors that might
  // map the DCT-CHANNEL differently.
  //
  SpdSocketIndex = GetSpdSocketIndex (NBPtr->RefPtr->PlatformMemoryConfiguration, NBPtr->MCTPtr->SocketId, &MemPtr->StdHeader);
  //
  // Traverse the Dct/Channel structures
  //
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_CZ; Dct++) {
    for (Channel = 0; Channel < MAX_CHANNELS_PER_DCT_CZ; Channel++) {

      NBPtr->MCTPtr->DctData[Dct].ChData[Channel].ColumnCount = MAX_DELAYS;

      //
      // Calculate the number of Dimms on this channel using the
      //   die/dct/channel to Socket/channel conversion.
      //
      SpdChannelIndex = GetSpdChannelIndex (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                            NBPtr->MCTPtr->SocketId,
                                            MemNGetSocketRelativeChannelNb (NBPtr, Dct & 1, Channel),
                                            &MemPtr->StdHeader);
      NBPtr->MCTPtr->DctData[Dct].ChData[Channel].SpdPtr = &(MemPtr->SpdDataStructure[SpdSocketIndex + SpdChannelIndex]);
    }
  }

  //
  // Initialize Dct and DctCfgSel bit
  //
  MemNSetBitFieldNb (NBPtr, BFDctCfgSel, 0);
  MemNSwitchDCTNb (NBPtr, 0);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes member functions and variables of NB block.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNInitNBDataCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  NBPtr->DctCachePtr = NBPtr->DctCache;
  NBPtr->PsPtr = NBPtr->PSBlock;

  MemNInitNBRegTableCZ (NBPtr, NBPtr->NBRegTable);
  NBPtr->Node = ((UINT8) NBPtr->PciAddr.Address.Device) - 24;
  NBPtr->Dct = 0;
  NBPtr->Channel = 0;
  NBPtr->DctCount = MAX_DCTS_PER_NODE_CZ;
  NBPtr->ChannelCount = MAX_CHANNELS_PER_DCT_CZ;
  NBPtr->NodeCount = MAX_NODES_SUPPORTED_CZ;

  NBPtr->MemCleared = FALSE;
  NBPtr->StartupSpeed = DDR667_FREQUENCY;
  NBPtr->DefDctSelIntLvAddr = 4;
  NBPtr->NbFreqChgState = 0;
  NBPtr->NbPsCtlReg = 0;
  NBPtr->CsRegMsk = 0x7FF8FFE0;
  NBPtr->MemPstate = MEMORY_PSTATE0;
  NBPtr->MemPstateStage = MEMORY_PSTATE_1ST_STAGE;
  NBPtr->CsPerChannel = MAX_CS_PER_CHANNEL_CZ;

  //PerRankTimingEn is always set to 0 which force CsPerDelay = 2.
  NBPtr->CsPerDelay = 2;

  // Legacy settings, can probably be removed
  NBPtr->Ganged = FALSE;
  NBPtr->PosTrnPattern = POS_PATTERN_256B;
  NBPtr->RdDqsDlyRetrnStat = RDDQSDLY_RTN_NEEDED;
  NBPtr->RcvrEnDlyLimit = 0x1FF;
  NBPtr->FreqChangeParam = NULL;
  NBPtr->MaxRxEnSeedTotal = 0x1FF;
  NBPtr->MinRxEnSeedGross = 0;

  LibAmdMemFill (NBPtr->DctCache, 0, sizeof (NBPtr->DctCache), &NBPtr->MemPtr->StdHeader);
  MemNInitNBDataNb (NBPtr);

  // ---------------------------------------------------------------------------
  // Common functions
  // ---------------------------------------------------------------------------
  NBPtr->MemNCmnGetSetFieldNb = MemNCmnGetSetFieldCZ;
  NBPtr->SetEccSymbolSize = MemNSetEccSymbolSizeNb;
  NBPtr->PollBitField = MemNPollBitFieldNb;
  NBPtr->BrdcstCheck = MemNBrdcstCheckNb;
  NBPtr->BrdcstSet = MemNBrdcstSetNb;
  NBPtr->GetTrainDly = MemNGetTrainDlyNb;
  NBPtr->SetTrainDly = MemNSetTrainDlyNb;
  NBPtr->MemNcmnGetSetTrainDly = MemNcmnGetSetTrainDlyCZ;
  NBPtr->StitchMemory = MemNStitchMemoryNb;
  NBPtr->DisableDCT = MemNDisableDctCZ;
  NBPtr->HtMemMapInit = MemNHtMemMapCZ;
  NBPtr->SyncAddrMapToAllNodes = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->CpuMemTyping = MemNCPUMemTypingNb;

  // ---------------------------------------------------------------------------
  // Misc functions
  // ---------------------------------------------------------------------------
  NBPtr->GetSocketRelativeChannel = MemNGetSocketRelativeChannelNb;
  NBPtr->MemNPlatformSpecificFormFactorInitNb = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefRet;

  NBPtr->SyncTargetSpeed = MemNSyncTargetSpeedNb;
  NBPtr->MemNCapSpeedBatteryLife = MemNCapSpeedBatteryLifeCZ;
  NBPtr->ProgramCycTimings = MemNProgramCycTimingsCZ;

  NBPtr->PlatformSpec = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefRet;

  NBPtr->GetMemClkFreqInCurrentContext = MemNGetMemClkFreqInCurrentContextCZ;
  NBPtr->ProgramNbPsDependentRegs = MemNProgramNbPstateDependentRegistersCZ;

  NBPtr->GetUmaSize = MemNGetUmaSizeCZ;
  NBPtr->UMAMemTyping = MemNUMAMemTypingNb;

  NBPtr->MemNDisableScrubber = MemNDisableScrubberCZ;
  NBPtr->MemNRestoreScrubber = MemNRestoreScrubberCZ;
  NBPtr->MemNSetEccExclusionRange = MemNSetEccExclusionRangeUnb;
  NBPtr->MemNDisableMemClk = MemNDisableMemClkUnb;
  NBPtr->MemNTristateUnusedChipSelects = MemNTristateUnusedChipSelectsNb;
  NBPtr->MemS3PspDetect = MemS3PspDetectionCZ;
  NBPtr->MemS3PspPlatformSecureBootEn = MemS3PspPlatformSecureBootEnCZ;
  NBPtr->MemRunningOnPsp = MemRunningOnPspCZ;

  // ---------------------------------------------------------------------------
  // Training functions
  // ---------------------------------------------------------------------------
  NBPtr->WritePattern = MemNContWritePatternUnb;
  NBPtr->ReadPattern = MemNContReadPatternUnb;
  NBPtr->FlushPattern = (VOID (*) (MEM_NB_BLOCK *, UINT32, UINT16)) memDefRet;
  NBPtr->TrainingPatternInit = (AGESA_STATUS (*) (MEM_NB_BLOCK *)) memDefRetSuccess;
  NBPtr->TrainingPatternFinalize = (AGESA_STATUS (*) (MEM_NB_BLOCK *)) memDefRetSuccess;
  NBPtr->CompareTestPattern = MemNCompareTestPatternUnb;
  NBPtr->GetSysAddr = (BOOLEAN (*) (MEM_NB_BLOCK *, UINT8, UINT32 *)) memDefTrue;
  NBPtr->getMaxLatParams = MemNGetMaxLatParamsCZ;
  NBPtr->WaitXMemClks = MemNWaitXMemClksNb;

  NBPtr->MemNGetMemoryWidth = MemNGetMemoryWidthUnb;

  // ---------------------------------------------------------------------------
  // Family specific flags
  // ---------------------------------------------------------------------------
  NBPtr->IsSupported[G5DimmInD3Socket] = TRUE;
  NBPtr->IsSupported[CheckFindPSOverideWithSocket] = TRUE;

  // ---------------------------------------------------------------------------
  // Family specific hooks
  // ---------------------------------------------------------------------------
  NBPtr->FamilySpecificHook[DCTSelectSwitch] = MemNDctCfgSelectCZ;
  NBPtr->FamilySpecificHook[SetupHwTrainingEngine] = MemNSetupHwTrainingEngineUnb;
  NBPtr->FamilySpecificHook[ReleaseNbPstate] = MemNReleaseNbPstateCZ;
  NBPtr->FamilySpecificHook[FixupUmaInfo] = MemNFixupUmaInfoCZ;
  NBPtr->FamilySpecificHook[AdjustCSIntLvLowAddr] = MemNCSIntLvLowAddrAdjCZ;
  NBPtr->FamilySpecificHook[ForceEccSymbolSize] = MemNForceEccSymbolSizeCZ;
  NBPtr->FamilySpecificHook[DisableMemHoleMapping] = MemNDisableMemHoleMappingCZ;
  NBPtr->FamilySpecificHook[RestoreMemHoleMapping] = MemNRestoreMemHoleMappingCZ;
  NBPtr->FamilySpecificHook[DramScrubErratum792] = MemNDramScrubErratum792CZ;
  NBPtr->FamilySpecificHook[GetDdrMaxRate] = MemNGetDdrMaxRateCZ;
  // ---------------------------------------------------------------------------
  // Initialize PS block's functions
  // ---------------------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    NBPtr->PSBlock[Dct].MemPGetPORFreqLimit = MemNGetPORFreqLimitTblDrvNb;
    NBPtr->PSBlock[Dct].MemPDoPs = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefRet;

    MemNSwitchDCTNb (NBPtr, Dct);
    NBPtr->DCTPtr->Timings.MaxRdLat0 = 0x3FF;
    NBPtr->DCTPtr->Timings.MaxRdLat1 = 0x3FF;
    NBPtr->DCTPtr->Timings.MaxRdLat2 = 0x3FF;
    NBPtr->DCTPtr->Timings.MaxRdLat3 = 0x3FF;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the default values in the MEM_DATA_STRUCT
 *
 *   NOTE: This function must first verify the NB Family.
 *
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *
 */
VOID
MemNInitDefaultsCZ (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  CPU_LOGICAL_ID LogicalId;
  ASSERT (MemPtr != NULL);

  // Only initialize on CZ processors, otherwise exit.
  GetLogicalIdOfCurrentCore (&LogicalId, &(MemPtr->StdHeader));
  if (!((LogicalId.Revision & AMD_F15_CZ_ALL) != 0)) {
    return;
  }

  //
  // DllPDBypassMode
  //
  MemPtr->ParameterListPtr->EnableDllPDBypassMode = FALSE;

  //
  // AMP
  //
  MemPtr->ParameterListPtr->AmpEnable = FALSE;
  MemPtr->ParameterListPtr->AmpWarningMsgEnable = FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Get Carrizo package type.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          The package type
 *
 */

CZ_PACKAGE_TYPE
MemNGetPackageTypeCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CPUID_DATA CpuId;
  UINT8 RegValue;
  CZ_PACKAGE_TYPE PkgType;

  LibAmdCpuidRead (AMD_CPUID_FMF, &CpuId, &(NBPtr->MemPtr->StdHeader));
  RegValue = (UINT8) (CpuId.EBX_Reg >> 28) & 0xF; // bit 31:28
  ///@todo - update the PkgType once it is reflected in BKDG
  if (RegValue == 0) {
    PkgType = CZ_FP4;
  } else {
    PkgType = UNKNOWN;
  }

  return PkgType;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Free the DRAM CAD Bus Configuration buffer.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return  TRUE - Free DRAM CAD Bus Configuration buffer successfully.
 *     @return  FALSE - Fail to free DRAM CAD Bus Configuration buffer.
 */

BOOLEAN
MemNPostDramCadBusConfigCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  return (BOOLEAN) (HeapDeallocateBuffer (AMD_MEM_DRAM_CAD_BUS_CONFIG_HANDLE, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Clock and power gate unused DCTs
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNClockAndPowerGateUnusedDctCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFDctClkGateEn, 0);
  MemNSetBitFieldNb (NBPtr, BFDctPwrGateEn, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Exclude DIMM failed PMU training.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDimmExcludesCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;
  BOOLEAN ErrorRecovery;

  ErrorRecovery = TRUE;

  IDS_OPTION_HOOK (IDS_MEM_ERROR_RECOVERY, &ErrorRecovery, &NBPtr->MemPtr->StdHeader);

  if (ErrorRecovery && (NBPtr->MCTPtr->ChannelTrainFail != 0)) {
    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb (NBPtr, Dct);

      // Disable the channel when CS fails PMU training.
      if (NBPtr->DCTPtr->Timings.CsTrainFail != 0) {
        IDS_HDT_CONSOLE (MEM_FLOW, "\tDct %d excluded due to PMU training failure on CS: %x\n", Dct, NBPtr->DCTPtr->Timings.CsTrainFail);
        NBPtr->DCTPtr->Timings.CsTestFail = NBPtr->DCTPtr->Timings.CsPresent;
        MemNDisableDctCZ (NBPtr);
      }

      MemNStitchMemoryNb (NBPtr);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This is function gets the erratum 792 for CZ to disable ScrubRate and ScrubRedir
 *
 *     @param[in,out]   *NBPtr      - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *OptParam   - Pointer to Optional Parameter
 *
 */
BOOLEAN
MemNDramScrubErratum792CZ (
  IN OUT   MEM_NB_BLOCK  *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  MemNSetBitFieldNb (NBPtr, BFDramScrub, 0);
  MemNSetBitFieldNb (NBPtr, BFScrubReDirEn, 0);
  return TRUE;
}
