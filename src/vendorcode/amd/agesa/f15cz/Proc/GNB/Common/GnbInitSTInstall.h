/* $NoKeywords:$ */
/**
 * @file
 *
 * ST service installation file
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312670 $   @e \$Date: 2015-02-10 10:17:13 +0800 (Tue, 10 Feb 2015) $
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

#ifndef _GNB_INIT_ST_INSTALL_H_
#define _GNB_INIT_ST_INSTALL_H_

//-----------------------------------------------------------------------
// Specify definition used by module services
//-----------------------------------------------------------------------

#include  "GnbPcie.h"
#include  "GnbUraServices.h"
#include  "GnbPcieFamServices.h"
#include  "GnbFamServices.h"
#include  "GnbGfxFamServices.h"

//-----------------------------------------------------------------------
// Export services
//-----------------------------------------------------------------------

#if  (AGESA_ENTRY_INIT_EARLY == TRUE)

  extern F_PCIEFMGETSBCONFIGINFO                  PcieGetSbConfigInfoST;
  extern F_PCIEFMGETCOMPLEXDATALENGTH             PcieGetComplexDataLengthST;
  extern F_PCIEFMBUILDCOMPLEXCONFIGURATION        PcieBuildComplexConfigurationST;
  extern F_PCIEFMCONFIGUREENGINESLANEALLOCATION   PcieConfigureEnginesLaneAllocationST;
  extern F_PCIEFMCHECKPORTPCIDEVICEMAPPING        PcieCheckPortPciDeviceMappingST;
  extern F_PCIEFMMAPPORTPCIADDRESS                PcieMapPortPciAddressST;
  extern F_PCIEFMCHECKPORTPCIELANECANBEMUXED      PcieCheckPortPcieLaneCanBeMuxedST;


  PCIe_FAM_CONFIG_SERVICES GnbPcieConfigProtocolST = {
    PcieGetComplexDataLengthST,
    PcieBuildComplexConfigurationST,
    PcieConfigureEnginesLaneAllocationST,
    PcieCheckPortPciDeviceMappingST,
    PcieMapPortPciAddressST,
    PcieCheckPortPcieLaneCanBeMuxedST,
    PcieGetSbConfigInfoST
  };

  GNB_SERVICE GnbPcieConfigServicesST = {
    GnbPcieFamConfigService,
    AMD_FAMILY_ST,
    &GnbPcieConfigProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieConfigServicesST
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  extern F_PCIEFMGETCORECONFIGURATIONVALUE        PcieGetCoreConfigurationValueST;
  extern F_PCIEFMGETLINKSPEEDCAP                  PcieGetLinkSpeedCapST;
  extern F_PCIEFMGETNATIVEPHYLANEBITMAP           PcieGetNativePhyLaneBitmapST;
  extern F_PCIEFMSETLINKSPEEDCAP                  PcieSetLinkSpeedCapV4;

  PCIe_FAM_INIT_SERVICES GnbPcieInitProtocolST = {
    PcieGetCoreConfigurationValueST,
    PcieGetLinkSpeedCapST,
    PcieGetNativePhyLaneBitmapST,
    PcieSetLinkSpeedCapV4
  };

  GNB_SERVICE GnbPcieInitServicesST = {
    GnbPcieFamInitService,
    AMD_FAMILY_ST,
    &GnbPcieInitProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieInitServicesST
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  #if IDSOPT_IDS_ENABLED == TRUE
    #if IDSOPT_TRACING_ENABLED == TRUE
      extern F_PCIEFMDEBUGGETHOSTREGADDRESSSPACESTRING  PcieDebugGetHostRegAddressSpaceStringST;
      extern F_PCIEFMDEBUGGETWRAPPERNAMESTRING          PcieDebugGetWrapperNameStringST;
      extern F_PCIEFMDEBUGGETCORECONFIGURATIONSTRING    PcieDebugGetCoreConfigurationStringST;

      PCIe_FAM_DEBUG_SERVICES GnbPcieDebugProtocolST = {
        PcieDebugGetHostRegAddressSpaceStringST,
        PcieDebugGetWrapperNameStringST,
        PcieDebugGetCoreConfigurationStringST
      };

      GNB_SERVICE GnbPcieDebugServicesST = {
        GnbPcieFamDebugService,
        AMD_FAMILY_ST,
        &GnbPcieDebugProtocolST,
        SERVICES_POINTER
      };
      #undef  SERVICES_POINTER
      #define SERVICES_POINTER  &GnbPcieDebugServicesST
    #endif
  #endif
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  extern F_GNB_REGISTER_ACCESS  GnbRegisterReadST;
  extern F_GNB_REGISTER_ACCESS  GnbRegisterWriteST;

  GNB_REGISTER_SERVICE GnbRegisterAccessProtocolST = {
    GnbRegisterReadST,
    GnbRegisterWriteST
  };

  GNB_SERVICE GnbRegisterAccessServicesST = {
    GnbRegisterAccessService,
    AMD_FAMILY_ST,
    &GnbRegisterAccessProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbRegisterAccessServicesST

  extern F_GNBFMCREATEIVRSENTRY    GnbCreateIvrsEntryST;
  extern F_GNBFMCHECKIOMMUPRESENT  GnbCheckIommuPresentST;

  GNB_FAM_IOMMU_SERVICES GnbIommuConfigProtocolST = {
    GnbCheckIommuPresentST,
    GnbCreateIvrsEntryST
  };

  GNB_SERVICE GnbIommuConfigServicesST = {
    GnbIommuService,
    AMD_FAMILY_ST,
    &GnbIommuConfigProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbIommuConfigServicesST

  extern F_GNBURASERVICELOCATEREGTBL GnbUraLocateRegTblST;
  extern F_GNBURASERVICEGET GnbUraGetST;
  extern F_GNBURASERVICESET GnbUraSetST;
  extern F_GNBURASERVICESTREAMSET GnbUraStreamSetST;

  GNB_URA_SERVICE GnbUraProtocolST = {
    GnbUraLocateRegTblST,
    GnbUraGetST,
    GnbUraSetST,
    GnbUraStreamSetST
  };

  GNB_SERVICE GnbUraServicesST = {
    GnbUraService,
    AMD_FAMILY_ST,
    &GnbUraProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbUraServicesST
  extern F_GFXMAPENGINETODISPLAYPATH GfxMapEngineToDisplayPathST;
  extern F_GFXDISABLECONTROLLER      GfxDisableControllerST;
  extern F_GFXCALCULATECLOCK         GfxCalculateClockST;
  extern F_GFXISVBIOSPOSTED          GfxIsVbiosPostedST;

  GFX_FAM_SERVICES GfxFamilyServiceProtocolST = {
    GfxMapEngineToDisplayPathST,
    GfxDisableControllerST,
    GfxCalculateClockST,
    GfxIsVbiosPostedST
  };

  GNB_SERVICE GfxFamilyServicesST = {
    GfxFamService,
    AMD_FAMILY_ST,
    &GfxFamilyServiceProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GfxFamilyServicesST

  extern F_GNBTIMESTAMP GnbTimeStampST;

  GNB_FAM_TS_SERVICES GnbFamTsProtocolST = {
    GnbTimeStampST,
  };

  GNB_SERVICE GnbFamTsServicesST = {
    GnbFamTsService,
    AMD_FAMILY_ST,
    &GnbFamTsProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbFamTsServicesST

  extern F_GNBFMALIBGETBASETABLE PcieAlibGetBaseTableST;
  #define AlibGetBaseTableST  PcieAlibGetBaseTableST

  GNB_FAM_ALIB_SERVICES AlibProtocolST = {
    AlibGetBaseTableST
  };

  GNB_SERVICE AlibServicesST = {
    GnbFamAlibService,
    AMD_FAMILY_ST,
    &AlibProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &AlibServicesST

  extern F_GNBFMALIBGETBUILDOPTIONS GnbGetBuildOptionsST;

  GNB_FAM_BUILDOPTIONS_POINTERS GnbOptionsProtocolST = {
    &GnbGetBuildOptionsST
  };

  GNB_SERVICE GnbOptionsServicesST = {
    GnbFamBuildOptionPointer,
    AMD_FAMILY_ST,
    &GnbOptionsProtocolST,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbOptionsServicesST

#endif
#endif  // _GNB_INIT_ST_INSTALL_H_
