/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: CPU Core Leveling
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

#ifndef _OPTION_CPU_CORELEVELING_INSTALL_H_
#define _OPTION_CPU_CORELEVELING_INSTALL_H_


/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */
#define OPTION_CPU_CORE_LEVELING_FEAT
#define F15_CZ_CPU_CORELEVELING_SUPPORT
#define F15_ST_CPU_CORELEVELING_SUPPORT

#if OPTION_CPU_CORELEVELING == TRUE
  #if (AGESA_ENTRY_INIT_EARLY == TRUE)
    // Family 15h
    #if OPTION_FAMILY15H == TRUE
      extern CONST CPU_FEATURE_DESCRIPTOR ROMDATA CpuFeatureCoreLeveling;
      #undef OPTION_CPU_CORE_LEVELING_FEAT
      #define OPTION_CPU_CORE_LEVELING_FEAT &CpuFeatureCoreLeveling,

      #if (OPTION_FAMILY15H_CZ == TRUE)
        extern CONST CPU_CORE_LEVELING_FAMILY_SERVICES ROMDATA F15CzCoreLeveling;
        #undef F15_CZ_CPU_CORELEVELING_SUPPORT
        #define F15_CZ_CPU_CORELEVELING_SUPPORT {AMD_FAMILY_15_CZ, &F15CzCoreLeveling},
      #endif

      #if (OPTION_FAMILY15H_ST == TRUE)
        extern CONST CPU_CORE_LEVELING_FAMILY_SERVICES ROMDATA F15StCoreLeveling;
        #undef F15_ST_CPU_CORELEVELING_SUPPORT
        #define F15_ST_CPU_CORELEVELING_SUPPORT {AMD_FAMILY_15_ST, &F15StCoreLeveling},
      #endif

    #endif
  #endif
#endif

CONST CPU_SPECIFIC_SERVICES_XLAT ROMDATA CoreLevelingFamilyServiceArray[] =
{
  F15_ST_CPU_CORELEVELING_SUPPORT
  F15_CZ_CPU_CORELEVELING_SUPPORT
  {0, NULL}
};
CONST CPU_FAMILY_SUPPORT_TABLE ROMDATA CoreLevelingFamilyServiceTable =
{
  (sizeof (CoreLevelingFamilyServiceArray) / sizeof (CPU_SPECIFIC_SERVICES_XLAT)),
  &CoreLevelingFamilyServiceArray[0]
};

#endif  // _OPTION_CPU_CORELEVELING_INSTALL_H_
