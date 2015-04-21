/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU services
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 311051 $   @e \$Date: 2015-01-16 10:18:15 +0800 (Fri, 16 Jan 2015) $
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
#include  "S3SaveState.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "OptionGnb.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbSmuInitLibV8.h"
#include  "heapManager.h"
#include  "GnbFamServices.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBSMULIBV8_GNBSMUINITLIBV8_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#ifndef INVALID_SMU_MSG
  #define INVALID_SMU_MSG  0xFF
#endif

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

// Needed for floating point support, linker expects this symbol to be defined.
CONST INT32 _fltused = 0;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
GnbSmuServiceCommonInitArgumentsV8 (
  IN       GNB_HANDLE               *GnbHandle,
  IN       AMD_CONFIG_PARAMS        *StdHeader,
  IN OUT   DEV_OBJECT               *DevObject,
  IN OUT   UINT32                   *SmuArg
  )
{
  DevObject->GnbHandle = GnbHandle;
  DevObject->StdHeader = StdHeader;
  DevObject->DevPciAddress = GnbGetHostPciAddress (GnbHandle);
  LibAmdMemFill (SmuArg, 0x00, 24, StdHeader);
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU service request
 *
 *
 * @param[in]       DevObject       Pointer to Device object
 * @param[in]       RequestId       Request ID
 * @param[in, out]  RequestArgument Request Argument
 * @param[in]       AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 */

VOID
GnbSmuServiceRequestV8 (
  IN       DEV_OBJECT               *DevObject,
  IN       UINT32                   RequestId,
  IN OUT   UINT32                   *RequestArgument,
  IN       UINT32                   AccessFlags
  )
{
  UINT32                      RxSmuMessageResponse;
  GNB_BUILD_OPTIONS_COMMON    *GnbCommonOptions;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuServiceRequestV8 Enter\n");

  GnbCommonOptions = (GNB_BUILD_OPTIONS_COMMON*) GnbFmGnbBuildOptions (DevObject->StdHeader);

  if (!GnbCommonOptions->CfgSmuMessageEnable) {
    IDS_HDT_CONSOLE (GNB_TRACE, "BIOS FORCE SKIP SMU SERVICE!!\n");
    return;
  }

  if (RequestId == INVALID_SMU_MSG) {
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuServiceRequestV8 INVALID!!\n");
    return;
  }

  IDS_HDT_CONSOLE (NB_MISC, "  Service Request 0x%x\n", RequestId);
  IDS_HDT_CONSOLE (NB_MISC, "  Service Request Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", RequestArgument[0], RequestArgument[1], RequestArgument[2], RequestArgument[3], RequestArgument[4], RequestArgument[5]);

  if ((AccessFlags & GNB_REG_ACC_FLAG_S3SAVE) != 0) {
    SMU_MSG_CONTEXT_V8 SmuMsgContext;
    SmuMsgContext.GnbPciAddress.AddressValue = DevObject->DevPciAddress.AddressValue;
    SmuMsgContext.RequestId = RequestId;
    SmuMsgContext.RequestArgument[0] = RequestArgument[0];
    SmuMsgContext.RequestArgument[1] = RequestArgument[1];
    SmuMsgContext.RequestArgument[2] = RequestArgument[2];
    SmuMsgContext.RequestArgument[3] = RequestArgument[3];
    SmuMsgContext.RequestArgument[4] = RequestArgument[4];
    SmuMsgContext.RequestArgument[5] = RequestArgument[5];
    S3_SAVE_DISPATCH (DevObject->StdHeader, GnbSmuServiceRequestV8S3Script_ID, sizeof (SmuMsgContext), &SmuMsgContext);
  }

  GnbUraGet (DevObject, TRxSmuMessageResponse, &RxSmuMessageResponse);
  // Log last time response status
  WriteIo8 (0x85, (UINT8) RxSmuMessageResponse);

  // 1 Clear Response
  RxSmuMessageResponse = 0;
  GnbUraSet (DevObject, TRxSmuMessageResponse, &RxSmuMessageResponse);

  do {
    GnbUraGet (DevObject, TRxSmuMessageResponse, &RxSmuMessageResponse);
  } while (RxSmuMessageResponse != 0x0);

  // 2 Write message arguments
  GnbUraSet (DevObject, TRxSmuMessageArgument0, &RequestArgument[0]);
  GnbUraSet (DevObject, TRxSmuMessageArgument1, &RequestArgument[1]);
  GnbUraSet (DevObject, TRxSmuMessageArgument2, &RequestArgument[2]);
  GnbUraSet (DevObject, TRxSmuMessageArgument3, &RequestArgument[3]);
  GnbUraSet (DevObject, TRxSmuMessageArgument4, &RequestArgument[4]);
  GnbUraSet (DevObject, TRxSmuMessageArgument5, &RequestArgument[5]);
  WriteIo32 (0x88, RequestArgument[0]);

  // 3 Write message ID
  GnbUraSet (DevObject, TRxSmuMessageId, &RequestId);
  WriteIo8 (0x84, (UINT8) RequestId);

  // 4 Poll Response until non-zero
  WriteIo8 (0x87, 0xff);
  do {
    GnbUraGet (DevObject, TRxSmuMessageResponse, &RxSmuMessageResponse);
    WriteIo8 (0x86, (UINT8) RxSmuMessageResponse);
  } while (RxSmuMessageResponse == 0x0);
  WriteIo8 (0x87, 0);

  // 5 Read updated SMU message arguments
  GnbUraGet (DevObject, TRxSmuMessageArgument0, &RequestArgument[0]);
  GnbUraGet (DevObject, TRxSmuMessageArgument1, &RequestArgument[1]);
  GnbUraGet (DevObject, TRxSmuMessageArgument2, &RequestArgument[2]);
  GnbUraGet (DevObject, TRxSmuMessageArgument3, &RequestArgument[3]);
  GnbUraGet (DevObject, TRxSmuMessageArgument4, &RequestArgument[4]);
  GnbUraGet (DevObject, TRxSmuMessageArgument5, &RequestArgument[5]);
  WriteIo32 (0x8c, RequestArgument[0]);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuServiceRequestV8 Exit\n");

}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU service request for S3 script
 *
 *
 * @param[in]  StdHeader       Standard configuration header
 * @param[in]  ContextLength   Context length
 * @param[in]  Context         Pointer to Context
 */

VOID
GnbSmuServiceRequestV8S3Script (
  IN      AMD_CONFIG_PARAMS     *StdHeader,
  IN      UINT16                ContextLength,
  IN      VOID                  *Context
  )
{
  SMU_MSG_CONTEXT_V8    *SmuMsgContext;
  DEV_OBJECT            DevObject;
  UINT32                SmuArg[6];
  SmuMsgContext =  (SMU_MSG_CONTEXT_V8 *) Context;

  DevObject.StdHeader = StdHeader;
  DevObject.DevPciAddress = SmuMsgContext->GnbPciAddress;
  DevObject.GnbHandle = GnbGetHandle (StdHeader);

  SmuArg[0] = SmuMsgContext->RequestArgument[0];
  SmuArg[1] = SmuMsgContext->RequestArgument[1];
  SmuArg[2] = SmuMsgContext->RequestArgument[2];
  SmuArg[3] = SmuMsgContext->RequestArgument[3];
  SmuArg[4] = SmuMsgContext->RequestArgument[4];
  SmuArg[5] = SmuMsgContext->RequestArgument[5];

  GnbSmuServiceRequestV8 (&DevObject, SmuMsgContext->RequestId, SmuArg, 0);
}


/*----------------------------------------------------------------------------------------*/
/**
 * SMU firmware test
 *
 *
 * @param[in]  GnbHandle       Pointer to GNB_HANDLE
 * @param[in]  RequestId       Request ID
 * @param[in]  StdHeader       Standard configuration header
 */

AGESA_STATUS
GnbSmuFirmwareTestV8 (
  IN       GNB_HANDLE               *GnbHandle,
  IN       UINT32                   RequestId,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  DEV_OBJECT                  DevObject;
  UINT32                      SmuArg[6];

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuFirmwareTestV8 Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  SmuArg[0] = 0xAA55AA55;
  GnbSmuServiceRequestV8 (
    &DevObject,
    RequestId,
    SmuArg,
    0
    );

  IDS_HDT_CONSOLE (NB_MISC, "  Service Test Return %x\n", SmuArg[0]);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuFirmwareTestV8 Exit\n");

  if (SmuArg[0] == 0) {
    return AGESA_ERROR;
  }
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power Up/Down iGPU
 *
 *
 *
 * @param[in]  GnbHandle       Pointer to GNB_HANDLE
 * @param[in]  RequestId       Request ID
 * @param[in]  StdHeader       Standard configuration header
 */
AGESA_STATUS
GnbSmuRequestGpuPowerV8 (
  IN       GNB_HANDLE               *GnbHandle,
  IN       UINT32                   RequestId,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  DEV_OBJECT                DevObject;
  UINT32                    SmuArg[6];

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  GnbSmuServiceRequestV8 (
    &DevObject,
    RequestId,
    SmuArg,
    0
    );

  return AGESA_SUCCESS;
}


AGESA_STATUS
GnbSmuLoadScsDataV8 (
  IN       GNB_HANDLE           *GnbHandle,
  IN       UINT8                *ScsDataPtr,
  IN       UINT32               RequestId,
  IN       AMD_CONFIG_PARAMS    *StdHeader
  )
{
  DEV_OBJECT                DevObject;
  AGESA_STATUS              Status;
  GNB_BUILD_OPTIONS_COMMON  *GnbCommonOptions;
  UINT32                    SmuArg[6];

  IDS_PERF_TIMESTAMP (TP_BEGINGNBLOADSCSDATA, StdHeader);

  Status = AGESA_SUCCESS;
  GnbCommonOptions = (GNB_BUILD_OPTIONS_COMMON*) GnbFmGnbBuildOptions (StdHeader);

  if ((!GnbCommonOptions->CfgSmuMessageEnable) || (RequestId == INVALID_SMU_MSG)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "BIOS FORCE SKIP SMU SERVICE(Id = %x)!!\n", RequestId);
    return AGESA_SUCCESS;
  }

  LibAmdMemFill (SmuArg, 0x00, sizeof (SmuArg), StdHeader);

  if (GnbCommonOptions->CfgScsSupport == TRUE) {
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadScsDataV8 Enter\n");
    //Check input parameters
    ASSERT ((ScsDataPtr != NULL) && (StdHeader != NULL));
    if ((ScsDataPtr == NULL) || (StdHeader == NULL)) {
      return AGESA_ERROR;
    }

    //Verify the SCS block signature
    ASSERT (*(UINT32 *)ScsDataPtr == SMU_SCS_SIGNATURE);
    if (*(UINT32 *)ScsDataPtr != SMU_SCS_SIGNATURE) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Verify SCS Binary fail\n", ScsDataPtr);
      return AGESA_ERROR;
    }

    //Load SCS block
    IDS_HDT_CONSOLE (GNB_TRACE, "Load SCS @%08x\n", ScsDataPtr);
    DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
    DevObject.GnbHandle = GnbHandle;
    DevObject.StdHeader = StdHeader;
    SmuArg[0] = (UINT32) (UINTN) ScsDataPtr;
    GnbSmuServiceRequestV8 (&DevObject, RequestId, &SmuArg[0], 0);

    //Get SCS result and save to Heap
    Status = GnbSmuGetScsResultV8 (GnbHandle, StdHeader);
    IDS_HDT_CONSOLE (GNB_TRACE, "Get SCS Result %s\n", (Status == AGESA_SUCCESS) ? "Success" : "Fail");
  }

  IDS_PERF_TIMESTAMP (TP_ENDGNBLOADSCSDATA, StdHeader);
  return Status;
}

