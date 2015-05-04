/* $NoKeywords:$ */
/**
 * @file
 *
 * Config KERN AB
 *
 * Init AB bridge.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
/*
*****************************************************************************
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
****************************************************************************
*/
#include "FchPlatform.h"
#include "GeneralServices.h"
#include "KernFch.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_KERN_KERNPCIE_KERNABENVSERVICE_FILECODE

//
// Declaration of local functions
//
VOID AbCfgTbl (IN AB_TBL_ENTRY  *ABTbl, IN AMD_CONFIG_PARAMS *StdHeader);

/**
 * KernInitEnvAbTable - AB-Link Configuration Table for Kern
 *
 */
AB_TBL_ENTRY KernInitEnvAbTable[] =
{
  //
  // Controls the USB OHCI controller prefetch used for enhancing performance of ISO out devices.
  // Setting B-Link Prefetch Mode (ABCFG 0x80 [18:17] = 11)
  //
  {ABCFG, FCH_ABCFG_REG80, BIT0 + BIT17 + BIT18, BIT0 + BIT17 + BIT18},

  //
  // Enabled SMI ordering enhancement. ABCFG 0x90[21]
  // USB Delay A-Link Express L1 State. ABCFG 0x90[17]
  //
  // Wait for BKDG update {ABCFG, FCH_ABCFG_REG90, BIT21 + BIT17, BIT21 + BIT17},
  {ABCFG, FCH_ABCFG_REG90, BIT21, BIT21},

  //
  // Enabling Detection of Upstream Interrupts ABCFG 0x94 [20] = 1
  // ABCFG 0x94 [19:0] = cpu interrupt delivery address [39:20]
  //
  {ABCFG, FCH_ABCFG_REG94, BIT20, BIT20 + 0x00FEE},

  //
  // Programming cycle delay for AB and BIF clock gating
  // Enable the AB and BIF clock-gating logic.
  // Enable the A-Link int_arbiter enhancement to allow the A-Link bandwidth to be used more efficiently
  //
  // Wait for BKDG update {ABCFG, FCH_ABCFG_REG10054,  0x00FFFFFF, 0x010407FF},
  {ABCFG, FCH_ABCFG_REG10054,  0x00FFFFFF, 0x000007FF},
  //{ABCFG, FCH_ABCFG_REG98,  0x0003FF00, 0x00034700},
  // Wait for BKDG update {ABCFG, FCH_ABCFG_REG54,  0x00FF0000, 0x00040000},

  //
  // Host Outstanding Completion Clock Gating
  //
  {ABCFG, FCH_ABCFG_REG208,  0xFFFFFFEF, 0x00081000},

  //
  // SD ALink prefetch
  //
  {ABCFG, FCH_ABCFG_REG10060, 0xFBFFFFFF, 0x02000000},

  {ABCFG, FCH_ABCFG_REG10090, BIT16, BIT16},
  {ABCFG, 0, 0, (UINT8) 0xFF},                                                 /// This dummy entry is to clear ab index
  { (UINT8)0xFF, (UINT8)0xFF, (UINT8)0xFF, (UINT8)0xFF},
};

