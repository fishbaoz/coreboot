/* $NoKeywords:$ */
/**
 * @file
 *
 * mns3cz.c
 *
 * CZ memory specific function to support S3 resume
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/CZ)
 * @e \$Revision: 314028 $ @e \$Date: 2015-03-03 05:56:35 -0600 (Tue, 03 Mar 2015) $
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
#include "mu.h"
#include "S3.h"
#include "mfs3.h"
#include "mncz.h"
#include "mnreg.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "mnS3cz.h"
#include "heapManager.h"
#include "merrhdl.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_NB_CZ_MNS3CZ_FILECODE
#define DCT0_MEMPSTATE_MASK 0x10
#define DCT1_MEMPSTATE_MASK 0x20
#define DO_NOT_CARE 0

#ifdef PSP_ALIGN_TO_4_BYTES
  #define _1_BYTE_STORAGE 0
  #define _2_BYTE_STORAGE 0
  #define _4_BYTE_STORAGE 0
  #define _8_BYTE_STORAGE 0
  #define _NONE_STORAGE 3
#else
  #define _1_BYTE_STORAGE 1
  #define _2_BYTE_STORAGE 2
  #define _4_BYTE_STORAGE 0
  #define _8_BYTE_STORAGE 0
  #define _NONE_STORAGE 3
#endif
/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */
/// Define S3 fields to reduce the number of encoding bits
typedef enum {
  S3RegCadTxImpedance,             ///< Register CAD Tx Impedance
  S3RegDataTxImpedance,            ///< Register Data Tx Impedance
  S3RegDataRxImpedance,            ///< Register Data Rx Impedance
  S3RegDataTxSlewRate,             ///< Register Data Tx Slew rate
  S3RegCadTxSlewRate,              ///< Register CAD Tx Slew rate
  S3RegDataRdPtrInitVal,           ///< Register Data Read Pointer Init
  S3RegRdPtrInitVal,               ///< Register Read Pointer Init
  S3RegTxDly,                      ///< Register TX delay
  S3RegDataRxDly,                  ///< Register Data Rx Delay
  S3RegDataTxDly,                  ///< Register Data Tx Delay
  S3RegDataTxEqHiImp,              ///< Register Data Tx EQ Hi Impedence
  S3RegDataTxEqLoImp,              ///< Register Data Tx EQ Lo Impedence
  S3RegDataTxEqBoostImp,           ///< Register Data Tx EQ Boost Impedence
  S3RegDbyteDqDqsRcvCntrl1,        ///< Register DDqDqsRcvCntrl1

  MaxS3Field
} S3_FIELD;

STATIC CONST UINTN ROMDATA S3FieldTransTab[] = {
  RegCadTxImpedance,
  RegDataTxImpedance,
  RegDataRxImpedance,
  RegDataTxSlewRate,
  RegCadTxSlewRate,
  RegDataRdPtrInitVal,
  RegRdPtrInitVal,
  RegTxDly,
  RegDataRxDly,
  RegDataTxDly,
  RegDataTxEqHiImp,
  RegDataTxEqLoImp,
  RegDataTxEqBoostImp,
  RegDbyteDqDqsRcvCntrl1
};

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
BOOLEAN
STATIC
MemNS3ResumeCZ (
  IN OUT   S3_MEM_NB_BLOCK *S3NBPtr,
  IN       UINT8 NodeID
  );

UINT16
STATIC
MemNS3GetRegLstPtrCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  );

AGESA_STATUS
STATIC
MemNS3GetDeviceRegLstCZ (
  IN       UINT32 RegisterLstID,
  OUT   VOID **RegisterHeader,
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3SetDfltPhyRegCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetPhyFreqCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetConPCIMaskCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  );

VOID
STATIC
MemNS3GetCSRCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetCSRCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetBitFieldCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetBitFieldCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3ChangeMemPStateContextCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetNBPStateDepRegCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetNBPStateDepRegCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SaveNBRegisterCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3RestoreNBRegisterCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

BOOLEAN
MemNS3DctCfgSelectCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  );

VOID
MemNS3DisablePmuCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3DisableChannelCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );


VOID
STATIC
MemNS3SetInstanceCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetInstanceCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3EnablePhyCalibrationCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3PendOnPhyCalibrateCompletionCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3RateOfPhyCalibrateCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNS3TriStateUnusedPadCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3PhyPowerSavingCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3SwitchMemPstateCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  );

VOID
MemNSaveRootComplexCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNRestoreRootComplexCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNSaveRootComplexMiscCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNRestoreRootComplexMiscCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

BOOLEAN
MemS3ResumeConstructNBBlockCZ (
  IN OUT   VOID *S3NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  );
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define S3_NB_PS(Dct)     ((1 << 1) | (Dct))
#define S3_CAD(Dct)       ((2 << 1) | (Dct))
#define S3_DATA(Dct)      ((3 << 1) | (Dct))

#define S3_PER_NB_PS(p, bf) ((((p) & 0xFF) << 4) | bf)
#define S3_PER_CAD_TYPE(t, bf) ((((t) & 0xFF) << 4) | bf)
#define S3_PER_DQ(d, b, bf) ((0 << 10) | (((d) & 0x3) << 8) | (((b) & 0xF) << 4) | bf)
#define S3_PER_DQS(d, b, bf) ((1 << 10) | (((d) & 0x3) << 8) | (((b) & 0xF) << 4) | bf)
#define S3_PER_DQS_L(d, b, bf) ((2 << 10) | (((d) & 0x3) << 8) | (((b) & 0xF) << 4) | bf)

PCI_SPECIAL_CASE PciSpecialCaseFuncCZ[] = {
  {MemNS3GetCSRCZ, MemNS3SetCSRCZ},
  {MemNS3GetBitFieldCZ, MemNS3SetBitFieldCZ},
  {MemNS3GetNBPStateDepRegCZ, MemNS3SetNBPStateDepRegCZ},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3SetDfltPhyRegCZ},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3SetPhyFreqCZ},
  {MemNS3ChangeMemPStateContextCZ, MemNS3ChangeMemPStateContextCZ},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3ForceNBP0Unb},
  {MemNS3SaveNBRegisterCZ, MemNS3RestoreNBRegisterCZ},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3ReleaseNBPSUnb},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3DisableChannelCZ},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3ConfigureDisDllShutdownSrCZ},
  {MemNS3GetInstanceCZ, MemNS3SetInstanceCZ},
  {MemNSaveRootComplexCZ, MemNRestoreRootComplexCZ},
  {MemNSaveRootComplexMiscCZ, MemNRestoreRootComplexMiscCZ}
};

PCI_REG_DESCRIPTOR ROMDATA S3PciPreSelfRefDescriptorCZ[] = {

  //
  // Must Restore Dram Type First
  //
  {{1, _2_BYTE_STORAGE, 1}, DCT0,  BFDramType,  0x3, 0 },
  {{1, _2_BYTE_STORAGE, 1}, DCT1,  BFDramType,  0x3, 0 },

  {{10, _NONE_STORAGE, 1}, DO_NOT_CARE, 0, 0, 0},             // Clear DisDllShutdownSR prior any pstate changes
  {{6, _NONE_STORAGE, 1},  DO_NOT_CARE, 0, 0, 0},             // Force NBP0
  {{0, _4_BYTE_STORAGE, 0},  FUNC_2, 0x110, 0xFFFFF8E4, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x40,  0xFFFFFFFF, 0},
  {{0, _1_BYTE_STORAGE, 0},  FUNC_1, 0x140, 0x000000FF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x44,  0xFFFFFFFF, 0},
  {{0, _1_BYTE_STORAGE, 0},  FUNC_1, 0x144, 0x000000FF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0xF0,  0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x120, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x124, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x200, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x204, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x208, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x20C, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x210, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x214, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x218, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x21C, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x240, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x244, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x248, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_1, 0x24C, 0xFFFFFFFF, 0},

  {{0, _4_BYTE_STORAGE, 0},  FUNC_2, 0x114, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_2, 0x118, 0xF773FFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_2, 0x11C, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_2, 0x1B0, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_2, 0x1B4, 0xFAFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_2, 0xA4,  0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_5, 0x228, 0xFFFFFFFF, 0},
  {{0, _1_BYTE_STORAGE, 0},  FUNC_5, 0x9C,  0x000000FF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_5, 0x160, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_5, 0x164, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_5, 0x168, 0xFFFFFFFF, 0},
  {{0, _4_BYTE_STORAGE, 0},  FUNC_5, 0x16C, 0xFFFFFFFF, 0}
};

CONST PCI_REGISTER_BLOCK_HEADER ROMDATA S3PciPreSelfRefCZ = {
  0,
  (sizeof (S3PciPreSelfRefDescriptorCZ) / sizeof (PCI_REG_DESCRIPTOR)),
  S3PciPreSelfRefDescriptorCZ,
  PciSpecialCaseFuncCZ
};

