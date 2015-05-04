/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific PCIe configuration data
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
#include  "Ids.h"
#include  "amdlib.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbPcieFamServices.h"
#include  "PcieComplexDataCZ.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_PCIECOMPLEXDATACZ_FILECODE
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

AGESA_STATUS
PcieGetComplexDataLengthCZ (
  IN       UINT8                         SocketId,
     OUT   UINTN                         *Length,
  IN       AMD_CONFIG_PARAMS             *StdHeader
  );

AGESA_STATUS
PcieBuildComplexConfigurationCZ (
  IN       UINT8                         SocketId,
     OUT   VOID                          *Buffer,
  IN       AMD_CONFIG_PARAMS             *StdHeader
  );

UINT32
PcieGetNativePhyLaneBitmapCZ (
  IN       UINT32                        PhyLaneBitmap,
  IN       PCIe_ENGINE_CONFIG            *Engine
  );


/*----------------------------------------------------------------------------------------*/
//
// Complex configuration
//

CZ_COMPLEX_CONFIG ComplexDataCZ = {
  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof (CZ_COMPLEX_CONFIG, GfxWrapper) - offsetof (CZ_COMPLEX_CONFIG, Silicon)
    },
    0,    // SiliconId
    0xFF, // NodeId
    0xFF, // LinkId
    0,    // Address
    0,    // GmmBase
  },
  //Gfx Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof (CZ_COMPLEX_CONFIG, GfxWrapper) - offsetof (CZ_COMPLEX_CONFIG, Silicon),
      offsetof (CZ_COMPLEX_CONFIG, GppWrapper) - offsetof (CZ_COMPLEX_CONFIG, GfxWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR9) - offsetof (CZ_COMPLEX_CONFIG, GfxWrapper)
    },

    GFX_WRAP_ID,
    GFX_NUMBER_OF_PIFs,
    GFX_START_PHY_LANE,
    GFX_END_PHY_LANE,
    GFX_CORE_ID,
    GFX_CORE_ID,
    GFX_END_PHY_LANE - GFX_START_PHY_LANE + 1,
    {
      1,  //PowerOffUnusedLanesEnabled,
      1,  //PowerOffUnusedPllsEnabled
      1,  //ClkGating
      1,  //LclkGating
      1,  //TxclkGatingPllPowerDown
      1,  //PllOffInL1
      1   //AccessEncoding
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Gpp Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof (CZ_COMPLEX_CONFIG, GppWrapper) - offsetof (CZ_COMPLEX_CONFIG, Silicon),
      offsetof (CZ_COMPLEX_CONFIG, DdiWrapper) - offsetof (CZ_COMPLEX_CONFIG, GppWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR4) - offsetof (CZ_COMPLEX_CONFIG, GppWrapper)
    },
    GPP_WRAP_ID,
    GPP_NUMBER_OF_PIFs,
    GPP_START_PHY_LANE,
    GPP_END_PHY_LANE,
    GPP_CORE_ID,
    GPP_CORE_ID,
    GPP_END_PHY_LANE - GPP_START_PHY_LANE + 1,
    {
      1,  //PowerOffUnusedLanesEnabled,
      1,  //PowerOffUnusedPllsEnabled
      1,  //ClkGating
      1,  //LclkGating
      1,  //TxclkGatingPllPowerDown
      1,  //PllOffInL1
      1   //AccessEncoding
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Virtual DDI Wrapper
  {
    {
      DESCRIPTOR_DDI_WRAPPER | DESCRIPTOR_VIRTUAL | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (CZ_COMPLEX_CONFIG, DdiWrapper) - offsetof (CZ_COMPLEX_CONFIG, Silicon),
      0,
      offsetof (CZ_COMPLEX_CONFIG, Ddi0) - offsetof (CZ_COMPLEX_CONFIG, DdiWrapper)
    },
    DDI_WRAP_ID,
    0,
    DDI_START_PHY_LANE,
    DDI_END_PHY_LANE,
    0,
    0,
    DDI_END_PHY_LANE - DDI_START_PHY_LANE + 1,
    {
      1,  //PowerOffUnusedLanesEnabled,
      1,  //PowerOffUnusedPllsEnabled
      1,  //ClkGating
      1,  //LclkGating
      1,  //TxclkGatingPllPowerDown
      0,  //PllOffInL1
      1   //AccessEncoding
    },
    0 //MasterPll
  },
//------------------------------ GFX WRAPPER START-------------------------------------
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR9) - offsetof (CZ_COMPLEX_CONFIG, GfxWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR8) - offsetof (CZ_COMPLEX_CONFIG, PortPBR9),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID },
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        UNUSED_LANE_ID,
        UNUSED_LANE_ID,
        PBR9_NATIVE_PCI_DEV,
        PBR9_NATIVE_PCI_FUN,
        PBR9_CORE_ID,
        PBR9_PORT_ID,
        PBR9_PCI_ADDRESS,
        LinkStateResetExit,
        PBR9,
        PBR9_UNIT_ID,
        PBR9_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR8) - offsetof (CZ_COMPLEX_CONFIG, GfxWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR7) - offsetof (CZ_COMPLEX_CONFIG, PortPBR8),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID },
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        UNUSED_LANE_ID,
        UNUSED_LANE_ID,
        PBR8_NATIVE_PCI_DEV,
        PBR8_NATIVE_PCI_FUN,
        PBR8_CORE_ID,
        PBR8_PORT_ID,
        PBR8_PCI_ADDRESS,
        LinkStateResetExit,
        PBR8,
        PBR8_UNIT_ID,
        PBR8_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR7) - offsetof (CZ_COMPLEX_CONFIG, GfxWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR6) - offsetof (CZ_COMPLEX_CONFIG, PortPBR7),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID },
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        UNUSED_LANE_ID,
        UNUSED_LANE_ID,
        PBR7_NATIVE_PCI_DEV,
        PBR7_NATIVE_PCI_FUN,
        PBR7_CORE_ID,
        PBR7_PORT_ID,
        PBR7_PCI_ADDRESS,
        LinkStateResetExit,
        PBR7,
        PBR7_UNIT_ID,
        PBR7_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR6) - offsetof (CZ_COMPLEX_CONFIG, GfxWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR5) - offsetof (CZ_COMPLEX_CONFIG, PortPBR6),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID },
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        UNUSED_LANE_ID,
        UNUSED_LANE_ID,
        PBR6_NATIVE_PCI_DEV,
        PBR6_NATIVE_PCI_FUN,
        PBR6_CORE_ID,
        PBR6_PORT_ID,
        PBR6_PCI_ADDRESS,
        LinkStateResetExit,
        PBR6,
        PBR6_UNIT_ID,
        PBR6_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR5) - offsetof (CZ_COMPLEX_CONFIG, GfxWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR4) - offsetof (CZ_COMPLEX_CONFIG, PortPBR5),
      0
    },
    { PciePortEngine, GFX_START_PHY_LANE, GFX_END_PHY_LANE },
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        0,
        7,
        PBR5_NATIVE_PCI_DEV,
        PBR5_NATIVE_PCI_FUN,
        PBR5_CORE_ID,
        PBR5_PORT_ID,
        PBR5_PCI_ADDRESS,
        LinkStateResetExit,
        PBR5,
        PBR5_UNIT_ID,
        PBR5_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
