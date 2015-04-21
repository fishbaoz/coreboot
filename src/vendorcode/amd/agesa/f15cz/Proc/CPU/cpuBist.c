/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU BIST Status Check Implementation.
 *
 * Implement CPU BIST Status checking
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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

/*----------------------------------------------------------------------------------------
 *                            M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "GeneralServices.h"
#include "cpuServices.h"
#include "cpuApComm.h"
#include "OptionApComm.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)
#define FILECODE PROC_CPU_CPUBIST_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

UINT32
STATIC
GetBistResults (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

extern OPTION_AP_COMMUNICATE_CONFIGURATION OptionApCommConfiguration;

 /*----------------------------------------------------------------------------------------
  *                          E X P O R T E D    F U N C T I O N S
  *----------------------------------------------------------------------------------------
 */

 /*---------------------------------------------------------------------------------------*/
 /**
  *
  * This function checks the status of BIST and places the error status in the event log
  * if there are any errors
  *
  * @param[in]      StdHeader              Header for library and services
  *
  * @retval         AGESA_SUCCESS          No BIST errors have been logged.
  * @retval         AGESA_ALERT            BIST errors have been detected and added to the
  *                                        event log.
  */
AGESA_STATUS
CheckBistStatus (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32                Core;
  UINT32                NumberOfCores;
  UINT32                ReturnCode;
  AGESA_STATUS          AgesaStatus;
  AP_TASK               TaskPtr;

  // Make sure that Standard Header is valid
  ASSERT (StdHeader != NULL);
  ASSERT (IsBsp (StdHeader));

  AgesaStatus = AGESA_SUCCESS;

  // Setup TaskPtr struct to execute routine on APs
  TaskPtr.FuncAddress.PfApTaskO = GetBistResults;
  TaskPtr.DataTransfer.DataSizeInDwords = 0;
  TaskPtr.ExeFlags = TASK_HAS_OUTPUT | WAIT_FOR_CORE;

  GetActiveCoresInCurrentSocket (&NumberOfCores, StdHeader);
  for (Core = 0; Core < NumberOfCores; Core++) {
    if (Core != 0) {
      ReturnCode = OptionApCommConfiguration.ApRunCodeOnSocketCore (0, (UINT8)Core, &TaskPtr, StdHeader);
    } else {
      ReturnCode = TaskPtr.FuncAddress.PfApTaskO (StdHeader);
    }

    // If BIST value is non-zero, add to BSP's event log
    if (ReturnCode != 0) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  BIST failure: core %d, status = 0x%x\n", Core, ReturnCode);
      AgesaStatus = AGESA_ALERT;
      PutEventLog (AGESA_ALERT,
                   CPU_EVENT_BIST_ERROR,
                   ReturnCode, 0, Core, 0, StdHeader);
    }
  }

  return AgesaStatus;
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L          F U N C T I O N S
 *----------------------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------------------*/
/**
 *
 * Reads the lower 32 bits of the BIST register
 *
 * @param[in]      StdHeader              Header for library and services
 *
 * @retval         Value of the BIST register
*/
UINT32
STATIC
GetBistResults (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64   BistResults;

  // Read MSRC001_0060 BIST Results Register
  LibAmdMsrRead (MSR_BIST, &BistResults, StdHeader);

  return (UINT32) (BistResults & 0xFFFFFFFF);
}