CONDITIONAL_PCI_REG_DESCRIPTOR ROMDATA S3CPciPreSelfDescriptorCZ[] = {
   // DCT 0
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x40,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x44,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x48,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x4C,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x50,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x54,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x58,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x5C,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x60,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x64,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x68,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x6C,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x78,  0x00020704, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT0,   0x80,  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x84,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x88,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x8C,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x90,  0xFBFDF780, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x94,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0xA8,  0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x140, 0xFFFFFFFF, DCT0_MASK, 0x01, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x144, 0xFFFFFFFF, DCT0_MASK, 0x01, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x148, 0xFFFFFFFF, DCT0_MASK, 0x01, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x14C, 0xFFFFFFFF, DCT0_MASK, 0x01, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x150, 0xFFFFFFFF, DCT0_MASK, 0x04, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x154, 0xFFFFFFFF, DCT0_MASK, 0x04, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x158, 0xFFFFFFFF, DCT0_MASK, 0x04, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x15C, 0xFFFFFFFF, DCT0_MASK, 0x04, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x160, 0xFFFFFFFF, DCT0_MASK, 0x10, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x164, 0xFFFFFFFF, DCT0_MASK, 0x10, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x168, 0xFFFFFFFF, DCT0_MASK, 0x10, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x16C, 0xFFFFFFFF, DCT0_MASK, 0x10, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x170, 0xFFFFFFFF, DCT0_MASK, 0x40, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x174, 0xFFFFFFFF, DCT0_MASK, 0x40, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x178, 0xFFFFFFFF, DCT0_MASK, 0x40, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x17C, 0xFFFFFFFF, DCT0_MASK, 0x40, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x1B8, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x1BC, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _1_BYTE_STORAGE, 1}, DCT0,   0x1C8, 0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x1F0, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x1F4, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x1F8, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x1FC, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x200, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x204, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x208, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x20C, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x214, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x218, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x21C, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT0,   0x220, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x224, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x228, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x22C, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x230, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x234, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x238, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x23C, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT0,   0x240, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _1_BYTE_STORAGE, 1}, DCT0,   0x244, 0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x248, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x24C, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x2A0, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x2E0, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x2E8, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x2EC, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x2F0, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT0,   0x2F4, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},

  // DCT 1
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x40,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x44,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x48,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x4C,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x50,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x54,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x58,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x5C,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x60,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x64,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x68,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x6C,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x78,  0x00020704, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT1,   0x80,  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x84,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x88,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x8C,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x90,  0xFBFDF780, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x94,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0xA8,  0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x140, 0xFFFFFFFF, DCT1_MASK, 0x02, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x144, 0xFFFFFFFF, DCT1_MASK, 0x02, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x148, 0xFFFFFFFF, DCT1_MASK, 0x02, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x14C, 0xFFFFFFFF, DCT1_MASK, 0x02, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x150, 0xFFFFFFFF, DCT1_MASK, 0x08, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x154, 0xFFFFFFFF, DCT1_MASK, 0x08, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x158, 0xFFFFFFFF, DCT1_MASK, 0x08, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x15C, 0xFFFFFFFF, DCT1_MASK, 0x08, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x160, 0xFFFFFFFF, DCT1_MASK, 0x20, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x164, 0xFFFFFFFF, DCT1_MASK, 0x20, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x168, 0xFFFFFFFF, DCT1_MASK, 0x20, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x16C, 0xFFFFFFFF, DCT1_MASK, 0x20, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x170, 0xFFFFFFFF, DCT1_MASK, 0x80, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x174, 0xFFFFFFFF, DCT1_MASK, 0x80, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x178, 0xFFFFFFFF, DCT1_MASK, 0x80, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x17C, 0xFFFFFFFF, DCT1_MASK, 0x80, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x1B8, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x1BC, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _1_BYTE_STORAGE, 1}, DCT1,   0x1C8, 0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x1F0, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x1F4, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x1F8, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x1FC, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x200, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x204, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x208, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x20C, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x214, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x218, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x21C, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT1,   0x220, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x224, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x228, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x22C, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x230, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x234, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x238, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x23C, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT1,   0x240, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _1_BYTE_STORAGE, 1}, DCT1,   0x244, 0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x248, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x24C, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x2A0, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x2E0, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x2E8, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x2EC, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x2F0, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT1,   0x2F4, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  // DRAM channel disable
  {{9, _NONE_STORAGE, 1}, DCT0,   0, 0, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK, 0},
  {{9, _NONE_STORAGE, 1}, DCT1,   0, 0, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK, 0},

  // Phy Initialization
  // 1. Set DDR3 mode
  {{3, _NONE_STORAGE, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK, 0},

  // 2. Phy general config
  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegDByteCtrl1,     0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegProcOdtTmg,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegVrefByteMaster, 0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegVrefByteAbyte,  0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegCalMisc2,       0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegCadDllLockMaintenance,  0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegDataDllLockMaintenance, 0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  // BIOS programs the following for maximum power savings prior to training
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegTxControlDq,    0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegTxControlDq2,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegCadByteDbgCtrl, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegDataByteDbgCtrl,       0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegMasterByteDbgCtrl,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegMasterD3MerrRcvrCntrl, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegMasterD3EvntMerr,      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRcvCntrl3,    0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,  RegDctPhyDllPowerdown0,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,  RegDctPhyDllPowerdown2,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},

  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegDByteCtrl1,     0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegProcOdtTmg,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegVrefByteMaster, 0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegVrefByteAbyte,  0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegCalMisc2,       0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegCadDllLockMaintenance,  0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegDataDllLockMaintenance, 0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  // BIOS programs the following for maximum power savings prior to training
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegTxControlDq,    0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegTxControlDq2,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegCadByteDbgCtrl, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegDataByteDbgCtrl,       0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegMasterByteDbgCtrl,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegMasterD3MerrRcvrCntrl, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegMasterD3EvntMerr,      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRcvCntrl3,    0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,  RegDctPhyDllPowerdown0,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,  RegDctPhyDllPowerdown2,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  // 3. Phy voltage Level Programming
  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                                 RegDbyteDqDqsRcvCntrl1Bcst, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS,      ECC_BYTE,  S3RegDbyteDqDqsRcvCntrl1),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,     ECC_BYTE,  S3RegDbyteDqDqsRcvCntrl1),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                                 RegCalVRefs,                0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                                 RegVrefInGlobal,            0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                                 RegDbyteDqDqsRcvCntrl1Bcst, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS,      ECC_BYTE,  S3RegDbyteDqDqsRcvCntrl1),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,     ECC_BYTE,  S3RegDbyteDqDqsRcvCntrl1),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                                 RegCalVRefs,                0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                                 RegVrefInGlobal,            0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  // 4. Frequency Change
  {{4, _NONE_STORAGE, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK, 0},

  // 5. CAD/Data bus config
  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                                RegDataTxImpedanceBcst, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS_L (ALL_DIMMS,  ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},

  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                                RegDataRxImpedanceBcst, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataRxImpedance),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},

  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                                RegDataTxImpedanceBcst, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS_L (ALL_DIMMS,  ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                                RegDataRxImpedanceBcst, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataRxImpedance),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance00,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance01,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance02,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance03,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance04,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance05,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance06,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance07,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance08,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance09,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance0a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance0b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance10,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance11,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance12,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance13,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance14,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance15,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance16,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance17,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance18,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance19,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance1a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance1b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance20,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance21,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance22,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance23,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance24,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance25,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance26,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance27,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance28,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance29,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance2a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance2b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance30,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance31,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance32,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance33,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance34,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance35,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance36,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance37,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance38,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance39,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance3a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance3b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance00,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance01,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance02,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance03,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance04,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance05,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance06,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance07,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance08,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance09,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance0a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance0b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance10,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance11,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance12,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance13,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance14,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance15,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance16,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance17,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance18,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance19,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance1a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance1b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance20,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance21,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance22,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance23,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance24,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance25,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance26,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance27,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance28,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance29,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance2a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance2b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance30,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance31,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance32,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance33,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance34,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance35,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance36,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance37,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance38,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance39,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance3a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCalTxImpedance3b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDllPstateConfigBcst,     0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDllPstateConfigBcst, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset0,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset1,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset2,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset3,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset4,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset5,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset6,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset7,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset8,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group0,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group1,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group2,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group3,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group0,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group1,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group2,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group3,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group0,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group1,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group2,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group3,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group0,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group1,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group2,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group3,         0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad0Group0,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad0Group1,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad0Group2,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad1Group0,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad1Group1,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad1Group2,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad2Group0,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad2Group1,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad2Group2,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad3Group0,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad3Group1,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad3Group2,     0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDllTuningGroupBcst,  0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly000,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly001,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly002,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly003,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly010,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly011,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly012,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly013,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly020,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly021,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly022,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly023,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly030,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly031,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly032,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly033,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly100,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly101,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly102,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly103,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly110,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly111,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly112,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly113,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly120,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly121,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly122,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly123,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly130,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly131,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly132,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly133,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly200,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly201,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly202,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly203,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly210,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly211,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly212,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly213,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly220,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly221,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly222,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly223,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly230,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly231,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly232,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly233,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly300,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly301,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly302,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly303,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly310,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly311,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly312,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly313,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly320,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly321,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly322,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly323,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly330,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly331,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly332,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly333,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly400,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly401,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly402,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly403,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly410,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly411,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly412,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly413,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly420,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly421,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly422,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly423,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly430,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly431,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly432,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly433,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly500,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly501,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly502,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly503,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly510,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly511,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly512,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly513,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly520,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly521,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly522,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly523,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly530,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly531,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly532,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly533,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly600,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly601,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly602,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly603,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly610,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly611,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly612,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly613,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly620,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly621,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly622,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly623,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly630,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly631,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly632,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly633,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly700,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly701,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly702,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly703,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly710,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly711,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly712,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly713,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly720,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly721,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly722,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly723,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly730,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly731,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly732,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly733,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly800,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly801,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly802,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly803,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly810,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly811,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly812,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly813,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly820,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly821,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly822,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly823,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly830,            0x0000FFFF, DCT0_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly831,            0x0000FFFF, DCT0_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly832,            0x0000FFFF, DCT0_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly833,            0x0000FFFF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly000,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly001,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly002,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly003,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly010,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly011,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly012,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly013,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly020,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly021,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly022,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly023,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly030,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly031,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly032,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly033,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly100,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly101,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly102,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly103,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly110,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly111,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly112,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly113,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly120,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly121,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly122,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly123,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly130,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly131,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly132,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly133,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly200,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly201,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly202,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly203,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly210,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly211,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly212,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly213,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly220,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly221,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly222,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly223,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly230,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly231,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly232,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly233,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly300,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly301,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly302,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly303,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly310,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly311,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly312,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly313,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly320,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly321,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly322,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly323,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly330,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly331,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly332,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly333,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly400,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly401,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly402,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly403,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly410,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly411,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly412,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly413,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly420,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly421,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly422,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly423,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly430,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly431,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly432,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly433,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly500,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly501,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly502,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly503,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly510,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly511,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly512,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly513,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly520,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly521,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly522,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly523,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly530,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly531,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly532,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly533,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly600,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly601,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly602,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly603,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly610,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly611,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly612,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly613,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly620,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly621,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly622,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly623,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly630,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly631,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly632,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly633,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly700,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly701,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly702,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly703,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly710,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly711,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly712,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly713,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly720,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly721,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly722,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly723,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly730,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly731,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly732,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly733,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly800,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly801,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly802,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly803,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly810,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly811,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly812,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly813,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly820,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly821,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly822,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly823,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly830,            0x000000FF, DCT0_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly831,            0x000000FF, DCT0_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly832,            0x000000FF, DCT0_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly833,            0x000000FF, DCT0_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance00,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance01,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance02,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance03,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance04,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance05,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance06,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance07,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance08,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance09,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance0a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance0b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance10,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance11,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance12,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance13,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance14,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance15,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance16,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance17,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance18,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance19,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance1a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance1b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance20,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance21,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance22,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance23,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance24,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance25,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance26,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance27,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance28,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance29,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance2a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance2b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance30,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance31,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance32,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance33,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance34,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance35,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance36,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance37,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance38,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance39,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance3a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance3b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance00,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance01,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance02,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance03,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance04,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance05,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance06,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance07,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance08,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance09,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance0a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance0b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance10,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance11,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance12,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance13,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance14,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance15,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance16,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance17,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance18,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance19,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance1a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance1b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance20,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance21,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance22,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance23,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance24,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance25,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance26,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance27,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance28,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance29,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance2a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance2b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance30,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance31,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance32,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance33,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance34,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance35,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance36,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance37,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance38,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance39,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance3a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCalTxImpedance3b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDllPstateConfigBcst,     0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDllPstateConfigBcst, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset0,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset1,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset2,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset3,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset4,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset5,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset6,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset7,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset8,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group0,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group1,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group2,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group3,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group0,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group1,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group2,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group3,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group0,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group1,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group2,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group3,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group0,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group1,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group2,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group3,         0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad0Group0,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad0Group1,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad0Group2,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad1Group0,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad1Group1,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad1Group2,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad2Group0,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad2Group1,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad2Group2,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad3Group0,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad3Group1,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad3Group2,     0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDllTuningGroupBcst,  0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly000,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly001,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly002,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly003,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly010,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly011,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly012,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly013,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly020,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly021,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly022,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly023,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly030,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly031,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly032,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly033,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly100,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly101,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly102,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly103,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly110,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly111,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly112,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly113,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly120,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly121,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly122,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly123,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly130,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly131,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly132,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly133,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly200,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly201,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly202,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly203,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly210,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly211,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly212,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly213,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly220,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly221,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly222,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly223,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly230,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly231,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly232,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly233,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly300,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly301,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly302,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly303,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly310,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly311,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly312,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly313,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly320,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly321,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly322,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly323,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly330,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly331,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly332,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly333,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly400,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly401,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly402,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly403,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly410,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly411,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly412,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly413,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly420,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly421,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly422,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly423,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly430,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly431,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly432,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly433,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly500,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly501,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly502,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly503,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly510,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly511,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly512,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly513,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly520,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly521,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly522,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly523,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly530,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly531,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly532,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly533,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly600,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly601,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly602,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly603,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly610,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly611,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly612,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly613,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly620,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly621,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly622,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly623,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly630,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly631,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly632,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly633,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly700,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly701,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly702,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly703,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly710,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly711,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly712,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly713,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly720,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly721,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly722,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly723,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly730,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly731,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly732,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly733,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly800,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly801,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly802,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly803,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly810,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly811,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly812,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly813,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly820,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly821,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly822,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly823,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly830,            0x0000FFFF, DCT1_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly831,            0x0000FFFF, DCT1_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly832,            0x0000FFFF, DCT1_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly833,            0x0000FFFF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly000,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly001,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly002,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly003,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly010,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly011,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly012,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly013,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly020,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly021,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly022,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly023,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly030,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly031,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly032,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly033,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly100,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly101,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly102,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly103,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly110,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly111,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly112,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly113,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly120,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly121,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly122,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly123,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly130,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly131,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly132,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly133,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly200,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly201,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly202,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly203,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly210,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly211,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly212,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly213,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly220,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly221,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly222,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly223,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly230,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly231,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly232,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly233,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly300,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly301,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly302,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly303,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly310,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly311,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly312,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly313,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly320,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly321,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly322,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly323,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly330,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly331,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly332,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly333,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly400,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly401,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly402,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly403,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly410,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly411,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly412,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly413,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly420,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly421,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly422,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly423,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly430,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly431,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly432,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly433,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly500,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly501,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly502,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly503,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly510,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly511,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly512,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly513,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly520,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly521,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly522,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly523,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly530,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly531,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly532,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly533,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly600,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly601,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly602,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly603,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly610,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly611,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly612,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly613,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly620,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly621,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly622,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly623,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly630,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly631,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly632,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly633,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly700,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly701,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly702,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly703,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly710,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly711,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly712,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly713,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly720,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly721,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly722,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly723,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly730,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly731,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly732,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly733,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly800,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly801,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly802,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly803,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly810,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly811,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly812,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly813,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly820,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly821,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly822,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly823,            0x000000FF, DCT1_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly830,            0x000000FF, DCT1_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly831,            0x000000FF, DCT1_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly832,            0x000000FF, DCT1_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly833,            0x000000FF, DCT1_MASK, 0x80, 0},

  // 6. Phy FIFO Config
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (0, S3RegRdPtrInitVal),             0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (0, S3RegDataRdPtrInitVal),         0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (1, S3RegRdPtrInitVal),             0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (1, S3RegDataRdPtrInitVal),         0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (2, S3RegRdPtrInitVal),             0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (2, S3RegDataRdPtrInitVal),         0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (3, S3RegRdPtrInitVal),             0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (3, S3RegDataRdPtrInitVal),         0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (0, S3RegRdPtrInitVal),             0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (0, S3RegDataRdPtrInitVal),         0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (1, S3RegRdPtrInitVal),             0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (1, S3RegDataRdPtrInitVal),         0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (2, S3RegRdPtrInitVal),             0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (2, S3RegDataRdPtrInitVal),         0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (3, S3RegRdPtrInitVal),             0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (3, S3RegDataRdPtrInitVal),         0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  // 7. Predriver
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CKE, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CS,  S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ODT, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate), 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CLK, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                    RegDataTxSlewRateBcst,    0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CKE, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CS,  S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ODT, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate), 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CLK, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                    RegDataTxSlewRateBcst,    0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  // Program MemPstate 1 registers
  // Switch to MemPstate context 1
  {{5, _NONE_STORAGE, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MEMPSTATE_MASK + DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK, 0},

  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegDByteCtrl1,                       0x000000FF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},

  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x200 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x204 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x208 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x20C ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x214 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x218 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x21C ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x22C ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x240 ,1), 0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x248 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x2A0 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x2E8 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x2EC ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x2F0 ,1), 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT0,   SET_S3_MEM_PSTATE_OFFSET (0x2F4 ,1), 0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},

  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegDByteCtrl1,                       0x000000FF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x200 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x204 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x208 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x20C ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x214 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x218 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x21C ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x22C ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x240 ,1), 0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x248 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x2A0 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x2E8 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x2EC ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x2F0 ,1), 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{7, _2_BYTE_STORAGE, 1}, DCT1,   SET_S3_MEM_PSTATE_OFFSET (0x2F4 ,1), 0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegCadDllLockMaintenance,  0x000000FF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT0,   RegDataDllLockMaintenance, 0x000000FF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegCadDllLockMaintenance,  0x000000FF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, DCT1,   RegDataDllLockMaintenance, 0x000000FF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                                RegDataTxImpedanceBcst, 0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS_L (ALL_DIMMS,  ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},

  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                                RegDataRxImpedanceBcst, 0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataRxImpedance),  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},

  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                                RegDataTxImpedanceBcst, 0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataTxImpedance),  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS_L (ALL_DIMMS,  ALL_BYTES, S3RegDataTxImpedance),  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                                RegDataRxImpedanceBcst, 0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS,     ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ALL_BYTES, S3RegDataRxImpedance),  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS,    ECC_BYTE,  S3RegDataRxImpedance),  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance00,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance01,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance02,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance03,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance04,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance05,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance06,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance07,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance08,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance09,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance0a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance0b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance10,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance11,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance12,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance13,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance14,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance15,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance16,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance17,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance18,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance19,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance1a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance1b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance20,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance21,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance22,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance23,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance24,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance25,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance26,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance27,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance28,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance29,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance2a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance2b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance30,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance31,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance32,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance33,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance34,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance35,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance36,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance37,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance38,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance39,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance3a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegCadTxImpedance3b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDllPstateConfigBcst,     0x000000FF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDllPstateConfigBcst, 0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset0,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset1,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset2,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset3,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset4,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset5,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset6,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset7,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRdPtrOffset8,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad0Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad1Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad2Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegTxDlyCad3Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad0Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad0Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad0Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad1Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad1Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad1Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad2Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad2Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad2Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad3Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad3Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDllTuningCad3Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDllTuningGroupBcst,  0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly000,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly001,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly002,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly003,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly010,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly011,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly012,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly013,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly020,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly021,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly022,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly023,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly030,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly031,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly032,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly033,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly100,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly101,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly102,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly103,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly110,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly111,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly112,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly113,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly120,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly121,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly122,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly123,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly130,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly131,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly132,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly133,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly200,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly201,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly202,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly203,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly210,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly211,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly212,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly213,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly220,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly221,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly222,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly223,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly230,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly231,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly232,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly233,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly300,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly301,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly302,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly303,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly310,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly311,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly312,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly313,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly320,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly321,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly322,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly323,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly330,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly331,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly332,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly333,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly400,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly401,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly402,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly403,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly410,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly411,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly412,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly413,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly420,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly421,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly422,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly423,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly430,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly431,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly432,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly433,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly500,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly501,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly502,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly503,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly510,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly511,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly512,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly513,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly520,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly521,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly522,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly523,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly530,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly531,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly532,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly533,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly600,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly601,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly602,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly603,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly610,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly611,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly612,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly613,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly620,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly621,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly622,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly623,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly630,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly631,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly632,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly633,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly700,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly701,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly702,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly703,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly710,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly711,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly712,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly713,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly720,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly721,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly722,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly723,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly730,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly731,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly732,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly733,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly800,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly801,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly802,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly803,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly810,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly811,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly812,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly813,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly820,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly821,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly822,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly823,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly830,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly831,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly832,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT0,   RegDataRxDly833,            0x0000FFFF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly000,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly001,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly002,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly003,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly010,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly011,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly012,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly013,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly020,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly021,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly022,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly023,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly030,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly031,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly032,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly033,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly100,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly101,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly102,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly103,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly110,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly111,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly112,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly113,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly120,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly121,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly122,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly123,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly130,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly131,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly132,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly133,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly200,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly201,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly202,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly203,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly210,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly211,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly212,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly213,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly220,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly221,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly222,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly223,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly230,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly231,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly232,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly233,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly300,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly301,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly302,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly303,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly310,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly311,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly312,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly313,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly320,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly321,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly322,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly323,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly330,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly331,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly332,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly333,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly400,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly401,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly402,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly403,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly410,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly411,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly412,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly413,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly420,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly421,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly422,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly423,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly430,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly431,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly432,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly433,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly500,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly501,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly502,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly503,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly510,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly511,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly512,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly513,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly520,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly521,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly522,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly523,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly530,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly531,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly532,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly533,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly600,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly601,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly602,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly603,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly610,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly611,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly612,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly613,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly620,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly621,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly622,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly623,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly630,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly631,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly632,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly633,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly700,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly701,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly702,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly703,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly710,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly711,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly712,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly713,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly720,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly721,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly722,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly723,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly730,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly731,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly732,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly733,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly800,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly801,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly802,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly803,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly810,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly811,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly812,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly813,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly820,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly821,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly822,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly823,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly830,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x01, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly831,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x04, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly832,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x10, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT0,   RegDataTxDly833,            0x000000FF, DCT0_MEMPSTATE_MASK, 0x40, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance00,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance01,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance02,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance03,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance04,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance05,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance06,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance07,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance08,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance09,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance0a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance0b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance10,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance11,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance12,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance13,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance14,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance15,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance16,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance17,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance18,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance19,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance1a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance1b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance20,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance21,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance22,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance23,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance24,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance25,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance26,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance27,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance28,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance29,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance2a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance2b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance30,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance31,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance32,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance33,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance34,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance35,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance36,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance37,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance38,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance39,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance3a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegCadTxImpedance3b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDllPstateConfigBcst,     0x000000FF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDllPstateConfigBcst, 0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset0,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset1,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset2,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset3,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset4,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset5,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset6,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset7,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRdPtrOffset8,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad0Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad1Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad2Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegTxDlyCad3Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad0Group0,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad0Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad0Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad1Group0,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad1Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad1Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad2Group0,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad2Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad2Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad3Group0,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad3Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDllTuningCad3Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDllTuningGroupBcst,  0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly000,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly001,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly002,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly003,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly010,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly011,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly012,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly013,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly020,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly021,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly022,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly023,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly030,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly031,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly032,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly033,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly100,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly101,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly102,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly103,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly110,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly111,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly112,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly113,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly120,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly121,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly122,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly123,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly130,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly131,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly132,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly133,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly200,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly201,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly202,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly203,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly210,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly211,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly212,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly213,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly220,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly221,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly222,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly223,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly230,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly231,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly232,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly233,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly300,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly301,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly302,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly303,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly310,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly311,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly312,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly313,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly320,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly321,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly322,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly323,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly330,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly331,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly332,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly333,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly400,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly401,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly402,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly403,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly410,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly411,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly412,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly413,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly420,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly421,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly422,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly423,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly430,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly431,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly432,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly433,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly500,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly501,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly502,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly503,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly510,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly511,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly512,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly513,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly520,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly521,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly522,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly523,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly530,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly531,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly532,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly533,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly600,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly601,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly602,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly603,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly610,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly611,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly612,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly613,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly620,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly621,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly622,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly623,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly630,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly631,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly632,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly633,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly700,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly701,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly702,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly703,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly710,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly711,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly712,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly713,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly720,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly721,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly722,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly723,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly730,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly731,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly732,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly733,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly800,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly801,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly802,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly803,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly810,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly811,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly812,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly813,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly820,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly821,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly822,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly823,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly830,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly831,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly832,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _2_BYTE_STORAGE, 1}, DCT1,   RegDataRxDly833,            0x0000FFFF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly000,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly001,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly002,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly003,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly010,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly011,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly012,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly013,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly020,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly021,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly022,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly023,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly030,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly031,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly032,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly033,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly100,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly101,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly102,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly103,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly110,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly111,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly112,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly113,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly120,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly121,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly122,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly123,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly130,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly131,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly132,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly133,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly200,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly201,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly202,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly203,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly210,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly211,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly212,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly213,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly220,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly221,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly222,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly223,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly230,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly231,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly232,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly233,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly300,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly301,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly302,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly303,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly310,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly311,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly312,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly313,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly320,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly321,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly322,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly323,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly330,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly331,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly332,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly333,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly400,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly401,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly402,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly403,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly410,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly411,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly412,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly413,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly420,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly421,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly422,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly423,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly430,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly431,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly432,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly433,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly500,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly501,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly502,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly503,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly510,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly511,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly512,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly513,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly520,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly521,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly522,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly523,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly530,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly531,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly532,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly533,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly600,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly601,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly602,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly603,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly610,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly611,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly612,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly613,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly620,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly621,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly622,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly623,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly630,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly631,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly632,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly633,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly700,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly701,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly702,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly703,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly710,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly711,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly712,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly713,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly720,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly721,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly722,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly723,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly730,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly731,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly732,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly733,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly800,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly801,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly802,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly803,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly810,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly811,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly812,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly813,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly820,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly821,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly822,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly823,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly830,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x02, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly831,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x08, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly832,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x20, 0},
  {{11, _1_BYTE_STORAGE, 1}, DCT1,   RegDataTxDly833,            0x000000FF, DCT1_MEMPSTATE_MASK, 0x80, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CKE,      S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CS,       S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ODT,      S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CLK,      S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT0,                                         RegDataTxSlewRateBcst,    0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CKE,      S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CS,       S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ODT,      S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CLK,      S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _2_BYTE_STORAGE, 1}, DCT1,                                         RegDataTxSlewRateBcst,    0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},

  // Switch back to MemPstate context 0
  {{5, _NONE_STORAGE, 1}, DO_NOT_CARE, 0, DO_NOT_CARE, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK, 0},

  // DCT0
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK, 0},

  // DCT1
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK, 0},

  // MemPstate 1
  {{5, _NONE_STORAGE, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MEMPSTATE_MASK + DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK, 0},

  // DCT0
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK, 0},

  // DCT1
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},
  {{1, _1_BYTE_STORAGE, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK, 0},

  {{5, _NONE_STORAGE, 1}, DO_NOT_CARE, 0, DO_NOT_CARE, DCT0_MEMPSTATE_MASK + DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK, 0}

};

