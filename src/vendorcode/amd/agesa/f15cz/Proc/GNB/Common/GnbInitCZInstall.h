/* $NoKeywords:$ */
/**
 * @file
 *
 * CZ service installation file
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312448 $   @e \$Date: 2015-02-06 10:57:34 +0800 (Fri, 06 Feb 2015) $
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

#ifndef _GNB_INIT_CZ_INSTALL_H_
#define _GNB_INIT_CZ_INSTALL_H_

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

  extern F_PCIEFMGETSBCONFIGINFO                  PcieGetSbConfigInfoCZ;
  extern F_PCIEFMGETCOMPLEXDATALENGTH             PcieGetComplexDataLengthCZ;
  extern F_PCIEFMBUILDCOMPLEXCONFIGURATION        PcieBuildComplexConfigurationCZ;
  extern F_PCIEFMCONFIGUREENGINESLANEALLOCATION   PcieConfigureEnginesLaneAllocationCZ;
  extern F_PCIEFMCHECKPORTPCIDEVICEMAPPING        PcieCheckPortPciDeviceMappingCZ;
  extern F_PCIEFMMAPPORTPCIADDRESS                PcieMapPortPciAddressCZ;
  extern F_PCIEFMCHECKPORTPCIELANECANBEMUXED      PcieCheckPortPcieLaneCanBeMuxedCZ;


  PCIe_FAM_CONFIG_SERVICES GnbPcieConfigProtocolCZ = {
    PcieGetComplexDataLengthCZ,
    PcieBuildComplexConfigurationCZ,
    PcieConfigureEnginesLaneAllocationCZ,
    PcieCheckPortPciDeviceMappingCZ,
    PcieMapPortPciAddressCZ,
    PcieCheckPortPcieLaneCanBeMuxedCZ,
    PcieGetSbConfigInfoCZ
  };

  GNB_SERVICE GnbPcieConfigServicesCZ = {
    GnbPcieFamConfigService,
    AMD_FAMILY_CZ,
    &GnbPcieConfigProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieConfigServicesCZ
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  extern F_PCIEFMGETCORECONFIGURATIONVALUE        PcieGetCoreConfigurationValueCZ;
  extern F_PCIEFMGETLINKSPEEDCAP                  PcieGetLinkSpeedCapCZ;
  extern F_PCIEFMGETNATIVEPHYLANEBITMAP           PcieGetNativePhyLaneBitmapCZ;
  extern F_PCIEFMSETLINKSPEEDCAP                  PcieSetLinkSpeedCapV4;

  PCIe_FAM_INIT_SERVICES GnbPcieInitProtocolCZ = {
    PcieGetCoreConfigurationValueCZ,
    PcieGetLinkSpeedCapCZ,
    PcieGetNativePhyLaneBitmapCZ,
    PcieSetLinkSpeedCapV4
  };

  GNB_SERVICE GnbPcieInitServicesCZ = {
    GnbPcieFamInitService,
    AMD_FAMILY_CZ,
    &GnbPcieInitProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieInitServicesCZ
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  #if IDSOPT_IDS_ENABLED == TRUE
    #if IDSOPT_TRACING_ENABLED == TRUE
      extern F_PCIEFMDEBUGGETHOSTREGADDRESSSPACESTRING  PcieDebugGetHostRegAddressSpaceStringCZ;
      extern F_PCIEFMDEBUGGETWRAPPERNAMESTRING          PcieDebugGetWrapperNameStringCZ;
      extern F_PCIEFMDEBUGGETCORECONFIGURATIONSTRING    PcieDebugGetCoreConfigurationStringCZ;

      PCIe_FAM_DEBUG_SERVICES GnbPcieDebugProtocolCZ = {
        PcieDebugGetHostRegAddressSpaceStringCZ,
        PcieDebugGetWrapperNameStringCZ,
        PcieDebugGetCoreConfigurationStringCZ
      };

      GNB_SERVICE GnbPcieDebugServicesCZ = {
        GnbPcieFamDebugService,
        AMD_FAMILY_CZ,
        &GnbPcieDebugProtocolCZ,
        SERVICES_POINTER
      };
      #undef  SERVICES_POINTER
      #define SERVICES_POINTER  &GnbPcieDebugServicesCZ
    #endif
  #endif
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  extern F_GNB_REGISTER_ACCESS  GnbRegisterReadCZ;
  extern F_GNB_REGISTER_ACCESS  GnbRegisterWriteCZ;

  GNB_REGISTER_SERVICE GnbRegisterAccessProtocolCZ = {
    GnbRegisterReadCZ,
    GnbRegisterWriteCZ
  };

  GNB_SERVICE GnbRegisterAccessServicesCZ = {
    GnbRegisterAccessService,
    AMD_FAMILY_CZ,
    &GnbRegisterAccessProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbRegisterAccessServicesCZ

  extern F_GNBFMCREATEIVRSENTRY    GnbCreateIvrsEntryCZ;
  extern F_GNBFMCHECKIOMMUPRESENT  GnbCheckIommuPresentCZ;

  GNB_FAM_IOMMU_SERVICES GnbIommuConfigProtocolCZ = {
    GnbCheckIommuPresentCZ,
    GnbCreateIvrsEntryCZ
  };

  GNB_SERVICE GnbIommuConfigServicesCZ = {
    GnbIommuService,
    AMD_FAMILY_CZ,
    &GnbIommuConfigProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbIommuConfigServicesCZ

  extern F_GNBURASERVICELOCATEREGTBL GnbUraLocateRegTblCZ;
  extern F_GNBURASERVICEGET GnbUraGetCZ;
  extern F_GNBURASERVICESET GnbUraSetCZ;
  extern F_GNBURASERVICESTREAMSET GnbUraStreamSetCZ;

  GNB_URA_SERVICE GnbUraProtocolCZ = {
    GnbUraLocateRegTblCZ,
    GnbUraGetCZ,
    GnbUraSetCZ,
    GnbUraStreamSetCZ
  };

  GNB_SERVICE GnbUraServicesCZ = {
    GnbUraService,
    AMD_FAMILY_CZ,
    &GnbUraProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbUraServicesCZ
  extern F_GFXMAPENGINETODISPLAYPATH GfxMapEngineToDisplayPathCZ;
  extern F_GFXDISABLECONTROLLER      GfxDisableControllerCZ;
  extern F_GFXCALCULATECLOCK         GfxCalculateClockCZ;
  extern F_GFXISVBIOSPOSTED          GfxIsVbiosPostedCZ;

  GFX_FAM_SERVICES GfxFamilyServiceProtocolCZ = {
    GfxMapEngineToDisplayPathCZ,
    GfxDisableControllerCZ,
    GfxCalculateClockCZ,
    GfxIsVbiosPostedCZ
  };

  GNB_SERVICE GfxFamilyServicesCZ = {
    GfxFamService,
    AMD_FAMILY_CZ,
    &GfxFamilyServiceProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GfxFamilyServicesCZ

  extern F_GNBTIMESTAMP GnbTimeStampCZ;

  GNB_FAM_TS_SERVICES GnbFamTsProtocolCZ = {
    GnbTimeStampCZ,
  };

  GNB_SERVICE GnbFamTsServicesCZ = {
    GnbFamTsService,
    AMD_FAMILY_CZ,
    &GnbFamTsProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbFamTsServicesCZ

  extern F_GNBFMALIBGETBASETABLE PcieAlibGetBaseTableCZ;
  #define AlibGetBaseTableCZ  PcieAlibGetBaseTableCZ

  GNB_FAM_ALIB_SERVICES AlibProtocolCZ = {
    AlibGetBaseTableCZ
  };

  GNB_SERVICE AlibServicesCZ = {
    GnbFamAlibService,
    AMD_FAMILY_CZ,
    &AlibProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &AlibServicesCZ

  extern F_GNBFMALIBGETBUILDOPTIONS GnbGetBuildOptionsCZ;

  GNB_FAM_BUILDOPTIONS_POINTERS GnbOptionsProtocolCZ = {
    &GnbGetBuildOptionsCZ
  };

  GNB_SERVICE GnbOptionsServicesCZ = {
    GnbFamBuildOptionPointer,
    AMD_FAMILY_CZ,
    &GnbOptionsProtocolCZ,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbOptionsServicesCZ

#endif
#endif  // _GNB_INIT_CZ_INSTALL_H_
