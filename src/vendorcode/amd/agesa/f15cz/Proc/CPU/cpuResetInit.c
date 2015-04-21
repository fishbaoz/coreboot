/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Reset API, and related functions.
 *
 * Contains code that initialized the CPU after early reset.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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
#include "Ids.h"
#include "cpuCacheInit.h"
#include "cpuRegisters.h"
#include "cpuServices.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuResetInit.h"
#include "PlatformMemoryConfiguration.h"
#include "mm.h"
#include "mn.h"
#include "mnreg.h"
#include "mnpmu.h"

#include "APOB.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_CPU_CPURESETINIT_FILECODE


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
SetSystemDescriptors (
  IN       AGESA_PSP_OUTPUT_BLOCK_STRUCT  *PspOutputBlock,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

AGESA_STATUS
SetVariableMTRRs (
  IN       UINT32            Base,
  IN OUT   UINT32            *LimitPtr,
  IN       UINT32            MtrrAddr,
  IN       UINT8             MtrrType,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
GetMemoryDataFromPsp (
  IN OUT   AGESA_PSP_OUTPUT_BLOCK_STRUCT  *PspOutputBlock,
  IN       AMD_CONFIG_PARAMS      *StdHeader
   );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
/*---------------------------------------------------------------------------------------*/

/**
 * Performs CPU related initialization at the reset entry point
 *
 * This function performs a large list of initialization items.  These items
 * include:
 *
 *    -1      Retrieve APOB structure from PSP
 *    -2      Implement TOM1/2 and MTRR settings based on info
 *            from APOB structure.
 *
 * This routine will be run by the BSC only.
 *
 * @param[in]  ResetParams        Pointer to reset params
 *
 * @retval     AGESA_SUCCESS
 *
 */
AGESA_STATUS
AmdCpuReset (
  IN OUT   AMD_RESET_PARAMS *ResetParams
  )
{
  AGESA_STATUS  Status;
  AGESA_PSP_OUTPUT_BLOCK_STRUCT  PspOutputBlock;

  Status = AGESA_SUCCESS;

  Status = GetMemoryDataFromPsp (&PspOutputBlock, &ResetParams->StdHeader);

  if (Status == AGESA_UNSUPPORTED) {
    // Setup ROM execution cache for non memory present platforms.
    Status = AllocateExecutionCache (&ResetParams->StdHeader, &ResetParams->CacheRegion[0]);
    return AGESA_SUCCESS;
  }
  if (Status == AGESA_FATAL) {
    return (Status);
  }

  Status = SetSystemDescriptors (&PspOutputBlock, &ResetParams->StdHeader);

  return (Status);
}

/*---------------------------------------------------------------------------------------
 *                           L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 * This function runs on the BSP only, it gets the memorty descriptor block from the
 * PSP.
 *
 * @param[in,out]  *PspOutputBlock   Pointer to the psp output block
 * @param[in]      StdHeader         Config handle for library and services
 *
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GetMemoryDataFromPsp (
  IN OUT   AGESA_PSP_OUTPUT_BLOCK_STRUCT  *PspOutputBlock,
  IN       AMD_CONFIG_PARAMS      *StdHeader
   )
{
    return AGESA_UNSUPPORTED;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *
 * This function runs on the BSP only, it sets the fixed MTRRs for common legacy ranges.
 * It sets TOP_MEM and TOM2 and some variable MTRRs.
 *
 * @param[in]  *PspOutputBlock   Pointer to the psp output block
 * @param[in]  StdHeader         Config handle for library and services
 *
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
SetSystemDescriptors (
  IN       AGESA_PSP_OUTPUT_BLOCK_STRUCT  *PspOutputBlock,
  IN       AMD_CONFIG_PARAMS              *StdHeader
  )
{
  UINT32 Bottom32bIO;
  UINT32 Bottom40bIO;
  UINT32 Cache32bTOP;
  UINT64 LocalMsrRegister;
  AGESA_STATUS  Status;

  //
  //======================================================================
  // Set top of memory from PSP data block.
  // Adjust top of memory down to accommodate 32-bit IO space.
  //======================================================================
  //Bottom40bIO=top of memory, right justified 16 bits  (defines dram versus IO space type)
  //Bottom32bIO=sub 4GB top of memory, right justified 16 bits (defines dram versus IO space type)
  //Cache32bTOP=sub 4GB top of WB cacheable memory, right justified 16 bits
  //
  if (PspOutputBlock->HoleBase != 0) {
    Bottom32bIO = PspOutputBlock->HoleBase;
  } else if (PspOutputBlock->BottomIo != 0) {
    Bottom32bIO = (UINT32)PspOutputBlock->BottomIo << (24 - 16);
  } else {
    Bottom32bIO = (UINT32)1 << (24 - 16);
  }

  Cache32bTOP = PspOutputBlock->SysLimit + 1;
  if (Cache32bTOP < _4GB_RJ16) {
    Bottom40bIO = 0;
    if (Bottom32bIO >= Cache32bTOP) {
      Bottom32bIO = Cache32bTOP;
    }
  } else {
    Bottom40bIO = Cache32bTOP;
  }

  Cache32bTOP = Bottom32bIO;


  //
  //======================================================================
  // Set default values for CPU registers
  //======================================================================
  //
  LibAmdMsrRead (MSR_SYS_CFG, &LocalMsrRegister, StdHeader);
  (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrFixDramEn) = 1;
  (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrFixDramModEn) = 1;
  (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrVarDramEn) = 1;
  LibAmdMsrWrite (MSR_SYS_CFG, &LocalMsrRegister, StdHeader);

  LocalMsrRegister = AMD_MTRR_FIX64K_WB_DRAM;
  LibAmdMsrWrite (AMD_MTRR_FIX64k_00000, &LocalMsrRegister, StdHeader);      // 0 - 512K = WB Mem
  LocalMsrRegister = AMD_MTRR_FIX16K_WB_DRAM;
  LibAmdMsrWrite (AMD_MTRR_FIX16k_80000, &LocalMsrRegister, StdHeader);      // 512K - 640K = WB Mem

  //
  //======================================================================
  // Set variable MTRR values
  //======================================================================
  //
  Status = SetVariableMTRRs (0, &Cache32bTOP, 0x200, 6, StdHeader);

/// @todo is this necessary ?  PspOutputBlock->Sub4GCacheTop = Cache32bTOP << 16;

  //
  //======================================================================
  // Set TOP_MEM and TOM2 CPU registers
  //======================================================================
  //
  LocalMsrRegister = Bottom32bIO << 16;
  LibAmdMsrWrite (MSR_TOM, &LocalMsrRegister, StdHeader);

  if (Bottom40bIO) {
    LocalMsrRegister = Bottom40bIO << 16;
  } else {
    LocalMsrRegister = 0;
  }
  LibAmdMsrWrite (MSR_TOM2, &LocalMsrRegister, StdHeader);

  LibAmdMsrRead (MSR_SYS_CFG, &LocalMsrRegister, StdHeader);
  if (Bottom40bIO) {
    (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrTom2En) = 1;
    (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrTom2ForceMemTypeWB) = 1;
  } else {
    // Disable TOM2
    (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrTom2En) = 0;
    (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrTom2ForceMemTypeWB) = 0;
  }
  (((SYS_CFG_MSR *) &LocalMsrRegister)->MtrrFixDramModEn) = 0;      // turn off modification enable bit
  LibAmdMsrWrite (MSR_SYS_CFG, &LocalMsrRegister, StdHeader);

  return Status;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 * Program MTRRs to describe given range as given cache type.  Use MTRR pairs
 * starting with the given MTRRphys Base address, and use as many as is
 * required up to (excluding) MSR 020C, which is reserved for OS.
 *
 * "Limit" in the context of this procedure is not the numerically correct
 * limit, but rather the Last address+1, for purposes of coding efficiency
 * and readability.  Size of a region is then Limit-Base.
 *
 * 1. Size of each range must be a power of two
 * 2. Each range must be naturally aligned (Base is same as size)
 *
 * There are two code paths: the ascending path and descending path (analogous
 * to bsf and bsr), where the next limit is a function of the next set bit in
 * a forward or backward sequence of bits (as a function of the Limit).  We
 * start with the ascending path, to ensure that regions are naturally aligned,
 * then we switch to the descending path to maximize MTRR usage efficiency.
 * Base=0 is a special case where we start with the descending path.
 * Correct Mask for region is 2comp(Size-1)-1,
 * which is 2comp(Limit-Base-1)-1 *
 * @param[in]     Base - Base address[47:16] of specified range.
 * @param[in]     *LimitPtr - Limit address[47:16] of specified range.
 * @param[in]     MtrrAddr - address of var MTRR pair to start using.
 * @param[in]     MtrrType - Cache type for the range.
 * @param[in]  StdHeader         Config handle for library and services
 *
 *
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
SetVariableMTRRs (
  IN       UINT32            Base,
  IN OUT   UINT32            *LimitPtr,
  IN       UINT32            MtrrAddr,
  IN       UINT8             MtrrType,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64 LocalMsrRegister;
  UINT32 CurBase;
  UINT32 CurLimit;
  UINT32 CurSize;
  UINT32 CurAddr;
  UINT32 Value32;

  CurBase = Base;
  CurLimit = *LimitPtr;
  CurAddr = MtrrAddr;

  while ((CurAddr >= AMD_MTRR_VARIABLE_BASE0) && (CurAddr < AMD_MTRR_VARIABLE_BASE6) && (CurBase < *LimitPtr)) {
    CurSize = CurLimit = (UINT32)1 << LibAmdBitScanForward (CurBase);
    CurLimit += CurBase;
    if ((CurBase == 0) || (*LimitPtr < CurLimit)) {
      CurLimit = *LimitPtr - CurBase;
      CurSize = CurLimit = (UINT32)1 << LibAmdBitScanReverse (CurLimit);
      CurLimit += CurBase;
    }

    // prog. MTRR with current region Base
    LocalMsrRegister = (CurBase << 16) | (UINT32)MtrrType;
    LibAmdMsrWrite (CurAddr, &LocalMsrRegister, StdHeader);

    // prog. MTRR with current region Mask
    CurAddr++;                              // other half of MSR pair
    Value32 = CurSize - (UINT32)1;
    Value32 = ~Value32;
    LocalMsrRegister = (Value32 << 16) | ((UINT32)1 << MTRR_VALID);
    LibAmdMsrWrite (CurAddr, &LocalMsrRegister, StdHeader);

    CurBase = CurLimit;
    CurAddr++;                              // next MSR pair
  }

  if (CurLimit < *LimitPtr) {
    // Announce failure
    *LimitPtr = CurLimit;
    IDS_ERROR_TRAP;
  }

  LocalMsrRegister = 0;
  while ((CurAddr >= AMD_MTRR_VARIABLE_BASE0) && (CurAddr < AMD_MTRR_VARIABLE_BASE6)) {
    LibAmdMsrWrite (CurAddr, &LocalMsrRegister, StdHeader);
    CurAddr++;
  }

  return TRUE;
}
