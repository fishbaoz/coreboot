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
#include "amdlib.h"
#include "cpuServices.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_KERN_KERNHWACPI_KERNHWACPILATE_FILECODE

#define AMD_CPUID_APICID_LPC_BID    0x00000001ul  // Local APIC ID, Logical Processor Count, Brand ID

extern VOID  HpetInit                          (IN VOID  *FchDataPtr);
extern VOID  GcpuRelatedSetting                (IN VOID  *FchDataPtr);
extern VOID  StressResetModeLate               (IN VOID  *FchDataPtr);


///
/// PCI_IRQ_REG_BLOCK- FCH PCI IRQ registers block
///
typedef struct _PCI_IRQ_REG_BLOCK {
  UINT8                PciIrqIndex;       // PciIrqIndex - selects which PCI interrupt to map
  UINT8                PciIrqData;        // PciIrqData  - Interrupt #
} PCI_IRQ_REG_BLOCK;

STATIC PCI_IRQ_REG_BLOCK FchInternalDeviceIrqForApicMode[] = {
    { (FCH_IRQ_INTA | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_IRQ_INTB | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_IRQ_INTC | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_IRQ_INTD | FCH_IRQ_IOAPIC), 0x13},
    { (FCH_IRQ_INTE | FCH_IRQ_IOAPIC), 0x14},
    { (FCH_IRQ_INTF | FCH_IRQ_IOAPIC), 0x15},
    { (FCH_IRQ_INTG | FCH_IRQ_IOAPIC), 0x16},
    { (FCH_IRQ_INTH | FCH_IRQ_IOAPIC), 0x17},
    { (FCH_IRQ_HDAUDIO | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_IRQ_SD | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_IRQ_IDE | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_IRQ_USB18INTB | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_IRQ_USB19INTB | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_IRQ_USB22INTB | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_IRQ_USB18INTA | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_IRQ_USB19INTA | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_IRQ_USB22INTA | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_IRQ_USB20INTC | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_IRQ_SATA | FCH_IRQ_IOAPIC), 0x13},
    { (FCH_IRQ_GPIO  | FCH_IRQ_IOAPIC), FCH_IRQ_GPIO_IRQ},
    { (FCH_IRQ_I2C0  | FCH_IRQ_IOAPIC), FCH_IRQ_I2C0_IRQ},
    { (FCH_IRQ_I2C1  | FCH_IRQ_IOAPIC), FCH_IRQ_I2C1_IRQ},
    { (FCH_IRQ_I2C2  | FCH_IRQ_IOAPIC), FCH_IRQ_I2C2_IRQ},
    { (FCH_IRQ_I2C3  | FCH_IRQ_IOAPIC), FCH_IRQ_I2C3_IRQ},
    { (FCH_IRQ_UART0 | FCH_IRQ_IOAPIC), FCH_IRQ_UART0_IRQ},
    { (FCH_IRQ_UART1 | FCH_IRQ_IOAPIC), FCH_IRQ_UART1_IRQ},
  };

#define NUM_OF_DEVICE_FOR_APICIRQ  sizeof (FchInternalDeviceIrqForApicMode) / sizeof (PCI_IRQ_REG_BLOCK)

/**
 * FchInitLateHwAcpi - Prepare HwAcpi controller to boot to OS.
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateHwAcpi (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8                  i;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  GcpuRelatedSetting (LocalCfgPtr);

  // Mt C1E Enable
  MtC1eEnable (LocalCfgPtr);

  if (LocalCfgPtr->Gpp.SerialDebugBusEnable == TRUE ) {
    RwMem (ACPI_MMIO_BASE + SERIAL_DEBUG_BASE +  FCH_SDB_REG00, AccessWidth8, 0xFF, 0x05);
  }

  StressResetModeLate (LocalCfgPtr);
  SbSleepTrapControl (FALSE);
  for (i = 0; i < NUM_OF_DEVICE_FOR_APICIRQ; i++) {
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &FchInternalDeviceIrqForApicMode[i].PciIrqIndex, StdHeader);
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &FchInternalDeviceIrqForApicMode[i].PciIrqData, StdHeader);
  }
}

