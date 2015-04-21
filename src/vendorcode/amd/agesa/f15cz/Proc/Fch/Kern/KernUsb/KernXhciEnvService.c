/* $NoKeywords:$ */
/**
 * @file
 *
 * Config KERN FCH USB3 controller
 *
 * Init USB3 features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 314087 $   @e \$Date: 2015-03-04 12:03:46 +0800 (Wed, 04 Mar 2015) $
 *
 */
/*;********************************************************************************
;
; Copyright 2008 - 2015 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software (the Materials)
; pursuant to the terms and conditions of your Software License Agreement
; with AMD.  This header does *NOT* give you permission to use the Materials
; or any rights under AMD's intellectual property.  Your use of any portion
; of these Materials shall constitute your acceptance of those terms and
; conditions.  If you do not agree to the terms and conditions of the Software
; License Agreement, please do not use any portion of these Materials.
;
; CONFIDENTIALITY:  The Materials and all other information, identified as
; confidential and provided to you by AMD shall be kept confidential in
; accordance with the terms and conditions of the Software License Agreement.
;
; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
;
; AMD does not assume any responsibility for any errors which may appear in
; the Materials or any other related information provided to you by AMD, or
; result from use of the Materials or any related information.
;
; You agree that you will not reverse engineer or decompile the Materials.
;
; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; further information, software, technical information, know-how, or show-how
; available to you.  Additionally, AMD retains the right to modify the
; Materials at any time, without notice, and is not obligated to provide such
; modified Materials to you.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgement of AMD's proprietary rights in them.
;
; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; direct product thereof will be exported directly or indirectly, into any
; country prohibited by the United States Export Administration Act and the
; regulations thereunder, without the required authorization from the U.S.
; government nor will be used for any purpose prohibited by the same.
;*********************************************************************************/
#include "FchPlatform.h"
#include "GeneralServices.h"
#include "KernFch.h"
#include "Ids.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_KERN_KERNUSB_KERNXHCIENVSERVICE_FILECODE

//
// Declaration of local functions
//

#define PRODUCT_INFO_REG1           0x1FC     // Product Information Register 1
#define AMD_CPU_DEV_FUN             ((0x18 << 3) + 3)

UINT32
FchCombineSigRomInfo (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  );

