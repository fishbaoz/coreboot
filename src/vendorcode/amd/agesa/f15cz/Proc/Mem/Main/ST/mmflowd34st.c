/* $NoKeywords:$ */
/**
 * @file
 *
 * mmflowd34st.c
 *
 * Main Memory initialization sequence for DDR3 and DDR4 for ST
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main/ST)
 * @e \$Revision: 312538 $ @e \$Date: 2015-02-09 16:53:54 +0800 (Mon, 09 Feb 2015) $
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
#include "Ids.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "mport.h"
#include "mm.h"
#include "mn.h"
#include "mnst.h"
#include "mnD3st.h"
#include "mnD4st.h"
#include "mt.h"
#include "mt3.h"
#include "mt4.h"
#include "mu.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mnPmuSramMsgBlockST.h"
#include "cpuFamilyTranslation.h"
#include "IdsF15StAllService.h"
#include "Filecode.h"
#include "GeneralServices.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_MAIN_ST_MMFLOWD34ST_FILECODE

#include "mftds.h"


extern MEM_FLOW_CFG MemMD3FlowST;
extern MEM_FLOW_CFG MemMD4FlowST;

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
 *
 *      This function defines the memory initialization flow for
 *      systems that support DDR3 and DDR4 and ST processor.
 *
 *     @param[in,out]   *MemMainPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          AGESA_STATUS
 *                          - AGESA_FATAL
 *                          - AGESA_CRITICAL
 *                          - AGESA_SUCCESS
 */
AGESA_STATUS
MemMFlowD34ST (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  )
{
  MEM_NB_BLOCK  *NBPtr;
  MEM_TECH_BLOCK *TechPtr;
  MEM_DATA_STRUCT *MemPtr;
  TECHNOLOGY_TYPE *TechTypePtr;

  NBPtr = MemMainPtr->NBPtr;
  TechPtr = MemMainPtr->TechPtr;
  MemPtr = MemMainPtr->MemPtr;

  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[BSP_DIE].SocketId, &(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedST (&(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid))) {
    MemPtr->IsFlowControlSupported = FALSE;
    return AGESA_FATAL;
  } else {
    MemPtr->IsFlowControlSupported = TRUE;
  }

  MemFInitTableDrive (&NBPtr[BSP_DIE], MTBeforeInitializeMCT);

  // Clear DisDllShutdownSR prior any P-State changes.
  MemNConfigureDisDllShutdownSrST (NBPtr);

  //----------------------------------------------------------------
  // Force NB-Pstate to NBP0
  //----------------------------------------------------------------
  MemNChangeNbFrequencyWrapUnb (NBPtr, 0);

  //
  // Check Installed DRAM Technology Type
  //
  TechTypePtr = (TECHNOLOGY_TYPE *) FindPSOverrideEntry (NBPtr->RefPtr->PlatformMemoryConfiguration, PSO_MEM_TECH, NBPtr->MCTPtr->SocketId, 0, 0, NULL, NULL);
  if (TechTypePtr != NULL) {
    //
    // DDR4
    //
    if (*TechTypePtr == DDR4_TECHNOLOGY) {
      if (NBPtr->MemPtr->ParameterListPtr->DimmTypeDddr4Capable == TRUE) {
        IDS_HDT_CONSOLE (MEM_FLOW, "\nAnalyze DDR4 DIMM SPD Data\n");
        if (MemTDIMMPresence4 (NBPtr[BSP_DIE].TechPtr) && (NBPtr[BSP_DIE].MCTPtr->DimmPresent != 0)) {
          //
          // Setup D4 Platform Specific Pointers here.
          //
          MemNInitNBDataD4ST (NBPtr);
          return MemMD4FlowST (MemMainPtr);
        } else {
          //
          // DDR4 requested but No DDR4 DIMMs found
          //
          IDS_HDT_CONSOLE (MEM_FLOW, "\nNo DDR4 DIMMs found.\n");
          PutEventLog (AGESA_FATAL, MEM_ERROR_NO_DIMM_FOUND_ON_SYSTEM, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
          return AGESA_FATAL;
        }
      } else {
        return FALSE;
      }
    }
    //
    // DDR3
    //
    if (*TechTypePtr == DDR3_TECHNOLOGY) {
      if (NBPtr->MemPtr->ParameterListPtr->DimmTypeDddr3Capable == TRUE) {
        IDS_HDT_CONSOLE (MEM_FLOW, "\nAnalyze DDR3 DIMM SPD Data\n");
        if (MemTDIMMPresence3 (NBPtr[BSP_DIE].TechPtr) && (NBPtr[BSP_DIE].MCTPtr->DimmPresent != 0)) {
          //
          // Setup D3 Platform Specific Pointers here.
          //
          MemNInitNBDataD3ST (NBPtr);
          return MemMD3FlowST (MemMainPtr);
        } else {
          //
          // DDR3 requested but No DDR3 DIMMs found
          //
          IDS_HDT_CONSOLE (MEM_FLOW, "\nNo DDR3 DIMMs found.\n");
          PutEventLog (AGESA_FATAL, MEM_ERROR_NO_DIMM_FOUND_ON_SYSTEM, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
          return AGESA_FATAL;
        }
      } else {
        return FALSE;
      }
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "\nNo Valid Dimms of the supported types found.\n");
    return AGESA_FATAL; // UNSUPPORTED DIMMs requested
  } else {
    IDS_HDT_CONSOLE (MEM_FLOW, "Platform BIOS with Combo DDR3/DDR4 Support must define DRAM Technology Type in the Platform Configuration Table\n");
    return AGESA_FATAL;
  }
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
