/* $NoKeywords:$ */
/**
 * @file
 *
 * IdsBvm.h
 *
 * Contains AMD AGESA Internal Integrated Debug Macros
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project: (Proc/Ids/Internal)
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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

#ifndef _IDS_BVM_H_
#define _IDS_BVM_H_
///BVM reg type

typedef enum {
  BVM_REG_TYPE_MSR = 0x1,       ///< 01 BVM MSR Type
  BVM_REG_TYPE_CPU_NB,          ///< 02 BVM Nb function Type
  BVM_REG_TYPE_DCT_ADD_DATA,    ///< 03 BVM DCT additional data Type
  BVM_REG_TYPE_PCI,             ///< 04 BVM common PCI Type
  BVM_REG_TYPE_IO,              ///< 05 BVM IO Type
  BVM_REG_TYPE_ISA,             ///< 06 BVM IO space Type
  BVM_REG_TYPE_MEM,             ///< 07 BVM MMIO Type
  BVM_REG_TYPE_DCT_EXTRA_DATA,  ///< 08 BVM DCT ectra Type
  BVM_REG_TYPE_DR,              ///< 09 BVM Debug register Type
  BVM_REG_TYPE_MEM_NB,          ///< 10 BVM DCT PCI Type
  BVM_REG_TYPE_PCI_INDIRECT,    ///< 11 BVM PCI indirect Type
  BVM_REG_TYPE_GMMX,            ///< 12 BVM GMMX Type
  BVM_MAX_SUPPORT_REG,          ///< Max BVM register type
} BVM_REG_TYPE;

#define  BVM_MAX_SUPPORT_REG BVM_REG_TYPE_MEM_NB

#define  BVM_CURRENT_MAJOR_VERSION 3
#define  BVM_CURRENT_MINOR_VERSION 4
#define  BVM_CURRENT_VERSION ((BVM_CURRENT_MAJOR_VERSION << 8) + BVM_CURRENT_MINOR_VERSION)

#define BVM_FLASH_SIZE  0x800000ul

#define IDS_TIMING_BIT(timing) ((UINT64) 1 << (timing))
/// Data Structure to define BVM Engine
typedef  struct {
  UINT64  BvmEngineSignature;                           ///< $BVM_ENG indicate the signature of BVM engine
  UINT8   BvmBinaryMajorVersion;                        ///< BVM bin major version
  UINT8   BvmBinaryMinorVersion;                        ///< BVM bin minor version
} BVM_ENGINE_STRUCT;

#pragma pack(push, 1)

///Structure for BVM timing
typedef struct _BVM_REG_TIMING_OPTION {
  UINT8 bvmreg;                 ///< BVM reg
  UINT64 bvmregtiming;          ///< Timing of BVM reg
} BVM_REG_TIMING_OPTION;

///Structure define for BVM image
typedef struct _BVM_IMAGE_HEADER {
  UINT32  Signature;          ///<  00 Signature for identify "$BVM"
  UINT8   ImageTableOffset;   ///<  04 Start of register table
  UINT32  UnitNumber;         ///<  05 The number of register unit
  UINT8   MajorVersion;       ///<  09 Major version of BVM image
  UINT8   MinorVersion;       ///<  10 Minor version of BVM image
  UINT8   Checksum;           ///<  11 Checksum include header, the sum of whole image must be 0
  UINT32  RequestId;          ///<  12 Id for BVM request
  UINT8   Requester[32];      ///<  16 BVM requester
  UINT8   RequsetTime[20];    ///<  48 BVM request time
  UINT16  RomSize;            ///<  68 Rom Size
  UINT16  ExtendTableOffset;  ///<  70 Point to extend Table offset
  UINT8   Reserved[56];       ///<  72 For future use
} BVM_IMAGE_HEADER;

#define BVM_EXTEND_MODULE_TYPE_UCODE    'DOCU'    ///< Type for ucode patch
#define BVM_EXTEND_MODULE_SIGNATURE     'TXE_'    ///< Signature for extended module
///Structure define extend Module header
typedef struct _BVM_EXTEND_COMMON_HEADER {
  UINT32   ExtSig;             ///<  00 Signature "_EXT"
  UINT32   Type;               ///<  04 Extend Module type
  UINT16   Size;               ///<  08 Size include Header
  UINT8    Reserved[6];       ///<  10 Reserved for future use
} BVM_EXTEND_COMMON_HEADER;

#define BVM_UCODE_TYPE_2K               'K2'    ///< Type for 2k
#define BVM_UCODE_TYPE_4K               'K4'    ///< Type for 4k
#define BVM_UCODE_TYPE_VS               'SV'    ///< Type for 4k

///Structure define for Ucode
typedef struct _BVM_EXTEND_UCODE_HEADER {
  BVM_EXTEND_COMMON_HEADER CommonExtHeader;   ///< Common Header
  UINT16                    UcodeSubType;      ///< Ucode SubType
  UINT8                     Reserved[14];      ///< reserved
} BVM_EXTEND_UCODE_HEADER;

///Structure define for Common register
typedef struct _BVM_REG_COMMON {
  UINT8   type;         ///< Register type
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
} BVM_REG_COMMON;

///Structure define for MSR register
typedef struct _BVM_REG_MSR {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT32  msraddr;      ///< Port address
  UINT8   socketid;     ///< Socket id,0xff for all sockets
  UINT8   coreid;       ///< Core id,0xff for all cores
  UINT64  AndMask;      ///< And Mask
  UINT64  OrMask;      ///< Or Mask
} BVM_REG_MSR;

///Structure define for CPU NB register
typedef struct _BVM_REG_CPU_NB {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT8   nodeid;       ///< Node id,0xff for all nodes
  UINT8   function;     ///< CPU NB function address
  UINT16  regoff;       ///< CPU NB register offset
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_CPU_NB;

///Structure define for MEM PCI register
typedef struct _BVM_REG_MEM_NB {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT8   Mstate;       ///< Specify M state, 0x7f for all Mstate, 0x0 for Mstate 0, 0x1 for Mstate 1, If bit 7 == 1 ignore
  UINT8   Moduleid;       ///< Node id,0xff for all nodes
  UINT8   dctnumber;    ///< DCT number 0,1, 0xff for all dct
  UINT8   PciFunc;         ///< PCI function
  UINT16  PciRegOff;       ///< PCI register offset
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_MEM_NB;

///Structure define for D18F2x[1,0]F0 DRAM Controller Extra Data Offset Register
typedef struct _BVM_REG_DCT_EXTRA_DATA {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT8   Mstate;       ///< Specify M state, 0x7f for all Mstate, 0x0 for Mstate 0, 0x1 for Mstate 1, If bit 7 == 1 ignore
  UINT8   nodeid;       ///< Node id,0xff for all nodes
  UINT8   dctnumber;    ///< DCT number 0,1, 0xff for all dct
  UINT32  regoff;       ///< CPU DCT additional data register offset
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_DCT_EXTRA_DATA;

///Structure define for DRAM controller register additional data F2x[1,0]9C
typedef struct _BVM_REG_DCT_ADD_DATA {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT8   Mstate;       ///< Specify M state, 0x7f for all Mstate, 0x0 for Mstate 0, 0x1 for Mstate 1, If bit 7 == 1 ignore
  UINT8   nodeid;       ///< Node id,0xff for all nodes
  UINT8   dctnumber;    ///< DCT number 0,1, 0xff for all dct
  UINT32  regoff;       ///< CPU DCT additional data register offset
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_DCT_ADD_DATA;

///Structure define for PCI register
typedef struct _BVM_REG_PCI {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT8   bus;          ///< bus
  UINT8   dev;          ///< dev
  UINT8   func;         ///< function
  UINT16  regoff;       ///< PCI register offset
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_PCI;


///Structure define for IO register
typedef struct _BVM_REG_IO {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT8   accesstype;   ///< 8bits, 16bits, 32 bits
  UINT16  ioport;       ///< port address
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_IO;

///Structure define for ISA register
typedef struct _BVM_REG_ISA {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT8   accesstype;   ///< 8bits, 16bits, 32bits
  UINT16  isaindex;     ///< isa index port
  UINT16  isadata;      ///< isa data port
  UINT32  isaoff;       ///< offset
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_ISA;

///Structure define for MMIO register
typedef struct _BVM_REG_MEM {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT32  memaddr;      ///< mem address
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;      ///< Or Mask
} BVM_REG_MEM;

///Structure define for debug register
typedef struct _BVM_REG_DR {
  UINT8   type;         ///< Register type for MSR
  UINT64  csttiming;    ///< Customed hacking timing
  UINT32  Reserved1;     ///< reserved for future use
  UINT16  Reserved2;     ///< reserved for future use
  UINT8   socketid;     ///< Socket id,0xff for all sockets
  UINT8   coreid;       ///< Core id,0xff for all cores
  UINT8   drindex;      ///< Debug register index from 0-7
  UINT64  AndMask;      ///< And Mask
  UINT64  OrMask;      ///< Or Mask
} BVM_REG_DR;

///Structure define for PCI indirect register
typedef struct _BVM_REG_PCI_INDIRECT {
  UINT8   type;               ///< Register type for MSR
  UINT64  csttiming;          ///< Customed hacking timing
  UINT64  Reserved;           ///< reserved for future use
  UINT8   bus;                ///< bus
  UINT8   dev;                ///< dev
  UINT8   func;               ///< function
  UINT16  DataPortOff;        ///< PCI register offset of indirect reigster data port
  UINT32  IndirectRegOff;     ///< PCI indirect register offset
  UINT32  WriteEnBit;         ///< Write Enable bit
  UINT32  AndMask;            ///< And Mask
  UINT32  OrMask;             ///< Or Mask
} BVM_REG_PCI_INDIRECT;


///Structure define for GMMX register
typedef struct _BVM_REG_GMMX {
  UINT8   type;         ///< Register type for GMMX
  UINT64  csttiming;    ///< Customed hacking timing
  UINT64  Reserved;     ///< reserved for future use
  UINT32  Offset;       ///< GMMX offset
  UINT32  AndMask;      ///< And Mask
  UINT32  OrMask;       ///< Or Mask
} BVM_REG_GMMX;

#define BVM_ROM_SIZE  2*1024
#pragma pack(pop)

#endif //_IDS_BVM_H_