//------------------------------ GFX WRAPPER END --------------------------------------
//------------------------------ GPP WRAPPER START-------------------------------------
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR4) - offsetof (CZ_COMPLEX_CONFIG, GppWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR3) - offsetof (CZ_COMPLEX_CONFIG, PortPBR4),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        UNUSED_LANE_ID,
        UNUSED_LANE_ID,
        PBR4_NATIVE_PCI_DEV,
        PBR4_NATIVE_PCI_FUN,
        PBR4_CORE_ID,
        PBR4_PORT_ID,
        PBR4_PCI_ADDRESS,
        LinkStateResetExit,
        PBR4,
        PBR4_UNIT_ID,
        PBR4_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR3) - offsetof (CZ_COMPLEX_CONFIG, GppWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR2) - offsetof (CZ_COMPLEX_CONFIG, PortPBR3),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        UNUSED_LANE_ID,
        UNUSED_LANE_ID,
        PBR3_NATIVE_PCI_DEV,
        PBR3_NATIVE_PCI_FUN,
        PBR3_CORE_ID,
        PBR3_PORT_ID,
        PBR3_PCI_ADDRESS,
        LinkStateResetExit,
        PBR3,
        PBR3_UNIT_ID,
        PBR3_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR2) - offsetof (CZ_COMPLEX_CONFIG, GppWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR1) - offsetof (CZ_COMPLEX_CONFIG, PortPBR2),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        UNUSED_LANE_ID,
        UNUSED_LANE_ID,
        PBR2_NATIVE_PCI_DEV,
        PBR2_NATIVE_PCI_FUN,
        PBR2_CORE_ID,
        PBR2_PORT_ID,
        PBR2_PCI_ADDRESS,
        LinkStateResetExit,
        PBR2,
        PBR2_UNIT_ID,
        PBR2_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR1) - offsetof (CZ_COMPLEX_CONFIG, GppWrapper),
      offsetof (CZ_COMPLEX_CONFIG, PortPBR0) - offsetof (CZ_COMPLEX_CONFIG, PortPBR1),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        4,
        7,
        PBR1_NATIVE_PCI_DEV,
        PBR1_NATIVE_PCI_FUN,
        PBR1_CORE_ID,
        PBR1_PORT_ID,
        PBR1_PCI_ADDRESS,
        LinkStateResetExit,
        PBR1,
        PBR1_UNIT_ID,
        PBR1_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof (CZ_COMPLEX_CONFIG, PortPBR0) - offsetof (CZ_COMPLEX_CONFIG, GppWrapper),
      offsetof (CZ_COMPLEX_CONFIG, Ddi0) - offsetof (CZ_COMPLEX_CONFIG, PortPBR0),
      0
    },
    { PciePortEngine, 0, 3 },
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {
      {
        {0},
        0,
        3,
        PBR0_NATIVE_PCI_DEV,
        PBR0_NATIVE_PCI_FUN,
        PBR0_CORE_ID,
        PBR0_PORT_ID,
        PBR0_PCI_ADDRESS,
        LinkStateResetExit,
        PBR0,
        PBR0_UNIT_ID,
        PBR0_NUM_UNIT_IDs,
        0,
        0,
        0
      },
    },
  },
