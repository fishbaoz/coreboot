/* $NoKeywords:$ */
/**
 * @file
 *
 * GFx tables
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 309574 $   @e \$Date: 2014-12-17 02:58:34 -0600 (Wed, 17 Dec 2014) $
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
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbRegistersCZ.h"
#include  "cpuFamilyTranslation.h"

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


/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */


GNB_TABLE ROMDATA GfxGmcInitTableCZ [] = {
  //Enable Garlic interface on UNB Through F5x178[19] = 0
  GNB_ENTRY_RMW (D18F5x178_TYPE, D18F5x178_ADDRESS, D18F5x178_SwGfxDis_MASK, 0 << D18F5x178_SwGfxDis_OFFSET),
  //2.1 System memory address translation
  GNB_ENTRY_COPY (GMMx2814_TYPE, GMMx2814_ADDRESS,  0, 32, D18F2x40_dct0_TYPE, D18F2x40_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2818_TYPE, GMMx2818_ADDRESS,  0, 32, D18F2x40_dct1_TYPE, D18F2x40_dct1_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx281C_TYPE, GMMx281C_ADDRESS,  0, 32, D18F2x44_dct0_TYPE, D18F2x44_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2820_TYPE, GMMx2820_ADDRESS,  0, 32, D18F2x44_dct1_TYPE, D18F2x44_dct1_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2824_TYPE, GMMx2824_ADDRESS,  0, 32, D18F2x48_dct0_TYPE, D18F2x48_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2828_TYPE, GMMx2828_ADDRESS,  0, 32, D18F2x48_dct1_TYPE, D18F2x48_dct1_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx282C_TYPE, GMMx282C_ADDRESS,  0, 32, D18F2x4C_dct0_TYPE, D18F2x4C_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2830_TYPE, GMMx2830_ADDRESS,  0, 32, D18F2x4C_dct1_TYPE, D18F2x4C_dct1_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2834_TYPE, GMMx2834_ADDRESS,  0, 32, D18F2x60_dct0_TYPE, D18F2x60_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx283C_TYPE, GMMx283C_ADDRESS,  0, 32, D18F2x64_dct0_TYPE, D18F2x64_dct0_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2838_TYPE, GMMx2838_ADDRESS,  0, 32, D18F2x60_dct1_TYPE, D18F2x60_dct1_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx2840_TYPE, GMMx2840_ADDRESS,  0, 32, D18F2x64_dct1_TYPE, D18F2x64_dct1_ADDRESS,  0, 32),
  // MC_FUS_DRAM0_BANK_ADDR_MAPPING
  GNB_ENTRY_COPY (GMMx2844_TYPE, GMMx2844_ADDRESS,  0,  8, D18F2x80_dct0_TYPE, D18F2x80_dct0_ADDRESS,  0,  8),
  GNB_ENTRY_COPY (GMMx2844_TYPE, GMMx2844_ADDRESS,  8,  1, D18F2x94_dct0_TYPE, D18F2x94_dct0_ADDRESS, 22,  1),
  GNB_ENTRY_COPY (GMMx2844_TYPE, GMMx2844_ADDRESS,  9,  1, D18F2xA8_dct0_TYPE, D18F2xA8_dct0_ADDRESS, 20,  1),
  GNB_ENTRY_COPY (GMMx2848_TYPE, GMMx2848_ADDRESS,  0,  8, D18F2x80_dct1_TYPE, D18F2x80_dct1_ADDRESS,  0,  8),
  GNB_ENTRY_COPY (GMMx2848_TYPE, GMMx2848_ADDRESS,  8,  1, D18F2x94_dct1_TYPE, D18F2x94_dct1_ADDRESS, 22,  1),
  GNB_ENTRY_COPY (GMMx2848_TYPE, GMMx2848_ADDRESS,  9,  1, D18F2xA8_dct1_TYPE, D18F2xA8_dct1_ADDRESS, 20,  1),
  // MC_FUS_DRAM0_CTL_BASE
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS,  0,  3, D18F1x200_TYPE, D18F1x200_ADDRESS,  4,  3),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS,  3,  4, D18F1x204_TYPE, D18F1x204_ADDRESS,  0,  4),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS,  7, 21, D18F1x200_TYPE, D18F1x200_ADDRESS, 11, 21),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS, 28,  1, D18F1x200_TYPE, D18F1x200_ADDRESS,  3,  1),
  GNB_ENTRY_COPY (GMMx284C_TYPE, GMMx284C_ADDRESS, 29,  1, D18F1x200_TYPE, D18F1x200_ADDRESS,  0,  1),
  // MC_FUS_DRAM1_CTL_BASE
  GNB_ENTRY_COPY (GMMx2850_TYPE, GMMx2850_ADDRESS,  0,  3, D18F1x208_TYPE, D18F1x208_ADDRESS,  4,  3),
  GNB_ENTRY_COPY (GMMx2850_TYPE, GMMx2850_ADDRESS,  3,  4, D18F1x20C_TYPE, D18F1x20C_ADDRESS,  0,  4),
  GNB_ENTRY_COPY (GMMx2850_TYPE, GMMx2850_ADDRESS,  7, 21, D18F1x208_TYPE, D18F1x208_ADDRESS, 11, 21),
  GNB_ENTRY_COPY (GMMx2850_TYPE, GMMx2850_ADDRESS, 28,  1, D18F1x208_TYPE, D18F1x208_ADDRESS,  3,  1),
  GNB_ENTRY_COPY (GMMx2850_TYPE, GMMx2850_ADDRESS, 29,  1, D18F1x208_TYPE, D18F1x208_ADDRESS,  0,  1),
  // MC_FUS_DRAM0_CTL_LIMIT
  GNB_ENTRY_COPY (GMMx2854_TYPE, GMMx2854_ADDRESS,  0, 21, D18F1x204_TYPE, D18F1x204_ADDRESS, 11, 21),
  GNB_ENTRY_COPY (GMMx2854_TYPE, GMMx2854_ADDRESS, 21,  1, D18F1x200_TYPE, D18F1x200_ADDRESS,  1,  1),
  // MC_FUS_DRAM1_CTL_LIMIT
  GNB_ENTRY_COPY (GMMx2858_TYPE, GMMx2858_ADDRESS,  0, 21, D18F1x20C_TYPE, D18F1x20C_ADDRESS, 11, 21),
  GNB_ENTRY_COPY (GMMx2858_TYPE, GMMx2858_ADDRESS, 21,  1, D18F1x208_TYPE, D18F1x208_ADDRESS,  1,  1),
  // MC_FUS_DRAM_MODE
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  3,  3, D18F2x78_dct0_TYPE, D18F2x78_dct0_ADDRESS,  8,  3),
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  6,  9, D18F1xF0_TYPE,      D18F1xF0_ADDRESS,       7,  9),
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  0,  2, D18F2x110_TYPE,     D18F2x110_ADDRESS,      6,  2),
  GNB_ENTRY_COPY (GMMx2864_TYPE, GMMx2864_ADDRESS,  2,  1, D18F2x114_TYPE,     D18F2x114_ADDRESS,      9,  1),
  // MC_FUS_DRAM_CTL_HIGH_01
  GNB_ENTRY_COPY (GMMx285C_TYPE, GMMx285C_ADDRESS,  0, 12, D18F1x240_TYPE, D18F1x240_ADDRESS, 11, 12),
  GNB_ENTRY_COPY (GMMx285C_TYPE, GMMx285C_ADDRESS, 12, 12, D18F1x244_TYPE, D18F1x244_ADDRESS, 11, 12),

  // 2.2 GMC DRAM/MMIO Aperture check registers Init
  GNB_ENTRY_COPY (GMMx3E650_TYPE, GMMx3E650_ADDRESS, 23,  9, D0F0x90_TYPE, D0F0x90_ADDRESS, 23, 9),
  GNB_ENTRY_COPY (GMMx3E654_TYPE, GMMx3E654_ADDRESS,  0,  1, D0F0x64_x19_TYPE, D0F0x64_x19_ADDRESS,  0,  1),
  GNB_ENTRY_COPY (GMMx3E654_TYPE, GMMx3E654_ADDRESS, 23,  9, D0F0x64_x19_TYPE, D0F0x64_x19_ADDRESS, 23,  9),
  GNB_ENTRY_COPY (GMMx3E658_TYPE, GMMx3E658_ADDRESS,  0,  8, D0F0x64_x1A_TYPE, D0F0x64_x1A_ADDRESS,  0,  8),
  GNB_ENTRY_COPY (GMMx3E65C_TYPE, GMMx3E65C_ADDRESS, 31,  1, D0F0x64_x4E_TYPE, D0F0x64_x4E_ADDRESS, 31,  1),
  GNB_ENTRY_COPY (GMMx3E65C_TYPE, GMMx3E65C_ADDRESS,  0, 30, D0F0x64_x4E_TYPE, D0F0x64_x4E_ADDRESS,  0, 30),
  GNB_ENTRY_COPY (GMMx3E640_TYPE, GMMx3E640_ADDRESS,  0, 32, D0F0x64_x17_TYPE, D0F0x64_x17_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx3E644_TYPE, GMMx3E644_ADDRESS,  0, 32, D0F0x64_x18_TYPE, D0F0x64_x18_ADDRESS,  0, 32),
  GNB_ENTRY_COPY (GMMx3E648_TYPE, GMMx3E648_ADDRESS, 23,  1, D0F0x4C_TYPE, D0F0x4C_ADDRESS, 23, 1),
  GNB_ENTRY_COPY (GMMx3E64C_TYPE, GMMx3E64C_ADDRESS,  3,  1, D0F0x84_TYPE, D0F0x84_ADDRESS, 3, 1),

  //---------------------------------------------------------------------------
  // 2.4 Power gating init
  // Initializing PGFSMs
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x100),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x200010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x300010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x210101),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xa00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x21003),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xb00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x2b00),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xc00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xd00010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x420000),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x100010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x120202),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x500010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x3e3e36),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x600010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x373f3e),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0x700010ff),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx353C_TYPE, GMMx353C_ADDRESS, 0x3e1332),
  GNB_ENTRY_WR (GMMx3538_TYPE, GMMx3538_ADDRESS, 0xe00010ff),
  // Initializing register engine
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10000800),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xf),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xf),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x4),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10000828),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3f),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x8),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1000051e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xffff),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xffff),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xc),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20500),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x10),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40504),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x16),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9050c),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x21),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9052c),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x2c),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10546),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x2f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xa054e),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x3b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1055f),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x3e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1056f),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x41),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10572),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x44),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40575),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x4a),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3507a0),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x81),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1e0801),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xa1),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20829),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xa5),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3082d),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xaa),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40833),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xb0),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x7083b),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xb9),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20844),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xbd),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x60848),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xc5),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xf0851),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0xd6),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x280862),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x100),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20891),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x104),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x8a1),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x106),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x8a5),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x108),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3308cd),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x13d),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10902),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x140),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x13090c),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x155),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x922),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x157),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x924),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x159),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x140926),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x16f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x2000095a),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x171),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x301f095b),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x192),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xc094d),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1a0),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1095c),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1a3),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1e096d),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1c3),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x98e),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1c5),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20992),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1c9),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20998),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1cd),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9a4),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1cf),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9a7),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x1d1),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3809be),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x20b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x309f9),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x210),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x9ff),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x212),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x30a01),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x217),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x60a13),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x21f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x30a1f),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x224),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10000ccd),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x3f),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x0),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x228),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x100f0ce7),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40000000),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x40000000),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x23b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x70cc0),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x244),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x20cc9),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x248),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10cd0),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x24b),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xf0cd4),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x25c),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xce6),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x25e),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x10d08),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x261),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0xccd),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x263),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1b0dc0),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x280),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x800),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x282),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x828),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x284),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x1f0de0),
  GNB_ENTRY_WR (GMMx3500_TYPE, GMMx3500_ADDRESS, 0x2a5),
  GNB_ENTRY_WR (GMMx3504_TYPE, GMMx3504_ADDRESS, 0x17f980),
  GNB_ENTRY_RMW (GMMx3508_TYPE, GMMx3508_ADDRESS, 0xfffff000, 0xaf6bd000),
  GNB_ENTRY_RMW (GMMx3544_TYPE, GMMx3544_ADDRESS, 0x20000000, 0x20000000),
  //---------------------------------------------------------------------------
  // 2.5 Performance tuning
  GNB_ENTRY_WR (GMMx276C_TYPE, GMMx276C_ADDRESS, 0x000001ff),    ///MC_ARB_SQM_CNTL
  GNB_ENTRY_WR (GMMx27D0_TYPE, GMMx27D0_ADDRESS, 0x10b08887),    ///MC_ARB_RET_CREDITS_RD
  GNB_ENTRY_WR (GMMx277C_TYPE, GMMx277C_ADDRESS, 0x00000807),    ///MC_ARB_WTM_CNTL_RD
  GNB_ENTRY_WR (GMMx2188_TYPE, GMMx2188_ADDRESS, 0x000021b1),    ///MC_HUB_RDREQ_DMIF
  GNB_ENTRY_WR (GMMx2204_TYPE, GMMx2204_ADDRESS, 0x002121b1),    ///MC_HUB_RDREQ_ACPG
  GNB_ENTRY_WR (GMMx2780_TYPE, GMMx2780_ADDRESS, 0x00000803),    ///MC_ARB_WTM_CNTL_WR
  GNB_ENTRY_WR (GMMx2218_TYPE, GMMx2218_ADDRESS, 0x002121b1),    ///MC_HUB_WDP_ACPG
  GNB_ENTRY_WR (GMMx2208_TYPE, GMMx2208_ADDRESS, 0x002021b1),    ///MC_HUB_RDREQ_ACPO
  GNB_ENTRY_WR (GMMx221C_TYPE, GMMx221C_ADDRESS, 0x002021b1),    ///MC_HUB_WDP_ACPO
  GNB_ENTRY_WR (GMMx2220_TYPE, GMMx2220_ADDRESS, 0x00000991),    ///MC_HUB_WDP_SAMMSP
  GNB_ENTRY_WR (GMMx201C_TYPE, GMMx201C_ADDRESS, 0x37773337),    ///MC_RD_GRP_OTH
  GNB_ENTRY_WR (GMMx2020_TYPE, GMMx2020_ADDRESS, 0x73773337),    ///MC_WR_GRP_OTH
  GNB_ENTRY_WR (GMMx2018_TYPE, GMMx2018_ADDRESS, 0x66333303),    ///MC_WR_GRP_SYS
  GNB_ENTRY_WR (GMMx2014_TYPE, GMMx2014_ADDRESS, 0x66300373),    ///MC_RD_GRP_SYS
  GNB_ENTRY_WR (GMMx2794_TYPE, GMMx2794_ADDRESS, 0xfcfcfdfc),    ///MC_ARB_LAZY0_RD
  GNB_ENTRY_WR (GMMx2798_TYPE, GMMx2798_ADDRESS, 0xfcfcfdfc),    ///MC_ARB_LAZY0_WR
  GNB_ENTRY_WR (GMMx27A4_TYPE, GMMx27A4_ADDRESS, 0x00ffffff),    ///MC_ARB_AGE_RD
  GNB_ENTRY_WR (GMMx27A8_TYPE, GMMx27A8_ADDRESS, 0x00ffffff),    ///MC_ARB_AGE_WR
  GNB_ENTRY_WR (GMMx2120_TYPE, GMMx2120_ADDRESS, 0x00000018),    ///MC_HUB_RDREQ_DMIF_LIMIT
  GNB_ENTRY_WR (GMMx278C_TYPE, GMMx278C_ADDRESS, 0x00000008),    ///MC_ARB_TM_CNTL_RD
  GNB_ENTRY_WR (GMMx2790_TYPE, GMMx2790_ADDRESS, 0x00000008),    ///MC_ARB_TM_CNTL_WR
  GNB_ENTRY_WR (GMMx2628_TYPE, GMMx2628_ADDRESS, 0x55111000),    ///MC_RD_GRP_LCL
  GNB_ENTRY_WR (GMMx25E0_TYPE, GMMx25E0_ADDRESS, 0x00000025),    ///MC_RD_GRP_EXT
  GNB_ENTRY_WR (GMMx262C_TYPE, GMMx262C_ADDRESS, 0x10555111),    ///MC_WR_GRP_LCL
  GNB_ENTRY_WR (GMMx25E4_TYPE, GMMx25E4_ADDRESS, 0x00000025),    ///MC_WR_GRP_EXT
  GNB_ENTRY_WR (GMMx25C8_TYPE, GMMx25C8_ADDRESS, 0x0080685f),    ///MC_CITF_CREDITS_ARB_RD
  GNB_ENTRY_WR (GMMx2624_TYPE, GMMx2624_ADDRESS, 0x00008080),    ///MC_CITF_CREDITS_XBAR
  GNB_ENTRY_WR (GMMx25CC_TYPE, GMMx25CC_ADDRESS, 0x0080807f),    ///MC_CITF_CREDITS_ARB_WR
  GNB_ENTRY_WR (GMMx20EC_TYPE, GMMx20EC_ADDRESS, 0x3a00001c),    ///MC_HUB_RDREQ_CNTL
  GNB_ENTRY_WR (GMMx3758_TYPE, GMMx3758_ADDRESS, 0x00000800),    ///MC_ARB_HARSH_CTL_RD
  GNB_ENTRY_WR (GMMx375C_TYPE, GMMx375C_ADDRESS, 0x00000800),    ///MC_ARB_HARSH_CTL_WR
  GNB_ENTRY_WR (GMMx2178_TYPE, GMMx2178_ADDRESS, 0x000289f1),    ///MC_HUB_RDREQ_VCE0
  GNB_ENTRY_WR (GMMx37F0_TYPE, GMMx37F0_ADDRESS, 0x000289f1),    ///MC_HUB_RDREQ_VCE1
  GNB_ENTRY_WR (GMMx21B8_TYPE, GMMx21B8_ADDRESS, 0x000289f1),    ///MC_HUB_WDP_VCE0
  GNB_ENTRY_WR (GMMx37F8_TYPE, GMMx37F8_ADDRESS, 0x000289f1),    ///MC_HUB_WDP_VCE1
  GNB_ENTRY_WR (GMMx2194_TYPE, GMMx2194_ADDRESS, 0x000189f1),    ///MC_HUB_RDREQ_VCEU0
  GNB_ENTRY_WR (GMMx37F4_TYPE, GMMx37F4_ADDRESS, 0x000189f1),    ///MC_HUB_RDREQ_VCEU1
  GNB_ENTRY_WR (GMMx21F4_TYPE, GMMx21F4_ADDRESS, 0x000189f1),    ///MC_HUB_WDP_VCEU0
  GNB_ENTRY_WR (GMMx37FC_TYPE, GMMx37FC_ADDRESS, 0x000189f1),    ///MC_HUB_WDP_VCEU1
  GNB_ENTRY_WR (GMMx2180_TYPE, GMMx2180_ADDRESS, 0x000289f1),    ///MC_HUB_RDREQ_UVD
  GNB_ENTRY_WR (GMMx21D8_TYPE, GMMx21D8_ADDRESS, 0x000289f1),    ///MC_HUB_WDP_UVD
  GNB_ENTRY_WR (GMMx3700_TYPE, GMMx3700_ADDRESS, 0xd1000000),    ///MC_ARB_HARSH_EN_RD
  GNB_ENTRY_WR (GMMx3704_TYPE, GMMx3704_ADDRESS, 0xd0000000),    ///MC_ARB_HARSH_EN_WR
  GNB_ENTRY_WR (GMMx3748_TYPE, GMMx3748_ADDRESS, 0x0000007f),    ///MC_ARB_HARSH_SAT0_RD
  GNB_ENTRY_WR (GMMx3750_TYPE, GMMx3750_ADDRESS, 0x25187a7e),    ///MC_ARB_HARSH_SAT1_RD
  GNB_ENTRY_WR (GMMx3754_TYPE, GMMx3754_ADDRESS, 0x25187a7e),    ///MC_ARB_HARSH_SAT1_WR
  GNB_ENTRY_WR (GMMx2108_TYPE, GMMx2108_ADDRESS, 0x00010143),    ///Mc_HUB_WDP_GBL1
  GNB_ENTRY_WR (GMMx27E4_TYPE, GMMx27E4_ADDRESS, 0x00017f00),    ///MC_ARB_GRUB_REALTIME_RD
  GNB_ENTRY_WR (GMMx3760_TYPE, GMMx3760_ADDRESS, 0x85554000),    ///MC_ARB_GRUB_PRIORITY1_RD
  GNB_ENTRY_WR (GMMx3768_TYPE, GMMx3768_ADDRESS, 0x242a8050),    ///MC_ARB_GRUB_PRIORITY2_RD
  GNB_ENTRY_WR (GMMx376C_TYPE, GMMx376C_ADDRESS, 0xA40AA001),    ///MC_ARB_GRUB_PRIORITY2_WR
  GNB_ENTRY_WR (GMMx3764_TYPE, GMMx3764_ADDRESS, 0x40280000),    ///MC_ARB_GRUB_PRIORITY1_WR
  GNB_ENTRY_WR (GMMx2738_TYPE, GMMx2738_ADDRESS, 0x80804000),    ///MC_ARB_GRUB_PROMOTE
  GNB_ENTRY_WR (GMMx2768_TYPE, GMMx2768_ADDRESS, 0x0000f0f0),    ///MC_ARB_MINCLKS
  GNB_ENTRY_WR (GMMx25E8_TYPE, GMMx25E8_ADDRESS, 0x00006448),    ///MC_CITF_REMREQ
  GNB_ENTRY_WR (GMMx2700_TYPE, GMMx2700_ADDRESS, 0x00880f78),    ///MC_ARB_RET_CREDITS2
  GNB_ENTRY_WR (GMMx2760_TYPE, GMMx2760_ADDRESS, 0x00007111),    ///MC_ARB_RAMCFG
  // 2.7 GRUB initialization
  GNB_ENTRY_WR (GMMx28DC_TYPE, GMMx28DC_ADDRESS, 0x01e8cf3f),    ///MC_GRUB_TX_CREDITS
  GNB_ENTRY_WR (GMMx28D4_TYPE, GMMx28D4_ADDRESS, 0x0066bf00),    ///MC_GRUB_PROBE_CREDITS
  GNB_ENTRY_WR (GMMx28CC_TYPE, GMMx28CC_ADDRESS, 0x00000a72),    ///MC_GRUB_PROBE_MAP
  //2.8 Remove blackout
  GNB_ENTRY_WR (GMMx20AC_TYPE, GMMx20AC_ADDRESS, 0x00000000),    ///MC_SHARED_BLACKOUT_CNTL

  //Enable FB writes
  GNB_ENTRY_RMW (GMMx5490_TYPE, GMMx5490_ADDRESS, GMMx5490_FB_WRITE_EN_MASK | GMMx5490_FB_READ_EN_MASK, (1 << GMMx5490_FB_READ_EN_OFFSET) | (1 << GMMx5490_FB_WRITE_EN_OFFSET)),


  // Watermark
  //Enable NB P-state
  GNB_ENTRY_WR (GMMx6CD8_TYPE, GMMx6CD8_ADDRESS, 0x7fff0111),
  GNB_ENTRY_WR (GMMx74D8_TYPE, GMMx74D8_ADDRESS, 0x7fff0111),
  GNB_ENTRY_WR (GMMx7CD8_TYPE, GMMx7CD8_ADDRESS, 0x7fff0111),
  //Enable NB C-state
  GNB_ENTRY_RMW (GMMx6CD4_TYPE, GMMx6CD4_ADDRESS, GMMx6CD4_STUTTER_ENABLE_MASK | GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK, (1 << GMMx6CD4_STUTTER_ENABLE_OFFSET) | (0x7fff << GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)),

  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GfxGmcClockGatingEnableCZ [] = {
  GNB_ENTRY_WR (GMMx20C0_TYPE, GMMx20C0_ADDRESS, 0x000c0c80),
  GNB_ENTRY_WR (GMMx2478_TYPE, GMMx2478_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx20B8_TYPE, GMMx20B8_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx20BC_TYPE, GMMx20BC_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx2648_TYPE, GMMx2648_ADDRESS, 0x00080400),
  GNB_ENTRY_WR (GMMx264C_TYPE, GMMx264C_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx2650_TYPE, GMMx2650_ADDRESS, 0x000c0400),
  GNB_ENTRY_WR (GMMx15C0_TYPE, GMMx15C0_ADDRESS, 0x000c0400),
  GNB_ENTRY_RMW (
    GMMx28D8_TYPE,
    GMMx28D8_ADDRESS,
    GMMx28D8_SCLK_CG_DISABLE_MASK,
    (0x0 << GMMx28D8_SCLK_CG_DISABLE_OFFSET)
    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GfxEnvInitTableCZ [] = {
  GNB_ENTRY_TERMINATE
};

