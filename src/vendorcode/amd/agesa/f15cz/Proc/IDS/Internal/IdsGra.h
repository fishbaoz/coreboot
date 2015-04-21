/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Integrated Debug library Routines
 *
 * Contains AMD AGESA debug macros and library functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 */
/*****************************************************************************
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
#ifndef _IDS_GRA_H_
#define _IDS_GRA_H_
///GRA Group
///Enable or disable GRA Group
typedef enum {
  IdsNvMiscGRAUsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRAUsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRAUsrCtl;

///GRA Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRARegTypePCI = 0,///<PCI
  IdsNvMiscGRARegTypeMSR = 1,///<MSR
  IdsNvMiscGRARegTypeMMIO = 2,///<MMIO
  IdsNvMiscGRARegTypePciIndirect = 3,///<PCI indirect
  IdsNvMiscGRARegTypeGmmx = 4,///<Gmmx
} IdsNvMiscGRARegType;

///GRA1 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA1TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA1TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA1TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA1TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA1TimePoint;

///GRA1 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA1GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA1GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA1GenRegNode;

///GRA Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRABusMin 0
#define IdsNvMiscGRABusMax 255

///GRA Device
///Applied to all devices or a single device
#define IdsNvMiscGRADevMin 0
#define IdsNvMiscGRADevMax 31

///GRA Function
///This is to select the function of register to set
#define IdsNvMiscGRAFunMin 0
#define IdsNvMiscGRAFunMax 7

///GRA Offset
///Offset
#define IdsNvMiscGRAOffsetMin 0
#define IdsNvMiscGRAOffsetMax 0xFFF

///GRA PCI Indirect Register Offset [31:16]
///Offset
#define IdsNvMiscGRAPciIndirectOffsetHighMin 0
#define IdsNvMiscGRAPciIndirectOffsetHighMax 0xFFFF

///GRA PCI Indirect Register Offset [15:0]
///Offset
#define IdsNvMiscGRAPciIndirectOffsetLowMin 0
#define IdsNvMiscGRAPciIndirectOffsetLowMax 0xFFFF

///GRA1 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRAPciIndirectWriteEn_Bit0 = 0,///< bit 0
  IdsNvMiscGRAPciIndirectWriteEn_Bit1,///< bit 1
  IdsNvMiscGRAPciIndirectWriteEn_Bit2,///< bit 2
  IdsNvMiscGRAPciIndirectWriteEn_Bit3,///< bit 3
  IdsNvMiscGRAPciIndirectWriteEn_Bit4,///< bit 4
  IdsNvMiscGRAPciIndirectWriteEn_Bit5,///< bit 5
  IdsNvMiscGRAPciIndirectWriteEn_Bit6,///< bit 6
  IdsNvMiscGRAPciIndirectWriteEn_Bit7,///< bit 7
  IdsNvMiscGRAPciIndirectWriteEn_Bit8,///< bit 8
  IdsNvMiscGRAPciIndirectWriteEn_Bit9,///< bit 9
  IdsNvMiscGRAPciIndirectWriteEn_Bit10,///< bit 10
  IdsNvMiscGRAPciIndirectWriteEn_Bit11,///< bit 11
  IdsNvMiscGRAPciIndirectWriteEn_Bit12,///< bit 12
  IdsNvMiscGRAPciIndirectWriteEn_Bit13,///< bit 13
  IdsNvMiscGRAPciIndirectWriteEn_Bit14,///< bit 14
  IdsNvMiscGRAPciIndirectWriteEn_Bit15,///< bit 15
  IdsNvMiscGRAPciIndirectWriteEn_Bit16,///< bit 16
  IdsNvMiscGRAPciIndirectWriteEn_Bit17,///< bit 17
  IdsNvMiscGRAPciIndirectWriteEn_Bit18,///< bit 18
  IdsNvMiscGRAPciIndirectWriteEn_Bit19,///< bit 19
  IdsNvMiscGRAPciIndirectWriteEn_Bit20,///< bit 20
  IdsNvMiscGRAPciIndirectWriteEn_Bit21,///< bit 21
  IdsNvMiscGRAPciIndirectWriteEn_Bit22,///< bit 22
  IdsNvMiscGRAPciIndirectWriteEn_Bit23,///< bit 23
  IdsNvMiscGRAPciIndirectWriteEn_Bit24,///< bit 24
  IdsNvMiscGRAPciIndirectWriteEn_Bit25,///< bit 25
  IdsNvMiscGRAPciIndirectWriteEn_Bit26,///< bit 26
  IdsNvMiscGRAPciIndirectWriteEn_Bit27,///< bit 27
  IdsNvMiscGRAPciIndirectWriteEn_Bit28,///< bit 28
  IdsNvMiscGRAPciIndirectWriteEn_Bit29,///< bit 29
  IdsNvMiscGRAPciIndirectWriteEn_Bit30,///< bit 30
  IdsNvMiscGRAPciIndirectWriteEn_Bit31,///< bit 31
  IdsNvMiscGRAPciIndirectWriteEn_Auto = 0xFF,///<Auto No WE bit
} IdsNvMiscGRAPciIndirectWriteEn;

///GRA Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRACoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRACoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRACores;

///GRA Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRANodeMin 0
#define IdsNvMiscGRANodeMax 255

///GRA Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRACoreMin 0
#define IdsNvMiscGRACoreMax 127

///GRA MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRAMsrHighMin 0
#define IdsNvMiscGRAMsrHighMax 0xFFFF

///GRA MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRAMsrLowMin 0
#define IdsNvMiscGRAMsrLowMax 0xFFFF

///GRA Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRAAndHighestMin 0
#define IdsNvMiscGRAAndHighestMax 0xFFFF

///GRA Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRAAndHigherMin 0
#define IdsNvMiscGRAAndHigherMax 0xFFFF

///GRA Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRAAndLowerMin 0
#define IdsNvMiscGRAAndLowerMax 0xFFFF

///GRA Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRAAndLowestMin 0
#define IdsNvMiscGRAAndLowestMax 0xFFFF

///GRA Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRAOrHighestMin 0
#define IdsNvMiscGRAOrHighestMax 0xFFFF

///GRA Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRAOrHigherMin 0
#define IdsNvMiscGRAOrHigherMax 0xFFFF

///GRA Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRAOrLowerMin 0
#define IdsNvMiscGRAOrLowerMax 0xFFFF

///GRA Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRAOrLowestMin 0
#define IdsNvMiscGRAOrLowestMax 0xFFFF

BOOLEAN
IdsGetGraBinaryInfo (
  IN OUT   UINT64 *IdsGraHeapBase,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/// Route Table for GRA When to IDS Option
typedef struct _GRA_WHEN_TO_IDS_OPTION {
  IDS_STATUS  GraWhen;          ///< GRA when option
  AGESA_IDS_OPTION IdsOption;   ///< IDS option
} GRA_WHEN_TO_IDS_OPTION;
#endif//_IDS_GRA_H_
