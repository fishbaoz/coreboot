/* $NoKeywords:$ */
/**
 * @file
 *
 * CS Resume SMM Library for family 0x15 CZ CPU
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   CS Resume SMM Driver
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
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
 ******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "CsResumeSmmLib.h"
#include "CsSaveSmmLibCz.h"
#include "PspBaseLib.h"
#include "CpuFuncs.h"
#include "GnbRegistersCZ.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define  PMIO_INDEX                    (0xCD6)
#define  PMIO_DATA                     (0xCD7)
#define    ISA_CONTROL_ADDRESS         (0x04)
#define      MMIO_EN_BIT               (0x02)
#define  FCH_PMIO_MMIO_OFFSET          (0x300)

#define  FCH_AB_BAR_OFFSET             (0xE0)
#define  FCH_AB_IO_DATA_OFFSET         (0x4)
#define  FCH_ABCFG_SELECT              (0xC0000000ul)

#define  PCI_MCFG_BASE_MSR             (0xC0010058ul)
#define    PCI_MCFG_MASK               (0x000000FFFFF00000ull)

#define  C6_BASE_MSR                   (0xC0010073ul)
#define    C6_BASE_MASK                (0xFFFF)
#define    C6_BASE_SHIFT               (0)
#define    C6_PORT_OFFSET              (1)

#define  GNB_ROOT_PORT_PCI_DEVICE      (0x2)
#define  GNB_ROOT_PORT_PCI_OFFSET      (0xE0)

#define  GNB_MISC_PCI_FUNCTION         (0x0)
#define  GNB_MISC_INDEX_PCI_OFFSET     (0x60)
#define  GNB_MISC_DATA_PCI_OFFSET      (0x64)

#define  GNB_ORB_PCI_FUNCTION          (0x0)
#define  GNB_ORB_INDEX_PCI_OFFSET      (0x94)
#define  GNB_ORB_DATA_PCI_OFFSET       (0x98)

#define  GNB_SMU_PCI_FUNCTION          (0x0)
#define  GNB_SMU_INDEX_PCI_OFFSET      (0xB8)
#define  GNB_SMU_DATA_PCI_OFFSET       (0xBC)

#define  GNB_DEV_PCI_FUNCTION          (0x0)
#define  GNB_DEV_INDEX_PCI_OFFSET      (0xC8)
#define  GNB_DEV_DATA_PCI_OFFSET       (0xCC)

#define  GNB_GBIF_PCI_FUNCTION         (0x0)
#define  GNB_GBIF_INDEX_PCI_OFFSET     (0xD0)
#define  GNB_GBIF_DATA_PCI_OFFSET      (0xD4)

#define  GNB_LINK_PCI_FUNCTION         (0x0)
#define  GNB_LINK_INDEX_PCI_OFFSET     (0xE0)
#define  GNB_LINK_DATA_PCI_OFFSET      (0xE4)

#define  GNB_IOAPIC_PCI_FUNCTION       (0x0)
#define  GNB_IOAPIC_INDEX_PCI_OFFSET   (0xF8)
#define  GNB_IOAPIC_DATA_PCI_OFFSET    (0xFC)

#define  GNB_IOMMU_L2_PCI_FUNCTION     (0x2)
#define  GNB_IOMMU_L2_INDEX_PCI_OFFSET (0xF0)
#define  GNB_IOMMU_L2_DATA_PCI_OFFSET  (0xF4)
#define    GNB_IOMMU_L2_WRITE_EN       (0x100)

#define  GNB_IOMMU_L1_PCI_FUNCTION     (0x2)
#define  GNB_IOMMU_L1_INDEX_PCI_OFFSET (0xF8)
#define  GNB_IOMMU_L1_DATA_PCI_OFFSET  (0xFC)
#define    GNB_IOMMU_L1_WRITE_EN       (0x80000000ul)

#define  GNB_GMM_BAR_PCI_DEVICE        (0x1)
#define  GNB_GMM_BAR_PCI_FUNCTION      (0x0)
#define  GNB_GMM_BAR_PCI_OFFSET        (0x24)
#define    GNB_GMM_BAR_MASK            (0xFFFF0000ull)

#define  GNB_ACG_PCI_DEVICE            (0x9)
#define  GNB_ACG_PCI_FUNCTION          (0x2)
#define  GNB_ACG_AZ_CMN_INDEX_PCI_OFFSET  (0xE8)
#define  GNB_ACG_AZ_CMN_DATA_PCI_OFFSET   (0xEC)
#define  GNB_ACG_AZ_INDEX_PCI_OFFSET  (0xF0)
#define  GNB_ACG_AZ_DATA_PCI_OFFSET   (0xF4)

#define  GNB_SPG_PCI_DEVICE            (0x8)
#define  GNB_SPG_PCI_FUNCTION          (0x0)
#define  GNB_SPG_CMN_INDEX_PCI_OFFSET  (0xE8)
#define  GNB_SPG_CMN_DATA_PCI_OFFSET   (0xEC)
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/// Specific PCI bits restoration entry
typedef struct {
  BOOLEAN       PortPresent;          ///< Specifies whether port was present at end of POST
  UINT8         TrainingState;        ///< Current state of training sequence
  UINT8         Device;               ///< Device number of this port
  UINT8         Function;             ///< Function number of this port
} CS_PCIE_TRAINING_DATA;

/// PCIe Link Training State
typedef enum {
  LinkStateResetAssert,                                 ///< Assert port GPIO reset
  LinkStateResetDuration,                               ///< Timeout for reset duration
  LinkStateResetExit,                                   ///< Deassert port GPIO reset
  LinkTrainingResetTimeout,                             ///< Port GPIO reset timeout
  LinkStateReleaseTraining,                             ///< Release link training
  LinkStateDetectPresence,                              ///< Detect device presence
  LinkStateDetecting,                                   ///< Detect link training.
  LinkStateBrokenLane,                                  ///< Check and handle broken lane
  LinkStateGen2Fail,                                    ///< Check and handle device that fail training if GEN2 capability advertised
  LinkStateL0,                                          ///< Device trained to L0
  LinkStateVcoNegotiation,                              ///< Check VCO negotiation complete
  LinkStateRetrain,                                     ///< Force retrain link.
  LinkStateTrainingFail,                                ///< Link training fail
  LinkStateTrainingSuccess,                             ///< Link training success
  LinkStateGfxWorkaround,                               ///< GFX workaround
  LinkStateCompliance,                                  ///< Link in compliance mode
  LinkStateDeviceNotPresent,                            ///< Link is not connected
  LinkStateTrainingCompleted                            ///< Link training completed
} PCIE_LINK_TRAINING_STATE;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


UINT64    mPciMcfgBase = 0;
UINT8     *mGnbMiscIndex = NULL;
UINT32    *mGnbMiscData = NULL;
UINT8     *mGnbOrbIndex = NULL;
UINT32    *mGnbOrbData = NULL;
UINT32    *mGnbSmuIndex = NULL;
UINT32    *mGnbSmuData = NULL;
UINT32    *mGnbDevIndex = NULL;
UINT32    *mGnbDevData = NULL;
UINT32    *mGnbGbifIndex = NULL;
UINT32    *mGnbGbifData = NULL;
UINT32    *mGnbLinkIndex = NULL;
UINT32    *mGnbLinkData = NULL;
UINT8     *mGnbIoapicIndex = NULL;
UINT32    *mGnbIoapicData = NULL;
UINT16    *mGnbIommuL2Index = NULL;
UINT32    *mGnbIommuL2Data = NULL;
UINT32    *mGnbIommuL1Index = NULL;
UINT32    *mGnbIommuL1Data = NULL;
UINT64    mGnbGmmBase = 0;
UINT8     *mGnbAcgAzIndex = NULL;
UINT32    *mGnbAcgAzData = NULL;
UINT8     *mGnbAcgAzCmnIndex = NULL;
UINT32    *mGnbAcgAzCmnData = NULL;
UINT8     *mGnbSpgCmnIndex = NULL;
UINT32    *mGnbSpgCmnData = NULL;
UINT16    mFchAbIoAddress = 0xFFFF;


CS_RESTORATION_ENTRY_HEADER*
F15CzRestorePciEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_PCI    *PciEntry;
  UINT64    PciAddr;
  UINT32    *Register;

  PciEntry = (CS_PCI *) CurrentEntry;
  PciAddr = mPciMcfgBase + PciEntry->Address;
  Register = (UINT32 *) (UINTN) PciAddr;
  *Register = PciEntry->Value;
  PciEntry++;
  return &PciEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestorePciBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_PCI_BITS  *PciBitsEntry;
  UINT64    PciAddr;
  UINT32    *Register;
  UINT32    Scratch;

  PciBitsEntry = (CS_PCI_BITS *) CurrentEntry;
  PciAddr = mPciMcfgBase + PciBitsEntry->Address;
  Register = (UINT32 *) (UINTN) PciAddr;
  Scratch = (*Register & PciBitsEntry->Mask) | PciBitsEntry->Value;
  *Register = Scratch;
  PciBitsEntry++;
  return &PciBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreMmioEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_MMIO  *MmioEntry;
  UINT32   *Register;

  MmioEntry = (CS_MMIO *) CurrentEntry;
  Register = (UINT32 *) (UINTN) MmioEntry->Address;
  *Register = MmioEntry->Value;
  MmioEntry++;
  return &MmioEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreMmioBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_MMIO_BITS  *MmioBitsEntry;
  UINT32   *Register;
  UINT32   Scratch;

  MmioBitsEntry = (CS_MMIO_BITS *) CurrentEntry;
  Register = (UINT32 *) (UINTN) MmioBitsEntry->Address;
  Scratch = (*Register & MmioBitsEntry->Mask) | MmioBitsEntry->Value;
  *Register = Scratch;
  MmioBitsEntry++;
  return &MmioBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbMiscIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_MISC  *GnbMiscEntry;

  GnbMiscEntry = (CS_GNB_MISC *) CurrentEntry;
  *mGnbMiscIndex = GnbMiscEntry->Address;
  *mGnbMiscData = GnbMiscEntry->Value;
  GnbMiscEntry++;
  return &GnbMiscEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbMiscIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_MISC_BITS  *GnbMiscBitsEntry;

  GnbMiscBitsEntry = (CS_GNB_MISC_BITS *) CurrentEntry;
  *mGnbMiscIndex = GnbMiscBitsEntry->Address;
  *mGnbMiscData = ((*mGnbMiscData & GnbMiscBitsEntry->Mask) | GnbMiscBitsEntry->Value);
  GnbMiscBitsEntry++;
  return &GnbMiscBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbOrbCfgEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ORB_CFG  *GnbOrbCfgEntry;

  GnbOrbCfgEntry = (CS_GNB_ORB_CFG *) CurrentEntry;
  *mGnbOrbIndex = GnbOrbCfgEntry->Address;
  *mGnbOrbData = GnbOrbCfgEntry->Value;
  GnbOrbCfgEntry++;
  return &GnbOrbCfgEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbOrbCfgBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ORB_CFG_BITS  *GnbOrbCfgBitsEntry;

  GnbOrbCfgBitsEntry = (CS_GNB_ORB_CFG_BITS *) CurrentEntry;
  *mGnbOrbIndex = GnbOrbCfgBitsEntry->Address;
  *mGnbOrbData = (*mGnbOrbData & GnbOrbCfgBitsEntry->Mask) | GnbOrbCfgBitsEntry->Value;
  GnbOrbCfgBitsEntry++;
  return &GnbOrbCfgBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbSmuIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_SMU  *GnbSmuEntry;

  GnbSmuEntry = (CS_GNB_SMU *) CurrentEntry;
  *mGnbSmuIndex = GnbSmuEntry->Address;
  *mGnbSmuData = GnbSmuEntry->Value;
  GnbSmuEntry++;
  return &GnbSmuEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbSmuIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_SMU_BITS  *GnbSmuBitsEntry;

  GnbSmuBitsEntry = (CS_GNB_SMU_BITS *) CurrentEntry;
  *mGnbSmuIndex = GnbSmuBitsEntry->Address;
  *mGnbSmuData = (*mGnbSmuData & GnbSmuBitsEntry->Mask) | GnbSmuBitsEntry->Value;
  GnbSmuBitsEntry++;
  return &GnbSmuBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbDevIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_DEV  *GnbDevEntry;

  GnbDevEntry = (CS_GNB_DEV *) CurrentEntry;
  *mGnbDevIndex = (UINT32) (((UINT32) GnbDevEntry->Device << 16) | (UINT32) GnbDevEntry->Address);
  *mGnbDevData = GnbDevEntry->Value;
  GnbDevEntry++;
  return &GnbDevEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbDevIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_DEV_BITS  *GnbDevBitsEntry;

  GnbDevBitsEntry = (CS_GNB_DEV_BITS *) CurrentEntry;
  *mGnbDevIndex = (UINT32) (((UINT32) GnbDevBitsEntry->Device << 16) | (UINT32) GnbDevBitsEntry->Address);
  *mGnbDevData = ((*mGnbDevData & GnbDevBitsEntry->Mask) | GnbDevBitsEntry->Value);
  GnbDevBitsEntry++;
  return &GnbDevBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbGbifIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_GBIF  *GnbGbifEntry;

  GnbGbifEntry = (CS_GNB_GBIF *) CurrentEntry;
  *mGnbGbifIndex = GnbGbifEntry->Address;
  *mGnbGbifData = GnbGbifEntry->Value;
  GnbGbifEntry++;
  return &GnbGbifEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbGbifIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_GBIF_BITS  *GnbGbifBitsEntry;

  GnbGbifBitsEntry = (CS_GNB_GBIF_BITS *) CurrentEntry;
  *mGnbGbifIndex = GnbGbifBitsEntry->Address;
  *mGnbGbifData = (*mGnbGbifData & GnbGbifBitsEntry->Mask) | GnbGbifBitsEntry->Value;
  GnbGbifBitsEntry++;
  return &GnbGbifBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbLinkIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_LINK  *GnbLinkEntry;

  GnbLinkEntry = (CS_GNB_LINK *) CurrentEntry;
  *mGnbLinkIndex = (UINT32) ((((UINT32) GnbLinkEntry->Block << 24) | (UINT32) GnbLinkEntry->Frame << 16) | (UINT32) GnbLinkEntry->Address);
  *mGnbLinkData = GnbLinkEntry->Value;
  GnbLinkEntry++;
  return &GnbLinkEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbLinkIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_LINK_BITS  *GnbLinkBitsEntry;

  GnbLinkBitsEntry = (CS_GNB_LINK_BITS *) CurrentEntry;
  *mGnbLinkIndex = (UINT32) ((((UINT32) GnbLinkBitsEntry->Block << 24) | (UINT32) GnbLinkBitsEntry->Frame << 16) | (UINT32) GnbLinkBitsEntry->Address);
  *mGnbLinkData = ((*mGnbLinkData & GnbLinkBitsEntry->Mask) | GnbLinkBitsEntry->Value);
  GnbLinkBitsEntry++;
  return &GnbLinkBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbIoapicIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOAPIC  *GnbIoapicEntry;

  GnbIoapicEntry = (CS_GNB_IOAPIC *) CurrentEntry;
  *mGnbIoapicIndex = GnbIoapicEntry->Address;
  *mGnbIoapicData = GnbIoapicEntry->Value;
  GnbIoapicEntry++;
  return &GnbIoapicEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbIoapicIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOAPIC_BITS  *GnbIoapicBitsEntry;

  GnbIoapicBitsEntry = (CS_GNB_IOAPIC_BITS *) CurrentEntry;
  *mGnbIoapicIndex = GnbIoapicBitsEntry->Address;
  *mGnbIoapicData = (*mGnbIoapicData & GnbIoapicBitsEntry->Mask) | GnbIoapicBitsEntry->Value;
  GnbIoapicBitsEntry++;
  return &GnbIoapicBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreIommuL2IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L2  *GnbIommuL2Entry;

  GnbIommuL2Entry = (CS_GNB_IOMMU_L2 *) CurrentEntry;
  *mGnbIommuL2Index = ((UINT16) GnbIommuL2Entry->Address) | GNB_IOMMU_L2_WRITE_EN;
  *mGnbIommuL2Data = GnbIommuL2Entry->Value;
  GnbIommuL2Entry++;
  return &GnbIommuL2Entry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreIommuL2IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L2_BITS  *GnbIommuL2BitsEntry;

  GnbIommuL2BitsEntry = (CS_GNB_IOMMU_L2_BITS *) CurrentEntry;
  *mGnbIommuL2Index = ((UINT16) GnbIommuL2BitsEntry->Address) | GNB_IOMMU_L2_WRITE_EN;
  *mGnbIommuL2Data = (*mGnbIommuL2Data & GnbIommuL2BitsEntry->Mask) | GnbIommuL2BitsEntry->Value;
  GnbIommuL2BitsEntry++;
  return &GnbIommuL2BitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreIommuL1IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L1  *GnbIommuL1Entry;

  GnbIommuL1Entry = (CS_GNB_IOMMU_L1 *) CurrentEntry;
  *mGnbIommuL1Index = ((((UINT32) GnbIommuL1Entry->Select << 16) | (UINT32) GnbIommuL1Entry->Address) | GNB_IOMMU_L1_WRITE_EN);
  *mGnbIommuL1Data = GnbIommuL1Entry->Value;
  GnbIommuL1Entry++;
  return &GnbIommuL1Entry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreIommuL1IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_IOMMU_L1_BITS  *GnbIommuL1BitsEntry;

  GnbIommuL1BitsEntry = (CS_GNB_IOMMU_L1_BITS *) CurrentEntry;
  *mGnbIommuL1Index = ((((UINT32) GnbIommuL1BitsEntry->Select << 16) | (UINT32) GnbIommuL1BitsEntry->Address) | GNB_IOMMU_L1_WRITE_EN);
  *mGnbIommuL1Data = (*mGnbIommuL1Data & GnbIommuL1BitsEntry->Mask) | GnbIommuL1BitsEntry->Value;
  GnbIommuL1BitsEntry++;
  return &GnbIommuL1BitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbRootPortIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            PciAddr;
  UINT32            *GnbRootPortAddr;
  CS_GNB_ROOT_PORT  *GnbRootPortEntry;

  GnbRootPortEntry = (CS_GNB_ROOT_PORT *) CurrentEntry;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, 0, GNB_ROOT_PORT_PCI_OFFSET);
  PciAddr += ((UINT64) GnbRootPortEntry->Function << 12);
  PciAddr += ((UINT64) GnbRootPortEntry->Device << 15);
  GnbRootPortAddr = (UINT32 *) (UINTN) PciAddr;
  *GnbRootPortAddr++ = (UINT32) GnbRootPortEntry->Address;
  *GnbRootPortAddr = GnbRootPortEntry->Value;
  GnbRootPortEntry++;
  return &GnbRootPortEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbRootPortIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 PciAddr;
  UINT32                 *GnbRootPortAddr;
  CS_GNB_ROOT_PORT_BITS  *GnbRootPortBitsEntry;

  GnbRootPortBitsEntry = (CS_GNB_ROOT_PORT_BITS *) CurrentEntry;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, 0, GNB_ROOT_PORT_PCI_OFFSET);
  PciAddr += ((UINT64) GnbRootPortBitsEntry->Function << 12);
  PciAddr += ((UINT64) GnbRootPortBitsEntry->Device << 15);
  GnbRootPortAddr = (UINT32 *) (UINTN) PciAddr;
  *GnbRootPortAddr++ = (UINT32) GnbRootPortBitsEntry->Address;
  *GnbRootPortAddr = (*GnbRootPortAddr & GnbRootPortBitsEntry->Mask) | GnbRootPortBitsEntry->Value;
  GnbRootPortBitsEntry++;
  return &GnbRootPortBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbGmmEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32      *GmmRegister;
  UINT64      MmioAddr;
  CS_GNB_GMM  *GnbGmmEntry;

  GnbGmmEntry = (CS_GNB_GMM *) CurrentEntry;
  MmioAddr = mGnbGmmBase + GnbGmmEntry->Address;
  GmmRegister = (UINT32 *) (UINTN) MmioAddr;
  *GmmRegister = GnbGmmEntry->Value;
  GnbGmmEntry++;
  return &GnbGmmEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGnbGmmBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32           *GmmRegister;
  UINT64           MmioAddr;
  CS_GNB_GMM_BITS  *GnbGmmBitsEntry;

  GnbGmmBitsEntry = (CS_GNB_GMM_BITS *) CurrentEntry;
  MmioAddr = mGnbGmmBase + GnbGmmBitsEntry->Address;
  GmmRegister = (UINT32 *) (UINTN) MmioAddr;
  *GmmRegister = (*GmmRegister & GnbGmmBitsEntry->Mask) | GnbGmmBitsEntry->Value;
  GnbGmmBitsEntry++;
  return &GnbGmmBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreAcgAzIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ACG_AZ  *GnbAcgAzEntry;

  GnbAcgAzEntry = (CS_GNB_ACG_AZ *) CurrentEntry;
  *mGnbAcgAzIndex = GnbAcgAzEntry->Address;
  *mGnbAcgAzData = GnbAcgAzEntry->Value;
  GnbAcgAzEntry++;
  return &GnbAcgAzEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreAcgAzIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ACG_AZ_BITS  *GnbAcgAzBitsEntry;

  GnbAcgAzBitsEntry = (CS_GNB_ACG_AZ_BITS *) CurrentEntry;
  *mGnbAcgAzIndex = GnbAcgAzBitsEntry->Address;
  *mGnbAcgAzData = (*mGnbAcgAzData & GnbAcgAzBitsEntry->Mask) | GnbAcgAzBitsEntry->Value;
  GnbAcgAzBitsEntry++;
  return &GnbAcgAzBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreAcgAzCmnIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ACG_AZ_CMN  *GnbAcgAzCmnEntry;

  GnbAcgAzCmnEntry = (CS_GNB_ACG_AZ_CMN *) CurrentEntry;
  *mGnbAcgAzCmnIndex = GnbAcgAzCmnEntry->Address;
  *mGnbAcgAzCmnData = GnbAcgAzCmnEntry->Value;
  GnbAcgAzCmnEntry++;
  return &GnbAcgAzCmnEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreAcgAzCmnIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_ACG_AZ_CMN_BITS  *GnbAcgAzCmnBitsEntry;

  GnbAcgAzCmnBitsEntry = (CS_GNB_ACG_AZ_CMN_BITS *) CurrentEntry;
  *mGnbAcgAzCmnIndex = GnbAcgAzCmnBitsEntry->Address;
  *mGnbAcgAzCmnData = (*mGnbAcgAzCmnData & GnbAcgAzCmnBitsEntry->Mask) | GnbAcgAzCmnBitsEntry->Value;
  GnbAcgAzCmnBitsEntry++;
  return &GnbAcgAzCmnBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreSpgCmnIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GNB_SPG_CMN_BITS  *GnbSpgCmnBitsEntry;

  GnbSpgCmnBitsEntry = (CS_GNB_SPG_CMN_BITS *) CurrentEntry;
  *mGnbSpgCmnIndex = GnbSpgCmnBitsEntry->Address;
  *mGnbSpgCmnData = (*mGnbSpgCmnData & GnbSpgCmnBitsEntry->Mask) | GnbSpgCmnBitsEntry->Value;
  GnbSpgCmnBitsEntry++;
  return &GnbSpgCmnBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchAbEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_FCH_AB  *FchAbEntry;
  UINT32     Address;

  FchAbEntry = (CS_FCH_AB *) CurrentEntry;
  Address = FchAbEntry->Address | FCH_ABCFG_SELECT;
  LibAmdIoWrite (AccessWidth32, mFchAbIoAddress, &Address, NULL);
  LibAmdIoWrite (AccessWidth32, (mFchAbIoAddress + FCH_AB_IO_DATA_OFFSET), &(FchAbEntry->Value), NULL);
  FchAbEntry++;
  return &FchAbEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchAbBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32          CurrentValue;
  UINT32          Address;
  CS_FCH_AB_BITS  *FchAbBitsEntry;

  FchAbBitsEntry = (CS_FCH_AB_BITS *) CurrentEntry;
  Address = FchAbBitsEntry->Address | FCH_ABCFG_SELECT;
  LibAmdIoWrite (AccessWidth32, mFchAbIoAddress, &Address, NULL);
  LibAmdIoRead (AccessWidth32, (mFchAbIoAddress + FCH_AB_IO_DATA_OFFSET), &CurrentValue, NULL);
  CurrentValue = (CurrentValue & FchAbBitsEntry->Mask) | FchAbBitsEntry->Value;
  LibAmdIoWrite (AccessWidth32, (mFchAbIoAddress + FCH_AB_IO_DATA_OFFSET), &CurrentValue, NULL);
  FchAbBitsEntry++;
  return &FchAbBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchMmioDwordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64             MmioAddr;
  UINT32             *FchMmioAddr;
  CS_FCH_MMIO_DWORD  *FchMmioDwordEntry;

  FchMmioDwordEntry = (CS_FCH_MMIO_DWORD *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FchMmioDwordEntry->Address;
  FchMmioAddr = (UINT32 *) (UINTN) MmioAddr;
  *FchMmioAddr = FchMmioDwordEntry->Value;
  FchMmioDwordEntry++;
  return &FchMmioDwordEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchMmioDwordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64             MmioAddr;
  UINT32             *FchMmioAddr;
  CS_FCH_MMIO_DWORD_BITS  *FchMmioDwordBitsEntry;

  FchMmioDwordBitsEntry = (CS_FCH_MMIO_DWORD_BITS *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FchMmioDwordBitsEntry->Address;
  FchMmioAddr = (UINT32 *) (UINTN) MmioAddr;
  *FchMmioAddr = (*FchMmioAddr & FchMmioDwordBitsEntry->Mask) | FchMmioDwordBitsEntry->Value;
  FchMmioDwordBitsEntry++;
  return &FchMmioDwordBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchMmioWordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            MmioAddr;
  UINT16            *FchMmioAddr;
  CS_FCH_MMIO_WORD  *FchMmioWordEntry;

  FchMmioWordEntry = (CS_FCH_MMIO_WORD *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FchMmioWordEntry->Address;
  FchMmioAddr = (UINT16 *) (UINTN) MmioAddr;
  *FchMmioAddr = FchMmioWordEntry->Value;
  FchMmioWordEntry++;
  return &FchMmioWordEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchMmioWordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 MmioAddr;
  UINT16                 *FchMmioAddr;
  CS_FCH_MMIO_WORD_BITS  *FchMmioWordBitsEntry;

  FchMmioWordBitsEntry = (CS_FCH_MMIO_WORD_BITS *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FchMmioWordBitsEntry->Address;
  FchMmioAddr = (UINT16 *) (UINTN) MmioAddr;
  *FchMmioAddr = (*FchMmioAddr & FchMmioWordBitsEntry->Mask) | FchMmioWordBitsEntry->Value;
  FchMmioWordBitsEntry++;
  return &FchMmioWordBitsEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchMmioByteEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            MmioAddr;
  UINT8             *FchMmioAddr;
  CS_FCH_MMIO_BYTE  *FchMmioByteEntry;

  FchMmioByteEntry = (CS_FCH_MMIO_BYTE *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FchMmioByteEntry->Address;
  FchMmioAddr = (UINT8 *) (UINTN) MmioAddr;
  *FchMmioAddr = FchMmioByteEntry->Value;
  FchMmioByteEntry++;
  return &FchMmioByteEntry->Header;
}

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreFchMmioByteBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 MmioAddr;
  UINT8                  *FchMmioAddr;
  CS_FCH_MMIO_BYTE_BITS  *FchMmioByteBitsEntry;

  FchMmioByteBitsEntry = (CS_FCH_MMIO_BYTE_BITS *) CurrentEntry;
  MmioAddr = FCH_MMIO_BASE + FchMmioByteBitsEntry->Address;
  FchMmioAddr = (UINT8 *) (UINTN) MmioAddr;
  *FchMmioAddr = (*FchMmioAddr & FchMmioByteBitsEntry->Mask) | FchMmioByteBitsEntry->Value;
  FchMmioByteBitsEntry++;
  return &FchMmioByteBitsEntry->Header;
}

VOID
F15CzDispatchEnableFchAbEntries (
  IN       UINT32  Context
  )
{
  UINT16    *FchAbRegBar;
  UINT64    MmioAddr;

  MmioAddr = FCH_MMIO_BASE + FCH_PMIO_MMIO_OFFSET + FCH_AB_BAR_OFFSET;
  FchAbRegBar = (UINT16 *) (UINTN) MmioAddr;
  mFchAbIoAddress = *FchAbRegBar;
}

VOID
F15CzDispatchEnableGnbGmmEntries (
  IN       UINT32  Context
  )
{
  UINT32    *GnbGmmBar;
  UINT64    PciAddr;

  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_GMM_BAR_PCI_DEVICE, GNB_GMM_BAR_PCI_FUNCTION, GNB_GMM_BAR_PCI_OFFSET);
  GnbGmmBar = (UINT32 *) (UINTN) PciAddr;
  mGnbGmmBase = (UINT64) (*GnbGmmBar) & GNB_GMM_BAR_MASK;
}

VOID
F15CzDispatchWaitForReconfig (
  IN       UINT32  Context
  )
{
  UINT32    DataValue;
  UINT64    PciIndexAddr;
  UINT64    PciDataAddr;

  ///@todo This version uses direct addressing until SMU service is functional and supported
  // Note that this could be the "permanent" solution since SB link is not present on PCIe core
  PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
  PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);

  *((UINT32 *) (UINTN) PciIndexAddr) = 0x01400101 | (Context << 16);
  DataValue = *((UINT32 *) (UINTN) PciDataAddr);
  DataValue &= ~(7 << 2);
  DataValue |= ((2 << 2) | 1);
  *((UINT32 *) (UINTN) PciDataAddr) = DataValue;

  *((UINT32 *) (UINTN) PciIndexAddr) = 0x01400100 | (Context << 16);
  *((UINT32 *) (UINTN) PciDataAddr) |= 0x1;

  do {
    *((UINT32 *) (UINTN) PciIndexAddr) = 0x01400100 | (Context << 16);
    DataValue = *((UINT32 *) (UINTN) PciDataAddr);
  } while ((DataValue & 0x0001) == 0x1);

  *((UINT32 *) (UINTN) PciIndexAddr) = 0x01400101 | (Context << 16);
  DataValue = *((UINT32 *) (UINTN) PciDataAddr);
  //DataValue |= 1 << 11;
  DataValue &= 0xFFFFFFFE;
  *((UINT32 *) (UINTN) PciDataAddr) = DataValue;

///@todo The following code uses SMU to execute PCIe reconfig.
// This code may be enabled after the SMU services are functional
//  PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_SMU_PCI_FUNCTION, GNB_SMU_INDEX_PCI_OFFSET);
//  PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_SMU_PCI_FUNCTION, GNB_SMU_DATA_PCI_OFFSET);
//
//  // 1 Clear Response
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000010;
//  *((UINT32 *) (UINTN) PciDataAddr) = 0;
//
//  // 2 Write message arguments
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000020;
//  *((UINT32 *) (UINTN) PciDataAddr) = Context;
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000024;
//  *((UINT32 *) (UINTN) PciDataAddr) = 0;
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000028;
//  *((UINT32 *) (UINTN) PciDataAddr) = 0;
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x1300002C;
//  *((UINT32 *) (UINTN) PciDataAddr) = 0;
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000030;
//  *((UINT32 *) (UINTN) PciDataAddr) = 0;
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000034;
//  *((UINT32 *) (UINTN) PciDataAddr) = 0;
//
//  // 3 Write message ID
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000000;
//  *((UINT32 *) (UINTN) PciDataAddr) = 0x19;
//
//  // 4 Poll Response until non-zero
//  *((UINT32 *) (UINTN) PciIndexAddr) = 0x13000010;
//  do {
//    DataValue = *((UINT32 *) (UINTN) PciDataAddr);
//  } while (DataValue == 0x0);
//
//  // 5 Read updated SMU message arguments, no need
}

VOID
F15CzDispatchPcieTraining (
  IN       UINT32  Context
  )
{
  UINT64                  PciIndexAddr;
  UINT64                  PciDataAddr;
  UINT8                   PortId;
  UINT8                   AllPortsDone;
  UINT8                   Index;
  UINT8                   TrainingHistory[24];
  CS_PCIE_TRAINING_DATA   TrainingData[10];
  UINT32                  GnbDataValue;
  UINT32                  InitialTimerValue;
  UINT32                  CurrentTimerValue;
  UINT32                  TimeStampDelta;
  UINT32                  *pTrainingHistory;

// 1) On entry, training is released... could be an issue with reset release?
  //a) Identify which ports are being trained - Need BOOLEAN for training enabled
  //aa) Need state machine value for
  for (PortId = 0; PortId < 10; PortId++) {
    PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
    PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
    if (PortId > 5) {
      *((UINT32 *) (UINTN) PciIndexAddr) = 0x01500800 + (0x100 * PortId);
    } else {
      *((UINT32 *) (UINTN) PciIndexAddr) = 0x01510800 + (0x100 * (PortId - 5));
    }
    GnbDataValue = *((UINT32 *) (UINTN) PciDataAddr);
    ///@todo, don't have defination for D0F0xE4_WRAP_0800_HoldTraining_MASK in GnbRegistersCZ.h now
    //if ((GnbDataValue & D0F0xE4_WRAP_0800_HoldTraining_MASK) == 0) {
    if ((GnbDataValue & 0x1) == 0x1) {
      TrainingData[PortId].TrainingState = LinkStateDeviceNotPresent;
      TrainingData[PortId].PortPresent = FALSE;
    } else {
      TrainingData[PortId].TrainingState = LinkStateDetectPresence;
      TrainingData[PortId].PortPresent = TRUE;
      PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_MISC_INDEX_PCI_OFFSET);
      PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_MISC_DATA_PCI_OFFSET);
      *((UINT32 *) (UINTN) PciIndexAddr) = 0x30 + PortId;
      GnbDataValue = *((UINT32 *) (UINTN) PciDataAddr);
      TrainingData[PortId].Function = (UINT8) (GnbDataValue & 0x7);
      TrainingData[PortId].Device = (UINT8) ((GnbDataValue & 0xF8) >> 3);
    }
  }
  //b) Is timer running???
  //c) Is reset released???
  //d) Set initial timer value

  PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
  PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
  // Timer
  *((UINT32 *) (UINTN) PciIndexAddr) = 0x013080F0;
  InitialTimerValue = *((UINT32 *) (UINTN) PciDataAddr);
  do {
    AllPortsDone = 0;

    PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
    PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
    *((UINT32 *) (UINTN) PciIndexAddr) = 0x013080F0;
    CurrentTimerValue = *((UINT32 *) (UINTN) PciDataAddr);
    TimeStampDelta = CurrentTimerValue - InitialTimerValue;

    for (PortId = 0; PortId < 10; PortId++) {
      if (TrainingData[PortId].PortPresent == TRUE) {
        // Read LC State History for training state determination
        PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, 0, GNB_ROOT_PORT_PCI_OFFSET);
        PciIndexAddr += ((UINT64) TrainingData[PortId].Function << 12);
        PciIndexAddr += ((UINT64) TrainingData[PortId].Device << 15);
        PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, 0, GNB_ROOT_PORT_PCI_OFFSET + 4);
        PciDataAddr += ((UINT64) TrainingData[PortId].Function << 12);
        PciDataAddr += ((UINT64) TrainingData[PortId].Device << 15);
        pTrainingHistory = (UINT32 *)&TrainingHistory[0];
        for (Index = 0; Index < 6; Index++) {
          *((UINT32 *) (UINTN) PciIndexAddr) = 0xA5 + Index;
          *pTrainingHistory = *((UINT32 *) (UINTN) PciDataAddr);
          pTrainingHistory++;
        }
      }
      switch (TrainingData[PortId].TrainingState) {
      case LinkStateDetectPresence:
        if (TrainingHistory[0] > 4) {
          TrainingData[PortId].TrainingState = LinkStateDetecting;
          }
        break;
      case LinkStateDetecting:
        if (TrainingHistory[0] > 0xF) {
          TrainingData[PortId].TrainingState = LinkStateTrainingSuccess;
          AllPortsDone++;
        } else {
          // Check for training failure - this should be a critical failure during CS Resume
          for (Index = 0; Index < 16; Index++) {
            if (TrainingHistory[Index] == 0x2A) {
              if ((TrainingHistory[Index + 1] == 0x6) || (TrainingHistory[Index + 1] == 0x9) || (TrainingHistory[Index + 1] == 0xB)) {
                TrainingData[PortId].TrainingState = LinkStateDeviceNotPresent;
                AllPortsDone++;
              }
            }
          }
        }
        break;
      case LinkStateTrainingSuccess:
      case LinkStateDeviceNotPresent:
      case LinkStateTrainingCompleted:
        AllPortsDone++;
        break;
      }
    }
  } while ((AllPortsDone < 10) && (TimeStampDelta < 60000));
}

VOID
F15CzDispatchGnbStall (
  IN       UINT32  Context
  )
{
  UINT32                  TimeStampStart;
  UINT32                  TimeStampDelta;
  UINT32                  TimeStampCurrent;
  UINT64                  PciIndexAddr;
  UINT64                  PciDataAddr;

  PciIndexAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
  PciDataAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
  // Timer
  *((UINT32 *) (UINTN) PciIndexAddr) = 0x013080F0;
  TimeStampStart = *((UINT32 *) (UINTN) PciDataAddr);

  do {
    *((UINT32 *) (UINTN) PciIndexAddr) = 0x013080F0;
    TimeStampCurrent = *((UINT32 *) (UINTN) PciDataAddr);
    TimeStampDelta = ((TimeStampCurrent > TimeStampStart) ? (TimeStampCurrent - TimeStampStart) : (0xffffffffull - TimeStampStart + TimeStampCurrent));
  } while (TimeStampDelta < Context);
}

#define X(GenericFunction) F15CzDispatch##GenericFunction,
CONST PF_GENERIC_DISPATCH_FUNCTION_RESTORE F15CzCsDispatchFunctions [] =
{
  LIST_OF_DISPATCHABLE_FUNCTIONS
};
#undef X

CS_RESTORATION_ENTRY_HEADER*
F15CzRestoreGenericDispatchEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GENERIC_DISPATCH  *GenericDispatchEntry;

  GenericDispatchEntry = (CS_GENERIC_DISPATCH *) CurrentEntry;
  F15CzCsDispatchFunctions[GenericDispatchEntry->FunctionIndex] (GenericDispatchEntry->Context);
  GenericDispatchEntry++;
  return &GenericDispatchEntry->Header;
}


#define X(CsType) F15CzRestore##CsType##Entry,
CONST PF_RESTORE_FROM_TABLE_ENTRY F15CzCsRestorationFunctions [] =
{
  LIST_OF_REGISTER_SPACES
};
#undef X

VOID
ProcessSocReplayBuffer (
  IN       VOID *CsRestoreBuffer
  )
{
  UINT8   PMx04;
  UINT8   IsaCtrlAddr;
  UINT64  PciAddr;
  CS_RESTORATION_ENTRY_HEADER  *CurrentEntry;

  // Initialize global data for faster processing
  mPciMcfgBase = (EfiReadMsr (PCI_MCFG_BASE_MSR)) & PCI_MCFG_MASK;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_MISC_PCI_FUNCTION, GNB_MISC_INDEX_PCI_OFFSET);
  mGnbMiscIndex = (UINT8 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_MISC_PCI_FUNCTION, GNB_MISC_DATA_PCI_OFFSET);
  mGnbMiscData = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_ORB_PCI_FUNCTION, GNB_ORB_INDEX_PCI_OFFSET);
  mGnbOrbIndex = (UINT8 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_ORB_PCI_FUNCTION, GNB_ORB_DATA_PCI_OFFSET);
  mGnbOrbData = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_SMU_PCI_FUNCTION, GNB_SMU_INDEX_PCI_OFFSET);
  mGnbSmuIndex = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_SMU_PCI_FUNCTION, GNB_SMU_DATA_PCI_OFFSET);
  mGnbSmuData = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_DEV_PCI_FUNCTION, GNB_DEV_INDEX_PCI_OFFSET);
  mGnbDevIndex = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_DEV_PCI_FUNCTION, GNB_DEV_DATA_PCI_OFFSET);
  mGnbDevData = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_GBIF_PCI_FUNCTION, GNB_GBIF_INDEX_PCI_OFFSET);
  mGnbGbifIndex = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_GBIF_PCI_FUNCTION, GNB_GBIF_DATA_PCI_OFFSET);
  mGnbGbifData = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_INDEX_PCI_OFFSET);
  mGnbLinkIndex = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_LINK_PCI_FUNCTION, GNB_LINK_DATA_PCI_OFFSET);
  mGnbLinkData = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOAPIC_PCI_FUNCTION, GNB_IOAPIC_INDEX_PCI_OFFSET);
  mGnbIoapicIndex = (UINT8 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOAPIC_PCI_FUNCTION, GNB_IOAPIC_DATA_PCI_OFFSET);
  mGnbIoapicData = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L2_PCI_FUNCTION, GNB_IOMMU_L2_INDEX_PCI_OFFSET);
  mGnbIommuL2Index = (UINT16 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L2_PCI_FUNCTION, GNB_IOMMU_L2_DATA_PCI_OFFSET);
  mGnbIommuL2Data = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L1_PCI_FUNCTION, GNB_IOMMU_L1_INDEX_PCI_OFFSET);
  mGnbIommuL1Index = (UINT32 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, 0, GNB_IOMMU_L1_PCI_FUNCTION, GNB_IOMMU_L1_DATA_PCI_OFFSET);
  mGnbIommuL1Data = (UINT32 *) (UINTN) PciAddr;

  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ACG_PCI_DEVICE, GNB_ACG_PCI_FUNCTION, GNB_ACG_AZ_INDEX_PCI_OFFSET);
  mGnbAcgAzIndex = (UINT8 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ACG_PCI_DEVICE, GNB_ACG_PCI_FUNCTION, GNB_ACG_AZ_DATA_PCI_OFFSET);
  mGnbAcgAzData = (UINT32 *) (UINTN) PciAddr;

  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ACG_PCI_DEVICE, GNB_ACG_PCI_FUNCTION, GNB_ACG_AZ_CMN_INDEX_PCI_OFFSET);
  mGnbAcgAzCmnIndex = (UINT8 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_ACG_PCI_DEVICE, GNB_ACG_PCI_FUNCTION, GNB_ACG_AZ_CMN_DATA_PCI_OFFSET);
  mGnbAcgAzCmnData = (UINT32 *) (UINTN) PciAddr;

  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_SPG_PCI_DEVICE, GNB_SPG_PCI_FUNCTION, GNB_SPG_CMN_INDEX_PCI_OFFSET);
  mGnbSpgCmnIndex = (UINT8 *) (UINTN) PciAddr;
  PciAddr = mPciMcfgBase + MAKE_SBDFO (0, 0, GNB_SPG_PCI_DEVICE, GNB_SPG_PCI_FUNCTION, GNB_SPG_CMN_DATA_PCI_OFFSET);
  mGnbSpgCmnData = (UINT32 *) (UINTN) PciAddr;

  // Enable FCH MMIO range for faster processing
  IsaCtrlAddr = ISA_CONTROL_ADDRESS;
  LibAmdIoWrite (AccessWidth8, PMIO_INDEX, &IsaCtrlAddr, NULL);
  LibAmdIoRead (AccessWidth8, PMIO_DATA, &PMx04, NULL);
  PMx04 |= MMIO_EN_BIT;
  LibAmdIoWrite (AccessWidth8, PMIO_DATA, &PMx04, NULL);

  for (CurrentEntry = (CS_RESTORATION_ENTRY_HEADER *) CsRestoreBuffer; CurrentEntry->Type != CsEndOfTable; CurrentEntry = F15CzCsRestorationFunctions [CurrentEntry->Type] (CurrentEntry)) {
  }
}

VOID
PutExecutingSocCoreIntoCC6State (
  )
{
  UINT64  C6BaseMsr;
  UINT16  C6Port;
  UINT8   DummyRead;

  C6BaseMsr = EfiReadMsr (C6_BASE_MSR);
  C6Port = (UINT16) ((C6BaseMsr & C6_BASE_MASK) >> C6_BASE_SHIFT);
  for (;;) {
    LibAmdIoRead (AccessWidth8, (C6Port + C6_PORT_OFFSET), &DummyRead, NULL);
  }
}
