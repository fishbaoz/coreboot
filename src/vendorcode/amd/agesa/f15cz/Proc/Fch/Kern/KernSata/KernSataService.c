/* $NoKeywords:$ */
/**
 * @file
 *
 * Graphics Controller family specific service procedure
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 313246 $   @e \$Date: 2015-02-19 02:11:27 +0800 (Thu, 19 Feb 2015) $
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
* ***************************************************************************
*/


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "FchPlatform.h"
#include "GeneralServices.h"
#include "KernFch.h"
#include "Ids.h"
#include  "Filecode.h"
#define FILECODE PROC_FCH_KERN_KERNSATA_KERNSATASERVICE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
UINT8  NumOfSataPorts = 2;

/**
 * FchInitMidProgramSataRegs - Sata Pci Configuration Space
 * register setting
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitMidProgramSataRegs (
  IN VOID  *FchDataPtr
  )
{
  UINT8        FchSataMsiCapability;
  UINT8        FchSataTargetSupport8Device;
  UINT8        FchSataDisableGenericMode;
  UINT8        FchSataPhyPllShutDown;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  FchSataMsiCapability = (UINT8) LocalCfgPtr->Sata.SataMsiCapability;
  FchSataTargetSupport8Device = (UINT8) LocalCfgPtr->Sata.SataTargetSupport8Device;
  FchSataDisableGenericMode = (UINT8) LocalCfgPtr->Sata.SataDisableGenericMode;
  // FchSataPhyPllShutDown = (UINT8) LocalCfgPtr->Sata.SataPhyPllShutDown;
  FchSataPhyPllShutDown = 0;

  if ((LocalCfgPtr->Sata.SataClass == SataNativeIde) || (LocalCfgPtr->Sata.SataClass == SataLegacyIde)) {
    FchSataMsiCapability = 0;
  }
  //
  // Enabled SATA MSI capability
  // SATA MSI and D3 Power State Capability MMC 0x2
  //
  if ( FchSataMsiCapability ) {
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG60 + 1), AccessWidth8, 0, 0x70, StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG70 + 1), AccessWidth8, 0, 0x50, StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG50 + 2), AccessWidth8, 0xF1, 0x06, StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG50 + 1), AccessWidth8, 0, 0, StdHeader);
  } else {
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG60 + 1), AccessWidth8, 0, 0x70, StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG70 + 1), AccessWidth8, 0, 0x00, StdHeader);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG50 + 2), AccessWidth8, 0xF1, 0x00, StdHeader);
  }
  //
  // Sata Target Support 8 devices function
  //
  if ( FchSataTargetSupport8Device ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth16, ~(UINT32) BIT12, BIT12);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth16, ~(UINT32) BIT12, 0x00);
  }

  //
  // Sata Generic Mode setting
  //
  if ( FchSataDisableGenericMode ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth16, ~(UINT32) BIT13, BIT13);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth16, ~(UINT32) BIT13, 0x00);
  }

  //
  // Sata Phy Pll Shutdown setting
  //
  if ( FchSataPhyPllShutDown ) {
    RwPci (((SATA_BUS_DEV_FUN << 16) + 0x87), AccessWidth8, ~(UINT32) (BIT6), BIT6, StdHeader);
  } else {
    RwPci (((SATA_BUS_DEV_FUN << 16) + 0x87), AccessWidth8, ~(UINT32) (BIT6), 0x00, StdHeader);
  }

  //RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, (UINT32) (~ (0xF8 << 26)), (UINT32) (0xF8 << 26), StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, (UINT32) (~ (0x01 << 31)), (UINT32) (0x00 << 31), StdHeader);

  //RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, (UINT32) (~ (0x01 << 30)), (UINT32) (0x00 << 30), StdHeader);

  //
  // OOB Detection Enhancement
  //
  if ( LocalCfgPtr->Sata.SataOobDetectionEnh ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDC + 1, AccessWidth8, 0xFE, 0x01);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDC + 1, AccessWidth8, 0xFE, 0);
  }

  //
  // E-Sata Power Saving Enhancement
  //
  //if ( LocalCfgPtr->Sata.SataPowerSavingEnh ) {
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C + 3), AccessWidth8, ~(UINT32) (BIT5), BIT5, StdHeader);
  //} else {
  //  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C + 3), AccessWidth8, ~(UINT32) (BIT5), 0, StdHeader);
  //}

  //
  // Sata Memory Power Saving
  //
  switch ( LocalCfgPtr->Sata.SataMemoryPowerSaving ) {
  case 0:
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68 + 2, AccessWidth8, 0xFD, 0);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, (UINT32) (~ (0x801)), (UINT32) 0x0001, StdHeader);
    break;
  case 1:
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68 + 2, AccessWidth8, 0xFD, 0);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, (UINT32) (~ (0x801)), (UINT32) 0x0800, StdHeader);
    break;
  case 2:
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68 + 2, AccessWidth8, 0xFD, 0);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, (UINT32) (~ (0x801)), (UINT32) 0x0801, StdHeader);
    break;
  case 3:
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x68 + 2, AccessWidth8, 0xFD, BIT1);
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, (UINT32) (~ (0x801)), (UINT32) 0x0000, StdHeader);
  }
}


/**
 * FchInitLateProgramSataRegs - Sata Pci Configuration Space
 * register setting
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateProgramSataRegs (
  IN VOID  *FchDataPtr
  )
{
  UINT8        PortNumByte;
  UINT32       Bar5;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  SataBar5setting (LocalCfgPtr, &Bar5);
  //
  //Clear error status
  //
  RwMem ((Bar5 + FCH_SATA_BAR5_REG130), AccessWidth32, 0xFFFFFFFF, 0xFFFFFFFF);
  RwMem ((Bar5 + FCH_SATA_BAR5_REG1B0), AccessWidth32, 0xFFFFFFFF, 0xFFFFFFFF);

  for ( PortNumByte = 0; PortNumByte < NumOfSataPorts; PortNumByte++ ) {
    RwMem ((Bar5 + 0x110 + (PortNumByte * 0x80)), AccessWidth32, 0xFFFFFFFF, 0x00);
  }
  if ( LocalCfgPtr->Sata.SataDevSlpPort0 ) {
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_GPIO_REG67, AccessWidth8, 0, 0x01);
    RwMem (ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_10C_GPIO55_AGPI067 + 2, AccessWidth8, (UINT32) (~ (FCH_GPIO_OUTPUT_ENABLE)), FCH_GPIO_OUTPUT_ENABLE);
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF4), AccessWidth32, 0xFFFFFEEF, BIT4 + BIT8);
    ACPIMMIO32 (FCH_GPIOx10C_DEVSLP0_EGPIO67) &= ~ (BIT22);
  } else {
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_GPIO_REG67, AccessWidth8, 0, 0x00);
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF4), AccessWidth32, 0xFFFFFEFF, 0);
    if ( !LocalCfgPtr->Sata.SataDevSlpPort1 ) {
      RwMem ((Bar5 + FCH_SATA_BAR5_REGF4), AccessWidth32, 0xFFFFFFEF, 0);
    }
  }
  if ( LocalCfgPtr->Sata.SataDevSlpPort1 ) {
    RwMem (ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_118_GPIO59_AGPI070 + 2, AccessWidth8, (UINT32) (~ (FCH_GPIO_OUTPUT_ENABLE)), FCH_GPIO_OUTPUT_ENABLE);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_GPIO_REG70, AccessWidth8, 0, 0x01);
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF4), AccessWidth32, 0xFFFFFDEF, BIT4 + BIT9);
  } else {
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_GPIO_REG70, AccessWidth8, 0, 0x00);
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF4), AccessWidth32, 0xFFFFFDFF, 0);
    if ( !LocalCfgPtr->Sata.SataDevSlpPort0 ) {
      RwMem ((Bar5 + FCH_SATA_BAR5_REGF4), AccessWidth32, 0xFFFFFFEF, 0);
    }
  }
  if ( LocalCfgPtr->Sata.SataAhciDisPrefetchFunction ) {
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x01 << 13)), (UINT32) (0x01 << 13), StdHeader);
  } else {
    RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth32, (UINT32) (~ (0x01 << 13)), 0, StdHeader);
  }
}

/**
 * sataBar5RegSet - Sata Bar5 register setting
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
SataBar5RegSet (
  IN  VOID     *FchDataPtr
  )
{
  UINT32       AndMaskDword;
  UINT32       OrMaskDword;
  UINT32       Bar5;
  UINT8        EfuseByte;
  UINT8        FchSataAggrLinkPmCap;
  UINT8        FchSataPortMultCap;
  UINT8        FchSataPscCap;
  UINT8        FchSataSscCap;
  UINT8        FchSataFisBasedSwitching;
  UINT8        FchSataCccSupport;
  UINT8        FchSataAhciEnclosureManagement;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  FchSataAggrLinkPmCap = (UINT8) LocalCfgPtr->Sata.SataAggrLinkPmCap;
  FchSataPortMultCap = (UINT8) LocalCfgPtr->Sata.SataPortMultCap;
  FchSataPscCap = (UINT8) LocalCfgPtr->Sata.SataPscCap;
  FchSataSscCap = (UINT8) LocalCfgPtr->Sata.SataSscCap;
  FchSataFisBasedSwitching = (UINT8) LocalCfgPtr->Sata.SataFisBasedSwitching;
  FchSataCccSupport = (UINT8) LocalCfgPtr->Sata.SataCccSupport;
  FchSataAhciEnclosureManagement = (UINT8) LocalCfgPtr->Sata.SataAhciEnclosureManagement;

  AndMaskDword = 0;
  OrMaskDword = 0;
  Bar5 = 0;

  SataBar5setting (LocalCfgPtr, &Bar5);
  EfuseByte = SATA_FIS_BASE_EFUSE_LOC;
  // GetEfuseStatus (&EfuseByte, LocalCfgPtr->StdHeader); ** Wait for BKDG update **

  if ( !FchSataPortMultCap ) {
    AndMaskDword |= BIT12;
  }

  if ( FchSataFisBasedSwitching ) {
    if (EfuseByte & BIT1) {
      AndMaskDword |= BIT10;
    } else {
      OrMaskDword |= BIT10;
    }
  } else {
    AndMaskDword |= BIT10;
  }

  if ( FchSataAggrLinkPmCap ) {
    OrMaskDword |= BIT11;
  } else {
    AndMaskDword |= BIT11;
  }

  if ( FchSataPscCap ) {
    OrMaskDword |= BIT1;
  } else {
    AndMaskDword |= BIT1;
  }

  if ( FchSataSscCap ) {
    OrMaskDword |= BIT26;
  } else {
    AndMaskDword |= BIT26;
  }

  //
  // Disabling CCC (Command Completion Coalescing) support.
  //
  if ( FchSataCccSupport ) {
    OrMaskDword |= BIT19;
  } else {
    AndMaskDword |= BIT19;
  }

  if ( FchSataAhciEnclosureManagement ) {
    OrMaskDword |= BIT27;
  } else {
    AndMaskDword |= BIT27;
  }

  RwMem ((Bar5 + FCH_SATA_BAR5_REGFC), AccessWidth32, ~AndMaskDword, OrMaskDword);

  //
  // SATA ESP port setting
  // These config bits are set for SATA driver to identify which ports are external SATA ports and need to
  // support hotplug. If a port is set as an external SATA port and need to support hotplug, then driver will
  // not enable power management (HIPM & DIPM) for these ports.
  //
  if ( LocalCfgPtr->Sata.SataEspPort.SataPortReg != 0 ) {
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF8), AccessWidth32, ~(LocalCfgPtr->Sata.SataEspPort.SataPortReg), 0);
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF8), AccessWidth32, 0xFF00FF00, (LocalCfgPtr->Sata.SataEspPort.SataPortReg << 16));
    //
    // External SATA Port Indication Registers
    // If any of the ports was programmed as an external port, HCAP.SXS should also be set
    //
    RwMem ((Bar5 + FCH_SATA_BAR5_REGFC), AccessWidth32, ~(UINT32) (BIT20), BIT20);
  } else {
    //
    // External SATA Port Indication Registers
    // If any of the ports was programmed as an external port, HCAP.SXS should also be set (Clear for no ESP port)
    //
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF8), AccessWidth32, 0xFF00FF00, 0x00);
    RwMem ((Bar5 + FCH_SATA_BAR5_REGFC), AccessWidth32, ~(UINT32) (BIT20), 0x00);
  }

  if ( FchSataFisBasedSwitching ) {
    if (EfuseByte & BIT1) {
      RwMem ((Bar5 + FCH_SATA_BAR5_REGF8), AccessWidth32, 0x00FFFFFF, 0x00);
    } else {
      RwMem ((Bar5 + FCH_SATA_BAR5_REGF8), AccessWidth32, 0x00FFFFFF, 0x03000000);
    }
  } else {
    RwMem ((Bar5 + FCH_SATA_BAR5_REGF8), AccessWidth32, 0x00FFFFFF, 0x00);
  }

  if ( LocalCfgPtr->Sata.BiosOsHandOff == 1 ) {
    RwMem ((Bar5 + FCH_SATA_BAR5_REG24), AccessWidth8, ~(UINT32) BIT0, BIT0);
  } else {
    RwMem ((Bar5 + FCH_SATA_BAR5_REG24), AccessWidth8, ~(UINT32) BIT0, 0x00);
  }
}

/**
 * FchSataSetDeviceNumMsi - Program Sata controller support
 * device number cap & MSI cap
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchSataSetDeviceNumMsi (
  IN  VOID     *FchDataPtr
  )
{
  // Leave as default for BU....
}


/**
 * FchSataDriveDetection - Sata drive detection
 *
 *   - Sata Ide & Sata Ide to Ahci only
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 * @param[in] *Bar5Ptr Sata BAR5 base address.
 *
 */
