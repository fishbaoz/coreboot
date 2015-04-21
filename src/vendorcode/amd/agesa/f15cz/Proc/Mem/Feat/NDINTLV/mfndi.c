/* $NoKeywords:$ */
/**
 * @file
 *
 * mfndi.c
 *
 * Feature applies Node memory interleaving
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/Ndintlv)
 * @e \$Revision: 309090 $ @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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
#include "amdlib.h"
#include "mm.h"
#include "mn.h"
#include "mport.h"
#include "mfndi.h"
#include "Ids.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_NDINTLV_MFNDI_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define _4GB_ (0x10000ul)

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
 *  Perform a check to see if node interleaving can be enabled on each node.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  Node interleaving can be enabled.
 *     @return          FALSE - Node interleaving cannot be enabled.
 */

BOOLEAN
MemFCheckInterleaveNodes (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  DIE_STRUCT *MCTPtr;

  ASSERT (NBPtr != NULL);

  MCTPtr = NBPtr->MCTPtr;

  if (MCTPtr->NodeMemSize != 0) {
    if (!NBPtr->SharedPtr->NodeIntlv.IsValid) {
      NBPtr->SharedPtr->NodeIntlv.NodeMemSize = MCTPtr->NodeMemSize;
      NBPtr->SharedPtr->NodeIntlv.Dct0MemSize = MCTPtr->DctData[0].Timings.DctMemSize;
      NBPtr->SharedPtr->NodeIntlv.IsValid = TRUE;
    } else {
      if ((NBPtr->SharedPtr->NodeIntlv.NodeMemSize != MCTPtr->NodeMemSize) ||
          (NBPtr->SharedPtr->NodeIntlv.Dct0MemSize != MCTPtr->DctData[0].Timings.DctMemSize)) {
        return FALSE;
      }
    }
  } else {
    // Only when all nodes have memory will node interleaving be enabled
    return FALSE;
  }
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  Applies Node memory interleaving for each node.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  This feature is enabled.
 *     @return          FALSE - This feature is not enabled.
 */

BOOLEAN
MemFInterleaveNodes (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 NodeCnt;
  UINT8 BitShift;
  UINT32 MemSize;
  UINT32 Dct0MemSize;
  UINT32 NodeSysBase;
  UINT32 NodeSysLimit;
  UINT32 HoleBase;
  UINT32 HoleSize;
  UINT32 HoleOffset;
  S_UINT64 SMsr;
  MEM_PARAMETER_STRUCT *RefPtr;
  DIE_STRUCT *MCTPtr;

  ASSERT (NBPtr != NULL);

  RefPtr = NBPtr->RefPtr;
  MCTPtr = NBPtr->MCTPtr;
  if (RefPtr->GStatus[GsbSoftHole] || RefPtr->GStatus[GsbHWHole]) {
    HoleBase = RefPtr->HoleBase;
    HoleSize = _4GB_ - HoleBase;
  } else {
    HoleBase = 0;
    HoleSize = 0;
  }

  NodeCnt = NBPtr->SharedPtr->NodeIntlv.NodeCnt;
  Dct0MemSize = NBPtr->SharedPtr->NodeIntlv.Dct0MemSize;
  MemSize = NBPtr->SharedPtr->NodeIntlv.NodeMemSize;

  BitShift = LibAmdBitScanForward (NodeCnt);
  Dct0MemSize <<= BitShift;
  if (HoleSize != 0) {
    RefPtr->GStatus[GsbHWHole] = TRUE;
    HoleOffset = HoleSize;
    if (Dct0MemSize >= HoleBase) {
      Dct0MemSize += HoleSize;
    } else {
      HoleOffset += Dct0MemSize;
    }
  } else {
    HoleOffset = 0;
  }

  MemSize = (MemSize << BitShift) + HoleSize;

  MCTPtr->NodeSysBase = 0;
  MCTPtr->NodeSysLimit = MemSize - 1;
  RefPtr->SysLimit = MemSize - 1;

  // When node interleaving is enabled with larger than 1012GB memory,
  // system memory limit will be lowered to fill in HT reserved region.
  // TOP_MEM2 was set in CpuMemTyping and needs to be updated as well.
  if (RefPtr->SysLimit >= HT_REGION_BASE_RJ16) {
    if (RefPtr->LimitMemoryToBelow1Tb) {
      SMsr.hi = HT_REGION_BASE_RJ16 >> (32 - 16);
      SMsr.lo = HT_REGION_BASE_RJ16 << 16;
    } else {
      SMsr.hi = MemSize >> (32 - 16);
      SMsr.lo = MemSize << 16;
    }
    LibAmdMsrWrite (TOP_MEM2, (UINT64 *)&SMsr, &(NBPtr->MemPtr->StdHeader));
    IDS_HDT_CONSOLE (MEM_FLOW, "TOP_MEM2: %08x0000\n", MemSize);
    RefPtr->Sub1THoleBase =  HT_REGION_BASE_RJ16;
    RefPtr->SysLimit = HT_REGION_BASE_RJ16 - 1;
  } else {
    RefPtr->Sub1THoleBase =  RefPtr->SysLimit + 1;
  }

  NBPtr->SetBitField (NBPtr, BFDramIntlvSel, NBPtr->SharedPtr->NodeIntlv.NodeIntlvSel);
  NBPtr->SetBitField (NBPtr, BFDramBaseAddr, 0);
  NBPtr->SetBitField (NBPtr, BFDramIntlvEn, NodeCnt - 1);
  NBPtr->SetBitField (NBPtr, BFDramLimitAddr, (MemSize - 1) >> (27 - 16));

  if (HoleSize != 0) {
    MCTPtr->Status[SbHWHole] = TRUE;
    // DramHoleBase will be set when sync address map to other nodes.
    NBPtr->SetBitField (NBPtr, BFDramHoleOffset, HoleOffset >> (23 - 16));
    NBPtr->SetBitField (NBPtr, BFDramHoleValid, 1);
  }

  if ((MCTPtr->DctData[1].Timings.DctMemSize != 0) && (!NBPtr->Ganged)) {
    NBPtr->SetBitField (NBPtr, BFDctSelBaseAddr, Dct0MemSize >> (27 - 16));
    NBPtr->SetBitField (NBPtr, BFDctSelBaseOffset, Dct0MemSize >> (26 - 16));
  }

  NodeSysBase = NodeCnt - 1;
  NodeSysLimit = ((MemSize - 1)& 0xFFFFFF00) | NBPtr->SharedPtr->NodeIntlv.NodeIntlvSel;
  NBPtr->SharedPtr->NodeMap[NBPtr->Node].IsValid = TRUE;
  NBPtr->SharedPtr->NodeMap[NBPtr->Node].SysBase = NodeSysBase;
  NBPtr->SharedPtr->NodeMap[NBPtr->Node].SysLimit = NodeSysLimit;

  NBPtr->SharedPtr->NodeIntlv.NodeIntlvSel++;
  return TRUE;
}
