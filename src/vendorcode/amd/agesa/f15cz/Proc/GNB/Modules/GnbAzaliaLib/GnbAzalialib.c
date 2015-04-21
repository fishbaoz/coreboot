/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB mid post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-26 11:00:43 +0800 (Thu, 26 Feb 2015) $
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
#include  "AMD.h"
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
#include  "GnbAzalialib.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBAZALIALIB_GNBAZALIALIB_FILECODE
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
 * Audio controller reset
 *
 *
 * @param[in]  HdaBaseAddress      BAR0 Address Map
 * @param[in]  StdHeader           Standard configuration header
 *
 */
AGESA_STATUS
ControllerReset (
  IN      UINT32            HdaBaseAddress,
  IN      AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32        Value32;
  UINT32        Loops;

  GnbLibMemRead (HdaBaseAddress + HDAx0008_ADDRESS, AccessWidth32, &Value32, StdHeader);
  Value32 |= BIT0;
  GnbLibMemWrite (HdaBaseAddress + HDAx0008_ADDRESS, AccessWidth32, &Value32, StdHeader);

  for (Loops = 0; Loops < TIMEOUT_LOOP_TIMES; Loops++) {
    Value32 = 0;
    /* TODO: Remove this when we found the reason why it is needed. */
    printk(BIOS_DEBUG, "%s,%d\n", __func__, __LINE__);

    GnbLibMemRead (HdaBaseAddress + HDAx0008_ADDRESS, AccessWidth32, &Value32, StdHeader);
    if ((Value32 & BIT0) == BIT0) {
      return AGESA_SUCCESS;
    }
    GnbLibStall (TIMEOUT_STALL, StdHeader);
  }
  return AGESA_ERROR;
}

/*----------------------------------------------------------------------------------------*/
/**
 * wait for Immediate Status result valid
 *
 *
 * @param[in]  HdaBaseAddress      BAR0 Address Map
 * @param[in]  StdHeader           Standard configuration header
 *
 */
AGESA_STATUS
WaitImmediateStatus (
  IN      UINT32            HdaBaseAddress,
  IN      AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32        BitValue;
  UINT32        Loops;

  for (Loops = 0; Loops < TIMEOUT_LOOP_TIMES; Loops++) {
    BitValue = 0;
    GnbLibMemRead (HdaBaseAddress + HDAx0068_ADDRESS, AccessWidth32, &BitValue, StdHeader);
    if ((BitValue & BIT1) == BIT1) {
      return AGESA_SUCCESS;
    }
    GnbLibStall (TIMEOUT_STALL, StdHeader);
  }
  return AGESA_ERROR;
}

/*----------------------------------------------------------------------------------------*/
/**
 * send HD Audio Codec Command
 *
 *
 * @param[in]       HdaBaseAddress      BAR0 Address Map
 * @param[in]       StdHeader           Standard configuration header
 * @param[in, out]  CodecCommandData    Command data
 * @param[in]       ReadBack            Retrieve the response
 *
 */