VOID
FchSataDriveDetection (
  IN  VOID       *FchDataPtr,
  IN  UINT32     *Bar5Ptr
  )
{
  UINT32       SataBarInfo;
  UINT8        PortNumByte;
  UINT8        SataPortType;
  UINT16       IoBaseWord;
  UINT32       SataLoopVarDWord;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  // For H/W emulator, support only Port 0
  for ( PortNumByte = 0; PortNumByte < 1; PortNumByte++ ) {

    ReadMem (*Bar5Ptr + FCH_SATA_BAR5_REG128 + PortNumByte * 0x80, AccessWidth32, &SataBarInfo);

    if ( ( SataBarInfo & 0x0F ) == 0x03 ) {
      if ( PortNumByte & BIT0 ) {
        //
        //this port belongs to secondary channel
        //
        ReadPci (((UINT32) (SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG18), AccessWidth16, &IoBaseWord, StdHeader);
      } else {
        //
        //this port belongs to primary channel
        //
        ReadPci (((UINT32) (SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG10), AccessWidth16, &IoBaseWord, StdHeader);
      }

      //
      //if legacy ide mode, then the bar registers don't contain the correct values. So we need to hardcode them
      //
      if ( LocalCfgPtr->Sata.SataClass == SataLegacyIde ) {
        IoBaseWord = ( (0x170) | ((UINT16) ( (~((UINT8) (PortNumByte & BIT0) << 7)) & 0x80 )) );
      }

      if ( PortNumByte & BIT1 ) {
        //
        //this port is slave
        //
        SataPortType = 0xB0;
      } else {
        //
        //this port is master
        //
        SataPortType = 0xA0;
      }

      IoBaseWord &= 0xFFF8;
      LibAmdIoWrite (AccessWidth8, IoBaseWord + 6, &SataPortType, StdHeader);

      //
      //Wait in loop for 30s for the drive to become ready
      //
      for ( SataLoopVarDWord = 0; SataLoopVarDWord < 300000; SataLoopVarDWord++ ) {
        LibAmdIoRead (AccessWidth8, IoBaseWord + 7, &SataPortType, StdHeader);
        if ( (SataPortType & 0x88) == 0 ) {
          break;
        }
        IEM_SKIP_CODE (IEM_WAIT) {
          FchStall (100, StdHeader);
        }
      }
    }
  }
}

/**
 * FchShutdownUnconnectedSataPortClock - Shutdown unconnected
 * Sata port clock
 *
 *   - Sata Ide & Sata Ide to Ahci only
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 * @param[in] Bar5 Sata BAR5 base address.
 *
 */
VOID
FchShutdownUnconnectedSataPortClock (
  IN  VOID       *FchDataPtr,
  IN  UINT32     Bar5
  )
{
  UINT8                  PortNumByte;
  UINT8                  PortSataStatusByte;
  UINT8                  NumOfPorts;
  UINT8                  FchSataClkAutoOff;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8                  SaveLocation;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  FchSataClkAutoOff = (UINT8) LocalCfgPtr->Sata.SataClkAutoOff;

  NumOfPorts = 0;
  //
  // Enable SATA auto clock control by default
  //
  if ( FchSataClkAutoOff ) {
    if ((ReadFchSleepType (StdHeader) != ACPI_SLPTYP_S3)) {
      for ( PortNumByte = 0; PortNumByte < NumOfSataPorts; PortNumByte++ ) {
        ReadMem (Bar5 + FCH_SATA_BAR5_REG128 + (PortNumByte * 0x80), AccessWidth8, &PortSataStatusByte);
        //
        // Shutdown the clock for the port and do the necessary port reporting changes.
        // Error port status should be 1 not 3
        //
        if ( ((PortSataStatusByte & 0x0F) != 0x03) && (! ((LocalCfgPtr->Sata.SataEspPort.SataPortReg) & (1 << PortNumByte))) ) {
          RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40 + 2), AccessWidth8, 0xFF, (1 << PortNumByte), StdHeader);
          RwMem (Bar5 + FCH_SATA_BAR5_REG0C, AccessWidth8, ~(1 << PortNumByte), 00);
        }
      }                                            ///end of for (PortNumByte=0;PortNumByte<6;PortNumByte++)
      SaveLocation = 0;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG72, &SaveLocation, StdHeader);
      ReadPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40 + 2), AccessWidth8, &PortSataStatusByte, StdHeader);
      SaveLocation = 1;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG73, &PortSataStatusByte, StdHeader);
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG72, &SaveLocation, StdHeader);
      ReadMem (Bar5 + FCH_SATA_BAR5_REG0C, AccessWidth8, &PortSataStatusByte);
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG73, &PortSataStatusByte, StdHeader);
    } else {
      SaveLocation = 0;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG72, &SaveLocation, StdHeader);
      LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG73, &PortSataStatusByte, StdHeader);
      WritePci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40 + 2), AccessWidth8, &PortSataStatusByte, StdHeader);
      SaveLocation = 1;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG72, &SaveLocation, StdHeader);
      LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG73, &PortSataStatusByte, StdHeader);
      RwMem (Bar5 + FCH_SATA_BAR5_REG0C, AccessWidth8, 0, PortSataStatusByte);
    }
  }

  ReadMem (Bar5 + FCH_SATA_BAR5_REG0C, AccessWidth8, &PortSataStatusByte);

  //
  //if all ports are in disabled state, report at least one port
  //
  if ( (PortSataStatusByte & 0xFF) == 0) {
    RwMem (Bar5 + FCH_SATA_BAR5_REG0C, AccessWidth8, ~(UINT32) (0xFF), 01);
  }

  ReadMem (Bar5 + FCH_SATA_BAR5_REG0C, AccessWidth8, &PortSataStatusByte);

  for (PortNumByte = 0; PortNumByte < NumOfSataPorts; PortNumByte ++) {
    if (PortSataStatusByte & (1 << PortNumByte)) {
      NumOfPorts++;
    }
  }

  if ( NumOfPorts == 0) {
    NumOfPorts = 0x01;
  }

  RwMem (Bar5 + FCH_SATA_BAR5_REG00, AccessWidth8, 0xE0, NumOfPorts - 1);
}

