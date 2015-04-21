/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD AP Communication option API.
 *
 * Contains structures and values used to control the AP communication option code.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  OPTION
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
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
 ******************************************************************************
 */

#ifndef _OPTION_AP_COMM_H_
#define _OPTION_AP_COMM_H_

/*----------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define AP_COMM_APIC 0
#define AP_COMM_PCI  1

/*----------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Reads the 'control byte' on the designated remote core.
 *
 * This function will read the current contents of the control byte
 * on the designated core.
 *
 * @param[in]      Socket        Socket number of the target core
 * @param[in]      Core          Core number of the target core
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 * @return         The current value of the remote cores control byte
 *
 */
typedef UINT8 OPTION_AP_READ_REMOTE_CTRL_BYTE (
  IN       UINT8 Socket,
  IN       UINT8 Core,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*---------------------------------------------------------------------------------------*/
/**
 * Writes the 'control byte' on the executing core.
 *
 * This function writes data to a local register used in inter-
 * processor communication.
 *
 * @param[in]      Value         Value to write
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 */
typedef VOID OPTION_AP_WRITE_CTRL_BYTE (
  IN       UINT8 Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*---------------------------------------------------------------------------------------*/
/**
 * Reads the 'data dword' on the designated remote core.
 *
 * This function will read the current contents of the data dword
 * on the designated core.
 *
 * @param[in]      Socket        Socket number of the target core
 * @param[in]      Core          Core number of the target core
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 * @return         The current value of the remote cores control byte
 *
 */
typedef UINT32 OPTION_AP_READ_REMOTE_DATA_DWORD (
  IN       UINT8 Socket,
  IN       UINT8 Core,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*---------------------------------------------------------------------------------------*/
/**
 * Writes the 'data dword' on the executing core.
 *
 * This function writes data to a local register used in inter-
 * processor communication.
 *
 * @param[in]      Value         Value to write
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 */
typedef VOID OPTION_AP_WRITE_DATA_DWORD (
  IN       UINT32 Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*---------------------------------------------------------------------------------------*/
/**
 * Runs the given task on the specified local core.
 *
 * This function is used to invoke an AP to run a specified AGESA
 * procedure.  It can only be called by cores that have subordinate
 * APs -- the BSC at POST, or any socket-relative core 0s at Early.
 *
 * @param[in]      Socket        Socket number of the target core
 * @param[in]      Core          Core number of the target core
 * @param[in]      TaskPtr       Function descriptor
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 * @return         Return value of the task that the AP core ran,
 *                 or zero if the task was VOID.
 *
 */
typedef UINT32 OPTION_AP_RUN_CODE_ON_SOCKET_CORE (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  IN       AP_TASK *TaskPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );


/*---------------------------------------------------------------------------------------*/
/**
 * Main entry point for all APs in the system.
 *
 * This routine puts the AP cores in an infinite loop in which the cores
 * will poll their masters, waiting to be told to perform a task.  At early,
 * all socket-relative core zeros will receive their tasks from the BSC.
 * All others will receive their tasks from the core zero of their local
 * processor.  At the end of AmdInitEarly, all cores will switch to receiving
 * their tasks from the BSC.
 *
 * @param[in]     StdHeader       Handle to config for library and services.
 * @param[in]     CpuParams       Cpu params pointer.
 *
 */
typedef VOID OPTION_AP_ENTRY (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       VOID              *CpuParams
  );

#define AP_COMMUNICATE_STRUCT_VERSION  0x01

/**
 * Provide build configuration of cpu multi-socket or single socket support.
 *
 */
typedef struct  {
  UINT16                             ApCommVersion;                ///< Table version
  UINT16                             ApCommMethod;                 ///< Data: specify the AP communication method
  OPTION_AP_READ_REMOTE_CTRL_BYTE   *ApReadRemoteCtrlByte;         ///< Method: Reads the 'control byte' on the designated remote core
  OPTION_AP_WRITE_CTRL_BYTE         *ApWriteCtrlByte;              ///< Method: Writes the 'control byte' on the executing core
  OPTION_AP_READ_REMOTE_DATA_DWORD  *ApReadRemoteDataDword;        ///< Method: Reads the 'data dword' on the designated remote core
  OPTION_AP_WRITE_DATA_DWORD        *ApWriteDataDword;             ///< Method: Writes the 'data dword' on the executing core
  OPTION_AP_RUN_CODE_ON_SOCKET_CORE *ApRunCodeOnSocketCore;        ///< Method: Runs the given task on the specified core
  OPTION_AP_ENTRY                   *ApEntry;                      ///< Method: Ap Entry
} OPTION_AP_COMMUNICATE_CONFIGURATION;

/*----------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *----------------------------------------------------------------------------------------
 */


#endif  // _OPTION_AP_COMM_H_
