/* $NoKeywords:$ */
/**
 * @file
 *
 * mnpmu.h
 *
 * Definitions for PMU support
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem)
 * @e \$Revision: 311790 $ @e \$Date: 2015-01-27 13:03:49 +0800 (Tue, 27 Jan 2015) $
 *
 **/
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
  * ***************************************************************************
  *
 */

#ifndef _MNPMU_H_
#define _MNPMU_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */

// Maximum PMU SRAM size for DMEM/IMEM
#define PMU_SRAM_DMEM_SIZE_MAX      0x2000  ///< PMU DMEM size is 6K Bytes
#define PMU_SRAM_IMEM_SIZE_MAX      0x4000  ///< PMU IMEM size is 14K Bytes

#define PMU_IMAGE_NOT_USEABLE    0x0000  ///< The Image is not valid or useable

// Upstream mailbox 1 messages
#define USMSG_DEVINIT      0x00  ///< PMU has completed DevInit
#define USMSG_TRAINWRLVL   0x01  ///< PMU has completed TSTAGE_WrLvl
#define USMSG_TRAINRXEN    0x02  ///< PMU has completed TSTAGE_RxEn
#define USMSG_TRAINRDDQS1D 0x03  ///< PMU has completed TSTAGE_TrainRdDqs1D
#define USMSG_TRAINWRDQ1D  0x04  ///< PMU has completed TSTAGE_TrainWrDq1D
#define USMSG_TRAINRD2D    0x05  ///< PMU has completed TSTAGE_TrainRd2D
#define USMSG_TRAINWR2D    0x06  ///< PMU has completed TSTAGE_TrainWr2D
#define USMSG_PMUQEMPTY    0x07  ///< PMU has completed all of its SequenceCtl tasks and is in a powergated idle state
#define USMSG_US2MSGRDY    0x08  ///< PMU is ready to stream a message through US mailbox 2
#define USMSG_FAIL         0xFF  ///< PMU has encountered an error which requires requester to abort waiting for remaining pending upstream messages

#define DCT_OFFSET_AUTO_INC_EN  ((UINT32) 1 << 29)
#define PMU_FIRMWARE_SRAM_START 0x00050000ul
#define PMU_SRAM_DMEM_OFFSET    0x0000  ///< PMU SRAM DMEM offset
#define PMU_SRAM_IMEM_OFFSET    0x4000  ///< PMU SRAM IMEM offset

// AMD PMU Signature.
#define PMU_SIGNATURE           {'!', '!', '!', 'P', 'M', 'U'}   ///< PMU Signature with 6 characters limited.

/// AMD PMU Header
typedef struct {
  CHAR8   Signature[6];                       ///< PMU signature
  CHAR8   Instance;                           ///< PMU instance
  CHAR8   PackageName[16];                    ///< Package string
  CHAR8   TerminatorNull;                     ///< null terminated string
  CHAR8   VerReserved[7];                     ///< reserved space
} AMD_PMU_HEADER;

// PMU firmware Code/Data can not be fragmented.  The offset for Data must
// start at 0x0000 and Code must start at 0x4000.
/// The Option of PMU firmware
typedef struct {
  AMD_PMU_HEADER VersionString;                                 ///< PMU embedded signature
  UINT32  SramSize;                                             ///< Total size of PMU SRAM, included DMEM/IMEM.
  UINT16  SramDataImage[PMU_SRAM_DMEM_SIZE_MAX / sizeof (UINT16)];  ///< PMU SRAM Data Image
  UINT16  SramCodeImage[PMU_SRAM_IMEM_SIZE_MAX / sizeof (UINT16)];  ///< PMU SRAM Code Image
} PMU_FIRMWARE;

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */

/**
  PMU_SRAM (TableName, BfIndex, RegOffset, Hi, Lo)

  @param[in]    TableName
  @param[in]    BfIndex
  @param[in]    RegOffset
  @param[in]    Hi
  @param[in]    Lo

  @return   PMU_SRAM  Encrypted PMU SRAM field location
--*/
#define PMU_SRAM(TableName, BfIndex, RegOffset, Hi, Lo) \
  TableName[BfIndex] = ( \
    ((UINT32) RegOffset << 12) | ( \
      (Hi > Lo) ? (\
        (((UINT32) Hi << 6) | (UINT32) Lo) \
      ) : ( \
        (((UINT32) Lo << 6) | (UINT32) Hi) \
      ) \
    ) \
  )

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */
VOID
MemNPmuResetNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNStartPmuNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNPendOnPmuCompletionNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNCalcD3MR0 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNCalcD3MR1 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  );

UINT32
MemNCalcD3MR2 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  );

UINT32
MemNCalcD4MR0 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNCalcD4MR1 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  );

UINT32
MemNCalcD4MR2 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  );

UINT32
MemNCalcD4MR3 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNCalcD4MR4 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNCalcD4MR5 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  );

UINT32
MemNCalcD4MR6 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

#endif  /* _MNPMU_H_ */

