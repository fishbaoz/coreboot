/* $NoKeywords:$ */
/**
 * @file
 *
 * Config KERN SD
 *
 * Init SD Controller.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 311871 $   @e \$Date: 2015-01-28 01:03:13 -0600 (Wed, 28 Jan 2015) $
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
#define FILECODE PROC_FCH_KERN_KERNSD_KERNSDENVSERVICE_FILECODE
/**
 * FchInitEnvSdProgram - Config SD controller before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSdProgram (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // SD Configuration
  //
  if ( (LocalCfgPtr->Sd.SdConfig != SdDisable) && (LocalCfgPtr->Sd.SdConfig != SdDump)) {
    ACPIMMIO8 (FCH_PMxE8_SDFlashCntrl) |= FCH_PMxE8_SDFlashCntrl_SdCd2Acpi; //Enable SD_CD0
    ACPIMMIO8 (FCH_IOMUXx66_SD0_PWR_CTRL_AGPIO102) = 0; //ENH448923
    ACPIMMIO32(FCH_MISCx68_MEMORY_POWER_SAVING_CONTROL) &= ~ BIT18; //SdBypassMemDsd

    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x40);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE8, AccessWidth8, 0xFE, BIT0);

    if (LocalCfgPtr->Sd.SdSsid != NULL ) {
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REG2C, AccessWidth32, 0, LocalCfgPtr->Sd.SdSsid, StdHeader);
    }

    RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0x3FFFFFFF, ((UINT32) (LocalCfgPtr->Sd.SdSlotType) << 30), StdHeader);

    if ( LocalCfgPtr->Sd.SdConfig == SdVer2) {
// SD 2.0
//The following programming sequence sets up SD controller to operate in SD 2.0 mode. (UBTS538428).
//A. Program D14F7xA4[BaseFreq]= 32h.
//B. Program D14F7xA8=[Ddr50Sup]=0.
//C. Program D14F7xA8=[Sdr100Sup]=0.
//D. Program D14F7xA8=[Sdr50Sup]=0.
//E. Program D14F7xB0=[HostVersionSel]=1h.
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, ~ (UINT32) (0xFF << 8), (UINT32) (0x32 << 8), StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, ~ (UINT32) (0x01 << 2), (UINT32) (0x00 << 2), StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, ~ (UINT32) (0x01 << 1), (UINT32) (0x00 << 1), StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, ~ (UINT32) (0x01 << 0), (UINT32) (0x00 << 0), StdHeader);
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, ~ (UINT32) (0xFF << 24), (UINT32) (0x01 << 24), StdHeader);
    } else {
      if (( LocalCfgPtr->Sd.SdConfig > SdVer2) && ( LocalCfgPtr->Sd.SdConfig < SdDump)) {
//SD3.0
//The following programming sequence sets up SD controller to operate in SD 3.0 mode. (UBTS538428)
//A. Program D14F7xA4[BaseFreq]= C8h.
//B. Program D14F7xA8=[Ddr50Sup]=1.
//C. Program D14F7xA8=[Sdr100Sup]=1.
//D. Program D14F7xA8=[Sdr50Sup]=1.
//E. Program D14F7xB0=[HostVersionSel]=2h.

        RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, ~ (UINT32) (0xFF << 8), (UINT32) (0xC8 << 8), StdHeader);
        if ( (LocalCfgPtr->Sd.SdConfig > SdVer3) && (LocalCfgPtr->Sd.SdConfig < SdDump) ) {
          if (LocalCfgPtr->Sd.SdConfig == SdV3DDR50) {
            RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, ~ (UINT32) (0x07 << 0), (UINT32) (0x01 << 2), StdHeader);
          }
          if (LocalCfgPtr->Sd.SdConfig == SdV3SDR104) {
            RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, ~ (UINT32) (0x07 << 0), (UINT32) (0x01 << 1), StdHeader);
          }
          if (LocalCfgPtr->Sd.SdConfig == SdV3SDR50) {
            RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, ~ (UINT32) (0x07 << 0), (UINT32) (0x01 << 0), StdHeader);
          }
        } else {
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, ~ (UINT32) (0x07 << 0), (UINT32) (0x07 << 0), StdHeader);
        }
        RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, ~ (UINT32) (0xFF << 24), (UINT32) (0x02 << 24), StdHeader);

      } else {
        if ( LocalCfgPtr->Sd.SdConfig == 15 ) {
          // SD AUTO 2.0 + Low speed
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0xC0000000, 0x21DE32B2, StdHeader);
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA8, AccessWidth32, 0, 0x00002570, StdHeader);
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, (UINT32) (BIT2), 0x01180C19, StdHeader);
          RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0, AccessWidth32, (UINT32) (BIT19 + BIT12), 0x000007CF, StdHeader);
        }
      }
    }
    if ( LocalCfgPtr->Sd.SdForce18 ) {
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGD0, AccessWidth32, ~ ((UINT32) (0x01 << 17)), ((UINT32) (0x01) << 17), StdHeader);
    }
    if (( LocalCfgPtr->Sd.SdForce18 ) || ( LocalCfgPtr->FchRunTime.FchDeviceEnableMap & BIT27)) {
      RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG78, AccessWidth32, ~(UINT32) BIT0, 0, StdHeader);
    } else {
      RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG78, AccessWidth32, ~(UINT32) BIT0, BIT0, StdHeader);
    }

    if (LocalCfgPtr->Sd.SdDbgConfig == 1) {
      //DMA clear BIT19,BIT20
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, ~ (UINT32) (BIT19 + BIT20), 0, StdHeader);
    }

    if (LocalCfgPtr->Sd.SdDbgConfig == 2) {
      //PIO clear BIT19,BIT20,BIT22
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, ~ (UINT32) (BIT19 + BIT20 + BIT22), 0, StdHeader);
    }

  } else {
    if (LocalCfgPtr->Sd.SdConfig == SdDisable) {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x00);
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE8, AccessWidth8, 0xFE, 0x00);
    }
  }
}
