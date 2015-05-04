/* $NoKeywords:$ */
/**
 * @file
 *
 * mfspr.c
 *
 * Feature enable online spare
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/Olspare)
 * @e \$Revision: 309090 $ @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
#include "mm.h"
#include "mn.h"
#include "mfspr.h"
#include "Ids.h"
#include "amdlib.h"
#include "Filecode.h"
#include "GeneralServices.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_OLSPARE_MFSPR_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

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
 *  Enable online spare on current node if it is requested.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  This feature is enabled.
 *     @return          FALSE - This feature is not enabled.
 */

BOOLEAN
MemFOnlineSpare (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;
  UINT8 q;
  UINT8  Value8;
  BOOLEAN Flag;
  BOOLEAN OnlineSprEnabled[MAX_CHANNELS_PER_SOCKET];

  MEM_PARAMETER_STRUCT *RefPtr;
  DIE_STRUCT *MCTPtr;

  ASSERT (NBPtr != NULL);

  RefPtr = NBPtr->RefPtr;
  Flag = FALSE;
  if (RefPtr->EnableOnLineSpareCtl != 0) {
    RefPtr->GStatus[GsbEnDIMMSpareNW] = TRUE;
    MCTPtr = NBPtr->MCTPtr;

    // Check if online spare can be enabled on current node
    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      ASSERT (Dct < sizeof (OnlineSprEnabled));
      NBPtr->SwitchDCT (NBPtr, Dct);
      OnlineSprEnabled[Dct] = FALSE;
      if ((MCTPtr->GangedMode == 0) || (MCTPtr->Dct == 0)) {
        if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
          // Make sure at least two chip-selects are available
          Value8 = LibAmdBitScanReverse (NBPtr->DCTPtr->Timings.CsEnabled);
          if (Value8 > LibAmdBitScanForward (NBPtr->DCTPtr->Timings.CsEnabled)) {
            OnlineSprEnabled[Dct] = TRUE;
            Flag = TRUE;
          } else {
            PutEventLog (AGESA_ERROR, MEM_ERROR_DIMM_SPARING_NOT_ENABLED, NBPtr->Node, NBPtr->Dct, NBPtr->Channel, 0, &NBPtr->MemPtr->StdHeader);
            MCTPtr->ErrStatus[EsbSpareDis] = TRUE;
          }
        }
      }
    }

    // If we don't have spared rank on any DCT, we don't run the rest part of the code.
    if (!Flag) {
      return FALSE;
    }

    MCTPtr->NodeMemSize = 0;
    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      NBPtr->SwitchDCT (NBPtr, Dct);
      if (OnlineSprEnabled[Dct]) {
        // Only run StitchMemory if we need to set a spare rank.
        NBPtr->DCTPtr->Timings.DctMemSize = 0;
        for (q = 0; q < MAX_CS_PER_CHANNEL; q++) {
          NBPtr->SetBitField (NBPtr, BFCSBaseAddr0Reg + q, 0);
        }
        Flag = NBPtr->StitchMemory (NBPtr);
        ASSERT (Flag == TRUE);
      } else if ((MCTPtr->GangedMode == 0) && (NBPtr->DCTPtr->Timings.DctMemSize != 0)) {
        // Otherwise, need to adjust the memory size on the node.
        MCTPtr->NodeMemSize += NBPtr->DCTPtr->Timings.DctMemSize;
        MCTPtr->NodeSysLimit = MCTPtr->NodeMemSize - 1;
      }
    }
    return TRUE;
  } else {
    return FALSE;
  }
}