CONST CPCI_REGISTER_BLOCK_HEADER ROMDATA S3CPciPreSelfRefCZ = {
  0,
  (sizeof (S3CPciPreSelfDescriptorCZ) / sizeof (CONDITIONAL_PCI_REG_DESCRIPTOR)),
  S3CPciPreSelfDescriptorCZ,
  PciSpecialCaseFuncCZ
};

CONDITIONAL_PCI_REG_DESCRIPTOR ROMDATA S3CPciPostSelfDescriptorCZ[] = {
  {{0, _4_BYTE_STORAGE, 0}, FUNC_3, 0x44,  0x00400004, ANY_DIMM_MASK, ANY_DIMM_MASK, 0},
  {{0, _4_BYTE_STORAGE, 0}, FUNC_3, 0x88,  0x08000000, ANY_DIMM_MASK, ANY_DIMM_MASK, 0},

  {{0, _4_BYTE_STORAGE, 0}, FUNC_5, 0x240, 0xFFFFFFFF, ANY_DIMM_MASK, ANY_DIMM_MASK, 0},
  {{0, _2_BYTE_STORAGE, 0}, FUNC_5, 0x244, 0x0000FFFF, ANY_DIMM_MASK, ANY_DIMM_MASK, 0},
  {{0, _4_BYTE_STORAGE, 0}, FUNC_5, 0x248, 0xFFFFFFFF, ANY_DIMM_MASK, ANY_DIMM_MASK, 0},
  {{0, _2_BYTE_STORAGE, 0}, FUNC_5, 0x24C, 0x0000FFFF, ANY_DIMM_MASK, ANY_DIMM_MASK, 0},

  {{12, _4_BYTE_STORAGE, 1}, FUNC_0,  0x90, 0xFFFFFFFF, ANY_DIMM_MASK, ANY_DIMM_MASK, SAVE_AT_POST_END | S3_RESUME_MODE},
  {{13, _4_BYTE_STORAGE, 1}, FUNC_0,  0x19, 0xFFFFFFFF, ANY_DIMM_MASK, ANY_DIMM_MASK, SAVE_AT_POST_END | S3_RESUME_MODE},
  {{13, _1_BYTE_STORAGE, 1}, FUNC_0,  0x1A, 0x000000FF, ANY_DIMM_MASK, ANY_DIMM_MASK, SAVE_AT_POST_END | S3_RESUME_MODE},

  // CC6SaveEn
  {{0, _4_BYTE_STORAGE, 0}, FUNC_2, 0x118, 0x00040000, ANY_DIMM_MASK, ANY_DIMM_MASK, 0},
  // LockDramCfg
  {{0, _4_BYTE_STORAGE, 0}, FUNC_2, 0x118, 0x00080000, ANY_DIMM_MASK, ANY_DIMM_MASK, S3_RESUME_MODE},
  // DramSprLock
  {{7, _4_BYTE_STORAGE, 1}, DCT0,   0x78,  0x40000000, DCT0_MASK, DCT0_ANY_DIMM_MASK, S3_RESUME_MODE},
  {{7, _4_BYTE_STORAGE, 1}, DCT1,   0x78,  0x40000000, DCT1_MASK, DCT1_ANY_DIMM_MASK, S3_RESUME_MODE},

  // Release NBPstate
  {{8, _NONE_STORAGE, 1}, DO_NOT_CARE, 0, DO_NOT_CARE, ANY_DIMM_MASK, ANY_DIMM_MASK, 0}
};

