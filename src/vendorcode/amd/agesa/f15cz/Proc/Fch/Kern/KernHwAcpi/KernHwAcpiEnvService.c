/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch HwAcpi controller
 *
 * Init HwAcpi Controller features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 309630 $   @e \$Date: 2014-12-18 09:49:25 +0800 (Thu, 18 Dec 2014) $
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
#define FILECODE PROC_FCH_KERN_KERNHWACPI_KERNHWACPIENVSERVICE_FILECODE

#define AMD_CPUID_APICID_LPC_BID    0x00000001ul  // Local APIC ID, Logical Processor Count, Brand ID

ACPI_REG_WRITE FchKernInitEnvSpecificHwAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG00 + 2, 0xFA, BIT0 + BIT2},     //hwemu                           // Set ASF SMBUS master function enabled here (temporary)
#ifdef ACPI_SLEEP_TRAP
  {SMI_BASE  >> 8,  FCH_SMI_REGB0, 0xF3, BIT2},                                                    // Set SLP_TYPE as SMI event
  {PMIO_BASE >> 8,  FCH_PMIOA_REGBE, 0xDF, 0x00},                                                  // Disabled SLP function for S1/S3/S4/S5
  {PMIO_BASE >> 8,  FCH_PMIOA_REG08 + 3, 0xFC, BIT1},                                              // Set S state transition disabled (BIT0) force ACPI to
                                                                                                   //  send SMI message when writing to SLP_TYP Acpi register. (BIT1)
  {SMI_BASE  >> 8,  FCH_SMI_REG98 + 3, 0x7F, 0x00},                                                // Enabled Global Smi ( BIT7 clear as 0 to enable )
#endif
  {PMIO_BASE >> 8,  FCH_PMIOA_REG80 + 1, 0xE7, BIT3 + BIT4},
  {0xFF, 0xFF, 0xFF, 0xFF},
};


/**
 * FchInitEnvHwAcpiMmioTable - Fch ACPI MMIO initial
 * during POST.
 *
 */
ACPI_REG_WRITE FchKernInitEnvHwAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature

  //
  // HPET workaround
  //
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54 + 2, 0x7F, BIT7},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54 + 2, 0x7F, 0x00},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC4, 0xFB, BIT2},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC0, 0, 0x3F},
  //
  // RtcSts              19-17 RTC_STS set only in Sleep State.
  // GppPme                 20 Set to 1 to enable PME request from FCH GPP.
  // Pcireset               22 Set to 1 to allow SW to reset PCIe.
  //
  {PMIO_BASE >> 8, FCH_PMIOA_REGC2, 0x20, 0x58},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC2 + 1, 0, 0x40},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC2, 0xEF, BIT4},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC4, 0xFB, 0},

  {PMIO_BASE >> 8, FCH_PMIOA_REGCC, 0xF8, 0x03},
  {PMIO_BASE >> 8, FCH_PMIOA_REG74, 0x00, BIT0 + BIT1 + BIT2 + BIT4},
  {PMIO_BASE >> 8, FCH_PMIOA_REG74 + 3, 0xDF, 0},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBA, 0xF7, BIT3},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBC, 0xFD, BIT1},
  //
  // Hiding Flash Controller PM_IO 0xDC [1:0]=0x01
  //
  {PMIO_BASE >> 8, FCH_PMIOA_REGDC, 0x7C, BIT1},

  {SMI_BASE >> 8, FCH_SMI_Gevent1, 0, 1},
  {SMI_BASE >> 8, FCH_SMI_Gevent3, 0, 3},
  {SMI_BASE >> 8, FCH_SMI_Gevent4, 0, 4},
  {SMI_BASE >> 8, FCH_SMI_Gevent5, 0, 5},
  {SMI_BASE >> 8, FCH_SMI_Gevent6, 0, 6},
  {SMI_BASE >> 8, FCH_SMI_Gevent20, 0, 20},
  {SMI_BASE >> 8, FCH_SMI_Gevent23, 0, 23},
  {SMI_BASE >> 8, FCH_SMI_xHC0Pme, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_xHC1Pme, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup0, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup1, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup2, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup3, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_IMCGevent0, 0, 12},
  {SMI_BASE >> 8, FCH_SMI_FanThGevent, 0, 13},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme0, 0, 15},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme1, 0, 16},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme2, 0, 17},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme3, 0, 18},
  {SMI_BASE >> 8, FCH_SMI_GecPme, 0, 19},
  {SMI_BASE >> 8, FCH_SMI_CIRPme, 0, 28},
  {SMI_BASE >> 8, FCH_SMI_Gevent8, 0, 24},
  {SMI_BASE >> 8, FCH_SMI_AzaliaPme, 0, 27},
  {SMI_BASE >> 8, FCH_SMI_SataGevent0, 0, 30},
  {SMI_BASE >> 8, FCH_SMI_SataGevent1, 0, 31},
  {SMI_BASE >> 8, FCH_SMI_REG08,  0xE7, 0},
  {SMI_BASE >> 8, FCH_SMI_REG0C + 2, 0xF7, BIT3},
  {SMI_BASE >> 8, FCH_SMI_TWARN, 0, 9},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchKernInitEnvHwAcpiPciTable - PCI device registers initial
 * during early POST.
 *
 */
