/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch HwAcpi controller
 *
 * Init HwAcpi Controller features (PEI phase).
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 314271 $   @e \$Date: 2015-03-08 03:37:30 -0500 (Sun, 08 Mar 2015) $
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
#include "Filecode.h"
#include "GeneralServices.h"
#include "KernFch.h"
#define FILECODE PROC_FCH_KERN_KERNHWACPI_KERNHWACPIRESET_FILECODE

extern ACPI_REG_WRITE FchInitResetAcpiMmioTable[];
extern VOID  ProgramFchHwAcpiResetP (IN VOID  *FchDataPtr);

UINT8 DevCount[] = {15, 18, 23, 24, 17, 5, 6, 7, 8, 11, 12};
/**
 * FchInitResetPowerAllDev - Power on All Devices
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetPowerAllDev (
  IN  VOID     *FchDataPtr
  )
{
  UINT8 i;
  AMD_CONFIG_PARAMS         *StdHeader;
  UINT8                     RegValue8;

  RegValue8 = 0;

  StdHeader = &((AMD_RESET_PARAMS *)FchDataPtr)->StdHeader;

  if (!(0x0C00 == (0x1C00 & ReadIo16 (UserOptions.FchBldCfg->CfgAcpiPm1CntBlkAddr)))) {
    for (i = 0; i < sizeof (DevCount); i++) {
      if (!((ACPIMMIO8 (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1)) & 0x3) == 0x3)) {
        ACPIMMIO8 (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1)) |= AOAC_PWR_ON_DEV;
        if (DevCount[i] == 24) {
          //ENH449309:Workaround for OBS448495 SD Host Controller SUBSYS ID change issue
          ReadMem (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1) + 1, AccessWidth8, &RegValue8);
          while (RegValue8 != 7) {
            ReadMem (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1) + 1, AccessWidth8, &RegValue8);
            RegValue8 &= 0x7;
          }
          ACPIMMIO8 (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1)) |= AOAC_IS_SW_CONTROL;
          ACPIMMIO8 (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1)) &= ~ AOAC_SW_RST_B;
          FchStall (200, StdHeader);
          ACPIMMIO8 (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1)) |= AOAC_SW_RST_B;
          ACPIMMIO8 (FCH_AOACx40_D3_CONTROL + (DevCount[i] << 1)) &= ~ AOAC_IS_SW_CONTROL;

        }
      } else {
        //Do Cf9 reset here to reset AOAC registers
        WriteIo8 (FCH_IOMAP_REGCF9, 0x06);
        FCH_DEADLOOP ();
      }
    }
  } else {
  }
  FchStall (1000, StdHeader);
}

/**
 * FchInitResetPowerG3Reset - If G3 then reset
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetPowerG3Reset (
  IN  VOID     *FchDataPtr
  )
{
  AMD_CONFIG_PARAMS         *StdHeader;

  StdHeader = &((AMD_RESET_PARAMS *)FchDataPtr)->StdHeader;

  if ((0 == (ACPIMMIO32 (FCH_PMxC0_ResetStatus) & FCH_PMxC0_ResetStatus_Mask)) && (0 == (ACPIMMIO32 (FCH_PMx00_DecodeEn) & BIT1))) {
    //Do Cf9 reset here to reset AOAC registers
    RwPmio (FCH_PMIOA_REG00, AccessWidth8, ~(UINT32) BIT1, BIT1, StdHeader);
    WriteIo8 (FCH_IOMAP_REGCF9, 0x06);
    FCH_DEADLOOP ();
  }
}

/**
 * FchInitResetHwAcpiP - Config HwAcpi controller ( Preliminary
 * ) during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetHwAcpiP (
  IN  VOID     *FchDataPtr
  )
{
  AMD_CONFIG_PARAMS         *StdHeader;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  UINT8 Value8;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = &((AMD_RESET_PARAMS *)FchDataPtr)->StdHeader;
  //
  // Enabled (Mmio_mem_enable)
  //
  RwPmio (FCH_PMIOA_REG04, AccessWidth8, 0xFF, BIT1, StdHeader);

  ACPIMMIO32 (FCH_AOACx94S013_CONTROL) &= ~ (FCH_AOACx94S013_CONTROL_ARBITER_DIS + FCH_AOACx94S013_CONTROL_INTERRUPT_DIS);
  ACPIMMIO32 (FCH_PMx08_PciControl) |= FCH_PMx08_PciControl_ShutDownOption; //BUG425824 / Improper handling of INIT and SHUTDOWN on Carrizo

  //
  // enable CF9
  //
  RwPmio (FCH_PMIOA_REGD2, AccessWidth8, ~(UINT32) BIT6, 0, StdHeader);

  ProgramFchHwAcpiResetP (FchDataPtr);

  //BTDC Coreboot. Reenable it on new board.:  FchInitResetPowerG3Reset (FchDataPtr);

  FchInitResetPowerAllDev (FchDataPtr);


  //
  // enable LpcClockDriveStrength
  //
  RwPmio (FCH_PMIOA_REGD2, AccessWidth8, ~(UINT32) ( BIT4 + BIT5 ), (UINT8) ((LocalCfgPtr->LpcClockDriveStrength & 3) << 4), StdHeader);
  if ((!( ACPIMMIO32 (FCH_MISCx50_JTAG_CONTROL_ECO) & FCH_MISCx50_JTAG_Control_ECO_bits_BIT16 )) || \
    (ACPIMMIO32 (FCH_PMIOxC0_S5ResetStatus) & FCH_PMIOxC0_S5ResetStatus_S_Status)) {
    ACPIMMIO32 (FCH_MISCx50_JTAG_CONTROL_ECO) |= FCH_MISCx50_JTAG_Control_ECO_bits_BIT16;
    ACPIMMIO32 (FCH_PMIOxC0_S5ResetStatus) |= FCH_PMIOxC0_S5ResetStatus_S_Status;
    Value8 = 0x06;
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCF9, &Value8, StdHeader);
    //issue cf9 reset
  } else {
    //normal reboot clear all Reset Status
    ACPIMMIO32 (FCH_PMIOxC0_S5ResetStatus) |= FCH_PMIOxC0_S5ResetStatus_All_Status;
  }
  //
  // Clear FourSecondPwrBtn
  //
  RwPmio (FCH_PMIOA_REGC0, AccessWidth8, 0, BIT1, StdHeader);
  //ACPIMMIO32 (FCH_AL2AHBx10_CONTROL) |= FCH_AL2AHBx10_CONTROL_CLOCK_GATE_EN;
  //ACPIMMIO32 (FCH_AL2AHBx30_HCLK_CONTROL) |= FCH_AL2AHBx30_HCLK_CONTROL_CLOCK_GATE_EN;

  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC4, AccessWidth8, 0xEF, 0x10);
  FchStall (200, StdHeader);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC4, AccessWidth8, 0xEF, 0x00);
}

/**
 * FchInitResetHwAcpi - Config HwAcpi controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetHwAcpi (
  IN  VOID     *FchDataPtr
  )
{
  UINT16       SmbusBase;
  UINT8        Value;
  UINT16       AsfPort;
  UINT32       GeventEnableBits;
  UINT32       GeventValue;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // Set Build option into SB
  //
  WritePci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG64, AccessWidth16, &(UserOptions.FchBldCfg->CfgSioPmeBaseAddress), StdHeader);

  //
  // Enabled SMBUS0/SMBUS1 (ASF) Base Address
  //
  SmbusBase = UserOptions.FchBldCfg->CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth16, 0x00FF, SmbusBase + BIT4);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60, AccessWidth16, 00, (UserOptions.FchBldCfg->CfgAcpiPm1EvtBlkAddr));
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG62, AccessWidth16, 00, (UserOptions.FchBldCfg->CfgAcpiPm1CntBlkAddr));
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64, AccessWidth16, 00, (UserOptions.FchBldCfg->CfgAcpiPmTmrBlkAddr));
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG66, AccessWidth16, 00, (UserOptions.FchBldCfg->CfgCpuControlBlkAddr));
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG68, AccessWidth16, 00, (UserOptions.FchBldCfg->CfgAcpiGpe0BlkAddr));
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A, AccessWidth16, 00, (UserOptions.FchBldCfg->CfgSmiCmdPortAddr));
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6E, AccessWidth16, 00, (UserOptions.FchBldCfg->CfgSmiCmdPortAddr) + 8);

  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth32, ~(UINT32) (BIT19 + BIT20), 0);                           ///clear BIT[2:1]
  Value = 0x00;
  LibAmdIoWrite (AccessWidth8, SmbusBase + 0x14, &Value, StdHeader);

  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchInitResetAcpiMmioTable[0]), StdHeader);

  if (UserOptions.FchBldCfg->CfgFchSciMapControl != NULL) {
    ProgramFchSciMapTbl ((UserOptions.FchBldCfg->CfgFchSciMapControl), LocalCfgPtr);
  }

  if (UserOptions.FchBldCfg->CfgFchGpioControl != NULL) {
    ProgramFchGpioTbl (UserOptions.FchBldCfg->CfgFchGpioControl);
  }

  if (LocalCfgPtr->EarlyOemGpioTable != NULL) {
    ProgramFchGpioTbl ((GPIO_CONTROL  *) (LocalCfgPtr->EarlyOemGpioTable));
  }

  if (UserOptions.FchBldCfg->CfgFchSataPhyControl != NULL) {
    ProgramFchSataPhyTbl ((UserOptions.FchBldCfg->CfgFchSataPhyControl), LocalCfgPtr);
  }
  //
  // RTC Workaround for Daylight saving time enable bit
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5E, AccessWidth8, 0, 0);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5F, AccessWidth8, 0xFE, BIT0 );   // Enable DltSavEnable
  Value = 0x0B;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG70, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  Value &= 0xFE;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5E, AccessWidth8, 0, 0);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5F, AccessWidth8, 0xFE, 0 );      // Disable DltSavEnable
  //
  // Prevent RTC error
  //
  Value = 0x0A;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG70, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
  Value &= 0xEF;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);

  if ( UserOptions.FchBldCfg->CfgFchRtcWorkAround ) {
    Value = RTC_WORKAROUND_SECOND;
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG70, &Value, StdHeader);
    LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    if ( Value > RTC_VALID_SECOND_VALUE ) {
      Value = RTC_SECOND_RESET_VALUE;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    }
    LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    Value &= RTC_SECOND_LOWER_NIBBLE;
    if ( Value > RTC_VALID_SECOND_VALUE_LN ) {
      LibAmdIoRead (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
      Value = RTC_SECOND_RESET_VALUE;
      LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REG71, &Value, StdHeader);
    }
  }

  Value = 0x08;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);

  if ( !LocalCfgPtr->EcKbd ) {
    //
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    //
    Value = Value | 0x0A;
  }
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);

  Value = 0x09;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &Value, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);
  if ( !LocalCfgPtr->EcKbd ) {
    //
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    //
    Value = Value & 0xF9;
  }

  if ( LocalCfgPtr->LegacyFree ) {
    //
    // Disable IRQ1/IRQ12 filter enable for Legacy free with USB KBC emulation.
    //
    Value = Value & 0x9F;
  }
  //
  // Enabled IRQ input
  //
  Value = Value | BIT4;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &Value, StdHeader);

  AsfPort = SmbusBase + 0x20;
  if ( AsfPort != 0 ) {
    UINT8  dbValue;
    dbValue = 0x70;
    LibAmdIoWrite (AccessWidth8, AsfPort + 0x0E, &dbValue, StdHeader);
    dbValue = 0x2F;
    LibAmdIoWrite (AccessWidth8, AsfPort + 0x0A, &dbValue, StdHeader);
  }
  //
  // PciExpWakeStatus workaround
  //
  ReadMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60, AccessWidth16, &AsfPort);
  AsfPort++;
  ReadMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG04, AccessWidth32, &GeventEnableBits);
  ReadMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG00, AccessWidth32, &GeventValue);
  if ( (GeventValue & GeventEnableBits) != 0 ) {
    Value = 0x40;
    LibAmdIoWrite (AccessWidth8, AsfPort, &Value, StdHeader);
  }
  LibAmdIoRead (AccessWidth8, AsfPort, &Value, StdHeader);
  if ((Value & (BIT2 + BIT0)) != 0) {
    Value = 0x40;
    LibAmdIoWrite (AccessWidth8, AsfPort, &Value, StdHeader);
  }

}


