/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch USB controller
 *
 * Init USB features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 290623 $   @e \$Date: 2014-04-23 19:36:30 -0500 (Wed, 23 Apr 2014) $
 *
 */
/*;********************************************************************************
;
; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
#include "Filecode.h"
#define FILECODE PROC_FCH_AVALON_AVALONUSB_AVALONUSBENV_FILECODE

/**
 * FchSetUsbEnableReg
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchSetUsbEnableReg (
  IN  FCH_DATA_BLOCK   *FchDataPtr
  )
{
  UINT8                  UsbModeReg;
  UINT8                  XhciReg00;
  UsbModeReg = 0;

  // Overwrite EHCI3/OHCI3 by Xhci1Enable
  ReadMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth8, &XhciReg00);
  if (XhciReg00 & BIT0) {
    FchDataPtr->Usb.Ehci3Enable = FALSE;
    UsbModeReg |= 0x80;
  } else {
    UsbModeReg &= 0x7F;
  }

  UsbModeReg |= 0x15;

  if ( FchDataPtr->Usb.Ehci1Enable ) {
    UsbModeReg |= 0x02;
  }

  if ( FchDataPtr->Usb.Ehci2Enable ) {
    UsbModeReg |= 0x08;
  }

  if ( FchDataPtr->Usb.Ehci3Enable ) {
    UsbModeReg |= 0x20;
  }

  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, 0, UsbModeReg);
}

/**
 * FchInitEnvUsb - Config USB controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvUsb (
  IN  VOID     *FchDataPtr
  )
{
  //
  // Disabled All USB controller *** Move to each controller ***
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, BIT7, 0);
  //
  // Clear PM_IO 0x65[4] UsbResetByPciRstEnable, Set this bit so that usb gets reset whenever there is PCIRST.
  // Enable UsbResumeEnable (USB PME) * Default value
  // USB SleepCtrl set as BIT9+BIT8 (6 uframes)
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth16, ~(UINT32) BIT2, BIT2 + BIT7 + BIT8 + BIT9);

  FchSetUsbEnableReg (FchDataPtr);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEE, AccessWidth8, ~(UINT32) (BIT2), 0 );
}

/**
 * FchUsbOverCurrentControl - Config USB Over Current Control
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchUsbOverCurrentControl (
  IN  VOID     *FchDataPtr
  )
{
  FCH_USB_OC     *UsbOc;
  UINT32      Value32;
  UINT8       i;
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  UsbOc = &LocalCfgPtr->Usb.FchUsbOcMap;
  Value32 = 0;
  for (i = 0; i < NUM_USB1_PORTS; i++) {
    Value32 |= ((UINT32) (UsbOc->Usb1OcPinMap[i] & 0x0F) << (4 * i));
  }
  WritePci ((USB1_OHCI_BUS_DEV_FUN << 16) + FCH_EHCI_REG70, AccessWidth32, &Value32, StdHeader);

  Value32 = 0;
  for (i = 0; i < NUM_USB2_PORTS; i++) {
    Value32 |= ((UINT32) (UsbOc->Usb2OcPinMap[i] & 0x0F) << (4 * i));
  }
  WritePci ((USB2_OHCI_BUS_DEV_FUN << 16) + FCH_EHCI_REG70, AccessWidth32, &Value32, StdHeader);

  Value32 = 0;
  for (i = 0; i < NUM_USB3_PORTS; i++) {
    Value32 |= ((UINT32) (UsbOc->Usb3OcPinMap[i] & 0x0F) << (4 * i));
  }
  WritePci ((USB3_OHCI_BUS_DEV_FUN << 16) + FCH_EHCI_REG70, AccessWidth32, &Value32, StdHeader);

  Value32 = 0;
  for (i = 0; i < NUM_USB4_PORTS; i++) {
    Value32 |= ((UINT32) (UsbOc->Usb4OcPinMap[i] & 0x0F) << (4 * i));
  }
  WritePci ((USB4_OHCI_BUS_DEV_FUN << 16) + FCH_EHCI_REG70, AccessWidth32, &Value32, StdHeader);

  RwXhci0IndReg ( FCH_XHCI_IND_REG04, ~(UINT32) (0xf << 16), ((UINT32) UsbOc->Xhci0OcPinMap[0] << 16), StdHeader);
  RwXhci0IndReg ( FCH_XHCI_IND_REG04, ~(UINT32) (0xf << 20), ((UINT32) UsbOc->Xhci0OcPinMap[1] << 20), StdHeader);

  RwXhci1IndReg ( FCH_XHCI_IND_REG04, ~(UINT32) (0xf << 16), ((UINT32) UsbOc->Xhci1OcPinMap[0] << 16), StdHeader);
  RwXhci1IndReg ( FCH_XHCI_IND_REG04, ~(UINT32) (0xf << 20), ((UINT32) UsbOc->Xhci1OcPinMap[1] << 20), StdHeader);

}


