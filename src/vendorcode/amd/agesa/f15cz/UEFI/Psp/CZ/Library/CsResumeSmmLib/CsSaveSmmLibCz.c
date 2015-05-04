/* $NoKeywords:$ */
/**
 * @file
 *
 * CS Save SMM Library for family 0x15 CZ CPU
 *
 * Save registers value for CS
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CS Resume SMM Driver
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "CsResumeSmmLib.h"
#include "CsSaveSmmLibCz.h"
#include "PspBaseLib.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern CONST UINT8 F15CzCsRestoreTable[];
extern CONST UINTN SizeOfF15CzCsRestoreTable;
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

UINT16  AbIoAddress = 0xFFFF;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a PCI entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessPciEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_PCI    *PciEntry;
  PCI_ADDR  PciAddr;

  PciEntry = (CS_PCI *) CurrentEntry;
  if (PciEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = PciEntry->Address;
    LibAmdPciRead (AccessWidth32, PciAddr, &PciEntry->Value, NULL);
  }
  PciEntry++;
  return &PciEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a PCI bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessPciBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR     PciAddr;
  CS_PCI_BITS  *PciBitsEntry;

  PciBitsEntry = (CS_PCI_BITS *) CurrentEntry;
  if (PciBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = PciBitsEntry->Address;
    LibAmdPciRead (AccessWidth32, PciAddr, &PciBitsEntry->Value, NULL);
    PciBitsEntry->Value &= ~PciBitsEntry->Mask;
  }
  PciBitsEntry++;
  return &PciBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a MMIO entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessMmioEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_MMIO  *MmioEntry;

  MmioEntry = (CS_MMIO *) CurrentEntry;
  if (MmioEntry->Header.SaveReadValue) {
    LibAmdMemRead (AccessWidth32, MmioEntry->Address, &MmioEntry->Value, NULL);
  }
  MmioEntry++;
  return &MmioEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a MMIO bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessMmioBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_MMIO_BITS  *MmioBitsEntry;

  MmioBitsEntry = (CS_MMIO_BITS *) CurrentEntry;
  if (MmioBitsEntry->Header.SaveReadValue) {
    LibAmdMemRead (AccessWidth32, MmioBitsEntry->Address, &MmioBitsEntry->Value, NULL);
    MmioBitsEntry->Value &= ~MmioBitsEntry->Mask;
  }
  MmioBitsEntry++;
  return &MmioBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB miscellaneous entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbMiscIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR     PciAddr;
  CS_GNB_MISC  *GnbMiscEntry;

  GnbMiscEntry = (CS_GNB_MISC *) CurrentEntry;
  if (GnbMiscEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectRead (PciAddr, GnbMiscEntry->Address, AccessWidth32, &GnbMiscEntry->Value);
  }
  GnbMiscEntry++;
  return &GnbMiscEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB miscellaneous bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbMiscIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR          PciAddr;
  CS_GNB_MISC_BITS  *GnbMiscBitsEntry;

  GnbMiscBitsEntry = (CS_GNB_MISC_BITS *) CurrentEntry;
  if (GnbMiscBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectRead (PciAddr, GnbMiscBitsEntry->Address, AccessWidth32, &GnbMiscBitsEntry->Value);
    GnbMiscBitsEntry->Value &= ~GnbMiscBitsEntry->Mask;
  }
  GnbMiscBitsEntry++;
  return &GnbMiscBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ORB entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbOrbCfgEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR        PciAddr;
  CS_GNB_ORB_CFG  *GnbOrbCfgEntry;

  GnbOrbCfgEntry = (CS_GNB_ORB_CFG *) CurrentEntry;
  if (GnbOrbCfgEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x94);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbOrbCfgEntry->Address, AccessWidth32, &GnbOrbCfgEntry->Value);
  }
  GnbOrbCfgEntry++;
  return &GnbOrbCfgEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ORB bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbOrbCfgBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR             PciAddr;
  CS_GNB_ORB_CFG_BITS  *GnbOrbCfgBitsEntry;

  GnbOrbCfgBitsEntry = (CS_GNB_ORB_CFG_BITS *) CurrentEntry;
  if (GnbOrbCfgBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x94);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbOrbCfgBitsEntry->Address, AccessWidth32, &GnbOrbCfgBitsEntry->Value);
    GnbOrbCfgBitsEntry->Value &= ~GnbOrbCfgBitsEntry->Mask;
  }
  GnbOrbCfgBitsEntry++;
  return &GnbOrbCfgBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB SMU entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbSmuIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR    PciAddr;
  CS_GNB_SMU  *GnbSmuEntry;

  GnbSmuEntry = (CS_GNB_SMU *) CurrentEntry;
  if (GnbSmuEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
    PspLibPciIndirectRead (PciAddr, GnbSmuEntry->Address, AccessWidth32, &GnbSmuEntry->Value);
  }
  GnbSmuEntry++;
  return &GnbSmuEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB SMU bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbSmuIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR         PciAddr;
  CS_GNB_SMU_BITS  *GnbSmuBitsEntry;

  GnbSmuBitsEntry = (CS_GNB_SMU_BITS *) CurrentEntry;
  if (GnbSmuBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
    PspLibPciIndirectRead (PciAddr, GnbSmuBitsEntry->Address, AccessWidth32, &GnbSmuBitsEntry->Value);
    GnbSmuBitsEntry->Value &= ~GnbSmuBitsEntry->Mask;
  }
  GnbSmuBitsEntry++;
  return &GnbSmuBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB device entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbDevIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32      Index;
  PCI_ADDR    PciAddr;
  CS_GNB_DEV  *GnbDevEntry;

  GnbDevEntry = (CS_GNB_DEV *) CurrentEntry;
  if (GnbDevEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xC8);
    Index = ((UINT32) GnbDevEntry->Device << 16) | (UINT32) GnbDevEntry->Address;
    PspLibPciIndirectRead (PciAddr, Index, AccessWidth32, &GnbDevEntry->Value);
  }
  GnbDevEntry++;
  return &GnbDevEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB device bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbDevIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32           Index;
  PCI_ADDR         PciAddr;
  CS_GNB_DEV_BITS  *GnbDevBitsEntry;

  GnbDevBitsEntry = (CS_GNB_DEV_BITS *) CurrentEntry;
  if (GnbDevBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xC8);
    Index = ((UINT32) GnbDevBitsEntry->Device << 16) | (UINT32) GnbDevBitsEntry->Address;
    PspLibPciIndirectRead (PciAddr, Index, AccessWidth32, &GnbDevBitsEntry->Value);
    GnbDevBitsEntry->Value &= ~GnbDevBitsEntry->Mask;
  }
  GnbDevBitsEntry++;
  return &GnbDevBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GBIF entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbGbifIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR     PciAddr;
  CS_GNB_GBIF  *GnbGbifEntry;

  GnbGbifEntry = (CS_GNB_GBIF *) CurrentEntry;
  if (GnbGbifEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xD0);
    PspLibPciIndirectRead (PciAddr, GnbGbifEntry->Address, AccessWidth32, &GnbGbifEntry->Value);
  }
  GnbGbifEntry++;
  return &GnbGbifEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GBIF bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbGbifIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR          PciAddr;
  CS_GNB_GBIF_BITS  *GnbGbifBitsEntry;

  GnbGbifBitsEntry = (CS_GNB_GBIF_BITS *) CurrentEntry;
  if (GnbGbifBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xD0);
    PspLibPciIndirectRead (PciAddr, GnbGbifBitsEntry->Address, AccessWidth32, &GnbGbifBitsEntry->Value);
    GnbGbifBitsEntry->Value &= ~GnbGbifBitsEntry->Mask;
  }
  GnbGbifBitsEntry++;
  return &GnbGbifBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB link entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbLinkIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32       Index;
  PCI_ADDR     PciAddr;
  CS_GNB_LINK  *GnbLinkEntry;

  GnbLinkEntry = (CS_GNB_LINK *) CurrentEntry;
  if (GnbLinkEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xE0);
    Index = (((UINT32) GnbLinkEntry->Block << 24) | ((UINT32) GnbLinkEntry->Frame << 16)) | (UINT32) GnbLinkEntry->Address;
    PspLibPciIndirectRead (PciAddr, Index, AccessWidth32, &GnbLinkEntry->Value);
  }
  GnbLinkEntry++;
  return &GnbLinkEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB link bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbLinkIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32            Index;
  PCI_ADDR          PciAddr;
  CS_GNB_LINK_BITS  *GnbLinkBitsEntry;

  GnbLinkBitsEntry = (CS_GNB_LINK_BITS *) CurrentEntry;
  if (GnbLinkBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xE0);
    Index = (((UINT32) GnbLinkBitsEntry->Block << 24) | ((UINT32) GnbLinkBitsEntry->Frame << 16)) | (UINT32) GnbLinkBitsEntry->Address;
    PspLibPciIndirectRead (PciAddr, Index, AccessWidth32, &GnbLinkBitsEntry->Value);
    GnbLinkBitsEntry->Value &= ~GnbLinkBitsEntry->Mask;
  }
  GnbLinkBitsEntry++;
  return &GnbLinkBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOAPIC entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbIoapicIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32         RegisterValue;
  PCI_ADDR       PciAddr;
  CS_GNB_IOAPIC  *GnbIoapicEntry;

  GnbIoapicEntry = (CS_GNB_IOAPIC *) CurrentEntry;
  if (GnbIoapicEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xF8);
    RegisterValue = (UINT32) GnbIoapicEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, NULL);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbIoapicEntry->Value, NULL);
  }
  GnbIoapicEntry++;
  return &GnbIoapicEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOAPIC bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbIoapicIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32              RegisterValue;
  PCI_ADDR            PciAddr;
  CS_GNB_IOAPIC_BITS  *GnbIoapicBitsEntry;

  GnbIoapicBitsEntry = (CS_GNB_IOAPIC_BITS *) CurrentEntry;
  if (GnbIoapicBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xF8);
    RegisterValue = (UINT32) GnbIoapicBitsEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, NULL);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbIoapicBitsEntry->Value, NULL);
    GnbIoapicBitsEntry->Value &= ~GnbIoapicBitsEntry->Mask;
  }
  GnbIoapicBitsEntry++;
  return &GnbIoapicBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L2 entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessIommuL2IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR         PciAddr;
  CS_GNB_IOMMU_L2  *GnbIommuL2Entry;
  UINT32           Misc0D;
  UINT32           Misc0DSave;

  GnbIommuL2Entry = (CS_GNB_IOMMU_L2 *) CurrentEntry;
  if (GnbIommuL2Entry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectRead (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
    Misc0D = 1;
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0D);

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF0);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbIommuL2Entry->Address, AccessWidth32, &GnbIommuL2Entry->Value);

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
  }
  GnbIommuL2Entry++;
  return &GnbIommuL2Entry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L2 bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessIommuL2IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR              PciAddr;
  CS_GNB_IOMMU_L2_BITS  *GnbIommuL2BitsEntry;
  UINT32                Misc0D;
  UINT32                Misc0DSave;

  GnbIommuL2BitsEntry = (CS_GNB_IOMMU_L2_BITS *) CurrentEntry;
  if (GnbIommuL2BitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectRead (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
    Misc0D = 1;
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0D);

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF0);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbIommuL2BitsEntry->Address, AccessWidth32, &GnbIommuL2BitsEntry->Value);
    GnbIommuL2BitsEntry->Value &= ~GnbIommuL2BitsEntry->Mask;

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
  }
  GnbIommuL2BitsEntry++;
  return &GnbIommuL2BitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L1 entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessIommuL1IndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32           Index;
  PCI_ADDR         PciAddr;
  CS_GNB_IOMMU_L1  *GnbIommuL1Entry;
  UINT32           Misc0D;
  UINT32           Misc0DSave;

  GnbIommuL1Entry = (CS_GNB_IOMMU_L1 *) CurrentEntry;
  if (GnbIommuL1Entry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectRead (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
    Misc0D = 1;
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0D);

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF8);
    Index = ((UINT32) GnbIommuL1Entry->Select << 16) | (UINT32) GnbIommuL1Entry->Address;
    PspLibPciIndirectRead (PciAddr, Index, AccessWidth32, &GnbIommuL1Entry->Value);

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
  }
  GnbIommuL1Entry++;
  return &GnbIommuL1Entry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB IOMMU L1 bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessIommuL1IndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32                Index;
  PCI_ADDR              PciAddr;
  CS_GNB_IOMMU_L1_BITS  *GnbIommuL1BitsEntry;
  UINT32                Misc0D;
  UINT32                Misc0DSave;

  GnbIommuL1BitsEntry = (CS_GNB_IOMMU_L1_BITS *) CurrentEntry;
  if (GnbIommuL1BitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectRead (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
    Misc0D = 1;
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0D);

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0xF8);
    Index = ((UINT32) GnbIommuL1BitsEntry->Select << 16) | (UINT32) GnbIommuL1BitsEntry->Address;
    PspLibPciIndirectRead (PciAddr, Index, AccessWidth32, &GnbIommuL1BitsEntry->Value);
    GnbIommuL1BitsEntry->Value &= ~GnbIommuL1BitsEntry->Mask;

    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0x60);
    PspLibPciIndirectWrite (PciAddr, 0x0D, AccessWidth32, &Misc0DSave);
  }
  GnbIommuL1BitsEntry++;
  return &GnbIommuL1BitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB root port entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbRootPortIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32            RegisterValue;
  PCI_ADDR          PciAddr;
  CS_GNB_ROOT_PORT  *GnbRootPortEntry;

  GnbRootPortEntry = (CS_GNB_ROOT_PORT *) CurrentEntry;
  if (GnbRootPortEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xE0);
    PciAddr.Address.Device = GnbRootPortEntry->Device;
    PciAddr.Address.Function = GnbRootPortEntry->Function;
    RegisterValue = (UINT32) GnbRootPortEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, NULL);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbRootPortEntry->Value, NULL);
  }
  GnbRootPortEntry++;
  return &GnbRootPortEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB root port bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbRootPortIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32                 RegisterValue;
  PCI_ADDR               PciAddr;
  CS_GNB_ROOT_PORT_BITS  *GnbRootPortBitsEntry;

  GnbRootPortBitsEntry = (CS_GNB_ROOT_PORT_BITS *) CurrentEntry;
  if (GnbRootPortBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xE0);
    PciAddr.Address.Device = GnbRootPortBitsEntry->Device;
    PciAddr.Address.Function = GnbRootPortBitsEntry->Function;
    RegisterValue = (UINT32) GnbRootPortBitsEntry->Address;
    LibAmdPciWrite (AccessWidth32, PciAddr, &RegisterValue, NULL);
    PciAddr.AddressValue += 4;
    LibAmdPciRead (AccessWidth32, PciAddr, &GnbRootPortBitsEntry->Value, NULL);
    GnbRootPortBitsEntry->Value &= ~GnbRootPortBitsEntry->Mask;
  }
  GnbRootPortBitsEntry++;
  return &GnbRootPortBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GMM entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbGmmEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32         *GmmRegister;
  UINT32         GmmBar;
  UINT64         MmioAddr;
  PCI_ADDR       PciAddr;
  CS_GNB_GMM     *GnbGmmEntry;

  GnbGmmEntry = (CS_GNB_GMM *) CurrentEntry;
  if (GnbGmmEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 1, 0, 0x24);
    LibAmdPciRead (AccessWidth32, PciAddr, &GmmBar, NULL);
    MmioAddr = (UINT64) (GmmBar & 0xFFFF0000);
    MmioAddr += GnbGmmEntry->Address;
    GmmRegister = (UINT32 *) (UINTN) MmioAddr;
    GnbGmmEntry->Value = *GmmRegister;
  }
  GnbGmmEntry++;
  return &GnbGmmEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB GMM bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGnbGmmBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32            *GmmRegister;
  UINT32            GmmBar;
  UINT64            MmioAddr;
  PCI_ADDR          PciAddr;
  CS_GNB_GMM_BITS   *GnbGmmBitsEntry;

  GnbGmmBitsEntry = (CS_GNB_GMM_BITS *) CurrentEntry;
  if (GnbGmmBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 1, 0, 0x24);
    LibAmdPciRead (AccessWidth32, PciAddr, &GmmBar, NULL);
    MmioAddr = (UINT64) (GmmBar & 0xFFFF0000);
    MmioAddr += GnbGmmBitsEntry->Address;
    GmmRegister = (UINT32 *) (UINTN) MmioAddr;
    GnbGmmBitsEntry->Value = *GmmRegister & ~GnbGmmBitsEntry->Mask;
  }
  GnbGmmBitsEntry++;
  return &GnbGmmBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ACG AZ entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessAcgAzIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR         PciAddr;
  CS_GNB_ACG_AZ  *GnbAcgAzEntry;

  GnbAcgAzEntry = (CS_GNB_ACG_AZ *) CurrentEntry;
  if (GnbAcgAzEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 9, 2, 0xF0);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbAcgAzEntry->Address, AccessWidth32, &GnbAcgAzEntry->Value);
  }
  GnbAcgAzEntry++;
  return &GnbAcgAzEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ACG AZ bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessAcgAzIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR              PciAddr;
  CS_GNB_ACG_AZ_BITS  *GnbAcgAzBitsEntry;

  GnbAcgAzBitsEntry = (CS_GNB_ACG_AZ_BITS *) CurrentEntry;
  if (GnbAcgAzBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 9, 2, 0xF0);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbAcgAzBitsEntry->Address, AccessWidth32, &GnbAcgAzBitsEntry->Value);
    GnbAcgAzBitsEntry->Value &= ~GnbAcgAzBitsEntry->Mask;
  }
  GnbAcgAzBitsEntry++;
  return &GnbAcgAzBitsEntry->Header;
}
/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ACG AZ Cmn entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessAcgAzCmnIndexEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR           PciAddr;
  CS_GNB_ACG_AZ_CMN  *GnbAcgAzCmnEntry;

  GnbAcgAzCmnEntry = (CS_GNB_ACG_AZ_CMN *) CurrentEntry;
  if (GnbAcgAzCmnEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 9, 2, 0xE8);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbAcgAzCmnEntry->Address, AccessWidth32, &GnbAcgAzCmnEntry->Value);
  }
  GnbAcgAzCmnEntry++;
  return &GnbAcgAzCmnEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB ACG AZ CMN bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessAcgAzCmnIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR              PciAddr;
  CS_GNB_ACG_AZ_CMN_BITS  *GnbAcgAzCmnBitsEntry;

  GnbAcgAzCmnBitsEntry = (CS_GNB_ACG_AZ_CMN_BITS *) CurrentEntry;
  if (GnbAcgAzCmnBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 9, 2, 0xE8);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbAcgAzCmnBitsEntry->Address, AccessWidth32, &GnbAcgAzCmnBitsEntry->Value);
    GnbAcgAzCmnBitsEntry->Value &= ~GnbAcgAzCmnBitsEntry->Mask;
  }
  GnbAcgAzCmnBitsEntry++;
  return &GnbAcgAzCmnBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a GNB SPG CMN bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessSpgCmnIndexBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  PCI_ADDR              PciAddr;
  CS_GNB_SPG_CMN_BITS  *GnbSpgCmnBitsEntry;

  GnbSpgCmnBitsEntry = (CS_GNB_SPG_CMN_BITS *) CurrentEntry;
  if (GnbSpgCmnBitsEntry->Header.SaveReadValue) {
    PciAddr.AddressValue = MAKE_SBDFO (0, 0, 8, 0, 0xE8);
    PspLibPciIndirectRead (PciAddr, (UINT32) GnbSpgCmnBitsEntry->Address, AccessWidth32, &GnbSpgCmnBitsEntry->Value);
    GnbSpgCmnBitsEntry->Value &= ~GnbSpgCmnBitsEntry->Mask;
  }
  GnbSpgCmnBitsEntry++;
  return &GnbSpgCmnBitsEntry->Header;
}
/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH AB link entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchAbEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32     RegisterIndex;
  UINT64     MmioAddr;
  CS_FCH_AB  *FchAbEntry;

  FchAbEntry = (CS_FCH_AB *) CurrentEntry;
  if (FchAbEntry->Header.SaveReadValue) {
    if (AbIoAddress == 0xFFFF) {
      MmioAddr = 0xFED803E0;
      LibAmdMemRead (AccessWidth16, MmioAddr, &AbIoAddress, NULL);
    }
    RegisterIndex = FchAbEntry->Address + 0xC0000000;
    LibAmdIoWrite (AccessWidth32, AbIoAddress, &RegisterIndex, NULL);
    LibAmdIoRead (AccessWidth32, AbIoAddress + 4, &FchAbEntry->Value, NULL);
  }
  FchAbEntry++;
  return &FchAbEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH AB link bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchAbBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT32          RegisterIndex;
  UINT64          MmioAddr;
  CS_FCH_AB_BITS  *FchAbBitsEntry;

  FchAbBitsEntry = (CS_FCH_AB_BITS *) CurrentEntry;
  if (FchAbBitsEntry->Header.SaveReadValue) {
    if (AbIoAddress == 0xFFFF) {
      MmioAddr = 0xFED803E0;
      LibAmdMemRead (AccessWidth16, MmioAddr, &AbIoAddress, NULL);
    }
    RegisterIndex = FchAbBitsEntry->Address + 0xC0000000;
    LibAmdIoWrite (AccessWidth32, AbIoAddress, &RegisterIndex, NULL);
    LibAmdIoRead (AccessWidth32, AbIoAddress + 4, &FchAbBitsEntry->Value, NULL);
    FchAbBitsEntry->Value &= ~FchAbBitsEntry->Mask;
  }
  FchAbBitsEntry++;
  return &FchAbBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 32bit memory mapped IO entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchMmioDwordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64             MmioAddress;
  CS_FCH_MMIO_DWORD  *FchMmioDwordEntry;

  FchMmioDwordEntry = (CS_FCH_MMIO_DWORD *) CurrentEntry;
  if (FchMmioDwordEntry->Header.SaveReadValue) {
    MmioAddress = FCH_MMIO_BASE + FchMmioDwordEntry->Address;
    LibAmdMemRead (AccessWidth32, MmioAddress, &FchMmioDwordEntry->Value, NULL);
  }
  FchMmioDwordEntry++;
  return &FchMmioDwordEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 32bit memory mapped IO bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchMmioDwordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                  MmioAddress;
  CS_FCH_MMIO_DWORD_BITS  *FchMmioDwordBitsEntry;

  FchMmioDwordBitsEntry = (CS_FCH_MMIO_DWORD_BITS *) CurrentEntry;
  if (FchMmioDwordBitsEntry->Header.SaveReadValue) {
    MmioAddress = FCH_MMIO_BASE + FchMmioDwordBitsEntry->Address;
    LibAmdMemRead (AccessWidth32, MmioAddress, &FchMmioDwordBitsEntry->Value, NULL);
    FchMmioDwordBitsEntry->Value &= ~FchMmioDwordBitsEntry->Mask;
  }
  FchMmioDwordBitsEntry++;
  return &FchMmioDwordBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 16bit memory mapped IO entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchMmioWordEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            MmioAddress;
  CS_FCH_MMIO_WORD  *FchMmioWordEntry;

  FchMmioWordEntry = (CS_FCH_MMIO_WORD *) CurrentEntry;
  if (FchMmioWordEntry->Header.SaveReadValue) {
    MmioAddress = FCH_MMIO_BASE + FchMmioWordEntry->Address;
    LibAmdMemRead (AccessWidth16, MmioAddress, &FchMmioWordEntry->Value, NULL);
  }
  FchMmioWordEntry++;
  return &FchMmioWordEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 16bit memory mapped IO bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchMmioWordBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 MmioAddress;
  CS_FCH_MMIO_WORD_BITS  *FchMmioWordBitsEntry;

  FchMmioWordBitsEntry = (CS_FCH_MMIO_WORD_BITS *) CurrentEntry;
  if (FchMmioWordBitsEntry->Header.SaveReadValue) {
    MmioAddress = FCH_MMIO_BASE + FchMmioWordBitsEntry->Address;
    LibAmdMemRead (AccessWidth16, MmioAddress, &FchMmioWordBitsEntry->Value, NULL);
    FchMmioWordBitsEntry->Value &= ~FchMmioWordBitsEntry->Mask;
  }
  FchMmioWordBitsEntry++;
  return &FchMmioWordBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 8bit memory mapped IO entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchMmioByteEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64            MmioAddress;
  CS_FCH_MMIO_BYTE  *FchMmioByteEntry;

  FchMmioByteEntry = (CS_FCH_MMIO_BYTE *) CurrentEntry;
  if (FchMmioByteEntry->Header.SaveReadValue) {
    MmioAddress = FCH_MMIO_BASE + FchMmioByteEntry->Address;
    LibAmdMemRead (AccessWidth8, MmioAddress, &FchMmioByteEntry->Value, NULL);
  }
  FchMmioByteEntry++;
  return &FchMmioByteEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare an FCH 8bit memory mapped IO bits entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessFchMmioByteBitsEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  UINT64                 MmioAddress;
  CS_FCH_MMIO_BYTE_BITS  *FchMmioByteBitsEntry;

  FchMmioByteBitsEntry = (CS_FCH_MMIO_BYTE_BITS *) CurrentEntry;
  if (FchMmioByteBitsEntry->Header.SaveReadValue) {
    MmioAddress = FCH_MMIO_BASE + FchMmioByteBitsEntry->Address;
    LibAmdMemRead (AccessWidth8, MmioAddress, &FchMmioByteBitsEntry->Value, NULL);
    FchMmioByteBitsEntry->Value &= ~FchMmioByteBitsEntry->Mask;
  }
  FchMmioByteBitsEntry++;
  return &FchMmioByteBitsEntry->Header;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare a generic function dispatch entry on a family 15h Carrizo core.
 *
 * @param[in]    CurrentEntry       Current entry to process
 *
 * @return       Pointer to next table entry
 */
