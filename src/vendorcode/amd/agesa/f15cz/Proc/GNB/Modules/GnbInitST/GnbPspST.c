/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB PSP access functions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2015-02-04 14:39:01 +0800 (Wed, 04 Feb 2015) $
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
* ***************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Ids.h"
#include  "amdlib.h"
#include  "Gnb.h"
#include  "OptionGnb.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbTable.h"
#include  "GnbSmuInitLibV8.h"
#include  "GnbRegisterAccST.h"
#include  "GnbRegistersST.h"
#include  "GnbPspST.h"
#include  "PspBaseLib.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITST_GNBPSPST_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Get PSP directory base address
 *
 *
 *
 * @param[in, out]  Address  Pointer to PSP diretory base address
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
STATIC
GetPspDirBaseST (
  IN OUT   UINTN     *Address
  )
{
  AGESA_STATUS              Status;
  AGESA_STATUS              AgesaStatus;
  UINTN                     i;
  FIRMWARE_ENTRY_TABLE      *FirmwareTableBase;
  CONST UINTN RomSigAddrTable[] =
  {
    0xFFFA0000, //  --> 512KB base
    0xFFF20000, //  --> 1MB base
    0xFFE20000, //  --> 2MB base
    0xFFC20000, //  --> 4MB base
    0xFF820000, //  --> 8MB base
    0xFF020000  //  --> 16MB base
  };

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  for (i = 0; i < sizeof (RomSigAddrTable) / sizeof (UINTN); i++) {
    FirmwareTableBase  = (FIRMWARE_ENTRY_TABLE *) (UINTN) RomSigAddrTable[i];
    // Search flash for unique signature 0x55AA55AA
    if (FirmwareTableBase->Signature  == FIRMWARE_TABLE_SIGNATURE) {
      IDS_HDT_CONSOLE (GNB_TRACE, " PSP FirmwareTableBase @%08x\n", RomSigAddrTable[i]);
      *Address = (UINTN)(FirmwareTableBase->PspDirBase);
      break;
    }
  }

  if (*Address == NULL) {
    Status = AGESA_ERROR;
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }

  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get SCS data
 *
 *
 * @param[in, out]  EntryAddress  Pointer to SCS data
 * @param[in, out]  EntrySize  The size of SCS data
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbGetScsDataST (
  IN OUT   UINTN        *EntryAddress,
  IN OUT   UINTN        *EntrySize
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  PSP_DIRECTORY     *PspDir;
  UINTN             i;

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  PspDir = NULL;
  Status = GetPspDirBaseST ((UINTN *)&PspDir);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  for (i = 0; i < PspDir->Header.TotalEntries; i++) {
    if (PspDir->PspEntry[i].Type == SCS_BIN_SIGNATURE) {
      *EntryAddress = (UINTN) PspDir->PspEntry[i].Location;
      *EntrySize = PspDir->PspEntry[i].Size;
      Status = AGESA_SUCCESS;
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
      break;
    }
  }

  return  AgesaStatus;
}