CONST CPCI_REGISTER_BLOCK_HEADER ROMDATA S3CPciPostSelfRefCZ = {
  0,
  (sizeof (S3CPciPostSelfDescriptorCZ) / sizeof (CONDITIONAL_PCI_REG_DESCRIPTOR)),
  S3CPciPostSelfDescriptorCZ,
  PciSpecialCaseFuncCZ
};

MSR_REG_DESCRIPTOR ROMDATA S3MSRPreSelfRefDescriptorCZ[] = {
  {{0, _8_BYTE_STORAGE, 0}, 0xC0010010, 0x00000000007F0000, 0},
  {{0, _8_BYTE_STORAGE, 0}, 0xC001001A, 0x0000FFFFFF800000, 0},
  {{0, _8_BYTE_STORAGE, 0}, 0xC001001D, 0x0000FFFFFF800000, 0},
  {{0, _8_BYTE_STORAGE, 0}, 0xC001001F, 0x0044601080000600, 0},
  {{0, _8_BYTE_STORAGE, 0}, 0xC00110A2, 0xFFFFFFFFFFFFFFFF, SAVE_AT_POST_END | S3_RESUME_MODE}, //For enable RdRand for S3 Exit
};

CONST MSR_REGISTER_BLOCK_HEADER ROMDATA S3MSRPreSelfRefCZ = {
  0,
  (sizeof (S3MSRPreSelfRefDescriptorCZ) / sizeof (MSR_REG_DESCRIPTOR)),
  S3MSRPreSelfRefDescriptorCZ,
  NULL
};

