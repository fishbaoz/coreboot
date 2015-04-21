/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: PState
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
 * @e \$Revision: 311976 $   @e \$Date: 2015-01-29 13:34:44 +0800 (Thu, 29 Jan 2015) $
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
 *
 ***************************************************************************/

#ifndef _OPTION_PSTATE_INSTALL_H_
#define _OPTION_PSTATE_INSTALL_H_

#include "cpuPstateTables.h"

/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */

#define F15_CZ_PSTATE_SERVICE_SUPPORT
#define F15_ST_PSTATE_SERVICE_SUPPORT

OPTION_SSDT_FEATURE               GenerateSsdtStub;
OPTION_ACPI_FEATURE               CreateAcpiTablesStub;
#define USER_SSDT_MAIN            GenerateSsdtStub
#define USER_PSTATE_OPTION_MAIN   CreateAcpiTablesStub
#define USER_CSTATE_OPTION_MAIN   CreateAcpiTablesStub
#define USER_PSTATE_CFG_PPC       FALSE
#define USER_PSTATE_CFG_PCT       FALSE
#define USER_PSTATE_CFG_PSD       FALSE
#define USER_PSTATE_CFG_PSS       FALSE
#define USER_PSTATE_CFG_XPSS      FALSE

//
//Define Pstate CPU Family service
//
#ifdef OPTION_FAMILY15H
  #if OPTION_FAMILY15H == TRUE
    #ifdef OPTION_FAMILY15H_CZ
      #if OPTION_FAMILY15H_CZ == TRUE
        extern CONST PSTATE_CPU_FAMILY_SERVICES ROMDATA F15CzPstateServices;
        #undef F15_CZ_PSTATE_SERVICE_SUPPORT
        #define F15_CZ_PSTATE_SERVICE_SUPPORT {AMD_FAMILY_15_CZ, &F15CzPstateServices},
      #endif

      #if OPTION_FAMILY15H_ST == TRUE
        extern CONST PSTATE_CPU_FAMILY_SERVICES ROMDATA F15StPstateServices;
        #undef F15_ST_PSTATE_SERVICE_SUPPORT
        #define F15_ST_PSTATE_SERVICE_SUPPORT {AMD_FAMILY_15_ST, &F15StPstateServices},
      #endif

    #endif
  #endif
#endif

#if ((AGESA_ENTRY_INIT_LATE == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE))

  #ifndef OPTION_ACPI_PSTATES
    #error  BLDOPT: Option not defined: "OPTION_ACPI_PSTATES"
  #endif

  #if (OPTION_ACPI_PSTATES == TRUE)

    //
    //Define ACPI Pstate objects.
    //

    OPTION_SSDT_FEATURE               GenerateSsdt;
    #undef USER_SSDT_MAIN
    #define USER_SSDT_MAIN            GenerateSsdt

    OPTION_ACPI_FEATURE               CreatePStateAcpiTables;
    #undef USER_PSTATE_OPTION_MAIN
    #define USER_PSTATE_OPTION_MAIN   CreatePStateAcpiTables

    #if CFG_ACPI_PSTATES_PPC == TRUE
      #undef USER_PSTATE_CFG_PPC
      #define USER_PSTATE_CFG_PPC   TRUE
    #endif
    #if CFG_ACPI_PSTATES_PCT == TRUE
      #undef USER_PSTATE_CFG_PCT
      #define USER_PSTATE_CFG_PCT   TRUE
    #endif
    #if CFG_ACPI_PSTATES_PSD == TRUE
      #undef USER_PSTATE_CFG_PSD
      #define USER_PSTATE_CFG_PSD   TRUE
    #endif
    #if CFG_ACPI_PSTATES_PSS == TRUE
      #undef USER_PSTATE_CFG_PSS
      #define USER_PSTATE_CFG_PSS   TRUE
    #endif
    #if CFG_ACPI_PSTATES_XPSS == TRUE
      #undef USER_PSTATE_CFG_XPSS
      #define USER_PSTATE_CFG_XPSS   TRUE
    #endif
  #endif

  // If ACPI Objects are disabled for PStates, we still need to check
  // whether ACPI Objects are enabled for CStates
  #if OPTION_IO_CSTATE == TRUE
    OPTION_SSDT_FEATURE               GenerateSsdt;
    OPTION_ACPI_FEATURE               CreateCStateAcpiTables;
    #undef  USER_SSDT_MAIN
    #define USER_SSDT_MAIN            GenerateSsdt
    #undef  USER_CSTATE_OPTION_MAIN
    #define USER_CSTATE_OPTION_MAIN   CreateCStateAcpiTables
  #endif
#endif


OPTION_PSTATE_LATE_CONFIGURATION    OptionPstateLateConfiguration = {
  PSTATE_STRUCT_VERSION,
  USER_SSDT_MAIN,
  USER_PSTATE_OPTION_MAIN,
  USER_CSTATE_OPTION_MAIN,
  USER_PSTATE_CFG_PPC,
  USER_PSTATE_CFG_PCT,
  USER_PSTATE_CFG_PSD,
  USER_PSTATE_CFG_PSS,
  USER_PSTATE_CFG_XPSS,
  CFG_ACPI_SET_OEM_ID,
  CFG_ACPI_SET_OEM_TABLE_ID
};

CONST CPU_SPECIFIC_SERVICES_XLAT ROMDATA PstateCpuFamilyServiceArray[] =
{
  F15_ST_PSTATE_SERVICE_SUPPORT
  F15_CZ_PSTATE_SERVICE_SUPPORT
  {0, NULL}
};
CONST CPU_FAMILY_SUPPORT_TABLE ROMDATA PstateFamilyServiceTable =
{
  (sizeof (PstateCpuFamilyServiceArray) / sizeof (CPU_SPECIFIC_SERVICES_XLAT)),
  &PstateCpuFamilyServiceArray[0]
};
#endif  // _OPTION_PSTATE_INSTALL_H_
