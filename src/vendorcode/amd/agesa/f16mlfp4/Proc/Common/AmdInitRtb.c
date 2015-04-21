/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD AGESA Basic Level Public APIs
 *
 * Contains basic Level Initialization routines.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Interface
 * @e \$Revision: 284306 $   @e \$Date: 2014-02-10 00:20:07 -0600 (Mon, 10 Feb 2014) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "OptionS3.h"
#include "CommonInits.h"
#include "AmdFch.h"
#include "Filecode.h"
#include "heapManager.h"
#include "cpuFeatures.h"
#include "CreateStruct.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_COMMON_AMDINITRTB_FILECODE

extern BLDOPT_FCH_FUNCTION  BldoptFchFunction;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

extern CONST BLDOPT_S3SAVE_FUNCTION ROMDATA BldoptS3SaveFunction;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
AGESA_STATUS
AmdInitRtbPlatformConfigInit (
  IN OUT   PLATFORM_CONFIGURATION    *PlatformConfig,
  IN OUT   AMD_CONFIG_PARAMS         *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Main entry point for the AMD_INIT_RTB function.
 *
 * This entry point is called very late in POST at the Ready-To-Boot (RTB) time.
 * It is responsible for last minute configuration and saving silicon component
 * registers to the SMM save area in preparation of entering system suspend-to-RAM mode.
 *
 * @param[in,out] AmdInitRtbParams  Required input parameters for the AMD_INIT_RTB
 *                                  entry point.
 *
 * @return        Aggregated status across all internal AMD ready to boot calls invoked.
 *
 */
AGESA_STATUS
AmdInitRtb (
  IN OUT   AMD_RTB_PARAMS *AmdInitRtbParams
  )
{
  AGESA_STATUS            ReturnStatus;
  AGESA_STATUS            AgesaStatus;

  ASSERT (AmdInitRtbParams != NULL);
  AGESA_TESTPOINT (TpIfAmdInitRtbEntry, &AmdInitRtbParams->StdHeader);
  IDS_OPTION_HOOK (IDS_INIT_RTB_BEFORE, AmdInitRtbParams, &(AmdInitRtbParams->StdHeader));

  ReturnStatus = AGESA_SUCCESS;

  AgesaStatus = BldoptS3SaveFunction.S3SaveEntry (AmdInitRtbParams);
  if (AgesaStatus > ReturnStatus) {
    ReturnStatus = AgesaStatus;
  }

  AgesaStatus = BldoptFchFunction.InitLate (AmdInitRtbParams);
  if (AgesaStatus > ReturnStatus) {
    ReturnStatus = AgesaStatus;
  }

  // Cpu Features
  IDS_PERF_TIMESTAMP (TP_BEGINDISPATCHCPUFEATURESINITRTB, &AmdInitRtbParams->StdHeader);
  IDS_HDT_CONSOLE (MAIN_FLOW, "DispatchCpuFeatures: InitRtbStart\n");
  AgesaStatus = DispatchCpuFeatures (CPU_FEAT_INIT_RTB_END, &AmdInitRtbParams->PlatformConfig, &AmdInitRtbParams->StdHeader);
  IDS_HDT_CONSOLE (MAIN_FLOW, "DispatchCpuFeatures: InitRtbEnd\n");
  if (AgesaStatus > ReturnStatus) {
    ReturnStatus = AgesaStatus;
  }
  IDS_PERF_TIMESTAMP (TP_ENDDISPATCHCPUFEATURESINITRTB, &AmdInitRtbParams->StdHeader);

  IDS_OPTION_HOOK (IDS_INIT_RTB_AFTER, AmdInitRtbParams, &AmdInitRtbParams->StdHeader);
  AGESA_TESTPOINT (TpIfAmdInitRtbExit, &AmdInitRtbParams->StdHeader);

  return  ReturnStatus;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Constructor for the AMD_INIT_RTB function.
 *
 * This routine is responsible for setting default values for the
 * input parameters needed by the AMD_INIT_RTB entry point.
 *
 * @param[in]    StdHeader       The standard header.
 * @param[in,out] RtbParams   Required input parameters for the AMD_INIT_RTB
 *                               entry point.
 *
 * @retval        AGESA_SUCCESS  Always Succeeds.
 *
 */
AGESA_STATUS
AmdInitRtbInitializer (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN OUT   AMD_RTB_PARAMS *RtbParams
  )
{
  ASSERT (StdHeader != NULL);
  ASSERT (RtbParams != NULL);

  RtbParams->StdHeader = *StdHeader;
  // Assuming no S3 save data
  RtbParams->S3DataBlock.VolatileStorageSize = 0x00000000;
  RtbParams->S3DataBlock.NvStorageSize = 0x00000000;
  BldoptS3SaveFunction.S3SaveInitializer (StdHeader, &RtbParams->S3DataBlock);

  AmdInitRtbPlatformConfigInit (&RtbParams->PlatformConfig, &RtbParams->StdHeader);

  BldoptFchFunction.InitLateConstructor (RtbParams);

  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Destructor for the AMD_INIT_RTB function.
 *
 * This routine is responsible for deallocation of heap space allocated during
 * AMD_S3_SAVE entry point.
 *
 * @param[in]     StdHeader       The standard header.
 * @param[in,out] RtbParams   Required input parameters for the AMD_INIT_RESUME
 *                               entry point.
 *
 * @retval        AGESA_STATUS
 *
 */
AGESA_STATUS
AmdInitRtbDestructor (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN OUT   AMD_RTB_PARAMS    *RtbParams
  )
{
  AGESA_STATUS ReturnStatus;
  AGESA_STATUS RetVal;

  ASSERT (RtbParams != NULL);

  ReturnStatus = AGESA_SUCCESS;

  RetVal = BldoptS3SaveFunction.S3SaveDestructor (StdHeader, &RtbParams->S3DataBlock);
  if (RetVal > ReturnStatus) {
    ReturnStatus = RetVal;
  }

  return ReturnStatus;
}

/*------------------------------------------------------------------------------------*/
/**
 * Initialize AmdInitRtb stage platform profile and user option input.
 *
 * @param[in,out]   PlatformConfig   Platform profile/build option config structure
 * @param[in,out]   StdHeader        AMD standard header config param
 *
 * @retval          AGESA_SUCCESS    Always Succeeds.
 *
 */
AGESA_STATUS
AmdInitRtbPlatformConfigInit (
  IN OUT   PLATFORM_CONFIGURATION    *PlatformConfig,
  IN OUT   AMD_CONFIG_PARAMS         *StdHeader
  )
{
  CommonPlatformConfigInit (PlatformConfig, StdHeader);

  return AGESA_SUCCESS;
}
