/* $NoKeywords:$ */
/**
 * @file
 *
 * Common Return routines.
 *
 * Routines which do nothing, returning a result (preferably some version of zero) which
 * is consistent with "do nothing" or "default".  Useful for function pointer tables.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Common
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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

#ifndef _COMMON_RETURNS_H_
#define _COMMON_RETURNS_H_


/**
* Return True
*
* @retval True    Default case, no special action
*/
BOOLEAN
CommonReturnTrue ( VOID );

/**
* Return False.
*
* @retval FALSE    Default case, no special action
*/
BOOLEAN
CommonReturnFalse ( VOID );

/**
 * Return (UINT8)zero.
 *
 *
 * @retval zero    None, or only case zero.
 */
UINT8
CommonReturnZero8 ( VOID );

/**
 * Return (UINT32)zero.
 *
 *
 * @retval zero    None, or only case zero.
 */
UINT32
CommonReturnZero32 ( VOID );

/**
 * Return (UINT64)zero.
 *
 *
 * @retval zero    None, or only case zero.
 */
UINT64
CommonReturnZero64 ( VOID );

/**
 * Return (UINT8)one.
 *
 *
 * @retval one     None, or only case one.
 */
UINT8
CommonReturnOne8 ( VOID );

/**
 * Return (UINT32)one.
 *
 *
 * @retval one     None, or only case one.
 */
UINT32
CommonReturnOne32 ( VOID );

/**
 * Return (UINT64)one.
 *
 *
 * @retval one     None, or only case one.
 */
UINT64
CommonReturnOne64 ( VOID );

/**
 * Return NULL
 *
 * @retval NULL    pointer to nothing
 */
VOID *
CommonReturnNULL ( VOID );

/**
* Return AGESA_SUCCESS.
*
* @retval AGESA_SUCCESS Success.
*/
AGESA_STATUS
CommonReturnAgesaSuccess ( VOID );

/**
* Return AGESA_ERROR.
*
* @retval AGESA_ERROR Error.
*/
AGESA_STATUS
CommonReturnAgesaError ( VOID );

/**
 * Do Nothing.
 *
 */
VOID
CommonVoid ( VOID );

/**
 * ASSERT if this routine is called.
 *
 */
VOID
CommonAssert ( VOID );

#endif // _COMMON_RETURNS_H_
