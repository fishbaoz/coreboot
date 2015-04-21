/* $NoKeywords:$ */
/**
 * @file
 *
 * mnst.h
 *
 * Northbridge ST
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/ST)
 * @e \$Revision: 316318 $ @e \$Date: 2015-04-07 18:39:12 +0800 (Tue, 07 Apr 2015) $
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

#ifndef _MNST_H_
#define _MNST_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */
#define DRAM_TYPE_DDR3_ST             0
#define DRAM_TYPE_DDR4_ST             2

#define MAX_DCTS_PER_NODE_ST          1
#define MAX_CHANNELS_PER_DCT_ST       1
#define MAX_NODES_SUPPORTED_ST        1
#define MAX_CS_PER_CHANNEL_ST         4

#define MAX_MEMORY_PSTATE_ST          2

#define DEFAULT_WR_ODT_ST 6
#define DEFAULT_RD_ODT_ST 6
#define DEFAULT_RD_ODT_TRNONDLY_ST 0

#define PSP_SMCIND_INDEX_DATA   0xC00C002Cul
#define PSP_STATUS_CCP_CYPH_DIS_BIT_POS  0x00000010ul
#define PSP_STATUS_CCP_CYPH_DIS_MASK  0x00000010ul
#define PCI_CONFIG_SMU_INDIRECT_INDEX   0xB8          ///<  Gnb Offset index for SMU mbox
#define PCI_CONFIG_SMU_INDIRECT_DATA    0xBC          ///<  Gnb Offset data for SMU mbox

#define SMU_CC_PSP_FUSES_STATUS                   0xC0018000ul //0xC0018000ul    ///< offset in GNB to find PSP fusing
#define PSP_FUSES_PROTO                           0x4           ///< BIT2
#define PSP_FUSES_PLATFORM_SECURE_BOOT_EN         0x10          ///< BIT4

#define PSP_ALIGN_TO_4_BYTES
/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */
#define MEMCLK_FREQ_TO_ID(Freq)  (((Freq) == DDR2400_FREQUENCY) ? 0x1F : (((Freq) / 33) - 6))

#define MEMCLK_ID_TO_FREQ(Id)  ( ((Id) == 0x1F) ? DDR2400_FREQUENCY : ((((Id) + 6) * 33) + (((Id) + 6) / 3)) )

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */

/// Carrizo package type
typedef enum {
  ST_FP4,           ///< FP4 (BGA)
  UNKNOWN           ///< Unknown package type
} ST_PACKAGE_TYPE;

/// DRAM CAD Bus Configuration for all Memory Pstate.
typedef struct _DRAM_CAD_BUS_CONFIG_ST {
  BOOLEAN SlowMode [MAX_MEMORY_PSTATE_ST];      ///< Slow Access Mode
  UINT32  DctAddrTmg [MAX_MEMORY_PSTATE_ST];    ///< AddrCmdSetup, CsOdtSetup, and CkeSetup
} DRAM_CAD_BUS_CONFIG_ST;

/// The structure of one entry of a TxPrePN table
typedef struct {
  UINT16 MaxSpeed;      ///< Highest memory speed for this setting
  UINT8  DrvStrenP;     ///< Driver strength
  UINT8  TxPrePNVal[3]; ///< Tx predriver value per voltage
} TXPREPN_ENTRY;

/// The structure describes how to extract data and program TxPrePN
typedef struct {
  BIT_FIELD_NAME BfName;   ///< Bit field to set
  TXPREPN_ENTRY  *TabPtr;  ///< Pointer to TxPrePN table
  UINT16         PsOffset; ///< Offset to find DrvStrenP from PS struct
} PROG_TXPREPN_STRUCT;

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */
BOOLEAN
MemConstructNBBlockST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       MEM_FEAT_BLOCK_NB *FeatPtr,
  IN       MEM_SHARED_DATA *SharedPtr,
  IN       UINT8 NodeID
  );

VOID
MemNInitNBDataST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNInitDefaultsST (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  );

UINT16
MemNGetMemClkFreqInCurrentContextST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramNbPstateDependentRegistersST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNReleaseNbPstateST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

VOID
MemNSetPhyDdrModeST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  );

VOID
MemNSwitchMemPstateST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  );

VOID
MemNSyncChannelInitST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNHtMemMapST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNPlatformSpecST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDisableDctST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNSwitchDctST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Dct
  );

VOID
MemNInitNBRegTableST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   TSEFO NBRegTable[]
  );

BOOLEAN
MemNIsIdSupportedST (
  IN       CPU_LOGICAL_ID *LogicalIdPtr
  );

BOOLEAN
MemNDctCfgSelectST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  );

UINT32
MemNCmnGetSetFieldST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       UINTN FieldName,
  IN       UINT32 InputValue
  );

UINT32
MemNGetUmaSizeST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNAllocateC6AndAcpEngineStorageST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNAutoConfigST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPhyGenCfgST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPhyVoltageLevelST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  );

UINT32
MemNcmnGetSetTrainDlyST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       TRN_DLY_TYPE TrnDly,
  IN       DRBN DrbnVar,
  IN       UINT16 Field
  );

VOID
MemNProgramChannelFreqST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  );

BOOLEAN
MemNCSIntLvLowAddrAdjST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *LowBit
  );

VOID
MemNGetMaxLatParamsST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT16 MaxRcvEnDly,
  IN OUT   UINT16 *MinDlyPtr,
  IN OUT   UINT16 *MaxDlyPtr,
  IN OUT   UINT16 *DlyBiasPtr
  );

VOID
MemNResetRcvFifoST (
  IN OUT   struct _MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Dummy
  );

VOID
MemNSetPmuSequenceControlST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );


BOOLEAN
MemNFixupUmaInfoST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNFinalizeMctST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNCapSpeedBatteryLifeST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNForceEccSymbolSizeST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Size
  );

VOID
MemNInitChannelIntlvAddressBitST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDramPhyPowerSavingsST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  );

VOID
MemNDisableScrubberST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNRestoreScrubberST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNAddlDramPhyPowerSavingsST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNLoadPmuFirmwareST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

ST_PACKAGE_TYPE
MemNGetPackageTypeST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNConfigureDisDllShutdownSrST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemTTrainMaxLatencyST (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  );

BOOLEAN
MemNDisableMemHoleMappingST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNRestoreMemHoleMappingST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

VOID
MemNEnablePhyCalibrationST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPendOnPhyCalibrateCompletionST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNRateOfPhyCalibrateST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDisablePmuST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNClockAndPowerGateUnusedDctST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDimmExcludesST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemS3PspDetectionST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemS3PspPlatformSecureBootEnST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemRunningOnPspST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemPspDetectionMechanismST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemPspPlatformSecureBootEnST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPopulatePmuSramConfigST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPopulatePmuSramTimingsST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNCheckPmuFirmwareImageST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNLoadPmuFirmwareImageST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );


VOID
MemNDramPowerMngTimingST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNDramScrubErratum792ST (
  IN OUT   MEM_NB_BLOCK  *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNGetDdrMaxRateST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *OptParam
  );

#endif  /* _MNST_H_ */