REG8_MASK FchKernInitEnvHwAcpiPciTable[] =
{
  //
  // SMBUS Device (Bus 0, Dev 20, Func 0)
  //
  {0x00, SMBUS_BUS_DEV_FUN, 0},
  {FCH_CFG_REG10, 0X00, (FCH_VERSION & 0xFF)},                ///Program the version information
  {FCH_CFG_REG11, 0X00, (FCH_VERSION >> 8)},
  {0xFF, 0xFF, 0xFF},
};


/**
 * ProgramEnvPFchAcpiMmio - Config HwAcpi MMIO registers
 *   Acpi S3 resume won't execute this procedure (POST only)
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramEnvPFchAcpiMmio (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchKernInitEnvHwAcpiMmioTable[0]), StdHeader);
}

/**
 * ProgramFchEnvHwAcpiPciReg - Config HwAcpi PCI controller
 * before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvHwAcpiPciReg (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  //Early post initialization of pci config space
  //
  ProgramPciByteTable ((REG8_MASK*) (&FchKernInitEnvHwAcpiPciTable[0]), sizeof (FchKernInitEnvHwAcpiPciTable) / sizeof (REG8_MASK), StdHeader);

  if ( LocalCfgPtr->Smbus.SmbusSsid != NULL ) {
    RwPci ((SMBUS_BUS_DEV_FUN << 16) + FCH_CFG_REG2C, AccessWidth32, 0x00, LocalCfgPtr->Smbus.SmbusSsid, StdHeader);
  }
  if ( LocalCfgPtr->Misc.NoneSioKbcSupport ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED, AccessWidth8, ~(UINT32) (BIT1), BIT1);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED, AccessWidth8, ~(UINT32) (BIT1), 0);
  }
  ProgramPcieNativeMode (FchDataPtr);
}

/**
 * FchVgaInit - Config VGA CODEC
 *
 * @param[in] VOID empty
 *
 */
VOID
FchVgaInit (
  OUT VOID
  )
{
}