/**
 * FchUsb3Debug - Config USB Usb3Debug
 * Calibration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchUsb3Debug (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //ENH462631:  [CZ] External USB2 device can be auto-detected on USB3 port after safely removal
  ACPIMMIO32 (FCH_XHC_PMx24_Misc0) &= ~ BIT31;

  if (LocalCfgPtr->Usb.Usb3Debug.ServiceIntervalEnable) {
    ACPIMMIO32 (FCH_XHC_PMx10_Xhc_Memory_Configure) |= BIT23 + BIT24 + BIT25 + BIT26;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.BandwidthExpandEnable) {
    ACPIMMIO32 (FCH_XHC_PMx18_Usb20_Link_Status) |= BIT8 + BIT9 + BIT10 + BIT11 + BIT12;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.AoacEnable) {
    ACPIMMIO32 (FCH_XHC_PMx18_Usb20_Link_Status) |= BIT24 + BIT25 + BIT26;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.HwLpmEnable) {
    //ACPIMMIO32 (FCH_XHC_PMx30_Xhci10_Enable) |= BIT27;
    ACPIMMIO32 (FCH_XHC_PMx28_Misc1) |= BIT18 + BIT19 + BIT20 + BIT21;
    ACPIMMIO32 (FCH_XHC_PMx24_Misc0) |= BIT18;
    RwXhci0IndReg ( FCH_XHCI_IND60_REG58, ~ ((UINT32) (0x1F << 0)), ((UINT32) (0x00 << 0)), StdHeader);
  }
  if (LocalCfgPtr->Usb.Usb3Debug.DbcEnable) {
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT10 + BIT11 + BIT12 + BIT13 + BIT14 + BIT15 + BIT19;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.MiscPlusEnable) {
    ACPIMMIO32 (FCH_XHC_PMx28_Misc1) |= BIT30 + BIT31 + BIT26;
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT7 + BIT8 + BIT9;
  } else {
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT5;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.EcoFixEnable) {
    ACPIMMIO32 (FCH_XHC_PMx28_Misc1) |= BIT4 + BIT14 + BIT15 + BIT25;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.SsifEnable) {
    ACPIMMIO32 (FCH_XHC_PMx28_Misc1) |= BIT27;
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT4 + BIT16 + BIT18 + BIT20;
  } else {
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT17;
  }

  if (LocalCfgPtr->Usb.Usb3Debug.U2ifEnable) {
    ACPIMMIO32 (FCH_XHC_PMx20_Usb20_Wake_Control) &= ~ (0x3 << 16);
    ACPIMMIO32 (FCH_XHC_PMx20_Usb20_Wake_Control) |= (0x2 << 16);
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT2;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.FseEnable) {
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT0 + BIT1 + BIT6;
  }
  if (LocalCfgPtr->Usb.Usb3Debug.XhcPmeEnable) {
    ACPIMMIO32 (FCH_XHC_PMx2C_Misc2) |= BIT3;
  }
}
/**
 * FchUsbCommonPhyCalibration - Config USB Common PHY
 * Calibration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
UINT8
FchUsbCommonPhyCalibration (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINT8                 RetEfuseValue;
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  ReadPci ((AMD_CPU_DEV_FUN << 16) + PRODUCT_INFO_REG1, AccessWidth8, &RetEfuseValue, StdHeader);
  RetEfuseValue = ((RetEfuseValue & 0x1E) >> 1);
  return RetEfuseValue;
}
/**
 * FchXhciUsbPhyCalibrated - Config XHCI Phy
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchXhciUsbPhyCalibrated (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINT8                 Index;
  UINT32                DrivingStrength;
  UINT32                Port;
  UINT32                Register;
  UINT32                RegValue;
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG8C, AccessWidth32, 0xFFF0FFFF, 0x00030000);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG8C, AccessWidth32, 0xFEFFFFFF, 0x01000000);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG8C, AccessWidth32, 0x3FFFFFFF, 0x0);
  //
  // xHCI USB 2.0 PHY Settings
  // Step 1 Program HSSLEW=10b.
  //

  // Program D10F0x4C_x4000_0000[VControlModeSel]=0110b.
  // Program HSSLEW value at D10F0x4C_x4000_0000[1:0]=10b.

  DrivingStrength = 0x302;
  for (Port = 0; Port < 2; Port ++) {
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
    Register = FCH_XHCI_IND60_REG00;
    Index = 0;
    do {
      WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
      ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
      Index++;
      IEM_SKIP_CODE (IEM_WAIT) {
        FchStall (10, StdHeader);
      }
    } while ((RegValue & BIT17) && (Index < 10 ));
    Index = 0;
    do {
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + DrivingStrength, StdHeader);
      WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
      ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
      Index++;
      IEM_SKIP_CODE (IEM_WAIT) {
        FchStall (10, StdHeader);
      }
    } while ((RegValue & BIT17) && (Index < 10 ));
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
  }
  RwXhci0IndReg ( FCH_XHCI_IND60_REG50, ~ ((UINT32) (0x0f)), ((UINT32) (0x06)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0x0f << 4)), ((UINT32) (0x02 << 4)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0x0f << 8)), ((UINT32) (0x02 << 8)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0x0f << 12)), ((UINT32) (0x01 << 12)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0x0f)), ((UINT32) (0x01)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0xff << 24)), ((UINT32) (0x90 << 24)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG54, ~ ((UINT32) (0x03 << 6)), ((UINT32) (0x01 << 6)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG54, ~ ((UINT32) (0x01 << 1)), ((UINT32) (0x00 << 1)), StdHeader);
  IEM_SKIP_CODE (IEM_WAIT) {
    FchStall (200, StdHeader);
  }
  RwXhci0IndReg ( FCH_XHCI_IND60_REG54, ~ ((UINT32) (0x01 << 1)), ((UINT32) (0x01 << 1)), StdHeader);
  IEM_SKIP_CODE (IEM_WAIT) {
    FchStall (400, StdHeader);
  }
  RwXhci0IndReg ( FCH_XHCI_IND60_REG54, ~ ((UINT32) (0x01 << 1)), ((UINT32) (0x00 << 1)), StdHeader);
}

/**
 * FchXhciInitIndirectReg - Config XHCI Indirect Registers
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchXhciInitIndirectReg (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINT8                 Index;
  UINT32                DrivingStrength;
  UINT32                Port;
  UINT32                Register;
  UINT32                RegValue;
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;
  UINT8                 RetEfuseValue;
  UINT32                UsbFuseCommonCalibrationValue;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  DrivingStrength = 0;

  FchXhciUsbPhyCalibrated (LocalCfgPtr);
  //
  // SuperSpeed PHY Configuration (adaptation mode setting)
  //
  RwXhci0IndReg ( FCH_XHCI_IND_REG94, 0xFFFFFC00, 0x00000021, StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND_REGD4, 0xFFFFFC00, 0x00000021, StdHeader);
  //
  // SuperSpeed PHY Configuration (CR phase and frequency filter settings)
  //

  //
  // BLM Meaasge
  //
  RwXhci0IndReg ( FCH_XHCI_IND_REG00, 0xF8FFFFFF, 0x07000000, StdHeader);
  //
  // xHC USB2.0 Port Drive Strength Adjustment
  // The drive strength is auto-calibrated on power up. If the calibrated value needs to be adjusted, use the following
  // procedure to do the adjustment.
  // 1. Program D10F0x4C_x4000_0000[PortNumber] with the controller port number according to Table 109.
  // 2. Program D10F0x4C_x4000_0000[VControlModeSel]=1000b to select Group 8.
  // 3. Program D10F0x4C_x4000_0000[VControl[4]]=0b and D10F0x4C_x4000_0000[VControl[5]]=1b to use
  // (Actual+Adjusted) CalAmp value for drive strength.
  // 4. Program D10F0x4C_x4000_0000[VControl[3:0]] with desired drive strength adjustment.
  // a. D10F0x4C_x4000_0000[VControl[3]] is the sign bit. 1= Negative offset. 0= Positive offset.
  // 5. Program D10F0x4C_x4000_0000[VLoadB]=1.
  // 6. Wait for D10F0x4C_x4000_0000[VBusy]=0.
  // 7. Program D10F0x4C_x4000_0000[VLoadB]=0.
  // 8. Wait for D10F0x4C_x4000_0000[VBusy]=0.
  // 9. Program D10F0x4C_x4000_0000[VLoadB]=1 to lock the PHY control interface.
  for (Port = 0; Port < 4; Port ++) {
    DrivingStrength = (UINT32) (LocalCfgPtr->Usb.Xhci20Phy[Port]);
    DrivingStrength &= 0xF;
    DrivingStrength |= BIT5;
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, (Port << 13) + BIT12 + BIT10 + DrivingStrength, StdHeader);
    Register = FCH_XHCI_IND60_REG00;
    Index = 0;
    do {
      WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
      ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
      Index++;
      IEM_SKIP_CODE (IEM_WAIT) {
        FchStall (10, StdHeader);
      }
    } while ((RegValue & BIT17) && (Index < 10 ));
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, ~(UINT32)BIT12, 0, StdHeader);

    Register = FCH_XHCI_IND60_REG00;
    Index = 0;
    do {
      WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
      ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
      Index++;
      IEM_SKIP_CODE (IEM_WAIT) {
        FchStall (10, StdHeader);
      }
    } while ((RegValue & BIT17) && (Index < 10 ));
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, ~(UINT32)BIT12, BIT12, StdHeader);
  }

  // Step 3

  RwXhci0IndReg ( FCH_XHCI_IND60_REG50, ~ ((UINT32) (0x0f)), ((UINT32) (0x06)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0x0f << 4)), ((UINT32) (0x02 << 4)), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0x0f << 8)), ((UINT32) (0x02 << 8)), StdHeader);
  RetEfuseValue = FchUsbCommonPhyCalibration ( FchDataPtr );
  if ( RetEfuseValue == 0) {
    RwXhci0IndReg ( FCH_XHCI_IND60_REG08, 0x80FD00FF, (UINT32) BIT16 , StdHeader);
  } else {
    UsbFuseCommonCalibrationValue = RetEfuseValue << 11;
    RwXhci0IndReg ( FCH_XHCI_IND60_REG08, 0x80FC00FF, UsbFuseCommonCalibrationValue , StdHeader);
  }

  // RPR 8.25 Clock Gating in PHY
  for (Port = 0; Port < 4; Port ++) {
    DrivingStrength = 0x1E4;
    if (Port < 2) {
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
      Register = FCH_XHCI_IND60_REG00;
      Index = 0;
      do {
        WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
        ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
        Index++;
        IEM_SKIP_CODE (IEM_WAIT) {
          FchStall (10, StdHeader);
        }
      } while ((RegValue & BIT17) && (Index < 10 ));
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, (Port << 13) + DrivingStrength, StdHeader);
      Register = FCH_XHCI_IND60_REG00;
      Index = 0;
      do {
        WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
        ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
        Index++;
        IEM_SKIP_CODE (IEM_WAIT) {
          FchStall (10, StdHeader);
        }
      } while ((RegValue & BIT17) && (Index < 10 ));
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
    }
  }
  // Enhance XHC LS Rise time
  for (Port = 0; Port < 2; Port ++) {
    DrivingStrength = 0x1C4;
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + DrivingStrength, StdHeader);
    RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
  }
  // RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT23), BIT23);

  // XHCI ISO device CRC false error detection
  for (Port = 0; Port < 4; Port ++) {
    if (Port < 2) {
      ReadXhci0Phy (Port, (0x7 << 7), &DrivingStrength, StdHeader);
      DrivingStrength &= 0x0000000F8;
      DrivingStrength |= BIT0 + BIT2;
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + (0x7 << 7) + DrivingStrength, StdHeader);
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + (0x7 << 7) + DrivingStrength, StdHeader);
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + (0x7 << 7) + DrivingStrength, StdHeader);
    }
  }
  // L1 Residency Duration
  RwXhci0IndReg ( FCH_XHCI_IND60_REG48, 0xFFFFFFFE0, BIT0, StdHeader);
  // follow BKDG to program SPHY Port[3:0] Test Control 0
  for (Port = 0; Port < 4; Port ++) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, (FCH_XHCI_IND_REG200 + Port*0x40));
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT0 + BIT1 + BIT2), 3);
  }

}

/**
 *  FchCombineSigRomInfo - Combine SIG ROM information
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
UINT32
FchCombineSigRomInfo (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINTN                 RomSigStartingAddr;
  UINT32                RomStore[NUM_OF_ROMSIG_FILED];
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;
  UINT32                SelNum;
  UINT32                RomSignatureTag;
  UINT16                XhciFwTag;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  for ( SelNum = 0; SelNum < NUM_OF_ROMSIG_FILED; SelNum++) {
    RomStore [SelNum] = 0;
  }
  GetRomSigPtr (&RomSigStartingAddr, StdHeader);
  RomSignatureTag = ACPIMMIO32 (RomSigStartingAddr);
  if ( RomSignatureTag == ROMSIG_SIG ) {
    for ( SelNum = 0; SelNum < NUM_OF_ROMSIG_FILED; SelNum++) {
      RomStore[SelNum] = ACPIMMIO32 (RomSigStartingAddr + (SelNum << 2));
    }
  } else {
    RomStore[XHCI_FILED_NUM] = 0;
  }
  if ( LocalCfgPtr->Usb.UserDefineXhciRomAddr != NULL ) {
    RomStore[XHCI_FILED_NUM] = LocalCfgPtr->Usb.UserDefineXhciRomAddr;
  }
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGC8 + 3, AccessWidth8, 0x7F, BIT7, StdHeader);
  for ( SelNum = 1; SelNum < NUM_OF_ROMSIG_FILED; SelNum++) {
    if (RomStore[SelNum] != 0) {
      RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGCC, AccessWidth32, ~(UINT32) (BIT2 + BIT1 + BIT0), (BIT2 + SelNum), StdHeader);
      RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGCC, AccessWidth32, ROMSIG_CFG_MASK, RomStore[SelNum], StdHeader);
    }
  }
  XhciFwTag = (UINT16) ACPIMMIO32 ((RomStore[XHCI_FILED_NUM]) + XHCI_BOOT_RAM_OFFSET);
  if ( XhciFwTag != INSTRUCTION_RAM_SIG ) {
    RomStore[XHCI_FILED_NUM] = 0;
  }
  return RomStore[XHCI_FILED_NUM];
}

/**
 * FchXhciInitBeforePciInit - Config XHCI controller before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchXhciInitBeforePciInit (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINT8                 Value8;
  UINT16                BcdAddress;
  UINT16                BcdSize;
  UINT16                AcdAddress;
  UINT16                AcdSize;
  UINT16                FwAddress;
  UINT16                FwSize;
  UINT32                XhciFwStarting;
  UINT32                SpiValidBase;
  UINT32                RegData;
  UINT16                Index;
  BOOLEAN               Xhci0Enable;
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  Xhci0Enable = LocalCfgPtr->Usb.Xhci0Enable;

  if ( Xhci0Enable == 0 ) {
    return;
  }
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, 0x00090000, 0x00400700);
  IEM_SKIP_CODE (IEM_WAIT) {
    FchStall (20, StdHeader);
  }

  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGC8 + 3, AccessWidth8, 0x7F, BIT7, StdHeader);

  Value8 = 0;
  while (!(Value8 & BIT7)) {
    ReadPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGC8 + 3, AccessWidth8, &Value8, StdHeader);
  }

  XhciFwStarting = FchCombineSigRomInfo (FchDataPtr);
  if ( XhciFwStarting == 0 ) {
    return;
  }
  //
  // Enable SuperSpeed receive special error case logic. 0x20 bit8
  // Enable USB2.0 RX_Valid Synchronization. 0x20 bit9
  // Enable USB2.0 DIN/SE0 Synchronization. 0x20 bit10
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, 0xFFFFF9FF, 0x00000600);
  //
  // SuperSpeed PHY Configuration (adaptation timer setting)
  // XHC U1 LFPS Exit time
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG90, AccessWidth32, 0xCFF00000, 0x000AAAAA);

  //
  // Step 1. to enable Xhci IO and Firmware load mode
  //
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG50 + 1, AccessWidth8, ~(UINT32) BIT1, BIT1);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT4 + BIT5), 0);                /// Disable Device 22
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT7), BIT7);                    /// Enable 2.0 devices
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, 0xF0FFFFFC, 0x01);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, 0xEFFFFFFF, 0x10000000);

  //
  // Step 2. to read a portion of the USB3_APPLICATION_CODE from BIOS ROM area and program certain registers.
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA0, AccessWidth32, 0x00000000, (SPI_HEAD_LENGTH << 16));

  BcdAddress = ACPIMMIO16 (XhciFwStarting + BCD_ADDR_OFFSET + XHC_BOOT_RAM_SIZE);
  BcdSize = ACPIMMIO16 (XhciFwStarting + BCD_SIZE_OFFSET + XHC_BOOT_RAM_SIZE);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA4, AccessWidth16, 0x0000, BcdAddress);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA4 + 2, AccessWidth16, 0x0000, BcdSize);

  AcdAddress = ACPIMMIO16 (XhciFwStarting + ACD_ADDR_OFFSET + XHC_BOOT_RAM_SIZE);
  AcdSize = ACPIMMIO16 (XhciFwStarting + ACD_SIZE_OFFSET + XHC_BOOT_RAM_SIZE);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA8, AccessWidth16, 0x0000, AcdAddress);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA8 + 2, AccessWidth16, 0x0000, AcdSize);

  SpiValidBase = SPI_BASE2 (XhciFwStarting + 4 + XHC_BOOT_RAM_SIZE) | SPI_BAR0_VLD | SPI_BASE0 | SPI_BAR1_VLD | SPI_BASE1 | SPI_BAR2_VLD;
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB0, AccessWidth32, 0x00000000, SpiValidBase);
  //
  //   Copy Type0/1/2 data block from ROM image to MMIO starting from 0xC0
  //
  for (Index = 0; Index < SPI_HEAD_LENGTH; Index++) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGC0 + Index, AccessWidth8, 0, ACPIMMIO8 (XhciFwStarting + XHC_BOOT_RAM_SIZE + Index));
  }

  for (Index = 0; Index < BcdSize; Index++) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGC0 + SPI_HEAD_LENGTH + Index, AccessWidth8, 0, ACPIMMIO8 (XhciFwStarting + BcdAddress + Index));
  }

  for (Index = 0; Index < AcdSize; Index++) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGC0 + SPI_HEAD_LENGTH + BcdSize + Index, AccessWidth8, 0, ACPIMMIO8 (XhciFwStarting + AcdAddress + Index));
  }

  //
  // Step 3. to enable the BOOT RAM preload functionality.
  //

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT0, BIT0);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT29, 0);

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04 + 2, AccessWidth16, 0x7FFF, BIT15);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04, AccessWidth16, 0x0000, 0);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG08 + 2, AccessWidth16, 0x0000, 0x8000);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG08, AccessWidth16, 0x0000, 0);

  //Record XHCI Rom Address
  LocalCfgPtr->FchRunTime.XHCI_PMx04_XhciFwRomAddr_Rom = ACPIMMIO16 (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04);
  LocalCfgPtr->FchRunTime.XHCI_PMx08_xHCI_Firmware_Addr_1_Rom = ACPIMMIO32 (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG08);

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT29, BIT29);

  for (;;) {
    ReadMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00 , AccessWidth32, &RegData);
    if (RegData & BIT30) {
      break;
    }
  }
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT29, 0);

  //
  //   Set the starting address offset for Instruction RAM preload.
  //

  FwAddress = ACPIMMIO16 (XhciFwStarting + FW_ADDR_OFFSET + XHC_BOOT_RAM_SIZE);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth16, 0x0000, ACPIMMIO16 (XhciFwStarting + FwAddress));
  FwAddress += 2;
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04, AccessWidth16, 0x0000, FwAddress + XHC_BOOT_RAM_SIZE);

  FwSize = ACPIMMIO16 (XhciFwStarting + FW_SIZE_OFFSET + XHC_BOOT_RAM_SIZE);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG08, AccessWidth16, 0x0000, 0);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG08 + 2, AccessWidth16, 0x0000, FwSize);

  //Record XHCI Ram Address
  LocalCfgPtr->FchRunTime.XHCI_PMx04_XhciFwRomAddr_Ram = ACPIMMIO16 (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04);
  LocalCfgPtr->FchRunTime.XHCI_PMx08_xHCI_Firmware_Addr_1_Ram = ACPIMMIO32 (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG08);

  //
  // Step 3.5 to enable the instruction RAM preload functionality.
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04 + 2, AccessWidth16, 0x7FFF, 0);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT29, BIT29);

  for (;;) {
    ReadMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00 , AccessWidth32, &RegData);
    if (RegData & BIT30) {
      break;
    }
  }
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT29, 0);

  //
  // Step 4. to release resets in XHCI_ACPI_MMIO_AMD_REG00. wait for USPLL to lock by polling USPLL lock.
  //

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) U3PLL_RESET, 0);       ///Release U3PLLreset
  for (;;) {
    ReadMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00 , AccessWidth32, &RegData);
    if (RegData & U3PLL_LOCK) {
      break;
    }
  }

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) U3PHY_RESET, 0);       ///Release U3PHY
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) U3CORE_RESET, 0);      ///Release core reset

  FchXhciInitIndirectReg (LocalCfgPtr);

  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT4 + BIT5), 0);                /// Disable Device 22
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT7), BIT7);                    /// Enable 2.0 devices
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) (BIT21), BIT21);
  //
  // Step 5.
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) (BIT17 + BIT18 + BIT19), BIT17 + BIT18 + BIT19);

  //
  // PLUG/UNPLUG of USB 2.0 devices make the XHCI USB 2.0 ports unfunctional - fix enable
  // ACPI_USB3.0_REG 0x20[12:11] = 2'b11
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x3 << 11)), (UINT32) (0x3 << 11));
  //
  // XHC 2 USB2 ports interactional issue - fix enable
  // ACPI_USB3.0_REG 0x20[16] = 1'b1
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x1 << 16)), (UINT32) (0x1 << 16));
  //
  // XHC USB2.0 Ports suspend Enhancement
  // ACPI_USB3.0_REG 0x20[15] = 1'b1
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x1 << 15)), (UINT32) (0x1 << 15));
  //
  // XHC HS/FS IN Data Buffer Underflow issue - fix enable
  // ACPI_USB3.0_REG 0x20[20:18] = 0x7
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x7 << 18)), (UINT32) (0x7 << 18));
  //
  // EHCI3/OHCI3 blocks Blink Global Clock Gating when EHCI/OHCI Dev 22 fn 0/2 are disabled
  // ACPI_PMIO_F0[13] =1
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth32, ~((UINT32) (0x1 << 13)), (UINT32) (0x1 << 13));
  //
  // USB leakage current on differential lines when ports are switched to XHCI - Fix enable
  // ACPI_PMIO_F0[14] =1
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth32, ~((UINT32) (0x1 << 14)), (UINT32) (0x1 << 14));

  // RRG 8.26 XHCI Clear pending PME on Sx entry
  RwXhci0IndReg ( FCH_XHCI_IND_REG54, ~(UINT32) (BIT15), BIT15, StdHeader);

  //
  // UMI Lane Configuration Information for XHCI Firmware to Calculate the Bandwidth for USB 3.0 ISOC Devices
  //
  //if (!(IsUmiOneLaneGen1Mode (StdHeader))) { (For BTS only)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~(UINT32) (BIT25 + BIT24), BIT24);
  //}
  // RPR 8.23 FS/LS devices not functional after resume from S4 fix enable (SB02699)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~(UINT32) (BIT22), BIT22);
  // RPR 8.24 XHC USB2.0 Hub disable issue fix enable (SB02702)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT20), BIT20);
  // Set HCIVERSION to 1.0
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT15), BIT15);
  // xHCI 1.0 Sub-Features Supported
  // Blobk Event Interrupt Flag (BEI)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT1), BIT1);
  // Force Stopped Event (FSE)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT2), BIT2);
  // Software LPM
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT3), BIT3);
  // Hardware LPM
  // Skip TRB IOC Event
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT6), BIT6);
  // Remove Secondary Bandwith Domain Reporting
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT7), BIT7);
  // Cold Attach Status
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT8), BIT8);
  // Endpoint Status Update Ordering
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT9), BIT9);
  // Report Event during SKIP on Missed Service Error
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT10), BIT10);
  // Soft Retry
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT11), BIT11);
  // U3 Exit
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT12), BIT12);
  // USB3.0 Link Command
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT13), BIT13);
  // MSE FrameId invalid
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT14), BIT14);
  // Port Test Mode
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT16), BIT16);
  // Miscellaneous Design Improvements
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG40, AccessWidth32, ~(UINT32) (BIT0), BIT0);
  //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG40, AccessWidth32, ~(UINT32) (BIT25), 0);

  // XHC USB2 Loopback RX SE0
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~(UINT32) (BIT13 + BIT14 + BIT21), BIT13 + BIT14 + BIT21);
  // XHC U2IF_Enabled_Quiettermination off
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT20), BIT20);
  // XHC S0 BLM Reset Mode
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF2, AccessWidth8, ~(UINT32) (BIT3), BIT3);
  // Enhance XHC Ent_Flag
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT22), BIT22);
  // Enhance TRB Pointer when both MSE and SKIP TRB IOC evt opened
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT17), BIT17);
  // LPM Broadcast disable
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT18), BIT18);

  // Enhance XHC FS/LS connect
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT24), BIT24);
  // Enhance XHC ISOCH td_cmp
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT25), BIT25);
  // LPM Clock 5us Select
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT8), BIT8);
  // Enhance DPP ERR as XactErr
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT9), BIT9);
  // Enhance U2IF PME Enable
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT10), BIT10);
  // U2IF S3 Disconnect detection
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT12), BIT12);
  // Stream Error Handling
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT20 + BIT21 + BIT22), (BIT20 + BIT21 + BIT22));
  // FLA Deassert
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT23), BIT23);

  // Enhance LPM Host initial L1 Exit
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) BIT29, BIT29);

  // Enhance resume after disconnect
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG98, AccessWidth32, ~(UINT32) (BIT30), BIT30);
  // Enhance SS HD Detected on Plug-in during S3
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG98, AccessWidth32, ~(UINT32) (BIT31), BIT31);
  // Frame Babble Reporting
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT27), BIT27);
  // USB20PHY FL Speed select
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT28), BIT28);
  // xHCI Debug Capability
  // DCP Halt RSTSM OFF
  // Enable DCP DPH check
  // DCP LTSSM Inactive to Rxdetect
  // Enhance DCP EP State
  // DCP Remote Wakeup Capable
  // Enable ERDY send once DBC detectes HIT/HOT
  // Block HIT/HOT until service interval done
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT0 + BIT1 + BIT2), BIT0 + BIT1 + BIT2);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG120);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT3), BIT3);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT21), BIT21);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG128);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT0), BIT0);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT3 + BIT4), BIT3 + BIT4);
  // Enable Fix for DBC compatibility
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT5), BIT5);

  // Enhance SS HS detected during S3
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT1), BIT1);
  // Enhance U1 timer (shorten U1 exit response time)

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT14), BIT14);
  // Enhance HW LPM U2Entry state
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT17), BIT17);
  // Enhance SSIF PME
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT15 + BIT14), (BIT15 + BIT14));
  // U2IF Remote Wake Select
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT11), BIT11);
  // HS Data Toggle Error Handling
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT16), BIT16);
  // Enable Fix for ACPI registers write issue.
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT0), BIT0);
  // LPM related items
  // HwLpmAddrSel (22) U2LpmTranArbEn, ColdAttachMode.
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT20 + BIT21 + BIT22), (BIT20 + BIT21 + BIT22));
  // HwLpmStateSel (24), HwLpmDeconfigL1Exit (25), HwLpmL1StopEpEn (26).
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT24 + BIT25 + BIT26), (BIT24 + BIT25 + BIT26));
  // Xhci10En: xHCI 10 Enable (0), HwLpmEn (4), SkipTrbIocEvtLenMode (24)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT0 + BIT4 + BIT24), BIT4 + BIT24);
  // DcpNumP1En
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32)  BIT5, BIT5);
  // LtssmDisconnectToConnect
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT16), BIT16);
  // CcuMode
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG10, AccessWidth32, 0xFFFF00FF, 0x3D00);
  // XtalPciClk_UseEcClkSel clear
  RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG50 + 1, AccessWidth8, ~(UINT32) BIT1, 0);
  // UBTS 428056 USB 3.0 LTSSM enters into Compliance when Debug Capability is enabled
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) BIT27, BIT27);
  // UBTS 440840/ Enable Fix for Black Magic USB 3.0 device when enabled/disabled under windows
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT1 + BIT2), BIT2);
  // UBTS 444590 PIL failures when reset device command hit link td points to invalid trb
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) ( BIT3 ), BIT3);
  // UBTS 443140 Buffer overrun during S4 loop
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT17), BIT17);
  // UBTS 438072 Enable Fix for PME not generated by the U2IF logic during S3 shutdown
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT30 + BIT31), (BIT30 + BIT31));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, 0xFFFFC33F, (BIT6 + BIT7 + BIT10 + BIT11 + BIT12 + BIT13));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT30 + BIT31), (BIT30 + BIT31));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT17 + BIT18 + BIT19 + BIT20 + BIT21 + BIT22), (BIT17 + BIT18 + BIT19 + BIT20 + BIT21 + BIT22));

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT18), (BIT18));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT23), (BIT23));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT28), (BIT28));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT27), 0);

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT5), (BIT5));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT7), 0);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT14), (BIT14));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT15), (BIT15));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT5), (BIT5));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT0), 0);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT1), 0);

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT20), (BIT20));

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT19), (BIT19));

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT26), (BIT26));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT27), (BIT27));
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT25), (BIT25));

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT8), (0));

  FchUsb3Debug (LocalCfgPtr);

  if (LocalCfgPtr->Usb.ReduceUSB3PortToLastTwo) {
    RwXhci0IndReg ( FCH_XHCI_IND60_REG6C, 0, 0x02070203, StdHeader);
    RwXhci0IndReg ( FCH_XHCI_IND60_REG68, 0xffff00fe, 0x00000801, StdHeader);
  }
  // USB 3.0 Port Init
  if (LocalCfgPtr->Usb.USB30PortInit) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32,  ~(UINT32) ((0xF) << 26), (UINT32) ((LocalCfgPtr->Usb.USB30PortInit) << 26));
  }
}

/**
 * FchXhciPowerSavingProgram - Config XHCI for Power Saving mode
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchXhciPowerSavingProgram (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  // add Efuse checking for Xhci enable/disable
  // GetEfuseStatus (&XhciEfuse, StdHeader); ** Wait for BKDG update **
//  if ((XhciEfuse & (BIT0 + BIT1)) != (BIT0 + BIT1)) {
//    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, 0xF0FFFBFF, 0x0);
//  }
}
