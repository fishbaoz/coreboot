/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific GFX library
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
#include  "amdlib.h"
#include  "Ids.h"
#include  "S3SaveState.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbGfx.h"
#include  "GfxLibV5.h"
#include  "GnbCommonLib.h"
#include  "GnbRegistersCommonV2.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBGFXINTTABLEV5_GFXLIBV5_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
CONST UINT16 GfxMemClockFrequencyDefinitionTableV5 [][8] = {
50,   100,  200,  300,  333,  366,  400,  433,
466,  500,  533,  566,  600,  633,  667,  700,
733,  766,  800,  833,  866,  900,  933,  966,
1000, 1050, 1066, 1100, 0,    1150, 0,    1200
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Extract DRAM frequency
 *
 *
 *
 * @param[in] Encoding   Memory Clock Frequency Value Definition
 * @param[in] StdHeader  Standard configuration header
 * @retval    Dram frequency Mhz
 */
UINT32
GfxLibExtractDramFrequencyV5 (
  IN       UINT8                  Encoding,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  if (Encoding >= (sizeof (GfxMemClockFrequencyDefinitionTableV5) / sizeof (UINT16))) {
    ASSERT (FALSE);
    return 0;
  }
  return GfxMemClockFrequencyDefinitionTableV5[Encoding / 8][Encoding % 8];
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable GMM Access for gBIF BAR Arrangement
 *
 *
 *
 * @param[in,out]   Gfx            Pointer to GFX configuration
 * @retval          AGESA_STATUS
 */

AGESA_STATUS
GfxEnableGmmAccessV5 (
  IN OUT   GFX_PLATFORM_CONFIG   *Gfx
  )
{
  UINT32      Value;
  GNB_HANDLE  *GnbHandle;

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);
  // GmmBase should be 0 before enable.
  ASSERT (GnbHandle->GmmBase == 0);

  if (!GnbLibPciIsDevicePresent (Gfx->GfxPciAddress.AddressValue, GnbLibGetHeader (Gfx))) {
    IDS_ERROR_TRAP;
    return AGESA_ERROR;
  }

  // Check if base address for GMM allocated by reading D1F0x24 Graphics Memory Mapped Base Address
  Gfx->GmmBase = 0;
  GnbLibPciRead (Gfx->GfxPciAddress.AddressValue | 0x24, AccessWidth32, &Value, GnbLibGetHeader (Gfx));
  Gfx->GmmBase |= (Value & 0xfffffff0);
  if (Gfx->GmmBase == 0) {
    IDS_ERROR_TRAP;
    return AGESA_ERROR;
  }

  // Check if base address for FB allocated
  GnbLibPciRead (Gfx->GfxPciAddress.AddressValue | 0x10, AccessWidth32, &Value, GnbLibGetHeader (Gfx));
  if ((Value & 0xfffffff0) == 0) {
    IDS_ERROR_TRAP;
    return AGESA_ERROR;
  }
  //Push CPU MMIO pci config to S3 script
  GnbLibS3SaveConfigSpace (MAKE_SBDFO (0, 0, 0x18, 1, 0), 0xBC, 0x80, AccessS3SaveWidth32, GnbLibGetHeader (Gfx));
  // Turn on memory decoding on GFX to enable access to GMM register space
  GnbLibPciRMW (Gfx->GfxPciAddress.AddressValue | 0x4, AccessWidth32, 0xffffffff, BIT1 | BIT2, GnbLibGetHeader (Gfx));
  //Push iGPU pci config to S3 script
  GnbLibS3SaveConfigSpace (Gfx->GfxPciAddress.AddressValue, 0x24, 0x10, AccessS3SaveWidth32, GnbLibGetHeader (Gfx));
  GnbLibS3SaveConfigSpace (Gfx->GfxPciAddress.AddressValue, 0x04, 0x04, AccessS3SaveWidth16, GnbLibGetHeader (Gfx));
  GnbHandle->GmmBase = Gfx->GmmBase;
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Determine number of audio ports for each connector
 *
 *
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
GfxIntAudioEpEnumCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8    *AudioCount;
  AudioCount = (UINT8*) Buffer;
  if (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeHDMI) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Found HDMI Connector\n");
    (*AudioCount)++;
  } else if (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeDP) {
    if ((Engine->Type.Ddi.DdiData.Flags & DDI_DATA_FLAGS_DP1_1_ONLY) == 0) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Found DP1.2 Connector\n");
      *AudioCount += 4;
    } else {
      IDS_HDT_CONSOLE (GNB_TRACE, "Found DP1.1 Connector\n");
      (*AudioCount)++;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "New AudioCount = %d\n", *AudioCount);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate audio endpoint in all display connectors.
 *
 *
 *
 * @param[in]       Gfx             Gfx configuration info
 * @param[in, out]  AudioEPCount    Total Audio endpoint number
 * @retval          AGESA_STATUS
 */
AGESA_STATUS
GfxIntAudioEPEnumV5 (
  IN       GFX_PLATFORM_CONFIG    *Gfx,
  IN OUT   UINT8                  *AudioEPCount
  )
{
  UINT8                   NumAudioEp;
  AGESA_STATUS            Status;
  PCIe_PLATFORM_CONFIG    *Pcie;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntAudioEPEnumV5 Enter\n");

  NumAudioEp = 0;
  Status = PcieLocateConfigurationData (GnbLibGetHeader (Gfx), &Pcie);
  if ((Status == AGESA_SUCCESS) && (Gfx->GnbHdAudio != 0)) {
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_DDI_ENGINE | DESCRIPTOR_VIRTUAL,
      GfxIntAudioEpEnumCallback,
      &NumAudioEp,
      Pcie
      );

    if (Gfx->GnbRemoteDisplaySupport) {
      NumAudioEp++;
    }
  }

  *AudioEPCount = NumAudioEp;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntAudioEPEnumV5 Exit\n");
  return Status;
}