AGESA_STATUS
GnbSmuGetScsResultV8 (
  IN       GNB_HANDLE           *GnbHandle,
  IN       AMD_CONFIG_PARAMS    *StdHeader
  )
{
  UINT32                      ScsResultAddr;
  UINT32                      NumApmWeights;
  AGESA_STATUS                Status;
  DEV_OBJECT                  DevObject;
  GNB_REGISTER_SERVICE        *GnbRegisterAccessProtocol;
  UINT32 fooval;

  Status = AGESA_SUCCESS;
  DevObject.GnbHandle = GnbHandle;
  DevObject.StdHeader = StdHeader;
  DevObject.DevPciAddress = GnbGetHostPciAddress (GnbHandle);

  Status = GnbLibLocateService (GnbRegisterAccessService, GnbGetSocketId (GnbHandle), &GnbRegisterAccessProtocol, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status != AGESA_SUCCESS) {
    return Status;
  }

  //Get the command result (A pointer to SMURAM)
  ScsResultAddr = 0;
  GnbUraGet (&DevObject, TRxSmuMessageArgument0, &ScsResultAddr);
  IDS_HDT_CONSOLE (GNB_TRACE, "SMURAMAddr %08x ", ScsResultAddr);

  if (ScsResultAddr == 0) {
    return Status;
  }

  Status = GnbRegisterAccessProtocol->Read (GnbHandle, TYPE_D0F0xBC,
      (ScsResultAddr | 0x10000000) + offsetof (SMU_RAM_CPU_INFO, CountApmWeights),
      &NumApmWeights, 0, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "NumApmWeights %d\n", NumApmWeights);

  fooval = NumApmWeights | 0xab550000;
  WriteIo32 (0x8c, fooval);

  return Status;
}

