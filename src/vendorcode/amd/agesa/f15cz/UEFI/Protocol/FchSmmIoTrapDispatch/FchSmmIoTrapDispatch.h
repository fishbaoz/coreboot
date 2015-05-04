/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD FCH IO Trap dispacther Protocol
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD FCH UEFI Drivers
 * @e sub-project:  Protocols
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
/******************************************************************************
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
****************************************************************************/
#ifndef _FCHSMMIOTRAPDISPATCHPROTOCOL_H_
#define _FCHSMMIOTRAPDISPATCHPROTOCOL_H_


#define FCH_SMM_IOTRAP_DISPATCH_PROTOCOL_GUID \
        {0x13bd659b, 0xb4c6, 0x47da, 0x9b, 0x22, 0x11, 0x50, 0xd4, 0xf3, 0xb, 0xd3}


extern EFI_GUID gFchSmmIoTrapDispatchProtocolGuid;

struct _FCH_SMM_IO_TRAP_DISPATCH_PROTOCOL;
typedef struct _FCH_SMM_IO_TRAP_DISPATCH_PROTOCOL FCH_SMM_IO_TRAP_DISPATCH_PROTOCOL;

/// Enumeration for SMM I/O trap dispatch type
typedef enum {
  AmdWriteTrap,                           ///< Write trap
  AmdReadTrap,                            ///< Read trap
  AmdReadWriteTrap,                       ///< Read/Write trap
  AmdIoTrapTypeMaximum                    ///< Maximum value of I/O trap
} FCH_SMM_IO_TRAP_DISPATCH_TYPE;

/// SMM I/O trap register context
typedef struct {
  UINT16 Address;                         ///< 16 bit address
  UINT16 Length;                          ///< Length
  FCH_SMM_IO_TRAP_DISPATCH_TYPE Type;     ///< I/O trap dispatcher type
} FCH_SMM_IO_TRAP_REGISTER_CONTEXT;


///SMM I/O Trap Dispatch Context
typedef struct {
  UINT32 WriteData;                       ///< Write Data
} FCH_SMM_IO_TRAP_DISPATCH_CONTEXT;

typedef EFI_STATUS (EFIAPI *FCH_SMM_HANDLER_ENTRY_POINT) (
  IN       EFI_HANDLE  DispatchHandle,
  IN CONST VOID        *Context        OPTIONAL,
  IN OUT   VOID        *CommBuffer     OPTIONAL,
  IN OUT   UINTN       *CommBufferSize OPTIONAL
  );

typedef EFI_STATUS (EFIAPI *FCH_SMM_IO_TRAP_DISPATCH_REGISTER) (
  IN CONST FCH_SMM_IO_TRAP_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_HANDLER_ENTRY_POINT         DispatchFunction,
  IN OUT   FCH_SMM_IO_TRAP_REGISTER_CONTEXT    *RegisterContext,
  OUT      EFI_HANDLE                          *DispatchHandle
);

typedef EFI_STATUS (EFIAPI *FCH_SMM_IO_TRAP_DISPATCH_UNREGISTER) (
  IN       FCH_SMM_IO_TRAP_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                          DispatchHandle
);

/// FCH IO trap protocol
struct  _FCH_SMM_IO_TRAP_DISPATCH_PROTOCOL {
  FCH_SMM_IO_TRAP_DISPATCH_REGISTER    Register;
  FCH_SMM_IO_TRAP_DISPATCH_UNREGISTER  UnRegister;
};

#endif
