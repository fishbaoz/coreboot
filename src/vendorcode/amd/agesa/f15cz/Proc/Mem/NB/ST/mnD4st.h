/* $NoKeywords:$ */
/**
 * @file
 *
 * mnD4st.h
 *
 * Northbridge ST
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/ST)
 * @e \$Revision: 312711 $ @e \$Date: 2015-02-10 19:05:39 +0800 (Tue, 10 Feb 2015) $
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

#ifndef _MND4ST_H_
#define _MND4ST_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */
#define DRAM_TYPE_DDR4_ST             2
#define MAX_D4_DCTS_PER_NODE_ST       1
/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */

VOID
MemNInitNBDataD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNSetDdrModeD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNConfigureDctForTrainingD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNConfigureDctNormalD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramMemPstateRegD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  );

VOID
MemNProgramCycTimingsD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPredriverInitD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramCadDataBusD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPhyFifoConfigD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

AGESA_STATUS
MemMD4FlowST (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  );

VOID
MemNSetPmuSequenceControlD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPopulatePmuSramConfigD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPopulatePmuSramTimingsD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNModeRegisterInitializationD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramTurnaroundTimingsD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNGetCSIntLvAddrD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 BankAddrMap,
  OUT      UINT8 *LowBit,
  OUT      UINT8 *HiBit
  );

VOID
MemNProgramDqMappingsD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

#endif  /* _MND4ST_H_ */