//------------------------------ GPP WRAPPER END -------------------------------------
//------------------------------ DDI WRAPPER START----------------------------------
  //Ddi0
  {
    {
      DESCRIPTOR_DDI_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, Ddi0) - offsetof (CZ_COMPLEX_CONFIG, DdiWrapper),
      offsetof (CZ_COMPLEX_CONFIG, Ddi1) - offsetof (CZ_COMPLEX_CONFIG, Ddi0),
      0
    },
    {PcieDdiEngine, 16, 19},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {0}                                             //PCIe_DDI_CONFIG
  },
  //Ddi1
  {
    {
      DESCRIPTOR_DDI_ENGINE,
      offsetof (CZ_COMPLEX_CONFIG, Ddi1) - offsetof (CZ_COMPLEX_CONFIG, DdiWrapper),
      offsetof (CZ_COMPLEX_CONFIG, Ddi2) - offsetof (CZ_COMPLEX_CONFIG, Ddi1),
      0
    },
    {PcieDdiEngine, 20, 23},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {0}                                             //PCIe_DDI_CONFIG
  },
  //Ddi2
  {
    {
      DESCRIPTOR_DDI_ENGINE | DESCRIPTOR_VIRTUAL | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (CZ_COMPLEX_CONFIG, Ddi2) - offsetof (CZ_COMPLEX_CONFIG, DdiWrapper),
      0,
      0
    },
    {PcieDdiEngine, 24, 27},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {0}                                             //PCIe_DDI_CONFIG
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  }
};


