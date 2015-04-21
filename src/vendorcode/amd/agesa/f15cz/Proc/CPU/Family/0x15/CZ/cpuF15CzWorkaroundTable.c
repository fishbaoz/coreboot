/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Carrizo Workaround table
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/CZ
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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
#include "Table.h"
#include "cpuFamilyTranslation.h"
#include "cpuPstateTables.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15CzPowerMgmt.h"
#include "cpuF15CzWorkaroundTable.h"
#include "Table.h"

#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CZ_CPUF15CZWORKAROUNDTABLE_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
F_FAM_SPECIFIC_WORKAROUND **
F15CzGetWorkaroundTable  (
  IN       CPU_SPECIFIC_SERVICES                *FamilyServices,
     OUT   UINT16                               *NumberOfWorkaroundTableEntries,
  IN       AMD_CONFIG_PARAMS                    *StdHeader
  );

VOID
F15CzNbMcaLock (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
F15CzLwpCoreId (
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );


/*---------------------------------------------------------------------------------------*/
/**
 *  MSR special programming requirements for MSR_0000_0413
 *
 *  AGESA should program MSR_0000_0413[61] dependant on the value read from MSR_0000_0413[50:49]
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
F15CzNbMcaLock (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  MC4_MISC0_MSR Mc4Misc0;
  MC4_MISC1_MSR Mc4Misc1;

  LibAmdMsrRead (MSR_MC4_MISC0, (UINT64 *) &Mc4Misc0, StdHeader);

  if (Mc4Misc0.IntType == 0x2) {
    Mc4Misc0.Locked = 1;
  } else {
    Mc4Misc0.Locked = 0;
  }
  LibAmdMsrWrite (MSR_MC4_MISC0, (UINT64 *) &Mc4Misc0, StdHeader);


  LibAmdMsrRead (MSR_MC4_MISC1, (UINT64 *) &Mc4Misc1, StdHeader);

  if (Mc4Misc1.IntType == 0x2) {
    Mc4Misc1.Locked = 1;
  } else {
    Mc4Misc1.Locked = 0;
  }
  LibAmdMsrWrite (MSR_MC4_MISC1, (UINT64 *) &Mc4Misc1, StdHeader);

  return;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Update the MSR_C000_0105[39:32] in current processor for Family15h CZ.
 *
 * This function program  MSR_C000_0105[39:32].
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
F15CzLwpCoreId (
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LWP_CFG_MSR LwpCfg;
  CPUID_DATA  LocalApicId;

  LibAmdCpuidRead (AMD_CPUID_APICID_LPC_BID, &LocalApicId, StdHeader);
  LibAmdMsrRead (0xC0000105, (UINT64 *) &LwpCfg, StdHeader);

  // LwpCfg[39:32] = CPUID Fn0000_0001_EBX[LocalApicId]
  LwpCfg.LwpCoreId = (LocalApicId.EBX_Reg >> 24) & 0xFF;

  LibAmdMsrWrite (0xC0000105, (UINT64 *) &LwpCfg, StdHeader);
}

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */



F_FAM_SPECIFIC_WORKAROUND ROMDATA *F15CzWorkaroundTable[] = {
  F15CzNbMcaLock,
  F15CzLwpCoreId,
  NULL
};

F_FAM_SPECIFIC_WORKAROUND **
F15CzGetWorkaroundTable  (
  IN       CPU_SPECIFIC_SERVICES                *FamilyServices,
     OUT   UINT16                               *NumberOfWorkaroundTableEntries,
  IN       AMD_CONFIG_PARAMS                    *StdHeader
  )
{
  *NumberOfWorkaroundTableEntries = NUMBER_OF_TABLE_ENTRIES (F15CzWorkaroundTable);
  return (&F15CzWorkaroundTable[0]);
}
