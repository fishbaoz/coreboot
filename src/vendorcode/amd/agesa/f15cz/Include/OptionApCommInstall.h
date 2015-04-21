/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: AP communication
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
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

#ifndef _OPTION_AP_COMM_INSTALL_H_
#define _OPTION_AP_COMM_INSTALL_H_

/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */
extern OPTION_AP_READ_REMOTE_CTRL_BYTE   ApReadRemoteControlByteApic;
extern OPTION_AP_WRITE_CTRL_BYTE         ApWriteControlByteApic;
extern OPTION_AP_READ_REMOTE_DATA_DWORD  ApReadRemoteDataDwordApic;
extern OPTION_AP_WRITE_DATA_DWORD        ApWriteDataDwordApic;
extern OPTION_AP_RUN_CODE_ON_SOCKET_CORE ApRunCodeOnSocketCoreApic;
extern OPTION_AP_ENTRY                   ApEntryApic;

#define AP_COMMUNICATE_METHOD            AP_COMM_APIC
#define AP_READ_REMOTE_CTRL_BYTE         ApReadRemoteControlByteApic
#define AP_WRITE_CTRL_BYTE               ApWriteControlByteApic
#define AP_READ_REMOTE_DATA_DWORD        ApReadRemoteDataDwordApic
#define AP_WRITE_DATA_DWORD              ApWriteDataDwordApic
#define AP_RUN_CODE_ON_SOCKET_CORE       ApRunCodeOnSocketCoreApic
#define AP_ENTRY                         ApEntryApic

#ifdef CFG_AP_COMM_METHOD
  #undef  AP_COMMUNICATE_METHOD
  #define AP_COMMUNICATE_METHOD          CFG_AP_COMM_METHOD

  #if CFG_AP_COMM_METHOD == AP_COMM_PCI
    extern OPTION_AP_READ_REMOTE_CTRL_BYTE   ApReadRemoteControlBytePci;
    extern OPTION_AP_WRITE_CTRL_BYTE         ApWriteControlBytePci;
    extern OPTION_AP_READ_REMOTE_DATA_DWORD  ApReadRemoteDataDwordPci;
    extern OPTION_AP_WRITE_DATA_DWORD        ApWriteDataDwordPci;
    extern OPTION_AP_RUN_CODE_ON_SOCKET_CORE ApRunCodeOnSocketCorePci;
    extern OPTION_AP_ENTRY                   ApEntryPci;

    #undef AP_READ_REMOTE_CTRL_BYTE
    #undef AP_WRITE_CTRL_BYTE
    #undef AP_READ_REMOTE_DATA_DWORD
    #undef AP_WRITE_DATA_DWORD
    #undef AP_RUN_CODE_ON_SOCKET_CORE
    #undef AP_ENTRY

    #define AP_READ_REMOTE_CTRL_BYTE         ApReadRemoteControlBytePci
    #define AP_WRITE_CTRL_BYTE               ApWriteControlBytePci
    #define AP_READ_REMOTE_DATA_DWORD        ApReadRemoteDataDwordPci
    #define AP_WRITE_DATA_DWORD              ApWriteDataDwordPci
    #define AP_RUN_CODE_ON_SOCKET_CORE       ApRunCodeOnSocketCorePci
    #define AP_ENTRY                         ApEntryPci
  #endif
#endif

/*  Declare the instance of the AP communication option configuration structure  */
OPTION_AP_COMMUNICATE_CONFIGURATION  OptionApCommConfiguration = {
  AP_COMMUNICATE_STRUCT_VERSION,
  AP_COMMUNICATE_METHOD,
  AP_READ_REMOTE_CTRL_BYTE,
  AP_WRITE_CTRL_BYTE,
  AP_READ_REMOTE_DATA_DWORD,
  AP_WRITE_DATA_DWORD,
  AP_RUN_CODE_ON_SOCKET_CORE,
  AP_ENTRY
};

#endif  // _OPTION_AP_COMM_INSTALL_H_