/**
 * FchInitEnvAbLinkInit - Set ABCFG registers before PCI
 * emulation.
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvAbLinkInit (
  IN  VOID     *FchDataPtr
  )
{
  UINT16                 AbTempVar;
  UINT8                  AbValue8;
  AB_TBL_ENTRY           *AbTblPtr;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // Set A-Link bridge access address.
  // This is an I/O address. The I/O address must be on 16-byte boundary.
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE0, AccessWidth32, 00, ALINK_ACCESS_INDEX);

  //
  // AB CFG programming
  //
  if ( LocalCfgPtr->Ab.SlowSpeedAbLinkClock ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG40, AccessWidth8, ~(UINT32) BIT1, BIT1);
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG40, AccessWidth8, ~(UINT32) BIT1, 0);
  }

  //
  // Read Arbiter address, Arbiter address is in PMIO 6Ch
  //
  ReadMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6C, AccessWidth16, &AbTempVar);
  /// Write 0 to enable the arbiter
  AbValue8 = 0;
  LibAmdIoWrite (AccessWidth8, AbTempVar, &AbValue8, StdHeader);

  AbTblPtr = (AB_TBL_ENTRY *) (&KernInitEnvAbTable[0]);
  AbCfgTbl (AbTblPtr, StdHeader);

//
// Need to check
//
  if ( LocalCfgPtr->Ab.ResetCpuOnSyncFlood ) {
    RwAlink (FCH_ABCFG_REG10050 | (UINT32) (ABCFG << 29), ~(UINT32) BIT2, BIT2, StdHeader);
  }


  if ( LocalCfgPtr->Ab.AbDmaMemoryWrtie3264B ) {
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 0), (UINT32) (0x0  << 0), StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 2), (UINT32) (0x1  << 2), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 0), (UINT32) (0x1  << 0), StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 2), (UINT32) 0x0, StdHeader);
  }



//
// A Clock Gate-OFF
//
  if ( LocalCfgPtr->Ab.ALinkClkGateOff ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2, AccessWidth8, 0xFE, BIT0);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04, AccessWidth32, 0xFFFEFFFF, BIT16);
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2, AccessWidth8, 0xFE, 0);
  }
//
// B Clock Gate-OFF
//
  if ( LocalCfgPtr->Ab.BLinkClkGateOff ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2, AccessWidth8, 0xFD, BIT1);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04, AccessWidth32, 0xFFFEFFFF, BIT16);
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2, AccessWidth8, 0xFD, 0);
  }
  if ((LocalCfgPtr->Ab.ALinkClkGateOff == 0) && (LocalCfgPtr->Ab.BLinkClkGateOff == 0)) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04, AccessWidth32, 0xFFFEFFFF, 0);
  }
//
// GPP and GFX Clock Request
// remove to enable pci-e device
//  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00, AccessWidth8, 0xF0, LocalCfgPtr->Ab.GppClockRequest0);
//  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00, AccessWidth8, 0x0F, LocalCfgPtr->Ab.GppClockRequest1);
//  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00 + 1, AccessWidth8, 0xF0, LocalCfgPtr->Ab.GppClockRequest2);
//  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG00 + 1, AccessWidth8, 0x0F, LocalCfgPtr->Ab.GppClockRequest3);
//  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG04, AccessWidth8, 0x0F, LocalCfgPtr->Ab.GfxClockRequest);

//
// AB Clock Gating
//
  if ( LocalCfgPtr->Ab.AbClockGating ) {
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 4), (UINT32) (0x1 << 4), StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 24), (UINT32) (0x1 << 24), StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 20), (UINT32) (0x1 << 20), StdHeader);
    RwAlink (FCH_ABCFG_REG10054 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x3 << 24), (UINT32) (0x3 << 24), StdHeader);
    RwAlink (FCH_ABCFG_REG10054 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 20), (UINT32) (0x1 << 20), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG10054 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 20), 0, StdHeader);
    RwAlink (FCH_ABCFG_REG10054 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x3 << 24), 0, StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 20), 0, StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 24), 0, StdHeader);
    RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 4), 0, StdHeader);
  }
//
// AB Memory Power Saving
//
  if ( LocalCfgPtr->Ab.AbMemoryPowerSaving ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68, AccessWidth8, 0xFB, 0x00);
    RwAlink (FCH_ABCFG_REG58 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 29), (UINT32) (0x1  << 29), StdHeader);
    RwAlink (FCH_ABCFG_REG58 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 31), (UINT32) (0x1  << 31), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG58 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x5 << 29), (UINT32) 0x0, StdHeader);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68, AccessWidth8, 0xFB, 0x04);
  }

//
// SBG Memory Power Saving
//
  if ( LocalCfgPtr->Ab.SbgMemoryPowerSaving ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68, AccessWidth8, 0xFD, 0x00);
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 7), (UINT32) (0x1  << 7), StdHeader);
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 9), (UINT32) (0x1  << 9), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 7), (UINT32) 0x0, StdHeader);
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 9), (UINT32) 0x0, StdHeader);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68, AccessWidth8, 0xFD, 0x02);
  }
//
// SBG Clock Gating
//
  if ( LocalCfgPtr->Ab.SbgClockGating ) {
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 4), (UINT32) (0x1  << 4), StdHeader);
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 15), (UINT32) (0x1  << 15), StdHeader);
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 22), (UINT32) (0x1  << 22), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 4), (UINT32) (0x0), StdHeader);
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 15), (UINT32) (0x0), StdHeader);
    RwAlink (FCH_ABCFG_REG208 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 22), (UINT32) (0x0), StdHeader);
  }
//
// XDMA DMA Write 16 byte Mode
//
  if ( LocalCfgPtr->Ab.XdmaDmaWrite16ByteMode ) {
    RwAlink (FCH_ABCFG_REG180 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 0), (UINT32) (0x1  << 0), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG180 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 0), (UINT32) (0x0), StdHeader);
  }
//
// XDMA Memory Power Saving
//
  if ( LocalCfgPtr->Ab.XdmaMemoryPowerSaving ) {
    RwAlink (FCH_ABCFG_REG184 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 2), (UINT32) (0x1  << 2), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG184 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 2), (UINT32) 0x0, StdHeader);
  }
//
// XDMA Pending NPR Threshold
//
  if ( LocalCfgPtr->Ab.XdmaPendingNprThreshold ) {
    AbValue8 = LocalCfgPtr->Ab.XdmaPendingNprThreshold;
    RwAlink (FCH_ABCFG_REG180 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x03 << 2), (UINT32) (AbValue8  << 2), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG180 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x03 << 2), (UINT32) (0x0), StdHeader);
  }
//
// XDMA DNCPL Order Dis
//
  if ( LocalCfgPtr->Ab.XdmaDncplOrderDis ) {
    RwAlink (FCH_ABCFG_REG180 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 5), (UINT32) (0x1  << 5), StdHeader);
  } else {
    RwAlink (FCH_ABCFG_REG180 | (UINT32) (ABCFG << 29), ~ (UINT32) (0x1 << 5), (UINT32) (0x0), StdHeader);
  }

}

/**
 * AbCfgTbl - Program ABCFG by input table.
 *
 *
 * @param[in] ABTbl  ABCFG config table.
 * @param[in] StdHeader
 *
 */