/**
 * FchSataSetPortGenMode - Set Sata port mode (each) for
 * Gen1/Gen2/Gen3
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchSataSetPortGenMode (
  IN  VOID     *FchDataPtr
  )
{
  UINT32       Bar5;
  UINT8        PortNumByte;
  UINT8        PortModeByte;
  UINT16       SataPortMode;
  BOOLEAN      FchSataHotRemovalEnh;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  FchSataHotRemovalEnh = LocalCfgPtr->Sata.SataHotRemovalEnh;

  SataBar5setting (LocalCfgPtr, &Bar5);
  SataPortMode = (UINT16)LocalCfgPtr->Sata.SataPortMd.SataPortMode;
  PortNumByte = 0;


  while ( PortNumByte < 8 ) {
    PortModeByte = (UINT8) (SataPortMode & 3);
    if ( (PortModeByte == BIT0) || (PortModeByte == BIT1) ) {
      if ( PortModeByte == BIT0 ) {
        //
        // set GEN 1
        //
        RwMem (Bar5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80, AccessWidth8, 0x0F, 0x10);
      }

      if ( PortModeByte == BIT1 ) {
        //
        // set GEN2 (default is GEN3)
        //
        RwMem (Bar5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80, AccessWidth8, 0x0F, 0x20);
      }

      RwMem (Bar5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80, AccessWidth8, 0xFF, 0x01);
    }

    SataPortMode >>= 2;
    PortNumByte ++;
  }

  IEM_SKIP_CODE (IEM_WAIT) {
    FchStall (1000, StdHeader);
  }
  SataPortMode = (UINT16)LocalCfgPtr->Sata.SataPortMd.SataPortMode;
  PortNumByte = 0;

  while ( PortNumByte < 8 ) {
    PortModeByte = (UINT8) (SataPortMode & 3);

    if ( (PortModeByte == BIT0) || (PortModeByte == BIT1) ) {
      RwMem (Bar5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80, AccessWidth8, 0xFE, 0x00);
    }

    PortNumByte ++;
    SataPortMode >>= 2;
  }

  //
  // Sata Hot Removal Enhance setting
  //
  if ( FchSataHotRemovalEnh ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDC, AccessWidth8, 0x7F, 0x80);
  }
}