VOID *MemS3RegListCZ[] = {
  (VOID *)&S3PciPreSelfRefCZ,
  NULL,
  (VOID *)&S3CPciPreSelfRefCZ,
  (VOID *)&S3CPciPostSelfRefCZ,
  (VOID *)&S3MSRPreSelfRefCZ,
  NULL,
  NULL,
  NULL
};

MSR_REG_DESCRIPTOR ROMDATA PspS3MSRPreSelfRefDescriptorCZ[] = {
  {0}
};
MSR_SPECIAL_CASE PspMsrSpecialCaseFuncCZ[] = {
  {NULL}
};
CONST MSR_REGISTER_BLOCK_HEADER ROMDATA PspS3MSRPreSelfRefCZ = {
  0,
  0,
  PspS3MSRPreSelfRefDescriptorCZ,
  PspMsrSpecialCaseFuncCZ
};
VOID *MemS3PspRegListCZ[] = {
  (VOID *)&S3PciPreSelfRefCZ,
  NULL,
  (VOID *)&S3CPciPreSelfRefCZ,
  (VOID *)&S3CPciPostSelfRefCZ,
  (VOID *)&PspS3MSRPreSelfRefCZ,
  NULL,
  NULL,
  NULL
};

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the northbridge block for S3 resume
 *
 *     @param[in,out]   *S3NBPtr   - Pointer to MEM_NB_BLOCK.
 *     @param[in,out]   *MemPtr  - Pointer to MEM_DATA_STRUCT.
 *     @param[in]       NodeID   - Node ID of the target node.
 *
 *      @return         BOOLEAN
 *                         TRUE - This is the correct constructor for the targeted node.
 *                         FALSE - This isn't the correct constructor for the targeted node.
 */
BOOLEAN
MemS3ResumeConstructNBBlockCZ (
  IN OUT   VOID *S3NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  )
{
  INT32 i;
  MEM_NB_BLOCK *NBPtr;

  NBPtr = ((S3_MEM_NB_BLOCK *)S3NBPtr)->NBPtr;
  NBPtr->MemPtr = MemPtr;
  //
  // Determine if this is the expected NB Type
  //
  if (!MemRunningOnPspCZ (NBPtr)) {
    GetLogicalIdOfSocket (MemPtr->DiesPerSystem[NodeID].SocketId, &(MemPtr->DiesPerSystem[NodeID].LogicalCpuid), &(MemPtr->StdHeader));
    if (!MemNIsIdSupportedCZ (&(MemPtr->DiesPerSystem[NodeID].LogicalCpuid))) {
      return FALSE;
    }
  }

  NBPtr->MCTPtr = &(MemPtr->DiesPerSystem[NodeID]);
  NBPtr->PciAddr.AddressValue = MemPtr->DiesPerSystem[NodeID].PciAddr.AddressValue;
  MemNInitNBRegTableCZ (NBPtr, NBPtr->NBRegTable);
  NBPtr->Node = ((UINT8) NBPtr->PciAddr.Address.Device) - 24;
  NBPtr->Dct = 0;
  NBPtr->Channel = 0;
  NBPtr->Ganged = FALSE;
  NBPtr->NodeCount = MAX_NODES_SUPPORTED_CZ;
  NBPtr->DctCount = MAX_DCTS_PER_NODE_CZ;
  NBPtr->MemPstate = MEMORY_PSTATE0;
  NBPtr->MemPstateStage = MEMORY_PSTATE_NONE;
  NBPtr->NbPsCtlReg = 0;

  for (i = 0; i < EnumSize; i++) {
    NBPtr->IsSupported[i] = FALSE;
  }

  for (i = 0; i < NumberOfHooks; i++) {
    NBPtr->FamilySpecificHook[i] = (BOOLEAN (*) (MEM_NB_BLOCK *, VOID *)) memDefTrue;
  }

  LibAmdMemFill (NBPtr->DctCache, 0, sizeof (NBPtr->DctCache), &MemPtr->StdHeader);

  NBPtr->SwitchDCT = MemNSwitchDCTNb;
  NBPtr->SwitchChannel = MemNSwitchChannelNb;
  NBPtr->MemNCmnGetSetFieldNb = MemNCmnGetSetFieldCZ;
  NBPtr->GetBitField = MemNGetBitFieldNb;
  NBPtr->SetBitField = MemNSetBitFieldNb;
  NBPtr->MemNIsIdSupportedNb = MemNIsIdSupportedCZ;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->S3DatablobRev = S3_DATABLOB_REV_CZ;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3ExitSelfRefReg = (VOID (*) (MEM_NB_BLOCK *, AMD_CONFIG_PARAMS *)) memDefRet;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetConPCIMask = MemNS3GetConPCIMaskCZ;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetConMSRMask = (VOID (*) (MEM_NB_BLOCK *, DESCRIPTOR_GROUP *)) memDefRet;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3Resume = MemNS3ResumeCZ;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3RestoreScrub = (VOID (*) (MEM_NB_BLOCK *, UINT8)) memDefRet;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetRegLstPtr = MemNS3GetRegLstPtrCZ;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetDeviceRegLst = MemNS3GetDeviceRegLstCZ;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3SpecialCaseHeapSize = 0;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3PspDetect = MemS3PspDetectionCZ;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3PspPlatformSecureBootEn = MemS3PspPlatformSecureBootEnCZ;
  NBPtr->MemRunningOnPsp = MemRunningOnPspCZ;
  NBPtr->FamilySpecificHook[DCTSelectSwitch] = MemNS3DctCfgSelectCZ;

  MemNSetBitFieldNb (NBPtr, BFDctCfgSel, 0);
  MemNSwitchDCTNb (NBPtr, 0);
  NBPtr->MemPstate = (MEM_PSTATE)0;
  NBPtr->DctCount = MAX_D3_DCTS_PER_NODE_CZ;

  return TRUE;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function brings the DIMMs out of self refresh for CZ
 *
 *     @param[in,out]   *S3NBPtr - Pointer to the S3_MEM_NB_BLOCK
 *     @param[in]       NodeID   - The Node id of the target die
 *
 *     @return         TRUE  - Successful
 *                     FALSE - Fail
 */
BOOLEAN
STATIC
MemNS3ResumeCZ (
  IN OUT   S3_MEM_NB_BLOCK *S3NBPtr,
  IN       UINT8 NodeID
  )
{
  UINT8 Dct;
  MEM_NB_BLOCK *NBPtr;
  MEM_DATA_STRUCT *MemPtr;

  NBPtr = S3NBPtr->NBPtr;
  MemPtr = NBPtr->MemPtr;
  IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, NULL, &MemPtr->StdHeader) {
    // Platform hook before exit self refresh
    AGESA_TESTPOINT (TpProcMemBeforeAgesaHookBeforeExitSelfRef, &MemPtr->StdHeader);
    if (AgesaHookBeforeExitSelfRefresh (0, MemPtr) == AGESA_SUCCESS) {
    }
    AGESA_TESTPOINT (TpProcMemAfterAgesaHookBeforeExitSelfRef, &MemPtr->StdHeader);
  }

  //Override the NB Pstate if needed
  IDS_OPTION_HOOK (IDS_NB_PSTATE_DIDVID, S3NBPtr->NBPtr, &MemPtr->StdHeader);

  // Clear DisAutoComp to complete predriver programming
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, &Dct, &MemPtr->StdHeader) {
      MemNSwitchDCTNb (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 0);
        // Enable PHY Calibration
        MemNS3EnablePhyCalibrationCZ (NBPtr);

        // PHY Power Saving
        MemNS3PhyPowerSavingCZ (NBPtr);
      } else {
        // Program D18F2x9C_x0002_0080[1:0][PmuClkDiv]]=7 (PMU CLK turned off).
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv), 7);
      }
    }
  }

  // Change power state
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, &Dct, &MemPtr->StdHeader) {
      MemNSwitchDCTNb (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        MemNSetBitFieldNb (NBPtr, BFMemResetL, 1);
        MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);
        MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 0);
      }
    }
  }

  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, &Dct, &MemPtr->StdHeader) {
      MemNSwitchDCTNb (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        // Query for the calibrate completion.
        MemNS3PendOnPhyCalibrateCompletionCZ (NBPtr);
        // Set calibration rate.
        MemNS3RateOfPhyCalibrateCZ (NBPtr);
      }
    }
  }

  // Sync channel init
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, &Dct, &MemPtr->StdHeader) {
      MemNSwitchDCTNb (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        MemNSetBitFieldNb (NBPtr, BFPtrInitReq, 1);
        while (MemNGetBitFieldNb (NBPtr, BFPtrInitReq) != 0) {}
      }
    }
  }

  IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, NULL, &MemPtr->StdHeader) {
    // Exit self refresh
    for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
      MemNSwitchDCTNb (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        MemNSetBitFieldNb (NBPtr, BFExitSelfRef, 1);
        while (MemNGetBitFieldNb (NBPtr, BFExitSelfRef) != 0) {}
        MemUWait10ns ((7800 * 2) / 10, MemPtr);  // wait Tref * 2
        // Program D18F2x78_dct[1:0][ChanVal]=1.
        MemNSetBitFieldNb (NBPtr, BFChanVal, 1);
      }
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function returns the register list for each device for CZ
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in, out]  *DescriptPtr - Pointer to DESCRIPTOR_GROUP
 *     @return          UINT16 - size of the device descriptor on the target node.
 */