CS_RESTORATION_ENTRY_HEADER*
F15CzProcessGenericDispatchEntry (
  IN       CS_RESTORATION_ENTRY_HEADER  *CurrentEntry
  )
{
  CS_GENERIC_DISPATCH  *GenericDispatchEntry;

  GenericDispatchEntry = (CS_GENERIC_DISPATCH *) CurrentEntry;
  // Maybe add a call on the save side to determine if the call is needed on resume
  GenericDispatchEntry++;
  return &GenericDispatchEntry->Header;
}

// Instantiate the array of processing functions
#define X(CsType) F15CzProcess##CsType##Entry,
CONST PF_PROCESS_RESTORATION_TABLE_ENTRY ProcessF15CzRestorationBufferFunctions [] =
{
  LIST_OF_REGISTER_SPACES
};
#undef X

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to build up the Connected Standby restoration table.
 *
 * @param[out]    RestorationTable          Complete family specific restoration table
 * @param[out]    TableSize                 Size of family specific restoration table
 *
 * @return       Family specific error value.
 *
 */
VOID
PrepareRestorationBuffer (
  OUT   VOID     **RestorationTable,
  OUT   UINTN    *TableSize
  )
{
  CS_RESTORATION_ENTRY_HEADER  *CurrentEntry;
  *TableSize = SizeOfF15CzCsRestoreTable;
  *RestorationTable = (VOID *) &F15CzCsRestoreTable;

  PSP_DEBUG ("Prepare Restoration Buffer\n");

  for (CurrentEntry = (CS_RESTORATION_ENTRY_HEADER *) *RestorationTable; CurrentEntry->Type != CsEndOfTable; CurrentEntry = ProcessF15CzRestorationBufferFunctions [CurrentEntry->Type] (CurrentEntry)) {
    ASSERT (CurrentEntry->Type < CsEndOfTable);
  }
  return;
}

