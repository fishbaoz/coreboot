/* $NoKeywords:$ */
/**
 * @file
 *
 * Load microcode patch
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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

#ifndef _CPU_MICROCODE_PATCH_H_
#define _CPU_MICROCODE_PATCH_H_

/*---------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *---------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *---------------------------------------------------------------------------------------
 */
#define MICROCODE_PATCH_2K_SIZE    2048
#define MICROCODE_PATCH_4K_SIZE    4096

/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */

/// Microcode patch field definitions
typedef struct {
  UINT32   DateCode;                   ///< Date of patch creation
  UINT32   PatchID;                    ///< Patch level
  UINT16   MicrocodePatchDataID;       ///< Internal use only
  UINT8    MicrocodePatchDataLength;   ///< Internal use only
  UINT8    InitializationFlag;         ///< Internal use only
  UINT32   MicrocodePatchDataChecksum; ///< Doubleword sum of data block
  UINT32   Chipset1DeviceID;           ///< Device ID of 1st Chipset device to match
  UINT32   Chipset2DeviceID;           ///< Device ID of 2nd Chipset device to match
  UINT16   ProcessorRevisionID;        ///< Equivalent ID
  UINT8    Chipset1RevisionID;         ///< Revision level of 1st Chipset device to match
  UINT8    Chipset2RevisionID;         ///< Revision level of 2nd Chipset device to match
  UINT8    BiosApiRevision;            ///< BIOS INT 15 API revision required
  UINT8    Reserved1[3];               ///< Reserved
  UINT32   MatchRegister0;             ///< Internal use only
  UINT32   MatchRegister1;             ///< Internal use only
  UINT32   MatchRegister2;             ///< Internal use only
  UINT32   MatchRegister3;             ///< Internal use only
  UINT32   MatchRegister4;             ///< Internal use only
  UINT32   MatchRegister5;             ///< Internal use only
  UINT32   MatchRegister6;             ///< Internal use only
  UINT32   MatchRegister7;             ///< Internal use only
  UINT8    PatchDataBlock[896];        ///< Raw patch data
  UINT8    Reserved2[896];             ///< Reserved
  UINT8    X86CodePresent;             ///< Boolean to determine if executable code exists
  UINT8    X86CodeEntry[191];          ///< Code to execute if X86CodePresent != 0
} MICROCODE_PATCH;

/// Two kilobyte array containing the raw
/// microcode patch binary data
typedef struct {
  IN       UINT8   MicrocodePatches[MICROCODE_PATCH_2K_SIZE];  ///< 2k UINT8 elements
} MICROCODE_PATCHES;

/// Four kilobyte array containing the raw
/// microcode patch binary data
typedef struct {
  IN       UINT8   MicrocodePatches[MICROCODE_PATCH_4K_SIZE];  ///< 4k UINT8 elements
} MICROCODE_PATCHES_4K;

/*---------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *---------------------------------------------------------------------------------------
 */
BOOLEAN
LoadMicrocodePatch (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
LoadMicrocodePatchInCurrentSocket (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

#endif  // _CPU_MICROCODE_PATCH_H_