AGESA_STATUS
SendCodecCommand (
  IN       UINT32            HdaBaseAddress,
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN OUT   UINT32            *CodecCommandData,
  IN       BOOLEAN           ReadBack
  )
{
  AGESA_STATUS  Status;
  UINT32 BitValue;
  UINT32 Loops;

  Status = AGESA_ERROR;
  if (CodecCommandData == NULL) {
    return Status;
  }

  //
  // Clear Immediate Command Status
  //
  GnbLibMemRead (HdaBaseAddress + HDAx0068_ADDRESS, AccessWidth32, &BitValue, StdHeader);
  BitValue |= BIT0;
  GnbLibMemWrite (HdaBaseAddress + HDAx0068_ADDRESS, AccessWidth32, &BitValue, StdHeader);

  for (Loops = 0; Loops < TIMEOUT_LOOP_TIMES; Loops++) {
    BitValue = 0;
    GnbLibMemRead (HdaBaseAddress + HDAx0068_ADDRESS, AccessWidth32, &BitValue, StdHeader);
    if ((BitValue & (BIT0 | BIT1)) == 0) {
      Status = AGESA_SUCCESS;
      break;
    }
    GnbLibStall (TIMEOUT_STALL, StdHeader);
  }

  if (Status == AGESA_SUCCESS) {
    GnbLibMemWrite (HdaBaseAddress + HDAx0060_ADDRESS, AccessWidth32, CodecCommandData, StdHeader);
    Status = WaitImmediateStatus (HdaBaseAddress, StdHeader);
    if (Status == AGESA_SUCCESS) {
      if (ReadBack == TRUE) {
        GnbLibMemRead (HdaBaseAddress + HDAx0064_ADDRESS, AccessWidth32, CodecCommandData, StdHeader);
      }
    }
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Config Codec verb table
 *
 *
 * @param[in]       HdaBaseAddress      BAR0 Address Map
 * @param[in]       VerbTable           Codec Verbtable address
 * @param[in]       StdHeader           Standard configuration header
 *
 */
AGESA_STATUS
ConfigureVerbTable (
  IN      UINT32            HdaBaseAddress,
  IN      UINT64            VerbTable,
  IN      AMD_CONFIG_PARAMS *StdHeader
  )
{
  AGESA_STATUS     Status;
  UINT16           ChannelNum;
  UINT32           CodecStateMap;
  UINT32           CodecMask;
  UINT32           VendorDeviceId;
  UINT8            ParameterId;
  UINT32           *VerbEntryList;
  CODEC_VERB_TABLE_LIST   *VerbTableList;
  CODEC_VERB_TABLE_LIST   *VerbTableAddress;

  VerbTableAddress = (CODEC_VERB_TABLE_LIST *)VerbTable;

  IDS_HDT_CONSOLE (GNB_TRACE, "ConfigureVerbTable: HdaBaseAddress = %x, CodecVerbTable = %x\n", HdaBaseAddress, VerbTableAddress);
  if (HdaBaseAddress == 0 || VerbTableAddress == NULL) {
    return AGESA_ERROR;
  }
  //
  // Perform CRST causes the controller to exit its Reset state and de-assert the link RESET# signal.
  //
  Status = ControllerReset (HdaBaseAddress, StdHeader);
  if (Status == AGESA_ERROR) {
    IDS_HDT_CONSOLE (GNB_TRACE, "ConfigureVerbTable: Active Controller fail! \n");
    return Status;
  }

  //
  // To find out all codec through identification of the state change status
  // "STATESTS" bits.
  //
  GnbLibStall (TIMEOUT_STALL, StdHeader);
  GnbLibMemRead (HdaBaseAddress + HDAx000C_ADDRESS, AccessWidth32, &CodecStateMap, StdHeader);
  CodecStateMap = (CodecStateMap & 0xf0000) >> 16;

  //
  // To confirm every SDIN if there is a codec on link.
  //
  for (ChannelNum = 0; ChannelNum < HDA_MAX_SDIN_NUM; ChannelNum++) {
    CodecMask = (1 << ChannelNum);
    if (CodecStateMap & CodecMask) {
      //
      // To read the Codec's vendor and device ID.
      //
      ParameterId = 0x00;
      VendorDeviceId = 0xF0000;
      VendorDeviceId |= (ChannelNum << 28);
      Status = SendCodecCommand (HdaBaseAddress, StdHeader, &VendorDeviceId, TRUE);
      if (Status == AGESA_SUCCESS) {
        for (VerbTableList = VerbTableAddress; VerbTableList->CodecId != 0xFFFFFFFF; VerbTableList++) {
          IDS_HDT_CONSOLE (GNB_TRACE, "ConfigureVerbTable: CodecId = %x, VendorDeviceId = %x\n", VerbTableList->CodecId, VendorDeviceId);
          if (VendorDeviceId == VerbTableList->CodecId) {
            //
            // Deliver Verb Entry
            //
            VerbEntryList = (UINT32*)VerbTableList->CodecTablePtr;
            while (*VerbEntryList != 0xFFFFFFFF) {
              Status = SendCodecCommand (HdaBaseAddress, StdHeader, VerbEntryList, FALSE);
              if (Status == AGESA_ERROR) {
                IDS_HDT_CONSOLE (GNB_TRACE, "ConfigureVerbTable: CodecId = %x, Deliver Verb Entry = %x fail! \n", VendorDeviceId, *VerbEntryList);
                break;
              }
              VerbEntryList++;
            }
            //
            // Finished, break to search the next SDIN to detect Codec.
            //
            break;
          }
        }
      }
    }
  }
  return Status;
}
