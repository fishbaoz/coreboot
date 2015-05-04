/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB Register Access Methods.
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
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbRegisterAccCZ.h"
#include  "GnbRegistersCZ.h"
#include  "GnbSmuInitLibV8.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_GNBREGISTERACCCZ_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define GNB_IGNORED_PARAM             0xFF
#define ORB_WRITE_ENABLE              0//0x100
#define IOMMU_L1_WRITE_ENABLE         0x80000000ul
#define IOMMU_L2_WRITE_ENABLE         0x100


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
GnbRegisterWriteCZDump (
  IN       UINT8               RegisterSpaceType,
  IN       UINT64              Address,
  IN       VOID                *Value
  );



/*----------------------------------------------------------------------------------------*/
/*
 *  Select Dct
 *
 *
 *
 * @param[in] DctCfgSel          Dct0/Dct1
 * @param[in] StdHeader          Standard configuration header
 */
VOID
STATIC
GnbDctConfigCZ (
  IN       UINT8               DctCfgSel,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  // Select DCT, D18F1x10C[DctCfgSel]
  D18F1x10C_STRUCT      D18F1x10C;


  GnbLibPciRead (
    MAKE_SBDFO (0, 0, 0x18, 1, D18F1x10C_ADDRESS),
    AccessWidth32,
    &D18F1x10C.Value,
    StdHeader
  );

  if ((DctCfgSel != 0xFF) && (DctCfgSel < 2)) {
    D18F1x10C.Field.DctCfgSel = DctCfgSel;
  }

  GnbLibPciWrite (
    MAKE_SBDFO (0, 0, 0x18, 1, D18F1x10C_ADDRESS),
    AccessWidth32,
    &D18F1x10C.Value,
    StdHeader
  );
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to read all register spaces.
 *
 *
 *
 * @param[in] RegisterSpaceType  Register space type
 * @param[in] Address            Register offset, but PortDevice
 * @param[out] Value             Return value
 * @param[in] Flags              Flags - BIT0 indicates S3 save/restore
 * @param[in] StdHeader          Standard configuration header
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
GnbRegisterReadCZ (
  IN       GNB_HANDLE          *GnbHandle,
  IN       UINT8               RegisterSpaceType,
  IN       UINT32              Address,
     OUT   VOID                *Value,
  IN       UINT32              Flags,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  ACCESS_WIDTH  Width;
  PCI_ADDR      GnbPciAddress;
  UINT64        GmmBase;

  GnbPciAddress = GnbGetHostPciAddress (GnbHandle);
  Width = (Flags == GNB_REG_ACC_FLAG_S3SAVE) ? AccessS3SaveWidth32 : AccessWidth32;
  GmmBase = GnbGetGmmBaseAddress (GnbHandle);


  switch (RegisterSpaceType) {
  case TYPE_D0F0:
    GnbLibPciRead (
      GnbPciAddress.AddressValue | Address,
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D0F2:
    GnbPciAddress.Address.Function = 2;
    GnbLibPciRead (
      GnbPciAddress.AddressValue | Address,
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_DxF0:
    GnbLibPciRead (
      Address,
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F0:
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 0, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F1:
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 1, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F2:
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 2, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F3:
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 3, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F4:
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 4, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F5:
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 5, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F2_dct0:
    GnbDctConfigCZ (0, StdHeader);
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 2, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F2_dct1:
    GnbDctConfigCZ (1, StdHeader);
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x18, 2, Address),
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0x64:
    // Miscellaneous Index Data, access the registers D0F0x64_x[FF:00]
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F0x60_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0x98:
    // Northbridge ORB Configuration Offset, access D0F0x98_x[FF:00]
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F0x94_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xFC:
    // GNB IOAPIC Offset
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F0xF8_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xBC:
    {
      UINT64  TempData;
      GnbLibPciIndirectRead (
        GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
        (Address & (~0x3ull)),
        Width,
        &TempData,
        StdHeader
      );
      if ((Address & 0x3) != 0) {
        //Non aligned access allowed to fuse block
        GnbLibPciIndirectRead (
          GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
          (Address & (~0x3ull)) + 4,
          Width,
          ((UINT32 *) &TempData) + 1,
          StdHeader
        );
      }
      * ((UINT32*) Value) = (UINT32) (TempData >> ((Address & 0x3) * 8));
      break;
    }
  case TYPE_D0F0xC4:
    // D0F0xC0 RAS Index Address, access D0F0xC4_x[FF:00]
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F0xC0_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xD4:
    // D0F0xD0 Link Index Address, access D0F0xD4_x[0130_14BF:0109_0000]
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F0xD0_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xE4:
    // D0F0xE0 Link Index Address, access D0F0xE4_x[FFFF_FFFF:0000_0000]
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F0xE0_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F2xF4:
    // IOMMU L2 Config Index, to access the registers D0F2xF4_x[FF:00].
    GnbPciAddress.Address.Function = 2;
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F2xF0_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F2xFC:
    // IOMMU L1 Config Index, access the registers D0F2xFC_x[FFFF:0000]_L1[3:0]
    GnbPciAddress.Address.Function = 2;
    GnbLibPciIndirectRead (
      GnbPciAddress.AddressValue | D0F2xF8_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D8F0xEC:
    // NBSPGCMNIND Registers
    GnbLibPciIndirectRead (
      MAKE_SBDFO (0, 0, 0x8, 0, D8F0xE8_ADDRESS),
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D9F2:
    GnbLibPciRead (
      MAKE_SBDFO (0, 0, 0x9, 2, Address),
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D9F2xEC:
    // NBACGAZCMNIND Registers
    GnbLibPciIndirectRead (
      MAKE_SBDFO (0, 0, 0x9, 0x2, D9F2xE8_ADDRESS),
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D9F2xF4:
    // NBACGAZIND Registers
    GnbLibPciIndirectRead (
      MAKE_SBDFO (0, 0, 0x9, 0x2, D9F2xF0_ADDRESS),
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_MSR:
    LibAmdMsrRead  (Address, Value, StdHeader);
    break;

  case TYPE_GMM:
    ASSERT (Address < 0x40000);
    ASSERT (GmmBase != 0);

    GnbLibMemRead (GmmBase + Address, Width, Value, StdHeader);
    break;

  default:
    ASSERT (FALSE);
    break;
  }
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to write all register spaces.
 *
 *
 * @param[in] GnbHandle          GnbHandle
 * @param[in] RegisterSpaceType  Register space type
 * @param[in] Address            Register offset, but PortDevice
 * @param[out] Value             The value to write
 * @param[in] Flags              Flags - BIT0 indicates S3 save/restore
 * @param[in] StdHeader          Standard configuration header
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
GnbRegisterWriteCZ (
  IN       GNB_HANDLE          *GnbHandle,
  IN       UINT8               RegisterSpaceType,
  IN       UINT32              Address,
  IN       VOID                *Value,
  IN       UINT32              Flags,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{

  ACCESS_WIDTH    Width;
  UINT64          TempAddress;
  PCI_ADDR        GnbPciAddress;
  DEV_OBJECT      DevObject;
  UINT64          GmmBase;

  GnbPciAddress = GnbGetHostPciAddress (GnbHandle);
  Width = (Flags == GNB_REG_ACC_FLAG_S3SAVE) ? AccessS3SaveWidth32 : AccessWidth32;
  TempAddress = Address;
  GmmBase = GnbGetGmmBaseAddress (GnbHandle);

  GNB_DEBUG_CODE (
    if (RegisterSpaceType == TYPE_GMM) {
      TempAddress = GmmBase + Address;
    }
    GnbRegisterWriteCZDump (RegisterSpaceType, TempAddress, Value);
  );

  switch (RegisterSpaceType) {
  case TYPE_D0F0:
    GnbLibPciWrite (
      GnbPciAddress.AddressValue | Address,
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D0F2:
    GnbPciAddress.Address.Function = 2;
    GnbLibPciWrite (
      GnbPciAddress.AddressValue | Address,
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_DxF0:
    GnbLibPciWrite (
      Address,
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F0:
    GnbLibPciWrite (
      MAKE_SBDFO (0, 0, 0x18, 0, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F1:
    GnbLibPciWrite (
      MAKE_SBDFO (0, 0, 0x18, 1, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F2:
    GnbLibPciWrite (
      MAKE_SBDFO (0, 0, 0x18, 2, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F3:
    GnbLibPciWrite (
      MAKE_SBDFO (0, 0, 0x18, 3, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F4:
    GnbLibPciWrite (
      MAKE_SBDFO (0, 0, 0x18, 4, Address),
      Width,
      Value,
      StdHeader
    );
    break;
  case TYPE_D18F5:
    GnbLibPciWrite (
      MAKE_SBDFO (0, 0, 0x18, 5, Address),
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0x64:
    // Miscellaneous Index Data, access the registers D0F0x64_x[FF:00]
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F0x60_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0x98:
    // Northbridge ORB Configuration Offset, access D0F0x98_x[FF:00]
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F0x94_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xFC:
    // GNB IOAPIC Offset
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F0xF8_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xBC:
    //SMU, access D0F0xBC_x[FFFFFFFF:00000000]
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F0xB8_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xC4:
    // D0F0xC0 RAS Index Address, access D0F0xC4_x[FF:00]
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F0xC0_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xD4:
    // D0F0xD0 Link Index Address, access D0F0xD4_x[0130_14BF:0109_0000]
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F0xD0_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F0xE4:
    // D0F0xE0 Link Index Address, access D0F0xE4_x[FFFF_FFFF:0000_0000]
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F0xE0_ADDRESS,
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F2xF4:
    // IOMMU L2 Config Index, to access the registers D0F2xF4_x[FF:00].
    GnbPciAddress.Address.Function = 2;
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F2xF0_ADDRESS,
      Address | IOMMU_L2_WRITE_ENABLE,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D0F2xFC:
    // IOMMU L1 Config Index, access the registers D0F2xFC_x[FFFF:0000]_L1[3:0]
    GnbPciAddress.Address.Function = 2;
    GnbLibPciIndirectWrite (
      GnbPciAddress.AddressValue | D0F2xF8_ADDRESS,
      Address | IOMMU_L1_WRITE_ENABLE,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D8F0xEC:
    // NBSPGCMNIND Registers
    GnbLibPciIndirectWrite (
      MAKE_SBDFO (0, 0, 0x8, 0, D8F0xE8_ADDRESS),
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D9F2:
    GnbLibPciWrite (
      MAKE_SBDFO (0, 0, 0x9, 2, Address),
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D9F2xEC:
    // NBACGAZCMNIND Registers
    GnbLibPciIndirectWrite (
      MAKE_SBDFO (0, 0, 0x9, 0x2, D9F2xE8_ADDRESS),
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_D9F2xF4:
    // NBACGAZIND Registers
    GnbLibPciIndirectWrite (
      MAKE_SBDFO (0, 0, 0x9, 0x2, D9F2xF0_ADDRESS),
      Address,
      Width,
      Value,
      StdHeader
    );
    break;

  case TYPE_MSR:
    LibAmdMsrWrite  (Address, Value, StdHeader);
    break;

  case TYPE_GMM:
    ASSERT (Address < 0x40000);
    ASSERT (GmmBase != 0);

    GnbLibMemWrite (GmmBase + Address, Width, Value, StdHeader);
    break;

  case TYPE_SMU_MSG:
    DevObject.StdHeader = StdHeader;
    DevObject.GnbHandle = GnbGetHandle (StdHeader);
    DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
    GnbSmuServiceRequestV8 (&DevObject, (UINT8) Address, Value, Flags);
    break;

  default:
    ASSERT (FALSE);
    break;
  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to dump all write register spaces.
 *
 *
 *
 * @param[in] RegisterSpaceType  Register space type
 * @param[in] Address            Register offset
 * @param[in] Value              The value to write
 */
VOID
GnbRegisterWriteCZDump (
  IN       UINT8               RegisterSpaceType,
  IN       UINT64              Address,
  IN       VOID                *Value
  )
{
  IDS_HDT_CONSOLE (NB_MISC, "  R WRITE Space %s  Address 0x%04x, Value 0x%04x\n",
    (RegisterSpaceType == TYPE_D0F0) ? "TYPE_D0F0" : (
    (RegisterSpaceType == TYPE_D0F0x64) ? "TYPE_D0F0x64" : (
    (RegisterSpaceType == TYPE_D0F0x98) ? "TYPE_D0F0x98" : (
    (RegisterSpaceType == TYPE_D0F0xBC) ? "TYPE_D0F0xBC" : (
    (RegisterSpaceType == TYPE_D0F0xC4) ? "TYPE_D0F0xC4" : (
    (RegisterSpaceType == TYPE_D0F0xD4) ? "TYPE_D0F0xD4" : (
    (RegisterSpaceType == TYPE_D0F0xE4) ? "TYPE_D0F0xE4" : (
    (RegisterSpaceType == TYPE_D0F0xFC) ? "TYPE_D0F0xFC" : (
    (RegisterSpaceType == TYPE_DxF0) ? "TYPE_DxF0" : (
    (RegisterSpaceType == TYPE_DxF0xE4) ? "TYPE_DxF0xE4" : (
    (RegisterSpaceType == TYPE_D0F2) ? "TYPE_D0F2" : (
    (RegisterSpaceType == TYPE_D0F2xF4) ? "TYPE_D0F2xF4" : (
    (RegisterSpaceType == TYPE_D0F2xFC) ? "TYPE_D0F2xFC" : (
    (RegisterSpaceType == TYPE_D18F0) ? "TYPE_D18F0" : (
    (RegisterSpaceType == TYPE_D18F1) ? "TYPE_D18F1" : (
    (RegisterSpaceType == TYPE_D18F2) ? "TYPE_D18F2" : (
    (RegisterSpaceType == TYPE_D18F3) ? "TYPE_D18F3" : (
    (RegisterSpaceType == TYPE_D18F4) ? "TYPE_D18F4" : (
    (RegisterSpaceType == TYPE_D18F5) ? "TYPE_D18F5" : (
    (RegisterSpaceType == TYPE_MSR) ? "TYPE_MSR" : (
    (RegisterSpaceType == TYPE_D1F0) ? "TYPE_D1F0" : (
    (RegisterSpaceType == TYPE_D1F1) ? "TYPE_D1F1" : (
    (RegisterSpaceType == TYPE_GMM) ? "TYPE_GMM" : (
    (RegisterSpaceType == TYPE_D18F2_dct0) ? "TYPE_D18F2_dct0" : (
    (RegisterSpaceType == TYPE_D18F2_dct1) ? "TYPE_D18F2_dct1" : (
    (RegisterSpaceType == TYPE_D8F0xEC) ? "TYPE_D8F0xEC" : (
    (RegisterSpaceType == TYPE_D9F2) ? "TYPE_D9F2" : (
    (RegisterSpaceType == TYPE_D9F2xEC) ? "TYPE_D9F2xEC" : (
    (RegisterSpaceType == TYPE_D9F2xF4) ? "TYPE_D9F2xF4" : (
    (RegisterSpaceType == TYPE_SMU_MSG) ? "TYPE_SMU_MSG" : "Invalid"))))))))))))))))))))))))))))),
    Address,
    *((UINT32*)Value)
  );
}
