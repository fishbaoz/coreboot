/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Integrated Debug Print Routines
 *
 * Contains all functions related to IDS Debug Print
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
 *
 ***************************************************************************/

#ifndef _IDS_DEBUGPRINT_H_
#define _IDS_DEBUGPRINT_H_

///Debug String End flag
#define DEBUG_STRING_END_FLAG (0x0)
/// return status for debug print
typedef enum {
  IDS_DEBUG_PRINT_SUCCESS = 0,      ///< success
  IDS_DEBUG_PRINT_BUFFER_OVERFLOW,      ///< Bufer overflow
} IDS_DEBUG_PRINT_STATUS;

/// Private datas for debug print
typedef struct _IDS_DEBUG_PRINT_PRIVATE_DATA {
  BOOLEAN saveContext;      /// save context
} IDS_DEBUG_PRINT_PRIVATE_DATA;

typedef BOOLEAN (*PF_IDS_DEBUG_PRINT_SUPPORT) (VOID);
typedef BOOLEAN (*PF_IDS_DEBUG_PRINT_FILTER) (UINT64 *Filter);
typedef VOID (*PF_IDS_DEBUG_PRINT_PRINT) (CHAR8 *Buffer, UINTN BufferSize, IDS_DEBUG_PRINT_PRIVATE_DATA *debugPrintPrivate);
typedef VOID (*PF_IDS_DEBUG_INIT_PRIVATE_DATA) (UINT64 flag, IDS_DEBUG_PRINT_PRIVATE_DATA *debugPrintPrivate);

/// Debug print Hw layer service class
typedef struct _IDS_DEBUG_PRINT {
  PF_IDS_DEBUG_PRINT_SUPPORT  support;    ///Check if support
  PF_IDS_DEBUG_PRINT_FILTER  customfilter;  ///Get if any customize filters
  PF_IDS_DEBUG_INIT_PRIVATE_DATA  InitPrivateData; ///Init private data
  PF_IDS_DEBUG_PRINT_PRINT  print;  ///Print data to Hw layer
} IDS_DEBUG_PRINT;


VOID
GetDebugPrintList (
  IN OUT   CONST IDS_DEBUG_PRINT   ***pIdsDebugPrintListPtr
  );


#endif //_IDS_DEBUGPRINT_H_

