/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU BrandId related functions and structures.
 *
 * Contains code that provides CPU BrandId information
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15
 * @e \$Revision: 314290 $   @e \$Date: 2015-03-09 11:08:26 +0800 (Mon, 09 Mar 2015) $
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
#include "AGESA.h"
#include "amdlib.h"
#include "cpuRegisters.h"
#include "cpuFamilyTranslation.h"
#include "cpuEarlyInit.h"
#include "cpuServices.h"
#include "GeneralServices.h"
#include "Gnb.h"
#include "GnbLib.h"
#include "GnbPcieConfig.h"
#include "GnbRegistersCZ.h"
#include "GnbHandleLib.h"
#include "GnbSmuInitLibV8.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CPUF15BRANDID_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
/// A structure containing brand string
typedef struct {
  CONST CHAR8 *Stringstart;  ///< The literal string
} CPU_F15_EXCEPTION_BRAND;

/// FAM15_BRAND_STRING_MSR
typedef struct _PROCESSOR_NAME_STRING {
  UINT32 lo;                 ///< lower 32-bits of 64-bit value
  UINT32 hi;                 ///< highest 32-bits of 64-bit value
} PROCESSOR_NAME_STRING;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN
STATIC
IsException (
     OUT   UINT32                 *ExceptionId,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

VOID
STATIC
GetNameStringFromFuse (
     OUT   UINT32                 *NameString,
  IN       DEV_OBJECT             *DevObject,
  IN       UINT32                 *SmuArg,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

VOID
F15SetBrandIdRegistersAtEarly (
  IN       CPU_SPECIFIC_SERVICES  *FamilyServices,
  IN       AMD_CPU_EARLY_PARAMS   *EarlyParams,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
// This is an example, need to be updated once Processor Revision Guide define brand string exception
// Brand string is always 48 bytes
CONST CHAR8 ROMDATA str_Exception_0[48] = "AMD PRO A12-8800B R7, 12 Compute Cores 4C+8G";
CONST CHAR8 ROMDATA str_Unprogrammed_Sample[48] = "AMD Unprogrammed Engineering Sample";
/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */
CONST CPU_F15_EXCEPTION_BRAND ROMDATA CpuF15ExceptionBrandIdString[] =
{
  str_Exception_0
};

/*---------------------------------------------------------------------------------------*/
/**
 * Set the Processor Name String register based on D0F0xBC_xD021_108C
 *
 * This function copies D0F0xBC_xD021_108C to MSR_C001_00[35:30]
 *
 *  @param[in]   FamilyServices      The current Family Specific Services.
 *  @param[in]   EarlyParams         Service parameters.
 *  @param[in]   StdHeader           Config handle for library and services.
 *
 */
VOID
F15SetBrandIdRegistersAtEarly (
  IN       CPU_SPECIFIC_SERVICES  *FamilyServices,
  IN       AMD_CPU_EARLY_PARAMS   *EarlyParams,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32 NameString0;
  UINT32 ExceptionId;
  UINT32 MsrIndex;
  UINT64 MsrData;
  UINT64 *MsrNameStringPtrPtr;
  GNB_HANDLE  *GnbHandle;
  DEV_OBJECT   DevObject;
  UINT32       SmuArg[6];

  if ((IsBsp (StdHeader)) && (!FamilyServices->IsSecureS3 (StdHeader))) {
    if (IsException (&ExceptionId, StdHeader)) {
      ASSERT (ExceptionId < (sizeof (CpuF15ExceptionBrandIdString) / sizeof (CpuF15ExceptionBrandIdString[0])));

      MsrNameStringPtrPtr = (UINT64 *) CpuF15ExceptionBrandIdString[ExceptionId].Stringstart;
    } else {
      // Normally, we get Name String from D0F0xBC_xD021_108C
      GnbHandle = GnbGetHandle (StdHeader);
      GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
      SmuArg[0] = 0xD0211088;
      SmuArg[1] = 0;
      GetNameStringFromFuse (&NameString0, &DevObject, SmuArg, StdHeader);

      if (NameString0 != 0) {
        for (MsrIndex = 0; MsrIndex <= (MSR_CPUID_NAME_STRING5 - MSR_CPUID_NAME_STRING0); MsrIndex++) {
          SmuArg[1] = MsrIndex * 2;
          GetNameStringFromFuse (&(((PROCESSOR_NAME_STRING *) (&MsrData))->lo), &DevObject, SmuArg, StdHeader);
          SmuArg[1] = (MsrIndex * 2) + 1;
          GetNameStringFromFuse (&(((PROCESSOR_NAME_STRING *) (&MsrData))->hi), &DevObject, SmuArg, StdHeader);

          LibAmdMsrWrite ((MsrIndex + MSR_CPUID_NAME_STRING0), &MsrData, StdHeader);
        }
        return;
      }
      // It is unprogrammed (unfused) parts and use a name string of "AMD Unprogrammed Engineering Sample"
      MsrNameStringPtrPtr = (UINT64 *) str_Unprogrammed_Sample;
    }
    // Put values into name MSRs,  Always write the full 48 bytes
    for (MsrIndex = MSR_CPUID_NAME_STRING0; MsrIndex <= MSR_CPUID_NAME_STRING5; MsrIndex++) {
      LibAmdMsrWrite (MsrIndex, MsrNameStringPtrPtr, StdHeader);
      MsrNameStringPtrPtr++;
    }
  }
}

/*---------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Check if it's an exception
 *
 * For family 15h, brand string is obtained from D0F0xBC_xD021_108C, but there may be exceptions.
 * This function checks if it's an exception.
 *
 *  @param[out]  ExceptionId         Id of exception
 *  @param[in]   StdHeader           Config handle for library and services.
 *
 *  @retval      TRUE                It's an exception
 *  @retval      FALSE               It's NOT an exception
 */
BOOLEAN
STATIC
IsException (
     OUT   UINT32                 *ExceptionId,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT16                    ConfigID;
  UINT32                    SmuIndex;
  UINT32                    SmuData;
  PCI_ADDR                  PciAddress;
  BOOLEAN                   IsCzA1;
  NATIVE_DIE_CPUID_REGISTER NativeDieCpuId;

  IsCzA1 = FALSE;
  // Get CPU Stepping from D18F4x160
  PciAddress.AddressValue = NATIVE_DIE_CPUID_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NativeDieCpuId, StdHeader);
  if ((NativeDieCpuId.ExtFamily == 0x6) && (NativeDieCpuId.ExtModel == 0x6) &&
      (NativeDieCpuId.BaseFamily == 0xF) && (NativeDieCpuId.BaseModel == 0x0) && (NativeDieCpuId.Stepping == 0x1)) {
    IsCzA1 = TRUE;
  }

  // Continue checking Config ID only if this is a CZ A1 part
  if (IsCzA1) {
    // Get Config ID from 0xC0016024
    SmuIndex = 0xC0016024;
    GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
    ConfigID = (UINT16) ((SmuData >> 15) & 0xFFF);

    if (ConfigID == 0x5D) {
      *ExceptionId = 0;
      return TRUE;
    }
  }

  *ExceptionId = 0xFFFF;
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 32bits name string from FUSE
 *
 * For family 15h Carrizo, brand string is obtained from D0F0xBC_xD021_108C.
 *
 *  @param[out]  NameString          Name string
 *  @param[in]   DevObject           Pointer to Device object
 *  @param[in]   SmuArg              Request Argument
 *  @param[in]   StdHeader           Config handle for library and services.
 *
 */
VOID
STATIC
GetNameStringFromFuse (
     OUT   UINT32                 *NameString,
  IN       DEV_OBJECT             *DevObject,
  IN       UINT32                 *SmuArg,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32       LocalPciRegister;
  PCI_ADDR     PciAddress;

  // Write Index to D0F0xBC_xD021_1088
  GnbSmuServiceRequestV8 (
    DevObject,
    SMC_MSG_SecureSmnWrite,
    SmuArg,
    0
    );
  // Get Name string from D0F0xBC_xD021_108C
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
  LocalPciRegister = 0xD021108C;
  LibAmdPciWrite (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
  PciAddress.Address.Register = 0xBC;
  LibAmdPciRead (AccessWidth32, PciAddress, NameString, StdHeader);
}

