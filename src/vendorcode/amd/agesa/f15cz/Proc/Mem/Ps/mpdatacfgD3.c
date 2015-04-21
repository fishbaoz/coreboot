/* $NoKeywords:$ */
/**
 * @file
 *
 * mpdatacfgD3.c
 *
 * Sub-engine to extract DDR43 data bus settings from recommended table
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Ps)
 * @e \$Revision: 311790 $ @e \$Date: 2015-01-27 13:03:49 +0800 (Tue, 27 Jan 2015) $
 *
 **/
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
* ***************************************************************************
*
*/
/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */



#include "AGESA.h"
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "cpuFamRegisters.h"
#include "cpuRegisters.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "mu.h"
#include "ma.h"
#include "mp.h"
#include "merrhdl.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_PS_MPDATACFGD3_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define _DONT_CARE 0xFF

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Look up data Bus config tables and return the pointer to the matched entry.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *EntryOfTables     - Pointer to MEM_PSC_TABLE_BLOCK
 *
 *     @return          TRUE - Table values can be extracted per dimm population and ranks type.
 *     @return          FALSE - Table values cannot be extracted per dimm population and ranks type.
 *
 */
BOOLEAN
MemPLookupDataBusCfgTabs3 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_PSC_TABLE_BLOCK *EntryOfTables
  )
{
  UINT8   i;
  UINT8   TableSize;
  UINT32  CurDDRrate;
  UINT8   DDRVoltage;
  UINT16  RankTypeInTable;
  UINT8   PsoMaskSAO;
  UINT8   Chipsel;
  PSCFG_DATABUS_ENTRY_D3 *TblPtr;
  CH_DEF_STRUCT *CurrentChannel;

  CurrentChannel = NBPtr->ChannelPtr;

  if (CurrentChannel->TechType != DDR3_TECHNOLOGY) {
    return FALSE;
  }

  TblPtr = (PSCFG_DATABUS_ENTRY_D3 *) MemPGetTableEntry (NBPtr, EntryOfTables->TblEntryOfDramTerm, &TableSize);
  IDS_HDT_CONSOLE(MEM_FLOW,"\t\tLooking up data bus config entries\n");

  if (TblPtr != NULL) {
    CurDDRrate = (UINT32) (1 << (CurrentChannel->DCTPtr->Timings.Speed / 66));
    DDRVoltage = (UINT8) (1 << CONVERT_VDDIO_TO_ENCODED (NBPtr->RefPtr->DDRVoltage, CurrentChannel->TechType));
    IDS_HDT_CONSOLE(MEM_FLOW,"\t\t\t            NumDimmSlots   DDR Rate     VDDIO      Rank Type\n");
    IDS_HDT_CONSOLE(MEM_FLOW,"\t\t\t Current ->      %d         %08x       %02x         %x\n",
                    NBPtr->PsPtr->NumOfDimmSlots, CurDDRrate, DDRVoltage, NBPtr->PsPtr->RankType);
    for (i = 0; i < TableSize; i++) {
      RankTypeInTable = ((UINT16) TblPtr->Dimm0) | ((UINT16) TblPtr->Dimm1 << 4) | (NP << 8) | (NP << 12);
      IDS_HDT_CONSOLE(MEM_FLOW,"\t\t\t    %d            %d         %08x       %02x         %04x\n",
                      i, TblPtr->DimmPerCh, TblPtr->DDRrate, TblPtr->VDDIO, RankTypeInTable);
      if ((TblPtr->DimmPerCh & NBPtr->PsPtr->NumOfDimmSlots) != 0) {
        if ((TblPtr->DDRrate & CurDDRrate) != 0) {
          if ((TblPtr->VDDIO & DDRVoltage) != 0) {
            if ((RankTypeInTable & NBPtr->PsPtr->RankType) == NBPtr->PsPtr->RankType) {
              for (Chipsel = 0; Chipsel < MAX_CS_PER_CHANNEL; Chipsel++) {
                NBPtr->PsPtr->RttNom[Chipsel] = (UINT8) TblPtr->RttNom;
                NBPtr->PsPtr->RttWr[Chipsel] = (UINT8) TblPtr->RttWr;
              }
              NBPtr->PsPtr->DqStrength  = (UINT8) TblPtr->DqStrength;
              NBPtr->PsPtr->DqsStrength = (UINT8) TblPtr->DqsStrength;
              NBPtr->PsPtr->OdtStrength = (UINT8) TblPtr->OdtStrength;
              break;
            }
          }
        }
      }
      TblPtr++;
    }
  } else {
    i = TableSize = 0;
  }

  PsoMaskSAO = (UINT8) MemPProceedTblDrvOverride (NBPtr, NBPtr->RefPtr->PlatformMemoryConfiguration, PSO_TBLDRV_SLOWACCMODE);
  PsoMaskSAO &= (UINT8) MemPProceedTblDrvOverride (NBPtr, NBPtr->RefPtr->PlatformMemoryConfiguration, PSO_TBLDRV_ADDRTMG);
  if ((PsoMaskSAO == 0) && (i == TableSize)) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tNo data bus config entries\n");
  } else {
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\tData bus config entry found:\n");
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t\tRttnom: %x Rttwr: %x DQStr: %x DQSStr %x ODTStr: %x\n\n",
                     TblPtr->RttNom, TblPtr->RttWr, NBPtr->PsPtr->DqStrength, NBPtr->PsPtr->DqsStrength, NBPtr->PsPtr->OdtStrength);
    return TRUE;
  }

  if (NBPtr->SharedPtr->VoltageMap != VDDIO_DETERMINED) {
    return TRUE;
  }
  PutEventLog (AGESA_ERROR, MEM_ERROR_DATA_BUS_CFG_NOT_FOUND, NBPtr->Node, NBPtr->Dct, NBPtr->Channel, 0, &NBPtr->MemPtr->StdHeader);
  SetMemError (AGESA_ERROR, NBPtr->MCTPtr);
  if (!NBPtr->MemPtr->ErrorHandling (NBPtr->MCTPtr, NBPtr->Dct, EXCLUDE_ALL_CHIPSEL, &NBPtr->MemPtr->StdHeader)) {
    ASSERT (FALSE);
  }
  return FALSE;
}

