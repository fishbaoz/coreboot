/* $NoKeywords:$ */
/**
 * @file
 *
 * AGESA Debug NULL Library
 *
 * Dummy instance for AGESA Debug functions, Used for fix build issue when
 * When Driver only reference amdlib without IDS debuglib
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */
/*
 ******************************************************************************
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
/**
 *  IDS Backend Function for ASSERT
 *
 * Halt execution with stop code display.  Stop Code is displayed on port 80, with rotation so that
 * it is visible on 8, 16, or 32 bit display.  The stop code is alternated with 0xDEAD on the display,
 * to help distinguish the stop code from a post code loop.
 * Additional features may be available if using simulation.
 *
 * @param[in]     FileCode    File code(define in FILECODE.h) mix with assert Line num.
 *
 * @retval         TRUE     No error
 **/
BOOLEAN
IdsAssert (
  IN       UINT32 FileCode
  )
{
  return TRUE;
}
/**
 *  Prints string to debug host like printf in C
 *
 *  @param[in] Flag    - filter flag
 *  @param[in] *Format - format string
 *  @param[in] ... Variable parameter
 *
**/
VOID
AmdIdsDebugPrint (
  IN       UINT64      Flag,
  IN       CONST CHAR8 *Format,
  IN       ...
  )
{
}

/**
 *  Prints memory debug strings
 *
 *  @param[in] *Format - format string
 *  @param[in] ... Variable parameter
 *
**/
VOID
AmdIdsDebugPrintMem (
  IN       CONST CHAR8 *Format,
  IN       ...
  )
{
}

/**
 *  Prints CPU debug strings
 *
 *  @param[in] *Format - format string
 *  @param[in] ... Variable parameter
 *
**/
VOID
AmdIdsDebugPrintCpu (
  IN       CONST CHAR8 *Format,
  IN       ...
  )
{
}

/**
 *  Prints Topology debug strings
 *
 *  @param[in] *Format - format string
 *  @param[in] ... Variable parameter
 *
**/
VOID
AmdIdsDebugPrintTopology (
  IN       CONST CHAR8 *Format,
  IN       ...
  )
{
}

/**
 *  Prints GNB debug strings
 *
 *  @param[in] *Format - format string
 *  @param[in] ... Variable parameter
 *
**/
VOID
AmdIdsDebugPrintGnb (
  IN       CONST CHAR8 *Format,
  IN       ...
  )
{
}

/**
 *  Prints All debug strings
 *
 *  @param[in] *Format - format string
 *  @param[in] ... Variable parameter
 *
**/
VOID
AmdIdsDebugPrintAll (
  IN       CONST CHAR8 *Format,
  IN       ...
  )
{
}
/**
 *  IDS Backend Function for Memory timeout control
 *
 *  This function is used to override Memory timeout control.
 *
 *  @param[in,out]   DataPtr      The Pointer of UINT8.
 *
 **/
VOID
IdsMemTimeOut (
  IN OUT   VOID *DataPtr
  )
{
}