//
// PCIe lane allocation  GPP
//
UINT8 ROMDATA GppPortLaneConfigurationTableCZ [] = {
  UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, 4,              7,              0,            3,
  UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, 6,              7,              4,              5,              0,            3,
  UNUSED_LANE_ID, UNUSED_LANE_ID, 7,              7,              6,              6,              4,              5,              0,            3,
  7,              7,              6,              6,              5,              5,              4,              4,              0,            3
};

//
// PCIe lane allocation  GFX
//
UINT8 ROMDATA GfxPortLaneConfigurationTableCZ [] = {
  UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, 0,            7,
  UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, 4,              7,              0,            3,
  UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, UNUSED_LANE_ID, 6,              7,              4,              5,              0,            3,
  UNUSED_LANE_ID, UNUSED_LANE_ID, 7,              7,              6,              6,              4,              5,              0,            3,
  7,              7,              6,              6,              5,              5,              4,              4,              0,            3
};

//
// DDI lane allocation  DDI
//
UINT8 ROMDATA DdiLaneConfigurationTableCZ [] = {
   0,            3,             4,            11, UNUSED_LANE_ID, UNUSED_LANE_ID,
   0,            3,             4,             7,              8,             11,
};

//
// PCIe lane allocation  desfriptors
//
PCIe_LANE_ALLOC_DESCRIPTOR ROMDATA PcieLaneAllocConfigurationCZ[] = {
  {
    0,
    GFX_WRAP_ID,
    PciePortEngine,
    NUMBER_OF_GFX_PORTS,
    sizeof (GfxPortLaneConfigurationTableCZ) / (NUMBER_OF_GFX_PORTS * 2),
    &GfxPortLaneConfigurationTableCZ[0]
  },
  {
    0,
    GPP_WRAP_ID,
    PciePortEngine,
    NUMBER_OF_GPP_PORTS,
    sizeof (GppPortLaneConfigurationTableCZ) / (NUMBER_OF_GPP_PORTS * 2),
    &GppPortLaneConfigurationTableCZ[0]
  },
  {
    DESCRIPTOR_TERMINATE_LIST,
    DDI_WRAP_ID,
    PcieDdiEngine,
    NUMBER_OF_DDI_DDIS,
    sizeof (DdiLaneConfigurationTableCZ) / (NUMBER_OF_DDI_DDIS * 2),
    &DdiLaneConfigurationTableCZ[0]
  }
};


/*----------------------------------------------------------------------------------------*/
/**
 * Get length of data block for complex
 *
 *
 *
 * @param[in]  SocketId              Socket ID.
 * @param[out] Length                Length of configuration info block
 * @param[out] StdHeader             Standard configuration header
 * @retval     AGESA_SUCCESS         Configuration data length is correct
 */
AGESA_STATUS
PcieGetComplexDataLengthCZ (
  IN       UINT8                         SocketId,
     OUT   UINTN                         *Length,
  IN       AMD_CONFIG_PARAMS             *StdHeader
  )
{
  *Length = sizeof (CZ_COMPLEX_CONFIG);
  return  AGESA_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Build configuration
 *
 *
 *
 * @param[in]  SocketId         Socket ID.
 * @param[out] Buffer           Pointer to buffer to build internal complex data structure
 * @param[out] StdHeader        Standard configuration header.
 * @retval     AGESA_SUCCESS    Configuration data build successfully
 */
AGESA_STATUS
PcieBuildComplexConfigurationCZ (
  IN       UINT8                         SocketId,
     OUT   VOID                          *Buffer,
  IN       AMD_CONFIG_PARAMS             *StdHeader
  )
{
  LibAmdMemCopy (Buffer, &ComplexDataCZ, sizeof (CZ_COMPLEX_CONFIG), StdHeader);
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  get native PHY lane bitmap
 *
 *
 * @param[in]  PhyLaneBitmap  Package PHY lane bitmap
 * @param[in]  Engine         Standard configuration header.
 * @retval     Native PHY lane bitmap
 */
UINT32
PcieGetNativePhyLaneBitmapCZ (
  IN       UINT32                        PhyLaneBitmap,
  IN       PCIe_ENGINE_CONFIG            *Engine
  )
{

  return PhyLaneBitmap;
}