UINT16
STATIC
MemNS3GetRegLstPtrCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  )
{
  UINT8 i;
  UINT16 Size;
  Size = 0;
  for (i = PRESELFREF; i <= POSTSELFREF; i ++) {
    DescriptPtr->PCIDevice[i].Type = (UINT8) (DEV_TYPE_PCI_PRE_ESR + i);
    DescriptPtr->PCIDevice[i].Node = NBPtr->Node;
    DescriptPtr->PCIDevice[i].RegisterListID = 0xFFFFFFFF;
    if (MemS3PspPlatformSecureBootEnCZ (NBPtr)) {
      if ((PCI_REGISTER_BLOCK_HEADER *) MemS3PspRegListCZ[PCI_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->PCIDevice[i].RegisterListID = PCI_LST_ESR_CZ + i;
        Size += sizeof (PCI_DEVICE_DESCRIPTOR);
      }
    } else {
      if ((PCI_REGISTER_BLOCK_HEADER *) MemS3RegListCZ[PCI_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->PCIDevice[i].RegisterListID = PCI_LST_ESR_CZ + i;
        Size += sizeof (PCI_DEVICE_DESCRIPTOR);
      }
    }
    DescriptPtr->CPCIDevice[i].Type = (UINT8) (DEV_TYPE_CPCI_PRE_ESR + i);
    DescriptPtr->CPCIDevice[i].Node = NBPtr->Node;
    DescriptPtr->CPCIDevice[i].RegisterListID = 0xFFFFFFFF;
    if (MemS3PspPlatformSecureBootEnCZ (NBPtr)) {
      if ((CPCI_REGISTER_BLOCK_HEADER *) MemS3PspRegListCZ[CPCI_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->CPCIDevice[i].RegisterListID = CPCI_LST_ESR_CZ + i;
        Size += sizeof (CONDITIONAL_PCI_DEVICE_DESCRIPTOR);
      }
    } else {
      if ((CPCI_REGISTER_BLOCK_HEADER *) MemS3RegListCZ[CPCI_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->CPCIDevice[i].RegisterListID = CPCI_LST_ESR_CZ + i;
        Size += sizeof (CONDITIONAL_PCI_DEVICE_DESCRIPTOR);
      }
    }
    DescriptPtr->MSRDevice[i].Type = (UINT8) (DEV_TYPE_MSR_PRE_ESR + i);
    DescriptPtr->MSRDevice[i].RegisterListID = 0xFFFFFFFF;
    if (MemS3PspPlatformSecureBootEnCZ (NBPtr)) {
      if ((MSR_REGISTER_BLOCK_HEADER *) MemS3PspRegListCZ[MSR_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->MSRDevice[i].RegisterListID = MSR_LST_ESR_CZ + i;
        Size += sizeof (MSR_DEVICE_DESCRIPTOR);
      }
    } else {
      if ((MSR_REGISTER_BLOCK_HEADER *) MemS3RegListCZ[MSR_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->MSRDevice[i].RegisterListID = MSR_LST_ESR_CZ + i;
        Size += sizeof (MSR_DEVICE_DESCRIPTOR);
      }
    }
    DescriptPtr->CMSRDevice[i].Type = (UINT8) (DEV_TYPE_CMSR_PRE_ESR + i);
    DescriptPtr->CMSRDevice[i].RegisterListID = 0xFFFFFFFF;
    if (MemS3PspPlatformSecureBootEnCZ (NBPtr)) {
      if ((CMSR_REGISTER_BLOCK_HEADER *) MemS3PspRegListCZ[CMSR_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->CMSRDevice[i].RegisterListID = CMSR_LST_ESR_CZ + i;
        Size += sizeof (CONDITIONAL_MSR_DEVICE_DESCRIPTOR);
      }
    } else {
      if ((CMSR_REGISTER_BLOCK_HEADER *) MemS3RegListCZ[CMSR_LST_ESR_CZ - PCI_LST_ESR_CZ + i] != NULL) {
        DescriptPtr->CMSRDevice[i].RegisterListID = CMSR_LST_ESR_CZ + i;
        Size += sizeof (CONDITIONAL_MSR_DEVICE_DESCRIPTOR);
      }
    }
  }
  return Size;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function return the register list according to the register ID.
 *
 *     @param[in]   RegisterLstID - value of the Register list ID.
 *     @param[out]  **RegisterHeader - pointer to the address of the register list.
  *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @return      AGESA_STATUS
 *                          - AGESA_FATAL
 *                          - AGESA_SUCCESS
 */
AGESA_STATUS
STATIC
MemNS3GetDeviceRegLstCZ (
  IN       UINT32 RegisterLstID,
     OUT   VOID **RegisterHeader,
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  if (MemS3PspPlatformSecureBootEnCZ (NBPtr)) {
    if (RegisterLstID >= (sizeof (MemS3PspRegListCZ) / sizeof (VOID *))) {
      ASSERT(FALSE); // RegisterListID exceeded size of Register list
      return AGESA_FATAL;
    }
    if (MemS3PspRegListCZ[RegisterLstID] != NULL) {
      *RegisterHeader = MemS3PspRegListCZ[RegisterLstID];
      return AGESA_SUCCESS;
    }
  } else {
    if (RegisterLstID >= (sizeof (MemS3RegListCZ) / sizeof (VOID *))) {
      ASSERT(FALSE); // RegisterListID exceeded size of Register list
      return AGESA_FATAL;
    }
    if (MemS3RegListCZ[RegisterLstID] != NULL) {
      *RegisterHeader = MemS3RegListCZ[RegisterLstID];
      return AGESA_SUCCESS;
    }
  }
  ASSERT(FALSE); // Device register list error
  return AGESA_FATAL;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function setups default phy registers eg. PmuReset, PmuStall, MajorMode...
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetDfltPhyRegCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;
  UINT8 Mode;
  UINT8 Dct;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb  (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        // 1. Program D18F2x9C_x0002_0099_dct[1:0][PmuReset,PmuStall] = 1,1.
        MemNSetBitFieldNb (NBPtr, BFPmuReset, 1);
        MemNSetBitFieldNb (NBPtr, BFPmuStall, 1);
        MemNSetBitFieldNb (NBPtr, BFPmuSramSd, 1);

        // 2. Program D18F2x9C_x0002_000E_dct[1:0][PhyDisable]=0.
        MemNSetBitFieldNb (NBPtr, BFPhyDisable, 0);

        // According to the type of DRAM attached, program the following phy mode
        // D18F2x9C_x0000_000E_dct[1:0][D4_Mode]
        Mode = ((MemNGetBitFieldNb (NBPtr, BFDramType) == 0) ? DRAM_TYPE_DDR3_CZ : DRAM_TYPE_DDR4_CZ);
        // According to the type of DRAM attached, program the following phy mode
        // D18F2x9C_x0000_000E_dct[1:0][D4_Mode]
        MemNSetBitFieldNb (NBPtr, BFD4ModeAbyte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
        // D18F2x9C_x00[F,8:0]1_000E_dct[1:0][D4_Mode]
        MemNSetBitFieldNb (NBPtr, BFD4ModeDbyte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
        // D18F2x9C_x0002_000E_dct[1:0][D4_Mode]
        MemNSetBitFieldNb (NBPtr, BFD4ModeMByte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
        // D18F2x9C_x00[F,3:0]0_[F,B:0]04A_dct[1:0][MajorMode]
        MemNSetBitFieldNb (NBPtr, BFMajorModeAbyte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x3);
        // D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]043_dct[1:0][MajorMode]
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFMajorModeDbyte), (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x3);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFMajorModeDbyte), (Mode == DRAM_TYPE_DDR3_CZ) ? 0x4 : 0x3);
        // D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]043_dct[1:0][ExtVrefRange]
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFExtVrefRange), (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFExtVrefRange), (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
        // D18F2x9C_x0004_00F0_dct[1:0][AcsmDdrMode(1=DDR4, 0=DDR3)] (ACSM)
        MemNSetBitFieldNb (NBPtr, BFAcsmDdrMode, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x1 : 0x0);

        // Program D18F2x9C_x0002_0087_dct[1:0][DisAutoComp, DisPredriverCal] = {1,1}.
        // Before restoring DRAM Data Bus Configuration
        MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 1);
        MemNSetBitFieldNb (NBPtr, BFDisPredriverCal, 1);
      }
    }
    MemNSwitchDCTNb  (NBPtr, 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function sets Phy frequency
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetPhyFreqCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;
  UINT16 Freq;
  UINT16 PllMultDiv;
  UINT16 PMUClkDiv;
  UINT8 Dct;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb  (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, RegVrefHSpeed), 0x40);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, RegVrefLpower), 0x40);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegVrefHSpeed), 0x40);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegVrefLpower), 0x40);

        // 1. Program D18F2x9C_x0002_0093_dct[1:0][PllRegWaitTime] = 4Bh.
        MemNSetBitFieldNb (NBPtr, RegPllRegWaitTime, 0x4B);

        // 2. Program D18F2x9C_x0002_0089_dct[1:0][PllLockTime] = C8h. Restore the default PLL lock time.
        MemNSetBitFieldNb (NBPtr, RegPllLockTime, 0xC8);

        // 3. Program D18F2x9C_x0002_0000_dct[1:0][PllMultDiv]. PMU Reset must be high.
        Freq = (UINT16) MEMCLK_ID_TO_FREQ (MemNGetBitFieldNb (NBPtr, BFMemClkFreq)) * 2;
        PllMultDiv = ((Freq / 100) * 0x100) + ((Freq % 100) / 20);
        MemNSetBitFieldNb (NBPtr, RegPllMemPs0, PllMultDiv);

        // 4. Program D18F2x9C_x0002_0080_dct[1:0][PMUClkDiv].
        // PMU must operate no faster than 533MHz
        PMUClkDiv = CEIL_DIV (((CEIL_DIV (Freq, 533) < 1) ? 0 : CEIL_DIV (Freq, 533)), 2);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv),  PMUClkDiv);

        // 5. Program D18F2x9C_x0002_0001_dct[1:0][PllMultDiv] = If Ddr4ModeThen '1333 MT/s' else '667 MT/s'.
        MemNSetBitFieldNb (NBPtr, RegPllMemPs1, ((MemNGetBitFieldNb (NBPtr, BFDramType)) == DRAM_TYPE_DDR3_CZ) ? 0x0603 : 0x0D01);

        // 6. IF (Ddr3Mode) program D18F2x9C_x0102_0080_dct[1:0][PMUClkDiv].
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFPMUClkDiv), 1);
      }
    }
    MemNSwitchDCTNb  (NBPtr, 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function does the channel disable sequence
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3DisableChannelCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINTN bitfield;
  UINT32 RegVal;
  UINT8 DimmMask;
  UINT8 Die;
  UINT8 Dct;

  DimmMask = 0;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    // Function field contains the DCT number
    NBPtr->SwitchDCT (NBPtr, (UINT8) Address.Address.Function);
    //RegValue = MemNGetBitFieldNb (NBPtr, RegPwrStateCmd);

    for (bitfield = BFCSBaseAddr0Reg; bitfield <= BFCSBaseAddr3Reg; ++bitfield) {
      RegVal = MemNGetBitFieldNb (NBPtr, bitfield);
      if (RegVal & 0x1) {
        DimmMask |= (UINT8) (1 << (bitfield - BFCSBaseAddr0Reg));
      }
    }

    if (DimmMask == 0) {
      // Clear this bit before phy access
      MemNSetBitFieldNb (NBPtr, BFDphyMemPsSelEn, 0);

      // Tristate Vref on unused channel
      MemNSetBitFieldNb (NBPtr, BFGlobalVrefInSel, 2);

      // Tri-state drivers
      MemNSetBitFieldNb (NBPtr, RegTxImpedance, 0);

      // Program D18F2x9C_x00FA_F04A_dct[3:03:03:0] = 0080h.  Abyte/abit bc. Pwrdn Rcvrs used only in loopback mode
      MemNSetBitFieldNb (NBPtr, RegFAF04A, 0x80);

      // Phy LP2 state
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);

      //Disable powergating the ss channel when the Dcc is powergated
      MemNSetBitFieldNb (NBPtr, BFDisDllShutdownSR, 1);

      // Disable DCT
      MemNSetBitFieldNb (NBPtr, BFDisDramInterface, 1);

      // Clear ZqToCsMap as it defaults to 0000_AA55h
      MemNSetBitFieldNb (NBPtr, RegZqToCsMap, 0);
    }

    //Program D18F2x78_dct[1:0][DccPgSsBusDis]=1 on both DCT0 and DCT1.
    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);
      MemNSwitchDCTNb (NBPtr, Dct);
      MemNSetBitFieldNb (NBPtr, BFDccPgSsBusDis, 1);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function returns the conditional PCI device mask
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in, out]  *DescriptPtr - Pointer to DESCRIPTOR_GROUP
 *      @return         none
 */
