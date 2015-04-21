/* $NoKeywords:$ */
/**
 * @file
 *
 * Config FCH Hwm controller
 *
 * Init Hwm Controller features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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
#define FILECODE PROC_FCH_KERN_KERNHWM_KERNHWMENVSERVICE_FILECODE

/**
 * HwmInitRegister - Init Hardware Monitor Register.
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
HwmInitRegister (
  IN  VOID     *FchDataPtr
  )
{
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xB2, AccessWidth8, 0, 0x55);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xB3, AccessWidth8, 0, 0x55);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x91, AccessWidth8, 0, 0x55);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x92, AccessWidth8, 0, 0x55);

  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x00, AccessWidth8, 0, 0x06);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x10, AccessWidth8, 0, 0x06);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x20, AccessWidth8, 0, 0x06);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x30, AccessWidth8, 0, 0x06);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x40, AccessWidth8, 0, 0x06);

  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x66, AccessWidth8, 0, 0x01);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x6B, AccessWidth8, 0, 0x01);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x70, AccessWidth8, 0, 0x01);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x75, AccessWidth8, 0, 0x01);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0x7A, AccessWidth8, 0, 0x01);

  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xE6, AccessWidth8, 0xff, 0x02);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xF8, AccessWidth8, 0, 0x05);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xF9, AccessWidth8, 0, 0x06);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xFF, AccessWidth8, 0, 0x42);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xE9, AccessWidth8, 0, 0xFF);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xEB, AccessWidth8, 0, 0x1F);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xEF, AccessWidth8, 0, 0x04);
  RwMem (ACPI_MMIO_BASE + PMIO2_BASE + 0xFB, AccessWidth8, 0, 0x00);
  //2.9 Enhancement of FanOut0 Control
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGB6, AccessWidth8, 0x0F, 0x00);
}
