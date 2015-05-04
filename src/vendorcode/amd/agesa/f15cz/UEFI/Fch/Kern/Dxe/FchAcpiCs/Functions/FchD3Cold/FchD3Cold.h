/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE Driver
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
#ifndef _FCH_SMBUSNFC_H_
#define _FCH_SMBUSNFC_H_

#include "Acpi3_0.h"
#include "AcpiCommon.h"
#include "EfiDriverlib.h"
#include "FchPlatform.h"
#include "FchDxeCommon.h"

#include EFI_PROTOCOL_DEFINITION (PciIo)
#include EFI_PROTOCOL_DEPENDENCY (SmmBase)
#include EFI_PROTOCOL_DEPENDENCY (LoadedImage)
#include EFI_PROTOCOL_DEPENDENCY (FchInitProtocol)
#include "PspResumeServiceProtocol.h"
#include EFI_PROTOCOL_CONSUMER   (AcpiTable)
#include EFI_PROTOCOL_CONSUMER   (AcpiSystemDescriptionTable)
#include EFI_PROTOCOL_CONSUMER   (FchSmmSwDispatch)
#include EFI_PROTOCOL_CONSUMER   (AcpiSupport)
#include EFI_PROTOCOL_CONSUMER   (FchAcpiProtocol)

#define AMD_FCH_D3COLD_ACPI_GUID \
  { 0x1137BCA1, 0x6298, 0x474f, 0xAD, 0x63, 0x63, 0x24, 0x24, 0x49, 0xC8, 0xD2 }


#define HW_REDUCED_ACPI           BIT20
#define LOW_POWER_S0_IDLE_CAPABLE BIT21

EFI_STATUS
EFIAPI
FchD3ColdInit (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
);

EFI_STATUS
EFIAPI
AmdFchCSCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext
);


VOID
FchSataRestore (
  IN      FCH_DATA_BLOCK     *FchDataPtr
);

VOID
EFIAPI
FchD3ColdAcpiConfigureNotify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
);

VOID
EFIAPI
FchD3ColdAcpiInstallNotify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
);

EFI_STATUS
FchD3ColdUpdateAcpiTable (
  IN       FCH_ACPI_PROTOCOL                *FchAcpiProtocol,
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      **AcpiTable
);

EFI_STATUS
EFIAPI
FchCsResumeCallBack (
  IN  RESUME_TYPE   ResumeType,
  IN  VOID          *Context
);

VOID
EFIAPI
FchCsResumeCallBackNotify (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
);
#endif