VOID
AbCfgTbl (
  IN  AB_TBL_ENTRY     *ABTbl,
  IN AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32   AbValue;

  while ( (ABTbl->RegType) != 0xFF ) {
    if ( ABTbl->RegType == AXINDC ) {
      AbValue = 0x30 | (ABTbl->RegType << 29);
      WriteAlink (AbValue, (ABTbl->RegIndex & 0x00FFFFFF), StdHeader);
      AbValue = 0x34 | (ABTbl->RegType << 29);
      WriteAlink (AbValue, ((ReadAlink (AbValue, StdHeader)) & (0xFFFFFFFF^ (ABTbl->RegMask))) | ABTbl->RegData, StdHeader);
    } else if ( ABTbl->RegType == AXINDP ) {
      AbValue = 0x38 | (ABTbl->RegType << 29);
      WriteAlink (AbValue, (ABTbl->RegIndex & 0x00FFFFFF), StdHeader);
      AbValue = 0x3C | (ABTbl->RegType << 29);
      WriteAlink (AbValue, ((ReadAlink (AbValue, StdHeader)) & (0xFFFFFFFF^ (ABTbl->RegMask))) | ABTbl->RegData, StdHeader);
    } else {
      AbValue = ABTbl->RegIndex | (ABTbl->RegType << 29);
      WriteAlink (AbValue, ((ReadAlink (AbValue, StdHeader)) & (0xFFFFFFFF^ (ABTbl->RegMask))) | ABTbl->RegData, StdHeader);
    }

    ++ABTbl;
  }

  //
  //Clear ALink Access Index
  //
  AbValue = 0;
  LibAmdIoWrite (AccessWidth32, ALINK_ACCESS_INDEX, &AbValue, StdHeader);
}

/**
 * Is UMI One Lane GEN1 Mode?
 *
 *
 * @retval  TRUE or FALSE
 *
 */
BOOLEAN
IsUmiOneLaneGen1Mode (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  return (TRUE);
}
