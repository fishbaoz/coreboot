/* $NoKeywords:$ */
/**
 * @file
 *
 *
 * Contains code that used for Ids Internal Dxe phase
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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
#include "EfiDriverLib.h"
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "IdsInternalLib.h"
#include "IdsLib.h"
#include "heapManager.h"

#define FILECODE 0xE814

/*---------------------------------------------------------------------------------------*/
/**
 *  BvmReadyToBoot
 *
 * Description
 *    BVM hook of event for gEfiEventReadyToBootGuid, when this event been signaled before booting OS
 *    It will be called.
 *
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 */
VOID
BvmReadyToBoot (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  if (StdHeader->HeapBasePtr != 0) {
    IdsBvmOptionHook (BVM_PLATFORM_TP_BEFORE_INT19, NULL, StdHeader);
  }
  AGESA_TESTPOINT (TpReadyToBoot, StdHeader);
}
/*---------------------------------------------------------------------------------------*/
/**
 *  IdsReadyToBootEntry
 *
 * Description
 *    Notify event for gEfiEventReadyToBootGuid, when this event been signaled before booting OS
 *    It will be called.
 *
 *  @param[in]      Event
 *  @param[in,out]  Context
 *
 *
 */
VOID
IdsReadyToBootEntry (
  IN       EFI_EVENT  Event,
  IN       VOID       *Context
  )
{
  AMD_CONFIG_PARAMS StdHeader;
  UINTN Length;
  UINT8 * Tmp;

  Length = sizeof (StdHeader);
  Tmp = (UINT8*) &StdHeader;
  while (Length --) {
    *Tmp++ = 0;
  }

  StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
  StdHeader.HeapBasePtr = HeapGetBaseAddress (&StdHeader);

  BvmReadyToBoot (&StdHeader);

}
/*---------------------------------------------------------------------------------------*/
/**
 *  UefiIdsIntLateAfter
 *
 * Description
 *    Create the event for gEfiEventReadyToBootGuid
 *
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 */
VOID
UefiIdsIntLateAfter (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  EFI_EVENT                        EventReadyToBoot;

  // Ready to Boot Event
  gBS->CreateEventEx (
                  EFI_EVENT_NOTIFY_SIGNAL,
                  EFI_TPL_CALLBACK,
                  IdsReadyToBootEntry,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &EventReadyToBoot
                  );
}
/*---------------------------------------------------------------------------------------*/
/**
 *  IdsInternalGetDataDxe
 *
 * Description
 *    Internal hook for AmdGetIdsData
 *
 *  @param[in]   Data    Indentify the input function ID.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 */
VOID
IdsInternalGetDataDxe (
  IN       UINTN              Data,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  if (Data == IDS_CALLOUT_INT_LATE_AFTER) {
    UefiIdsIntLateAfter (StdHeader);
  }
}