/**
 * ProgramSpecificFchInitEnvAcpiMmio - Config HwAcpi MMIO before
 * PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramSpecificFchInitEnvAcpiMmio (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchKernInitEnvSpecificHwAcpiMmioTable[0]), StdHeader);

  //
  // Set LDTSTP# duration
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG94, AccessWidth8, 0, 0x01);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG80 + 3, AccessWidth8, 0xFE, 0x20);
  //
  // LpcClk0DrivenZero & RTC Wake Mode Control
  //
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG6C + 2, AccessWidth8, 0x5F, 0xA0);
  //
  // Ac Loss Control
  //
  AcLossControl ((UINT8) LocalCfgPtr->HwAcpi.PwrFailShadow);
  //
  // FCH VGA Init
  //
  FchVgaInit ();

  //
  // Set ACPIMMIO by OEM Input table
  //
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE *) (LocalCfgPtr->HwAcpi.OemProgrammingTablePtr), StdHeader);
  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x17; //USB3
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x1C110300;
  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x12;  //USB2
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x05030320;
  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x0F;  //SATA
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x270F0200;
  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x18;  //SD
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x0D0B0280;

  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x01;  //AB
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x331F0100;
  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x02;  //ACPI
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x16130380;
  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x04;  //LPC
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x64320480;
  ACPIMMIO16 (FCH_AOACx88Shadow_Register_SRAM_Addr) = 0x1B;  //ESPI
  ACPIMMIO32 (FCH_AOACx8CShadow_Register_SRAM_Data) = 0x120F0580;

  ACPIMMIO32 (FCH_AOACx9C_Shadow_Timer_Control) |= FCH_AOACx9C_Shadow_Timer_Control_ShadowHpetEn + FCH_AOACx9C_Shadow_Timer_Control_ShadowAcpiTimerEn;


  //BUG450772:FCH code doesn't match BKDG
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGD0, AccessWidth32, ~ BIT31, BIT31, StdHeader);
  ACPIMMIO8 (FCH_PMxEC_LpcGating) |= BIT2;
  //change use wake trigger from edge to level to fix Mouse continous click wakeup failure
  if ( LocalCfgPtr->Misc.S3Resume == 0 ) {
    ACPIMMIO32 (FCH_SMIx0C_Level) |= (1 << (ACPIMMIO8 (FCH_SMIx58_EHCI_WAKE))) | (1 << (ACPIMMIO8 (FCH_SMIx78_XHCI_WAKE)));
  }

}

/**
 * ValidateFchVariant - Validate FCH Variant
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
VOID
ValidateFchVariant (
  IN  VOID     *FchDataPtr
  )
{
}

/**
 * IsExternalClockMode - Is External Clock Mode?
 *
 *
 * @retval  TRUE or FALSE
 *
 */
BOOLEAN
IsExternalClockMode (
  IN  VOID     *FchDataPtr
  )
{
  UINT8    MISC80;
  ReadMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80 + 2, AccessWidth8, &MISC80);
  return ( (BOOLEAN) ((MISC80 & BIT1) == 0) );
}


/**
 * ProgramFchEnvSpreadSpectrum - Config SpreadSpectrum before
 * PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvSpreadSpectrum (
  IN  VOID     *FchDataPtr
  )
{
  UINT8        PortStatus;
  UINT8        FchSpreadSpectrum;

  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  FchSpreadSpectrum = LocalCfgPtr->HwAcpi.SpreadSpectrum;

  if ( FchSpreadSpectrum ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG08, AccessWidth8, 0xFE, 0x00);
    //Create profile
    /// -0.375%
//Misc_Reg x10h[9:0]=10'h003 (Ref_Div)
//Misc_Reg x10h[21:10]= 12'h004B (FB_Div)
//Misc_Reg x18h[11:8]=4'h2 (SS_AMOUNT_NFRAC_SLIP)
//Misc_Reg x14h[31:16]= 16'hD000 (SS_AMOUNT_DSFRAC)
//Misc_Reg x14h[15:0]=16'h02D5 (SS_STEP_SIZE)

//Misc_Reg x40[26]= 1'b1 (Update Ref/FB_DIV)
//PM_xC8[4]=1'b1
//PM_xC5[7:0]=8'h06 (CF9 Reset)

//PM_xC8[4]=1'b0
//Misc_Reg x08[0]= 1'b1  (enable SSC)
    if (( ACPIMMIO32 (FCH_MISCx40_MiscClkCntl1) & FCH_MISCx40_MiscClkCntl1_CG1PLL_FBDIV_Test ) == 0) {
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG10, AccessWidth16, 0xFC00, 0x03);
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG10, AccessWidth32, 0xFFC003FF, (0x4B << 10));
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG18 + 1, AccessWidth8, 0xF0, 0x02);
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG14 + 2, AccessWidth16, 0, 0xD000);
      RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG14, AccessWidth16, 0, 0x02D5);
      //ACPIMMIO8 (FCH_PMxC8_Misc) |= FCH_PMxC8_Misc_UseAcpiStraps;
      ACPIMMIO8 (FCH_PMIOx10_Power_Reset_Config) |= FCH_PMIOx10_Power_Reset_Config_ToggleAllPwrGoodOnCf9;

      ACPIMMIO32 (FCH_MISCx40_MiscClkCntl1) |= FCH_MISCx40_MiscClkCntl1_CG1PLL_FBDIV_Test;
      //Do Cf9 reset here to make profile available
      WriteIo8 (FCH_IOMAP_REGCF9, 0x06);
      FCH_DEADLOOP ();
    }
    ACPIMMIO8 (FCH_PMIOx10_Power_Reset_Config) &= ~ FCH_PMIOx10_Power_Reset_Config_ToggleAllPwrGoodOnCf9;
    ACPIMMIO8 (FCH_PMxC8_Misc) &= ~ FCH_PMxC8_Misc_UseAcpiStraps;
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG08, AccessWidth8, 0xFE, BIT0);
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG08, AccessWidth8, 0xFE, 0x00);
  }

  //
  // PLL 100Mhz Reference Clock Buffer setting for internal clock generator mode (BIT5)
  // OSC Clock setting for  internal clock generator mode (BIT6)
  //
  GetChipSysMode (&PortStatus, StdHeader);
  if ( ((PortStatus & ChipSysIntClkGen) == ChipSysIntClkGen) ) {
// remove to enable pci-e device
//    RwMem (ACPI_MMIO_BASE + MISC_BASE +  FCH_MISC_REG04 + 1, AccessWidth8, ~(UINT32) (BIT5 + BIT6), BIT5 + BIT6);
  }
}

/**
 * TurnOffCG2
 *
 *
 * @retval  VOID
 *
 */