VOID
STATIC
MemNS3GetConPCIMaskCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  )
{
  UINTN bitfield;
  UINT32 RegVal;
  UINT8 DCT;
  UINT8 DimmMask;
  UINT8 BadDimmMask;
  UINT8 NbPsCapMsk;
  UINT8 MemPstateMsk;
  UINT8 CsPerDelay;

  NbPsCapMsk = 0;
  MemPstateMsk = 0;
  DimmMask = 0;
  BadDimmMask = 0;
  CsPerDelay = 1;

  for (DCT = 0; DCT < NBPtr->DctCount; DCT ++) {
    MemNSwitchDCTNb (NBPtr, DCT);
    if (MemNGetBitFieldNb (NBPtr, BFMemClkFreqVal)) {
      if (MemNGetBitFieldNb (NBPtr, BFPerRankTimingEn) == 0) {
        CsPerDelay = 2;
      }
      for (bitfield = BFCSBaseAddr0Reg; bitfield <= BFCSBaseAddr3Reg; bitfield ++) {
        RegVal = MemNGetBitFieldNb (NBPtr, bitfield);
        if (RegVal & 0x1) {
          DimmMask |= (UINT8) (1 << ((((bitfield - BFCSBaseAddr0Reg) / CsPerDelay) << 1) + DCT));
        } else if (RegVal & 0x4) {
          BadDimmMask |= (UINT8) (1 << ((((bitfield - BFCSBaseAddr0Reg) / CsPerDelay) << 1) + DCT));
        }
      }
    }
  }

  IDS_OPTION_HOOK (IDS_OVERRIDE_DIMM_MASK, &DimmMask, &(NBPtr->MemPtr->StdHeader));

  // Check if the system is capable of doing NB Pstate change
  if (MemNGetBitFieldNb (NBPtr, BFNbPstateDis) == 0) {
    NbPsCapMsk = DCT0_NBPSTATE_SUPPORT_MASK;
  }
  if (MemNGetBitFieldNb (NBPtr, BFMemPstateDis) == 0) {
    MemPstateMsk = DCT0_MEMPSTATE_MASK;
  }

  MemNSwitchDCTNb (NBPtr, 0);
  // Set channel mask
  DescriptPtr->CPCIDevice[PRESELFREF].Mask1 = 0;
  DescriptPtr->CPCIDevice[POSTSELFREF].Mask1 = 0;
  for (DCT = 0; DCT < NBPtr->DctCount; DCT ++) {
    if (DimmMask & (0x55 << DCT)) {
      // Set mask before exit self refresh
      DescriptPtr->CPCIDevice[PRESELFREF].Mask1 |= (NbPsCapMsk | MemPstateMsk | 1) << DCT;
      // Set mask after exit self refresh
      DescriptPtr->CPCIDevice[POSTSELFREF].Mask1 |= (NbPsCapMsk | MemPstateMsk | 1) << DCT;
    } else if (BadDimmMask & (0x55 << DCT)) {
      // Need to save function 2 registers for bad dimm
      DescriptPtr->CPCIDevice[PRESELFREF].Mask1 |= 1 << DCT;
    }
  }

  // Set dimm mask
  DescriptPtr->CPCIDevice[PRESELFREF].Mask2 = DimmMask;
  DescriptPtr->CPCIDevice[POSTSELFREF].Mask2 = DimmMask;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function read the value of CSR register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetCSRCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT32 ExtendOffset;
  UINT32 TempFunc;

  ExtendOffset = Address.Address.Register;
  TempFunc = Address.Address.Function;

  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC);
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  Address.Address.Function = TempFunc;
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, TempValue);

  Address.Address.Register = 0x98;
  ExtendOffset &= 0x3FF;
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &ExtendOffset, ConfigPtr);
  IDS_OPTION_HOOK (IDS_AFTER_DCT_PHY_ACCESS, NULL, ConfigPtr);
  Address.Address.Register = 0x9C;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F2_9C_%x = %x\n", Address, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write to a CSR register
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetCSRCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT32 ExtendOffset;
  UINT32 ValueWrite;
  UINT32 TempFunc;

  ExtendOffset = Address.Address.Register;

  TempFunc = Address.Address.Function;

  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC);
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, TempValue);

  Address.Address.Function = TempFunc;
  Address.Address.Register = 0x9C;

  ExtendOffset &= 0x3FF;
  ExtendOffset |= 0x40000000;
  switch (AccessWidth) {
  case AccessS3SaveWidth8:
    ValueWrite = *(UINT8 *) Value;
    break;
  case AccessS3SaveWidth16:
    ValueWrite = *(UINT16 *) Value;
    break;
  case AccessS3SaveWidth32:
    ValueWrite = *(UINT32 *) Value;
    break;
  default:
    ASSERT (FALSE);
  }
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &ValueWrite, ConfigPtr);
  Address.Address.Register = 0x98;
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &ExtendOffset, ConfigPtr);
  IDS_OPTION_HOOK (IDS_AFTER_DCT_PHY_ACCESS, NULL, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F2_9C_%x = %x\n", Address, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function reads and writes register bitfield
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in]   IsSet - if this is a register read or write
 *     @param[in, out]  *Value - Pointer to the value be read or  written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetSetBitFieldCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       BOOLEAN IsSet,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINTN BitField;
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT32 RegValue;
  UINT8 Die;
  UINT8 InstNum;
  UINT8 Byte;

  RegValue = 0;
  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    // Function field:
    //   Bit 0:     DCT number
    NBPtr->SwitchDCT (NBPtr, (UINT8) (Address.Address.Function & 1));

    // Bit/Register field:
    BitField = Address.Address.Register;

    ASSERT (MaxS3Field <= 16);  // Only 16 S3 fields are supported for chiplet select

    // Function field:
    //   Bit [2:1]: If non-zero, support SW broadcast for phy registers
    switch ((Address.Address.Function >> 1)) {
    case 0:
      // Non-chiplet-select registers, no translation needed
      break;
    case 1:
      // Select per NB Pstate
      InstNum = (UINT8) (BitField >> 4);
      BitField = PER_NB_PS (InstNum, S3FieldTransTab[BitField & 0xF]);
      break;
    case 2:
      // Select per CAD pad name
      InstNum = (UINT8) (BitField >> 4);
      BitField = PER_CAD_PAD (InstNum, ALL_PADS, S3FieldTransTab[BitField & 0xF]);
      break;
    case 3:
      InstNum = 0;
      switch ((BitField & 0xC00) >> 10) {
      case 0:
        InstNum = PAD_DQ;
        break;
      case 1:
        InstNum = PAD_DQS;
        break;
      case 2:
        InstNum = PAD_DQS_L;
        break;
      default:
        ASSERT (FALSE);
      }
      Byte = (UINT8) (BitField >> 4) & 0xF;
      if (Byte == 0xF) {
        Byte = ALL_BYTES;
      }

      BitField = PER_DATA_BYTE (InstNum, Byte, S3FieldTransTab[BitField & 0xF]);
      break;
    default:
      ASSERT (FALSE);
    }

    if (IsSet) {
      switch (AccessWidth) {
      case AccessS3SaveWidth8:
        RegValue = *(UINT8 *) Value;
        break;
      case AccessS3SaveWidth16:
        RegValue = *(UINT16 *) Value;
        break;
      case AccessS3SaveWidth32:
        RegValue = *(UINT32 *) Value;
        break;
      default:
        ASSERT (FALSE);
      }
      MemNSetBitFieldNb (NBPtr, BitField, RegValue);
    } else {
      RegValue = MemNGetBitFieldNb (NBPtr, BitField);

      switch (AccessWidth) {
      case AccessS3SaveWidth8:
        *(UINT8 *) Value = (UINT8) RegValue;
        break;
      case AccessS3SaveWidth16:
        *(UINT16 *) Value = (UINT16) RegValue;
        break;
      case AccessS3SaveWidth32:
        *(UINT32 *) Value = RegValue;
        break;
      default:
        ASSERT (FALSE);
      }
    }
  } else {
    ASSERT (FALSE);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function reads PHY register by instance
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetInstanceCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINTN BitField;
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT32 RegValue;
  UINT8 Die;

  RegValue = 0;
  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) != AGESA_SUCCESS) {
    ASSERT (FALSE);
  }

  S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
  NBPtr = S3NBPtr[Die].NBPtr;

  // Function field:
  //   Bit 0:     DCT number
  NBPtr->SwitchDCT (NBPtr, (UINT8) (Address.Address.Function & 1));

  // Bit/Register field:
  BitField = Address.Address.Register;

  RegValue = MemNGetBitFieldNb (NBPtr, PER_INSTANCE (BitField));

  switch (AccessWidth) {
  case AccessS3SaveWidth8:
    *(UINT8 *) Value = (UINT8) RegValue;
    break;
  case AccessS3SaveWidth16:
    *(UINT16 *) Value = (UINT16) RegValue;
    break;
  case AccessS3SaveWidth32:
    *(UINT32 *) Value = RegValue;
    break;
  default:
    ASSERT (FALSE);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function writes PHY register by instance
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetInstanceCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINTN BitField;
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT32 RegValue;
  UINT8 Die;

  RegValue = 0;
  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) != AGESA_SUCCESS) {
    ASSERT (FALSE);
  }

  S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
  NBPtr = S3NBPtr[Die].NBPtr;

  // Function field:
  //   Bit 0:     DCT number
  NBPtr->SwitchDCT (NBPtr, (UINT8) (Address.Address.Function & 1));

  // Bit/Register field:
  BitField = Address.Address.Register;

  switch (AccessWidth) {
  case AccessS3SaveWidth8:
    RegValue = *(UINT8 *) Value;
    break;
  case AccessS3SaveWidth16:
    RegValue = *(UINT16 *) Value;
    break;
  case AccessS3SaveWidth32:
    RegValue = *(UINT32 *) Value;
    break;
  default:
    ASSERT (FALSE);
  }

  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (BitField), RegValue);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function reads register bitfield
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetBitFieldCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MemNS3GetSetBitFieldCZ (AccessWidth, Address, FALSE, Value, ConfigPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function writes register bitfield
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetBitFieldCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MemNS3GetSetBitFieldCZ (AccessWidth, Address, TRUE, Value, ConfigPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function changes memory Pstate context
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 * ----------------------------------------------------------------------------
 */
VOID
STATIC
MemNS3ChangeMemPStateContextCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;
  UINT8 MemPstate;

  MemPstate = (UINT8) Address.Address.Register;

  ASSERT (MemPstate < 2);   // Only support 2 memory pstates

  IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, NULL, ConfigPtr) {
    // See which Node should be accessed
    Die = (UINT8) (Address.Address.Device - 24);

    LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
    if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
      S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
      NBPtr = S3NBPtr[Die].NBPtr;
      NBPtr->MemPstate = (MEM_PSTATE)MemPstate;
      IDS_HDT_CONSOLE (MEM_FLOW, "Changing Memory Pstate to %x\n", NBPtr->MemPstate);
    } else {
      IDS_HDT_CONSOLE (MEM_FLOW, "Can not find the S3 handle\n");
    }
  }

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write to a register that has one copy for each NB Pstate
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetNBPStateDepRegCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 NBPstate;
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  NBPstate = (UINT8) (Temp >> 10);
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 1);
  Temp &= 0x3FF;

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xC8) | ((NBPstate << 4) | Dct);
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Function = FUNC_2;
  Address.Address.Register = Temp;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);

  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  TempValue = 0;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write to a register that has one copy for each NB Pstate
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetNBPStateDepRegCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 NBPstate;
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  NBPstate = (UINT8) (Temp >> 10);
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 1);
  Temp &= 0x3FF;

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xCE) | ((NBPstate << 4) | Dct);
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Function = FUNC_2;
  Address.Address.Register = Temp;
  LibAmdPciWrite (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);

  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  TempValue = 0;
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function tri-state unused pads.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNS3TriStateUnusedPadCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  //===================================================================
  // Tristate DATA[8] if ECC is not supported
  //===================================================================
  if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state ECC Pad\n");
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDataTxImpedance), 0);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ECC_BYTE, RegDataTxImpedance), 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      Disable PMU on all DCTs
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNS3DisablePmuCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // Program D18F2x9C_x0002_0099_dct[3:0][PmuReset,PmuStall] = 1,1.
  MemNSetBitFieldNb (NBPtr, BFPmuStall, 1);
  MemNSetBitFieldNb (NBPtr, BFPmuReset, 1);

  // Program D18F2x9C_x0002_0099_dct[1:0][SRAM_SD] = 1
  MemNSetBitFieldNb (NBPtr, BFPmuSramSd, 1);

  // Program D18F2x9C_x0002_0080[3:0][PmuClkDiv]]=7 (PMU CLK turned off).
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv), 7);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     PHY Power Saving
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNS3PhyPowerSavingCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST UINT8 DLL_RESET_RELOCK_SHIFT = 7;
  INT8 MemPstate;
  UINT8 LowestMemPstate;

  LowestMemPstate = 1;

  //----------------------------------------------------------------
  //  Tri-State unused pads for Power Saving
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tS3 DRAM PHY Power Savings\n");
  for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
    // When memory pstate is enabled, this loop will goes through M1 first then M0
    // Otherwise, this loop only goes through M0.
    MemNS3SwitchMemPstateCZ (NBPtr, MemPstate);

    // 1. Program DctPhyA41 as follows to disable unused pads.
    // DrvStrenP = DrvStrenP = 0.
    // See 2.10.4. Software does this for each unconnected pad in the package or each pad
    // connected to unused pins
    MemNS3TriStateUnusedPadCZ (NBPtr);

    // 2. Program D18F2x9C_x0[3,1:0][F,3:0]0_0014_dct[1:0][MaxDurDllNoLock] = 0.
    // DctPhyA78[DllResetRelock] must be set to 1 prior to writing this register
    // and then DctPhyA78[DllResetRelock] must be cleared after the register
    MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, (1 << DLL_RESET_RELOCK_SHIFT));
    MemNSetBitFieldNb (NBPtr, BFCadMaxDurDllNoLock, 0);
    MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, 0);

    if (NBPtr->MemPstate == 1) {
      // 3. Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[1:0] as follows:
      // For M1 context program ODTStrenN = ODTStrenP = 0
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), 0);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), 0);
    }
  }

  // 4. Program D18F2x9C_x00[F,3:0]0_[F,B:0]04A_dct[1:0][PowerDownRcvr] = 1.
  MemNSetBitFieldNb (NBPtr, BFPowerDownRcvrAbyte, 1);

  // 6. Disable unused DLL components in ABYTE as follows:
  // Program D18F2x9C_x0020_0077_dct[] = 07C0h. Disable Tx DLL in Abyte 2.
  // Program D18F2x9C_x0030_0077_dct[] = 07C0h. Disable Tx DLL in Abyte 3.
  // Program D18F2x9C_x0000_0077_dct[] = 07C4h.
  // Program D18F2x9C_x0010_0077_dct[] = 07C4h.
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00200077), 0x0);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00300077), 0x0);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00000077), 0x4);
  if (MemNGetBitFieldNb (NBPtr, BFParEn) != 0) {
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00100077), 0x0);
  } else {
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00100077), 0x8);
  }

  // 7. If ECC memory is not connected or is disabled, or the package does not support ECC,
  // then power down data chiplet 8 with the following:
  // Write to all memory P-state instances if available.
  // Program D18F2x9C_x00[F,8:0]1_[F,B:0]04A_dct[1:0][PowerDownRcvr]=1
  // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[1:0] = 07CFh.  Turn off all DLLs
  // Program D18F2x9C_x00[F,8:0]1_0000_dct[1:0][DByteEnable]=1. Actually "disables"
  // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[1:0] = 07EFh.  Turn off XCLK
  // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[1:0] = 0000h.  Disable Odt
  // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]041_dct[1:0] = 0000h.  Disable output driver impedance
  if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, BFPowerDownRcvrDbyte), 1);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ECC_BYTE, BFPowerDownRcvrDbyte), 1);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07CF);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910000), 0x0004);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07DF);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F04D), 0x0000);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F041), 0x0000);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0091F04E), 0x0000);
  }

  // 8. Power down unused receivers in data chips as follows:
  // Program D18F2x9C_x00F1_904A_dct[1:0][PowerDownRcvr]=1. DBI.
  // If x4 DIMMs are not present then program D18F2x9C_x00F1_B04A_dct[1:0][PowerDownRcvr]=1.
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1904A), 0x0100);
  if (MemNGetBitFieldNb (NBPtr, BFX4Dimm) == 0) {
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1B04A), 0x0100);
  }

  // 9. Power down the PMU
  MemNS3DisablePmuCZ (NBPtr);

  // D18F2x9C_x0002_005D_dct[1:0][ForceHClk] = 0.
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegMisc5), 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function switches memory pstate for KV
 *
 *     @param[in,out]   *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in]       MemPstate - Mem Pstate
 *
 * ----------------------------------------------------------------------------
 */
