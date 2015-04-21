/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch Sata controller
 *
 * Init Sata Controller features (PEI phase).
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 314764 $   @e \$Date: 2015-03-16 14:59:11 +0800 (Mon, 16 Mar 2015) $
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
#include "Ids.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_KERN_KERNSATA_KERNSATARESETSERVICE_FILECODE

/**
 * FchInitResetSataProgram - Config Sata controller during
 * Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetSataProgram (
  IN  VOID     *FchDataPtr
  )
{
  UINT8        FchSataMode;
  UINT8        FchSataClkMode;
  UINT32       Bar5;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  Bar5 = 0xFEC01000;

  //
  //New structure need calculate Sata Register value
  //
  FchSataMode = 0;
  if ( LocalCfgPtr->FchReset.SataEnable ) {
    FchSataMode |= 0x01;
  }
  if ( LocalCfgPtr->SataSetMaxGen2 ) {
    FchSataMode |= 0x04;
  }

  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REGA0), AccessWidth8, ~(UINT32) (BIT2 + BIT3 + BIT4 + BIT5 + BIT6), 0, StdHeader);
  FchSataClkMode = LocalCfgPtr->SataClkMode;
  //
  // Sata Setting for clock mode only
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth8, 0, FchSataMode);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, 0xFFFFFFFD, 0x00, StdHeader); // PHY in offline mode
  if ( FchSataClkMode != 9 ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth8, 0x0F, (FchSataClkMode << 4));
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth8, 0x0F, (0x01 << 4));
  }
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84 + 3), AccessWidth8,  ~(UINT32) BIT2, BIT2, StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, 0xFFFFFFFD, 0x02, StdHeader); // Restore PHY in offline mode
  //
  // For Kern design (reference board) is connected both external and internal clock.
  // Sata clock mode will set by AGESA FCH (FCH_RESET_DATA_BLOCK) SataClkMode.
  //
  switch ( FchSataClkMode ) {
  case 0:
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG8C), AccessWidth32, 0xFFFFFF00, 0xF0, StdHeader);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG1C, AccessWidth32, (UINT32) (~(1 << 20)), (UINT32) 0);
    break;
  case 1:
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG8C), AccessWidth32, 0xFFFFFF00, 0x7D, StdHeader);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG1C, AccessWidth32, (UINT32) (~(1 << 20)), (UINT32) 0);
    break;
  case 9:
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG8C), AccessWidth32, 0xFFFFFF00, 0x3C, StdHeader);
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG1C, AccessWidth32, (UINT32) (~(1 << 20)), (UINT32) (0x1 << 20));
    break;
  }
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDC + 2, AccessWidth8, 0xFE, 0x01);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, 0xFFFFFFFB, 0x00, StdHeader);
  // for H/W emulator FchStall (1000, StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, 0xFFFFFFFF, 0x04, StdHeader);
  if ( LocalCfgPtr->FchReset.SataEnable ) {
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, ~ (UINT32) (BIT2), (UINT32) BIT2, StdHeader);
    IEM_SKIP_CODE (IEM_WAIT) {
      FchStall (100, StdHeader);
    }
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, ~ (UINT32) (BIT2), 0, StdHeader);

    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, 0xFDFFFCFE, 0x02001003, StdHeader); //ENH456821
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x3 << 1)), (UINT32) (0x00 << 1), StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, 0x00, 0xC0070800, StdHeader);
//    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x3 << 1)), (UINT32) (0x01 << 1), StdHeader);
//    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, 0x0020000, 0x2188, StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x3 << 1)), (UINT32) (0x02 << 1), StdHeader);
//PCI cfg 0x48-2[23] =0
//PMIO register 0xDC [23]=1
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, (UINT32) (~ (0x1 << 23)), (UINT32) (0x00 << 23), StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, (UINT32) (~ (0x1 << 15)), (UINT32) (0x01 << 15), StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, (UINT32) (~ (0x1 << 25)), (UINT32) (0x01 << 25), StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, (UINT32) (~ (0x1 << 26)), (UINT32) (0x01 << 26), StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, (UINT32) (~ (0x1 << 28)), (UINT32) (0x01 << 28), StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, (UINT32) (~ (0x1 << 29)), (UINT32) (0x01 << 29), StdHeader);
    ACPIMMIO32 (FCH_PMxDC_SataConfig2) |= BIT23;
    //RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x1 << 13)), (UINT32) (0x01 << 13), StdHeader);
  }
  RwMem (ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_REG5E, AccessWidth8, 0xF7, AOAC_PWR_ON_DEV);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG88), AccessWidth32, 0x0FFFFFFF, 0x20000000, StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG24), AccessWidth32, 0x0, Bar5, StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG04), AccessWidth32, 0xFFFFFFFF, 0x07, StdHeader);
  RwMem ((Bar5 + FCH_SATA_BAR5_REG04), AccessWidth32, ~(UINT32) (BIT0), BIT0);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG04), AccessWidth32, 0xFFFFFFFF, 0x00, StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG24), AccessWidth32, 0x0, 0x0, StdHeader);

}


