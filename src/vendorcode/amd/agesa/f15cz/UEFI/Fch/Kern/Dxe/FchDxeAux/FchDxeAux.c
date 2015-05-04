/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE AUX Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE AUX Driver
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
/*****************************************************************************
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
 ******************************************************************************
 */
#include "Tiano.h"
#include "Filecode.h"
#define FILECODE UEFI_FCH_KERN_DXE_FCHDXEAUX_FCHDXEAUX_FILECODE

#include "FchDxeAux.h"

#include EFI_PROTOCOL_PRODUCER (FchDxeAuxInitProtocol)
//
// Driver Global Data
//
EFI_DRIVER_ENTRY_POINT (FchDxeAux);

/*********************************************************************************
 * Name: FchDxeAuxInit
 *
 * Description
 *   Entry point of the AMD FCH DXE AUX driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchDxeAuxInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status;
  FCH_DXE_AUX_PRIVATE     *FchPrivate;
  EFI_HANDLE              Handle;
  //FCH_DATA_BLOCK          *FchDataBlock;

  //
  // Initialize EFI library
  //
  Status = LibFchDxeInitialization (SystemTable);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize the configuration structure and private data area
  //
  // Allocate memory for the private data
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  sizeof (FCH_DXE_AUX_PRIVATE),
                  &FchPrivate
                  );

  ASSERT_EFI_ERROR (Status);

  // Initialize the private data structure
  FchPrivate->Signature                             = FCH_DXE_AUX_PRIVATE_DATA_SIGNATURE;

  // Initialize the FCH Dxe Aux Init protocol
  FchPrivate->FchDxeAuxInit.Revision                = FCH_DXE_AUX_INIT_REV;
  FchPrivate->FchDxeAuxInit.FchRev                  = FCH_VERSION;
  FchPrivate->FchDxeAuxInit.FpFchDxeAuxGpio         = FchDxeAuxGpio;
  FchPrivate->FchDxeAuxInit.FpFchDxeAuxGpioRead     = FchDxeAuxGpioRead;
  FchPrivate->FchDxeAuxInit.FpFchDxeAuxBootTimer    = FchDxeAuxBootTimer;
  FchPrivate->FchDxeAuxInit.FpFchDxeAuxDisUsbPort   = FchDxeDisUsbPort;
  FchPrivate->FchDxeAuxInit.FpFchDxeAuxEsataPort    = FchDxeEsataPortSet;

  //
  // Publish the FchDxeAux protocol
  //
  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gFchDxeAuxInitProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &FchPrivate->FchDxeAuxInit
                  );

  if (EFI_ERROR (Status)) {
    return (Status);
  }
  return (Status);
}