VOID
TurnOffCG2 (
  OUT VOID
  )
{
}

/**
 * BackUpCG2
 *
 *
 * @retval  VOID
 *
 */
VOID
BackUpCG2 (
  OUT VOID
  )
{
}

/**
 * HpetInit - Program Fch HPET function
 *
 *
 *
 * @param[in] FchDataPtr         Fch configuration structure pointer.
 *
 */
VOID
HpetInit (
  IN  VOID     *FchDataPtr
  )
{
  DESCRIPTION_HEADER   *HpetTable;
  BOOLEAN              FchHpetTimer;
  BOOLEAN              FchHpetMsiDis;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  FchHpetTimer = LocalCfgPtr->Hpet.HpetEnable;
  FchHpetMsiDis = LocalCfgPtr->Hpet.HpetMsiDis;

  HpetTable = NULL;
  if ( FchHpetTimer == TRUE ) {
    //
    //Program the HPET BAR address
    //
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth8, 0xFF, BIT6);

    //
    //Enabling decoding of HPET MMIO
    //Enable HPET MSI support
    //Enable High Precision Event Timer (also called Multimedia Timer) interrupt
    //
    if ( FchHpetMsiDis == FALSE ) {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth32, ~(UINT32) BIT29, BIT29);
#ifdef FCH_TIMER_TICK_INTERVAL_WA
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth32, ~(UINT32) BIT29, 0);
#endif
    } else {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00, AccessWidth32, ~(UINT32) BIT29, 0);
    }

  } else {
    if ( ! (LocalCfgPtr->Misc.S3Resume) ) {
      HpetTable = (DESCRIPTION_HEADER*) AcpiLocateTable ('TEPH');
    }
    if ( HpetTable != NULL ) {
      HpetTable->Signature = 'HPET';
    }
  }
}

/**
 * ProgramPcieNativeMode - Config Pcie Native Mode
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramPcieNativeMode (
  IN  VOID     *FchDataPtr
  )
{
  UINT8        FchNativepciesupport;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  FchNativepciesupport = (UINT8) LocalCfgPtr->Misc.NativePcieSupport;

  //
  // PCIE Native setting
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBA + 1, AccessWidth8, ~(UINT32) BIT6, 0);
  if ( FchNativepciesupport == 1) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG74 + 3, AccessWidth8, ~(UINT32) (BIT3 + BIT1 + BIT0), BIT3 + BIT0);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG74 + 3, AccessWidth8, ~(UINT32) (BIT3 + BIT1 + BIT0), BIT3);
  }
}