VOID
STATIC
MemNS3SwitchMemPstateCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  )
{
  ASSERT (MemPstate < 2);   // Only support 2 memory pstates
  IDS_HDT_CONSOLE (MEM_FLOW, "MemPsSel = %d\n", MemPstate);
  NBPtr->MemPstate = (MEM_PSTATE) MemPstate;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function read the value of Function 2 PCI register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the NB register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SaveNBRegisterCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 1);

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC) | Dct;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Register = Temp;
  Address.Address.Function = FUNC_2;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function set the value of Function 2 PCI register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the NB register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be write.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3RestoreNBRegisterCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 1);

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC) | Dct;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Register = Temp;
  Address.Address.Function = FUNC_2;
  LibAmdPciWrite (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function is used by families that use a separate DctCfgSel bit to
 *     select the current DCT which will be accessed by function 2.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *Dct     - Pointer to ID of the target DCT
 *
 */

BOOLEAN
MemNS3DctCfgSelectCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  )
{
  //
  // Set the DctCfgSel to new DCT
  //
  MemNCmnGetSetFieldCZ (NBPtr, 1, BFDctCfgSel, *(UINT8 *) Dct);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Enable DDR3 PHY calibrating
 *
 *   This function determines if the PSP is present
 *
 */

VOID
STATIC
MemNS3EnablePhyCalibrationCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xA3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xB3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 0);
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x83);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Waits for PHY calibrating to complete
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNS3PendOnPhyCalibrateCompletionCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // read CalBusy and discard result - this is needed to latch the
  // initial 'busy' status
  MemNGetBitFieldNb (NBPtr, RegCalBusy);

  // Wait until D18F2x9C_x20097 == 0.
  MemNPollBitFieldNb (NBPtr, RegCalBusy, 0, PCI_ACCESS_TIMEOUT, FALSE);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Set the rate for PHY calibrate
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNS3RateOfPhyCalibrateCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalInitMode, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x03);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x13);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Configure disable DLL shutdown in self-refresh mode.
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 *
 */

VOID
MemNS3ConfigureDisDllShutdownSrCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    MemNBrdcstSetUnConditionalNb (NBPtr, BFDisDllShutdownSR, 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function determines if the PSP is present
 *
  *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *      @return         BOOLEAN
 *                         TRUE - PSP is present
 *                         FALSE - PSP is not present
 */
BOOLEAN
MemS3PspDetectionCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  return MemPspDetectionMechanismCZ (NBPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function determines whether it is a platform secure boot
 *
  *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *      @return         BOOLEAN
 *                         TRUE - PSP is present
 *                         FALSE - PSP is not present
 */
BOOLEAN
MemS3PspPlatformSecureBootEnCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  return MemPspPlatformSecureBootEnCZ (NBPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function read the value of D0F0 root complex register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
MemNSaveRootComplexCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  Address.AddressValue = Address.Address.Register;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write the value of D0F0 root complex register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
MemNRestoreRootComplexCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  Address.AddressValue = Address.Address.Register;
  LibAmdPciWrite (AccessWidth, Address, Value, ConfigPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function read the value of D0F0 root complex misc register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
MemNSaveRootComplexMiscCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT32 MiscIndex;

  MiscIndex = Address.Address.Register;
  Address.AddressValue = (UINT32)GNB_D0F0x60_ADDRESS;
  LibAmdPciWrite (AccessWidth, Address, &MiscIndex, ConfigPtr);
  Address.AddressValue = (UINT32)GNB_D0F0x64_DATA;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write the value of D0F0 root complex misc register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
MemNRestoreRootComplexMiscCZ (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT32 MiscIndex;

  MiscIndex = Address.Address.Register;
  Address.AddressValue = (UINT32)GNB_D0F0x60_ADDRESS;
  LibAmdPciWrite (AccessWidth, Address, &MiscIndex, ConfigPtr);
  Address.AddressValue = (UINT32)GNB_D0F0x64_DATA;
  LibAmdPciWrite (AccessWidth, Address, Value, ConfigPtr);
}
